#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node{
	char value[20];
	int index;
	int weight;
	struct node* next;
};

struct Graph{
	int num;
	struct node** adjList;
	int* visited;
};

struct node* makeAdjList(struct node*, struct node*); //BUILD ADJLIST/GRAPH METHOD //head node, node to be added

struct node* makeAdjList(struct node* head, struct node* temp){
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
						ptr = makeAdjList(ptr -> next, temp);
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

void dfs(struct Graph*, struct node*);

void dfs(struct Graph* g, struct node* src){ //reminder to print out the island cases!!!!
	int ptrIndex = src -> index;
	struct node* ptr = g->adjList[ptrIndex];
	struct node* temp = ptr;
	printf("%s ", temp->value);
	g->visited[ptrIndex] = 1;
	while(temp != 0){
		if(g->visited[temp->index] == 0){
			dfs(g, temp);
			temp -> index = 1;
		}
		temp = temp->next;
	}
	
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
	char vert[20]; // vertex
	char adj[20];
	int num; // number of vertices
	int weight;
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
	while(fscanf(f, "%s %s %d\n", vert, adj, &weight) != EOF){
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
				graph->adjList[i] = makeAdjList(graph->adjList[i], temp);
			}
			
		}
		
	}
	struct node* src = malloc(sizeof(struct node));
	strcpy(src -> value, graph->adjList[0] -> value);
	src -> next = 0;
	src -> index = 0;
	dfs(graph, src);
	for(int i = src->index; i < num; i++){
		if(graph->visited[i] == 0){
		//printf("END ");
			strcpy(src->value, graph->adjList[i]->value);
			src->next = 0;
			src->index = i;
			//graph->visited[i] = 1;
			dfs(graph, src);
		}
	}
	//print graph
	/*for(int i = 0; i < num; i++){
		struct node* ptr = graph->adjList[i];
		printf("\n");
		while(ptr != 0){
			printf("%s->", ptr -> value);
			ptr = ptr -> next;
		}
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
	free(src);
	free(graph -> adjList);
	free(graph -> visited);
	free(graph);
	fclose(f);
}
