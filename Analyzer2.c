#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "estructura.h"
#define split ' '
#define FILE_LENGTH 30
#define largeColumn "%-25s"
#define shortColumn "%-4s"
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
	dim[0]+=2;
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


void CreateArray(char *fileName, data dataMatrix,int *dim, int *biggestName){
	
	char c;
	FILE *in;
	int i=1;
	int j=0;
	int z=0;
	int size=0;
	in=fopen(fileName,"r");
	SetColumnNames(dataMatrix, dim);
	
	while ((c=fgetc(in))!=EOF){
		if (c>32){
 			dataMatrix[i][j][z]=c;
			size+=1;
			z+=1;
			
		}
		if (c==split){
			dataMatrix[i][j][z+1]=0;
			dataMatrix[i][j]=realloc(dataMatrix[i][j], (strlen(dataMatrix[i][j])+1)*sizeof(char));
			if (*biggestName<(int)strlen(dataMatrix[i][j])) *biggestName=(int)strlen(dataMatrix[i][j]);
			z=0;
			j+=1;
		}
		if (c=='\n'){
			sprintf(dataMatrix[i][colpp], "%d", atoi(dataMatrix[i][colpj])-atoi(dataMatrix[i][colpg])-atoi(dataMatrix[i][colpe]));
			sprintf(dataMatrix[i][colgc], "%d", atoi(dataMatrix[i][colgf])-atoi(dataMatrix[i][coldg]));
			sprintf(dataMatrix[i][colgl], "%d", atoi(dataMatrix[i][colgf])-atoi(dataMatrix[i][colgv]));
			z=0;
			j=0;
			i+=1;
		}	
	}
	if (dataMatrix[i][0][0]!=0){
		sprintf(dataMatrix[i][colpp], "%d", atoi(dataMatrix[i][colpj])-atoi(dataMatrix[i][colpg])-atoi(dataMatrix[i][colpe]));
		sprintf(dataMatrix[i][colgc], "%d", atoi(dataMatrix[i][colgf])-atoi(dataMatrix[i][coldg]));
		sprintf(dataMatrix[i][colgl], "%d", atoi(dataMatrix[i][colgf])-atoi(dataMatrix[i][colgv]));
	}
	
	fclose(in);
			
}

void SetColumnNames(char ***dataMatrix,int *dim){
	char columnName[11][7]={"Equipo","pj", "puntos", "pg", "pe", "dg", "gf","gv", "pp", "gc", "gl"};
	for (int i=0; i<dim[1]; i++) for (int j=0; j<(int)strlen(columnName[i])+1; j++) dataMatrix[0][i][j]=columnName[i][j];
}

void SetColumnWidth(int *biggestName, char *stringLength, char *stringFormat){
	sprintf(stringLength,"%d",(*biggestName*(-1))-1);	
	strcpy(stringFormat,"%");	
	strcat(stringFormat,stringLength);
	strcat(stringFormat,"s");
}

void w(char ***dataMatrix, char *outfileName, int *dim, int *maxPrint, int *biggestName,int *maxPoint){
	FILE *out;	
	if (outfileName[0]!=0) out=fopen(outfileName,"w");
	else out=stdout;

	for (int i=0; i<*maxPrint; i++){
		for (int j=0; j<dim[1]; j++){
 			if (j==0 || j==colpt){
				char *stringLength = calloc(4,sizeof(char));
				char *stringFormat = calloc(6,sizeof(char));
				if (j==0) SetColumnWidth(biggestName, stringLength, stringFormat); 			
				else SetColumnWidth(maxPoint, stringLength, stringFormat); 
				fprintf(out, stringFormat, dataMatrix[i][j]);
				free(stringLength);
				free(stringFormat);
			}
			else fprintf(out, shortColumn, dataMatrix[i][j]);
			if (j<dim[1]-1) fprintf(out, "%c", ' ');
			else fprintf(out, "%c", '\n');
		}	 	
	}
}

void n(int max, int *maxPrint){
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
}

typedef struct data{
	char * Equipo;
	unsigned char pj;
	unsigned char puntos;
	unsigned char pg;
	unsigned char pe;
	unsigned char dg;
	unsigned char gf;
	unsigned char gv;
	unsigned char pp;
	unsigned char gc;
	unsigned char gl;

} data;


//int main()
int main(int argc,char **argv)
{
	//int (*ff[])(int, int) = {-w};
	int nf0=2;
	void (*ff0[])(char*, int*) = {SetRows,SetColumns};
	char *fileName = calloc(FILE_LENGTH,sizeof(char));
	int dim[2]={0};
	char *outFileName = calloc(FILE_LENGTH,sizeof(char)); 
	int maxPrint=0;
	int biggestName=0;
	int maxPoint=6;

	//int option=0;
	strcpy(fileName,argv[1]);
	if (!CheckFileExist(fileName)) return 1;
	for (int i=0; i<nf0;i++) ff0[i](fileName,dim);

	/*//Incializar arreglo de datos
	char *** dataMatrix=calloc(dim[0]*dim[1]*FILE_LENGTH,sizeof(char));
	for (int i=0; i<dim[0]; i++){
		dataMatrix[i]=calloc(dim[1]*FILE_LENGTH,sizeof(char));
		for (int j=0; j<dim[1];j++) dataMatrix[i][j]=calloc(FILE_LENGTH,sizeof(char));
 	}*/

	data dataMatrix;

	CreateArray(fileName, dataMatrix,dim, &biggestName);
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
	if (argc>2) w(dataMatrix,argv[2],dim, &maxPrint, &biggestName,&maxPoint);
	else w(dataMatrix,outFileName,dim, &maxPrint,&biggestName, &maxPoint);
	
	free(fileName);
	free(outFileName);
	free(dataMatrix);


	return 0;
}




