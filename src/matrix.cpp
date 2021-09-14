#include "global.h"

Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

Matrix::Matrix(string name)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + name + ".csv";
    this->matrixName = name;
}

bool Matrix::load()
{
    logger.log("Matrix::load");
    int here = this->check_sparse();
    if(here == -1)
        return false;
    if(here == 1)
        return this->load_sparse();
    return this->load_normal();
}

bool Matrix::load_normal()
{
    logger.log("Matrix::load_normal");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(this->n, 0);
    vector<vector<int>>rowsInPage(this->maxRowsPerBlock, row);
    int pageCounter = 0;
    cout << "Max Rows Per Block: " << this->maxRowsPerBlock << "\n";
    while(getline(fin, line))
    {
        stringstream s(line);
        for(int col = 0; col < this->n; col++)
        {
            cout << col << "\n";
            if(!getline(s, word, ','))
                return false;
            rowsInPage[pageCounter][col] = stoi(word);
        }
        pageCounter++;
        cout << pageCounter << "\n";
        if(pageCounter == this->maxRowsPerBlock)
        {
            bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
    }
    if(pageCounter)
    {
        bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, pageCounter);
        this->blockCount++;
        this->rowsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
    }
    return true;
}

bool Matrix::load_sparse()
{
    logger.log("Matrix::load_sparse");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(3, 0);
    vector<vector<int>>rowsInPage(this->maxRowsPerBlock, row);
    int pageCounter = 0;
    int rowCounter = 0;
    int idx = 0;
    while(getline(fin, line))
    {
        stringstream s(line);
        for(int col = 0; col < this->n; col++)
        {
            if(!getline(s, word, ','))
                return false;
            row[1] = col;
            row[2] = stoi(word);
            rowsInPage[idx] = row;
        }
        pageCounter++;
        if(pageCounter == this->maxRowsPerBlock)
        {
            bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
        rowCounter++;
        row[0] = rowCounter;
    }
    if(pageCounter)
    {
        bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, pageCounter);
        this->blockCount++;
        this->rowsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
    }
    return true;
}

int Matrix::check_sparse()
{
    logger.log("Matrix::check_sparse");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    int numZero = 0;
    int numRows = 0;
    while(getline(fin, line))
    {
        numRows++;
        stringstream s(line);
        if(this->n == 0)
        {
            while(getline(s, word, ','))
            {
                this->n++;
                numZero += (stoi(word) == 0);
            }
        }
        else 
        {
            int numCols = 0;
            while(getline(s, word, ','))
            {
                numCols++;
                numZero += (stoi(word) == 0);
            }
            if(numCols != this->n)
                return -1;
        }
    }
    if(numRows != this->n)
        return -1;
    if((1LL * numZero) * 5LL >= ((1LL * n) * (1LL * n)) * 3LL)
    {
        this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * 3));
        return 1;
    }
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * this->n));
    return 0;
}

void Matrix::print()
{
    logger.log("Matrix::print");
    uint count = min(PRINT_COUNT, this->n);

    Cursor cursor(this->matrixName, 0);
    vector<int>row;
    for(int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, cout);
    }
    printRowCount(this->n);
}

void Matrix::getNextPage(Cursor *cursor)
{
    logger.log("Matrix::getNextPage");
    if (cursor->pageIndex < this->blockCount - 1)
        cursor->nextPage(cursor->pageIndex+1);
}