cmake_minimum_required(VERSION 3.16)
project(DocumentSimilarityAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DocumentSimilarityAnalyzer
    main.cpp
    TextUtils.cpp
    Similarity.cpp
)

