#ifndef CLICONFIG_H
#define CLICONFIG_H

#include <string>

#include "Similarity.h"

struct CliConfig {
    bool showHelp = false;
    bool showVersion = false;
    bool runInteractive = true;
    bool keepStopwords = false;
    bool cosineOnly = false;

    std::string file1;
    std::string file2;
    std::string jsonOut;
    std::string csvOut;

    Similarity::MetricWeights weights{};
    int topMatchLimit = 15;
};

bool parseCli(int argc, char* argv[], CliConfig& config);

void printUsage(const char* program);
void printVersion();

#endif
