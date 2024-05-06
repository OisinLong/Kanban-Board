#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "input.h"
#include "test.h"

person_node create_example_list(void)
{
	/* FIRST SUBLIST */
	item_node item_1_1 = malloc(sizeof(struct Item));
	strcpy(item_1_1->name, "Happy Meal");

	item_node item_1_2 = malloc(sizeof(struct Item));
	strcpy(item_1_2->name, "Large Fanta");
	item_1_2->next_item_node = NULL;

	// Linking the list
	item_1_1->next_item_node = item_1_2;

	/* SECOND SUBLIST */
	item_node item_2_1 = malloc(sizeof(struct Item));
	strcpy(item_2_1->name, "5 Chicken Selects");

	item_node item_2_2 = malloc(sizeof(struct Item));
	strcpy(item_2_2->name, "Oreo McFlurry");
	item_2_2->next_item_node = NULL;

	// Linking the list
	item_2_1->next_item_node = item_2_2;

	/* THIRD SUBLIST */
	item_node item_3_1 = malloc(sizeof(struct Item));
	strcpy(item_3_1->name, "20 McNuggets");
	item_3_1->next_item_node = NULL;

	/* FOURTH SUBLIST */
	item_node item_4_1 = malloc(sizeof(struct Item));
	strcpy(item_4_1->name, "Medium Coke");
	item_4_1->next_item_node = NULL;

	// Now with all the sublists initialised, we can create the puter list for the double linked list to be fully initialised
	/* PERSON 1 */
	person_node person_1 = malloc(sizeof(struct Person));
	strcpy(person_1->name, "Alex");
	person_1->first_item = item_1_1;

	/* PERSON 2 */
	person_node person_2 = malloc(sizeof(struct Person));
	strcpy(person_2->name, "Jack");
	person_2->first_item = item_2_1;

	/* PERSON 3 */
	person_node person_3 = malloc(sizeof(struct Person));
	strcpy(person_3->name, "Barry");
	person_3->first_item = item_3_1;

	/* PERSON 4 */
	person_node person_4 = malloc(sizeof(struct Person));
	strcpy(person_4->name, "Ronald");
	person_4->first_item = item_4_1;

	// Linking the outer list
	person_1->next_person_node = person_2;
	person_2->next_person_node = person_3;
	person_3->next_person_node = person_4;
	person_4->next_person_node = NULL;

	return person_1;
}