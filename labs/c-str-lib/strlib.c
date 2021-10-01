
int mystrlen(char *str){
    char *word = str;
    int lenght = 0;
    while (*word != '\0'){
        word++;
        lenght++;
    }
    return lenght;
}

char *mystradd(char *origin, char *addition){
    char *new_str = origin;
    char *add = addition;
    while (*new_str != '\0'){
        new_str++;
    }
    while (*add != '\0'){
        *new_str = *add;
        new_str++;
        add++;
    }
    *new_str = '\0';
    return origin;
}

int mystrfind(char *origin, char *substr){
    char *str = origin;
    char *sub = substr;
    int len = mystrlen(substr);
    int found = 0;
    int pos = 0;
    while (*str != '\0' && *sub != '\0'){
        if (*str == *sub){
            sub++;
            found++;
        } else {
            pos++;
        }
        if (len == found){
            return pos;
        }
        str++;
    }
    return -1;
}
