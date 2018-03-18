#include"stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#define SizePetData sizeof(struct petData)
const int hashSize = 7919;
struct petData{
	char name[32];
	char kind[32];
	int age;
	char breed[16];
	int height;
	float weight ;
	char sex ;
	int next;
};

int hash(char *st)
{
	int size = strlen(st);
	int b = 26;
	int mult = 1;
	int hash = 0;
	for ( int i = 0 ; i < size ; i ++ )
	{
		mult = ( mult *b ) % hashSize;
		hash += (tolower(st[i])) * mult;
		hash = hash % hashSize;
	}
	return hash;
}

void animalPrint(void *ap){
	struct petData *pet;
	pet = ap;
	printf("******************************************************\n");
	printf("    Nombre: %s \n" , pet->name);
	printf("    Tipo: %s \n", pet->kind);
	printf("    Edad: %i \n" , pet->age);
	printf("    Raza: %s \n" , pet->breed);
	printf("    Estatura: %i \n" , pet->height);
	printf("    Peso: %f \n" , pet->weight);
	printf("    Sexo: %c \n" , pet->sex);
	printf("******************************************************\n");
	
}

void printMenu(){
	printf("******************************************************\n");
	printf("            Seleccione su opcion:\n");
	printf("            1 Para ingresar datos\n");
    printf("            2 Para ver un registro\n");
    printf("            3 Para eliminar un registro\n");
	printf("            4 Para buscar un registro\n");
	printf("            5 Para salir\n" );
	printf("******************************************************\n        ");
	return;
}

int getnReg(void *ap){
	FILE *dt = ap;
	fseek(dt,0,SEEK_END);
	int len = ftell(dt);
	return len/SizePetData;
}
/*
void warningMenu(void *ap, int n){
	int *numReg;
	int nReg = n;
	numReg = malloc(sizeof(int));	
	numReg = ap ;
	printf("La cantidad de registros es %i \n" , nReg);
	int Register = *numReg;
	while (Register > nReg || Register < 1){
		printf("Numero de registro invalido. Intente de nuevo: \n");
		scanf("%i",numReg);
	}
   	free(numReg);
	return;
} */
void receiveReg(void *ap){
	struct petData *pet;
	pet = ap;
	printf("Name: \n");
	scanf("%s" , pet->name);
	printf("Kind:	\n");
	scanf("%s" , pet->kind);
	printf("Age:  \n");
	scanf("%i" , &pet->age);
	printf("Breed:  \n");
	scanf("%s" , pet->breed);
	printf("Height:	\n");
	scanf("%i" , &pet->height);
	printf("Weight: \n");
	scanf("%f" , &pet->weight);
	printf("Sex: \n");
	scanf(" %c" , &pet->sex);
	pet->next = -1;
	return;
}

