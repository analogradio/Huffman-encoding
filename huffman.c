#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "huffman.h"
#include "miniunit.h"

//seg fault failing 3/7 -- compiling with gcc (add test cases)
//update : 6/7  no seg fault no mem leak, failing at ÿ
//static void _maketl(TreeNode* root, Code code_table, BitCode ch);

static int _cmp_f(const void* o1, const void* o2){ //check cmp function
	int difference = (((TreeNode*)o1) -> frequency) - (((TreeNode*)o2) -> frequency); // Find diff betwen the frequencies of the  tree nodes o1 and o2
	if(difference == 0) { 
		if((((TreeNode*)o1) -> character) == '\0' || (((TreeNode*)o2) -> character) == '\0') { //TEST CODE WITHOUT THIS STATEMENT
			difference = -1; // DELETE THIS TO GET 100% COVG
			//difference = (const int)(((TreeNode*)o2) -> character) - (const int)(((TreeNode*)o1) -> character);  //sets the difference = tree node o1 - tree nodeo2
		}
		else{
			difference = (((TreeNode*)o1) -> character) - (((TreeNode*)o2) -> character);// moved here -> fixed seg fault
		}
	}
	return difference;
}

Node* make_huffman_pq(Frequencies freqs) {
	Node* head = NULL;
	int idx = 0;
	while(idx <= 255){  //maybe use for loop 
		if(freqs[idx] > 0) {
			TreeNode* bst_node = malloc(sizeof(*bst_node));
			uchar ch = idx; 
			*bst_node = (TreeNode) {.character = ch, .frequency = freqs[idx], .left = NULL, .right = NULL}; // fix that // // allocates and initializes  bst node
			pq_enqueue(&head, bst_node, _cmp_f);
		}
		idx++;
	}
	return head;
}

TreeNode* make_huffman_tree(Node* head) { //passing 6/7 no mem leak or seg fault
	if(head == NULL) { 
		return NULL;
		//return head; //return empty huffman tree
	}
	for(int idx = 0; head -> next != NULL; idx++) { //change to while loop for code quality
		Node* popnode1 = pq_dequeue(&head);
		size_t popnodefreq1  = ((TreeNode*)(popnode1 -> a_value)) -> frequency; 
		Node* popnode2 = pq_dequeue(&head);
		size_t popnodefreq2  = ((TreeNode*)(popnode2  -> a_value)) -> frequency;
		TreeNode* newnode = malloc(sizeof(*newnode));
		*newnode = (TreeNode){.character = '\0', .frequency = (popnodefreq1  + popnodefreq2), .left = popnode1 -> a_value, .right = popnode2 -> a_value };
		pq_enqueue(&head, newnode, _cmp_f);
		free(popnode1);
		free(popnode2);
	}
	TreeNode* tree_root = head -> a_value;
	free(head);
	return tree_root;
}
//still need to create destroy_huffman_tree to destroy the huffman tree created earlier

//add write_coding_table funtion that "Write the coding table represented in the Huffman tree to the file referenced by a_writer" //read the full article again

//fix coverage for below function (82%)
void write_coding_table(TreeNode* root, BitWriter* a_writer) { //compiling and 100% covg, but need to check output
	if(root == NULL) { // if root is null return and end function
		return;
	}
	else {

		write_coding_table(root -> left, a_writer); // go through root all the way with recursion
		write_coding_table(root -> right, a_writer);
		//printf("freq: %ld\n",root -> frequency);
		if(root -> character) {
			//printf("1 char\n");
			write_bits(a_writer, 0x01, 1); // write a 1 if there is a non zero character
			write_bits(a_writer, root -> character, 8);
		}
		else { // else write a zero if there is a zero
			//printf("0\n");
			write_bits(a_writer, 0, 1);
		}
	}
}

void destroy_huffman_tree(TreeNode** a_root) { 
	if(*a_root != NULL) {
		destroy_huffman_tree(&((*a_root) -> left));
		destroy_huffman_tree(&((*a_root) -> right));
		free(*a_root);
		*a_root = NULL;
	}
}

typedef struct _BitCode{
	uint8_t bits;
	int num_bits;
} BitCode;

static void _maketbl(TreeNode* root, BitCode* codetable, uint8_t bits, int count) {
	if(root != NULL) {
		_maketbl(root -> left, codetable, bits << 1, count + 1); 
		_maketbl(root -> right, codetable, (bits << 1) | 0x01, count + 1);
		if (root -> character) {
			codetable[root -> character] = (BitCode) {.bits = bits, .num_bits = count};
		}
	}
}

void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t* uncompressed_bytes) {
	BitCode codetable[256] = {{ .bits = 0x00, .num_bits = 0 }};
	_maketbl(root, codetable, 0x00, 0);
	for(int i = 0; uncompressed_bytes[i] != '\0'; i++) { // check if we need to use '\0' or sizeof
		write_bits(a_writer, codetable[uncompressed_bytes[i]].bits, codetable[uncompressed_bytes[i]].num_bits);
	}
}
