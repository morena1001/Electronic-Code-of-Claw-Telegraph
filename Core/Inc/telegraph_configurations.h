/*
 * telegraph_configurations.h
 *
 *  Created on: Aug 18, 2024
 *      Author: josue
 */

#ifndef INC_TELEGRAPH_CONFIGURATIONS_H_
#define INC_TELEGRAPH_CONFIGURATIONS_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines
#define CLICK_PERIOD 2639
#define SCRATCH_PERIOD 2351
#define TAP_PERIOD 2094
#define SEND_PERIOD 3138

#define CLICK_COUNTER 220
#define SCRATCH_COUNTER 247
#define TAP_COUNTER 278
#define SEND_COUNTER 185

#define LETTER_END_COUNTER 250

#define BASE_STRING_LENGTH 50

// Private Variables
typedef struct Trie_Node trie_node;
extern char* input_string;
extern int input_string_length;
extern trie_node* root;

struct Trie_Node {
    char data;
    trie_node* children[3];
};

// Function Prototypes
trie_node* Telegraph_Config_Init();
void Add_Letter(char letter);
char Traverse_Tree(trie_node** current_node, char character);

trie_node* Make_Trie_Node (char data);
void Free_Trie_Node (trie_node* node);
trie_node* Trie_Init(void);



#endif /* INC_TELEGRAPH_CONFIGURATIONS_H_ */
