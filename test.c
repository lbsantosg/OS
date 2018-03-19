#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
int main()
{
	int a;
	scanf("%d",&a);
	char inte[10];
	sprintf(inte,"%d",a);
	char cmd[100] = "nano historiaClinica-";
	char end[10] = ".txt";
	strcat(cmd,inte);
	strcat(cmd,end);
	system(cmd);
	printf("easfdas\n");
}
