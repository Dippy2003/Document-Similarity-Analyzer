# Building

## CMake (recommended)

```bash
cmake -S . -B build
cmake --build build
```

The executable target is **PlagiarismChecker**. After building, look under `build/` (and sometimes `build/Debug` or `build/Release` depending on the generator) for `PlagiarismChecker` or `PlagiarismChecker.exe`.

## g++ (single command)

```bash
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp ReportExport.cpp CliConfig.cpp -o PlagiarismChecker
```

On Windows with MSYS2 MinGW, the compiler writes `PlagiarismChecker.exe` in the current directory.

## MSYS2 / MinGW (Windows)

Use the **MSYS2 UCRT64** terminal from the Start menu so `g++` from MinGW-w64 is on your PATH.

Install the compiler if needed (from a UCRT64 shell):

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Change to your project directory using MSYS2 paths, for example:

```bash
cd /c/Users/YourName/path/to/Document-Similarity-Analyzer
```

Then run the same `g++` command as above and start the program:

```bash
./PlagiarismChecker.exe
```

## CLI quick test (after build)

From the project root:

```bash
./PlagiarismChecker.exe samples/doc1.txt samples/doc2.txt
./PlagiarismChecker.exe samples/doc1.txt samples/doc2.txt --json report.json --csv report.csv
./PlagiarismChecker.exe samples/short1.txt samples/short2.txt --cosine-only
./PlagiarismChecker.exe --help
```
