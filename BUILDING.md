# Building

## CMake (recommended)

```bash
cmake -S . -B build
cmake --build build
```

The executable will be inside the `build/` folder.

## g++ (single command)

```bash
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp -o PlagiarismChecker
```

