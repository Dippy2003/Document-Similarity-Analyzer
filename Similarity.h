#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <string>
#include <vector>

namespace Similarity {
    struct MetricWeights {
        double jaccard = 0.6;
        double ordered = 0.2;
        double reverse = 0.2;
    };

    struct AnalyzeOptions {
        bool removeStopwords = true;
        MetricWeights weights{};
        int topMatchLimit = 15;
    };

    struct MatchingWord {
        std::string word;
        int count1;
        int count2;
        int total;
    };

    struct SimilarityReport {
        std::size_t totalWords1;
        std::size_t totalWords2;
        std::size_t uniqueWords1;
        std::size_t uniqueWords2;
        std::size_t intersectionSize;

        double jaccardPercent;
        double orderedPercent;
        double reversePercent;
        double finalPercent;

        std::size_t alphanumericChars1;
        std::size_t alphanumericChars2;
        double avgWordLength1;
        double avgWordLength2;

        std::vector<MatchingWord> topMatches;
    };

    SimilarityReport analyze(const std::string& text1, const std::string& text2,
                             const AnalyzeOptions& options = {});

    double computeSimilarity(const std::string& text1, const std::string& text2,
                             const AnalyzeOptions& options = {});

    std::string similarityBand(double finalPercent);
}

#endif
