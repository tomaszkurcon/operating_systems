#include <stdio.h>
#include <dirent.h>
#include <string.h>

void flipper(const char* in_path, const char* out_path) {
    DIR *pDir;
    pDir = opendir(in_path);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", in_path);
        return;
    }
    DIR *outDir;
    outDir = opendir(out_path);
    if(outDir==NULL) {
        if (mkdir(out_path)==-1) {
            printf ("Cannot create new folder '%s'\n", out_path);
            return;
        }
    }

    struct dirent *file;
    char path_to_file[100];
    char path_to_out_file[100];
    while((file = readdir(pDir)) != NULL) {
        if(strcmp(file->d_name + strlen(file->d_name)-4, ".txt") != 0) {
            printf("%s is not .txt file \n", file->d_name);
            continue;
        }
        snprintf(path_to_file, sizeof(path_to_file),"%s%s%s", in_path, "/",file->d_name);
        snprintf(path_to_out_file, sizeof(path_to_out_file),"%s%s%s", out_path, "/",file->d_name);
        printf("%s \n ", path_to_file);
        FILE *ptr =  fopen(path_to_file, "r");
        if(!ptr) {
            printf("Cannot open file");
            continue;
        }
        FILE *new_file = fopen(path_to_out_file, "w");
        if(!new_file) {
            printf("Cannot create new file \n");
            continue;
        }
        char line[255];
        while(fgets(line, 255, ptr)) {
            int len = strlen(line);
            for(int i = 0, j=len-2; i<j; i++, j--) {
                char buff = line[i];
                line[i]=line[j];
                line[j]=buff;
            }
            fputs(line, new_file);
        }
        fclose(ptr);
    }
    closedir(pDir);
}

int main ()
{
    flipper("./art", "./art_reversed");
    return 0;
}

