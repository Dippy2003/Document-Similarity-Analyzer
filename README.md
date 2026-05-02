## Digital Plagiarism Similarity Checker

### Purpose

This console application compares two pieces of text and reports how similar they are.  
It is designed as a data-structures–oriented project, showing how core structures can be used to build a simple digital plagiarism / similarity checker.

### Data Structures Used

- **Array (`std::string textArr[2]`)**:  
  Stores the original, unmodified text for document 1 and document 2.

- **Linked List (`LinkedList`)**:  
  Holds all extracted (cleaned) words from each document. It demonstrates dynamic storage of a word sequence without using STL containers.

- **Queue (`Queue`)**:  
  Stores words of each document in their **original order**. This is used to compute an **ordered similarity**, comparing words at the same positions.

- **Stack (`Stack`)**:  
  Stores words in LIFO order so we can traverse them in **reverse order**. This is used for **reverse similarity**, comparing documents from the end backwards.

- **Binary Search Tree (`BST`)**:  
  Stores **unique** words with a `count` field for frequency.  
  Its in-order traversal provides sorted unique words and frequencies, used for:
  - Jaccard similarity (unique-word intersection / union)
  - Matching-words report (top common words and their counts)

### How It Works (Step by Step)

1. **Input selection**
   - User chooses:
     - Manual text entry for both documents, or
     - Loading each document from a file, or
     - Demo mode with built-in sample paragraphs.

2. **Store original texts**
   - The two raw texts are stored in:
     - `textArr[0]` → document 1
     - `textArr[1]` → document 2

3. **Text cleaning and tokenization**
   - `TextUtils::normalizeText`:
     - Lowercases text
     - Removes punctuation (treats it as spaces)
     - Collapses multiple spaces into one
   - `TextUtils::tokenizeWords`:
     - Splits normalized text by spaces into `std::vector<std::string>`
     - Optionally removes common stopwords (`the, a, an, is, are, to, of, and, in`)
   - `TextUtils::countAlphanumericChars`:
     - Counts letters and digits in the **original** input (before normalization), useful for raw document size hints.
   - `TextUtils::averageTokenLength`:
     - Returns the mean length in characters of the cleaned tokens for a document (0.0 if there are no tokens).
   - The project uses `tokenizeWords(..., true)` to remove stopwords.

4. **Populate data structures**
   For each document:
   - **LinkedList**: insert all cleaned tokens.
   - **Queue**: enqueue tokens in original order.
   - **Stack**: push tokens to allow reverse-order traversal.
   - **BST**: insert tokens so that:
     - Each unique word appears once in the tree.
     - The `count` field increments for each occurrence.

5. **Compute similarity metrics**

   All metrics are implemented in `Similarity.cpp` (namespace `Similarity`):

   - **Jaccard Similarity (unique words + BST)**
     - Get sorted unique words from each BST via in-order traversal.
     - Let:
       - \( |A| \) = number of unique words in doc1
       - \( |B| \) = number of unique words in doc2
       - \( |A \cap B| \) = number of common unique words
       - \( |A \cup B| = |A| + |B| - |A \cap B| \)
     - Jaccard similarity (0–100%) is:
       \[ \text{Jaccard} = \frac{|A \cap B|}{|A \cup B|} \times 100 \]

   - **Ordered Similarity (Queue)**
     - Convert each document’s queue to a vector of words in original order.
     - Compare words at the same indices:
       - `matches` = number of positions where `word1[i] == word2[i]`
       - `maxLen` = `max(len1, len2)`
     - Ordered similarity (0–100%):
       \[ \text{Ordered} = \frac{\text{matches}}{\text{maxLen}} \times 100 \]

   - **Reverse Similarity (Stack)**
     - Convert each document’s stack to a vector, which represents the words in **reversed** order.
     - Compare reversed vectors position-by-position, similar to ordered similarity.
     - Reverse similarity (0–100%):
       \[ \text{Reverse} = \frac{\text{matches}}{\text{maxLen}} \times 100 \]

   - **Final Weighted Similarity**
     - Combine the three metrics as a weighted average:
       \[
       \text{Final} = 0.6 \times \text{Jaccard} +
                      0.2 \times \text{Ordered} +
                      0.2 \times \text{Reverse}
       \]

   - **Similarity band (`Similarity::similarityBand`)**
     - Maps the final percentage to a short label for quick interpretation:
       - **Very high** (at least 80%), **High** (at least 60%), **Moderate** (at least 40%), **Low** (at least 20%), **Very low** (below 20%), or **Unknown** for non-finite values.

### Matching Words Report

Using BST traversal, the project builds a list of common words:

- Traverse doc1’s BST to get `(word, count1)` pairs.
- For each word, look it up in doc2’s BST to get `count2`.
- If the word exists in both:
  - `total = count1 + count2`
  - Store `(word, count1, count2, total)`.
- Sort all such entries by `total` descending, then by `word` ascending.
- Print the **top 15** matching words as part of the report.

### Output

For a pair of documents, the program prints a formatted report including:

- Total and unique word counts per document.
- Alphanumeric character counts and average cleaned word length per document.
- Intersection size (common unique words).
- Jaccard, ordered, and reverse similarity percentages.
- Final similarity score (weighted average) and its similarity band label.
- A “Top Matching Words” section showing the most frequent shared words and counts.

### Demo Mode and Basic Checks

- **Demo Mode** (menu option 3) runs the checker on two built-in sample paragraphs and prints the full report.
- Before demo, it runs a few simple assert-style checks:
  - `tokenizeWords` removes stopwords and splits correctly.
  - `BST` insert/search behaves as expected.
  - `Queue` preserves word order.

### How to Compile and Run

From the `PlagiarismChecker_CPP` folder, use a C++17-capable compiler such as `g++`:

```bash
g++ -std=c++17 main.cpp TextUtils.cpp Similarity.cpp -o PlagiarismChecker
```

Then run the program:

```bash
./PlagiarismChecker      # on Linux/macOS
PlagiarismChecker.exe    # on Windows
```

Follow the on-screen menu to enter texts manually, load from files, or run demo mode.

