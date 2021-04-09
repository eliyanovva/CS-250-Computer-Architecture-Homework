#include <stdio.h>
#include <stdbool.h>
#include <math.h>
const int word_size = 16;

int main(int argc, char* argv[]) {

    char* file_name;
    int size_KB, ways, block_size;
    bool write_back= 0;
    bool write_through = 1;

    file_name = argv[1];
    size_KB = argv[2];
    ways = argv[3];
    if(argv[4]=="wb"){
        write_back = 1;
        write_through = 0;
    }
    else {
        write_through = 1;
        write_back = 0;
    }
    block_size = argv[5];

    int frames, sets, offset, index, tag;

    frames = size_KB*1000/block_size;
    sets = frames/ways;
    offset = log2(block_size);
    index = log2(sets);
    tag = word_size - offset-index;

// start IO
    FILE *fptr;
    fptr = fopen(file_name, 'r');
    if(fptr == NULL)
        return 0;

    int line_length = 1000;
    char* line[line_length];
    while(fgets(line, line_length, fptr))
        printf("%s\n", line);

    return 0;
}
