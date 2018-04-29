#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
char *allowedFlags[]={"-o","-f","-g","-n","-w"};
int minFlagsParams[nFlags]={2, 2, 0, 1, 1};
int maxFlagsParams[nFlags]={1000, 2, 0, 1, 1};
data *dataMatrix;

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
int GetPj(int line){
	return dataMatrix[line].pj;
}
int GetPoints(int line){
	return dataMatrix[line].points;
}
int GetPg(int line){
	return dataMatrix[line].pg;
}
int GetPe(int line){
	return dataMatrix[line].pe;
}
int GetDg(int line){
	return dataMatrix[line].dg;
}
int GetGf(int line){
	return dataMatrix[line].gf;
}
int GetGv(int line){
	return dataMatrix[line].gv;
}
int GetPp(int line){
	return dataMatrix[line].pp;
}
int GetGc(int line){
	return dataMatrix[line].gc;
}
int GetGl(int line){
	return dataMatrix[line].gl;
}





void CreateArray(char *fileName){
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

void w(char *outfileName, int *maxPrint, int (*ff2[])(int)){
	FILE *out;	
	char columnName[][7]={"Equipo","pj", "puntos", "pg", "pe", "dg", "gf","gv", "pp", "gc", "gl"};
	int shortColumn=3;	
	if (outfileName[0]!=0) out=fopen(outfileName,"w");
	else out=stdout;
	char *stringFormat = calloc(6,sizeof(char));
	int exist=0;
	int count=0;
	//Imprimir nombres de columnas
	for (int i=0; i<dim[1]; i++){
		if (i==0) SetColumnWidth(&largestName, stringFormat,"s");
		else if (i==2) SetColumnWidth(&maxPoints, stringFormat, "s");		
		else SetColumnWidth(&shortColumn, stringFormat,"s");		
		fprintf(out, stringFormat, columnName[i]);
		fprintf(out, "%c", ' ');		
		if (i==dim[1]-1) fprintf(out, "%c",'\n');
	}

		
	for (int i=0; i<dim[0]; i++){
		if (count<*maxPrint){
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
							fprintf(out, stringFormat, ff2[j](i));
						}
						else {
							SetColumnWidth(&maxPoints, stringFormat, "s"); 
							fprintf(out, stringFormat, dataMatrix[i].gpoints);
						}
					}
					else fprintf(out, shortColumnD, ff2[j](i));
					if (j<dim[1]-2) fprintf(out, "%c", ' ');
					else fprintf(out, "%c", '\n');
					memset(stringFormat,0,6*sizeof(char));
				}
				count+=1;
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

void g(){
	for (int i=0; i<dim[0]; i++){
		dataMatrix[i].gbool=1;
		if (maxPoints<dataMatrix[i].points) maxPoints=dataMatrix[i].points;		
	}

	for (int i=0; i<dim[0]; i++){
		dataMatrix[i].gpoints=calloc(maxPoints+1, sizeof(char));
		for (int j=0; j<dataMatrix[i].points; j++) dataMatrix[i].gpoints[j]='0';	
	}	 	
}

void f(char *column, char *filter,int (*ff2[])(int)){
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
			if (!(ff2[colnum](i)<num))  dataMatrix[i].filterbool=0;
		}
		else if(!strcmp(crit,"<=")){
			if (!(ff2[colnum](i)<=num)) dataMatrix[i].filterbool=0;
		}
		else if(!strcmp(crit,">")){
			if (!(ff2[colnum](i)>num)) dataMatrix[i].filterbool=0;
		}		
		else if(!strcmp(crit,">=")){
			if (!(ff2[colnum](i)>=num)) dataMatrix[i].filterbool=0;
		}		
		else{
		 	if (!(ff2[colnum](i)==num)) dataMatrix[i].filterbool=0;
		}
	}	


	free(cnum);
	free(crit);
}

