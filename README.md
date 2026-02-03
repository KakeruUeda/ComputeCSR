# Sparse Matrix Assembly

Demonstrates COO to CSR matrix assembly with:
- Compression from COO to CSR format
- Deduplication of entries (summing duplicates)
- Sorting column indices within each row
- Efficient value updates with mapping function (sparsity pattern remains unchanged)

## Example Model
A simple loop model:

```
n0 -- c0 -- n1
|           |
----- c1 ----
```

n0: node 0 <br>
n1: node 1 <br>
c0: component 0 (has internal equations) <br>
c1: component 1 

### Local Matrices

**Component 0** (3×3) 
|         | n0  | n1  | int |
|---------|-----|-----|-----|
| **n0**  | 2.4 | 0.0 | 0.6 |
| **n1**  | 0.0 | 3.1 | 0.0 |
| **int** | 0.0 | 0.0 | 5.0 |

**Component 1** (2×2) 
|        | n1  | n0  |
|--------|-----|-----|
| **n1** | 0.0 | 0.5 |
| **n0** | 4.5 | 1.1 |

### Global Matrix 

|         | n0  | n1  | int |
|---------|-----|-----|-----|
| **n0**  | 3.5 | 4.5 | 0.6 |
| **n1**  | 0.5 | 3.1 | 0.0 |
| **int** | 0.0 | 0.0 | 5.0 |


### Global COO
Assembly order: c0 -> c1

```
row indices:    [0, 0, 1, 2, 1, 0, 0]
column indices: [0, 2, 1, 2, 0, 1, 0]
values:         [2.4, 0.6, 3.1, 5.0, 0.5, 4.5, 1.1]
```

## Matrix Transformations

### 1. `compress()` 

This includes duplicates
```
row pointers:   [0, 4, 6, 7]
column indices: [0, 2, 1, 0, 1, 0, 2]
values:         [2.4, 0.6, 4.5, 1.1, 3.1, 0.5, 5.0]
```
### 2. `deduplicate()`

```
row pointers:   [0, 3, 5, 6]
column indices: [0, 2, 1, 1, 0, 2]
values:         [3.5, 0.6, 4.5, 3.1, 0.5, 5.0]
```
### 3. `sort()`

```
row pointers:   [0, 3, 5, 6]
column indices: [0, 1, 2, 0, 1, 2]
values:         [3.5, 4.5, 0.6, 0.5, 3.1, 5.0]
```
### 4. `update()` 

```
updated values (c0): [4.8, 0.0, 1.2, 6.2, 10.0]
updated values (c1): [0.0, 1.0, 9.0, 2.2]
```

Updated CSR matrix:

```
row pointers:   [0, 3, 5, 6]
column indices: [0, 1, 2, 0, 1, 2]
values:         [7.0, 9.0, 1.2, 1.0, 6.2, 10.0]
```
