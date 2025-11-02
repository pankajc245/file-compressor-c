// header.h
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Magic identifier for compressed file
#define HUF_MAGIC "HUF1"

// Basic types
typedef struct HuffmanNode {
    unsigned char symbol;        // byte value
    uint64_t freq;               // frequency
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

typedef struct {
    HuffmanNode **data;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    unsigned char *bits;
    int bit_count;
} BitBuffer;

// prototypes
// utils.c
uint32_t crc32(const unsigned char *buf, size_t len);
void write_u32(FILE *f, uint32_t v);
void write_u64(FILE *f, uint64_t v);
uint32_t read_u32(FILE *f);
uint64_t read_u64(FILE *f);

// huffman.c
MinHeap *create_min_heap(int capacity);
void destroy_min_heap(MinHeap *h);
void min_heap_push(MinHeap *h, HuffmanNode *node);
HuffmanNode *min_heap_pop(MinHeap *h);
HuffmanNode *build_huffman_tree(uint64_t freq[256]);
void free_huffman_tree(HuffmanNode *root);
void build_code_table(HuffmanNode *root, char *code, int depth, char **table);

// compress.c
int compress_file(const char *input_path, const char *output_path);

// decompress.c
int decompress_file(const char *input_path, const char *output_path);

// bitio (in huffman.c)
void bitbuf_init(BitBuffer *b);
void bitbuf_write_bit(BitBuffer *b, int bit);
void bitbuf_flush_and_write(BitBuffer *b, FILE *f);
int bitbuf_read_bit_from_byte(unsigned char byte, int pos); // helper

#endif
