#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "estructura2.h"
#define split ' '
#define FILE_LENGTH 30
#define largeColumn "%-25s"
#define shortColumn "%-4d"
#define colpj 1
#define colpt 2 
#define colpg 3
#define colpe 4
#define coldg 5
#define colgf 6
#define colgv 7
#define colpp 8
#define colgc 9
#define colgl 10


//Global var

int largestName=0;
int maxPoints=6;

int CheckFileExist(char *fileName){
	FILE *in;
	fileName[strlen(fileName)]=0;
	if (!(in=fopen(fileName,"r"))){
 		fprintf(stderr, "%s %s %s", "Error: Archivo", fileName, "no existe\n");
		return 0;
	}
	else{
		fclose(in);
		return 1;
	}
}

void SetRows(char *fileName, int *dim){
//Se lee el archivo por char de manera de tener el largo de las lineas y la cantidad de estas.	
//Esto se usará para definir el tamaño del arreglo de strings.
	FILE *in;
	char c;
	int max;
	in=fopen(fileName,"r");
	while((c=fgetc(in)) != EOF){
		if (c!='\n') max+=1;
		if (c=='\n'){
			if (max>dim[1]) dim[1]=max;
			max=0;	
 			dim[0]+=1;
		}
	}
	dim[0]+=1;
	fclose(in);
}

void SetColumns(char *fileName, int *dim){
	FILE *in;
	char c;
	dim[1]=0;
	in=fopen(fileName,"r");
	while((c=fgetc(in))!='\n'){
		if (c==split) dim[1]+=1;
	}
	dim[1]+=4;
	fclose(in);
}


//set dataMatrix values
void SetTeam(data dataMatrix[],int line, char *datum){
	strcpy(dataMatrix[line].team, datum);
}
void SetPj(data dataMatrix[],int line, char *datum){
	dataMatrix[line].pj=atoi(datum);
}
void SetPoints(data dataMatrix[],int line, char *datum){
	dataMatrix[line].points=atoi(datum);
	if (maxPoints<dataMatrix[line].points) maxPoints=dataMatrix[line].points;
}
void SetPg(data dataMatrix[],int line, char *datum){
	dataMatrix[line].pg=atoi(datum);
}
void SetPe(data dataMatrix[],int line, char *datum){
	dataMatrix[line].pe=atoi(datum);
}
void SetDg(data dataMatrix[],int line, char *datum){
	dataMatrix[line].dg=atoi(datum);
}
void SetGf(data dataMatrix[],int line, char *datum){
	dataMatrix[line].gf=atoi(datum);
}

void SetGv(data dataMatrix[],int line, char *datum){
	dataMatrix[line].gv=atoi(datum);
}

//Get dataMatrix int values
int GetPj(data dataMatrix[],int line){
	return dataMatrix[line].pj;
}
int GetPoints(data dataMatrix[],int line){
	return dataMatrix[line].points;
}
int GetPg(data dataMatrix[],int line){
	return dataMatrix[line].pg;
}
int GetPe(data dataMatrix[],int line){
	return dataMatrix[line].pe;
}
int GetDg(data dataMatrix[],int line){
	return dataMatrix[line].dg;
}
int GetGf(data dataMatrix[],int line){
	return dataMatrix[line].gf;
}
int GetGv(data dataMatrix[],int line){
	return dataMatrix[line].gv;
}
int GetPp(data dataMatrix[],int line){
	return dataMatrix[line].pp;
}
int GetGc(data dataMatrix[],int line){
	return dataMatrix[line].gc;
}
int GetGl(data dataMatrix[],int line){
	return dataMatrix[line].gl;
}





void CreateArray(char *fileName, data dataMatrix[], void (*ff1[])(data[], int, char*)){
	FILE *in;
	in=fopen(fileName,"r");
	//void (*ff1[])(data[], unsigned char, char*) = {SetTeam,SetPj,SetPoints,SetPg,SetPe,SetDg,SetGf,SetGv};
	char * datum = calloc(FILE_LENGTH, sizeof(char));
	char * clean = calloc(FILE_LENGTH, sizeof(char));
	char c;
	int count= 0;
	int line= 0;
	int column= 0;
	while ((c=fgetc(in))!=EOF){
		if (c>32){
 			datum[count]=c;
			count+=1;
		}
		if (c==split){
			datum[count+1]=0;
			ff1[column](dataMatrix,line, datum);
			if (count>largestName) largestName=count;
			fprintf(stderr, "%s\n",datum);			
			strcpy(datum,clean);
			column+=1;
			count=0;
		}
		if (c=='\n'){
			ff1[column](dataMatrix,line, datum);
			dataMatrix[line].pp=dataMatrix[line].pj-dataMatrix[line].pg-dataMatrix[line].pe;
			dataMatrix[line].gc=dataMatrix[line].gf-dataMatrix[line].dg;
			dataMatrix[line].gl=dataMatrix[line].gf-dataMatrix[line].gv;
			line+=1;
			column=0;
			count=0;
		}	
			
	}
	free(datum);
	free(clean);

}






