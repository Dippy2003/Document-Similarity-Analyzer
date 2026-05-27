# Document Similarity Analyzer

A C++17 console application that compares two text documents and reports how similar they are. The project demonstrates core data structures—linked list, queue, stack, and binary search tree—applied to a digital plagiarism / similarity checker.

**Repository:** [github.com/Dippy2003/Document-Similarity-Analyzer](https://github.com/Dippy2003/Document-Similarity-Analyzer)

## Features

- **Input modes:** interactive menu, **CLI file comparison**, or demo paragraphs
- **Multi-metric scoring:**
  - **Jaccard** — overlap of unique words (via BST traversal)
  - **Ordered** — words matching at the same index (via queue)
  - **Reverse** — matches when read from the end (via stack)
- **Configurable weights** via `--weights jaccard,ordered,reverse` (default `0.6,0.2,0.2`)
- **JSON and CSV export** for reports (`--json`, `--csv`)
- **Cosine similarity metric** based on token frequency vectors
- **Similarity band** label (Very low → Very high)
- **Top matching words** with configurable limit (`--top N`)

## Project structure

| Path | Role |
|------|------|
| `main.cpp` | Menu, CLI, input handling, report output |
| `TextUtils.h` / `TextUtils.cpp` | Normalization, tokenization, stopwords, stats |
| `Similarity.h` / `Similarity.cpp` | Metrics, report model, similarity bands |
| `ReportExport.h` / `ReportExport.cpp` | JSON and CSV serialization |
| `CliConfig.h` / `CliConfig.cpp` | Command-line parsing |
| `LinkedList.h`, `Queue.h`, `Stack.h`, `BST.h` | Custom data structures |
| `samples/doc1.txt`, `samples/doc2.txt` | Example texts |
| `build.bat` | Quick Windows build with `g++` |
| `CMakeLists.txt` | CMake build target `PlagiarismChecker` |
| `BUILDING.md` | MSYS2 / MinGW and CMake details |

## Prerequisites

- C++17 compiler (`g++`, `clang++`, or MSVC)
- Optional: **CMake 3.16+**
- Terminal: PowerShell, Command Prompt, Bash, or MSYS2

## Build and run

### Option A — `g++`

```bash
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp ReportExport.cpp CliConfig.cpp -o PlagiarismChecker
```

```bash
./PlagiarismChecker          # Linux / macOS
.\PlagiarismChecker.exe      # Windows
```

### Option B — `build.bat` (Windows)

```bat
build.bat
PlagiarismChecker.exe
```

### Option C — CMake

```bash
cmake -S . -B build
cmake --build build
```

See [BUILDING.md](BUILDING.md) for MSYS2 / MinGW setup on Windows.

## Command-line usage

Compare two files without the menu:

```powershell
.\PlagiarismChecker.exe samples\doc1.txt samples\doc2.txt
```

Export reports:

```powershell
.\PlagiarismChecker.exe samples\doc1.txt samples\doc2.txt --json report.json --csv report.csv
```

Custom weights and top-word limit:

```powershell
.\PlagiarismChecker.exe samples\doc1.txt samples\doc2.txt --weights 0.5,0.3,0.2 --top 10
```

| Flag | Description |
|------|-------------|
| `-h`, `--help` | Show usage |
| `-v`, `--version` | Show version (1.2.0) |
| `--weights j,o,r` | Metric weights |
| `--top N` | Max top matching words (default 15) |
| `--json path` | Write JSON report |
| `--csv path` | Write CSV report |
| `--keep-stopwords` | Do not filter stopwords |
| `--cosine-only` | Print only the cosine similarity score |

## Interactive usage

Run without file arguments and choose:

| Option | Description |
|--------|-------------|
| **1** | Type each document; end with a **blank line** |
| **2** | Enter paths to two `.txt` files |
| **3** | **Demo mode** — sample paragraphs and sanity checks |

### Bundled samples

```text
samples\doc1.txt
samples\doc2.txt
```

## Sample output

The report includes:

- Line counts per document
- Total and unique word counts
- Alphanumeric character counts and average token length
- Unique-word intersection size
- Jaccard, ordered, reverse, and cosine similarity (%)
- Final weighted score (Jaccard + ordered + reverse) and **similarity band**
- Top matching words: `word : count1 + count2 = total`

## Similarity bands

| Band | Final score |
|------|-------------|
| Very high | ≥ 80% |
| High | ≥ 60% |
| Moderate | ≥ 40% |
| Low | ≥ 20% |
| Very low | &lt; 20% |

## Troubleshooting

| Issue | What to try |
|-------|-------------|
| `g++` not found | Install a C++ toolchain; reopen the terminal. See [BUILDING.md](BUILDING.md). |
| File mode fails | Use an absolute path, or run from the project directory. |
| Very low scores | Short or unrelated texts produce few shared tokens. |
| `--weights` error | Use three comma-separated numbers, e.g. `0.6,0.2,0.2`. |

## Educational focus

This project is intended for learning data structures and basic text analysis—not production plagiarism detection.

## License

No license file is specified yet. Contact the repository owner before redistributing.
