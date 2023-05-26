#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#define MAXI_PREF 100

int main() {
    trie_t trie;
    int alphabet_size = 26;
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    trie_init(&trie, alphabet_size, alphabet, NULL);
	char *prefix; 
	int criterion;
    char command[100]; 

    while (1) {
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strncmp(command, "INSERT", 6) == 0) {
            char word[100]; 
            sscanf(command, "INSERT %s", word);
            trie_insert(&trie, word, NULL);
        } else if (strncmp(command, "LOAD", 4) == 0) {
            char filename[100]; 
            sscanf(command, "LOAD %s", filename);
            load(&trie, filename);
        } 
        else if (strncmp(command, "AUTOCORRECT", 11) == 0) {
            char word[100]; 
            int k;
            sscanf(command, "AUTOCORRECT %s %d", word, &k);
            autocorrect(&trie, word, k);
        } else if (strncmp(command, "EXIT", 4) == 0) {
			trie_free(&trie);
			break;
		} else if (strncmp(command, "AUTOCOMPLETE", 12) == 0) {
			prefix = malloc(MAXI_PREF * sizeof(char));
			sscanf(command, "AUTOCOMPLETE %s %d", prefix, &criterion);
			autocomplete(&trie, prefix, criterion);
			free(prefix);
		} else if (strncmp(command, "REMOVE", 6) == 0) {
   	 	char word[100];
    	sscanf(command, "REMOVE %s", word);
    	trie_remove(&trie, word);
		} else if (strncmp( command, "PRINT", 5) == 0) {
            trie_print(&trie);
        } else if (strncmp( command, "SEARCH", 6) == 0) {
            char *word = malloc(MAXI_PREF * sizeof(char));
			sscanf(command, "SEARCH %s", word);
            if (trie_search(&trie, word)) {
                printf("Cuvantul %s exista in trie.\n", word);
            } else {
                printf("Cuvantul %s nu exista in trie.\n", word);
            }
			free(word);
        } else if (strncmp(command, "SAVE_IN_FILE", 12) == 0) {
            char filename1[100]; 
            sscanf(command, "SAVE_IN_FILE %s", filename1);
            trie_save(&trie, filename1);
        } else if (strncmp(command, "EXPORT_CSV", 10) == 0) {
            char filename2[100]; 
            // util pentru transferarea datelor dintr-o baza de date
            sscanf(command, "EXPORT_CSV %s", filename2);
            trie_export_csv(&trie, filename2);
            display_csv_file(filename2);
        } else if (strncmp(command, "PARTIAL_SEARCH", 14) == 0) {
            char *substring = malloc(MAXI_PREF * sizeof(char));
            sscanf(command, "PARTIAL_SEARCH %s", substring);
            search_partial_in_trie(&trie, substring);
            free(substring);
        
        } else if (strncmp(command, "WILDCARD", 8) == 0) { // se determina paronimele
            char *wildcard = malloc(MAXI_PREF * sizeof(char));
            sscanf(command, "WILDCARD %s", wildcard);
            wildcard_search_in_trie(&trie, wildcard);
        }
	}
    return 0;
}