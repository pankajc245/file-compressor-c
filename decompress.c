// decompress.c
#include "header.h"

int decompress_file(const char *input_path, const char *output_path){
    FILE *in = fopen(input_path,"rb");
    if(!in){ fprintf(stderr,"Error: cannot open '%s'\n", input_path); return 1; }

    // read and verify magic
    char magic[4];
    if(fread(magic,1,4,in) != 4){ fclose(in); fprintf(stderr,"Invalid file\n"); return 1; }
    if(memcmp(magic, HUF_MAGIC, 4) != 0){ fclose(in); fprintf(stderr,"Not a valid huffman file\n"); return 1; }

    uint64_t original_size = read_u64(in);
    uint32_t stored_crc = read_u32(in);

    uint32_t nonzero = read_u32(in);
    uint64_t freq[256] = {0};
    for(uint32_t i=0;i<nonzero;i++){
        int sym = fgetc(in);
        uint64_t f = read_u64(in);
        if(sym < 0){ fclose(in); fprintf(stderr,"Corrupt freq table\n"); return 1; }
        freq[(unsigned char)sym] = f;
    }

    uint64_t total_bits = read_u64(in);

    // rebuild tree
    HuffmanNode *root = build_huffman_tree(freq);
    if(!root && original_size > 0){ fclose(in); fprintf(stderr,"Error: no huffman tree\n"); return 1; }

    // read remaining bytes (bitstream)
    // compute how many bytes to read for total_bits
    uint64_t bytes_needed = (total_bits + 7) / 8;
    unsigned char *bitbytes = malloc(bytes_needed ? bytes_needed : 1);
    if(bytes_needed){
        size_t got = fread(bitbytes, 1, bytes_needed, in);
        if(got != bytes_needed){
            // if file shorter, proceed with available bytes (error)
        }
    }

    // decode bits walking tree
    unsigned char *outbuf = malloc(original_size ? original_size : 1);
    uint64_t outpos = 0;
    HuffmanNode *cur = root;
    uint64_t bit_index = 0;
    while(bit_index < total_bits && outpos < original_size){
        uint64_t byte_pos = bit_index / 8;
        int bit_pos_in_byte = bit_index % 8;
        int bit = bitbuf_read_bit_from_byte(bitbytes[byte_pos], bit_pos_in_byte);
        if(bit == 0) cur = cur->left;
        else cur = cur->right;
        if(!cur->left && !cur->right){
            outbuf[outpos++] = cur->symbol;
            cur = root;
        }
        bit_index++;
    }

    // write output
    FILE *out = fopen(output_path,"wb");
    if(!out){ free(outbuf); free(bitbytes); fclose(in); free_huffman_tree(root); fprintf(stderr,"Cannot open output\n"); return 1; }
    if(original_size) fwrite(outbuf,1,original_size,out);
    fclose(out);

    // compute CRC and compare
    uint32_t crc_new = crc32(outbuf, (size_t)original_size);
    if(crc_new == stored_crc) printf("Verification OK (CRC32 matches)\n");
    else printf("Verification FAILED (stored: %08x, computed: %08x)\n", stored_crc, crc_new);

    // cleanup
    free(outbuf);
    free(bitbytes);
    fclose(in);
    free_huffman_tree(root);

    printf("Decompressed to '%s' (%llu bytes)\n", output_path, (unsigned long long)original_size);
    return 0;
}
