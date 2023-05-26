#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define CHAR_TO_INDEX(c) ((int)(c) - (int)'a')

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
    /* Value associated with key (set if end_of_word = 1) */
void *value;

    /* 1 if current node marks the end of a word, 0 otherwise */
int end_of_word;

trie_node_t **children;
int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
trie_node_t *root;

    /* Number of keys */
int size;

    /* Generic Data Structure */
int data_size;

    /* Trie-Specific, alphabet properties */
int alphabet_size;
char *alphabet;

    /* Callback to free value associated with key,
    should be called when freeing */
void (*free_value_cb)(void *);
    /* Optional - number of nodes, useful to test correctness */
int n_nodes;
};

void trie_init(trie_t *trie, int alphabet_size, const char *alphabet, void
(*free_value_cb)(void *));

void trie_insert(trie_t *trie, const char *key, void *value);
void load(trie_t *trie, const char *filename);
void autocorrect(trie_t *trie, char *word, int k);

void trie_free(trie_t *ptrie);
void autocomplete(trie_t *trie, char *prefix, int criterion);
char *my_strdup(const char *str);
void trie_remove(trie_t *trie, const char *key);
void trie_print(trie_t *trie);
bool trie_search(trie_t *trie, const char *key);
void trie_save(trie_t *trie, const char *filename);

void trie_export_csv(trie_t *trie, const char *filename);
void display_csv_file(const char *filename);
void search_partial_in_trie(trie_t *trie, char *substring);
void wildcard_search_in_trie(trie_t *trie, char *wildcard);
