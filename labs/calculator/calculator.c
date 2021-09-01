#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    long res = 0;
    if (operator == 1){ // add
        for (int i = 0; i < nValues; i++){
            res += values[i];
        }
    } else if (operator == 2){ // sub
        res = values[0];
        for (int i = 1; i < nValues; i++){
            res -= values[i];
        }
    } else if (operator == 3){ // mult
        res = 1;
        for (int i = 0; i < nValues; i++){
            res *= values[i];
        }
    }
    return res;
}

int main(int argc, char **argv) {
    char *op = argv[1];
    int *values;
    int i = 0;
    if (argc <= 3){
        printf("ERROR: Not enough arguments\n");
        return -1;
    }
    for (int j = 2; j < argc; j++){
        int val = atoi(argv[j]);
        if (val == 0 && strcmp(&argv[j], "0") != 0) { // chech if val is not an int
            printf("ERROR: %s is not an int\n", argv[j]);
            return -1;
        }
        values[i] = val;
        i++;
    }
    int size = argc - 2;
    long res;
    if (strcmp(op, "add") == 0){ // operator is add
        res = calc(1, size, values);
        for (int i = 0; i < size - 1; i++){
            printf("%d + ", values[i]);
        }
        printf("%d = %d\n", values[size - 1], res);
    } else if (strcmp(op, "sub") == 0){ // operator is sub
        res = calc(2, size, values);
        for (int i = 0; i < size - 1; i++){
            printf("%d - ", values[i]);
        }
        printf("%d = %d\n", values[size - 1], res);
    } else if (strcmp(op, "mult") == 0){ // operator is mult
        res = calc(3, size, values);
        for (int i = 0; i < size - 1; i++){
            printf("%d * ", values[i]);
        }
        printf("%d = %d\n", values[size - 1], res);
    } else { // invalid operator
        printf("ERROR: %s is an Invalid Operator\n", op);
        return -1;
    }
    return 0;
}
