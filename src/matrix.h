#ifndef matrix_H
#define matrix_H
#include "cursor.h"

class Matrix
{
public:
    string sourceFileName = "";
    string matrixName = "";
    bool sparseMatrix = false;
    bool isZero = true;
    bool transposed = false;
    uint n = 0;
    int maxElementsPerBlock;
    int blockCount = 0;
    vector<uint> elementsPerBlockCount;

    bool load();
    bool load_sparse();
    bool load_normal();
    int check_sparse();

    void print();
    void transpose();
    void exportMatrix();
    bool isPermanent();
    int get_element(int row, int col);
    int get_row_sparse(int row, int col);

    Matrix(string);
    Matrix();
    int getPageNumber(int i, int j);
    int positionInPage(int i, int j);
    pair<int, int> pageName(int pageIndex);

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
