#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- HEADER --- */

struct list_item {
    int value;
    struct list_item *next;
};

/* puts x at the end of the list */
void append(struct list_item *first, int x);

/* puts x at the beginning of the list */
void prepend(struct list_item *first, int x);

/* prints all elements in the list */
void print(struct list_item *first);

/* input_sorted: find the first element in the list
 larger than x and input x right before that element */
void input_sorted(struct list_item *first, int x);

/* free everything dynamically allocated */
void clear(struct list_item *first);


/* --- Implementation --- */

void append(struct list_item *first, int x) {
    struct list_item *item = malloc(sizeof(struct list_item));
    item->value = x;
    item->next = NULL;

    struct list_item *curr = first;
    while(curr->next != NULL) { // loop until we're at the last item
        curr = curr->next;    
    }
    curr->next = item;
}

void prepend(struct list_item *first, int x) {
    struct list_item *item = malloc(sizeof(struct list_item));
    item->value = x;

    item->next = first->next;
    first->next = item;
}

void print(struct list_item *first) {
	struct list_item *curr = first;
    while(curr->next != NULL) {
		curr = curr->next;
		printf("%d ", curr->value);
		// printf(curr->value);
	}
    printf("\n");
}

/* --- Main --- */
int main(int argc, char **argv) {
    struct list_item root;
    root.value = -1; /* This value is always ignored */
    root.next = NULL;

    int opt;
    
    do {
		printf("Menu:\n");
		printf("1 - Append item\n");
		printf("2 - Prepend item\n");
		printf("3 - Print item\n");
		printf("4 - Input sorted\n");
        printf("5 - Clear\n");
		scanf("%d", &opt);
		switch (opt) {
            case 1: 
            {
				int *val = malloc(100);
				printf("value to insert: ");
				scanf("%d", val);
                append(&root, *val);
				break;
            }
            case 2: 
            {
                prepend(&root, opt);
				break;
            }
            case 3: 
            {
                print(&root);
				break;
            }
            case 4: 
            {
                //input_sorted(&root, opt);
				break;
            }
            case 5: 
            {
                //clear(&root);
				break;
            }
            default: {
                break;
            }
        }
	} while(1);
	return 0;
}