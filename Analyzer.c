#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estructura.h"
#define split ' '
#define FILE_LENGTH 30
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

/*void cifrado_desplazamiento(int numero, char *linea)
{
	for (int i=0; linea[i]!='\0';i++) linea[i]+=numero;	
}




void -w(FILE *in,FILE *out, char *oracion, char *codificado, char *nombre)
{

	char limpio[MAX_LENGTH];
	for (int i=0; strlen(limpio); i++) limpio[i]='\0';	
	if (!(in=fopen(nombre,"r"))) fprintf(stderr, "%s %s %s", "Error: Archivo", nombre, "no existe\n") ;
	archivo=fopen("encriptado.txt","w");
	strcpy(oracion,limpio);
	strcpy(codificado,limpio);
	while(fgets(oracion, MAX_LENGTH-1, in)!=0) 
	{
		strcpy(codificado,oracion);
		Invertir_Caracteres(codificado);			
		cifrado_desplazamiento(numero_desplazamiento,codificado);
		negacion(codificado); //Negacion
		XOR(codificado,CaractXor); //XOR
		fprintf(archivo, "%s", codificado);
		strcpy(oracion,limpio);
		strcpy(codificado,limpio);
		
	}
	fclose(archivo);
	fclose(in);	
}*/

int CheckFileExist(char *fileName){
	FILE *in;
	fileName[strlen(fileName)-1]=0;
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


void CreateArray(char *fileName, char ***dataMatrix,int *dim){
	
	char c;
	FILE *in;
	int i=1;
	int j=0;
	int z=0;
	int size=0;
	in=fopen(fileName,"r");
	SetColumnNames(dataMatrix, dim);
	while ((c=fgetc(in))!=EOF){
		if (c!=split && c!='\n'){
 			dataMatrix[i][j][z]=c;
			size+=1;
			z+=1;
		}
		if (c==split){
			dataMatrix[i][j][z+1]=0;
			dataMatrix[i][j]=realloc(dataMatrix[i][j], (strlen(dataMatrix[i][j])+1)*sizeof(char));
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
	fclose(in);
			
}

void SetColumnNames(char ***dataMatrix,int *dim){
	char columnName[11][7]={"Equipo","pj", "puntos", "pg", "pe", "dg", "gf","gv", "pp", "gc", "gl"};
	for (int i=0; i<dim[1]; i++) for (int j=0; j<(int)strlen(columnName[i])+1; j++) dataMatrix[0][i][j]=columnName[i][j];
}

void w(char ***dataMatrix, char *outfileName, int *dim){
	FILE *out;
	if (outfileName[0]!=0) out=fopen(outfileName,"w");
	else out=stdout;
	for (int i=0; i<dim[0]; i++){
		for (int j=0; j<dim[1]; j++){
 			fprintf(out, "%15s", dataMatrix[i][j]);
			if (j<dim[1]-1) fprintf(out, "%c", ' ');
			else fprintf(out, "%c", '\n');
		}	 	
	}
}


//int main()
int main(int argc,char **argv)
{
	//int (*ff[])(int, int) = {-w};
	int nf0=2;
	void (*ff0[])(char*, int*) = {SetRows,SetColumns};
	char *fileName = malloc(FILE_LENGTH*sizeof(char));
	int dim[2]={0};
	char *outfileName = calloc(FILE_LENGTH,sizeof(char)); 

	

	printf ("\nIngrese nombre de archivo a analizar:\n");
	fgets(fileName, FILE_LENGTH-1, stdin);
	if (!CheckFileExist(fileName)) return 1;
	for (int i=0; i<nf0;i++) ff0[i](fileName,dim);

	//Incializar arreglo de datos
	char *** dataMatrix=malloc(dim[0]*dim[1]*FILE_LENGTH*sizeof(char));
	for (int i=0; i<dim[0]; i++){
		dataMatrix[i]=malloc(dim[1]*FILE_LENGTH*sizeof(char));
		for (int j=0; j<dim[1];j++) dataMatrix[i][j]=malloc(FILE_LENGTH*sizeof(char));
 	}
	CreateArray(fileName, dataMatrix,dim);
	if (argc>2) w(dataMatrix,argv[1],dim);
	else w(dataMatrix,outfileName,dim);

	free(fileName);
	free(dataMatrix);

/*

	
	

	int numero_desplazamiento;

	char opcion;
	char desplazamiento[10];
	
	char CaractXor;
	int revision=1;
	int revision2=1;
	char oracion[MAX_LENGTH];
	char codificado[MAX_LENGTH];
	char nombre[MAX_LENGTH];



	printf("Elija opcion para encriptar o desencriptar:\n (1) Encriptar\n (2) Desencriptar\n");	
	while (revision==1)
	{	
		scanf(" %c%*c", &opcion);
		if ((int)opcion != 49 && (int)opcion != 50) printf("Favor ingresar 1 o 2, el caracter %c no es válido:\n", opcion);
		else revision=0;
	}

	//Solicitud de archivo
	if ((int)opcion == 49) printf ("\nEncriptado:\nIngrese nombre de archivo a encriptar:\n");
	if ((int)opcion == 50) printf ("\nDesencriptado:\nIngrese nombre de archivo a desencriptar:\n");
	fgets(nombre, MAX_LENGTH, stdin);
	nombre[strlen(nombre)-1]=0;
	//Parametros para encriptado
	printf ("\nIngrese numero entero positivo para desplazamiento ASCII:\n");
	revision2=1;
	while (revision2==1)
	{	
		revision2=0;
		fgets(desplazamiento, sizeof desplazamiento, stdin);
		numero_desplazamiento= str_int(desplazamiento);
	  	for (int i=0; desplazamiento[i] != '\n'; i++) if ((int)desplazamiento[i]<48 || (int)desplazamiento[i]>57) revision2=1;
		if (revision2==1) printf("Favor ingresar numero entero positivo:\n");
		else
		{	
			revision2=1;
			if (numero_desplazamiento>0) revision2=0;
			if (revision2==1) printf("Favor ingresar numero entero positivo:\n");
		}
	}
	printf ("\nIngrese caracter para realizar XOR:\n");
	scanf(" %c%*c", &CaractXor);
	
	//Encriptado
	if ((int)opcion == 49) Encriptar(numero_desplazamiento,CaractXor, oracion, codificado, nombre);	
	//Desencriptado
	if ((int)opcion == 50) Desencriptar(numero_desplazamiento,CaractXor, oracion, codificado, nombre);*/
		
	return 0;
}




