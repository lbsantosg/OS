#include"stdio.h"
#include "stdlib.h" 
#include "string.h"
#define SizePetData sizeof(struct petData)
struct petData{
		char name[32];
		char kind[32];
		int age;
		char breed[16];
		int height;
		float weight ; 
		char sex ; 

};

void animalPrint(void *ap)
{
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
	
		printf("Seleccione su opcion:\n");
	    printf("1 Para ingresar datos\n");
    	printf("2 Para ver un registro\n");
    	printf("3 Para eliminar un registro\n");
		printf("4 Para buscar un registro\n");
		printf("5 Para salir\n" );
	return;
}
int getnReg(void *ap)
{
		FILE *dt = ap;
		fseek(dt,0,SEEK_END);
		int len = ftell(dt);
		printf("File size %d \n" , len);
		return len/SizePetData;
				
}
void receiveReg(void *ap)
{
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
		animalPrint(pet);
		fwrite(pet,SizePetData,1,dt);
		printf("Finish. \n Num Reg %i\n",getnReg(dt));
		fclose(dt);
		free(pet);
		printMenu();
		return;
}		
void showData() {
		
		FILE *dt = fopen("dataDogs.data","r");
		struct petData *pet;
		printf("print\n");
		printf(" num reg : %i \n" ,getnReg(dt));
		printf("Ingrese el numero de registro que desea ver \n" );
		int indReg;
		scanf("%i",&indReg); 
		fseek(dt, SizePetData*(indReg-1), SEEK_SET);
		pet = malloc(SizePetData);	
		fread(pet,SizePetData,1,dt);
		animalPrint(pet);
		free(pet);
		fclose(dt);
		printMenu();
}
void delete(){
	FILE *dt; 
	FILE *newdt;
	struct petData* pet ;
	dt = fopen("dataDogs.data", "a+");
	int numReg;
	int nReg = getnReg(dt);
	printf("El numero actual de registros es: %i \n " , nReg );
	printf("Ingrese el numero de registro que desea borrar: \n");
	scanf("%i" , &numReg ); 
   	newdt =	fopen("dataDogsNew.data" , "a+");
	int deleteable = (numReg-1) * SizePetData ; 
	pet = malloc(SizePetData);
	for ( int i = 0 ; i < nReg ; i ++){
		if( i == numReg-1) continue; 
		fseek(dt,SizePetData*i, SEEK_SET);
		fread(pet , SizePetData,1,dt);
		fwrite(pet,SizePetData , 1 , newdt);
	}
	fclose(dt);
	fclose(newdt);
	remove("dataDogs.data");
	rename("dataDogsNew.data" , "dataDogs.data");
	printf("Registro %i removido de manera exitosa \n",numReg);
	printMenu();
	return;

}
void search(){
	return;
}
void exitApp(){
	exit(0);
}

void warningMenu(){
	return;
}
int main (){
		struct petData* pet;
		pet = malloc(sizeof (struct petData));
		printMenu();
		int option ;
		while(	scanf("%i" , &option))
		{
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
					warningMenu();

			}
		}	
		free(pet);
		return 0 ; 
}
