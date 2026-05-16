@echo off
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp -o PlagiarismChecker.exe
if %ERRORLEVEL% EQU 0 (
    echo Build succeeded: PlagiarismChecker.exe
) else (
    echo Build failed.
    exit /b 1
)
