#include "global.h"
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT()
{
    logger.log("syntacticParseSORT");
    if (tokenizedQuery.size() == 10)
    {
        if (tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN" || tokenizedQuery[8] != "BUFFER")
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = SORT;
        parsedQuery.sortResultRelationName = tokenizedQuery[0];
        parsedQuery.sortRelationName = tokenizedQuery[3];
        parsedQuery.sortColumnName = tokenizedQuery[5];
        string sortingStrategy = tokenizedQuery[7];
        if (sortingStrategy == "ASC")
            parsedQuery.sortingStrategy = ASC;
        else if (sortingStrategy == "DESC")
            parsedQuery.sortingStrategy = DESC;
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.sortBufferSize = tokenizedQuery[9];
        return true;
    }
    else if (tokenizedQuery.size() == 8)
    {
        if (tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN")
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = SORT;
        parsedQuery.sortResultRelationName = tokenizedQuery[0];
        parsedQuery.sortRelationName = tokenizedQuery[3];
        parsedQuery.sortColumnName = tokenizedQuery[5];
        string sortingStrategy = tokenizedQuery[7];
        if (sortingStrategy == "ASC")
            parsedQuery.sortingStrategy = ASC;
        else if (sortingStrategy == "DESC")
            parsedQuery.sortingStrategy = DESC;
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.sortBufferSize = "10";
        return true;
    }
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
}

bool semanticParseSORT()
{
    logger.log("semanticParseSORT");

    if (tableCatalogue.isTable(parsedQuery.sortResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.sortRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    auto it = parsedQuery.sortBufferSize.begin();
    while (it != parsedQuery.sortBufferSize.end() && isdigit(*it))
        ++it;
    if (it != parsedQuery.sortBufferSize.end())
    {
        cout << "SEMANTIC ERROR: Buffer size needs to be an integer" << endl;
        return false;
    }
    int bufferSize = stoi(parsedQuery.sortBufferSize);
    if (bufferSize < 3)
    {
        cout << "SEMANTIC ERROR: Buffer size needs to be atleast 3" << endl;
        return false;
    }
    return true;
}

Table* sort_phase()
{
    Table *T = new Table(parsedQuery.sortRelationName);
    int columnIndex = T->getColumnIndex(parsedQuery.sortColumnName);
    Table *result = new Table(parsedQuery.sortResultRelationName, T->columns);
    vector<vector<int>> rows;
    Cursor cursor = T->getCursor();
    for(int a = 0; a < T->rowCount; a++)
    {
        vector<int> here = cursor.getNext();
        result->addRowSorted(here, rows, columnIndex);
    }
    return result;
}

void merge_phase(Table* result)
{
    
}

void executeSORT()
{
    logger.log("executeSORT");
    Table *result = sort_phase();
    merge_phase(result);
}