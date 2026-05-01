#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cassert>

#include "TextUtils.h"
#include "Similarity.h"
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

    void runBasicChecks() {
        // tokenizeWords
        std::vector<std::string> t = TextUtils::tokenizeWords("The quick brown fox", true);
        assert(!t.empty());
        assert(t[0] == "quick");

        // BST insert/search
        BST bst;
        bst.insert("apple");
        bst.insert("banana");
        bst.insert("apple");
        assert(bst.search("apple"));
        assert(bst.search("banana"));
        assert(!bst.search("cherry"));

        // Queue order
        Queue q;
        q.enqueue("first");
        q.enqueue("second");
        std::vector<std::string> qv = q.toVector();
        assert(qv.size() == 2);
        assert(qv[0] == "first");
        assert(qv[1] == "second");
    }

    void printReport(const Similarity::SimilarityReport& report) {
        std::cout << "\n========================================\n";
        std::cout << "        Similarity Report\n";
        std::cout << "========================================\n\n";

        std::cout << "Total words in doc1: " << report.totalWords1 << "\n";
        std::cout << "Total words in doc2: " << report.totalWords2 << "\n";
        std::cout << "Unique words in doc1: " << report.uniqueWords1 << "\n";
        std::cout << "Unique words in doc2: " << report.uniqueWords2 << "\n";
        std::cout << "Intersection (common unique words): " << report.intersectionSize << "\n\n";

        std::cout << "Jaccard Similarity (unique words): " << report.jaccardPercent << "%\n";
        std::cout << "Ordered Similarity (by position):  " << report.orderedPercent << "%\n";
        std::cout << "Reverse Similarity:                " << report.reversePercent << "%\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Final Similarity Score:            " << report.finalPercent << "%\n\n";

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
        std::cout << "========================================\n";
    }

    void runDemoMode() {
        const std::string sample1 =
            "Plagiarism detection compares documents to measure how similar they are. "
            "It analyzes words, order, and structure to find overlapping content.";

        const std::string sample2 =
            "A digital plagiarism checker measures similarity between texts. "
            "It looks at shared words, sentence order, and structure to detect overlap.";

        runBasicChecks();

        std::string textArr[2];
        textArr[0] = sample1;
        textArr[1] = sample2;

        Similarity::SimilarityReport report = Similarity::analyze(textArr[0], textArr[1]);
        printReport(report);
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << " Digital Plagiarism Similarity Checker\n";
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
        runDemoMode();
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

    std::string textArr[2];
    textArr[0] = text1;
    textArr[1] = text2;

    Similarity::SimilarityReport report = Similarity::analyze(textArr[0], textArr[1]);
    printReport(report);

    return 0;
}

