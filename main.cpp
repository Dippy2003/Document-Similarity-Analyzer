#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cassert>

#include "TextUtils.h"
#include "Similarity.h"
#include "ReportExport.h"
#include "CliConfig.h"
#include "BST.h"
#include "Queue.h"

namespace {
    std::string readTextFromStdin(const std::string& prompt) {
        std::cout << prompt << " (end with a blank line):\n";
        std::string text;
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                break;
            }
            if (!text.empty()) {
                text += '\n';
            }
            text += line;
        }
        return text;
    }

    std::string readTextFromFile(const std::string& path) {
        std::ifstream in(path.c_str());
        if (!in) {
            std::cerr << "Failed to open file: " << path << "\n";
            return std::string();
        }
        std::string content;
        std::string line;
        while (std::getline(in, line)) {
            if (!content.empty()) {
                content += '\n';
            }
            content += line;
        }
        return content;
    }

    Similarity::AnalyzeOptions optionsFromCli(const CliConfig& cli) {
        Similarity::AnalyzeOptions options;
        options.removeStopwords = !cli.keepStopwords;
        options.weights = cli.weights;
        options.topMatchLimit = cli.topMatchLimit;
        return options;
    }

    void runBasicChecks() {
        std::vector<std::string> t = TextUtils::tokenizeWords("The quick brown fox", true);
        assert(!t.empty());
        assert(t[0] == "quick");

        BST bst;
        bst.insert("apple");
        bst.insert("banana");
        bst.insert("apple");
        assert(bst.search("apple"));
        assert(bst.search("banana"));
        assert(!bst.search("cherry"));

        Queue q;
        q.enqueue("first");
        q.enqueue("second");
        std::vector<std::string> qv = q.toVector();
        assert(qv.size() == 2);
        assert(qv[0] == "first");
        assert(qv[1] == "second");

        assert(TextUtils::countAlphanumericChars("Hello, World! 123") == 13);
        std::vector<std::string> lenTest = {"aa", "bbb"};
        assert(TextUtils::averageTokenLength(lenTest) == 2.5);
        assert(TextUtils::countLines("a\nb\nc") == 3);
    }

    void printReport(const Similarity::SimilarityReport& report,
                     std::size_t lines1,
                     std::size_t lines2,
                     const CliConfig& cli) {
        std::cout << "\n========================================\n";
        std::cout << "   Document Similarity Report\n";
        std::cout << "========================================\n\n";

        std::cout << "Lines in doc1: " << lines1 << "\n";
        std::cout << "Lines in doc2: " << lines2 << "\n";
        std::cout << "Total words in doc1: " << report.totalWords1 << "\n";
        std::cout << "Total words in doc2: " << report.totalWords2 << "\n";
        std::cout << "Alphanumeric characters (doc1): " << report.alphanumericChars1 << "\n";
        std::cout << "Alphanumeric characters (doc2): " << report.alphanumericChars2 << "\n";
        std::cout << "Avg word length (doc1): " << report.avgWordLength1 << "\n";
        std::cout << "Avg word length (doc2): " << report.avgWordLength2 << "\n";
        std::cout << "Unique words in doc1: " << report.uniqueWords1 << "\n";
        std::cout << "Unique words in doc2: " << report.uniqueWords2 << "\n";
        std::cout << "Intersection (common unique words): " << report.intersectionSize << "\n\n";

        if (!cli.cosineOnly) {
            std::cout << "Jaccard Similarity (unique words): " << report.jaccardPercent << "%\n";
            std::cout << "Ordered Similarity (by position):  " << report.orderedPercent << "%\n";
            std::cout << "Reverse Similarity:                " << report.reversePercent << "%\n";
        }
        std::cout << "Cosine Similarity (frequency):     " << report.cosinePercent << "%\n";
        if (!cli.cosineOnly) {
            std::cout << "----------------------------------------\n";
            std::cout << "Final Similarity Score:            " << report.finalPercent << "%\n";
            std::cout << "Similarity band:                   "
                      << Similarity::similarityBand(report.finalPercent) << "\n\n";
        } else {
            std::cout << "----------------------------------------\n\n";
        }

        if (!cli.cosineOnly) {
            std::cout << "Top Matching Words (word : count1 + count2 = total)\n";
            std::cout << "----------------------------------------\n";
            if (report.topMatches.empty()) {
                std::cout << "No common words found.\n";
            } else {
                for (const auto& mw : report.topMatches) {
                    std::cout << mw.word << " : "
                              << mw.count1 << " + " << mw.count2
                              << " = " << mw.total << "\n";
                }
            }
        }
        std::cout << "========================================\n";
    }

    bool writeOptionalExports(const CliConfig& cli,
                            const Similarity::SimilarityReport& report) {
        if (!cli.jsonOut.empty()) {
            if (!ReportExport::writeJsonFile(cli.jsonOut, report)) {
                std::cerr << "Failed to write JSON report: " << cli.jsonOut << "\n";
                return false;
            }
            std::cout << "JSON report saved to " << cli.jsonOut << "\n";
        }
        if (!cli.csvOut.empty()) {
            if (!ReportExport::writeCsvFile(cli.csvOut, report)) {
                std::cerr << "Failed to write CSV report: " << cli.csvOut << "\n";
                return false;
            }
            std::cout << "CSV report saved to " << cli.csvOut << "\n";
        }
        return true;
    }

    void analyzeAndReport(const std::string& text1,
                          const std::string& text2,
                          const CliConfig& cli) {
        Similarity::AnalyzeOptions options = optionsFromCli(cli);
        Similarity::SimilarityReport report =
            Similarity::analyze(text1, text2, options);
        printReport(report, TextUtils::countLines(text1), TextUtils::countLines(text2), cli);
        writeOptionalExports(cli, report);
    }

    void runDemoMode(const CliConfig& cli) {
        const std::string sample1 =
            "Plagiarism detection compares documents to measure how similar they are. "
            "It analyzes words, order, and structure to find overlapping content.";

        const std::string sample2 =
            "A digital plagiarism checker measures similarity between texts. "
            "It looks at shared words, sentence order, and structure to detect overlap.";

        runBasicChecks();
        analyzeAndReport(sample1, sample2, cli);
    }

    int runInteractive(const CliConfig& cli) {
        std::cout << "========================================\n";
        std::cout << "  Document Similarity Analyzer\n";
        std::cout << "========================================\n\n";

        int choice = 0;
        while (choice < 1 || choice > 3) {
            std::cout << "Choose mode:\n";
            std::cout << "1) Enter texts manually\n";
            std::cout << "2) Load texts from files\n";
            std::cout << "3) Demo Mode (sample paragraphs)\n";
            std::cout << "Enter choice (1-3): ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                choice = 0;
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (choice == 3) {
            runDemoMode(cli);
            return 0;
        }

        std::string text1;
        std::string text2;

        if (choice == 1) {
            text1 = readTextFromStdin("Enter first text");
            text2 = readTextFromStdin("Enter second text");
        } else {
            std::string path1;
            std::string path2;
            std::cout << "Enter path for first text file: ";
            std::getline(std::cin, path1);
            std::cout << "Enter path for second text file: ";
            std::getline(std::cin, path2);

            text1 = readTextFromFile(path1);
            text2 = readTextFromFile(path2);
        }

        analyzeAndReport(text1, text2, cli);
        return 0;
    }
}

int main(int argc, char* argv[]) {
    CliConfig cli;
    if (!parseCli(argc, argv, cli)) {
        if (cli.showHelp) {
            printUsage(argv[0]);
            return 0;
        }
        if (cli.showVersion) {
            printVersion();
            return 0;
        }
        return 1;
    }

    if (!cli.runInteractive) {
        std::string text1 = readTextFromFile(cli.file1);
        std::string text2 = readTextFromFile(cli.file2);
        if (text1.empty() || text2.empty()) {
            return 1;
        }
        analyzeAndReport(text1, text2, cli);
        return 0;
    }

    return runInteractive(cli);
}
