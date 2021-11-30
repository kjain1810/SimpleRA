#ifndef executor_H
#define executor_H
#include"semanticParser.h"

void executeCommand();

void executeCLEAR();
void executeCROSS();
void executeDISTINCT();
void executeEXPORT();
void executeINDEX();
void executeJOIN();
void nestedJOINpartition(Table* T1, Table* T2, Table* result, int part, int columnIndexT1, int columnIndexT2, int partSizeT1, int partSizeT2, int bufferSize, bool swap, vector<vector<int>> &rows);
void executeLIST();
void executeLOAD();
void executePRINT();
void executePROJECTION();
void executeRENAME();
void executeSELECTION();
Table *sort_phase();
void merge_phase(Table* result);
void executeSORT();
void executeSOURCE();
void executeGROUP();

bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator);
void printRowCount(int rowCount);
#endif
