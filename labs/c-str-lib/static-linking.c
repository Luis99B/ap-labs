#include <stdio.h>

int mystrlen(char *);
char *mystradd(char *, char *);
int mystrfind(char *, char *);

int main(int argc, char** argv) {
    if (argc <= 3){
        printf("ERROR: Not enough arguments\n");
        return -1;
    }
    char *op = argv[1];
    char *origin = argv[2];
    if (mystrfind(op, "-add") == 0){
        char *addition = argv[3];
        printf("Initial Lenght      : %d\n", mystrlen(origin));
        char *new_word = mystradd(origin, addition);
        printf("New String          : %s\n", new_word);
        printf("New Lenght          : %d\n", mystrlen(new_word));
    } else if (mystrfind(op, "-find") == 0){
        char *substr = argv[3];
        int pos = mystrfind(origin, substr);
        if (pos != -1){
            printf("['%s'] string was found at [%d] position\n", substr, pos);
        } else {
            printf("ERROR: ['%s'] was not found\n", substr);
            return -1;
        }
    } else {
        printf("ERROR: The input should be '-add' or '-find'\n");
        return -1;
    }
    return 0;
}
