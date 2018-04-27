#ifndef ESTRUCTURA_H_   
#define ESTRUCTURA_H_

//structs
struct data{
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

};
typedef struct data data;

int CheckFileExist(char *fileName);
void SetRows(char *fileName, int *dim);
void SetColumns(char *fileName, int *dim);

void SetTeam(data dataMatrix[],int line, char *datum);
void SetPj(data dataMatrix[],int line, char *datum);
void SetPoints(data dataMatrix[],int line, char *datum);
void SetPg(data dataMatrix[],int line, char *datum);
void SetPe(data dataMatrix[],int line, char *datum);
void SetDg(data dataMatrix[],int line, char *datum);
void SetGf(data dataMatrix[],int line, char *datum);
void SetGv(data dataMatrix[],int line, char *datum);
int GetPj(data dataMatrix[],int line);
int GetPoints(data dataMatrix[],int line);
int GetPg(data dataMatrix[],int line);
int GetPe(data dataMatrix[],int line);
int GetDg(data dataMatrix[],int line);
int GetGf(data dataMatrix[],int line);
int GetGv(data dataMatrix[],int line);
int GetPp(data dataMatrix[],int line);
int GetGc(data dataMatrix[],int line);
int GetGl(data dataMatrix[],int line);


//void CreateArray(char *fileName, char ***dataMatrix, int *dim, int* biggestName);
void CreateArray(char *fileName, data dataMatrix[],void (*ff1[])(data[], int, char*));
void SetColumnNames(char ***dataMatrix,int *dim);
void SetColumnWidth(int *biggestName, char *stringFormat, char *type);
void w(data dataMatrix[], char *outfileName, int *dim, int *maxPrint,int (*ff2[])(data[], int));
//void w(char ***dataMatrix, char *outfileName, int *dim, int *maxPrint,int *biggestName, int *maxPoint);
//void n(int max, int *maxPrint);
//void g(char ***dataMatrix, int *dim,  int *maxPoint);


#endif