void SetColumnNames(char ***dataMatrix,int *dim){
	char columnName[11][7]={"Equipo","pj", "puntos", "pg", "pe", "dg", "gf","gv", "pp", "gc", "gl"};
	for (int i=0; i<dim[1]; i++) for (int j=0; j<(int)strlen(columnName[i])+1; j++) dataMatrix[0][i][j]=columnName[i][j];
}

void SetColumnWidth(int *biggestName, char *stringFormat, char *type){
	char *stringLength = calloc(4,sizeof(char));	
	sprintf(stringLength,"%d",(*biggestName*(-1))-1);	
	strcpy(stringFormat,"%");	
	strcat(stringFormat,stringLength);
	strcat(stringFormat, type);
	free(stringLength);
}

void w(data dataMatrix[], char *outfileName, int *dim, int *maxPrint, int (*ff2[])(data[],int)){
	FILE *out;	
	if (outfileName[0]!=0) out=fopen(outfileName,"w");
	else out=stderr;
	char *clean = calloc(6,sizeof(char));	
	char *stringFormat = calloc(6,sizeof(char));
	for (int i=0; i<*maxPrint; i++){
			SetColumnWidth(&largestName, stringFormat,"s");
			fprintf(out, stringFormat, ff2[0](dataMatrix,i));
			strcpy(stringFormat,clean);
		for (int j=1; j<dim[1]; j++){
 			if (j==colpt){			
				SetColumnWidth(&maxPoints, stringFormat, "d"); 
				fprintf(out, stringFormat, ff2[j](dataMatrix,i));
				strcpy(stringFormat,clean);
			}
			else fprintf(out, shortColumn, ff2[j](dataMatrix,i));
			if (j<dim[1]-1) fprintf(out, "%c", ' ');
			else fprintf(out, "%c", '\n');
		}	 	
	}
	free(clean);
	free(stringFormat);
}

/*void n(int max, int *maxPrint){
	*maxPrint=max+1;	
}

void g(char ***dataMatrix, int *dim, int *maxPoint){
	
	for (int i=1; i<dim[0]; i++){
		int z=0;
		int size=atoi(dataMatrix[i][colpt]);
		if (*maxPoint<size) *maxPoint= size;
		if (size>0) {
			dataMatrix[i][colpt]=realloc(dataMatrix[i][colpt],size*sizeof(char)+1);		
 			while (z<size){
				dataMatrix[i][colpt][z]='0';
				z++;
			}
		}
		else dataMatrix[i][colpt][0]=0;	
	}	 	
}*/




//int main()
int main(int argc,char **argv)
{
	int nf0=2;
	void (*ff0[])(char*, int*) = {SetRows,SetColumns};
	void (*ff1[])(data[], int, char*) = {SetTeam,SetPj,SetPoints,SetPg,SetPe,SetDg,SetGf,SetGv};
	int (*ff2[])(data[], int) = {GetPj,GetPoints,GetPg,GetPe,GetDg,GetGf,GetGv,GetPp,GetGc,GetGl};
	char *fileName = calloc(FILE_LENGTH,sizeof(char));
	int dim[2]={0};
	char *outFileName = calloc(FILE_LENGTH,sizeof(char)); 
	int maxPrint=0;

	//int option=0;
	strcpy(fileName,argv[1]);
	if (!CheckFileExist(fileName)) return 1;
	for (int i=0; i<nf0;i++) ff0[i](fileName,dim);
	data dataMatrix[dim[0]];
	CreateArray(fileName, dataMatrix,ff1);
	maxPrint=dim[0];
	
	/*while ((option = getopt (argc, argv, "w:gn:")) != -1)
	    switch (option)
	      {
	      case 'w':
		wflag = 1;
		break;
	      case 'g':
		gflag = 1;
		break;
	      case 'n':
		nvalue = 1;
		break;
	      default:
		fprintf(stderr, "Error: Accion no conocoida --> %c\n", c);
		return 1;
	      }*/

			
	//g(dataMatrix,dim, &maxPoint);
	//n(5,&maxPrint);	
	if (argc>2) w(dataMatrix,argv[2],dim, &maxPrint,ff2);
	else w(dataMatrix,outFileName,dim, &maxPrint,ff2);
	
	free(fileName);
	free(outFileName);
	//free(dataMatrix);


	return 0;
}




