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
    unsigned int x = strlen(argv[1]);

// TODO: fix this overflow please
    if(argc + x < argc )
        printf("smaller");
    else
        printf("bigger");



	char source[MAXBUFLEN + 1];
	FILE *fp = fopen("foo.txt", "r");
	if (fp != NULL) {
		size_t newLen = fread(source, sizeof(char), fsize(fp), fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
			source[newLen++] = '\0'; /* Just to be safe. */
		}

		fclose(fp);
	}




    return 0;
}