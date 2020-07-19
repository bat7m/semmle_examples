#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXBUFLEN 80

typedef struct{
	char sourcedata[MAXBUFLEN + 1];
} SOURCE_DATA;

static SOURCE_DATA gSrcData;

int init_src_data(){
	memset(gSrcData.sourcedata,0,sizeof(MAXBUFLEN + 1));
	return 0;
}
int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
}

int min (int a, int b){
	if (a<b){
		return a;
	}
	return b;
}

int overflow_local_simple(){
	char source1[MAXBUFLEN + 1];
	FILE *fp = fopen("foo.txt", "r");
	
	if (fp != NULL) {
		int fileSize1 = fsize(fp);
		//overflow
		size_t newLen = fread(source1, sizeof(char), fileSize1, fp);

		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source1[newLen++] = '\0'; /* Just to be safe. */
		}

		fclose(fp);
	}
	return 0;
}	

int no_overflow_local_simple(){
	char source2[MAXBUFLEN + 1];
	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		int fileSize2 = fsize(fp);
		//no overflow
		if (sizeof (source2) > fileSize2) {

			size_t newLen = fread(source2, sizeof(char), fileSize2, fp);
			if ( ferror( fp ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
				source2[newLen++] = '\0'; /* Just to be safe. */
			}
		}

		fclose(fp);
	}
	return 0;
}

int no_overflow_local_with_taint(){
	char source3[MAXBUFLEN + 1];
	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		int fileSize3 = fsize(fp);
		int x = fileSize3;
		//no overflow
		if (sizeof (source3) > x) {

			size_t newLen = fread(source3, sizeof(char), fileSize3, fp);
			if ( ferror( fp ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
				source3[newLen++] = '\0'; /* Just to be safe. */
			}
		}

		fclose(fp);
	}	
	return 0;
}

int no_overflow_min(){
	char source4[MAXBUFLEN + 1];
	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		int fileSize4 = fsize(fp);
		int size4 = min (fileSize4, sizeof (source4)); 
		//no overflow

		size_t newLen = fread(source4, sizeof(char), size4, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source4[newLen++] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}	
	
	return 0;
}

