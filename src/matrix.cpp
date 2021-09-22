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
    {
        this->sparseMatrix = true;
        return this->load_sparse();
    }
    return this->load_normal();
}

bool Matrix::load_normal()
{
    logger.log("Matrix::load_normal");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(this->maxElementsPerBlock, 0);
    int pageCounter = 0;
    int startRow = 0, startCol = 0;
    int curRow = 0;
    while(getline(fin, line))
    {
        stringstream s(line);
        for(int col = 0; col < this->n; col++)
        {
            if(!getline(s, word, ','))
                return false;
            row[pageCounter] = stoi(word);
            pageCounter++;
            if(pageCounter == this->maxElementsPerBlock)
            {
                bufferManager.writePageMatrix(this->matrixName, this->blockCount, row, pageCounter, startRow, startCol);
                this->blockCount++;
                this->elementsPerBlockCount.emplace_back(pageCounter);
                pageCounter = 0;
                if(col == n - 1)
                {
                    startRow = curRow + 1;
                    startCol = 0;
                }
                else
                {
                    startRow = curRow;
                    startCol = col + 1;
                }
            }
        }
        curRow++;
    }
    if(pageCounter)
    {
        bufferManager.writePageMatrix(this->matrixName, this->blockCount, row, pageCounter, startRow, startCol);
        this->blockCount++;
        this->elementsPerBlockCount.emplace_back(pageCounter);
    }
    return true;
}

bool Matrix::load_sparse()
{
    logger.log("Matrix::load_sparse");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(3, 0);
    vector<vector<int>>rowsInPage(this->maxElementsPerBlock, row);
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
            if(row[2] != 0)
            {
                rowsInPage[pageCounter] = row;
                pageCounter++;
            }
            if(pageCounter == this->maxElementsPerBlock)
            {
                bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, pageCounter);
                this->blockCount++;
                this->elementsPerBlockCount.emplace_back(pageCounter);
                pageCounter = 0;
            }
        }
        rowCounter++;
        row[0] = rowCounter;
    }
    if(pageCounter)
    {
        bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, pageCounter);
        this->blockCount++;
        this->elementsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
    }
    return true;
}

int Matrix::check_sparse()
{
    logger.log("Matrix::check_sparse");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    long long numZero = 0;
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
    this->isZero = ((this->n * 1LL) * (this->n * 1LL) == numZero);
    if(numRows != this->n)
        return -1;
    if((1LL * numZero) * 5LL >= ((1LL * n) * (1LL * n)) * 3LL)
    {
        this->maxElementsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * 3));
        return 1;
    }
    this->maxElementsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int)));
    return 0;
}

void Matrix::print()
{
    logger.log("Matrix::print");
    if(!this->sparseMatrix && !this->transposed)
    {
        uint count = min(PRINT_COUNT, this->n);
        for(int rowCounter = 0; rowCounter < count; rowCounter++)
        {
            int i = rowCounter;
            int j = 0;
            // vector<int> finalRow;
            while(j < n){
                int pageNumber = this->getPageNumber(i, j);
                pair<int, int> name = this->pageName(pageNumber);
                Cursor cursor(this->matrixName, pageNumber, name.first, name.second);
                vector<int> row = cursor.getMatrixPage();
                int idx = 0;
                while(name.first != i || name.second != j)
                {
                    idx++;
                    name.second++;
                    if(name.second == this->n)
                    {
                        name.second = 0;
                        name.first++;
                    }
                }
                while(idx < row.size() && j < this->n)
                {
                    cout << row[idx++];
                    if(j == n - 1)
                        cout << "\n";
                    else
                        cout << ", ";
                    j++;
                }
            }
        }
    }
    else if(!this->sparseMatrix && this->transposed)
    {
        uint count = min(PRINT_COUNT, this->n);
        for(int row = 0; row < count; row++)
        {
            for(int col = 0; col < this->n; col++)
            {
                cout << this->get_element(col, row);
                if(col < this->n - 1)
                    cout << ", ";
            }
            cout << "\n";
        }
    }
    else if(this->sparseMatrix && !this->transposed)
    {
        uint count = min(PRINT_COUNT, this->n);
        
        for(int r = 0; r < count; r++)
        {
            for(int c = 0; c < this->n; c++)
            {
                int here = this->get_row_sparse(r, c);
                cout << here;
                if(c != this->n)
                    cout << ",";
            }
            cout << "\n";
        }
    }
    else if(this->sparseMatrix &&& this->transposed)
    {
        uint count = min(PRINT_COUNT, this->n);
        
        for(int r = 0; r < count; r++)
        {
            for(int c = 0; c < this->n; c++)
            {
                int here = this->get_row_sparse(c, r);
                cout << here;
                if(c != this->n)
                    cout << ",";
            }
            cout << "\n";
        }

    }
    printRowCount(this->n);
}

