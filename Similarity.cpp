#include "Similarity.h"

#include "TextUtils.h"
#include "LinkedList.h"
#include "BST.h"
#include "Stack.h"
#include "Queue.h"

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

namespace {
    struct JaccardResult {
        double value;
        std::size_t intersectionSize;
        std::size_t unionSize;
        std::size_t unique1;
        std::size_t unique2;
        std::vector<BST::Entry> entries1;
        std::vector<BST::Entry> entries2;
    };

    JaccardResult jaccardUsingBST(const std::vector<std::string>& tokens1,
                                  const std::vector<std::string>& tokens2) {
        BST bst1;
        BST bst2;
        for (const auto& w : tokens1) {
            bst1.insert(w);
        }
        for (const auto& w : tokens2) {
            bst2.insert(w);
        }

        std::vector<std::string> unique1 = bst1.inorderTraversal();
        std::vector<std::string> unique2 = bst2.inorderTraversal();

        JaccardResult result;
        result.unique1 = unique1.size();
        result.unique2 = unique2.size();
        result.intersectionSize = 0;
        result.unionSize = 0;
        result.value = 0.0;
        result.entries1 = bst1.inorderEntries();
        result.entries2 = bst2.inorderEntries();

        if (unique1.empty() && unique2.empty()) {
            result.value = 1.0;
            return result;
        }

        std::size_t i = 0;
        std::size_t j = 0;
        std::size_t intersectionCount = 0;
        std::size_t unionCount = 0;

        while (i < unique1.size() && j < unique2.size()) {
            if (unique1[i] == unique2[j]) {
                ++intersectionCount;
                ++unionCount;
                ++i;
                ++j;
            } else if (unique1[i] < unique2[j]) {
                ++unionCount;
                ++i;
            } else {
                ++unionCount;
                ++j;
            }
        }
        unionCount += (unique1.size() - i);
        unionCount += (unique2.size() - j);

        result.intersectionSize = intersectionCount;
        result.unionSize = unionCount;

        if (unionCount == 0) {
            result.value = 0.0;
            return result;
        }

        result.value = static_cast<double>(intersectionCount) /
                       static_cast<double>(unionCount);
        return result;
    }

    double orderedSimilarityUsingQueue(const std::vector<std::string>& tokens1,
                                       const std::vector<std::string>& tokens2) {
        Queue q1;
        Queue q2;
        for (const auto& w : tokens1) {
            q1.enqueue(w);
        }
        for (const auto& w : tokens2) {
            q2.enqueue(w);
        }

        std::vector<std::string> v1 = q1.toVector();
        std::vector<std::string> v2 = q2.toVector();

        if (v1.empty() && v2.empty()) {
            return 1.0;
        }

        std::size_t maxLen = std::max(v1.size(), v2.size());
        if (maxLen == 0) {
            return 0.0;
        }

        std::size_t matches = 0;
        std::size_t limit = std::min(v1.size(), v2.size());
        for (std::size_t i = 0; i < limit; ++i) {
            if (v1[i] == v2[i]) {
                ++matches;
            }
        }

        return static_cast<double>(matches) / static_cast<double>(maxLen);
    }

    double reverseSimilarityUsingStack(const std::vector<std::string>& tokens1,
                                       const std::vector<std::string>& tokens2) {
        Stack s1;
        Stack s2;
        for (const auto& w : tokens1) {
            s1.push(w);
        }
        for (const auto& w : tokens2) {
            s2.push(w);
        }

        std::vector<std::string> v1 = s1.toVector(); // from top to bottom (reversed original order)
        std::vector<std::string> v2 = s2.toVector();

        if (v1.empty() && v2.empty()) {
            return 1.0;
        }

        std::size_t maxLen = std::max(v1.size(), v2.size());
        if (maxLen == 0) {
            return 0.0;
        }

        std::size_t matches = 0;
        std::size_t limit = std::min(v1.size(), v2.size());
        for (std::size_t i = 0; i < limit; ++i) {
            if (v1[i] == v2[i]) {
                ++matches;
            }
        }

        return static_cast<double>(matches) / static_cast<double>(maxLen);
    }
}

