#include "ReportExport.h"

#include <fstream>
#include <sstream>
#include <iomanip>

namespace {
    std::string escapeJsonString(const std::string& value) {
        std::ostringstream oss;
        for (char ch : value) {
            switch (ch) {
                case '\\': oss << "\\\\"; break;
                case '"': oss << "\\\""; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default: oss << ch; break;
            }
        }
        return oss.str();
    }

    void appendNumberField(std::ostringstream& oss, const char* key, double value) {
        oss << ",\"" << key << "\":" << std::fixed << std::setprecision(2) << value;
    }

    void appendSizeField(std::ostringstream& oss, const char* key, std::size_t value) {
        oss << ",\"" << key << "\":" << value;
    }
}

namespace ReportExport {
    std::string toJson(const Similarity::SimilarityReport& report) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "{";
        oss << "\"totalWords1\":" << report.totalWords1;
        oss << ",\"totalWords2\":" << report.totalWords2;
        appendSizeField(oss, "uniqueWords1", report.uniqueWords1);
        appendSizeField(oss, "uniqueWords2", report.uniqueWords2);
        appendSizeField(oss, "intersectionSize", report.intersectionSize);
        appendNumberField(oss, "jaccardPercent", report.jaccardPercent);
        appendNumberField(oss, "orderedPercent", report.orderedPercent);
        appendNumberField(oss, "reversePercent", report.reversePercent);
        appendNumberField(oss, "finalPercent", report.finalPercent);
        oss << ",\"similarityBand\":\""
            << escapeJsonString(Similarity::similarityBand(report.finalPercent))
            << "\"";
        oss << ",\"topMatches\":[";
        for (std::size_t i = 0; i < report.topMatches.size(); ++i) {
            if (i > 0) {
                oss << ',';
            }
            const auto& mw = report.topMatches[i];
            oss << "{\"word\":\"" << escapeJsonString(mw.word) << "\"";
            oss << ",\"count1\":" << mw.count1;
            oss << ",\"count2\":" << mw.count2;
            oss << ",\"total\":" << mw.total << '}';
        }
        oss << "]}";
        return oss.str();
    }

    std::string toCsv(const Similarity::SimilarityReport& report) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "metric,value\n";
        oss << "total_words_doc1," << report.totalWords1 << "\n";
        oss << "total_words_doc2," << report.totalWords2 << "\n";
        oss << "unique_words_doc1," << report.uniqueWords1 << "\n";
        oss << "unique_words_doc2," << report.uniqueWords2 << "\n";
        oss << "intersection_size," << report.intersectionSize << "\n";
        oss << "jaccard_percent," << report.jaccardPercent << "\n";
        oss << "ordered_percent," << report.orderedPercent << "\n";
        oss << "reverse_percent," << report.reversePercent << "\n";
        oss << "final_percent," << report.finalPercent << "\n";
        oss << "similarity_band," << Similarity::similarityBand(report.finalPercent) << "\n";
        oss << "\nword,count_doc1,count_doc2,total\n";
        for (const auto& mw : report.topMatches) {
            oss << mw.word << ',' << mw.count1 << ',' << mw.count2 << ',' << mw.total << "\n";
        }
        return oss.str();
    }

    bool writeJsonFile(const std::string& path, const Similarity::SimilarityReport& report) {
        std::ofstream out(path.c_str());
        if (!out) {
            return false;
        }
        out << toJson(report);
        return true;
    }

    bool writeCsvFile(const std::string& path, const Similarity::SimilarityReport& report) {
        std::ofstream out(path.c_str());
        if (!out) {
            return false;
        }
        out << toCsv(report);
        return true;
    }
}
