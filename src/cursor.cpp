#include "global.h"

Cursor::Cursor(string tableName, int pageIndex)
{
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getPage(tableName, pageIndex);
    this->pagePointer = 0;
    this->tableName = tableName;
    this->pageIndex = pageIndex;
}

Cursor::Cursor(string matrixName, int pageIndex, int row, int col)
{
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getPage(matrixName, pageIndex, row, col);
    this->pagePointer = 0;
    this->tableName = matrixName;
    this->pageIndex = pageIndex;
}

/**
 * @brief This function reads the next row from the page. The index of the
 * current row read from the page is indicated by the pagePointer(points to row
 * in page the cursor is pointing to).
 *
 * @return vector<int> 
 */
vector<int> Cursor::getNext()
{
    logger.log("Cursor::getNext");
    vector<int> result = this->page.getRow(this->pagePointer);
    this->pagePointer++;
    if(result.empty()){
        if(tableCatalogue.isTable(this->tableName))
            tableCatalogue.getTable(this->tableName)->getNextPage(this);
        else
            tableCatalogue.getMatrix(this->tableName)->getNextPage(this);
        if(!this->pagePointer){
            result = this->page.getRow(this->pagePointer);
            this->pagePointer++;
        }
    }
    return result;
}

vector<int> Cursor::getMatrixPage()
{
    logger.log("Cursor::getMatrixPage");
    vector<int> result = this->page.getElements();
    this->pagePointer++;
    if(result.empty())
    {
        cout << "Results empty!";
        tableCatalogue.getMatrix(this->tableName)->getNextPage(this);
        result = this->page.getElements();
        this->pagePointer++;
    }
    return result;
}
/**
 * @brief Function that loads Page indicated by pageIndex. Now the cursor starts
 * reading from the new page.
 *
 * @param pageIndex 
 */
void Cursor::nextPage(int pageIndex)
{
    logger.log("Cursor::nextPage");
    this->page = bufferManager.getPage(this->tableName, pageIndex);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
}

void Cursor::nextPage(int pageIndex, int row, int col)
{
    logger.log("Cursor::nextPage");
    this->page = bufferManager.getPage(this->tableName, pageIndex, row, col);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
}