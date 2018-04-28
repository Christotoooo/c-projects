#include "A4_sort_helpers.h"
#include <stdlib.h>
// Function: read_all() 
// Provided to read an entire file, line by line.
// No need to change this one.
void read_all( char *filename ){
    
    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
	
    while( curr_line < MAX_NUMBER_LINES && 
           fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) )
    {
        curr_line++;
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Function: read_all() 
// Provided to read only the lines of a file staring with first_letter.
// No need to change this one.
void read_by_letter( char *filename, char first_letter ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
    text_array[curr_line][0] = '\0';
	
    while( fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) ){
        if( text_array[curr_line][0] == first_letter ){
            curr_line++;
        }

        if( curr_line == MAX_NUMBER_LINES ){
            sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
            write( 1, buf, strlen(buf) );
            break;
        }
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.
void sort_words( ){
	int length=0;
	char temp[MAX_LINE_LENGTH];
	
	for(int x=0;x<MAX_NUMBER_LINES;x++){
		if (text_array[x][0]=='\0'){
			break;
		}
		length = length +1;
	}

	for(int j =1; j<length;j++){
		int i=j;
		while(i>0 && strcmp(text_array[i-1],text_array[i])>0){
			strcpy(temp,text_array[i]);
			strcpy(text_array[i],text_array[i-1]);
			strcpy(text_array[i-1],temp);
			i--;
		}
	}
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2.
	//: use a 27-element array to hold all the semaphores including the wrap-up
	char names[27][256]={"finalize","first","second","third","fourth","fifth","sixth","seventh","eighth",
						"nineth","tenth","eleventh","twelveth","thirteenth","forteenth",
						"fifteenth","sixteenth","seventeenth","eighteenth","nineteenth",
						"twentieth","twenty-first","twenty-second","twenty-third","twenty-fourth",
						"twenty-fifth","twenty-sixth"};
	
	sem_t* sems[27];
	
int initialize( ){
	//for finalize semaphore
	sem_unlink( names[0]);
	sems[0]=sem_open(names[0],O_CREAT,0666,0);
	
	//first one is special
	sem_unlink( names[1] );
	sems[1]=sem_open(names[1],O_CREAT,0666,1);
	for(int i =2;i<27;i++){
		sem_unlink( names[i] );
		sems[i]=sem_open(names[i],O_CREAT,0666,0);
	}
	
	FILE* fp = fopen("temp.txt", "w");
	fclose(fp);
	/* to be shared among processes */
    return 0;
}

// YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.   
int process_by_letter( char* input_filename, char first_letter ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "This process will sort..." lines
    // are printed in the right order (alphabetical).
	sem_wait(sems[first_letter - 'a'+1]);
	read_by_letter( input_filename, first_letter );
    sort_words( );
	int length = 0;
	for(int x=0;x<MAX_NUMBER_LINES;x++){
		if (text_array[x][0]=='\0'){
			break;
		}
		length = length +1;
	}
	
	
	FILE*temp = fopen("temp.txt","a+");
	if(temp==NULL){
		printf("Error openning the temp file");
		exit(-1);
	}else{
		for(int k=0;k<length;k++){
			fprintf(temp,"%s",text_array[k]);
		}
	}
	
	
    //sprintf( buf, "This process will sort the letter %c.\n",  first_letter );//given
    //write(1,buf,strlen(buf));//given
	
	if((first_letter)!=('z')){
		//printf("%c - %d - %s\n", first_letter, first_letter -'a'+2, names[first_letter -'a'+2]);
		sem_post(sems[first_letter -'a'+2]);
	}else if((first_letter)==('z')){
		sem_post(sems[0]);// call the finalize semaphore
	}
    // For Q3, uncomment the following 2 lines and integrate them with your overall solution.


    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
int finalize( ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "Sorting complete!" line
    // is printed at the very end, after all letter lines.
	sem_wait(sems[0]);
	

    // For Q3, come up with a way to accumulate the sorted results from each
    // letter process and print the overal sorted values to standard out.
    // You are not allowed to read from the input file, or call sort_words
    // directly from this function.
	FILE *file = fopen("temp.txt","r");
	if(file==NULL){
		printf("Error openning the temp file");
		exit(-1);
	}else{
		char buffer[MAX_LINE_LENGTH];
		while (fgets(buffer, sizeof(buffer), file) != NULL){
			//fputs(buffer, stdout);
			//fflush(stdout);
			write(1, buffer, strlen(buffer));
		}
	}
	
	sprintf( buf, "Sorting complete!\n" );//given
    write( 1, buf, strlen(buf) );//given

	fclose(file);
	remove("temp.txt");
	
    return 0;
}

