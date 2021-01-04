#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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
	char** stack;
	int* distance;
};

struct node* makeAdjList(struct node*, struct node*);

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

struct Graph* dfs(struct Graph*, struct node*);

struct Graph* dfs(struct Graph* g, struct node* src){
	int ptrIndex = src -> index;
	struct node* ptr = g->adjList[ptrIndex];
	struct Graph* result = g;
	if(result->visited[ptrIndex] == 0){ //add it to the stack	
		for(int i = 0; i < result->num; i++){
			if(strcmp(result->stack[i], "") == 0){
				result -> stack[i] = ptr -> value;
				break;
			}
		}
	}
	result->visited[ptrIndex] = 1;
	while(ptr != 0){
		if(result->visited[ptr->index] == 0){
			result = dfs(result, ptr);
			ptr -> index = 1;
		}
		ptr = ptr->next;
	}	
	return result;
}

void shortestPath(struct Graph*, struct node*);

void shortestPath(struct Graph* g, struct node* src){
	int ptrIndex = src->index;
	g->distance[ptrIndex] = 0;
	int end = g->num-1;
	for(int i = 0; i < g->num; i++){
		if(strcmp(g->stack[i], "") == 0){
			end = i-1;
			break;
		}
	}
	for(int i = 0; i <= end; i++){ //for each g->stack[i]
		struct node* u;
		char val[20];
		strcpy(val, g->stack[i]);
		//find the stack[i] node in adjlist
		for(int a = 0; a < g->num; a++){
			if(strcmp(val, g->adjList[a]->value) == 0){
				u = g->adjList[a];//u is vertex u in topsorted array
			}
		}
		//now traverse thru u
		struct node* v = u->next;
		while(v != 0){ //for each vert v in u.adjacent
			//find v index and u index in the distance array
			int vindex, uindex;
			for(int b = 0; b < g->num; b++){
				if(strcmp(g->adjList[b]->value, v->value) == 0){
					vindex = b;
				}
				if(strcmp(g->adjList[b]->value, u->value) == 0){
					uindex = b;
				}
			}
			if(g->distance[vindex] > g->distance[uindex] + v->weight){
				g->distance[vindex] = g->distance[uindex] + v->weight;//
				i=0;
			}
			v = v->next;
		}
	}
	for(int r = 0; r < g->num; r++){
		if(g->visited[r] == 0){
			printf("%s INF\n", g->adjList[r]->value);
		}
		else{
			printf("%s %d\n", g->adjList[r]->value, g->distance[r]);
		}
	}
}

int main(int argc, char* argv[argc+1]){
	if(argc != 3){
		printf("Please provide file name\n");
		return EXIT_SUCCESS;
	}
	FILE* f2 = fopen(argv[2], "r");
	if(f2 == 0){
		printf("error");
		return EXIT_SUCCESS;
	}
	char vert[20]; // vertex
	char adj[20];
	int num; // number of vertices
	int weight;
	char srcInput[20];
	int t = 0;

	while(fscanf(f2, "%s\n", srcInput) != EOF){
		FILE* f = fopen(argv[1], "r");
		fscanf(f, "%d\n", &num);
		struct Graph* graph = malloc(sizeof(struct Graph));
		graph -> num = num;
		graph -> adjList = malloc(num * sizeof(struct node*));
		graph -> visited = malloc(num * sizeof(int));
		graph -> stack = malloc(num*sizeof(char*));
		graph -> distance = malloc(num * sizeof(int));
		struct node* src = malloc(sizeof(struct node));
		strcpy(src -> value, srcInput);
		src -> next = 0;
		//initialize array
		for(int i = 0; i < num; i++){
			fscanf(f, "%s\n", vert);
			struct node* temp = malloc(sizeof(struct node));
			strcpy(temp -> value, vert);
			temp -> next = 0;
			temp -> index = i;
			//add to adjlist
			graph -> visited[i] = 0;
			graph -> stack[i] = "";
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
			graph->visited[i] = 0;
			graph->stack[i] = 0;
		}
		//build graph
		while(fscanf(f, "%s %s %d\n", vert, adj, &weight) != EOF){
			for(int i = 0; i < num; i++){
				if(strcmp(graph->adjList[i] -> value, vert) == 0){
					//add node to graph
					struct node* temp = malloc(sizeof(struct node));
					strcpy(temp -> value, adj);
					temp -> next = 0;
					temp -> weight = weight;
					for(int j = 0; j < num; j++){
						if(strcmp(graph->adjList[j]->value, temp->value) == 0){
							temp -> index = j;
						}
					}
					graph->adjList[i] = makeAdjList(graph->adjList[i], temp);
				}
				
			}
			
		}
		//reset graph
		for(int v = 0; v < num; v++){
			graph -> num = num;
			graph -> visited[v] = 0;
			graph -> distance[v] = INT_MAX;
			graph -> stack[v] = "";
		}
		for(int q = 0; q < num; q++){
			if(strcmp(graph->adjList[q]->value, srcInput) == 0){
				src -> index = q;
			}
		}
		graph = dfs(graph, src);
		int count = 0;
		for(int i = 0; i < num; i++){
			if(graph->visited[i] == 1){
				count++;
			}
		}
		if(count == num){
			printf("CYCLE");
		}
		else{
			if(t == 0){
				printf("\n");
				t = 1;
			}
			shortestPath(graph, src);
		}
		printf("\n");
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
		free(src);
		for(int i = 0; i < num; i++){
			struct node* ptr = graph->adjList[i];
			while(ptr != 0){
				struct node* temp = ptr;
				ptr = ptr -> next;
				free(temp);
			}
		}
		free(graph -> stack);
		free(graph -> distance);
		free(graph -> adjList);
		free(graph -> visited);
		free(graph);
		fclose(f);
	}
	fclose(f2);
}
