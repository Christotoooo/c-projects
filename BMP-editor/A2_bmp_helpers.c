/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 * 
 * Each of the functions below can be considered a start for you. 
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int bmp_open( char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset, unsigned char** img_data ){

              
  // YOUR CODE FOR Q1 SHOULD REPLACE EVERYTHING FROM HERE
	FILE* filePtr = fopen(bmp_filename,"rb");
	if(filePtr == NULL){
		printf("Error: fopen failed\n");
		return(-1);
	}
	
	unsigned char *header = malloc(32);
	fread(header,sizeof(unsigned char),32,filePtr);
	//printf("Print : %d - %d - %d\n", (unsigned int)header[2], (unsigned int)header[10], 
			//(unsigned int)header[18]);
	unsigned int *data_size_pointer = (unsigned int*)(header+2);
	*data_size = *data_size_pointer;
	unsigned int *data_offset_pointer = (unsigned int*)(header+10);
	*data_offset = *data_offset_pointer;
	unsigned int *width_pointer = (unsigned int*)(header+18);
	*width = *width_pointer;
	int *h = (int *)&header[22];
	*height = *h;
	unsigned int *bits_per_pixel_pointer = (unsigned int*)(header+28);
	*bits_per_pixel = *bits_per_pixel_pointer;

	//calculate padding
	int pad = 0;
	while(((*width)*3+pad)%4 != 0){
		pad++;
	}
	*padding = pad;
	unsigned char* voidPtr = (unsigned char *)malloc(*data_size);
	rewind(filePtr);
	fread(voidPtr, *data_size, 1,filePtr);	
	*img_data = voidPtr;
  // TO HERE
  
  return 0;  
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close( unsigned char **img_data ){

  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
}

int bmp_mask( char* input_bmp_filename, char* output_bmp_filename, 
              unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max,
              unsigned char red, unsigned char green, unsigned char blue )
{
  unsigned int img_width;
  unsigned int img_height;
  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned int data_offset;
  unsigned char* img_data    = NULL;
  
  int open_return_code = bmp_open( input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data ); 
  
  if( open_return_code ){ printf( "bmp_open failed. Returning from bmp_mask without attempting changes.\n" ); return -1; }
 
  // YOUR CODE FOR Q2 SHOULD REPLACE EVERYTHING FROM HERE
  unsigned char* new_image = (unsigned char*)malloc(data_size);//allocate space for the new image
 // unsigned char* temp = img_data;
  unsigned char* temp = (unsigned char*)memcpy(new_image,img_data,data_size);
  if(temp==NULL){            //check memcpy 
	  printf("Bad input");
	  return(-1);
  }
  unsigned int num_colors = bits_per_pixel/8;
  unsigned char *pixel_data = temp + data_offset;
  int i = 0;
  int j = 0;
  for(i=y_min;i<=y_max;i++){
	  for(j=x_min;j<=x_max;j++){
		  //red, Green, Blue
		  pixel_data[ i*(img_width*num_colors+padding) + j*num_colors + 2 ] = red;
		  pixel_data[ i*(img_width*num_colors+padding) + j*num_colors + 1 ] = green;
		  pixel_data[ i*(img_width*num_colors+padding) + j*num_colors + 0 ] = blue;
	  }
  }
  //write into the new file
  FILE *newfile = fopen(output_bmp_filename, "wb");
  if(newfile==NULL){
	 printf("Bad file\n");
	 return(-1);
  }
  fwrite(temp,data_size,1,newfile);
  fclose(newfile);
  // TO HERE!
  
  bmp_close( &img_data );
  
  return 0;
}         

int bmp_collage( char* bmp_input1, char* bmp_input2, char* bmp_result, int x_offset, int y_offset ){

  unsigned int img_width1;
  unsigned int img_height1;
  unsigned int bits_per_pixel1;
  unsigned int data_size1;
  unsigned int padding1;
  unsigned int data_offset1;
  unsigned char* img_data1    = NULL;
  
  int open_return_code = bmp_open( bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1 ); return -1; }
  
  unsigned int img_width2;
  unsigned int img_height2;
  unsigned int bits_per_pixel2;
  unsigned int data_size2;
  unsigned int padding2;
  unsigned int data_offset2;
  unsigned char* img_data2    = NULL;
  
  open_return_code = bmp_open( bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2 ); return -1; }
  
  // YOUR CODE FOR Q3 SHOULD REPLACE EVERYTHING FROM HERE
  //incompatible images
  if(bits_per_pixel1!=bits_per_pixel2){
	  printf("incompatible images\n");
	  return(-1);
  }
  //number of colors
  unsigned int num_colors = bits_per_pixel1/8;
  
  //determine the width and the height of the new image
  int new_width = 0;
  int new_height = 0;
  if((img_width1>=img_width2)&&(img_height1>=img_height2)){
	  //determine width
		if(x_offset<=0){new_width = -x_offset+img_width1;}
		else if(x_offset>0 && x_offset<=(img_width1-img_width2)){new_width = img_width1;}
		else if(x_offset>0 && x_offset>(img_width1-img_width2)){new_width = x_offset+img_width2;}
	  //determine height
		if(y_offset<0){new_height = -y_offset+img_height1;}
		else if(y_offset>=0 && y_offset<=(img_height1-img_height2)){new_height = img_height1;}
		else if(y_offset>(img_height1-img_height2)){new_height=y_offset+img_height2;}
		
  }else if((img_width1<img_width2)&&(img_height1<img_height2)){
	  //determine width
		if(x_offset>=0){new_width = x_offset+img_width2;}
		else if(x_offset<0 && (img_width2-img_width1)>=(-x_offset)){new_width = img_width2;}
		else if(x_offset<0 && (img_width2-img_width1)<(-x_offset)){new_width = -x_offset+img_width1;}
	  //determine height
		if(y_offset>=0){new_height = y_offset+img_height2;}
		else if(y_offset<0 && (-y_offset)<=(img_height2-img_height1)){new_height = img_height2;}
		else if(y_offset<0 && (-y_offset)>(img_height2-img_height1)){new_height=-y_offset+img_height1;}
		
  }else if((img_width1>=img_width2)&&(img_height1<img_height2)){
  	  //determine width
		if(x_offset<=0){new_width = -x_offset+img_width1;}
		else if(x_offset>0 && (img_width1-img_width2)>=(x_offset)){new_width = img_width1;}
		else if(x_offset>0 && (img_width1-img_width2)<(x_offset)){new_width = x_offset+img_width2;}
	  //determine height
		if(y_offset>=0){new_height = y_offset+img_height2;}
		else if(y_offset<0 && (-y_offset)<=(img_height2-img_height1)){new_height = img_height2;}
		else if(y_offset<0 && (-y_offset)>(img_height2-img_height1)){new_height=-y_offset+img_height1;}
	
  }else if((img_width1<img_width2)&&(img_height1>=img_height2)){
  	  //determine width
		if(x_offset>=0){new_width = x_offset+img_width2;}
		else if(x_offset<0 && (img_width2-img_width1)>=(-x_offset)){new_width = img_width2;}
		else if(x_offset<0 && (img_width2-img_width1)<(-x_offset)){new_width = -x_offset+img_width1;}
	  //determine height
		if(y_offset<=0){new_height = -y_offset+img_height1;}
		else if(y_offset>0 && (y_offset)<=(img_height1-img_height2)){new_height = img_height1;}
		else if(y_offset>0 && (y_offset)>(img_height1-img_height2)){new_height=y_offset+img_height2;}	
  }

  //calculate new padding
	int pad = 0;
	while(((new_width)*3+pad)%4 != 0){
		pad++;
	}
	
  //allocate space for the new image
