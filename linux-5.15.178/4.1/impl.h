#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>


#define SET_XATTR 188
#define GET_XATTR 191
#define LIST_XATTR 194
#define REMOVE_XATTR 197



int set_xattr(const char *path, const char *name, const char *value){
    return syscall(SET_XATTR, path, name, value, strlen(value))+1;
};

// Function to get an extended attribute
// If name does not exist, return -1
// If name exists, return 1 and copy value to dst
char* get_xattr(const char *path, const char *name){
    // The syscall number for reading key-value pairs
    char *dst = malloc(1024); // Allocate memory for the value
    ssize_t len = syscall(GET_XATTR, path, name, dst, 1024);
    if (len < 0) {
        perror("get_xattr failed");
        return NULL; // Return NULL if the attribute does not exist or an error occurred
    }
    dst[len] = '\0'; // Null-terminate the value
    return dst; // Return the value
};

// Function to remove an extended attribute
int remove_xattr(const char *path, const char *name){
    return removexattr(path, name) + 1;
};

int list_xattrs(const char *path) {
    char list[1024];
    ssize_t len = syscall(LIST_XATTR, path, list, 1024);
    printf("Extended attributes for %s:\n", path);
    list[len] = '\0'; // Null-terminate the list
    char *p = list;
    while (*p) {
        printf(" - %s\n", p);
        p += strlen(p) + 1; // Move to the next attribute
    }
    return 0;
}

 void get_inode_info(const char *path) {
    struct stat file_stat;

    if (stat(path, &file_stat) == -1) {
        perror("Failed to get file stats");
        return;
    }

    printf("Inode information for file: %s\n", path);
    printf("Inode number: %lu\n", file_stat.st_ino);
    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("File permissions: %o\n", file_stat.st_mode & 0777);
 }