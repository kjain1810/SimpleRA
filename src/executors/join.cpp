#include "global.h"
#include <string>
/**
 * @brief 
 * SYNTAX: R <- JOIN relation_name1, relation_name2 ON column_name1 bin_op column_name2
 */
bool syntacticParseJOIN()
{
    logger.log("syntacticParseJOIN");
    if (tokenizedQuery.size() != 13 || tokenizedQuery[7] != "ON" || tokenizedQuery[11] != "BUFFER")
    {
        cout << "SYNTAC ERROR" << endl;
        cout << "Size: " << tokenizedQuery.size() << "\n";
        return false;
    }
    parsedQuery.queryType = JOIN;
    parsedQuery.joinResultRelationName = tokenizedQuery[0];
    parsedQuery.joinAlgorithm = tokenizedQuery[4];
    parsedQuery.joinFirstRelationName = tokenizedQuery[5];
    parsedQuery.joinSecondRelationName = tokenizedQuery[6];
    parsedQuery.joinFirstColumnName = tokenizedQuery[8];
    parsedQuery.joinSecondColumnName = tokenizedQuery[10];
    parsedQuery.joinBufferSize = tokenizedQuery[12];

    string binaryOperator = tokenizedQuery[9];
    if (binaryOperator == "<")
        parsedQuery.joinBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.joinBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.joinBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.joinBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.joinBinaryOperator = EQUAL;
    else if (binaryOperator == "!=")
        parsedQuery.joinBinaryOperator = NOT_EQUAL;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    cout << "Result relation: " << parsedQuery.joinResultRelationName << "\n";
    cout << "Join algorithm: " << parsedQuery.joinAlgorithm << "\n";
    cout << "First relation: " << parsedQuery.joinFirstRelationName << "\n";
    cout << "Second relation: " << parsedQuery.joinSecondRelationName << "\n";
    cout << "First column: " << parsedQuery.joinFirstColumnName << "\n";
    cout << "Operator: " << binaryOperator << "\n";
    cout << "Second column: " << parsedQuery.joinSecondColumnName << "\n";
    cout << "Buffer size: " << parsedQuery.joinBufferSize << "\n";
    return true;
}

bool semanticParseJOIN()
{
    logger.log("semanticParseJOIN");

    if (tableCatalogue.isTable(parsedQuery.joinResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.joinFirstRelationName) || !tableCatalogue.isTable(parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.joinFirstColumnName, parsedQuery.joinFirstRelationName) || !tableCatalogue.isColumnFromTable(parsedQuery.joinSecondColumnName, parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }

    if(atoi(parsedQuery.joinBufferSize.c_str()) < 3)
    {
        cout << "SEMANTIC ERROR: Buffer size should be atleast 3\n";
        return false;
    }
    return true;
}

void executeJOIN()
{
    logger.log("executeJOIN");
    return;
}