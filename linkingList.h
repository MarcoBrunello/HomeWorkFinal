#ifndef linkinList_h
#define linkinList_h

struct node{
	float position;
	int time;
	struct node* next;
};

struct node* addToList(struct node* list, float pos, int t);
struct node* searchTime(struct node* list, int t);
struct node* deleteNode(struct node* list, int t);
struct node* getHead(struct node* element);
struct node* destroy(struct node *head);
void printList(struct node *list); 

#endif