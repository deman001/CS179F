#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "stddef.h"
//#include "string.h"

void conca(char* dest, char* source) {
    //printf("debug statement\n");
    int sized = strlen(dest);
    int sizes = strlen(source);
    int i = 0;
    int j = 0;
    printf("%d %d done ", sized, sizes);
    for (i = sized; i < sized + sizes; i++) {
        for (j = 0; j < sizes; j++) {
            dest[i] = source[j];
        }
    }
    printf("%d size of concatenated array\n", strlen(dest));
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n--) {
        if (*s1 != *s2) {
            return *(unsigned char *)s1 - *(unsigned char *)s2;
        } else if (*s1 == '\0') {
            return 0;
        }
        s1++;
        s2++;
    }
    return 0;
}


char *strstr(const char *haystack, const char *needle) {
    int needleLength = strlen(needle);
    int haystackLength = strlen(haystack);
    int i;
    for (i = 0; i <= haystackLength - needleLength; i++) {
        if (strncmp(haystack + i, needle, needleLength) == 0) {
            return (char*) haystack + i;
        }
    }
    return NULL;
}

void find(char* path, char* fileName) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    //printf("%s\n", fileName);

    fd = open(path, 0);
    if(fd < 0){
        printf("Cannot open %s\n", path);
        exit(0);
    }

    if(fstat(fd, &st) < 0){
        printf("Cannot stat %s\n", path);
        close(fd);
        exit(0);
    }

    switch(st.type){
        case T_FILE:
            if(strstr(path, fileName) != 0){
                printf("%s\n", path);
                close(fd);
                return;
            }
        break;

        case T_DIR:
            strcpy(buf,path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                if(strcmp(de.name, ".") == 0||strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, fileName);
            }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: find [fileName]\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
