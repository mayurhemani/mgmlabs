#include <stdio.h>
#include <stdlib.h>

struct node_t {
	int num;
	struct node_t* ptr;
};


struct node_t* new_node(int num) {
	struct node_t* n = (struct node_t*)malloc(sizeof(struct node_t));
	n->num = num;
	n->ptr = NULL;
	return n;
}

struct node_t* push_node(struct node_t* stack, int num) {
	struct node_t* n = new_node(num);
	if (stack == NULL) return n;
	stack->ptr = n;
	return n;
}


struct node_t* show_all(struct node_t* stack) {
	struct node_t* p = stack;
	for(; p != NULL; p = p->ptr) 
		printf("%d\n", p->num);
	return stack;
}

int main() {
	struct node_t* s = NULL;
	s = push_node(s, 12);
	s = push_node(s, 13);
	s = push_node(s, 14);
	show_all(s);
	printf("\n");
	return 0;
}

