#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node{
	char value[20];
	struct node* next;
};

struct node* push(struct node*, struct node*); //head node, node to be added

struct node* push(struct node* head, struct node* temp){
	struct node* ptr = head;
	struct node* prev = 0;
	//traverse thru ptr, insert in alphabetical order
	while(ptr != 0){
		if(strcmp(ptr -> value, temp -> value) > 0){ //ptr > temp
			//insert
			if(prev == 0){
				if(ptr -> next == 0){
					ptr -> next = temp;
				}
				else{
					if(strcmp(ptr -> next -> value, temp -> value) < 0){ //ptr.next < temp
						ptr = push(ptr -> next, temp);
						return head;
					}
					temp -> next = ptr -> next;
					ptr -> next = temp;
				}
				return head;
			}
			temp -> next = ptr;
			prev -> next = temp;
			return head;
		}
		prev = ptr;
		ptr = ptr -> next;
	}
	prev -> next = temp;
	return head;
}

int main(int argc, char* argv[argc+1]){
	if(argc != 3){
		printf("Please provide file name\n");
		return EXIT_SUCCESS;
	}
	FILE* f = fopen(argv[1], "r");
	FILE* f2 = fopen(argv[2], "r");
	if(f == 0){
		printf("error");
		return EXIT_SUCCESS;
	}
	char vert[20]; // vertex
	char adj[20];
	int num; // number of vertices
	fscanf(f, "%d\n", &num);
	struct node** graph = malloc(num * sizeof(struct node*));
	//initialize array
	for(int i = 0; i < num; i++){
		fscanf(f, "%s\n", vert);
		struct node* temp = malloc(sizeof(struct node));
		strcpy(temp -> value, vert);
		temp -> next = 0;
		//add to adjlist
		graph[i] = temp;
	}
	//build graph
	while(fscanf(f, "%s %s\n", vert, adj) != EOF){
		for(int i = 0; i < num; i++){
			if(strcmp(graph[i] -> value, vert) == 0){
				//add node to graph
				struct node* temp = malloc(sizeof(struct node));
				strcpy(temp -> value, adj);
				temp -> next = 0;
				graph[i] = push(graph[i], temp);
			}
			
		}
		//since undirected, add node to graph reverse
		for(int j = 0; j < num; j++){
			if(strcmp(graph[j] -> value, adj) == 0){
				//add node to graph
				struct node* temp = malloc(sizeof(struct node));
				strcpy(temp -> value, vert);
				temp -> next = 0;
				graph[j] = push(graph[j], temp);
			}
		}
	}
	char query[20];
	char v[20];
	while(fscanf(f2, "%s %s\n", query, v) != EOF){
		if(strcmp(query, "d") == 0){ // out degrees
			for(int i = 0; i < num; i++){
				if(strcmp(graph[i] -> value, v) == 0){
					int result = 0;
					struct node* ptr = graph[i] -> next;
					while(ptr != 0){
						result++;
						ptr = ptr -> next;
					}
					printf("%d", result);
				}
			}
		}
		else{ //list adj vertices
			for(int i = 0; i < num; i++){
				if(strcmp(graph[i] -> value, v) == 0){
					struct node* ptr = graph[i] -> next;
					while(ptr != 0){
						if(ptr -> next != 0){
							printf("%s ", ptr -> value);
							ptr = ptr -> next;
						}
						else{
							printf("%s", ptr -> value);
							ptr = ptr -> next;
						}
					}
				}
			}
		}
		printf("\n");
	}
	//print graph
	/*for(int i = 0; i < num; i++){
		struct node* ptr = graph[i];
		printf("\n");
		while(ptr != 0){
			printf("%s->", ptr -> value);
			ptr = ptr -> next;
		}
		printf("\n");
	}*/
	/* free */
	for(int i = 0; i < num; i++){
		struct node* ptr = graph[i];
		while(ptr != 0){
			struct node* temp = ptr;
			ptr = ptr -> next;
			free(temp);
		}
	}
	free(graph);
	fclose(f);
	fclose(f2);
}
