#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include <string>
#include <vector>

namespace TextUtils {
    // Normalize text: lowercase, remove punctuation, collapse multiple spaces.
    // If removeStopwords is true, common stopwords are removed.
    std::string normalizeText(const std::string& input, bool removeStopwords = false);

    // Tokenize text into words separated by spaces.
    // If removeStopwords is true, common stopwords are removed.
    std::vector<std::string> tokenizeWords(const std::string& text, bool removeStopwords = false);

    // Count letters and digits in the original string (before normalization).
    std::size_t countAlphanumericChars(const std::string& input);

    // Mean token length in characters; returns 0.0 if tokens is empty.
    double averageTokenLength(const std::vector<std::string>& tokens);
}

#endif

