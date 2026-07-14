# Comparison Algorithms Metrics

## Compilation

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-long-long comparison_algorithms.c -o comparison_algorithms

## Raw Measurements

### Run 1
Naive algorithm result: 625025000
Naive algorithm time: 2.759290 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000106 seconds

### Run 2
Naive algorithm result: 625025000
Naive algorithm time: 2.761769 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000106 seconds

### Run 3
Naive algorithm result: 625025000
Naive algorithm time: 2.755006 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000106 seconds

## Averages

| Implementation | Run 1 | Run 2 | Run 3 | Average |
|---|---|---|---|---|
| Naive (O(n^2)) | 2.759290 | 2.761769 | 2.755006 | 2.758688 |
| Single-pass (O(n)) | 0.000106 | 0.000106 | 0.000106 | 0.000106 |

## Relative Difference

Average naive time / average single-pass time = 2.758688 / 0.000106

The naive implementation is approximately **26,025 times slower** than the
single-pass implementation.

## Analysis

The naive version uses nested loops. For each of the 50,000 elements it re-scans
all previous elements, performing roughly n^2 / 2 = 1.25 billion operations.
Its time complexity is O(n^2).

The single-pass version scans the array once, performing 50,000 operations.
Its time complexity is O(n).

The two implementations report different counts (625,025,000 vs 25,000) because
the naive version counts each even element repeatedly across its nested passes,
while the single-pass version counts each even element exactly once. The actual
number of even values in the array is 25,000. The naive version therefore does
vastly more work and still needs post-processing to recover the real answer.

Measurement variability across the three runs was very small (naive: 2.755-2.762 s,
single-pass: constant at 0.000106 s), which indicates the difference is caused by
algorithmic complexity, not by measurement noise.
