#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#define MAX_WORD_LENGTH 100

// mocrouri necesare pentru a face distinctie pentru cuvintele din 
// functia autocomplete
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"
#define YELLOW(string) "\x1b[33m" string "\x1b[0m"


#define INDEX_TO_CHAR(i) ((char)((i) + 'a'))

#define CHAR_TO_INDEX(c) ((int)(c) - (int)'a')

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

typedef void (trie_free_value_cb)(void);

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
    /* Value associated with key (set if end_of_word = 1) */
void *value;

    /* 1 if current node marks the end of a word, 0 otherwise */
int end_of_word;

trie_node_t **children;
int n_children;
int insert_count;
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

// aceasta functie initializeaza structura trie
void trie_init(trie_t *trie, int alphabet_size, const char *alphabet, void
(*free_value_cb)(void *))
{
    trie->root = malloc(sizeof(trie_node_t));
    trie->root->n_children = alphabet_size;
    trie->root->children = calloc(alphabet_size, sizeof(trie_node_t *));
    trie->root->end_of_word = 0;
    trie->root->value = NULL;
    trie->root->insert_count = 0;

    trie->size = 0;
    trie->data_size = sizeof(void *);
    trie->alphabet_size = alphabet_size;
    trie->alphabet = malloc(alphabet_size * sizeof(char));
    memcpy(trie->alphabet, alphabet, alphabet_size * sizeof(char));
    trie->free_value_cb = free_value_cb;
    trie->n_nodes = 1; // Numărul de noduri inițial este 1 pentru rădăcină
    }

// functii folosite pentru debugging
void trie_print_node(trie_node_t *node, char *str, int level)
{
    if (node->end_of_word) {
        str[level] = '\0';
        printf("%s\n", str);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            str[level] = i + 'a';
            trie_print_node(node->children[i], str, level + 1);
        }
    }
}

void trie_print(trie_t *trie)
{
    char *buffer = malloc((trie->n_nodes + 1) * sizeof(char));
    printf("\n\n");
    trie_print_node(trie->root, buffer, 0);
    printf("\n\n");
    free(buffer);
}

char *my_strdup(const char *str) 
{
    size_t len = strlen(str) + 1;
    char *new_str = malloc(len);
    if (!new_str) return NULL;
    return (char *)memcpy(new_str, str, len);
}

void trie_insert(trie_t *trie, const char *key, void *value)
{
trie_node_t *node = trie->root;

// Parcurgem fiecare caracter din cuvantul cheie
for (int i = 0; key[i] != '\0'; i++) {
    char c = key[i];
    int idx = -1;

// Cautam indicele caracterului in alfabet
for (int j = 0; j < trie->alphabet_size; j++) {
if (trie->alphabet[j] == c) {
    idx = j;
    break;
}
}

// Daca nu am gasit caracterul in alfabet, returnam
if (idx == -1) 
return;

// Daca nu exista un nod copil pentru acest caracter, il cream
if (!node->children[idx]) {
    node->children[idx] = malloc(sizeof(trie_node_t));
    node->children[idx]->n_children = trie->alphabet_size;
    node->children[idx]->children = calloc
    (trie->alphabet_size, sizeof(trie_node_t *));
    node->children[idx]->end_of_word = 0;
    // if (value != NULL)
        node->children[idx]->value =  value;
    // Incrementeaza numarul de noduri
    trie->n_nodes++;
    node->children[idx]->insert_count = 1;
}
    // Ne mutam la nodul copil
    node = node->children[idx];
       
}
if (node->end_of_word) {
    ++node->insert_count;
    return;
}

// Marcam sfarsitul cuvantului si asociem valoarea
node->end_of_word = 1; 
trie->root->value = NULL;
trie->size++; // Incrementeaza numarul de cuvinte
// trie_print(trie);

}

