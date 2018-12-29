#include <stdio.h>
#include <stdlib.h>
#include "linkingList.h"

struct node* addToList(struct node* list, float pos, int t){  //Assuming times as integer
struct node* newNode;
newNode = malloc(sizeof(struct node*));
newNode->position = pos;
newNode->time = t;
newNode->next=list;
list = newNode; //Next node
return newNode;
}

struct node* searchTime(struct node* list, int t){
	struct node* p;
	for(p=list; p != NULL; p=p->next){
		if((p->time) == t)
			return p;
	}
	return NULL;
}

struct node* deleteNode(struct node* list, int t){ //times must be different
struct node *cur, *prev;
cur=list;
prev=NULL;
while((cur != NULL) && (cur->time != t)){ // To scroll the list until it finds the node we want to delete
	prev = cur;  
	cur = cur->next;
}
	if(cur == NULL) //cur points at null, t is not found
		return list;
	if(prev == NULL) // condition in while is true, so t is in the first node
		list = list->next;
	else
		prev->next = cur->next;
	free(cur);
	return list;
}

struct node* getHead(struct node* element){
	while((element->next != NULL)){ // To scroll the list until it finds the node we want to get
	element=element->next;
    }
	return element;
}

struct node* destroy(struct node* h){
    struct node *current = h;
    struct node *next;

    while (current != NULL)
    {
       next = current->next;
       free(current);
       current = next;
    }
    h = NULL;
	return h;
}

void printList(struct node *list) {

  struct node *p;
  for (p=list; p != NULL; p=p->next) 
      printf("%d ", p->time); 
  
  printf("\n"); 
  
}