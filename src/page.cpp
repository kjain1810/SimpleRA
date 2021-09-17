#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    
    Table table;
    Matrix matrix;
    bool isMatrix = false;

    if(tableCatalogue.isTable(tableName))
        table = *tableCatalogue.getTable(tableName);
    else
    {
        matrix = *tableCatalogue.getMatrix(tableName);
        isMatrix = true;
    }
    uint maxRowCount;
    if(!isMatrix)
    {
        this->columnCount = table.columnCount;
        maxRowCount = table.maxRowsPerBlock;
    }
    else
    {
        this->columnCount = 3;
        maxRowCount = matrix.maxElementsPerBlock;
    }
    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);

    ifstream fin(pageName, ios::in);
    if (!isMatrix)
        this->rowCount = table.rowsPerBlockCount[pageIndex];
    else
        this->rowCount = matrix.elementsPerBlockCount[pageIndex];
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}

Page::Page(string matrixName, int pageIndex, int row, int col)
{
    logger.log("Page::Page 4 params");
    this->tableName = matrixName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/"+this->tableName+"_Page"+to_string(pageIndex) + "_" + to_string(row) + "_" + to_string(col);

    Matrix matrix = *tableCatalogue.getMatrix(matrixName);
    this->elementCount = matrix.elementsPerBlockCount[pageIndex];
    this->elements.assign(this->elementCount, 0);

    ifstream fin(pageName, ios::in);
    for(int elementCounter = 0; elementCounter < elementCount; elementCounter++)
        fin >> this->elements[elementCounter];
    fin.close();
}

Page::Page(string matrixName, int pageIndex, vector<int> elements, int elementCount, int startRow, int startCol)
{
    logger.log("Page::Page");
    this->tableName = matrixName;
    this->pageIndex = pageIndex;
    this->elements = elements;
    this->startCol = startCol;
    this->startRow = startRow;
    this->elementCount = this->elements.size();
    this->pageName = "../data/temp/"+this->tableName+"_Page"+to_string(pageIndex) + "_" + to_string(startRow) + "_" + to_string(startCol);
}


/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

vector<int> Page::getElements()
{
    logger.log("Page::getElements");
    return this->elements;
}

Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}

void Page::writePageMatrix()
{
    logger.log("Page::writePageMatrix");
    ofstream fout(this->pageName, ios::trunc);
    for(int elementCounter = 0; elementCounter < this->elementCount; elementCounter++)
    {
        if(elementCounter != 0)
            fout << " ";
        fout << this->elements[elementCounter];
    }
    fout.close();
}