int CompareGames(int datum1, int datum2){
	if (dataMatrix[datum1].pj>dataMatrix[datum2].pj) return datum1;
	if (dataMatrix[datum1].pj<dataMatrix[datum2].pj) return datum2;
	return -1; 
}
int ComparePoints(int datum1, int datum2){
	if (dataMatrix[datum1].points>dataMatrix[datum2].points) return datum1;
	if (dataMatrix[datum1].points<dataMatrix[datum2].points) return datum2;
	return -1; 
}
int CompareWins(int datum1, int datum2){
	if (dataMatrix[datum1].pg>dataMatrix[datum2].pg) return datum1;
	if (dataMatrix[datum1].pg<dataMatrix[datum2].pg) return datum2;
	return -1; 
}
int CompareDraws(int datum1, int datum2){
	if (dataMatrix[datum1].pe>dataMatrix[datum2].pe) return datum1;
	if (dataMatrix[datum1].pe<dataMatrix[datum2].pe) return datum2;
	return -1; 
}
int CompareGoalDif(int datum1, int datum2){
	if (dataMatrix[datum1].dg>dataMatrix[datum2].dg) return datum1;
	if (dataMatrix[datum1].dg<dataMatrix[datum2].dg) return datum2;
	return -1; 
}
int CompareGoals(int datum1, int datum2){
	if (dataMatrix[datum1].gf>dataMatrix[datum2].gf) return datum1;
	if (dataMatrix[datum1].gf<dataMatrix[datum2].gf) return datum2;
	return -1; 
}
int CompareVGoals(int datum1, int datum2){
	if (dataMatrix[datum1].gv>dataMatrix[datum2].gv) return datum1;
	if (dataMatrix[datum1].gv<dataMatrix[datum2].gv) return datum2;
	return -1; 
}
int CompareLost(int datum1, int datum2){
	if (dataMatrix[datum1].pp>dataMatrix[datum2].pp) return datum1;
	if (dataMatrix[datum1].pp<dataMatrix[datum2].pp) return datum2;
	return -1; 
}
int CompareAGoals(int datum1, int datum2){
	if (dataMatrix[datum1].gc>dataMatrix[datum2].gc) return datum1;
	if (dataMatrix[datum1].gc<dataMatrix[datum2].gc) return datum2;
	return -1; 
}
int CompareLGoals(int datum1, int datum2){
	if (dataMatrix[datum1].gl>dataMatrix[datum2].gl) return datum1;
	if (dataMatrix[datum1].gl<dataMatrix[datum2].gl) return datum2;
	return -1; 
}


void ChangeRow(data *datum1, data *datum2, int *swap){
	data temp=*datum1;
	*datum1=*datum2;
	*datum2=temp;
	*swap=1;
}



void o(int argo, char **critFlagsO, int (*ff1[])(int,int)){
	int swap=1;
	int column=0;
	while (swap){
		swap=0;		
		for (int i=1; i<dim[0];i++){
			for(int j=0; j<argo;j+=2){
				column=GetColumnIndex(critFlagsO[j]);
				fprintf(stderr,"%s", critFlagsO[j+1]);
				if (ff1[column](i-1,i)==(i-1) && !strcmp(critFlagsO[j+1],"asc")) ChangeRow(&dataMatrix[i-1],&dataMatrix[i],&swap);
				if (ff1[column](i-1,i)==(i) && !strcmp(critFlagsO[j+1],"desc")) ChangeRow(&dataMatrix[i-1],&dataMatrix[i],&swap);				
				if (swap) break;
			}
			if (swap) break;
		}			
	}
}


//Read flags

int GetFlagIndex(char *flag){
	for (int i=0; i<nFlags; i++) if(!strcmp(allowedFlags[i],flag)) return i;
	fprintf(stderr, "ERROR Flag %s --> no conocido. Finaliza el programa\n", flag);
	return -1;
}

int ReadFlags(int *flags, char *readedFlag){
	int index = GetFlagIndex(readedFlag);
	if (index<0) exit(index);
	else{
		flags[index]=1;
	}
	return index;
}
		
