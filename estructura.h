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
int GetPj(int line);
int GetPoints(int line);
int GetPg(int line);
int GetPe(int line);
int GetDg(int line);
int GetGf(int line);
int GetGv(int line);
int GetPp(int line);
int GetGc(int line);
int GetGl(int line);

void CreateArray(char *fileName);
void SetColumnWidth(int *biggestName, char *stringFormat, char *type);
void w(char *outfileName, int *maxPrint,int (*ff2[])(int));
void n(int max, int *maxPrint);
void g();
void f(char *column, char *filter,int (*ff2[])(int));

int CompareGames(int datum1, int datum2);
int ComparePoints(int datum1, int datum2);
int CompareWins(int datum1, int datum2);
int CompareDraws(int datum1, int datum2);
int CompareGoalDif(int datum1, int datum2);
int CompareGoals(int datum1, int datum2);
int CompareVGoals(int datum1, int datum2);
int CompareLost(int datum1, int datum2);
int CompareAGoals(int datum1, int datum2);
int CompareLGoals(int datum1, int datum2);

void o(int argo, char **critFlagsO, int (*ff1[])(int,int));
int GetFlagIndex(char *flag);
int ReadFlags(int *flags, char *readedFlag);
void CheckFlagsParams(int index, int args);
void ResizeMatrix(data *dataMatrix, int newLength);

#endif
