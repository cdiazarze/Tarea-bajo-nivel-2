#ifndef ESTRUCTURA_H_   
#define ESTRUCTURA_H_


int CheckFileExist(char *fileName);
void SetRows(char *fileName, int *dim);
void SetColumns(char *fileName, int *dim);
void CreateArray(char *fileName, char ***dataMatrix, int *dim, int* biggestName);
void SetColumnNames(char ***dataMatrix,int *dim);
void SetColumnWidth(int *biggestName, char *stringLength, char *stringFormat);
void w(char ***dataMatrix, char *outfileName, int *dim, int *maxPrint,int *biggestName, int *maxPoint);
void n(int max, int *maxPrint);
void g(char ***dataMatrix, int *dim,  int *maxPoint);


#endif
