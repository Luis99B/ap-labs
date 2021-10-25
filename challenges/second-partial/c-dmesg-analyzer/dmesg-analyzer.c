#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define REPORT_FILE "report.txt"

/* hashtable */
struct nlist{
    struct nlist *next;
    char *key;
    char *value;
};

#define HASHSIZE 100000

static struct nlist *hashtable[HASHSIZE];

// hashing fuction
unsigned hash(char *str){
    unsigned hashval;
    for(hashval = 0; *str != '\0'; str++){
        hashval = *str + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

// return the pointer to the initial key if an element is found
struct nlist *lookup(char *str){
	struct nlist *np;
	for (np = hashtable[hash(str)]; np != NULL; np = np -> next){
		if (strcmp(str, np -> key) == 0){
			return np;
        }
    }
	return NULL;
}

// add the key value to the hashtable
struct nlist *install(char *key, char *value){
    struct nlist *np;
    unsigned hashval;
    np = (struct nlist *) malloc(sizeof(*np));
    if(np == NULL || (np -> key =  key) == NULL){
        return NULL;
    }
    hashval = hash(key);
    np -> next = hashtable[hashval];
    hashtable[hashval] = np;
    if((np -> value = value) == NULL){
        return NULL;
    }
    return np;
}

// generate the log data
int generate(char *str){
    if(str[0] != '['){
        return -1;
    }
    int length = strlen(str);
    // timestamp
    int time_len = 0;
    char *curr = str;
    while(*curr != ']' && time_len < length){
        time_len++;
        curr++;
    }
    if(time_len == length){
        return 1;
    }
    time_len++;
    curr++;
    char *timestamp;
    timestamp = (char *) malloc(time_len * sizeof(char));
    strncpy(timestamp, str, time_len);
    //printf("timestamp |%s|\n", timestamp);
    // key & value
    int key_len = 0;
    int colon = 0;
    int trail = 0;
    while(*curr){
        if(key_len == 0 && *curr == ' '){
            trail++;
        } else if(colon && *curr==' '){
            break;
        } else if(*curr == ':' && !colon){
            colon = 1;
            key_len++;
        } else {
            key_len++;
        }
        curr++;
    }
    char *key;
    char *value;
    if(colon && key_len < length - time_len){
        key = (char *) malloc(key_len * sizeof(char));
        strncpy(key, str + time_len + trail, key_len);
        value = (char *) malloc(strlen(str) * sizeof(char) - trail - key_len + 1);
        strcpy(value, timestamp);
        strcat(value, curr);
    } else {
        key = (char *) malloc(strlen("General:") * sizeof(char) + 1);
        strcpy(key, "General:");
        value = (char *) malloc(strlen(str) * sizeof(char) + 1);
        strcpy(value, str);
    }
    //printf("key |%s|\n", key);
    //printf("value |%s|\n", value);
    install(key, value);
    return 0;
}

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage:./dmesg-analizer logfile.txt\n");
        return 1;
    }
    analizeLog(argv[1], REPORT_FILE);
    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    // log file
    int log_f = open(logFile, O_RDONLY);
    if (log_f == -1){
        printf("ERROR: File not found or can not be opened\n");
		exit(-1);
	}
    int size = 0;
    char buffer;
    while(read(log_f, &buffer, 1)){
        if(buffer == '\n'){
            size++;
        }
    }
    //printf("Analyzing: %d lines\n", size);
    lseek(log_f, 0, SEEK_SET);
    int line_len = 0;
    while(read(log_f, &buffer, 1)){
        if(buffer != '\n'){
            line_len++;
        } else {
            line_len++;
            lseek(log_f, -line_len, SEEK_CUR);
            char *line_buffer = (char *) malloc(line_len * sizeof(char));
            read(log_f, line_buffer, line_len);
            line_buffer[strcspn(line_buffer, "\n")] = 0;
            //printf("line buffer: %s\n", line_buffer);
            line_len = 0;
            generate(line_buffer);
        }
    }
    close(log_f);
    // report file
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // rw - rw - rw
    int report_f = open(report, flags, mode);
    if (report_f == -1){
        printf("ERROR: File can not be created/opened\n");
		exit(-1);
	}
    struct nlist *head;
    for(int i = 0; i <= HASHSIZE; i++){
        head = hashtable[i];
        if(head != NULL){
            struct nlist *curr;
            for(curr = head; curr != NULL; curr = curr -> next){
                if(curr == head){
                    write(report_f, strcat(curr -> key, "\n"), strlen(curr -> key) + 1);
                }
                write(report_f, "    ", 4);
                write(report_f, strcat(curr -> value, "\n"), strlen(curr -> value) + 1);
            }
        }
    }
    close(report_f);
    printf("Report is generated at: [%s]\n", report);
}
