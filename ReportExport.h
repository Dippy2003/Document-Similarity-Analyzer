#ifndef REPORTEXPORT_H
#define REPORTEXPORT_H

#include <string>

#include "Similarity.h"

namespace ReportExport {
    std::string toJson(const Similarity::SimilarityReport& report);
    std::string toCsv(const Similarity::SimilarityReport& report);
    bool writeJsonFile(const std::string& path, const Similarity::SimilarityReport& report);
    bool writeCsvFile(const std::string& path, const Similarity::SimilarityReport& report);
}

#endif
