#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "huffman.h"
#include "miniunit.h"

// //Part 7 : seg fault when i try to have read file helper function

// //static void _destroy_tree(void* tree){
// 	//Maybe  free right and left separately ?? if so we'll need an if statment ? If(tree== null) ? check that later

// 	//free((TreeNode*) tree);
// 	//return;
// //}

static void _printlist(Node* head) { 
	if(head == NULL)
	{
		//might need return;
	}
	printf("List: \n");

	int idx = 0;
	Node* prst = head;

	while(prst != NULL){
		uchar c = ((TreeNode*)(prst -> a_value)) -> character;
		long f = ((TreeNode*)(prst -> a_value)) -> frequency;
		printf("Index %d (%d):  %c -- %ld\n",idx ,(int)c, c , f);
		idx++;
		prst = prst -> next;
	}
	printf("\n----------------------\n");
	return;
}

void _print_helper(Frequencies freq, char* path, const char* error){
	if(!calc_frequencies(freq, path, &error)){
		printf("%s\n", error);
		return;
	}
}

int main(int argc, char* argv[]) {  

	Frequencies freq = {0};
    char* path = ("testing.txt"); 
    const char* error  = NULL;
    _print_helper( freq, path, error); 
    Node* list = make_huffman_pq(freq);
    _printlist(list);
    TreeNode* tree = make_huffman_tree(list);
	BitWriter writer007 = open_bit_writer("test.bits");
	//write_coding_table(tree, &writer007);
	 write_compressed(tree, &writer007, (unsigned char*) "huffman fluffs many mums"); 
	 close_bit_writer(&writer007);
    destroy_huffman_tree(&tree);

    return EXIT_SUCCESS;

    }


// static int _test_write_compressed() {
//     mu_start();

//     Frequencies f = {0};
//     const char* path = "testing.txt";
//     const char* error = NULL;
//     bool isSuccess = calc_frequencies(f, path, &error);
//     mu_check(isSuccess);
//     f['\n'] = 0; // to match online example
    
//     Node* a_head = make_huffman_pq(f);
//     mu_check(a_head != NULL);
//     TreeNode* tree = make_huffman_tree(a_head);
//     mu_check(tree != NULL);

//     BitWriter writer = open_bit_writer("test.bits");
//     unsigned char* inputString = (unsigned char*)("huffman fluffs many mums");
//     write_compressed(tree, &writer, inputString);
//     close_bit_writer(&writer);
    
//     destroy_huffman_tree(&tree);
//     mu_check(tree == NULL);

//     mu_end();
// }

// int main(int argc, char* argv[]) {

//     mu_run(_test_write_compressed);
//  //   mu_run(_test_fail);
    
//     return EXIT_SUCCESS;
// }