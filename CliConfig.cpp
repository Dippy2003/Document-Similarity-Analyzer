#include "CliConfig.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

namespace {
    bool parseWeightTriple(const char* value, Similarity::MetricWeights& weights) {
        double j = 0.0;
        double o = 0.0;
        double r = 0.0;
        if (std::sscanf(value, "%lf,%lf,%lf", &j, &o, &r) != 3) {
            return false;
        }
        if (j < 0.0 || o < 0.0 || r < 0.0) {
            return false;
        }
        weights.jaccard = j;
        weights.ordered = o;
        weights.reverse = r;
        return true;
    }
}

void printVersion() {
    std::cout << "Document Similarity Analyzer 1.2.0\n";
}

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " [options] [file1 file2]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help              Show this help message\n";
    std::cout << "  -v, --version           Show version information\n";
    std::cout << "  --weights j,o,r         Metric weights (default 0.6,0.2,0.2)\n";
    std::cout << "  --keep-stopwords        Do not remove common stopwords\n";
    std::cout << "  --top N                 Max top matching words (default 15)\n";
    std::cout << "  --json <path>           Write JSON report to file\n";
    std::cout << "  --csv <path>            Write CSV report to file\n";
    std::cout << "  --cosine-only           Print only the cosine similarity score\n";
    std::cout << "\nIf two file paths are given, compares them without the interactive menu.\n";
}

bool parseCli(int argc, char* argv[], CliConfig& config) {
    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (std::strcmp(arg, "-h") == 0 || std::strcmp(arg, "--help") == 0) {
            config.showHelp = true;
            return false;
        }
        if (std::strcmp(arg, "-v") == 0 || std::strcmp(arg, "--version") == 0) {
            config.showVersion = true;
            return false;
        }
        if (std::strcmp(arg, "--keep-stopwords") == 0) {
            config.keepStopwords = true;
            continue;
        }
        if (std::strcmp(arg, "--cosine-only") == 0) {
            config.cosineOnly = true;
            continue;
        }
        if (std::strcmp(arg, "--weights") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --weights\n";
                return false;
            }
            if (!parseWeightTriple(argv[++i], config.weights)) {
                std::cerr << "Invalid --weights format. Use three comma-separated numbers.\n";
                return false;
            }
            continue;
        }
        if (std::strcmp(arg, "--top") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --top\n";
                return false;
            }
            config.topMatchLimit = std::atoi(argv[++i]);
            continue;
        }
        if (std::strcmp(arg, "--json") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Missing path for --json\n";
                return false;
            }
            config.jsonOut = argv[++i];
            continue;
        }
        if (std::strcmp(arg, "--csv") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Missing path for --csv\n";
                return false;
            }
            config.csvOut = argv[++i];
            continue;
        }
        if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            return false;
        }
        if (config.file1.empty()) {
            config.file1 = arg;
        } else if (config.file2.empty()) {
            config.file2 = arg;
        } else {
            std::cerr << "Unexpected argument: " << arg << "\n";
            return false;
        }
    }

    if (!config.file1.empty() && !config.file2.empty()) {
        config.runInteractive = false;
    } else if (!config.file1.empty() || !config.file2.empty()) {
        std::cerr << "Provide both file paths or use interactive mode.\n";
        return false;
    }

    return true;
}
