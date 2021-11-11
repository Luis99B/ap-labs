#define _XOPEN_SOURCE 500

#include <stdio.h>
#include "logger.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <sys/inotify.h>

#define BUFFER_LEN 1024 * (sizeof(struct inotify_event) + NAME_MAX + 1)

int inotify_fd;

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    int n = 0;
    if(tflag == FTW_D){
        n = inotify_add_watch(inotify_fd, fpath, IN_MOVE | IN_CREATE | IN_DELETE);
        if(n == -1){
            errorf("Display info inotify error");
        }
    }
    return n;
}

int main(int argc, char** argv){
    if (argc < 2){
        warnf("Not enough arguments");
        exit(-1);
    }
    char *path = argv[1];
    inotify_fd = inotify_init();
    if (inotify_fd == -1){
        errorf("Could not initiate inotify");
        exit(-1);
    }
    int flags = 0;
    if (strchr(path, 'd') != NULL){
        flags |= FTW_DEPTH;
    }
    if (strchr(path, 'p') != NULL){
        flags |= FTW_PHYS;
    }
    int inotify_watch = nftw(path, display_info, 20, flags);
    if (inotify_watch == -1){
        errorf("Could not add a watch to inotify");
        exit(-1);
    }
    initLogger("stdout");
    infof("Starting File/Directory Monitor on %s", path);
    char buffer[BUFFER_LEN];
    ssize_t num_r;
    for (;;){
        num_r = read(inotify_fd, buffer, BUFFER_LEN);
        if (num_r == -1){
            panicf("There has been an error in read");
            exit(-1);
        } else if (num_r == 0) {
            panicf("There is nothing to read in inotify");
            exit(-1);
        }
        char *p;
        for (p = buffer; p < buffer + num_r;){
            struct inotify_event *event = (struct inotify_event *) p;
            if (event -> len){
                if (event -> mask & IN_CREATE){
                    if (event -> mask & IN_ISDIR){
                        infof("- [Directory - Create] - %s", event -> name);
                    } else {
                        infof("- [File - Create] - %s", event -> name);
                    }
                } else if (event -> mask & IN_DELETE){
                    if (event -> mask & IN_ISDIR){
                        infof("- [Directory - Removal] - %s", event -> name);
                    } else {
                        infof("- [File - Removal] - %s", event -> name);
                    }
                } else if (event -> cookie != 0){
                    if (event -> mask & IN_MOVED_FROM){
                        if(event -> mask & IN_ISDIR){
                            infof("- [Directory - Rename] - %s -> ", event -> name);
                        } else {
                            infof("- [File - Rename] - %s -> ", event -> name);
                        }
                    } else {
                        infof("%s",event -> name);
                    }
                } else {
                    errorf("error");
                }
                p +=  sizeof(struct inotify_event) + event -> len;
            }
        }
    }
    inotify_rm_watch(inotify_fd, inotify_watch);
    close(inotify_fd);
    return 0;
}
