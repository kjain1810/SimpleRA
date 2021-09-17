#ifndef page_H
#define page_H
#include"logger.h"
/**
 * @brief The Page object is the main memory representation of a physical page
 * (equivalent to a block). The page class and the page.h header file are at the
 * bottom of the dependency tree when compiling files. 
 *<p>
 * Do NOT modify the Page class. If you find that modifications
 * are necessary, you may do so by posting the change you want to make on Moodle
 * or Teams with justification and gaining approval from the TAs. 
 *</p>
 */

class Page{

    string tableName;
    string pageIndex;
    int columnCount;
    int rowCount;
    vector<vector<int>> rows;

    vector<int> elements;
    int startRow, startCol;
    long long elementCount;

    public:

    string pageName = "";
    Page();
    Page(string tableName, int pageIndex);
    Page(string matrixName, int pageIndex, bool isMatrix);
    Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);
    Page(string matrixName, int pageIndex, vector<int> elements, int elementCount, int startRow, int startCol);
    Page(string matrixName, int pageIndex, int row, int col);
    vector<int> getRow(int rowIndex);
    vector<int> getElements();
    void writePage();
    void writePageMatrix();
};
#endif
