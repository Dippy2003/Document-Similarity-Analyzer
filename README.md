# PlagiarismChecker CPP

A C++ console application that compares two documents and reports textual similarity using custom data structures.

## Highlights

- Multiple input modes: manual typing, file-based input, and demo mode.
- Multi-metric similarity scoring:
  - Jaccard similarity (unique-word overlap)
  - Ordered similarity (same word at same position)
  - Reverse similarity (same word when compared from the end)
- Weighted final score with a readable similarity band.
- Top shared words report with per-document counts.
- Data-structures-focused design (linked list, queue, stack, BST).

## Project Structure

- `main.cpp` - Program entry point, menu flow, input handling, and report printing.
- `TextUtils.h` / `TextUtils.cpp` - Text normalization, tokenization, and basic text statistics.
- `Similarity.h` / `Similarity.cpp` - Similarity analysis engine and reporting model.
- `LinkedList.h`, `Queue.h`, `Stack.h`, `BST.h` - Custom data structure implementations.
- `BUILDING.md` - Detailed CMake and MSYS2/MinGW build instructions.

## How It Works

1. Read two input documents.
2. Normalize and tokenize text (lowercase, punctuation handling, optional stopword removal).
3. Populate custom data structures for each document:
   - linked list for dynamic token storage
   - queue for original order comparisons
   - stack for reverse order comparisons
   - BST for unique words and frequency counts
4. Compute metrics:
   - `Jaccard` = unique intersection / unique union
   - `Ordered` = positional word matches / max token length
   - `Reverse` = reverse-positional matches / max token length
5. Compute final weighted score:
   - `Final = 0.6 * Jaccard + 0.2 * Ordered + 0.2 * Reverse`
6. Print summary and top matching words.

## Prerequisites

- A C++17-compatible compiler (`g++`, `clang++`, or MSVC).
- CMake 3.10+ if you want to use the CMake workflow.
- A terminal (PowerShell, Command Prompt, Bash, or MSYS2 shell).

## Build and Run

### Quick build with g++

```bash
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp -o PlagiarismChecker
```

### Run

```bash
./PlagiarismChecker
```

On Windows:

```powershell
.\PlagiarismChecker.exe
```

## CMake Build

```bash
cmake -S . -B build
cmake --build build
```

Run the generated executable from the `build` directory (or its configuration subfolder, depending on generator).

## Input Modes

When the app starts, choose one:

1. `Enter texts manually`  
   Type text and finish each document with a blank line.
2. `Load texts from files`  
   Provide file paths for both documents.
3. `Demo Mode`  
   Runs built-in sample paragraphs and basic assert-style checks.

## Quick Example Workflow

1. Build the app with `g++` or CMake.
2. Run `.\PlagiarismChecker.exe` (Windows) or `./PlagiarismChecker` (Linux/macOS).
3. Choose mode `1` for manual text input.
4. Paste document 1 and end it with a blank line.
5. Paste document 2 and end it with a blank line.
6. Review the generated similarity report and top matching words.

## Output Report

The report includes:

- total words and unique words in each document
- alphanumeric character counts
- average cleaned token length
- intersection of unique words
- Jaccard, ordered, and reverse similarity percentages
- final weighted similarity score + band label
- top matching words (`count_doc1 + count_doc2 = total`)

## Similarity Bands

- `Very high`: >= 80
- `High`: >= 60
- `Moderate`: >= 40
- `Low`: >= 20
- `Very low`: < 20

## Troubleshooting

- If `g++` is not recognized, install a C++ toolchain and reopen your terminal.
- If the executable does not run, verify you are in the build/output directory.
- If file mode cannot open a path, provide an absolute path to the text file.
- Empty or very short input can lead to very low similarity percentages.

## Notes

- This is an educational project focused on applying core data structures to text analysis.
- For platform-specific compiler setup, see `BUILDING.md`.
