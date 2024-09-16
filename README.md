# Octave-Simulator

//Copyright Popescu Mihnea-Gabriel 312CAb 2023-2024

The problem involves simulating operations in Octave.
Thus, we will work with the following operations represented as characters:

- L (Load), which loads a new matrix into memory.
- D (Dimension), which displays the dimensions of a matrix by a given index.
- P (Print), which prints a matrix by a given index.
- C (Crop), which resizes a matrix by a given index.
- M (Multiply), which multiplies two matrices by two given indices.
- O (Order), which sorts the stored matrices in ascending order based on the sum of their elements.
- T (Transpose), which transposes a matrix by a given index.
- R (Raise), which raises a matrix by a given index to a given power.
- F (Free), which frees a matrix from memory by a given index.
- S (Strassen), which multiplies two matrices by two given indices using the Strassen algorithm.
- Q (Quit), which ends the program and frees the used memory.

If any other command is introduced, the message "Unrecognized command" will be displayed. If non-existent indices are provided, the message "No matrix with the given index" will appear. If matrix multiplication cannot be performed, the message "Cannot perform matrix multiplication" will be displayed. If a negative index is given for raising to a power, the message "Power should be positive" will appear. To avoid overflow issues, multiplication and power operations will use %MOD, where MOD = 10007.

I have decided to structure the problem into multiple functions, keeping the main function minimal, only including the command calls, variable declarations and inputs, and error messages.

## Storing matrices

I approached this by using a matrix vector called "matrix", in which I store each matrix introduced with Load, and each matrix resulting from normal multiplication or Strassen multiplication, if conditions are met. If the matrix vector is full, we will reallocate double the memory.

## Transposition

To transpose a matrix, I decided to approach the problem as follows: I will determine the maximum between the number of rows and columns to allocate space for a matrix of size max(rows, columns) since the number of rows is not necessarily equal to the number of columns. I will swap the element at row i, column j with the element at row j, column i, but only if i is different from j (these elements belong to the main diagonal).

## Raising to a power

Raising a matrix to a power will be done in logarithmic time using the following algorithm:
-> If the power is even, we will split the power in two. For example, if we have matrix A, then A ^ n = A ^ (n / 2) * A ^ (n / 2).
-> If the power is odd, then A ^ n can be calculated as: A * A ^ ((n-1) / 2) * A ^ ((n-1) / 2).
This algorithm has a complexity of O(log2 n).

## Freeing memory

When we receive the free command, we will need to free the memory allocated for the matrix at position i. We will also free the memory where the matrix dimensions are stored in the 'rows' and 'columns' vectors. Moreover, we will shift each matrix and dimension one position to the left to correctly track the new indices.

## Strassen

This algorithm can only be applied to square matrices with dimensions of 2^n. For this algorithm, I used a Divide and Conquer approach. The matrix will be partitioned into 4 blocks, which will, in turn, be partitioned into 4 more blocks, and so on. The new matrix (C) will be formed from 7 new blocks, which are calculated as follows:

M1 = (A1 + A4) * (B1 + B4)
M2 = (A3 + A4) * B1
M3 = A1 * (B2 - B4)
M4 = A4 * (B3 - B1)
M5 = (A1 + A2) * B4
M6 = (A3 - A1) * (B1 + B2)
M7 = (A2 - A4) * (B3 + B4)

Where:

A = [A1, A2] ; B = [B1, B2] ; C = [M1+M4-M5+M7, M3+M5]
    [A3, A4]       [B3, B4]       [M2+M4, M1-M2+M3+M6]

## Exiting the program

When Q is entered, all resources will be freed: the matrix vector, the rows vector, and the columns vector.
