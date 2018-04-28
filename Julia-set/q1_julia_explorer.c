#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int main(int argc, char*argv[]){
	int maxChar = 1000;
	char string[maxChar];
		
	//check if both a and b are valid floats	
	int i;
	int j;
	for(i=0;i<strlen(argv[3]);i++){
		if((!isdigit(argv[3][i])) && argv[3][i] != '.'){
			printf("Error: bad float arg");
			return(-1);
		}
	}

	for(i=0;i<strlen(argv[2]);i++){
		if((!isdigit(argv[2][i])) && argv[2][i] != '.'){
			printf("Error: bad float arg");
			return(-1);
		}
	}


	float a = atof(argv[2]);
	float b = atof(argv[3]);

	//open the file
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL){
		printf("%s failed to open file %s for reading. Check permissions.\n", argv[0], argv[1]);
		exit(-1);
	}

	//read the file content to "string"
	char* wordA = "#A#";
	char* wordB = "#B#";
	char* ptrA;
	char* ptrB;
	//printf("%p",fp);
	char* temp;
	int testA = 0;
	int testB = 0;
	while(fgets(string,maxChar,fp)!=NULL){
		//check if the file contains #A# and #B# 
	
		if(string != NULL)
		{

			ptrA = strstr(string,wordA);
			ptrB = strstr(string,wordB);	
			if(ptrA==NULL && ptrB==NULL){
				for(temp = string; *temp != '\0'; temp++){
					printf("%c",*temp);
				}
			}else if(ptrA!=NULL && ptrB ==NULL)
			{
				testA = testA + 1; //record for #A#
				for(temp = string; temp!=ptrA;temp++){
					printf("%c",*temp);
				}
				printf("%.6f",a);
				for(temp = ptrA+3; *temp!='\0';temp++){
					printf("%c",*temp);
				}
			}else if(ptrA==NULL && ptrB !=NULL)
			{
				testB = testB + 1; //record for #B#
				for(temp = string; temp!=ptrB;temp++){
					printf("%c",*temp);
				}
				printf("%.6f",b);
				for(temp = ptrB+3; *temp!='\0';temp++){
					printf("%c",*temp);
				}
			}else if(ptrA!=NULL && ptrB!=NULL){
				testA = testA + 1; 
				testB = testB + 1;//record both
				//A before B
				if(ptrA<ptrB){
					//test
					printf("10\n");
					for(temp = string; temp!=ptrA;temp++){
						printf("%c",*temp);
					}
					printf("%.6f",a);
					for(temp = ptrA+3; temp!=ptrB;temp++){
						printf("%c",*temp);
					}
					printf("%.6f",b);
					for(temp = ptrB+3; *temp!='\0'; temp++){
						printf("%c",*temp);
					}
				}else//B before A
				{
					for(temp = string; temp!=ptrB;temp++){
						printf("%c",*temp);
					}
					printf("%.6f",b);
					for(temp = ptrB+3; temp!=ptrA;temp++){
						printf("%c",*temp);
					}
					printf("%.6f",a);
					for(temp = ptrA+3; *temp!='\0'; temp++){
						printf("%c",*temp);
					}	
				}
			}
		}
	}
	if(testA != 1 || testB !=1 ){
		printf("Error: bad file");
		return(-1);
	}




	fclose(fp);
}


