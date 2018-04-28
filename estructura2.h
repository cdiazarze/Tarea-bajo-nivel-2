#ifndef ESTRUCTURA_H_   
#define ESTRUCTURA_H_

//structs
struct data{
	char* team;
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
	char* gpoints;
	int gbool;
	int filterbool;

};


typedef struct data data;



int CheckFileExist(char *fileName);
void SetRows(char *fileName);
void SetColumns(char *fileName);
int GetColumnIndex(char *column);
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

void CreateArray(char *fileName, data dataMatrix[]);
void SetColumnWidth(int *biggestName, char *stringFormat, char *type);
void w(data dataMatrix[], char *outfileName, int *maxPrint,int (*ff2[])(data[], int));
void n(int max, int *maxPrint);
void g(data dataMatrix[]);
void f(data dataMatrix[], char *column, char *filter,int (*ff2[])(data[],int));


#endif
