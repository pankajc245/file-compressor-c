// huffman.c
#include "header.h"

// ---------------------- Min-heap (priority queue) -----------------------
MinHeap *create_min_heap(int capacity){
    MinHeap *h = malloc(sizeof(MinHeap));
    h->data = malloc(sizeof(HuffmanNode*) * capacity);
    h->size = 0;
    h->capacity = capacity;
    return h;
}
void destroy_min_heap(MinHeap *h){
    if(!h) return;
    free(h->data);
    free(h);
}
static void heap_swap(HuffmanNode **a, HuffmanNode **b){
    HuffmanNode *t = *a; *a = *b; *b = t;
}
void min_heap_push(MinHeap *h, HuffmanNode *node){
    if(h->size >= h->capacity){
        h->capacity *= 2;
        h->data = realloc(h->data, sizeof(HuffmanNode*) * h->capacity);
    }
    int i = h->size++;
    h->data[i] = node;
    // bubble up
    while(i && h->data[(i-1)/2]->freq > h->data[i]->freq){
        heap_swap(&h->data[i], &h->data[(i-1)/2]);
        i = (i-1)/2;
    }
}
HuffmanNode *min_heap_pop(MinHeap *h){
    if(h->size == 0) return NULL;
    HuffmanNode *ret = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while(1){
        int l = 2*i+1, r = 2*i+2, smallest = i;
        if(l < h->size && h->data[l]->freq < h->data[smallest]->freq) smallest = l;
        if(r < h->size && h->data[r]->freq < h->data[smallest]->freq) smallest = r;
        if(smallest == i) break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return ret;
}

// ---------------------- Huffman tree build & codes -----------------------
HuffmanNode *create_node(unsigned char symbol, uint64_t freq){
    HuffmanNode *n = malloc(sizeof(HuffmanNode));
    n->symbol = symbol;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}

HuffmanNode *build_huffman_tree(uint64_t freq[256]){
    // count distinct symbols
    int distinct = 0;
    for(int i=0;i<256;i++) if(freq[i]) distinct++;
    // if only one symbol: still build tree with root -> left = node
    MinHeap *h = create_min_heap(distinct ? distinct : 2);
    for(int i=0;i<256;i++){
        if(freq[i]) min_heap_push(h, create_node((unsigned char)i, freq[i]));
    }
    if(h->size == 0){
        destroy_min_heap(h);
        return NULL;
    }
    if(h->size == 1){
        // special: duplicate the node to make a tree
        HuffmanNode *only = min_heap_pop(h);
        HuffmanNode *parent = create_node(0, only->freq);
        parent->left = only;
        parent->right = create_node(0, 0);
        min_heap_push(h, parent);
    }
    while(h->size > 1){
        HuffmanNode *x = min_heap_pop(h);
        HuffmanNode *y = min_heap_pop(h);
        HuffmanNode *z = create_node(0, x->freq + y->freq);
        z->left = x;
        z->right = y;
        min_heap_push(h, z);
    }
    HuffmanNode *root = min_heap_pop(h);
    destroy_min_heap(h);
    return root;
}

void free_huffman_tree(HuffmanNode *root){
    if(!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

// build code table: table[byte] = strdup("0101...")
void build_code_table(HuffmanNode *root, char *code, int depth, char **table){
    if(!root) return;
    if(!root->left && !root->right){
        code[depth] = '\0';
        table[root->symbol] = strdup(code);
        return;
    }
    if(root->left){
        code[depth] = '0';
        build_code_table(root->left, code, depth+1, table);
    }
    if(root->right){
        code[depth] = '1';
        build_code_table(root->right, code, depth+1, table);
    }
}

// ---------------------- Bit buffer -----------------------
void bitbuf_init(BitBuffer *b){
    b->bits = malloc(1);
    b->bits[0] = 0;
    b->bit_count = 0;
}
void bitbuf_write_bit(BitBuffer *b, int bit){
    int byte_index = b->bit_count / 8;
    int bit_index = 7 - (b->bit_count % 8); // write MSB-first
    if(byte_index >= 0){
        b->bits = realloc(b->bits, byte_index + 1);
    }
    if(bit) b->bits[byte_index] |= (1 << bit_index);
    else b->bits[byte_index] &= ~(1 << bit_index);
    b->bit_count++;
}
void bitbuf_flush_and_write(BitBuffer *b, FILE *f){
    int total_bytes = (b->bit_count + 7) / 8;
    if(total_bytes > 0){
        fwrite(b->bits, 1, total_bytes, f);
    }
    free(b->bits);
    b->bits = NULL;
    b->bit_count = 0;
}

// helper for reading a single bit from a byte at given pos (0..7)
int bitbuf_read_bit_from_byte(unsigned char byte, int pos){
    int bit_index = 7 - pos;
    return (byte >> bit_index) & 1;
}
