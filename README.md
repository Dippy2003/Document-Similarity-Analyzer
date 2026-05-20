# Document Similarity Analyzer (PlagiarismChecker CPP)

A C++17 console application that compares two text documents and reports how similar they are. The project demonstrates core data structures—linked list, queue, stack, and binary search tree—applied to a simple digital plagiarism / similarity checker.

**Repository:** [github.com/Dippy2003/Document-Similarity-Analyzer](https://github.com/Dippy2003/Document-Similarity-Analyzer)

## Features

- **Three input modes:** manual entry, file paths, or built-in demo paragraphs
- **Multi-metric scoring:**
  - **Jaccard** — overlap of unique words (via BST traversal)
  - **Ordered** — words matching at the same index (via queue)
  - **Reverse** — matches when read from the end (via stack)
- **Weighted final score:** `0.6 × Jaccard + 0.2 × Ordered + 0.2 × Reverse`
- **Similarity band** label (Very low → Very high)
- **Top matching words** with per-document frequency counts

## Project structure

| Path | Role |
|------|------|
| `main.cpp` | Menu, input handling, report output |
| `TextUtils.h` / `TextUtils.cpp` | Normalization, tokenization, stopwords, stats |
| `Similarity.h` / `Similarity.cpp` | Metrics, report model, similarity bands |
| `LinkedList.h`, `Queue.h`, `Stack.h`, `BST.h` | Custom data structures (no STL containers for storage) |
| `samples/doc1.txt`, `samples/doc2.txt` | Example texts for file-load mode |
| `build.bat` | Quick Windows build with `g++` |
| `CMakeLists.txt` | CMake build target `PlagiarismChecker` |
| `BUILDING.md` | MSYS2 / MinGW and CMake details |

## How it works

1. Read two documents (stdin, files, or demo strings).
2. **Normalize** text: lowercase, strip punctuation, collapse spaces.
3. **Tokenize** into words; optional stopword removal (`the`, `a`, `is`, …).
4. For each document, populate:
   - **Linked list** — all tokens in insertion order
   - **Queue** — tokens for positional comparison
   - **Stack** — tokens for reverse-order comparison
   - **BST** — unique words with occurrence counts
5. Compute Jaccard, ordered, and reverse percentages; combine into a final score.
6. List up to **15** shared words ranked by combined frequency.

## Prerequisites

- C++17 compiler (`g++`, `clang++`, or MSVC)
- Optional: **CMake 3.16+** for the CMake workflow
- Terminal: PowerShell, Command Prompt, Bash, or MSYS2

## Build and run

### Option A — `g++` (single command)

```bash
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp -o PlagiarismChecker
```

```bash
./PlagiarismChecker          # Linux / macOS
.\PlagiarismChecker.exe      # Windows
```

### Option B — `build.bat` (Windows)

From the project folder (requires `g++` on your PATH):

```bat
build.bat
PlagiarismChecker.exe
```

### Option C — CMake

```bash
cmake -S . -B build
cmake --build build
```

Run the executable from `build/` (or `build/Debug` / `build/Release`, depending on your generator).

See [BUILDING.md](BUILDING.md) for MSYS2 / MinGW setup on Windows.

## Usage

At startup, choose:

| Option | Description |
|--------|-------------|
| **1** | Type each document; end input with a **blank line** |
| **2** | Enter paths to two `.txt` files |
| **3** | **Demo mode** — runs sample paragraphs and internal sanity checks |

### Try it with bundled samples

1. Build the app (any option above).
2. Run the executable and select **2**.
3. Enter paths (relative to the project folder):

   ```
   samples\doc1.txt
   samples\doc2.txt
   ```

   On Linux/macOS use `samples/doc1.txt` and `samples/doc2.txt`.

4. Review the similarity report.

### Demo mode

Option **3** uses two built-in paragraphs about plagiarism detection and prints a full report without typing or files.

## Sample output

The report includes:

- Total and unique word counts per document
- Alphanumeric character counts and average token length
- Size of the unique-word intersection
- Jaccard, ordered, and reverse similarity (%)
- Final weighted score and **similarity band**
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
| File mode fails | Use an absolute path, or run from the project directory so `samples\...` resolves. |
| Very low scores | Short or unrelated texts produce few shared tokens. |
| Executable missing | Run from the folder where you built, or use the path under `build/`. |

## Educational focus

This project is intended for learning data structures and basic text analysis—not production plagiarism detection. Real systems use n-grams, hashing, databases, and much larger corpora.

## Future improvements

- Configurable metric weights (CLI flags)
- Export reports to CSV or JSON
- Unit tests for `TextUtils` and `Similarity`
- Compare more than two documents at once

## License

No license file is specified yet. Contact the repository owner before redistributing or using this code in other projects.
