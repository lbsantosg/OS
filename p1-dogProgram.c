#include"stdio.h"
#include "stdlib.h"
#include "string.h"
#define SizePetData sizeof(struct petData)
const int hashSize = 2000;
struct petData{
	char name[32];
	char kind[32];
	int age;
	char breed[16];
	int height;
	float weight ;
	char sex ;
//	int next;
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
	printf("name: %s \n" , pet->name);
	printf("kind: %s \n", pet->kind);
	printf("age: %i \n" , pet->age);
	printf("breed: %s \n" , pet->breed);
	printf("height: %i \n" , pet->height);
	printf("weight: %f \n" , pet->weight);
	printf("sex: %c \n" , pet->sex);

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
void delete(){
	FILE *dt;
	FILE *newdt;
	struct petData* pet ;
	dt = fopen("dataDogs.data", "a+");
	int numRegDel;
	int nReg = getnReg(dt);
	printf("El numero actual de registros es: %i \n " , nReg );
	printf("Ingrese el numero de registro que desea borrar: \n");
	scanf("%i" , &numRegDel );
//	warningMenu(&numRegDel,nReg);	
	while (numRegDel > nReg||numRegDel < 1){
		printf("Numero de registro invalido. Intente de nuevo: \n");
		scanf("%i",&numRegDel);
	}
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
