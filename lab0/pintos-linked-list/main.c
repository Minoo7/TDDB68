#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	char *name;
	struct list_elem elem;
};
typedef struct student student;
typedef struct list_elem list_elem;

void insert (struct list *student_list) {
	student *new_student = (student*) malloc(sizeof(struct student));
	char *name = (char*) malloc(sizeof(char)*20); //char *name = (char*) malloc(sizeof(char));

	// fetch name
	printf("What's your name: ");
	scanf("%s", name);
	new_student->name = name;
	
	// Puts the student last in the list (next to tail)
	list_push_back(student_list, &new_student->elem);	
}

void delete (struct list *student_list) {
	// Fetches a name
	char *name = (char*) malloc(sizeof(char)*20);
	printf("Enter a name to delete: \n");
	scanf("%s", name);

	// Loop until we find a matching name 
	for (list_elem *e = list_begin (student_list); e != list_end (student_list); e = list_next (e)) {
		student *stud = list_entry(e, struct student, elem);
		if (!strcmp(stud->name, name)) {
			list_remove(&stud->elem);
			free(stud->name);
			free(stud);
			free(name);
			return;
		}
	}
	printf("No student with that name exists\n");
}

void print (struct list *student_list) {
	list_elem *e;
	if (list_empty(student_list)) {
		printf("There are no students in this class!\n");
	}
	else {
		for (e = list_begin (student_list); e != list_end (student_list); e = list_next (e))
			{
				student *stud = list_entry(e, struct student, elem);
				printf("Name is: %s\n", stud->name);
			}
	}
}

void clear (struct list *student_list) {
	// Loop through list and pop element from the front
	while (!list_empty (student_list)) {
		list_elem *e = list_pop_front(student_list);
		student *stud = list_entry(e, struct student, elem);
		free(stud->name);
		free(stud);
	}

}

int main() {
	struct list student_list;
	list_init (&student_list);
	int opt;

	do {
		printf("Menu:\n");
		printf("1 - Insert student\n");
		printf("2 - Delete student\n");
		printf("3 - Print students\n");
		printf("4 - Clear\n");
		scanf("%d", &opt);
		switch (opt) {
			case 1:
				{
					insert(&student_list);
					break;
				}
			case 2:
				{
					delete(&student_list);
					break;
				}
			case 3:
				{
					print(&student_list);
					break;
				}

			case 4:
				{
					clear(&student_list);
					break;
				}
			default:
				{
					printf("Quit? (1/0):\n");
					scanf("%d", &opt);
					if (opt)
						clear(&student_list);
					break;
				}
		}
	} while(1);
		
	return 0;
}
