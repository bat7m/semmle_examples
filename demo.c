#include <stdio.h>
#include <string.h>
#define MAXBUFLEN 80


int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
}

int main(unsigned int argc, char* argv[])
{

	char source[MAXBUFLEN + 1];
	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		int fileSize = fsize(fp);
		//overflow
		size_t newLen = fread(source, sizeof(char), fileSize, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source[newLen++] = '\0'; /* Just to be safe. */
		}

		fclose(fp);
	}
	
	char source2[MAXBUFLEN + 1];
	fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		int fileSize2 = fsize(fp);
		//no overflow
		size_t newLen = fread(source2, sizeof(char), fileSize2, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source[newLen++] = '\0'; /* Just to be safe. */
		}

		fclose(fp);
	}

    return 0;
}