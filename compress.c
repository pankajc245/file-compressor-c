#include "header.h"

int compress_file(const char *input_path, const char *output_path) {
    // Open input
    FILE *in = fopen(input_path, "rb");
    if (!in) {
        fprintf(stderr, "Error: cannot open input '%s'\n", input_path);
        return 1;
    }

    // Get file size
    fseek(in, 0, SEEK_END);
    uint64_t in_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    // Allocate buffer
    unsigned char *buffer = malloc(in_size ? in_size : 1);
    if (!buffer) {
        fclose(in);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    if (in_size) fread(buffer, 1, in_size, in);
    fclose(in);

    // Compute CRC32 for verification
    uint32_t crc = crc32(buffer, (size_t)in_size);

    // Frequency table
    uint64_t freq[256] = {0};
    for (uint64_t i = 0; i < in_size; i++)
        freq[buffer[i]]++;

    // Build Huffman tree and code table
    HuffmanNode *root = build_huffman_tree(freq);
    char *code_table[256] = {0};
    char tmp[512];
    if (root)
        build_code_table(root, tmp, 0, code_table);

    // Open output
    FILE *out = fopen(output_path, "wb");
    if (!out) {
        fprintf(stderr, "Error: cannot open output file '%s'\n", output_path);
        free(buffer);
        free_huffman_tree(root);
        return 1;
    }

    // Write header
    fwrite(HUF_MAGIC, 1, 4, out);   // magic bytes
    write_u64(out, in_size);        // original size
    write_u32(out, crc);            // crc32

    // Count and write nonzero frequencies
    uint32_t nonzero = 0;
    for (int i = 0; i < 256; i++)
        if (freq[i]) nonzero++;

    write_u32(out, nonzero);
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            unsigned char sym = (unsigned char)i;
            fwrite(&sym, 1, 1, out);
            write_u64(out, freq[i]);
        }
    }

    // Compute total bit length
    uint64_t total_bits = 0;
    for (uint64_t i = 0; i < in_size; i++) {
        unsigned char s = buffer[i];
        if (code_table[s]) total_bits += strlen(code_table[s]);
        else total_bits += 8; // fallback
    }
    write_u64(out, total_bits);

    // Initialize bit buffer
    BitBuffer bitbuf;
    bitbuf_init(&bitbuf);

    // Write Huffman-encoded data
    for (uint64_t i = 0; i < in_size; i++) {
        unsigned char s = buffer[i];
        char *code = code_table[s];
        if (!code) {
            // fallback if code missing
            for (int b = 7; b >= 0; b--)
                bitbuf_write_bit(&bitbuf, (s >> b) & 1);
        } else {
            for (int j = 0; code[j]; j++)
                bitbuf_write_bit(&bitbuf, code[j] - '0');
        }
    }

    bitbuf_flush_and_write(&bitbuf, out);
    fflush(out); // ensure all data is written

    // ✅ Get compressed file size before closing
    fseek(out, 0, SEEK_END);
    long long compressed_size = ftell(out);
    fclose(out);

    // Cleanup
    free(buffer);
    for (int i = 0; i < 256; i++)
        if (code_table[i]) free(code_table[i]);
    free_huffman_tree(root);

    // Print results with compression ratio
    double ratio = 0.0;
    if (in_size > 0)
        ratio = (1.0 - ((double)compressed_size / (double)in_size)) * 100.0;

    printf("Compressed '%s' -> '%s'\n", input_path, output_path);
    printf("Original: %llu bytes | Compressed: %llu bytes | Saved: %.2f%%\n",
           (unsigned long long)in_size,
           (unsigned long long)compressed_size,
           ratio);

    return 0;
}