void load(trie_t *trie, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Nu s-a putut deschide fisierul %s.\n", filename);
        return;
    }

    char word[100]; // presupunând că lungimea maximă a unui cuvânt este de 100 de caractere
    while (fscanf(file, "%s", word) != EOF) {
        trie_insert(trie, word, NULL); 
    }
    //trie_print(trie);
    fclose(file);
}

void autocorrect_helper(trie_node_t *node, char *word, int level, int k, char *output_word) {
    if (node == NULL) {
        return;
    }
    
    // Cuvântul este completat și verificăm dacă este un cuvânt valid
    if (level == (int)strlen(word)) {
        if (node->end_of_word && k >= 0) {
            output_word[level] = '\0';
            printf("%s\n", output_word);
        }
        return;
    }
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        output_word[level] = 'a' + i;
        int new_k = k;

        // Decrementăm k doar dacă am schimbat caracterul
        if (i != CHAR_TO_INDEX(word[level])) {
            new_k--;
        }
        
        if (new_k >= 0) {
            autocorrect_helper(node->children[i], word, level + 1, new_k, output_word);
        }
    }
}

void autocorrect(trie_t *trie, char *word, int k) {
    char *output_word = malloc(strlen(word) + 1);
    autocorrect_helper(trie->root, word, 0, k, output_word);
    free(output_word);
}

void trie_node_free(trie_node_t* node, void (*free_value_cb)(void* ),
int alphabet_size) 
{
    if (node == NULL) return;

    // Eliberam toate nodurile copil
    for (int i = 0; i < alphabet_size; i++) {
        trie_node_free(node->children[i], free_value_cb, alphabet_size);
    }

    if (node->children) {
        free(node->children);
        node->children = NULL;
    }

    if (free_value_cb) {
        free_value_cb(node->value);
    }

    // Eliberam nodul insusi
    free(node);
}

void trie_free(trie_t *trie) {
    // Eliberam toate nodurile din trie
    trie_node_free(trie->root, trie->free_value_cb, trie->alphabet_size);

    //  Eliberam alfabetul
    free(trie->alphabet);
}


typedef struct {
    char* smallest_lex;
    char* shortest_word;
    char* most_frequent_word;
    int shortest_length;
    int highest_frequency;
} AutocompleteResult;

void autocomplete_recursive(trie_node_t *node, const char *prefix, int level,
AutocompleteResult *result, char *original_prefix) 
{
    if (node == NULL) {
        return;
    }

    if (node->end_of_word && level == (int)strlen(prefix)) {
        char* word = malloc((level + 1) * sizeof(char));
        strncpy(word, prefix, level);
        word[level] = '\0';
       // printf("\n\n%s\n\n", prefix);
        if ((result->smallest_lex == NULL || strcmp(word, result->smallest_lex)
         < 0) && strncmp(original_prefix, prefix, (int)strlen(original_prefix)) == 0) {
            result->smallest_lex = realloc(result->smallest_lex, (level + 1) * sizeof(char));
            strcpy(result->smallest_lex, word);
        }

        if ((result->shortest_word == NULL || (int)strlen(word) < result
        ->shortest_length) && strncmp(original_prefix, prefix, (int)
        strlen(original_prefix)) == 0) {
            result->shortest_word = realloc(result->shortest_word, (level + 1) * sizeof(char));
            strcpy(result->shortest_word, word);
            result->shortest_length = strlen(word);
        }

        int frequency = node->insert_count;
        if (frequency > result->highest_frequency && strncmp(original_prefix, prefix, 
        (int)strlen(original_prefix)) == 0) {
            // printf("\n\n%s\n\n", prefix);
            result->most_frequent_word = realloc(result->most_frequent_word, (level + 1) * sizeof(char));
            strcpy(result->most_frequent_word, word);
            result->highest_frequency = node->insert_count;
        }
        free(word);
    }

    for (int i = 0; i < node->n_children; i++) {
        if (node->children[i] != NULL) {
            char new_prefix[level + 2];
            strncpy(new_prefix, prefix, level);
            new_prefix[level] = INDEX_TO_CHAR(i);
            new_prefix[level + 1] = '\0';
            autocomplete_recursive(node->children[i], new_prefix, level + 1, result, original_prefix);
        }
    }
}