//	FILE* filePtr = fopen(bmp_input1,"rb");
//	if(filePtr == NULL){
	//	printf("Error: allocation fopen failed\n");
		//return(-1);
//	}
//	unsigned char *head = malloc(1000);
//	fread(head,sizeof(unsigned char),1000,filePtr); //read one bmp
//	unsigned int *data_offset_pointer = (unsigned int*)(head+10); //get its header size
//	unsigned int header_size = *data_offset_pointer;
    unsigned char* new_image = (unsigned char*)malloc((new_width*num_colors+pad)*new_height+data_offset1);
  //copy one header to the new image
	unsigned char* temp = (unsigned char*)memcpy(new_image,img_data1,data_offset1);
	if(temp==NULL){
		printf("Memcpy Error\n");
		return(-1);
	}
  //change the fields I need
	int total_file_size = ((new_width*num_colors+pad)*new_height+data_offset1);
	memcpy((new_image+18),&new_width,4);
	memcpy((new_image+22),&new_height,4);
	memcpy((new_image+2),&total_file_size,4);
	
 //loop to change the pixels
	unsigned char *pixel_data = new_image + data_offset1;
	unsigned char *pixel_data1 = img_data1 + data_offset1;
	unsigned char *pixel_data2 = img_data2 + data_offset2;
	
	int i =0; int j = 0;
	//firstly, set all to black 
	for(i=0;i<new_height;i++){                  
		for(j=0;j<new_width;j++){
		  pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 2 ] = 0;
		  pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 1 ] = 0;
		  pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 0 ] = 0;
		}
	}
	//secondly, draw the first background image
	if(x_offset>=0 && y_offset>=0){
		for(i=0;i<img_height1;i++){
			for(j=0;j<img_width1;j++){
				pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 2 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+2];
				pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 1 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+1];
				pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 0 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+0];
			}
		}
	}else if(x_offset>=0 && y_offset<0){
		for(i=0;i<img_height1;i++){
			for(j=0;j<img_width1;j++){
				pixel_data[ (i-y_offset)*(new_width*num_colors+pad) + j*num_colors + 2 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+2];
				pixel_data[ (i-y_offset)*(new_width*num_colors+pad) + j*num_colors + 1 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+1];
				pixel_data[ (i-y_offset)*(new_width*num_colors+pad) + j*num_colors + 0 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+0];
			}
		}		
	}else if(x_offset<0 && y_offset>=0){
		for(i=0;i<img_height1;i++){
			for(j=0;j<img_width1;j++){
				pixel_data[ i*(new_width*num_colors+pad) + (j-x_offset)*num_colors + 2 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+2];
				pixel_data[ i*(new_width*num_colors+pad) + (j-x_offset)*num_colors + 1 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+1];
				pixel_data[ i*(new_width*num_colors+pad) + (j-x_offset)*num_colors + 0 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+0];
			}
		}		
	}else if(x_offset<0 && y_offset<0){
		for(i=0;i<img_height1;i++){
			for(j=0;j<img_width1;j++){
				pixel_data[ (i-y_offset)*(new_width*num_colors+pad) + (j-x_offset)*num_colors + 2 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+2];
				pixel_data[ (i-y_offset)*(new_width*num_colors+pad) + (j-x_offset)*num_colors + 1 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+1];
				pixel_data[ (i-y_offset)*(new_width*num_colors+pad) + (j-x_offset)*num_colors + 0 ] = pixel_data1[i*(img_width1*num_colors+padding1)+j*num_colors+0];
			}
		}		
	}
    //lastly, draw the second image at the top
	if(x_offset<0 && y_offset<0){
		for(i=0;i<img_height2;i++){
			for(j=0;j<img_width2;j++){
				pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 2 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+2];
				pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 1 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+1];
				pixel_data[ i*(new_width*num_colors+pad) + j*num_colors + 0 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+0];
			}
		}		
	}else if(x_offset<0 && y_offset>=0){
		for(i=0;i<img_height2;i++){
			for(j=0;j<img_width2;j++){
				pixel_data[ (i+y_offset)*(new_width*num_colors+pad) + j*num_colors + 2 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+2];
				pixel_data[ (i+y_offset)*(new_width*num_colors+pad) + j*num_colors + 1 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+1];
				pixel_data[ (i+y_offset)*(new_width*num_colors+pad) + j*num_colors + 0 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+0];
			}
		}			
	}else if(x_offset>=0 && y_offset<0){
		for(i=0;i<img_height2;i++){
			for(j=0;j<img_width2;j++){
				pixel_data[ i*(new_width*num_colors+pad) + (j+x_offset)*num_colors + 2 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+2];
				pixel_data[ i*(new_width*num_colors+pad) + (j+x_offset)*num_colors + 1 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+1];
				pixel_data[ i*(new_width*num_colors+pad) + (j+x_offset)*num_colors + 0 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+0];
			}
		}			
	}else if(x_offset>=0 && y_offset>=0){
		for(i=0;i<img_height2;i++){
			for(j=0;j<img_width2;j++){
				pixel_data[(i+y_offset)*(new_width*num_colors+pad) + (j+x_offset)*num_colors + 2 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+2];
				pixel_data[(i+y_offset)*(new_width*num_colors+pad) + (j+x_offset)*num_colors + 1 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+1];
				pixel_data[(i+y_offset)*(new_width*num_colors+pad) + (j+x_offset)*num_colors + 0 ] = pixel_data2[i*(img_width2*num_colors+padding2)+j*num_colors+0];
			}
		}		
	}
	
  //Save the result in a new BMP file
  //write into the new file
  FILE *newfile = fopen(bmp_result, "wb");
  if(newfile==NULL){
	 printf("Bad file\n");
	 return(-1);
  }
  fwrite(new_image,((new_width*num_colors+pad)*new_height+data_offset1),1,newfile);
  fclose(newfile);
  // TO HERE!
      
  bmp_close( &img_data1 );
  bmp_close( &img_data2 );
  
  return 0;
}                  
