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
    parsedQuery.queryType = GROUP;
    parsedQuery.groupResultRelationName = tokenizedQuery[0];
    parsedQuery.groupAttribute = tokenizedQuery[4];
    parsedQuery.groupRelationName = tokenizedQuery[6];
    parsedQuery.groupType = tokenizedQuery[8];
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
}