void hashLastItem(void *ap)
{
	struct petData *pet = ap;
	int hashInd = hash(pet->name);
	FILE *hashTable = fopen("hash.dat","r+");
    FILE *dt = fopen("dataDogs.data","r+");
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	int next ;
	fread(&next,sizeof(int),1,hashTable);
	if(next == -1)
	{
		fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
		next = getnReg(dt) -1;
		fwrite(&next,sizeof(int),1,hashTable);
	}
	else
	{
		while(1<2)
		{
			fseek(dt,SizePetData*next,SEEK_SET);
			fread(pet,SizePetData,1,dt);
			if(pet->next == -1 )
			{
				pet->next = getnReg(dt) -1 ;
				fseek(dt,SizePetData*next,SEEK_SET);
				fwrite(pet,SizePetData,1,dt);
				break;
			}
			else
				next = pet->next;
		}
	}
	fclose(dt);
	fclose(hashTable);
	return;
}
void  inputData(){
	struct petData *pet;
	pet = malloc(SizePetData);
	FILE *dt = fopen("dataDogs.data","a");
	receiveReg(pet);
	fwrite(pet,SizePetData,1,dt);
	printf("Finish. \nNum Reg %i\n",getnReg(dt));
	fclose(dt);
	hashLastItem(pet);
	free(pet);
	return;
}
void showData() {
	FILE *dt = fopen("dataDogs.data","r");
	struct petData *pet;
	printf("print\n");
	printf("Numero de registros : %i \n" ,getnReg(dt));
	printf("Ingrese el numero de registro que desea ver \n" );
	int indReg;
	scanf("%i",&indReg);	
	while (indReg > getnReg(dt)||indReg < 1){
		printf("Numero de registro invalido. Intente de nuevo: \n");
		scanf("%i",&indReg);
	}
	fseek(dt, SizePetData*(indReg-1), SEEK_SET);
	pet = malloc(SizePetData);
	fread(pet,SizePetData,1,dt);
	animalPrint(pet);
	free(pet);
	fclose(dt);
}
void fixHash(int ind)
{
	FILE *dt;
	FILE *hashTable;
	dt = fopen("dataDogs.data","r+");
	hashTable = fopen("hash.dat","r+");
	struct petData *pet;
	pet = malloc(SizePetData);
	ind = ind - 1 ;
	int indLast = getnReg(dt) -1;
	fseek(dt,SizePetData*indLast,SEEK_SET);
	fread(pet,SizePetData,1,dt);
	int hashInd = hash(pet->name);
	int next ;
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	fread(&next,sizeof(int),1,hashTable);
	if(next == indLast)
	{
		fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
		fwrite(&ind,sizeof(int),1,hashTable);
//		fclose(hashTable);
//		hashTable = fopen("hash.dat","r+");
	}
	else
	{
		while(1<2)
		{
			fseek(dt,SizePetData*next,SEEK_SET);
			fread(pet,SizePetData,1,dt);
			if(pet->next == indLast)
			{
				pet->next = ind;
				fseek(dt,SizePetData*next,SEEK_SET);
				fwrite(pet,SizePetData,1,dt);
		//		fclose(dt);
	//			dt = fopen("dataDogs.data","r+");
				break;
			}
			else
				next = pet->next;
		}
	}
	struct petData *delete;
	delete = malloc(SizePetData);
	fseek(dt,SizePetData*ind,SEEK_SET);
	fread(delete,SizePetData,1,dt);
	hashInd = hash(delete->name);
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	fread(&next,sizeof(int),1,hashTable);
	if(next == ind)
	{
		fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
		int tmp = delete->next;
		fwrite(&tmp,sizeof(int),1,hashTable);
	}
	else
	{
		while(1<2)
		{
			fseek(dt,SizePetData*next,SEEK_SET);
			fread(pet,SizePetData,1,dt);
			if(pet->next == ind)
			{
				pet->next = delete->next;
				fseek(dt,SizePetData*next,SEEK_SET);
				fwrite(pet,SizePetData,1,dt);
				break;
			}
			else
				next = pet->next;
		}
	}
	free(delete);
	free(pet);
	fclose(dt);
	fclose(hashTable);
	return;
}
void delete(){
	FILE *dt;
	FILE *newdt;
	struct petData* pet ;
	dt = fopen("dataDogs.data", "a+");
	int numRegDel;
	int nReg = getnReg(dt);
	printf("El numero actual de registros es: %i \n " , nReg );
	printf("Ingrese el numero de registro que desea borrar: \n");
	fclose(dt);
	scanf("%i" , &numRegDel );
	fixHash(numRegDel);
	dt = fopen("dataDogs.data","a+");
//	warningMenu(&numRegDel,nReg);	
	while (numRegDel > nReg||numRegDel < 1){
		printf("Numero de registro invalido. Intente de nuevo: \n");
		scanf("%i",&numRegDel);
	}
	
//	printf("while suposed to be 1\n");
	newdt =	fopen("dataDogsNew.data" , "a+");
	struct petData *tmp ; 
	tmp = malloc(SizePetData);
	fseek(dt,SizePetData*(nReg-1), SEEK_SET);
	fread(tmp, SizePetData ,1,dt);
	pet = malloc(SizePetData);
	for ( int i = 0 ; i < nReg-1 ; i ++){
		fseek(dt,SizePetData*i, SEEK_SET);
		if( i == numRegDel-1) {
			fwrite(tmp,SizePetData,1,newdt);
			continue;
		}
		fread(pet , SizePetData,1,dt);
		fwrite(pet,SizePetData , 1 , newdt);
	}
	free(pet);
	fclose(dt);
	fclose(newdt);
	remove("dataDogs.data");
	rename("dataDogsNew.data" , "dataDogs.data");
	printf("Registro %i removido de manera exitosa \n",numRegDel);;
	return;

}

void search(){
	char toSearch[32];
	scanf("%s",toSearch);
	int hashInd = hash(toSearch);
	FILE *hashTable;
	FILE *dt;
	hashTable = fopen("hash.dat","r");
	dt = fopen("dataDogs.data","r");
	int cReg = 0;
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	int next ;
	fread(&next,sizeof(int),1,hashTable);
	struct petData *pet;
	pet = malloc(SizePetData);
	for(int i = 0 ; i < strlen(toSearch); i ++ )
		toSearch[i] = tolower(toSearch[i]);
	while(next!=-1)
	{
		fseek(dt,SizePetData*next,SEEK_SET);
		fread(pet,SizePetData,1,dt);
		char tmp[32];
		strcpy(tmp,pet->name);
		for(int i = 0 ; i < strlen(tmp); i ++ )
			tmp[i] = tolower(tmp[i]);
		
		if(strcmp(toSearch,tmp) == 0 )
		{
			printf("Registro en la posicion %d\n",(next+1));
			animalPrint(pet);
			cReg++;
		}
		next = pet->next;
	}
	printf("Se encontraron %d registros con nombre %s\n",cReg,toSearch);
	free(pet);
	fclose(hashTable);
	fclose(dt);
	return;
}
void exitApp(){
	printf("Vuelva pronto \n");
	exit(0);
}
void invalidOption(){
	printf("Usted ha seleccionado una opcion invalida. Intente de nuevo. \n" );
	return;
}
int main (){
//		printHashTable();
		struct petData* pet;
		pet = malloc(sizeof (struct petData));
		printMenu();
		int option ;
		while(	scanf("%i" , &option)){
			switch(option){
				case 1 :
					inputData();
					break;
				case 2 :
					showData();
					break;
				case 3 :
					delete();
					break;
				case 4 :
					search();
					break;
				case 5:
					exitApp();
					break;
				default :
					invalidOption();

			}
			printMenu();
		}
		free(pet);
		return 0 ;
}