void autocomplete(trie_t* trie, const char* prefix, int criterion) {
    AutocompleteResult result = {NULL, NULL, NULL, INT_MAX, -1};
    char* original_prefix = malloc((strlen(prefix) + 1) * sizeof(char));
    strcpy(original_prefix, prefix);
    autocomplete_recursive(trie->root, prefix, 0, &result, original_prefix);
    free(original_prefix);
    int sw = 0;
    if (criterion == 0 || criterion == 1) {
        printf("" BLUE("%s") "\n", result.smallest_lex);
        sw = 1;
    }
    if (criterion == 0 || criterion == 2) {
        printf(""RED("%s") "\n", result.shortest_word);
        sw = 1;
    }
    if (criterion == 0 || criterion == 3) {
        printf("" YELLOW("%s") "\n", result.most_frequent_word);
        sw = 1;
    }

    if( sw == 0 ) { 
        printf("No words found\n");
        if (criterion == 0) {
            printf("No words found\n");
            printf("No words found\n");
        }
    }
    free(result.smallest_lex);
    free(result.shortest_word);
    free(result.most_frequent_word);
}

void trie_remove_helper(trie_node_t** node, char* key, int index, int* removed) {
    if (*node == NULL) {
        return;
    }

    if (index == (int)strlen(key)) {
        if ((*node)->end_of_word == 1) {
            (*node)->end_of_word = 0;
            if ((*node)->value != NULL) {
                free((*node)->value);
                (*node)->value = NULL;
            }
            *removed = 1;
        }
        return;
    }

    int childIndex = key[index] - 'a';
    trie_remove_helper(&(*node)->children[childIndex], key, index + 1, removed);

    if (*removed && (*node)->children[childIndex] != NULL && (*node)
    ->children[childIndex]->end_of_word == 0 && (*node)->children[childIndex]
    ->value == NULL && (*node)->children[childIndex]->n_children == 0) {
        free((*node)->children[childIndex]);
        (*node)->children[childIndex] = NULL;
        (*node)->n_children--;
        *removed = 0;
    }
}

void trie_remove(trie_t* trie, char* key) {
    trie_node_t* node = trie->root;
    int removed = 0;
    trie_remove_helper(&node, key, 0, &removed);
    if (removed) {
        trie->size--;
    }
}

// aceasta functie insereaza in ordine inversa in trie
void trie_insert_reverse(trie_t *trie, const char *key, void *value)
{
    trie_node_t *node = trie->root;
    int key_len = strlen(key);

    for (int i = key_len - 1; i >= 0; i--) {
        char c = key[i];
        int idx = CHAR_TO_INDEX(c);

        if (!node->children[idx]) {
            node->children[idx] = malloc(sizeof(trie_node_t));
            node->children[idx]->n_children = trie->alphabet_size;
            node->children[idx]->children = calloc(trie->alphabet_size, sizeof(trie_node_t *));
            node->children[idx]->end_of_word = 0;
            node->children[idx]->value = NULL;
            trie->n_nodes++;
        }
        node = node->children[idx];
    }
    if (node->end_of_word) {
        return;
    }
    node->end_of_word = 1;
    node->value = value;
    trie->size++;
}

