#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <string>
#include <vector>

namespace Similarity {
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

    SimilarityReport analyze(const std::string& text1, const std::string& text2);

    double computeSimilarity(const std::string& text1, const std::string& text2);
}

#endif

