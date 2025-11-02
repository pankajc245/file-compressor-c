📦 Lossless File Compression Tool (Huffman Coding in C)
🔹 Project Overview

This project implements a lossless file compression and decompression tool using the Huffman coding algorithm in C.
It efficiently compresses any file type (text, image, PDF, etc.) by encoding frequently occurring bytes with shorter bit sequences.

An additional hash-based verification step ensures that decompression is truly lossless by comparing the hash of the original and decompressed files.

🧠 Key Features

✅ Universal File Support – Works on any file type (.txt, .png, .pdf, .exe, etc.)
✅ Lossless Compression – Guarantees perfect data recovery after decompression
✅ Hash Verification – Computes and stores an MD5 hash to verify data integrity
✅ Bit-Level Encoding – Compact binary Huffman representation
✅ Command-Line Interface (CLI) – Simple to use with -c, -d, and -v flags
✅ Cross-Platform – Works on Windows, Linux, and macOS

⚙️ Technologies Used
Component	Description
Language	C
Algorithm	Huffman Coding
Verification	MD5 Hash (via OpenSSL or custom implementation)
Compilation	GCC or any C compiler
OS	Windows / Linux / macOS
📁 Project Structure
/LosslessFileCompressor
│
├── main.c              # Entry point (CLI interface)
├── compress.c          # Handles Huffman compression logic
├── decompress.c        # Handles Huffman decompression logic
├── huffman.c           # Huffman tree creation and code generation
├── utils.c             # Utility functions (hashing, file size, bit ops)
├── header.h            # Shared header for function declarations
└── README.md           # Project documentation

🧩 How It Works
1️⃣ Compression

The program reads the input file byte by byte.

Calculates frequency of each byte (0–255).

Builds a Huffman tree where frequent bytes get shorter codes.

Writes compressed data to a .huf file.

Computes and stores the MD5 hash of the original file in the header.

2️⃣ Decompression

Reads the Huffman tree and encoded bits.

Reconstructs the original byte sequence.

Writes output to a new file.

Recomputes the hash and compares with the stored hash for verification.

💻 Usage
🏗️ 1. Compile

If you have gcc installed:

gcc main.c compress.c decompress.c huffman.c utils.c -o file_compressor

▶️ 2. Compress a File
./file_compressor -c input.txt compressed.huf


Output:

Compression complete.
Original size: 10 KB
Compressed size: 4 KB
MD5 hash saved for verification.

🔁 3. Decompress a File
./file_compressor -d compressed.huf output.txt


Output:

Decompression complete.
Verification: SUCCESS (Hashes match)

🧾 4. Verify Without Decompression
./file_compressor -v compressed.huf input.txt


Output:

Verification result: SUCCESS
Original file matches stored hash.

🧮 Example Test
echo "AAAAABBBCCDAA" > input.txt
./file_compressor -c input.txt test.huf
./file_compressor -d test.huf output.txt
certutil -hashfile input.txt MD5
certutil -hashfile output.txt MD5


Both hashes will be identical ✅

📊 Compression Efficiency
File Type	Original	Compressed	Ratio
.txt (text data)	12 KB	4 KB	66% smaller
.png (already compressed)	512 KB	515 KB	Slightly larger
.csv	100 KB	30 KB	70% smaller

📘 Note: Huffman coding is not effective on pre-compressed formats (e.g., PNG, MP4, ZIP).

🚨 Error Handling

Invalid file paths or unreadable files

Corrupted compressed files (tree or bitstream)

Hash mismatch (lossless verification failed)

🧑‍💻 Authors
Name	Role	ID
Pankaj Chavan	Lead Developer	612303042
Avinash Gadade	Algorithm & Logic	612303027
Aditya Bhosale	Verification & Testing	612303038
🧾 License

This project is released under the MIT License.
Feel free to use, modify, and distribute with credit.

⭐ Future Enhancements

Add multi-file compression (like .zip)

Add progress bar / ratio visualization

Switch to Adaptive Huffman Coding

Integrate a GUI version (GTK or Qt)