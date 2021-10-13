#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quicksort(void *lineptr[], int left, int right, int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right, int (*comp)(void *, void *));

/* Compare numerically */
int numcmp(const char *a, const char *b){
	int n1 = atoi(a);
	int n2 = atoi(b);
	if(n1 < n2)
		return -1;
	else if (n1 > n2)
		return 1;
	else
		return 0;
}

int main(int argc, char **argv)
{
	if (argc < 5){
        printf("ERROR: Not enough arguments\n");
        return -1;
    }
	// checking if the input received are numbers
	int n = 0;
    if (strcmp(argv[1], "-n") == 0){
		n = 1;
	}

	// reading the file
	FILE *in_file;
	char *in_file_name = argv[1 + n];
	in_file = fopen(in_file_name, "r");
	if (in_file == NULL){
        printf("ERROR: File not found\n");
		return -1;
	}
	void *values[1024];
	char buffer[255];
	int size = 0;
	while(fgets(buffer, sizeof(buffer), in_file)) {
		char *line;
		line = malloc(strlen(buffer));
		strncpy(line, buffer, strlen(buffer) - 1);
		//printf("%s\n", line);
		values[size] = line;
		size++;
	}
	fclose(in_file);

	// checking if the argument -o exist
	if (strcmp(argv[3 + n], "-o") != 0){
		printf("ERROR: Argument -o must be present for an output file\n");
		return -1;
	}

	// sorting the values
	char *sort = argv[2 + n];
	printf("... sorting %s file with %s\n", in_file_name, sort);
	if (strcmp(sort, "-mergesort") == 0){
		mergesort((void** ) values, 0, size - 1, (int (*)(void *, void *)) (n ? numcmp : strcmp));
	} else if (strcmp(sort, "-quicksort") == 0){
		quicksort((void** ) values, 0, size - 1, (int (*)(void *, void *)) (n ? numcmp : strcmp));
	} else {
		printf("ERROR: Sorting Algorithm not valid\n");
		return -1;
	}
	
	// writing the result to an output file
	FILE *out_file;
	char *out_file_name = argv[4 + n];
	out_file = fopen(out_file_name, "wb");
	for (int i = 0; i < size; i++){
		fputs(strcat(values[i], "\n"), out_file);
	}
	fclose(out_file);

	printf("... Results file can be found at ./%s\n", out_file_name);
    return 0;
}
