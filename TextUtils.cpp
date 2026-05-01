#include "TextUtils.h"

#include <cctype>
#include <sstream>
#include <unordered_set>

namespace {
    const std::unordered_set<std::string>& stopwords() {
        static const std::unordered_set<std::string> words = {
            "the", "a", "an", "is", "are", "to", "of", "and", "in"
        };
        return words;
    }

    bool isStopword(const std::string& word) {
        const auto& s = stopwords();
        return s.find(word) != s.end();
    }
}

namespace TextUtils {
    std::string normalizeText(const std::string& input, bool removeStopwords) {
        // Lowercase and replace punctuation with spaces.
        std::string lowered;
        lowered.reserve(input.size());
        for (unsigned char ch : input) {
            if (std::isalnum(ch) || std::isspace(ch)) {
                lowered.push_back(static_cast<char>(std::tolower(ch)));
            } else {
                // Treat punctuation as space.
                lowered.push_back(' ');
            }
        }

        // Collapse multiple spaces and trim.
        std::string normalized;
        normalized.reserve(lowered.size());
        bool inSpace = false;
        for (char ch : lowered) {
            if (std::isspace(static_cast<unsigned char>(ch))) {
                if (!inSpace && !normalized.empty()) {
                    normalized.push_back(' ');
                }
                inSpace = true;
            } else {
                normalized.push_back(ch);
                inSpace = false;
            }
        }
        // Remove trailing space if any.
        if (!normalized.empty() && normalized.back() == ' ') {
            normalized.pop_back();
        }

        if (!removeStopwords) {
            return normalized;
        }

        // Remove stopwords by tokenizing and rebuilding.
        std::vector<std::string> tokens = tokenizeWords(normalized, true);
        std::ostringstream oss;
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            if (i > 0) {
                oss << ' ';
            }
            oss << tokens[i];
        }
        return oss.str();
    }

    std::vector<std::string> tokenizeWords(const std::string& text, bool removeStopwords) {
        // Ensure basic normalization of spacing and case first (without removing stopwords).
        std::string normalized = normalizeText(text, false);

        std::vector<std::string> tokens;
        std::istringstream iss(normalized);
        std::string word;
        while (iss >> word) {
            if (removeStopwords && isStopword(word)) {
                continue;
            }
            tokens.push_back(word);
        }
        return tokens;
    }
}

