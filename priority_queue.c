#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "priority_queue.h"

//pq is a linkedlist with obj sorted in a certain way 

Node* pq_enqueue(Node** a_head, void* a_value, int (*cmp_fn)(const void*, const void*)) {  // creating and initializing nodes and allocating memory
	//inserting nodes in pq
	Node* starthead = *a_head; 
	Node* newnode = malloc(sizeof(*newnode));  //allocating memory to newnode
	newnode -> a_value = a_value; //faulting check statement again later
	if((*a_head) == NULL || cmp_fn((*a_head) -> a_value, a_value) > 0){
		/*that if statement checks for if *a_head equals null then it will set the *a_head to newnode it also checks if the cmp_fn returns a negative
		  if it does
		  then it means that newnode is smaller than *a_head*/
		newnode -> next = *a_head;
		*a_head = newnode;  // set newnode to head of linked list
	}
	else{
		while(starthead -> next != NULL && cmp_fn(starthead ->next -> a_value, a_value) <= 0){ 
			// keep the while loop running until its value is greater than preceding node or its the tail
			starthead = starthead -> next;
		}
		newnode -> next = starthead -> next;
		starthead -> next = newnode;
	}
	return newnode;
}

Node* pq_dequeue(Node** a_head) { // Removing element with highest priority from priority queue, and then return it.
	//Cannot allocate memeory here ?!
	Node* newnode = *a_head;
	if(*a_head != NULL){
		*a_head = (*a_head) -> next;
		newnode -> next = NULL;
	}
	return newnode;
}

Node* stack_push(Node** a_head, void* a_value) {  //try just calling dequeue like the screenshot from the prof?? 
	Node* newnode = malloc(sizeof(*newnode)); // allocate new node
	newnode -> a_value = a_value;// initialize newnode
	newnode -> next = *a_head;// initialize newnode
	*a_head = newnode; // set a_head to the adress of the new node
	return newnode;
}

Node* stack_pop(Node** a_head) { //calling dequeue function //detach and return the node from the head of the linked list
	// Node* newnode = pq_dequeue(a_head);
	//return newnode;
	return pq_dequeue(a_head); 
}

void destroy_list(Node** a_head, void (*destroy_value_fn)(void*)) {
	while((*a_head) != NULL) {
		//stores the address of the first Node in the list
		Node* popnode = stack_pop(a_head); 
		destroy_value_fn(popnode -> a_value);
		free(popnode); //deallocating memory
	}
}
//still need to create a function that returns nothing and call it in test to free list

#define HUFFMAN_C_V1
