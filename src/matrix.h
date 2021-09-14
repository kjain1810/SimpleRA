#ifndef matrix_H
#define matrix_H
#include "cursor.h"

class Matrix
{
public:
    string sourceFileName = "";
    string matrixName = "";
    bool sparseMatrix = false;
    bool transposed = false;
    uint n = 0;
    int maxRowsPerBlock;
    int blockCount = 0;
    vector<uint> rowsPerBlockCount;

    bool load();
    bool load_sparse();
    bool load_normal();
    int check_sparse();

    void print();
    Matrix(string);
    Matrix();

    void getNextPage(Cursor *cursor);

    template <typename T>
    void writeRow(vector<T> row, ostream &fout)
    {
        logger.log("Table::printRow");
        for (int columnCounter = 0; columnCounter < row.size(); columnCounter++)
        {
            if (columnCounter != 0)
                fout << ", ";
            fout << row[columnCounter];
        }
        fout << endl;
    }

    template <typename T>
    void writeRow(vector<T> row)
    {
        logger.log("Table::printRow");
        ofstream fout(this->sourceFileName, ios::app);
        this->writeRow(row, fout);
        fout.close();
    }
};
#endif
