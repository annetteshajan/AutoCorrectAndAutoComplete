# AutoCorrectAndAutoComplete
Performs autocorrection and autocompletion on input words using trie data structure

Building trie: A dictionary file is scanned and every character of the input word(key) is inserted as an individual Trie node. 
If the input key is new or an extension of existing key, we need to construct non-existing nodes of the key, and mark end of word for last node. 
If the input key is prefix of existing key in Trie, we simply mark the last node of key as end of word. 
The key length determines Trie depth.

Autocomplete: Autocomplete uses trie search to look up common prefix and gives priority to the count of the word.

Predictive text: Using the concept of n grams, the next word is predicted by organising sentences as a trie.

User customisation: User is given the option to insert his own words into the trie. These words are are predicted during autocomplete. These words are also saved in the same dictionary file.
