#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"


//Kommentar
struct node* allocate(){
	struct node* temp_node = malloc(sizeof(struct node));
	if(temp_node == NULL){
		perror("Error: Malloc failed");
		exit(0);
		return NULL;
	}
	return temp_node;
}

list_t list_create()
{
		// sizeof(list_t) fordi 
 	list_t list = NULL;
 	list = malloc(sizeof(list_t));
 	if(list == NULL){
 		perror("Error: Failed to malloc list_t");
 		exit(0);
 		return NULL;
 	}
 	// Dette måten man setter verdier til pekere?
	list->head = allocate();
	list->tail = allocate();
	list->length = 0;
	return list;
}

void list_delete(list_t list)
{
	for(struct node* iter = list->tail->prev; iter != list->head; iter = iter->prev){
		free(iter->next);
	}
	free(list->head);
	free(list);
}

void list_insert(list_t list, int index, int data)
{
	int i = 0;
	struct node* new_node = allocate();

	for(struct node* iter = list->head; iter != NULL; iter = iter->next){
		printf("i: %i\n",i );
		if(i == index){
			if(index == 0){
				list->head->prev = new_node;
				new_node->next = list->head;
				list->head = new_node;
				new_node->prev = NULL;
			}else if(index == list->length){
				new_node->prev = list->tail;
				list->tail->next = new_node;
				list->tail = new_node;
				new_node->next = NULL;
			}else{
				iter->prev->next = new_node;
				new_node->next = iter;
				new_node->prev = iter->prev;
				iter->prev = new_node;
			}
			new_node->data = data;
			(list->length)++;
			return;
		}
		i = i + 1;
	}
}

void list_append(list_t list, int data){
	struct node* new_node = allocate();
	if(list->length == 0){
		// Listen er tom
		list->head = new_node;
		new_node->prev = NULL;
	}else{
		// Listen er IKKE tom
		new_node->prev = list->tail;
		list->tail->next = new_node;
	}
	new_node->data = data;
	new_node->next = NULL;		
	list->tail = new_node;
	(list->length)++;
}


// void list_print(list_t list){

// 	for(struct node* iter = list->head; iter != NULL; iter = iter->next){
// 		printf("Data: %s\n", iter->data);
// 	}
// }



// void list_append(list_t list, int data)
// {
// 	struct node* new_node = allocate();
// 	if(list->length == 0){
// 		list->head = new_node;

// 	}
// }

void list_print(list_t list)
{
	// Hvorfor må vi ha "*" her? Fordi det peker til en spesifikk node?
	for (struct node* iter = list->head; iter != NULL; iter = iter->next) {
        printf("%i ", iter->data);
    }
    printf("\n");
}

long list_sum(list_t list)
{
	long sum = 0;
	for (struct node* iter = list->head; iter != NULL; iter = iter->next){
		sum = sum + iter->data;
	}
	return sum;
}

int list_get(list_t list, int index)
{
	int i = 0;
	for(struct node* iter = list->head; iter != NULL; iter = iter->next){
		if(i == index){
			return iter->data;
		}
		i = i + 1;
	}
	return 0;
}

int list_extract(list_t list, int index)
{
	// Deletes the node at index = index and fixed links betweens nodes
	int return_value;
	int i = 0;
	for(struct node* iter = list->head; iter != NULL; iter = iter->next){
		if(i == index){
			if(i == 0){
				list->head = list->head->next;
				if(list->length != 1){
					list->head->prev = NULL;
				}
				return_value = iter->data;
				free(iter);
			}else if(i == list->length){
				list->tail = list->tail->prev;
				return_value = iter->data;
				free(iter);
			}else{
				iter->prev->next = iter->next;
				iter->next->prev = iter->prev;
				return_value = iter->data;
				free(iter);
			}
			(list->length)--;
			return return_value;
		}
		i = i + 1;
	}
	return 0;
}