int no_overflow_const_size(){
	char source5[MAXBUFLEN];
	int fileSize5 = MAXBUFLEN; 
	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {

		//no overflow
		size_t newLen = fread(source5, sizeof(char), fileSize5, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source5[newLen++] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}	

	return 0;
}

int no_overflow_sizeof(){
	char source6[MAXBUFLEN];

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {

		//no overflow
		size_t newLen = fread(source6, sizeof(char), sizeof(source6), fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source6[newLen++] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}	
	return 0;
}

int no_overflow_local_malloc(){
   char* source7 = (char *) malloc(MAXBUFLEN + 1);
   if (source7 == NULL) 
   {
      return -1;
   }

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		int fileSize7 = MAXBUFLEN + 1;

		size_t newLen = fread(source7, sizeof(char), fileSize7, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source7[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	} 
	free (source7);
	return 0;
}
int init_buf(char** buf, int* size){
	*size = MAXBUFLEN + 1;
	*buf = (char *) malloc(*size);
	return 0;
}

int no_overflow_global_const_size(){
	init_src_data();
	int size_g = MAXBUFLEN + 1;

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		
		size_t newLen = fread(gSrcData.sourcedata, sizeof(char), size_g, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			gSrcData.sourcedata[newLen++] = '\0'; /* Just to be safe. */
		}
		

		fclose(fp);
	}
	return 0;
}


int no_overflow_function_call(){
   char* source8;
   int size8;
   init_buf(&source8, &size8);

	FILE *fp = fopen("foo.txt", "r");
	if ((fp != NULL) && (source8 != NULL)) {


		size_t newLen = fread(source8, sizeof(char), size8, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source8[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	return 0;
}
int read_func(char** source_r, int* size_r){

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {


		size_t newLen = fread(*source_r, sizeof(char), *size_r, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			*source_r[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	return 0;
}
int no_overflow_function_that_reads(){
   char* source9;
   int size9;
   init_buf(&source9, &size9);
   read_func(&source9, &size9);
   return 0;
}

int g_call_function_that_reads(char** source, int* size){
   read_func(source, size);
   return 0;
}
int no_overflow_f_call_g_call_function_that_reads(){
   char* source10;
   int size10;
   init_buf(&source10, &size10);
   g_call_function_that_reads (&source10, &size10);
   return 0;
}
int g_call_function_that_reads_with_taint(char** source_taint, int* size_taint){
   int x = *size_taint;
   read_func(source_taint, &x);
   *size_taint = x;
   return 0;
}
int no_overflow_f_call_g_call_function_that_reads_with_taint(){
   char* source11;
   int size11;
   init_buf(&source11, &size11);
   g_call_function_that_reads (&source11, &size11);
   return 0;
}

int get_size(char *src){
	//  deepcode ignore SizeofPassedArray: intentional error. looking for others
	return sizeof(src); 
}
int no_overflow_size_from_func(){
	char source12[MAXBUFLEN];

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {

		//no overflow
		size_t newLen = fread(source12, sizeof(char), get_size(source12), fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source12[newLen++] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}	
	return 0;
}

int no_overflow_func_malloc(){
   char* source20;
   int size20;
   init_buf(&source20, &size20);

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {


		size_t newLen = fread(source20, sizeof(char), size20, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source20[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	free(source20);
	return 0;
}

int no_overflow_func_malloc_const(){
   char* source21;
   int size21;
   init_buf(&source21, &size21);

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {


		size_t newLen = fread(source21, sizeof(char), MAXBUFLEN+1, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source21[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	free(source21);
	return 0;
}

int no_overflow_func_malloc_var(int size){
   char* source22;

   init_buf(&source22, &size);

	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {


		size_t newLen = fread(source22, sizeof(char), size, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source22[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	free(source22);
	return 0;
}

int init_large_buf(char** buf, int* size){
	*size = MAXBUFLEN + MAXBUFLEN;
	*buf = (char *) malloc(*size);
	return 0;
}
int no_overflow_func_malloc_read_less(int x){
   char* source23;
   int size23;
   init_large_buf(&source23, &size23);

	FILE *fp = fopen("foo.txt", "r");
	if ( (fp != NULL) && (source23 != NULL)) {


		size_t newLen = fread(source23, sizeof(char), x+MAXBUFLEN, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source23[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	free(source23);
	return 0;
}

int no_overflow_func_malloc_read_less_one_scope(int x){
   char* source24;
   int size24;
   	size24 = MAXBUFLEN + MAXBUFLEN;
	source24 = (char *) malloc(size24);

	FILE *fp = fopen("foo.txt", "r");
	if ((fp != NULL) && (source24 !=NULL)) {


		size_t newLen = fread(source24, sizeof(char), x+MAXBUFLEN, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source24[newLen++] = '\0'; /* Just to be safe. */
		}
		
		fclose(fp);
	}
	free(source24);
	return 0;
}

int may_overflow_local(int k){
	char source30[MAXBUFLEN + 1];
	size_t newLen = 0;
	FILE *fp = fopen("foo.txt", "r");
	
	if (fp != NULL) {
		int fileSize30 = fsize(fp);
		
		if (k < 7){
			//overflow
			newLen = fread(source30, sizeof(char), fileSize30, fp);
		}
		else{
			fileSize30 = min (fileSize30, sizeof (source30)); 
			//no overflow

			newLen = fread(source30, sizeof(char), fileSize30, fp);			
		}
		

		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source30[newLen++] = '\0'; /* Just to be safe. */
		}

		fclose(fp);
	}
	return 0;
}	

int main(unsigned int argc, char* argv[])
{
	overflow_local_simple(); //1
	no_overflow_local_simple(); //2
	no_overflow_local_with_taint(); //3
	no_overflow_min(); //4
	no_overflow_const_size(); //5
	no_overflow_sizeof(); //6 sizeof
	no_overflow_local_malloc(); //7
	no_overflow_global_const_size(); //size_g
	no_overflow_function_call(); //8
	no_overflow_function_that_reads(); //9
	no_overflow_f_call_g_call_function_that_reads(); //10
	no_overflow_f_call_g_call_function_that_reads_with_taint();//11
	no_overflow_size_from_func(); //12 get_size()
	no_overflow_func_malloc(); //20
	no_overflow_func_malloc_const(); //21
	no_overflow_func_malloc_var(MAXBUFLEN+1); //22
	no_overflow_func_malloc_read_less(1); //23
	no_overflow_func_malloc_read_less_one_scope(1); //24
	may_overflow_local(5); //30
    return 0;
}