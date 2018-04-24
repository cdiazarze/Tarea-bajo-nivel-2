#ifndef ESTRUCTURA_H_   
#define ESTRUCTURA_H_


//void CreateArray(char *fileName, int length)
int CheckFileExist(char *fileName);
void SetRows(char *fileName, int *dim);
void SetColumns(char *fileName, int *dim);
void CreateArray(char *fileName, char ***dataMatrix);

#endif
