
#include"stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "unistd.h"
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
	// Hash function: recieve one character string and return an integer
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
void toContinue(){
	printf("Ingrese C para continuar \n " );
	char option;
    while(scanf(" %c" , &option)){
		return;
	}
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
	// receive a pointer from a FILE and return the number of registers (struct
  //petData) that the file has.
	FILE *dt = ap;
	fseek(dt,0,SEEK_END);
	int len = ftell(dt);
	return len/SizePetData;
}
void NameClinicHistory(int n , void *reg)
{
    int a = n;
    char inte[10];
    sprintf(inte,"%d",a);
    char cmd[50] = "historiaClinica-";
    char end[5] = ".txt";
    strcat(cmd,inte);
    strcat(cmd,end);
	  strcpy(reg,cmd);
    return;
}

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
	//Add one new register to the hash Table
	struct petData *pet = ap;
	int hashInd = hash(pet->name);
	FILE *hashTable = fopen("hash.dat","r+");
  FILE *dt = fopen("dataDogs.data","r+");
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	int next ;
	fread(&next,sizeof(int),1,hashTable);
	if(next == -1)
	{
		//if the hash doesn't has any register, locates the actual register
		// in the "head"
		fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
		next = getnReg(dt) -1;
		fwrite(&next,sizeof(int),1,hashTable);
	}
	else
	{
		// if the hash has already any register, go throught the hash
		// using checking the "next" attribute of each pet
		while(1<2)
		{
			fseek(dt,SizePetData*next,SEEK_SET);
			fread(pet,SizePetData,1,dt);
			if(pet->next == -1 )
			{
				//when the tail is found, locates the new register in the last position
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
	printf("Registro agregado de manera exitosa. \nNum Reg %i\n",getnReg(dt));
	fclose(dt);
	hashLastItem(pet);
	free(pet);
	toContinue();
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
	printf("Desea abrir historial medico? Precione Y para abirlo, N para continuar \n");
	free(pet);
	fclose(dt);
	char opt;
	scanf(" %c" , &opt ) ;
	char cmd1[50] = "nano ";
	char cmd[20] ;
	if ( opt == 'y' || opt == 'Y' ){
		NameClinicHistory(indReg,cmd);
		strcat(cmd1 , cmd);
		system(cmd1);
	}
	toContinue();
	return;
}
void delClinicHistory(int _delIndex, int _lastIndex){
	int lastIndex = _lastIndex;
	int delIndex = _delIndex;
	char nameCHDel[30];
	char nameCHLast[30];
	NameClinicHistory(delIndex,nameCHDel);
	NameClinicHistory(lastIndex,nameCHLast);
	int chexistDel = access(nameCHDel , F_OK);
	int chexistLast = access(nameCHLast , F_OK);
	if(chexistDel == 0){
		remove(nameCHDel);
	}
	if(chexistLast == 0){
		rename(nameCHLast, nameCHDel);
	}
	return;
}
void fixHash(int indx)
{
	//fixes the hasTable after one register has been deleted
	//1.Shifts the last register with the one that will be delete
	FILE *dt;
	FILE *hashTable;
	dt = fopen("dataDogs.data","r+");
	hashTable = fopen("hash.dat","r+");
	struct petData *pet;
	pet = malloc(SizePetData);
	int Delind = indx - 1 ;
	int indLast = getnReg(dt) -1;
	// locates the pointer in the last register of DataPet.data (this one will be shifting) and
	// reads it on pet, then get the name and make the hash.
	fseek(dt,SizePetData*indLast,SEEK_SET);
	fread(pet,SizePetData,1,dt);
	int hashInd = hash(pet->name);
	int next ;
	// locates the pointer in the position of the hash of pet and reads it in next
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	fread(&next,sizeof(int),1,hashTable);
	if(next == indLast)
	{
		// if the index that has been read is the same as next, then writes the
		// index of the register that will be remove
		fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
		fwrite(&Delind,sizeof(int),1,hashTable);
	}
	else
	{
		while(1<2)
		{
			//continues reading throught the hash using the pointer "next" of each structure
			// and when the number of the last register is found, it is changed for the
			// number of the register that will be delete
			fseek(dt,SizePetData*next,SEEK_SET);
			fread(pet,SizePetData,1,dt);
			if(pet->next == indLast)
			{
				pet->next = Delind;
				fseek(dt,SizePetData*next,SEEK_SET);
				fwrite(pet,SizePetData,1,dt);
				break;
			}
			else
				next = pet->next;
		}
	}
	//2. The register that will be remove is deleted from the hash
	// Locates the pointer of DataDogs.data in the position of the register that
	// will be remove, then reads it on "delete" and gets the hash
	// It will look for the register of "delete" through the hash and when it is
	//found the pointer of the previous will point to the register that is pointed
	// by "delete" instead of the register of "delete"
	struct petData *delete;
	delete = malloc(SizePetData);
	fseek(dt,SizePetData*Delind,SEEK_SET);
	fread(delete,SizePetData,1,dt);
	hashInd = hash(delete->name);
	fseek(hashTable,sizeof(int)*hashInd,SEEK_SET);
	fread(&next,sizeof(int),1,hashTable);
	if(next == Delind)
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
			if(pet->next == Delind)
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
	while (numRegDel > nReg||numRegDel < 1){
		printf("Numero de registro invalido. Intente de nuevo: \n");
		scanf("%i",&numRegDel);
	}
	fixHash(numRegDel);
	dt = fopen("dataDogs.data","a+");
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
	delClinicHistory(numRegDel,nReg);
	free(pet);
	fclose(dt);
	fclose(newdt);
	remove("dataDogs.data");
	rename("dataDogsNew.data" , "dataDogs.data");
	printf("Registro %i removido de manera exitosa \n",numRegDel);;
	toContinue();
	return;

}

void search(){
	// recieves a string and shows all the registers that match with it
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
	// receives the string,make the hash and get the register that is in there.
	// Then follows the hash until the end (when the next is -1); check that
	// the string match by complete in order to avoid collitions of different names
	// and prints it
	fread(&next,sizeof(int),1,hashTable);
	struct petData *pet;
	pet = malloc(SizePetData);
	for(int i = 0 ; i < strlen(toSearch); i ++ )
		toSearch[i] = tolower(toSearch[i]);
	while(next!=-1)
	{
		//reads the register of next in pet, and then read the "next" attribute of
		// the actual pet.
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
	toContinue();
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
