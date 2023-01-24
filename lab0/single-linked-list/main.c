#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- HEADER --- */

typedef struct list_item {
    int value;
    struct list_item *next;
} list_item;

/* puts x at the end of the list */
void append(list_item *first, int x);

/* puts x at the beginning of the list */
void prepend(list_item *first, int x);

/* prints all elements in the list */
void print(list_item *first);

/* input_sorted: find the first element in the list
 larger than x and input x right before that element */
void input_sorted(list_item *first, int x);

/* free everything dynamically allocated */
void clear(list_item *first);


/* --- Implementation --- */

void append(list_item *first, int x) {
    list_item *item = (list_item*) malloc(sizeof(list_item));
    item->value = x;
    item->next = NULL;

    list_item *curr = first;
    while(curr->next != NULL) { // loop until we're at the last item
        curr = curr->next;    
    }
    curr->next = item;
}

void prepend(list_item *first, int x) {
    list_item *item = (list_item*) malloc(sizeof(list_item));
    item->value = x;

    item->next = first->next;
    first->next = item;
}

void print(list_item *first) {
	list_item *curr = first;
    while(curr->next != NULL) {
		curr = curr->next;
		printf("%d ", curr->value);
	}
    printf("\n");
}

void input_sorted(list_item *first, int x) {
    list_item *curr = first;
    while(curr->next != NULL) {
        if (curr->next->value > x) {
            break;
        }
        curr = curr->next;  
    }
    prepend(curr, x);
}

void clear(list_item *first) {
    // Clears all list items except for HEAD
    list_item *curr = first->next;
    list_item *next = NULL;
	while (curr != NULL) {
        next = curr->next;
		free(curr);
        curr = next;
    }
    first->next = NULL;
}

/* --- Main --- */
int main(int argc, char **argv) {
    list_item root; // why works * with and without pointer for root???
    root.value = -1; /* This value is always ignored */
    root.next = NULL;

    append(&root, 2);
    append(&root, 5);
    print(&root);

    input_sorted(&root, 3);
    print(&root);
    prepend(&root, 9);
    print(&root);
    
    clear(&root);
    print(&root);
    append(&root, 1);
    print(&root);
    clear(&root);

    /*int opt;
    
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
                free(val);
				break;
            }
            case 2: 
            {
                int *val = malloc(100);
				printf("value to prepend: ");
				scanf("%d", val);
                input_sorted(&root, *val);
                prepend(&root, opt);
                free(val);
				break;
            }
            case 3: 
            {
                print(&root);
				break;
            }
            case 4: 
            {
                int *val = malloc(100);
				printf("value to input insert: ");
				scanf("%d", val);
                input_sorted(&root, *val);
                free(val);
				break;
            }
            case 5: 
            {
                clear(&root);
				break;
            }
            default: {
                break;
            }
        }
	} while(1);
	return 0;*/
}