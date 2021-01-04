#include <stdlib.h>
#include <stdio.h>

struct node{
	int value;
	struct node* next;
};

struct node* allocate(int);

struct node* allocate(int val){
	struct node* temp = malloc(sizeof(struct node));
	temp -> value = val;
	temp -> next = 0;
	return temp;
}

struct node* insert(struct node*, int);

struct node* insert(struct node* head, int val){
	struct node* temp = allocate(val);
	
	if(head == 0){
		return temp;
	}
	//iterate, find if temp -> value 
	struct node* ptr = head;
	struct node* prev = 0;
	while(ptr != 0){
		if(ptr -> value > val){	//insert
			if(prev == 0){ //first item in LL
				temp -> next = ptr;
				return temp;
			}
			prev -> next = temp;
			temp -> next = ptr;
			return head;
		}
		else if(ptr -> value == val){ /* if duplicate add */
			free(temp);
			return head;
		}
		prev = ptr;
		ptr = ptr -> next;
	}
	/* if node tobe added is the last value */
	prev -> next = temp;
	return head;
}

struct node* delete(struct node*, int);

struct node* delete(struct node* head, int val){

	if(head == 0){ //delete nothing
		return head;	
	}
	//iterate, find if ptr - > value == val, then make prev -> next = ptr -> next
	struct node* ptr = head;
	struct node* prev = 0;
	while(ptr != 0){
		if(ptr -> value == val){ /* delete found */
			struct node* temp = ptr -> next;
			if(prev != 0){ /* if its a regular node to be deleted */
				prev -> next = ptr -> next;
				free(ptr);
				return head;
			}
			else{ /*if the first node is to be deleted */
				free(ptr);
				return temp;
			}
		}
		prev = ptr;
		ptr = ptr -> next;
	}
	/* if iterated all the way through, not found, return original LL */
	return head;
}

int main(int argc, char* argv[argc+1]){

	if(argc != 2){
		printf("Please provide file name\n");
		return EXIT_SUCCESS;
	}
	FILE* f = fopen(argv[1], "r");
	if(f == 0){
		printf("error");
		return EXIT_SUCCESS;
	}
	char content[100];//INSERT or DELETE
	int num; //number to be inserted or deleted

	struct node* head = 0;

	while(fscanf(f, "%s %d\n", content, &num) != EOF){
		if(content[0] == 'I'){
			head = insert(head, num);
		}
		if(content[0] == 'D'){
			head = delete(head, num);
		}
		/* print list */
		struct node* ptr = head;
		while(ptr != 0){
			if(ptr -> next != 0){ /* printing regular nodes */
				printf("%d ", ptr -> value);
				ptr = ptr -> next;
			}
			else{ /* printing last node (without space) */
				printf("%d", ptr -> value);
				ptr = ptr -> next;
			}
		}
		if(head == 0){ /*if empty LL */
			printf("EMPTY");
		}
		/* new line for formatting */
		printf("\n");
	}
	/* free */
	struct node* ptr = head;
	while(ptr != 0){
		struct node* temp = ptr;
		ptr = ptr -> next;
		free(temp);
	}
	return EXIT_SUCCESS;
}

