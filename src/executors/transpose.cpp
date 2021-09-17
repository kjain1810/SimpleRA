#include "global.h"
/**
 * @brief 
 * SYNTAX: TRANSPOSE matrix_name
 */

bool syntacticParseTRANSPOSE()
{
    logger.log("syntacticParseTRANSPOSE");
    if(tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE;
    parsedQuery.transposeMatrixName = tokenizedQuery[1];
    return true;
}

bool semanticParseTRANSPOSE()
{
    logger.log("semanticParseTRANSPOSE");
    if(!tableCatalogue.isMatrix(parsedQuery.transposeMatrixName))
    {
        cout << "SEMANTIC ERROR: No such matrix exist" << endl;
        return false;
    }
    return true;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    Matrix* matrix = tableCatalogue.getMatrix(parsedQuery.transposeMatrixName);
    matrix->transpose();
    cout << "Matrix lazily transposed :)" << endl;
    return;
}