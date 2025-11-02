File Compressor (Huffman Coding)

A simple lossless file compression and decompression tool written in C, based on the Huffman Encoding Algorithm.

Features
--------
- Compresses and decompresses any binary or text file
- Uses Huffman coding for lossless compression
- Includes CRC32 verification for data integrity
- Modular and portable C code
- Works on Windows, Linux, and macOS

Project Structure
-----------------
main.c          → Entry point (CLI interface)
compress.c      → Handles compression logic
decompress.c    → Handles decompression logic
huffman.c       → Huffman tree creation & traversal
utils.c         → Helper functions (bit operations, CRC32)
header.h        → Shared declarations

Compilation
-----------
On Windows (no Makefile):
    gcc main.c compress.c decompress.c huffman.c utils.c -o file_compressor

On Linux or macOS (if Makefile available):
    make

Usage
-----
Usage:
  ./file_compressor -c <input_file> <output_file>   # compress
  ./file_compressor -d <input_file> <output_file>   # decompress

Examples:
  ./file_compressor -c sample.txt sample.huf
  ./file_compressor -d sample.huf output.txt

Example Output
--------------
Compressed 'sample.txt' -> 'sample.huf' (original 10240 bytes, compressed 7140 bytes)
Decompressed 'sample.huf' -> 'output.txt' (CRC verified ✅)

How It Works
------------
1. Counts frequency of each byte in the file
2. Builds a Huffman tree from frequencies
3. Assigns variable-length binary codes to each byte
4. Writes compressed data to .huf file
5. Decompression rebuilds the tree, restores original data
6. CRC32 ensures lossless verification

Requirements
------------
- GCC or any standard C compiler
- No external dependencies

License
-------
This project is licensed under the MIT License.
You are free to use, modify, and distribute it.

Author
------
Pankaj Chavan
B.Tech Computer Engineering | 5th Semester
