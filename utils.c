// utils.c
#include "header.h"

// ---------------- CRC32 (standard) ----------------
static uint32_t crc_table[256];
static int crc_table_inited = 0;
static void init_crc_table(){
    uint32_t poly = 0xEDB88320u;
    for (uint32_t i = 0; i < 256; i++){
        uint32_t c = i;
        for (int j = 0; j < 8; j++){
            if (c & 1) c = poly ^ (c >> 1);
            else c >>= 1;
        }
        crc_table[i] = c;
    }
    crc_table_inited = 1;
}
uint32_t crc32(const unsigned char *buf, size_t len){
    if(!crc_table_inited) init_crc_table();
    uint32_t c = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; i++){
        c = crc_table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFFu;
}

// portable reads/writes (little-endian)
void write_u32(FILE *f, uint32_t v){
    unsigned char b[4];
    b[0] = v & 0xFF; b[1] = (v>>8) & 0xFF; b[2] = (v>>16) & 0xFF; b[3] = (v>>24) & 0xFF;
    fwrite(b,1,4,f);
}
void write_u64(FILE *f, uint64_t v){
    unsigned char b[8];
    for(int i=0;i<8;i++) b[i] = (v >> (8*i)) & 0xFF;
    fwrite(b,1,8,f);
}
uint32_t read_u32(FILE *f){
    unsigned char b[4];
    if(fread(b,1,4,f) != 4) return 0;
    return (uint32_t)b[0] | ((uint32_t)b[1]<<8) | ((uint32_t)b[2]<<16) | ((uint32_t)b[3]<<24);
}
uint64_t read_u64(FILE *f){
    unsigned char b[8];
    if(fread(b,1,8,f) != 8) return 0;
    uint64_t v = 0;
    for(int i=0;i<8;i++) v |= ((uint64_t)b[i]) << (8*i);
    return v;
}
