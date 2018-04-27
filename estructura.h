#ifndef ESTRUCTURA_H_   
#define ESTRUCTURA_H_

//structs
typedef struct data{
	char * team;
	int pj;
	int points;
	int pg;
	int pe;
	int dg;
	int gf;
	int gv;
	int pp;
	int gc;
	int gl;

} data;


int CheckFileExist(char *fileName);
void SetRows(char *fileName, int *dim);
void SetColumns(char *fileName, int *dim);

void SetTeam(data dataLine, char *datum);
void SetPj(data dataLine, char *datum);
void SetPoints(data dataLine, char *datum);
void SetPg(data dataLine, char *datum);
void SetPe(data dataLine, char *datum);
void SetDg(data dataLine, char *datum);
void SetGf(data dataLine, char *datum);
void SetGv(data dataLine, char *datum);

//void CreateArray(char *fileName, char ***dataMatrix, int *dim, int* biggestName);
void CreateArray(char *fileName, data dataMatrix[]);
void SetColumnNames(char ***dataMatrix,int *dim);
void SetColumnWidth(int *biggestName, char *stringLength, char *stringFormat);
void w(data dataMatrix[], char *outfileName, int *dim, int *maxPrint,int *biggestName, int *maxPoint);
//void w(char ***dataMatrix, char *outfileName, int *dim, int *maxPrint,int *biggestName, int *maxPoint);
//void n(int max, int *maxPrint);
//void g(char ***dataMatrix, int *dim,  int *maxPoint);


#endif
