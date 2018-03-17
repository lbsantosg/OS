#include<stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
const int NumberRegisters = 1000;
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
int main()
{
    char names[2000][32];
    char tmp[32];
	FILE *fp;
    fp = fopen("nombresMascotas.txt","r");
    int cn = 0;
    while(fscanf(fp,"%s",names[cn])!=EOF)
        cn++;

    fclose(fp);
    fp = fopen("dataDogs.data","w");
    int r = 0;
    struct petData *reg;
    reg = malloc(sizeof(struct petData));
    for ( int i = 0 ; i < NumberRegisters; i ++ )
    {
        r = rand()%cn;
        strcpy(reg->name,names[r]);
        strcpy(reg->kind, "Perro");
        reg->age = rand()%12;
        reg->sex = ( (rand()%2) ? 'M' : 'F');
        float w = (rand()% 600) + 1;
        w /= 10.0;
        reg->weight = w;
        reg->height = (rand() % 120) + 10;
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
		reg->next = -1;
        fwrite(reg,sizeof(struct petData),1,fp);
    }
    free(reg);
    fclose(fp);
}
