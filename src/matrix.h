#include "cursor.h"

class Matrix
{
public:
    string sourceFileName = "";
    string matrixName = "";
    bool sparseMatrix = false;
    int n = 0;

    bool load();
    Matrix(string);
};