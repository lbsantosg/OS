

// to reserve space in memmory is used the maloc function
// when the space is not needed anymore is used the free() function to free the space


#include"stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "unistd.h"
#define SizePetData sizeof(struct petData)
const int hashSize = 7919;
//declaration of the struct to save the information of a pet
// the next atribute stores the index of the next element of the file with the same hash name
// or -1 if that element doesnt exists
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
	// Hash function: recieve one character pointer of a string and return an integer
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
	//a function that receives a character from console 
	//its used to wait for a confirmation to continue by the user
	printf("Ingrese C para continuar \n " );
	char option;
    while(scanf(" %c" , &option)){
		return;
	}
}
void animalPrint(void *ap){
	// a function that receive a pointer *ap where is stored a struct petData
	// and then print its data on the console
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
	// print the information of the main menu of  the app
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
	// funtion that saves the string "historiaClinica-n.txt" in the position of  memmory 
	// where reg1 points. the variable n is the index of a register.
//	char *reg = reg1;
	
    int a = n;
    char inte[10];
    sprintf(inte,"%d",a);
    char cmd[50] = "historiaClinica-";
    char end[5] = ".txt";
    strcat(cmd,inte);
    strcat(cmd,end);
	printf("%s\n",cmd);
	strcpy(reg,cmd);
    return;
}

void receiveReg(void *ap){
	// function that receives information of a new register from the user by the console and stores it
	// in a struct petData instance located on *ap
	struct petData *pet;
	pet = ap;
	printf("Nombre: \n");
	scanf("%s" , pet->name);
	printf("Tipo:	\n");
	scanf("%s" , pet->kind);
	printf("Edad:  \n");
	scanf("%i" , &pet->age);
	printf("Raza:  \n");
	scanf("%s" , pet->breed);
	printf("Estatura:	\n");
	scanf("%i" , &pet->height);
	printf("Peso: \n");
	scanf("%f" , &pet->weight);
	printf("Sexo: \n");
	scanf(" %c" , &pet->sex);
	pet->next = -1;
	return;
}

void hashLastItem(void *ap)
{
	//Add the last entered register to its linked list
	struct petData *pet = ap;
	int hashInd = hash(pet->name);
	FILE *hashTable = fopen("hash.dat","r+");
	FILE *dt = fopen("dataDogs.data","r+");
	if (hashTable == NULL || dt == NULL )
	{
		perror("Arhivos no encontrados\n");
		exit(-1);
	}
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
	// OPTION  1 of the menu
	// open the file dataDogs.data for updating
	// call the function to input information of a new register from console
	// then writes the information at the end of the file
	// print a msg of succes 
   	// finally call the function to add the new register to the hast table	
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

	// a function that shows the number of current registers on the file and then ask for a numter to the user to print
	// the information of that register
	// then the user decides if he want to open the clinicHistory of the register
	FILE *dt = fopen("dataDogs.data","r");
	if(dt == NULL)
	{
		printf("No hay archivo de registros");
		return ;
	}
	struct petData *pet;
//	printf("print\n");
	int nreg = getnReg(dt);
	printf("Numero de registros : %i \n" ,nreg);
	if(nreg == 0 )
	{
		printf("No hay registros\n");
		return ;
	}
	printf("Ingrese el numero de registro que desea ver \n" );
	int indReg;
	scanf("%i",&indReg);
	// check if the input is valid
	while (indReg > nreg||indReg < 1){
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
//	printf("%s\n",cmd1);
	char cmd[35] ;
	if ( opt == 'y' || opt == 'Y' ){
//		printf("%s\n", cmd1);
		NameClinicHistory(indReg,cmd);
//		printf("%s\n", cmd);
//		printf("%s\n", cmd1);
		strcat(cmd1 , cmd);
//		printf(" msg %s\n",cmd1);
		system(cmd1);
	}
	toContinue();
	return;
}
void delClinicHistory(int _delIndex, int _lastIndex){
	// a fuction that receives the index of the deleted element of the file and the index of the last element of the file
	// if the clinic history of the deleted element exists is deleted
	// if the last register has clinic history file it changes it name because last element is moved where the deleted element was
	 
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
	if(dt == NULL || hashTable == NULL )
	{
		perror("Archivos no encontrados\n");
		exit(-1);
	}
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


	//Option 3 of the app

	// a fuction that asks for the index of a register to delete
	// to delete the register a temporal file is created where all the registers all copied excepted for the deleted one
	// and the last register is writed where the deleted register was
	// then the original file is deleted and the temporal file is renamed as the original 
	FILE *dt;
	FILE *newdt;
	struct petData* pet ;
	dt = fopen("dataDogs.data", "a+");
	if(dt == NULL )
	{
		perror("archivo no encontrado\n");
		exit(-1);
	}
	int numRegDel;
	int nReg = getnReg(dt);
	printf("El numero actual de registros es: %i \n " , nReg );
	printf("Ingrese el numero de registro que desea borrar: \n");
	fclose(dt);
	scanf("%i" , &numRegDel );
	// check if the number entered is valid
	while (numRegDel > nReg||numRegDel < 1){
		printf("Numero de registro invalido. Intente de nuevo: \n");
		scanf("%i",&numRegDel);
	}
	// call a function to modify the linked list of the deleted item and the last item
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
	
		// if the position is equal where the deleted register was, the last item is wrote
		// and continue with the next position
		if( i == numRegDel-1) {
			fwrite(tmp,SizePetData,1,newdt);
			continue;
		}
		
		fread(pet , SizePetData,1,dt);
		// write the ith register from the original file on the temporal file 
		fwrite(pet,SizePetData , 1 , newdt);
	}
	
	delClinicHistory(numRegDel,nReg);
	free(pet);
	fclose(dt);
	fclose(newdt);
	// the original file is replaced by the temporal file
	
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
	if(hashTable == NULL || dt == NULL)
	{
		perror("Archivo no encontrado\n");
		exit(-1);
	}
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
	// option 5
	// print a goodbye msg
	// and then call the function exit to end the execution of the program
	printf("Vuelva pronto \n");
	exit(0);
}
void invalidOption(){
	// function that prints a warning msg
	printf("Usted ha seleccionado una opcion invalida. Intente de nuevo. \n" );
	return;
}
int main (){
		struct petData* pet;
		pet = malloc(sizeof (struct petData));
		printMenu();
		int option ;
		// wait for the user to input an option 
		while(	scanf("%i" , &option)){

			// choose the case of the number entered by the user
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
