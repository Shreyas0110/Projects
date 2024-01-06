/*
 * fifteens.c
 *
 *  Created on: Jan 9, 2023
 *      Author: shrey
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

static int size = 4;
static int maxsize = 16;
int* adjList;
int* issolved;
int* isequal;
int pathcounter = 0; //size of solved path

struct boardNode{
	//theoretically could lower memory by making char
	int *board;
	unsigned g;
	unsigned f;
	struct boardNode *parent;
	int *whereBlank;
};

struct linkedList{//USE LINK LISTS
	struct boardNode *data;
	struct linkedList *next;
};

void init_boardNode1(struct boardNode *);
void init_boardNode2(struct boardNode *, struct boardNode *);
void printNode(int *);
void swapper(int *, int, int);
int* init_AdjList();
int* shuffle(int*, int);
int heuristic1(int *);
int* solve_a_star(struct boardNode *);
void isSolved(int *);
void nodeEqual(int *, int *);

int main(void){

    issolved = (int*)malloc(sizeof(int));
    isequal = (int*)malloc(sizeof(int));
	srand(time(NULL));
	struct boardNode *b;
	struct boardNode *temp;
	b = (struct boardNode *) malloc(sizeof(struct boardNode));
	temp = (struct boardNode *) malloc(sizeof(struct boardNode));
	//init_boardNode1(b);
	init_boardNode1(b);
	//printNode(b->board);

	adjList = init_AdjList();

	//swapper(b->board,&i,&j);
	if(b->whereBlank != NULL)
		free(b->whereBlank);
	b->whereBlank = shuffle(b->board, 10);
	printNode(b->board);
	//printf("%d\n", *(b->whereBlank));
	b->f = heuristic1(b->board);

	int* path;
	init_boardNode2(temp, b);

    clock_t start, end;
    double cpu_time_used;
    start = clock();
	path = solve_a_star(temp);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	int i;
	printNode(b->board);

	//printf("index: ");
	int c;
	c = pathcounter-2;
	for (i = 0; i <pathcounter-1; i++){
		printf("moving: %d\n", b->board[path[c]]);
		//printf("%d:%d, \n",c, path[c]);
		swapper(b->board, path[c], *(b->whereBlank));
		*(b->whereBlank) = path[c];
		printNode(b->board);
		c -=1;
	}
	printf("pathsize = %d\n", pathcounter-2);
    printf("time taken = %f", cpu_time_used);

    //Remember to free b
    //Also consider making a delete() function for the linkedList

	return 0;

}

int* solve_a_star(struct boardNode *b){

	struct linkedList *openList_start;
	openList_start = (struct linkedList*) malloc(sizeof(struct linkedList));
	openList_start->data = NULL;
	struct linkedList *openList_curr;
	openList_curr = (struct linkedList*) malloc(sizeof(struct linkedList));
	openList_curr->data = b;
	openList_curr->next = NULL;
	openList_start->next = openList_curr;

	struct linkedList *closeList_start;
	closeList_start = (struct linkedList*) malloc(sizeof(struct linkedList));
	closeList_start->data = NULL;
	closeList_start->next = NULL;
	struct linkedList *closeList_last;
	closeList_last = closeList_start;

	struct linkedList *parent_of_selected;
	struct boardNode *selected;
	struct boardNode *children[4];

	int cc, visited, repl;
	int *index;
	index = (int *)malloc(sizeof(int));
	//Checks to see if length 1 (first node is dummy)
	while(openList_start->next != NULL){

		openList_curr = openList_start;
		parent_of_selected = openList_start;

		//Iterates through openList once, looks for lowest f value
		while(openList_curr->next != NULL){
			if(parent_of_selected->next->data->f > openList_curr->next->data->f){
				parent_of_selected = openList_curr;
			}
			openList_curr = openList_curr->next;

		}
		if(parent_of_selected->next == NULL)
			exit(0);
		closeList_last->next = parent_of_selected->next;
		closeList_last = closeList_last->next;
		selected = parent_of_selected->next->data;
		if(parent_of_selected->next->next != NULL)
			parent_of_selected->next = parent_of_selected->next->next;
		else
			parent_of_selected->next = NULL;
		isSolved(selected->board);
		if(*issolved == 1){
			int* path;
			int pathsize = 25;
			path = (int *)malloc(sizeof(int)*pathsize);
			while(selected != NULL){
				if(pathcounter >= pathsize){
					//printf("resized\n");
					int* newpath;
					newpath = (int*) malloc(pathsize+20);
					pathsize += 20;
					for(int w = 0; w<pathcounter; w++){
						newpath[w] = path[w];
					}
					free(path);
					path = newpath;
				}
				path[pathcounter] = *(selected->whereBlank);
				//printf("%d, ", path[pathcounter]);
				selected = selected->parent;
				pathcounter += 1;
			}
			/*
			printf("\n");
			for (int i = 0; i < pathcounter; i++){
				printf("%d:%d, ",i, path[i]);
			}

			printf("\n");
			int i,c;
			c = pathcounter-1;
			for (i = 0; i <pathcounter; i++){
				//truePath[i] = path[c];
				printf("%d:%d, ",c, path[c]);
				c -=1;
			}
			 */
			//FREE OPENLIST
			openList_curr = openList_start->next;
			while(openList_curr->next != NULL){
				parent_of_selected = openList_curr;
				free(parent_of_selected->data->board);
				free(parent_of_selected->data->whereBlank);
				free(parent_of_selected->data);
				free(parent_of_selected);
				openList_curr = openList_curr->next;
			}
			free(openList_start);
			//FREE CLOSELIST
			openList_curr = closeList_start->next;
			while(openList_curr->next != NULL){
				parent_of_selected = openList_curr;
				free(parent_of_selected->data->board);
				free(parent_of_selected->data->whereBlank);
				free(parent_of_selected->data);
				free(parent_of_selected);
				openList_curr = openList_curr->next;
			}
			free(closeList_start);

			return path;
		}

		//gen children
		for(cc = 0; cc<4; cc++){
			if((*index = adjList[*(selected->whereBlank)*4+cc]) != -1){
				children[cc] = (struct boardNode *)malloc(sizeof(struct boardNode));
				init_boardNode2(children[cc], selected);
				swapper(children[cc]->board, *index, *(selected->whereBlank));
				*(children[cc]->whereBlank) = *index;
				children[cc]->f = children[cc]->g+heuristic1(children[cc]->board);
			}
			else{
				children[cc] = NULL;
			}
		}
		for(cc = 0; cc<4; cc++){
			if(children[cc]!=NULL){
				visited = 0;
				//traverse CLOSEDLIST
				openList_curr = closeList_start;
				while(openList_curr->next != NULL){
					openList_curr = openList_curr->next;
					nodeEqual(openList_curr->data->board, children[cc]->board);
					if(*isequal == 1)
						visited = 1;
				}
				if(visited == 0){
					repl = 1;
					//check if child already exists in open list with smaller g value
					openList_curr = openList_start;
					while(openList_curr->next != NULL){
						openList_curr = openList_curr->next;
						nodeEqual(openList_curr->data->board, children[cc]->board);
						if(*isequal == 1 && children[cc]->g > openList_curr->data->g)
							repl = 0;
					}
					if(repl == 1){
						openList_curr->next = (struct linkedList *)malloc(sizeof(struct linkedList));
						openList_curr->next->data = children[cc];
						openList_curr->next->next = NULL;
					}
					if(repl == 0){

						//printf("freeing pointer at: %p",(void *) children[cc]);
						children[cc]->parent = NULL;
						free(children[cc]->board);
						free(children[cc]->whereBlank);
						free(children[cc]);
						children[cc] = NULL;
					}
				}
			}
		}
	}
	return NULL;
}

