#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

struct node* push(struct node*, int);

struct node* push(struct node* head, int val){ /* pushes val node into first spot of LL, basically same as insert */
	struct node* temp = allocate(val);
	
	if(head == 0){ /* if first thing to be pushed onto stack */
		return temp;
	}
	/* link temp to head node, temp is new head node */
	temp -> next = head;
	return temp;
}

struct node* pop(struct node*);

struct node* pop(struct node* head){ /* removes first item of linked list (head) */
	if(head == 0){ //delete nothing
		return head;	
	}
	/* use temp to free head, make temp = head.next */
	struct node* temp = head -> next;
	free(head);
	return temp;
}

struct node* enq(struct node*, int);

struct node* enq(struct node* head, int val){ /* iterate while(ptr != 0), at end, add to end */
	struct node* temp = allocate(val);
	if(head == 0){ /* add it to empty stack */
		return temp;
	}
	struct node* ptr = head;
	struct node* prev = 0;
	while(ptr != 0){ /* iteration */
		prev = ptr;
		ptr = ptr -> next;
	}
	/* end of LL, add to prev */
	prev -> next = temp;
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

	char content[100]; // pop, enqueu, push
	int num; // num
	struct node* head = 0;

	while(fscanf(f, "%s %d\n", content, &num) != EOF){
		if(strcmp(content, "POP") == 0){
			head = pop(head);
		}
		else if(strcmp(content, "ENQUEUE") == 0){
			head = enq(head, num);
		}
		else{
			head = push(head, num);
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
