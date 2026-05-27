Cosine similarity notes
========================

This project now computes a cosine similarity score in addition to the
existing Jaccard / ordered / reverse metrics.

- Tokens are normalized with the same pipeline as the other metrics.
- Each document is converted into a bag-of-words frequency map.
- Cosine similarity is computed as:

  cos(theta) = (sum_i f1_i * f2_i) / (sqrt(sum_i f1_i^2) * sqrt(sum_i f2_i^2))

The score is clamped to the [0, 1] range and shown as a percentage
in the console report, JSON, and CSV exports.

