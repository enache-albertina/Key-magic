# Key-magic
##Trie Data Structure in C##

Această implementare Trie în C permite introducerea, căutarea și autocorectarea cuvintelor.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Descrierea funcțiilor

*trie_init(trie_t *trie, int alphabet_size, const char *alphabet, void (*free_value_cb)(void *)): inițializează structura trie.

*trie_print_node(trie_node_t *node, char *str, int level), trie_print(trie_t *trie): funcții folosite pentru debugging, afișează conținutul trie-ului.

*my_strdup(const char *str): duplică un șir de caractere.

*trie_insert(trie_t *trie, const char *key, void *value): introduce un cuvânt în trie.

*load(trie_t *trie, const char *filename): încarcă cuvinte dintr-un fișier în trie.
autocorrect_helper(trie_node_t *node, char *word, int level, int k, char *output_word): ajută la autocorectarea cuvintelor.

* trie_node_free: Eliberează memoria alocată unui nod din trie și a tuturor descendenților săi, folosind o funcție de callback free_value_cb pentru a elibera memoria alocată valorii asociate cu fiecare nod.

*trie_free: Eliberează memoria alocată unui trie, eliberând fiecare nod și alfabetul asociat.

*autocomplete_recursive: O funcție recursivă folosită de funcția autocomplete pentru a genera sugestii de completare pentru un prefix dat. Rezultatele sunt colectate în structura AutocompleteResult.

*autocomplete: Generați sugestii de completare pentru un prefix dat, în funcție de un criteriu specificat. Criteriul poate fi lexicografic, lungime sau frecvență.

*trie_remove_helper și trie_remove: Îndepărtează un cuvânt din trie.

*trie_insert_reverse: Inserează un cuvânt în trie, dar în ordine inversă.

*trie_search: Caută un cuvânt în trie.

*trie_save, trie_export_csv, display_csv_file: Salvează conținutul trie-ului într-un fișier sau exportă în format CSV.

*search_partial_in_trie: Caută toate cuvintele din trie care conțin un anumit substring.

*wildcard_search_in_trie: Caută toate cuvintele din trie care se potrivesc cu un pattern wildcard specificat (de exemplu, "t*t" ar putea returna "tat", "tit", "tot", etc.).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Cum să rulezi codul
Pentru a rula codul, aveți nevoie de un compilator C.

Compilează codul folosind comanda gcc -o trie main.c trie.c. 
Rulează executabilul cu ./trie saub poti utiliza Makefile-ul atasat si rula comanda ./mk in terminal.