void CheckFlagsParams(int index, int args){
	if (args<minFlagsParams[index] || args>maxFlagsParams[index]){
		if(minFlagsParams[index]==maxFlagsParams[index]){
			fprintf(stderr, "ERROR: Flag %s requiere %d argumentos. Se entregaron %d. Finaliza el programa\n", allowedFlags[index], minFlagsParams[index], args);
			exit(-1);
		}
		else{ 
			fprintf(stderr, "ERROR: Flag %s requiere de %d a %d argumentos. Se entregaron %d. Finaliza el programa\n", allowedFlags[index],minFlagsParams[index],maxFlagsParams[index], args);
			exit(-1);
		}
	}
	if (index==oFlag && args%2!=0){
		fprintf(stderr, "ERROR: Flag %s requiere tuplas{columna, criterio} de argumentos. Se entrego numero impar de %d. Finaliza el programa\n", allowedFlags[index], args);
			exit(-1);
	} 
}




int main(int argc,char **argv)
{
	int nf0=2;
	void (*ff0[])(char*) = {SetRows,SetColumns};
	int (*ff1[])(int,int) = {CompareGames,ComparePoints,CompareWins,CompareDraws,CompareGoalDif,CompareGoals,CompareVGoals,CompareLost,CompareAGoals,CompareLGoals};
	int (*ff2[])(int) = {GetPj,GetPoints,GetPg,GetPe,GetDg,GetGf,GetGv,GetPp,GetGc,GetGl};
	char *fileName = calloc(FILE_LENGTH,sizeof(char));
	char *outFileName = calloc(FILE_LENGTH,sizeof(char)); 
	int maxPrint=0;
	int flags[nFlags] ={0};
	int argFlags[nFlags] ={0};
	char *** critFlags=calloc(nFlags*1*FILE_LENGTH,sizeof(char));
	for (int i=0; i<nFlags; i++){
		critFlags[i]=calloc(1*FILE_LENGTH,sizeof(char));
 	}
	
	strcpy(fileName,argv[1]);
	if (!CheckFileExist(fileName)) return 1;
	
	for (int i=0; i<nf0;i++) ff0[i](fileName);
	dataMatrix=malloc(dim[0]*sizeof(data));
	for(int i = 0; i < dim[0]; i++)
		dataMatrix[i].team = calloc(FILE_LENGTH, sizeof(char));
	CreateArray(fileName);
	maxPrint=dim[0];	
	

	//lectura de flafs
	int arg=0;
	int index=2; //index para leer los flags
	int index2=0;
	int flagIndex=0;
	while(index<argc){
		//Se busca un flag. Antes de que aparezca el primer flag, las opciones seran ignoradas.
		if (argv[index][0]=='-'){	
			flagIndex=ReadFlags(flags, argv[index]);
			arg=0;
			index2=index+1;
			for(; index2<argc && argv[index2][0]!='-'; index2++){
				critFlags[flagIndex]=realloc(critFlags[flagIndex],(arg+1)*FILE_LENGTH*sizeof(char));
				critFlags[flagIndex][arg]=calloc(FILE_LENGTH,sizeof(char));		
				strcpy(critFlags[flagIndex][arg],argv[index2]);
				arg+=1;
			}
			for (int i=0; i<arg; i++) 
				critFlags[flagIndex][i]=realloc(critFlags[flagIndex][i],(int)strlen(critFlags[flagIndex][i])*sizeof(char));
			CheckFlagsParams(flagIndex,arg);
			argFlags[flagIndex]=arg;
			index=index2;
		}

	}

	  
//Ejecucion del programa
	//Default order
	char *defaultO[]={"puntos","desc"};
	o(2, defaultO,ff1);
	if (flags[oFlag]) o(argFlags[oFlag], critFlags[oFlag],ff1);
	if (flags[fFlag]) f(critFlags[fFlag][0],critFlags[fFlag][1],ff2);
	if (flags[gFlag]) g();
	if (flags[nFlag]) n(atoi(critFlags[nFlag][0]),&maxPrint);	
	if (flags[wFlag]) strcpy(outFileName,critFlags[wFlag][0]); 	
	w(outFileName, &maxPrint,ff2);

//Liberar los punteros declarados	
	free(fileName);
	free(outFileName);
	free(critFlags);
	free(dataMatrix);
	return 0;
}