// functie care cauta intr-un in trie sa vada daca se gaseste un cuvant specificat 
bool trie_search(trie_t *trie, const char *key)
{
    trie_node_t *node = trie->root;

    // Parcurgem fiecare caracter din cuvantul cheie
    for (int i = 0; key[i] != '\0'; i++) {
        char c = key[i];
        int idx = -1;

        // Cautam indicele caracterului in alfabet
        for (int j = 0; j < trie->alphabet_size; j++) {
            if (trie->alphabet[j] == c) {
                idx = j;
                break;
            }
        }

        // Daca nu am gasit caracterul in alfabet, cuvantul nu exista
        if (idx == -1) 
            return false;

        // Daca nu exista un nod copil pentru acest caracter, cuvantul nu exista
        if (!node->children[idx]) {
            return false;
        }

        // Ne mutam la nodul copil
        node = node->children[idx];
    }

    // Verificam daca nodul curent marcheaza sfarsitul unui cuvant
    if (node && node->end_of_word) {
        return true;
    }

    return false;
}

// functii care salveaza continutul intr-un fisier daca se doreste a fi
// utilizat mai tarziu
void trie_save_node(trie_node_t *node, char *str, int level, FILE *file)
{
    if (node->end_of_word) {
        str[level] = '\0';
        fprintf(file, "%s\t%d\n", str, node->insert_count);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            str[level] = INDEX_TO_CHAR(i);
            trie_save_node(node->children[i], str, level + 1, file);
        }
    }
}

void trie_save(trie_t *trie, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului %s.\n", filename);
        return;
    }

    char *buffer = malloc((trie->n_nodes + 1) * sizeof(char));
    trie_save_node(trie->root, buffer, 0, file);

    fclose(file);
    free(buffer);
}

void trie_export_csv_helper(trie_node_t *node, char *buffer, int level, FILE *file)
{
    if (node->end_of_word) {
        buffer[level] = '\0';
        fprintf(file, "%s,%d\n", buffer, node->insert_count);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[level] = INDEX_TO_CHAR(i);
            trie_export_csv_helper(node->children[i], buffer, level + 1, file);
        }
    }
}

void trie_export_csv(trie_t *trie, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    // Parcurgeți nodurile trie și scrieți cuvintele și frecvențele în fișierul CSV
    char buffer[100];
    trie_export_csv_helper(trie->root, buffer, 0, file);

    // Eliberați resursele și închideți fișierul
    fclose(file);
}

void display_csv_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

// functie care cauta intr-un string si dupa continutul de litere
void search_partial(trie_node_t *node, char *current_word, int depth, char *substring) {
    if (node == NULL) {
        return;
    }
    // Daca nodul actual marcheaza sfarsitul unui cuvant si acel cuvant contine substring-ul, il afisam
    if (node->end_of_word && strstr(current_word, substring) != NULL) {
        printf("%s\n", current_word);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            // Adaugam caracterul corespunzator copilului curent la cuvantul curent
            current_word[depth] = INDEX_TO_CHAR(i);
            current_word[depth + 1] = '\0';
            // Apelam recursiv functia pentru copilul curent
            search_partial(node->children[i], current_word, depth + 1, substring);
        }
    }
}

void search_partial_in_trie(trie_t *trie, char *substring) {
    char current_word[MAX_WORD_LENGTH + 1] = {0};  // Initializez current_word cu un sir gol
    search_partial(trie->root, current_word, 0, substring);
}


// utilizatorul poate introduce cuvinte de forma t*t si programul va afisa ex: tit, tot
void wildcard_search(trie_node_t *node, char *current_word, char *wildcard, int depth) {
    if (node == NULL) {
        return;
    }

    if (*wildcard == '\0') {
        if (node->end_of_word) {
            printf("%s\n", current_word);
        }
    } else {
        if (*wildcard == '*') {
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                char c = INDEX_TO_CHAR(i);
                current_word[depth] = c;
                wildcard_search(node->children[i], current_word, wildcard + 1, depth + 1);
            }
        } else {
            int index = CHAR_TO_INDEX(*wildcard);
            current_word[depth] = *wildcard;
            wildcard_search(node->children[index], current_word, wildcard + 1, depth + 1);
        }
    }
    current_word[depth] = '\0';
}

void wildcard_search_in_trie(trie_t *trie, char *wildcard) {
    char current_word[100] = "";
    wildcard_search(trie->root, current_word, wildcard, 0);
}