void nodeEqual(int* b1, int* b2){
	int i;
	for(i = 0; i<maxsize; i++){
		if(b1[i] != b2[i]){
			*isequal = 0;
			return;
		}
	}
	*isequal = 1;
}

void isSolved(int* board){
	int i;
	for(i = 0; i<maxsize-1; i++){
		if(board[i] != i+1){
			*issolved = 0;
			return;
		}
	}
	if(board[i] != 0){
		*issolved =  0;
		return;
	}
	*issolved = 1;
}

//init parent boardnode
void init_boardNode1(struct boardNode *node){
	int i;
	node->board = (int *) malloc(maxsize*sizeof(int));
	for(i = 0; i < maxsize-1; i++ ){
		node->board[i] = i+1;
	}
	node->board[maxsize-1] = 0;
	node->whereBlank = malloc(sizeof(int));
	*(node->whereBlank) = 15;
	node->parent = NULL;
	node->g = 0;
	node->f = 0;
}

void init_boardNode2(struct boardNode *node, struct boardNode *parent){
	int i;
	node->board = (int *) malloc(maxsize*sizeof(int));
	for(i = 0; i < maxsize; i++ ){
		node->board[i] = parent->board[i];
	}
	//FIGURE OUT WTF TO DO WITH WHEREBLANK!!!
	node->parent = parent;
	node->g = parent->g+1;
	node->f = 0;
	node->whereBlank = malloc(sizeof(int));
	*(node->whereBlank) = *(parent->whereBlank);
}