int Matrix::get_element(int r, int c)
{
    logger.log("Matrix::get_element");
    int pageNumber = this->getPageNumber(r, c);
    pair<int, int> name = this->pageName(pageNumber);
    Cursor cursor(this->matrixName, pageNumber, name.first, name.second);
    vector<int> row = cursor.getMatrixPage();
    int idx = 0;
    while(name.first != r || name.second != c)
    {
        idx++;
        name.second++;
        if(name.second == this->n)
        {
            name.second = 0;
            name.first++;
        }
    }
    return row[idx];
}

int Matrix::get_row_sparse(int r, int c)
{
    logger.log("Matrix::get_element_sparse");
    Cursor cursor(this->matrixName, 0);
    vector<int> vec;
    int x = 0, y = 1;
    if(this->transposed)
        swap(x, y);
    while(true)
    {
        vec = cursor.getNext();
        if(vec.size() == 0)
            break;
        if(vec[x] == r && vec[y] == c)
            return vec[2];
    }
    return 0;
}

void Matrix::getNextPage(Cursor *cursor)
{
    logger.log("Matrix::getNextPage");
    if (cursor->pageIndex < this->blockCount - 1)
        cursor->nextPage(cursor->pageIndex+1);
}

int Matrix::getPageNumber(int i, int j)
{
    logger.log("Matrix::getPageNumber");
    return (i * this->n + j) / this->maxElementsPerBlock;
}

int Matrix::positionInPage(int i, int j)
{
    logger.log("Matrix::positionInPage");
    return (i * this->n + j) % this->maxElementsPerBlock;
}

pair<int, int> Matrix::pageName(int pageIndex)
{
    logger.log("Matrix::pageName");
    long long numElements = this->maxElementsPerBlock * pageIndex;
    return {numElements / this->n, numElements % this->n};
}

void Matrix::transpose()
{
    logger.log("Matrix::transpose");
    this->transposed = !this->transposed;
}

void Matrix::exportMatrix()
{
    logger.log("Matrix::exportMatrix");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    if(!this->sparseMatrix && !this->transposed)
    {
        uint count = this->n;
        for(int rowCounter = 0; rowCounter < count; rowCounter++)
        {
            int i = rowCounter;
            int j = 0;
            while(j < n){
                int pageNumber = this->getPageNumber(i, j);
                pair<int, int> name = this->pageName(pageNumber);
                Cursor cursor(this->matrixName, pageNumber, name.first, name.second);
                vector<int> row = cursor.getMatrixPage();
                int idx = 0;
                while(name.first != i || name.second != j)
                {
                    idx++;
                    name.second++;
                    if(name.second == this->n)
                    {
                        name.second = 0;
                        name.first++;
                    }
                }
                while(idx < row.size() && j < this->n)
                {
                    fout << row[idx++];
                    if(j == n - 1)
                        fout << "\n";
                    else
                        fout << ",";
                    j++;
                }
            }
        }
    }
    else if(!this->sparseMatrix && this->transposed)
    {
        uint count = this->n;
        for(int row = 0; row < count; row++)
        {
            for(int col = 0; col < this->n; col++)
            {
                fout << this->get_element(col, row);
                if(col < this->n - 1)
                    fout << ",";
            }
            fout << "\n";
        }
    }
    else if(this->sparseMatrix && !this->transposed)
    {
        uint count = this->n;
        for(int r = 0; r < count; r++)
        {
            for(int c = 0; c < this->n; c++)
            {
                int here = this->get_row_sparse(r, c);
                fout << here;
                if(c != this->n)
                    fout << ",";
            }
            fout << "\n";
        }
    }
    else if(this->sparseMatrix &&& this->transposed)
    {
        uint count = this->n;
        for(int r = 0; r < count; r++)
        {
            for(int c = 0; c < this->n; c++)
            {
                int here = this->get_row_sparse(c, r);
                fout << here;
                if(c != this->n)
                    fout << ",";
            }
            fout << "\n";
        }


    }

}

bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->matrixName + ".csv")
        return true;
    return false;
}