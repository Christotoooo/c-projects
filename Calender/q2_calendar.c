#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<string.h>

//helper methods
void pipe();
void endpipe();
void dashes(int);
void separator(int);
void spaces(int);
void space_before(int,int);
void space_after(int,int);
void monthline(int,char[],int);
void weekline(int);
int days(int,int); // loop through 30 days of a month
void blanks(int,int);
void blanks_after(int,int);

//MAIN here
int main(int argc, char*argv[]){
	int DAYSIZE = atoi(argv[1]);
	int FIRSTDAY = atoi(argv[2]);

	if(DAYSIZE<2){
		printf("Invalid DAYSIZE");
		exit(-1);
	}
	if(FIRSTDAY<1 || FIRSTDAY>7){
		printf("Invalid FIRSTDAY");
		exit(-1);
	}
	
	int totalchar = (DAYSIZE+3)*7-1; //excluding two pipes at two sides
	
	//array of months,leaving the first spot blank
	char month[13][10] = {" ","January","February","March","April","May","June","July","August","September","October","November","December"};
	//array of length of months, leaving the first spot blank
	int length[13] = {0,7,8,5,5,3,4,4,6,9,7,8,8};

	//test
	//January
	separator(totalchar);
	monthline(totalchar,month[1],length[1]);
	separator(totalchar);
	weekline(DAYSIZE);
	separator(totalchar);
	int temp = days(DAYSIZE,FIRSTDAY);

	int i;
	for ( i = 2; i<13;i++){
		separator(totalchar);
		monthline(totalchar,month[i],length[i]);
		separator(totalchar);
		weekline(DAYSIZE);
		separator(totalchar);
		temp = days(DAYSIZE,temp);
	}
	separator(totalchar);

}

//helper method to print blank blocks
void blanks(int DAYSIZE,int num){
	int a;
	for (a=0;a<num;a++){
		pipe();
		spaces(DAYSIZE+2);
	}
}

//helper method to print blank blocks at the end of a line
void blanks_after(int DAYSIZE,int num){
	int a;
	for (a=0;a<num;a++){
		pipe();
		spaces(DAYSIZE+2);
	}
	pipe();
	printf("\n");
}

//helper method to loop through 30 days given the daysize and the first day
int days(int DAYSIZE, int FIRSTDAY){
	if(FIRSTDAY>1){
		blanks(DAYSIZE,FIRSTDAY-1);
	}
	int i = FIRSTDAY;
	int a;
	for (a=1;a<31;a++){
		pipe();
		printf(" %d",a);
		if(a<10){
			spaces(DAYSIZE);
		}else{
			spaces(DAYSIZE-1);
		}
		//check if we need to go to a new line
		if(i==7){
			endpipe();
			i = 0;
		}
		i++;
	}
	//if still spots in this line
	if(i == 7){
		blanks_after(DAYSIZE,1);
	}
	if(i<7 && i!=1){
		blanks_after(DAYSIZE,8-i);
	}
	
	return(i);
}




//helper method to print a month line. Passing totalchar,month array and length array
void monthline(int char_total, char month[], int length){
	int a; //temp
	pipe();
	space_before(char_total,length);
	for (a=0;a<length;a++){
		printf("%c",month[a]);
	}
	space_after(char_total,length);
	endpipe();
}


//helper method to print the row of seven days of a week
void weekline(int DAYSIZE){
	//array of seven days of a week
	char weekday[8][100] = {" ","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	//array of length of seven days of a week
	int length2[8] = {0,6,6,7,9,8,6,8};

	int a;
	int b;
	for (a = 1; a<8;a++){
		pipe();
		printf(" ");
		for (b=0; b<DAYSIZE;b++){
			printf("%c",weekday[a][b]);
		}
		printf(" ");
	     //	if(DAYSIZE>length2[a]){
             //	spaces(DAYSIZE-length2[a]);
	     // }	
	}
	endpipe();
}



//helper method to print a pipe "|"
void pipe(){
	printf("|");
}

//helper method to print a pipe at the end of a line and a new line
void endpipe(){
	printf("|\n");
}

//helper method to print a line of dashes "-"
void dashes(int num){
	int a;
	for (a = 0;a<num; a++){
		printf("-");
	}
}

//helper method to print a whole separator line
void separator(int num){
	pipe();
	dashes(num);
	endpipe();
}

//helper method to print spaces
void spaces(int num){
	int a;
	for (a=0;a<num;a++){
		printf(" ");
	}
}


//helper method to print spaces before
void space_before(int num, int length){
	int space_total = num - length;
	spaces(space_total/2);
}

//helper method to print spaces after
void space_after(int num, int length){
	int space_total = num - length;
	if(space_total%2 == 0){
		spaces(space_total/2);
	}
	if(space_total%2 != 0){
		spaces(space_total/2 + 1);
	}
}
