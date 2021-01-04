#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node{
	char value[20];
	int index;
	struct node* next;
};

struct Graph{
	int num;
	struct node** adjList;
	int* visited;
};

struct node* deq(struct node*);

struct node* deq(struct node* head){
	if(head == 0){ //delete nothing
		return head;	
	}
	struct node* ptr = head;
	struct node* temp = ptr -> next;
	free(ptr);
	return temp;
}

struct node* enq(struct node*, char[], int); //is it char*? char[]?

struct node* enq(struct node* head, char valu[], int index){ // iterate while(ptr != 0), at end, add to end 
	struct node* temp = malloc(sizeof(struct node));
	strcpy(temp -> value, valu);
	temp -> index = index;
	temp -> next = 0;
	if(head == 0){ // add it to empty stack 
		return temp;
	}
	struct node* ptr = head;
	struct node* prev = 0;
	while(ptr != 0){ // iteration 
		prev = ptr;
		ptr = ptr -> next;
	}
	// end of LL, add to prev 
	prev -> next = temp;
	return head;
}

struct node* push(struct node*, struct node*); //BUILD ADJLIST/GRAPH METHOD //head node, node to be added

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

void bfs(struct Graph*, struct node*);

void bfs(struct Graph* g, struct node* src){
	struct node* queue = 0;
	g -> visited[src -> index] = 1;
	queue = enq(queue, src -> value, src -> index);
	
	while(queue != 0){
		int ptrIndex = queue -> index;
		char ptrVal[20];
		strcpy(ptrVal, queue -> value);

		queue = deq(queue);
		printf("%s ", ptrVal);
		struct node* temp = g -> adjList[ptrIndex];
		//now enqueue all of next nodes
		while(temp != 0){		
			int vert = temp -> index;
			//printf("vert: %s index: %d visited: %d", temp-> value, temp -> index, graph -> visited[temp -> index]);
			if(g -> visited[vert] == 0){
				g -> visited[vert] = 1;
				//printf("enqueue: %s", temp -> value);
				queue = enq(queue, temp -> value, temp -> index);
			}
			temp = temp -> next;
		}
	}
	free(queue);
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
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph -> num = num;
	graph -> adjList = malloc(num * sizeof(struct node*));
	graph -> visited = malloc(num * sizeof(int));
	//initialize array
	for(int i = 0; i < num; i++){
		fscanf(f, "%s\n", vert);
		struct node* temp = malloc(sizeof(struct node));
		strcpy(temp -> value, vert);
		temp -> next = 0;
		temp -> index = i;
		//add to adjlist
		graph -> visited[i] = 0;
		//SORTING THE INITIAL VERTICES IN LEXICO
		for(int j = 0; j < i; j++){
			if(strcmp(graph->adjList[j]->value, temp->value) > 0){
				char hold[20];
				strcpy(hold, graph->adjList[j]->value);
				strcpy(graph->adjList[j] -> value, temp -> value);
				strcpy(temp->value, hold);
				graph->adjList[i] = temp;	
			}
		}
		graph -> adjList[i] = temp;
	}
	for(int i = 0; i < num; i++){
		graph->adjList[i]->index = i;
	}
	//build graph
	while(fscanf(f, "%s %s\n", vert, adj) != EOF){
		for(int i = 0; i < num; i++){
			if(strcmp(graph->adjList[i] -> value, vert) == 0){
				//add node to graph
				struct node* temp = malloc(sizeof(struct node));
				strcpy(temp -> value, adj);
				temp -> next = 0;
				for(int j = 0; j < num; j++){
					if(strcmp(graph->adjList[j]->value, temp->value) == 0){
						temp -> index = j;
					}
				}
				graph->adjList[i] = push(graph->adjList[i], temp);
			}
			
		}
		//since undirected, add node to graph reverse
		for(int j = 0; j < num; j++){
			if(strcmp(graph->adjList[j] -> value, adj) == 0){
				//add node to graph
				struct node* temp = malloc(sizeof(struct node));
				strcpy(temp -> value, vert);
				temp -> next = 0;
				for(int i = 0; i < num; i++){
					if(strcmp(graph->adjList[i]->value, temp->value) == 0){
						temp -> index = i;
					}
				}
				graph->adjList[j] = push(graph->adjList[j], temp);
			}
		}
	}
	char v[20];
	while(fscanf(f2, "%s\n", v) != EOF){
		//we have to initialize graph here since we did not make a method for that
		//a.k.a. we have to make a deep copy of graph (reset graph's visited array)
		struct Graph* alteredGraph = graph;
		for(int v = 0; v < num; v++){
			alteredGraph -> visited[v] = 0;
		}
		for(int i = 0; i < num; i++){	
			if(strcmp(alteredGraph -> adjList[i] -> value, v) == 0){
				struct node* src = alteredGraph -> adjList[i];
				bfs(alteredGraph, src);
			}
		}
		printf("\n");
	}
	//print graph
	/*for(int i = 0; i < num; i++){
		struct node* ptr = graph->adjList[i];
		printf("\n");
		while(ptr != 0){
			printf("%s->", ptr -> value);
			ptr = ptr -> next;
		}
		printf("\n");
	}*/
	//free
	for(int i = 0; i < num; i++){
		struct node* ptr = graph->adjList[i];
		while(ptr != 0){
			struct node* temp = ptr;
			ptr = ptr -> next;
			free(temp);
		}
	}
	free(graph -> adjList);
	free(graph -> visited);
	free(graph);
	fclose(f);
	fclose(f2);
}
