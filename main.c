#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
const int word_size = 16;
const int mem_size = 65536;
const int block_size_max = 64;

struct block{
    bool valid;
    bool dirty;
    int tag_index;
    unsigned char c_bytes [64];
    int LRE;
};

void print_hexadecimal(unsigned char* str, int size){
    for(int i=0; i<size; i++){
        printf("%02x", str[i]);
    }
    printf("\n");
}

void load(int address, int access, int ways, int sets, struct block** cache, char* mem,  int block_size){

// hit scenario
    bool hit =0 ;
    int address_block = address / block_size;
    int check = address % block_size;
    unsigned char print_val [access];

    for (int i=0; i<ways; i++){
        for(int j=0; j<sets; j++){
            if(cache[i][j].tag_index<= address_block && cache[i][j].tag_index>=address_block && cache[i][j].valid==1){
                // obtain the correct block that needs to be ouputted
                for(int k = check; k<check+access; k++)
                    print_val[k-check] = cache[i][j].c_bytes[k];
//                printf(cache[i][j].c_bytes);
//                printf("\n %s\n", print_val);
                printf("load %d hit ", address);
                hit=1;
                print_hexadecimal(print_val, access);
                // update LRE chunk for the column
                if(cache[i][j].LRE !=0){
                    int prev_LRE = cache[i][j].LRE;
                    cache[i][j].LRE = 0;
                    // a column is a set, thus update the column values
                    for (int k = 0; k<ways; k++){
                        if(k!=i && cache[k][j].LRE<prev_LRE)
                            cache[k][j].LRE++;
                    }
                }
        }
    }
}

    //miss scenario
// ADD A DIRTY BIT WAY OF DOING THINGS - PUT THE CURRENT THING TO MEMORY IF DIRTY BIT IS 1

    if(hit==0){

        //print part
        int page = address_block *block_size;

        for(int k = check; k<check+access; k++)
            print_val[k-check] = mem[page + k];
//        printf(cache[i][j].c_bytes);
//        printf("\n %s\n", print_val);
        printf("load %d miss ", address);
        print_hexadecimal(print_val, access);

        // push to the cache from memory part

        int cur_set = page % sets;
        for(int i=0; i<ways; i++){
            //update LRE, set valid bit to 1, set
            if(cache[i][cur_set].LRE == ways-1){
                cache[i][cur_set].valid = 1;
                cache[i][cur_set].tag_index = address_block;
                cache[i][cur_set].LRE = 0;

                // check for dirty bit and push to memory current contents
                if(cache[i][cur_set].dirty==1){
                    for(int j = cache[i][cur_set].tag_index*block_size; j<block_size; j++)
                        mem[j] = cache[i][cur_set].c_bytes[j-cache[i][cur_set].tag_index*block_size];
                    cache[i][cur_set].dirty = 0;
                }
                for(int j=address; j<address +access; j++ ){
                    cache[i][cur_set].c_bytes[check + j - address] = mem[j];
                }

            // update the LRE values of the rest of the numebrs in the set
                for (int k = 0; k<ways; k++){
                    if(k!=i && cache[k][cur_set].LRE<ways-1)
                        cache[k][cur_set].LRE++;
                }

                break;
            }
        }
    }

}


