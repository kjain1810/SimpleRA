#include "global.h"

bool syntacticParseGROUP()
{
    logger.log("syntacticParseGROUP");
    if(tokenizedQuery.size() != 9)
    {
        cout << "SYNTAC ERROR" << endl;
        cout << "Size: " << tokenizedQuery.size() << endl;
        return false;
    }
    if(tokenizedQuery[8].length() <= 5 ||  tokenizedQuery[8][3] != '(' || tokenizedQuery[8][tokenizedQuery[8].length() - 1] != ')')
    {
        cout << "SYNTAC ERROR" << endl;
        cout << "Invalid grouping style" << endl;
        return false;
    }
    parsedQuery.queryType = GROUP;
    parsedQuery.groupResultRelationName = tokenizedQuery[0];
    parsedQuery.groupAttribute = tokenizedQuery[4];
    parsedQuery.groupRelationName = tokenizedQuery[6];
    parsedQuery.groupType = "";
    parsedQuery.groupType += tokenizedQuery[8][0];
    parsedQuery.groupType += tokenizedQuery[8][1];
    parsedQuery.groupType += tokenizedQuery[8][2];
    parsedQuery.groupApplyAttribute = "";
    for(int a = 4; a < tokenizedQuery[8].length() - 1; a++)
        parsedQuery.groupApplyAttribute += tokenizedQuery[8][a];
    cout << "groupResultRelationName: " << parsedQuery.groupResultRelationName << "\n";
    cout << "groupAttribute: " << parsedQuery.groupAttribute << "\n";
    cout << "groupRelationName: " << parsedQuery.groupRelationName << "\n";
    cout << "groupType: " << parsedQuery.groupType << "\n";
    cout << "groupApplyAttribute: " << parsedQuery.groupApplyAttribute << "\n";
    return true;
}

bool semanticParseGROUP()
{
    logger.log("semanticParseGROUP");
    if (tableCatalogue.isTable(parsedQuery.groupResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }
    if (tableCatalogue.isTable(parsedQuery.groupRelationName) == false)
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    if(tableCatalogue.isColumnFromTable(parsedQuery.groupAttribute, parsedQuery.groupRelationName) == false)
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    if(tableCatalogue.isColumnFromTable(parsedQuery.groupApplyAttribute, parsedQuery.groupRelationName) == false)
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    if(parsedQuery.groupType != "MIN" && parsedQuery.groupType != "MAX" && parsedQuery.groupType != "SUM" && parsedQuery.groupType != "AVG")
    {
        cout << "SEMANTIC ERROR: Group type does not exist" << endl;
        return false;    
    }
    return true;
}

void executeGROUP()
{
    logger.log("executeGROUP");
    Table *T1 = tableCatalogue.getTable(parsedQuery.groupRelationName);
    int columnIndex = T1->getColumnIndex(parsedQuery.groupAttribute);
    int columnIndexToApplyOn = T1->getColumnIndex(parsedQuery.groupApplyAttribute);
    string type = parsedQuery.groupType;
    vector<string> columns;
    columns.push_back(parsedQuery.groupAttribute);
    columns.push_back(type + parsedQuery.groupApplyAttribute);
    Table *result = new Table(parsedQuery.groupResultRelationName, columns);
    unordered_set<int> vals = T1->getDistinctValuesOfColumn(columnIndex);
    map<int, int> result_values;
    map<int, int> total_count;
    for(auto it = vals.begin(); it != vals.end(); it++)
    {
        if(type == "MAX" || type == "AVG" || type == "SUM")
            result_values[*it] = 0;
        else
            result_values[*it] = INT_MAX;
        if(type == "AVG")
            total_count[*it] = 0;
    }
    Cursor cursor = T1->getCursor();
    for(int a = 0; a < T1->rowCount; a++)
    {
        vector<int> row = cursor.getNext();
        if(type == "SUM")
            result_values[row[columnIndex]] += row[columnIndexToApplyOn];
        else if(type == "AVG")
        {
            result_values[row[columnIndex]] += row[columnIndexToApplyOn];
            total_count[row[columnIndex]]++;
        }
        else if(type == "MIN")
            result_values[row[columnIndex]] = min(result_values[row[columnIndex]], row[columnIndexToApplyOn]);
        else if(type == "MAX")
            result_values[row[columnIndex]] = max(result_values[row[columnIndex]], row[columnIndexToApplyOn]);
    }
    vector<vector<int>> rows;
    for(auto it = result_values.begin(); it != result_values.end(); it++)
    {
        if(type == "AVG")
            it->second /= total_count[it->first];
        vector<int> newRow;
        newRow.push_back(it->first);
        newRow.push_back(it->second);
        result->addRow(newRow, rows);
    }
    if(rows.size())
        result->addPage(rows);
    tableCatalogue.insertTable(result);
}