# Matrix

## Page Layout
For normal matrices, the numbers are stored as arrays in a row major fashion.
Since the size of a single can be large, it can across several pages as well.
The next row starts right after where the current row ends to save space.

## Compression technique
For sparse matrices, we store the non-zero elements in the for of $(r * n + c, val)$ in rows, where $r$ is the row, $c$ is the column and $val$ is the value stored at $(r, c)$

## Compression ratio
For matrices with $x$ fraction of cells filled, we use $2xn^2$ space. Compared to the normal page layout, where the storage space is $n^2$, our compression ratio is $2x$ 
Therefore, when the number of cells filled is less than a third, we save space by store the numbres in this manner.

## Transpose 
Transpose of a transposed matrix is the original matrix itself. Therefore, we do a lazy transpose. That is, we mark whether we need the transpose of the original matrix or not and handle the other 2 queries, PRINT and EXPORT, accordingly.

During both of them, we don't need to change the original pages themselves, we simply retrieve the required index from the pages and print them accordingly to either an output file or to the standard output.

The method remains the same for both, sparse and normal matrices. The implementation does need to change a bit