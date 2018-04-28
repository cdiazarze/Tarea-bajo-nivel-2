#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "estructura2.h"
#define split ' '
#define nFlags 5
#define oFlag 0
#define fFlag 1
#define gFlag 2
#define nFlag 3
#define wFlag 4
#define FILE_LENGTH 30
#define shortColumnD "%-4d"



//Global var

int largestName=0;
int maxPoints=6;
int dim[2]={0};


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

void SetRows(char *fileName){
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

void SetColumns(char *fileName){
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

int GetColumnIndex(char *column){
	char columnName[][7]={"pj", "puntos", "pg", "pe", "dg", "gf","gv", "pp", "gc", "gl"};
	for (int i=0; i<dim[1]-1;i++) if(!strcmp(columnName[i],column)) return i;
	return -1; 
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





void CreateArray(char *fileName, data dataMatrix[]){
	FILE *in;
	in=fopen(fileName,"r");
	char *team = calloc(FILE_LENGTH, sizeof(char));
	int pj, points, pg,pe,dg,gf,gv;
	int line= 0;
	while (fscanf(in, "%s %d %d %d %d %d %d %d", team, &pj, &points, &pg, &pe, &dg , &gf, &gv)!=EOF)	
	{		
		strcpy(dataMatrix[line].team,team);
		if ((int)strlen(dataMatrix[line].team)>largestName) largestName=(int)strlen(dataMatrix[line].team);
		dataMatrix[line].pj=pj;		
		dataMatrix[line].points=points;
		dataMatrix[line].pg=pg;
		dataMatrix[line].pe=pe;
		dataMatrix[line].dg=dg;
		dataMatrix[line].gf=gf;
		dataMatrix[line].gv=gv;
		dataMatrix[line].pp=dataMatrix[line].pj-dataMatrix[line].pg-dataMatrix[line].pe;
		dataMatrix[line].gc=dataMatrix[line].gf-dataMatrix[line].dg;
		dataMatrix[line].gl=dataMatrix[line].gf-dataMatrix[line].gv;
		dataMatrix[line].gbool=0;
		dataMatrix[line].filterbool=1;
		memset(team,0,FILE_LENGTH*sizeof(char));
		line+=1;
	}
	free(team);
	fclose(in);
}



void SetColumnWidth(int *biggestName, char *stringFormat, char *type){
	char *stringLength = calloc(4,sizeof(char));	
	sprintf(stringLength,"%d",(*biggestName*(-1))-1);	
	strcpy(stringFormat,"%");	
	strcat(stringFormat,stringLength);
	strcat(stringFormat, type);
	free(stringLength);
}

void w(data dataMatrix[], char *outfileName, int *maxPrint, int (*ff2[])(data[],int)){
	FILE *out;	
	char columnName[][7]={"Equipo","pj", "puntos", "pg", "pe", "dg", "gf","gv", "pp", "gc", "gl"};
	int shortColumn=3;	
	if (outfileName[0]!=0) out=fopen(outfileName,"w");
	else out=stdout;
	char *stringFormat = calloc(6,sizeof(char));
	int exist=0;
	//Imprimir nombres de columnas
	for (int i=0; i<dim[1]; i++){
		if (i==0) SetColumnWidth(&largestName, stringFormat,"s");
		else if (i==2) SetColumnWidth(&maxPoints, stringFormat, "s");		
		else SetColumnWidth(&shortColumn, stringFormat,"s");		
		fprintf(out, stringFormat, columnName[i]);
		fprintf(out, "%c", ' ');		
		if (i==dim[1]-1) fprintf(out, "%c",'\n');
	}

		
	for (int i=0; i<*maxPrint; i++){
		if (dataMatrix[i].filterbool){ 
			exist=1;
			SetColumnWidth(&largestName, stringFormat,"s");			
			fprintf(out, stringFormat, dataMatrix[i].team);
			fprintf(out, "%c", ' ');
			memset(stringFormat,0,6*sizeof(char));
			for (int j=0; j<dim[1]-1; j++){
	 			if (j==1){	
					if (!dataMatrix[i].gbool){				
						SetColumnWidth(&maxPoints, stringFormat, "d"); 
						fprintf(out, stringFormat, ff2[j](dataMatrix,i));
					}
					else {
						SetColumnWidth(&maxPoints, stringFormat, "s"); 
						fprintf(out, stringFormat, dataMatrix[i].gpoints);
					}
				}
				else fprintf(out, shortColumnD, ff2[j](dataMatrix,i));
				if (j<dim[1]-2) fprintf(out, "%c", ' ');
				else fprintf(out, "%c", '\n');
				memset(stringFormat,0,6*sizeof(char));
			}
		}	 	
	}
	if (!exist) fprintf(out, "No existe equipo que cumpla los criterios solicitados\n");
	free(stringFormat);
	fclose(out);
}

void n(int max, int *maxPrint){
	if (max<=dim[0]) *maxPrint=max;	
}

void g(data dataMatrix[]){
	for (int i=0; i<dim[0]; i++){
		dataMatrix[i].gbool=1;
		if (maxPoints<dataMatrix[i].points) maxPoints=dataMatrix[i].points;		
	}

	for (int i=0; i<dim[0]; i++){
		dataMatrix[i].gpoints=calloc(maxPoints+1, sizeof(char));
		for (int j=0; j<dataMatrix[i].points; j++) dataMatrix[i].gpoints[j]='0';	
	}	 	
}

void f(data dataMatrix[], char *column, char *filter,int (*ff2[])(data[],int)){
	char *cnum=calloc((int)strlen(filter)+1, sizeof(char));	
	char *crit=calloc((int)strlen(filter)+1, sizeof(char));
	int icrit=0;
	int icnum=0;
	int colnum=GetColumnIndex(column);
	if (colnum==-1) {
		fprintf(stderr, "Columna ingresada: %s es erronea. Se termina el programa.\n",column);
		exit(colnum);
	}
	for (int i=0; i<(int)strlen(filter);i++){ 
		if(filter[i]=='<' || filter[i]=='>' || filter[i]=='=') crit[icrit++]=filter[i];
		else cnum[icnum++]=filter[i];
	}
	int num= atoi(cnum);
	
	for (int i=0; i<dim[0]; i++){
		if(!strcmp(crit,"<")){
			if (!(ff2[colnum](dataMatrix,i)<num))  dataMatrix[i].filterbool=0;
		}
		else if(!strcmp(crit,"<=")){
			if (!(ff2[colnum](dataMatrix,i)<=num)) dataMatrix[i].filterbool=0;
		}
		else if(!strcmp(crit,">")){
			if (!(ff2[colnum](dataMatrix,i)>num)) dataMatrix[i].filterbool=0;
		}		
		else if(!strcmp(crit,">=")){
			if (!(ff2[colnum](dataMatrix,i)>=num)) dataMatrix[i].filterbool=0;
		}		
		else{
		 	if (!(ff2[colnum](dataMatrix,i)==num)) dataMatrix[i].filterbool=0;
		}
	}	


	free(cnum);
	free(crit);
}



int main(int argc,char **argv)
{
	int nf0=2;
	void (*ff0[])(char*) = {SetRows,SetColumns};
	int (*ff2[])(data[], int) = {GetPj,GetPoints,GetPg,GetPe,GetDg,GetGf,GetGv,GetPp,GetGc,GetGl};
	char *fileName = calloc(FILE_LENGTH,sizeof(char));
	char *outFileName = calloc(FILE_LENGTH,sizeof(char)); 
	int maxPrint=0;
	int flags[nFlags] ={0};
	char *** critFlags=calloc(nFlags*1*FILE_LENGTH,sizeof(char));
	for (int i=0; i<nFlags; i++){
		critFlags[i]=calloc(1*FILE_LENGTH,sizeof(char));
 	}
	
	strcpy(fileName,argv[1]);
	if (!CheckFileExist(fileName)) return 1;
	for (int i=0; i<nf0;i++) ff0[i](fileName);
	data dataMatrix[dim[0]];
	for(int i = 0; i < dim[0]; i++)
		dataMatrix[i].team = calloc(FILE_LENGTH, sizeof(char));
	CreateArray(fileName, dataMatrix);
	maxPrint=dim[0];
	int option=0;	
	int argo=0;
	int arg=0;
	while ((option = getopt (argc, argv, "o:f:gn:w:")) != -1){
		switch (option)
	    	{
			case 'o':
				flags[oFlag] = 1;		
				optind--;
				arg=0;
				for( ;optind < argc && *argv[optind] != '-'; optind++){
					critFlags[oFlag]=realloc(critFlags[oFlag],(arg+1)*FILE_LENGTH*sizeof(char));
					critFlags[oFlag][arg]=calloc(FILE_LENGTH,sizeof(char));		
					strcpy(critFlags[oFlag][arg],argv[optind]);
					arg+=1;       		 
				}
				for (int i=0; i<arg; i++) critFlags[oFlag][i]=realloc(critFlags[oFlag][i],(int)strlen(critFlags[oFlag][i])*sizeof(char)); 
				argo=arg;		
				break;
			case 'f':
				flags[fFlag] = 1;
				arg=0;		
				optind--;
				for( ;optind < argc && argv[optind][0] != '-'; optind++){
					critFlags[fFlag]=realloc(critFlags[fFlag],(arg+1)*FILE_LENGTH*sizeof(char));
					critFlags[fFlag][arg]=calloc(FILE_LENGTH,sizeof(char));		
					strcpy(critFlags[fFlag][arg],argv[optind]);
					arg+=1;       		 
				}
				for (int i=0; i<arg; i++) critFlags[fFlag][i]=realloc(critFlags[fFlag][i],(int)strlen(critFlags[fFlag][i])*sizeof(char)); 		
				break;
			case 'g':
				flags[gFlag] = 1;
				break;
			case 'n':
				flags[nFlag] = 1;
				critFlags[nFlag][0]=calloc(FILE_LENGTH,sizeof(char));
				strcpy(critFlags[nFlag][0],optarg);
				critFlags[nFlag][0]=realloc(critFlags[nFlag][0],(int)strlen(critFlags[nFlag][0])*sizeof(char));
				break;
			case 'w':
				flags[wFlag] = 1;
				strcpy(outFileName,optarg);
				break;
			default:
				fprintf(stderr, "Error: Accion no conocida --> %c\n", optopt);
				return 1;
		}
	}
	  

	//if (flags[oFlag])
	if (flags[fFlag]) f(dataMatrix,critFlags[fFlag][0],critFlags[fFlag][1],ff2);
	if (flags[gFlag]) g(dataMatrix);
	if (flags[nFlag]) n(atoi(critFlags[nFlag][0]),&maxPrint);
	w(dataMatrix,outFileName, &maxPrint,ff2);
	if(argo>1) printf("hola");
	
	free(fileName);
	free(outFileName);
	free(critFlags);
	for(int i = 0; i < dim[0]; i++) {
		free(dataMatrix[i].team);
		if (flags[gFlag]) free(dataMatrix[i].gpoints);
	}

	return 0;
}




