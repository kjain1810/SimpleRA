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
    logger.log("executeJoin");
    int bufferSize = stoi(parsedQuery.joinBufferSize.c_str());
    Table *T1 = tableCatalogue.getTable(parsedQuery.joinFirstRelationName);
    Table *T2 = tableCatalogue.getTable(parsedQuery.joinSecondRelationName);
    vector<string> columnNames;
    for(int a = 0; a < T1->columns.size(); a++)
        columnNames.push_back(T1->columns[a]);
    for(int a = 0; a < T2->columns.size(); a++)
        columnNames.push_back(T2->columns[a]);
    Table *result = new Table(parsedQuery.joinResultRelationName, columnNames);
    int columnIndexT1 = T1->getColumnIndex(parsedQuery.joinFirstColumnName);
    int columnIndexT2 = T2->getColumnIndex(parsedQuery.joinSecondColumnName);
    if (parsedQuery.joinAlgorithm == "NESTED")
    {
        Cursor cursorT1 = T1->getCursor();
        int pageCounterT2 = 0;
        int pageCounterT1 = 0;
        vector<vector<int>> rowsResult;
        while(pageCounterT1 < T1->rowsPerBlockCount.size())
        {
            Cursor cursorT2 = T2->getCursor();
            vector<vector<int>> rowsT1;
            int cnt = 0;
            int pagesRead = 0;
            while(true)
            {
                if (cnt == T1->rowsPerBlockCount[pageCounterT1])
                {
                    cnt = 0;
                    pageCounterT1++;
                    pagesRead++;
                    if(pageCounterT1 < T1->rowsPerBlockCount.size())
                        T1->getNextPage(&cursorT1);
                }
                if (pageCounterT1 == T1->rowsPerBlockCount.size())
                    break;
                if(pagesRead == bufferSize - 2)
                    break;
                rowsT1.push_back(cursorT1.getNext());
                cnt++;
            }
            vector<vector<int>> rowsT2;
            pageCounterT2 = 0;
            while(pageCounterT2 < T2->rowsPerBlockCount.size())
            {
                rowsT2.clear();
                for(int a = 0; a < T2->rowsPerBlockCount[pageCounterT2]; a++)
                    rowsT2.push_back(cursorT2.getNext());

                for(int a = 0; a < rowsT2.size(); a++)
                    for(int b = 0; b < rowsT1.size(); b++)
                        if(rowsT2[a][columnIndexT2] == rowsT1[b][columnIndexT1])
                        {
                            vector<int> rowHere;
                            for(int c = 0; c < rowsT1[b].size(); c++)
                                rowHere.push_back(rowsT1[b][c]);
                            for(int c = 0; c < rowsT2[a].size(); c++)
                                rowHere.push_back(rowsT2[a][c]);
                            result->addRow(rowHere, rowsResult);
                        }
                pageCounterT2++;
                if(pageCounterT2 < T2->rowsPerBlockCount.size())
                    T2->getNextPage(&cursorT2);
            }
        }
        if(rowsResult.size())
            result->addPage(rowsResult);
        tableCatalogue.insertTable(result);
    }
}