#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT relation_name
 */
bool syntacticParsePRINT()
{
    logger.log("syntacticParsePRINT");
    if(tokenizedQuery.size() == 3)
    {
        if(tokenizedQuery[1] != "MATRIX")
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = PRINT_MATRIX;
        parsedQuery.printRelationName = tokenizedQuery[2];
        return true;
    }
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = PRINT;
    parsedQuery.printRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParsePRINT()
{
    logger.log("semanticParsePRINT");
    if(tableCatalogue.isTable(parsedQuery.printRelationName))
        return true;
    if(tableCatalogue.isMatrix(parsedQuery.printRelationName))
        return true;
    cout << "SEMANTIC ERROR: Relation or matrix doesn't exist" << endl;
    return false;
}

void executePRINT()
{
    logger.log("executePRINT");
    Table* table = tableCatalogue.getTable(parsedQuery.printRelationName);
    table->print();
    return;
}

void executePRINT_MATRIX()
{
    logger.log("executePRINT_MATRIX");
    Matrix* matrix = tableCatalogue.getMatrix(parsedQuery.printRelationName);
    matrix->print();
    return;
}