int heuristic1(int *board){ //Manhattan Distance
	int h, w, l, i;
	h = 0;
	for(i = 0; i<maxsize; i++){
		if(board[i]!= 0 && board[i] != i+1){
			w = abs(i%size - (board[i]-1)%size);
			l = abs(i/size - (board[i]-1)/size);
			h += w+l;
		}
		else if(board[i] == 0 && i != maxsize-1){
			w = 3-i%size;
			l = 3-i/size;
			h += w+1;
		}
	}
	return h;
}

void printNode(int *board){
	int i;
	for(i = 0; i < size; i++ ){
		int j;
		for(j = 0; j < size; j++)
			printf("%2d ", board[i*size+j]);
		printf("\n");
	}
	printf("\n");
}

int *init_AdjList(){
	int* adjList;
	adjList = (int*) malloc(sizeof(int)*maxsize*4);
	int i;
	int count;
	for(i = 0; i<maxsize; i++){
		count = 0;
		if((i+1)%size !=0){
			adjList[i*4+count]= i+1;
			count +=1;
		}
		if(i%size !=0){
			adjList[i*4+count] = i-1;
			count +=1;
		}
		if(i-size >=0){
			adjList[i*4+count] = i-size;
			count +=1;
		}
		if(i+size<maxsize){
			adjList[i*4+count] = i+size;
			count +=1;
		}
		for(;count<size;count++){
			adjList[i*4+count] = -1;
		}
	}
	/*
	int j;
	for(i = 0; i <maxsize; i++){
		for(j = 0; j < 4; j++){
			printf("%d ", adjList[i*4+j]);
		}
		printf("\n");
	}*/
	return adjList;
}

//worse than passing by value
void swapper(int *board, int i, int j){
	int temp = board[i];
	board[i] = board[j];
	board[j] = temp;
}

int* shuffle(int *board, int steps){
	int *index, *k;
	index = (int*)malloc(sizeof(int));
	k = (int*)malloc(sizeof(int));
	*index = maxsize-1;
	int i,j;
	for(i = 0; i < steps; i++){
		for(j = 0; j < 4; j++){
			if(adjList[*index*4+j] == -1){
				break;
			}
		}
		*k = adjList[*index*4+(rand()%j)];
		if(*k == -1)
			printf("invalid");
		swapper(board, *index, *k);
		*index = *k;
	}
	free(k);
	return index;
}