namespace Similarity {
    SimilarityReport analyze(const std::string& text1, const std::string& text2,
                             const AnalyzeOptions& options) {
        SimilarityReport report{};

        std::vector<std::string> tokens1 =
            TextUtils::tokenizeWords(text1, options.removeStopwords);
        std::vector<std::string> tokens2 =
            TextUtils::tokenizeWords(text2, options.removeStopwords);

        report.totalWords1 = tokens1.size();
        report.totalWords2 = tokens2.size();

        report.alphanumericChars1 = TextUtils::countAlphanumericChars(text1);
        report.alphanumericChars2 = TextUtils::countAlphanumericChars(text2);
        report.avgWordLength1 = TextUtils::averageTokenLength(tokens1);
        report.avgWordLength2 = TextUtils::averageTokenLength(tokens2);

        JaccardResult jac = jaccardUsingBST(tokens1, tokens2);
        report.uniqueWords1 = jac.unique1;
        report.uniqueWords2 = jac.unique2;
        report.intersectionSize = jac.intersectionSize;

        double ordered = orderedSimilarityUsingQueue(tokens1, tokens2);
        double reverse = reverseSimilarityUsingStack(tokens1, tokens2);

        auto toPercent = [](double v) {
            if (!std::isfinite(v)) {
                return 0.0;
            }
            if (v < 0.0) {
                v = 0.0;
            }
            if (v > 1.0) {
                v = 1.0;
            }
            return v * 100.0;
        };

        report.jaccardPercent = toPercent(jac.value);
        report.orderedPercent = toPercent(ordered);
        report.reversePercent = toPercent(reverse);

        const MetricWeights& w = options.weights;
        double finalScore = w.jaccard * jac.value + w.ordered * ordered + w.reverse * reverse;
        report.finalPercent = toPercent(finalScore);

        // Build matching words report using BST traversal and counts.
        std::vector<BST::Entry> entries1 = jac.entries1;
        std::vector<BST::Entry> entries2 = jac.entries2;

        std::unordered_map<std::string, int> counts2;
        counts2.reserve(entries2.size());
        for (const auto& e : entries2) {
            counts2[e.key] = e.count;
        }

        std::vector<MatchingWord> matches;
        for (const auto& e1 : entries1) {
            auto it = counts2.find(e1.key);
            if (it != counts2.end()) {
                MatchingWord mw;
                mw.word = e1.key;
                mw.count1 = e1.count;
                mw.count2 = it->second;
                mw.total = e1.count + it->second;
                matches.push_back(mw);
            }
        }

        std::sort(matches.begin(), matches.end(),
                  [](const MatchingWord& a, const MatchingWord& b) {
                      if (a.total != b.total) {
                          return a.total > b.total;
                      }
                      return a.word < b.word;
                  });

        int topLimit = options.topMatchLimit;
        if (topLimit < 1) {
            topLimit = 1;
        }
        if (matches.size() > static_cast<std::size_t>(topLimit)) {
            matches.resize(static_cast<std::size_t>(topLimit));
        }
        report.topMatches = matches;

        return report;
    }

    double computeSimilarity(const std::string& text1, const std::string& text2,
                             const AnalyzeOptions& options) {
        SimilarityReport report = analyze(text1, text2, options);
        return report.finalPercent / 100.0;
    }

    std::string similarityBand(double finalPercent) {
        if (!std::isfinite(finalPercent)) {
            return "Unknown";
        }
        if (finalPercent < 0.0) {
            finalPercent = 0.0;
        }
        if (finalPercent > 100.0) {
            finalPercent = 100.0;
        }
        if (finalPercent >= 80.0) {
            return "Very high";
        }
        if (finalPercent >= 60.0) {
            return "High";
        }
        if (finalPercent >= 40.0) {
            return "Moderate";
        }
        if (finalPercent >= 20.0) {
            return "Low";
        }
        return "Very low";
    }
}

