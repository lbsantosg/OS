#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
#define SizePetData sizeof(struct petData)
const int NumberRegisters = 1000;
const int hashSize = 7919;
int hashLast[10000]; // array to store the last item of the hash on each position
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
void initHashFile()
{
	FILE *fp;
	fp = fopen("hash.dat","w");
	int nu = -1;
	for(int i = 0 ; i < hashSize ; i ++ )
	{
		hashLast[i] = -1;
		fwrite(&nu,sizeof(int),1,fp);
	}
	fclose(fp);
	//	memset(hashLast,-1,sizeof(hashLast));
	return;
}

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
void printHashTable()
{
	FILE *hashTable;
	hashTable = fopen("hash.dat","r");
	for(int i = 0 ; i < hashSize ; i ++ )
	{
		fseek(hashTable,sizeof(int)*i,SEEK_SET);
		int num ;
		fread(&num,sizeof(int),1,hashTable);
		if(num!=-1)
			printf("%d %d\n",i,num);
	}
	fclose(hashTable);
}
void initHash()
{
	// Reads every register in dataDogs and locates it in the hash
	FILE *hashTable, *data;
	hashTable = fopen("hash.dat","r+");
	data = fopen("dataDogs.data","r+");
	int n = NumberRegisters ;
	struct petData *pet;
	pet = malloc(sizeof(struct petData));
	int dif;
	int obj;
	fread(pet,SizePetData,1,data);
	for ( int i = 0 ; i < n ; i ++ )
	{
		obj = SizePetData*i;
		dif = obj - ftell(data);
		fseek(data,dif,SEEK_CUR);
		fread(pet, SizePetData,1,data);
		int indHash = hash(pet->name);
		fseek(hashTable, sizeof(int) * indHash, SEEK_SET);
		int next = hashLast[indHash];
		//if the hash is in -1, means that it has no register in there by the moment
		//so, it locates the curren index in the hash file and store it on the
		// hashLast array
		if( next == -1 )
		{
			fseek(hashTable,sizeof(int)*indHash,SEEK_SET);
			fwrite(&i,sizeof(int),1,hashTable);
			hashLast[indHash] = i;
		}
		else
		{
			//if it is not -1 means that already has a register, so locates the pointer
			// of dataDogs in the next register of the hash ("next") and reads it on pet
			//Then write pet on petData and finaly update the last item of the hash
			fseek(data,SizePetData*next,SEEK_SET);
			fread(pet,SizePetData,1,data);
			pet->next = i;
			obj = SizePetData*next;
			dif = obj - ftell(data);
			fseek(data,dif,SEEK_CUR);
			fwrite(pet,SizePetData,1,data);
			hashLast[indHash] = i;
		}
	}
	free(pet);
	fclose(hashTable);
	fclose(data);
	return;
}

int main()
{
  srand(time(NULL));
  char names[2000][32];
  char tmp[32];
	FILE *fp;
  fp = fopen("nombresMascotas.txt","r");
  int cn = 0;
	// counts the amount of available names and store them in an array
  while(fscanf(fp,"%s",names[cn])!=EOF)
      cn++;
  fclose(fp);
  fp = fopen("dataDogs.data","w");
  int r = 0;
  struct petData *reg;
  reg = malloc(sizeof(struct petData));
	//creates NumberRegisters registers and store them in DataDogs.dat
  for ( int i = 0 ; i < NumberRegisters; i ++ )
  {
		// randomly selects the properties and store them on pet
    r = rand()%cn; // selects a random name
    strcpy(reg->name,names[r]);
    strcpy(reg->kind, "Perro");
    reg->age = rand()%12;// selects a random age between 0 and 12
    reg->sex = ( (rand()%2) ? 'M' : 'F'); // selects the sex
    float w = (rand()% 600) + 1; // selects the weight
    w /= 10.0;
    reg->weight = w;
    reg->height = (rand() % 120) + 10;
		// selects one breed from a list of breeds
    int ind = rand() % 7;
    switch (ind)
    {
       case 0:
              strcpy(reg->breed,"Salchicha");
              break;
        case 1:
              strcpy(reg->breed,"Shitzu");
              break;
        case 2:
              strcpy(reg->breed,"Xoloescuincle");
              break;
        case 3:
              strcpy(reg->breed,"Criollo");
              break;
        case 4:
              strcpy(reg->breed,"PastorAleman");
              break;
        case 5:
              strcpy(reg->breed,"Golden");
              break;
    		case 6:
              strcpy(reg->breed,"Unknown");
              break;
    }
		reg->next = -1; // at the beggining the hash hasn't been created
    fwrite(reg,sizeof(struct petData),1,fp); // stores pet in DataDogs.dat
  }
    free(reg);
    fclose(fp);
    initHashFile();
    initHash();
}