void store(int address, int access, int ways, int sets, struct block** cache, char* mem,  int block_size, bool write_back, unsigned char* info){
    // write through and non-allocate
    //update the memory when updating the cache + if write miss write directly to memory, without bringing it up to the cache
    bool hit = 0;
    int address_block = address / block_size;
    int check = address % block_size;

    if(write_back == 0){
        //hit
        for (int i=0; i<ways; i++){
            for(int j=0; j<sets; j++){
                if(cache[i][j].tag_index == address_block && cache[i][j].valid==1){
                    //printf("error2\n");
                    // obtain the correct block that needs to be changed
                    // put the new information both in memory and in the cache
                    for(int k = check; k<check+access; k++){
                        cache[i][j].c_bytes[k] = info[k-check];
                       // printf("%d, i is %d, j is %d \n",cache[i][j].c_bytes[k], i, j);
                        mem[address + k -check] = info[k-check];
                        //printf("%d ", address + k - check);
                    }
                    printf("store %d hit\n", address);
                    hit=1;
                    // update LRE chunk for the column
                    if(cache[i][j].LRE !=0){
                        int prev_LRE = cache[i][j].LRE;
                        cache[i][j].LRE = 0;
                        // a column is a set, thus update the column values
                        for (int k = 0; k<ways; k++){
                            if(k!=i && cache[k][j].LRE<prev_LRE)
                                cache[k][j].LRE++;
                        }
                    }
                }
            }
        }

        if(hit==0){
            printf("store %d miss\n", address);

            for(int i = address; i<address + access; i++)
                mem[i] = info[i-address];
        }
    }

    if(write_back==1){
    //update lower level of memory upon removing the current value
    // update the cache only on a miss

    //search for a hit
    for (int i=0; i<ways; i++){
            for(int j=0; j<sets; j++){
                if(cache[i][j].tag_index == address_block && cache[i][j].valid==1){
                    //printf("error2\n");
                    // obtain the correct block that needs to be changed
                    // if it is a dirty block, push the current value to memory

                    if(cache[i][j].dirty == 1){
                        for(int k = cache[i][j].tag_index*block_size; k<block_size; k++)
                            mem[k] = cache[i][j].c_bytes[k-cache[i][j].tag_index*block_size];

                    }

                    // put the new value in the cache, dirty remains 1
                    for(int k = check; k<check+access; k++){
                        cache[i][j].c_bytes[k] = info[k-check];
                        }

                    printf("store %d hit\n", address);
                    hit=1;
                    // update LRE chunk for the column
                    if(cache[i][j].LRE !=0){
                        int prev_LRE = cache[i][j].LRE;
                        cache[i][j].LRE = 0;
                        // a column is a set, thus update the column values
                        for (int k = 0; k<ways; k++){
                            if(k!=i && cache[k][j].LRE<prev_LRE)
                                cache[k][j].LRE++;
                        }
                    }
                }
            }
        }

        if(hit==0){
            printf("store %d miss\n", address);

            int page = address_block *block_size;
            int cur_set = page % sets;

            for(int i=0; i<ways; i++){
                //update LRE, set valid bit to 1, set
                if(cache[i][cur_set].LRE == ways-1){
                    cache[i][cur_set].valid = 1;
                    cache[i][cur_set].tag_index = address_block;
                    cache[i][cur_set].LRE = 0;

                    // check for dirty bit and push to memory current contents
                    if(cache[i][cur_set].dirty==1){
                        for(int j = cache[i][cur_set].tag_index*block_size; j<block_size; j++)
                            mem[j] = cache[i][cur_set].c_bytes[j-cache[i][cur_set].tag_index*block_size];
                        cache[i][cur_set].dirty = 0;
                    }

                    for(int j=address; j<address +access; j++ ){
                        cache[i][cur_set].c_bytes[check + j - address] = info[j-address];
                    }

                    // update the LRE values of the rest of the numebrs in the set
                    for (int k = 0; k<ways; k++){
                        if(k!=i && cache[k][cur_set].LRE<ways-1)
                            cache[k][cur_set].LRE++;
                    }

                    break;
                }
            }
        }

    }
}

int main(int argc, char* argv[]) {

//    char* file_name;
    long size_KB = 128, ways = 2, block_size = 64;
    bool write_back= 0;
    bool write_through = 1;
    int frames, sets, offset, index, tag;

    frames = size_KB*1024/block_size;
    sets = frames/ways;
    offset = log2(block_size);
    index = log2(sets);
    tag = word_size - offset-index;


    struct block * cache [ways];
    for(int i=0; i<ways; i++)
        cache[i] = (struct block *)malloc(sets*sizeof(struct block));

    char memory[mem_size];

    for (int i=0; i<ways; i++)
        for(int j=0; j<sets; j++){
            cache[i][j].dirty = 0;
            cache[i][j].valid = 0;
            cache[i][j].LRE = ways-1-i;
        }

    for(int i=0; i<mem_size; i++)
        memory[i] = 0;

    //cache[0][0].valid = 1;
    //cache[0][0].tag_index = 0;

    //cache[0][0].c_bytes[0] = 0xde;
    //cache[0][0].c_bytes[1] = 0xad;
    //cache[0][0].c_bytes[2] = 0xbe;
    //cache[0][0].c_bytes[3] = 0xef;
    //cache[0][0].c_bytes[4] = 'e';
    //cache[0][0].c_bytes[5] = ' ';
    //cache[0][0].c_bytes[6] = 'm';



    //memory[0] = 0xde;
    //memory[1] = 0xad;
    //memory[2] = 0xbe;
    //memory[3] = 0xef;
    //memory[4] = 'e';
    //memory[5] = ' ';
    //memory[6] = 'm';


    store(0, 14, ways, sets,  cache, memory, block_size, 1, "!T!T!T!T!T!T!T");
    store(25, 4, ways, sets,  cache, memory, block_size, 1, "mama");
    load(0, 6, ways, sets,  cache, memory, block_size);
    load(0, 6, ways, sets,  cache, memory, block_size);
    store(0, 3, ways, sets,  cache, memory, block_size, 1, "MMM");
    load(0, 6, ways, sets,  cache, memory, block_size);
    load(0, 6, ways, sets,  cache, memory, block_size);


    //printf(cache[0][0].c_bytes);
    //printf("\n ");

    //for (int i=0; i<6; i++)
    //    printf("%c ", memory[i]);


//    file_name = argv[1];
//    size_KB = argv[2];
//    ways = argv[3];
//    if(argv[4]=="wb"){
//        write_back = 1;
//        write_through = 0;
//    }
 //   else {
 //       write_through = 1;
 //       write_back = 0;
  //  }
 //   block_size = argv[5];


// start IO
//    FILE *fptr;
//    fptr = fopen(file_name, 'r');
//    if(fptr == NULL)
 //       return 0;

 //   int line_length = 1000;
 //   char* line[line_length];
 //   while(fgets(line, line_length, fptr))
 //       printf("%s\n", line);

    return 0;
}
