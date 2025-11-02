// main.c
#include "header.h"
#include <string.h>
void print_usage(const char *prog_path) {
    const char *prog_name = prog_path;

#ifdef _WIN32
    const char *p = strrchr(prog_path, '\\');
#else
    const char *p = strrchr(prog_path, '/');
#endif
    if (p) prog_name = p + 1;

    printf("Usage:\n");
    printf("  ./%s -c <input_file> <output_file>   # compress\n", prog_name);
    printf("  ./%s -d <input_file> <output_file>   # decompress\n", prog_name);
}


int main(int argc, char **argv){
    if(argc < 4){ 
    print_usage(argv[0]);
    return 1;
}
    if(strcmp(argv[1],"-c")==0){
        return compress_file(argv[2], argv[3]);
    } else if(strcmp(argv[1],"-d")==0){
        return decompress_file(argv[2], argv[3]);
    } else {
        print_usage(argv[0]);
        return 1;
    }
}
