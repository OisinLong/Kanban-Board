// gcc -Wall main.c test.c input.c output.c -o main.exe

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "input.h"
#include "output.h"
#include "test.h"

// Displays the choices to the user, acts as control point for program
void menu(void);

// Prototypes; Will Move Declarations Elsewhere
void DisplayBoard(person_node header);
void LoadBoardFromFile(person_node header);
void EditList(person_node header);
void EditBoard(person_node header);
void SaveBoard(person_node header);
void Quit(person_node header) { puts("6"); }

// Array of function poinnters for the menu system
void (*MenuOption[6]) (person_node header);

int main(void)
{
	// Assigning each pointer it's respective function according to the menu order
	MenuOption[0] = DisplayBoard;
	MenuOption[1] = LoadBoardFromFile;
	MenuOption[2] = EditList;
	MenuOption[3] = EditBoard;
	MenuOption[4] = SaveBoard;
	MenuOption[5] = Quit;

	// Calling the menu
	menu();

	return 0;
}

void menu(void)
{
	short int user_choice = 0;
	
	// Creating an example list to start with
	person_node header = create_example_list();
	// Asking the user for their choice
	while (user_choice != 6) {
		// Show the user the menu
		PrintMenuOptions();

		printf("Enter your choice (1-6): ");
		// Ensures valid user input
		user_choice = GetNumInRange(1, 6);

		// Clears the screen for cleaner UI
		ClearScreen();
		// Calling the function that corresponds to the user's choice
		MenuOption[user_choice - 1](header);
	}
}

void DisplayBoard(person_node header)
{
	// Declaring a pointer to iterate through the linked lists
	person_node current_person = header;
	int personCount = 0;
	while(current_person != NULL)
	{
	    // Prints the current person's name
	    printf("%d) %s\n", personCount++, current_person->name);

	    // Iterates thru the sublist of items for each person
	    item_node current_item = current_person->first_item;
		int itemCount = 0;
	    while (current_item != NULL)
	    {
	        // Prints the current item
	        printf("\t%c. %s\n", 'a' + itemCount++, current_item->name);
	        // Moves to the next item
	        current_item = current_item->next_item_node;
	    }
	    // Moves to the next person
	    current_person = current_person->next_person_node;
	}
}

void GetLine(char* str, FILE *fPtr) {
	int i = 0;
	// Stores the current chr read from the file
	char character = fgetc(fPtr);
	// Reads chrs from the file until a newline chr of the end of the file is encountered
	while(!feof(fPtr) && character != '\n' && i < MAX_LEN) {
		str[i++] = character;
		character = fgetc(fPtr);
	}
	// Adding the null terminator to the end of the string to ensure it is in a valid format
	str[i] = '\0';
}

void LoadBoardFromFile(person_node header) {
	// Pointer to the file
	FILE* fPtr;

	// Getting input from the user
	char fileName[MAX_LEN + 6] = "Saves/";
	printf("\nEnter the Name of a File Load: ");
	scanf("%s", fileName + 6);

	// Ensuring there was no errors opening the file
	while ((fPtr = fopen(fileName, "rb")) == NULL) {
		puts(RED "\n#ERROR: FILE COULD NOT BE OPENED.\n"
			"  MAKE SURE IT EXISTS AND IS IN THE FORMAT: 'Example.dat'#\n" RESET);
		printf("\nEnter the Name of a File Load: ");
		scanf("%s", fileName);
	};

	printf("\n");

	// Declaring variable for reading the file
	person_node current_person;
	item_node current_item;
	person_node last_person = NULL;
	char delimiter = fgetc(fPtr);

	// Will loop thru the file until the end of the file is reached
	while (delimiter != EOF) {
		// Ensuring the file is opended sucessfully
		if (delimiter != '-' && delimiter != '\t') {
			puts(RED "#ERROR: FILE CANNOT BE READ (CORRUPTED OR UNFORMATTED)#" RESET);
			return;
		}

		// Allocating memory for the new person
		current_person = malloc(sizeof(struct Person));
		// Reading the name of the new person from the file
		GetLine(current_person->name, fPtr);
		// Linking the new person to the list
		if (last_person != NULL)
			last_person->next_person_node = current_person;
		else {
			*header = *current_person;
			current_person = header;
		}
		last_person = current_person;

		// Reading the delimiter for the next item, same logic as above block
		delimiter = fgetc(fPtr);
		current_item = malloc(sizeof(struct Item));
		GetLine(current_item->name, fPtr);
		current_person->first_item = current_item;

		// Reading the delimiter for the next item
		delimiter = fgetc(fPtr);
		// Will iterate unti the delimiter indicates that the current persons items have al been iterated over
		while (delimiter == '\t') {
			item_node last_item = current_item;
			current_item = calloc(1, sizeof(struct Item));
			GetLine(current_item->name, fPtr);
			last_item->next_item_node = current_item;

			delimiter = fgetc(fPtr);
		}
		// Ensuring that the linked list for items is correctly terminated
		current_item->next_item_node = NULL;
	}
	// Closing the file
	fclose(fPtr);

	// Likewise for the persons linked list
	current_person->next_person_node = NULL;

	// Displaying the board to the user
	DisplayBoard(header);
}

person_node FindList(person_node header) {
	char listToEdit[MAX_LEN];

	// Reads the name of the list from the user
	fgets(listToEdit, MAX_LEN, stdin);

	// Formatting the input, removing the \n chr and replacing it with the null terminator
	int length = strlen(listToEdit);
	listToEdit[length - 1] = '\0';

	// Initialising variables for searching the list
	int found = 0;
	person_node currentPerson = header;
	// Iterates thru the list until the desired list is found or the end of the list is reached
	while (currentPerson != NULL && !found) {
		// Comparing the name of the current list with the name given by the user
		if(!strcmp(listToEdit, currentPerson->name)) {
			// If they are the same, we alert that we have found the desired list to break out of the loop
			found = 1;
		} else
		// Traverse the list by moving the pointer4 along
			currentPerson = currentPerson->next_person_node;
	}

	return(currentPerson);
}

// EXACT SAME LOGIC AS FINDLIST, SEE ABOVE FOR COMMENTS ON LOGIC
item_node FindItem(item_node header) {
	char itemToEdit[MAX_LEN];

	fgets(itemToEdit, MAX_LEN, stdin);

	int length = strlen(itemToEdit);
	itemToEdit[length - 1] = '\0';

	int found = 0;
	item_node currentItem = header;
	while (currentItem != NULL && !found) {
		if (!strcmp(itemToEdit, currentItem->name)) {
			found = 1;
		}
		else
			currentItem = currentItem->next_item_node;
	}

	return(currentItem);
}

int DeleteItem(item_node *header) {
	char itemToDelete[MAX_LEN];
	// Reads the name of the item the users wishes to delete
	fgets(itemToDelete, MAX_LEN, stdin);

	// Removes the \n and replaces it with \0
	int length = strlen(itemToDelete);
	itemToDelete[length - 1] = '\0';

	// Initialising variables for searching
	int found = 0;
	// Two pointers currentItem and lastItem
	item_node currentItem = *header, lastItem;
	// Iterates over th elist until the item is found or end of list is reached
	while (currentItem != NULL && !found) {
		// Comparing the item given with the cureent item
		if (!strcmp(itemToDelete, currentItem->name)) {
			if (currentItem == *header) {
				// If the current item is the first item in the list, update the header pointer to point to the next item
				*header = currentItem->next_item_node;
			}
			else {
				// The current item isn't the first item in the list, thus link the previous
				// item to the next item, skipping the current item
				lastItem->next_item_node = currentItem->next_item_node;
			}
			// Free the memory for the current item as it it no longer needed
			free(currentItem);
			// Flag that the item was found and deleted
			found = 1;
		}
		else {
			// If the current item is't to be deleted, increment the pointers to traverse the linked list
			lastItem = currentItem;
			currentItem = currentItem->next_item_node;
		}
	}

	//Return Found to Cancel Deleting if Nothing is Entered
	if (length == 1)
		found = 1;

	// Returns 1(true) if item was found and deleted else, 0(false)
	return(found);
}

int DeleteList(person_node header) {
	char listToDelete[MAX_LEN];
	// Get the name of the list to be deleted from the user
	fgets(listToDelete, MAX_LEN, stdin);

	// Replacing the \n with \0
	int length = strlen(listToDelete);
	listToDelete[length - 1] = '\0';

	// Initialising variables for searching
	int found = 0;
	person_node currentList = header, lastList;
	// Iterates over the linked list until list is found or end of linked list is encountered
	while (currentList != NULL && !found) {
		// If the list given matches the current list name
		if (!strcmp(listToDelete, currentList->name)) {
			// If the current list is the first list in the linked list
			if (currentList == header) {
				// Update the header pointer to point to the next list
				*header = *currentList->next_person_node;
			}
			else {
				// It's not the first item in ther linked list, hence we link the previous list to the next list
				lastList->next_person_node = currentList->next_person_node;
				// Freeing the current list as it's no longer needed
				free(currentList);
			}
			// Flagging that we found and deleted the list
			found = 1;
		}
		else {
			// Ther current list wasn't to be deleted, hence we update the pointers and continue our search
			lastList = currentList;
			currentList = currentList->next_person_node;
		}
	}

	//Return Found to Cancel Deleting if Nothing is Entered
	if (length == 1)
		found = 1;

	// Returns 1(true) if item was found and deleted else, 0(false)
	return(found);
}

void EditList(person_node header) {
	// Pointer to the list(person) to be edited
	person_node listToEdit = NULL;
	// SHowing the user the current state of the board
	DisplayBoard(header);

	// Asking the user for a list name to edit
	printf("\nEnter The Name of the List to Edit: ");
	// Searching for said list within the linked list, prompting user to try again if not found
	listToEdit = FindList(header);
	while (listToEdit == NULL) {
		printf(RED "\n#ERROR: LIST NOT FOUND PLEASE TRY AGAIN#\n" RESET);
		listToEdit = FindList(header);
	}
	// Storing users choice
	short int user_choice = 0;
	// Will continue editing the list until the user chooses 4(exit)
	while (user_choice != 4) {
		// Clears the screen for cleaner UI
		ClearScreen();

		DisplayBoard(header);

		PrintEditListOptions();
		// Gets the user's choice for how to edit the list
		printf("\nEnter your choice (1-4): ");
		user_choice = GetNumInRange(1, 4);

		// Prints the current person's name
		printf("\n    %s\n", listToEdit->name);

		// Iterates thru the sublist of items for each person
		item_node current_item = listToEdit->first_item;
		int itemCount = 0;
		while (current_item != NULL)
		{
			// Prints the current item
			printf("\t%c. %s\n", 'a' + itemCount++, current_item->name);
			// Moves to the next item
			current_item = current_item->next_item_node;
		}

		// Conditional block that edits the list according to what the user chose to do
		item_node itemToEdit;    
		if (user_choice == 1) {
			// User wants to edit an item
			printf("\nEnter The Name of the Item to Edit: ");
			// Scans the linked list for the given item
			itemToEdit = FindItem(listToEdit->first_item);
			// Error checking the input
			while (itemToEdit == NULL) {
				printf(RED "\n#ERROR: ITEM NOT FOUND PLEASE TRY AGAIN#\n" RESET);
				itemToEdit = FindItem(listToEdit->first_item);
			}
			char newName[MAX_LEN];
			// The new name of the item
			printf("Enter the New Name of the Item: ");
			fgets(newName, MAX_LEN, stdin);
			printf("\n");

			newName[strlen(newName) - 1] = '\0';
			// Replacing the item with it's new name
			strcpy(itemToEdit->name, newName);
		}
		else if (user_choice == 2) {
			// Allocating memory for the new item
			item_node newItem = malloc(sizeof(struct Item));

			// Ensuruing that the linked list is properly terminated
			newItem->next_item_node = NULL;

			if (listToEdit->first_item != NULL) {
				// User wants to add a new item
				itemToEdit = listToEdit->first_item;
				// Iterates across the list until the last item is found
				while (itemToEdit->next_item_node != NULL) {
					itemToEdit = itemToEdit->next_item_node;
				}
				
				// Link the item to the end of the list
				itemToEdit->next_item_node = newItem;
			}
			else {
				listToEdit->first_item = newItem;
			}

			char name[MAX_LEN];
			// Getting the name of the new item
			printf("\nEnter the Name of the New Item: ");
			fgets(name, MAX_LEN, stdin);

			name[strlen(name) - 1] = '\0';
			// Writing the new item name to the new node in the linked list
			strcpy(newItem->name, name);
		}
		else if (user_choice == 3) {
			// User wishes to delete an item
			printf("\nEnter the Name of the Item to Delete: ");
			int found = DeleteItem(&listToEdit->first_item);
			while (!found) {
				printf(RED "\n#ERROR: ITEM NOT FOUND PLEASE TRY AGAIN\n"
					"        (OR ENTER NOTHING TO EXIT)#\n\n" RESET);
				found = DeleteItem(&listToEdit->first_item);
			}
		}
	}

	// Clears the screen for cleaner UI
	ClearScreen();
	// Shows the state of the board after an edit
	DisplayBoard(header);
}

// EXACT SAME LOGIC AS EDIT LIST FUNCTION, SEE ABOVE FOR COMMENTS ON LOGIC
void EditBoard(person_node header) {
	short int user_choice = 0;
	while (user_choice != 4) {
		// Clears the screen for cleaner UI
		ClearScreen();

		DisplayBoard(header);

		PrintEditBoardOptions();

		printf("Enter your choice (1-4): ");
		user_choice = GetNumInRange(1, 4);
		printf("\n");

		person_node listToEdit;
		if (user_choice == 1) {
			printf("\nEnter The Name of the List to Edit: ");
			listToEdit = FindList(header);
			while (listToEdit == NULL) {
				printf(RED "\n#ERROR: LIST NOT FOUND PLEASE TRY AGAIN#\n" RESET);
				listToEdit = FindList(header);
			}
			char newName[MAX_LEN];

			printf("\nEnter the New Name of the List: ");
			fgets(newName, MAX_LEN, stdin);

			newName[strlen(newName) - 1] = '\0';

			strcpy(listToEdit->name, newName);
		}
		else if (user_choice == 2) {
			listToEdit = header;
			while (listToEdit->next_person_node != NULL) {
				listToEdit = listToEdit->next_person_node;
			}

			person_node newList = malloc(sizeof(struct Person));
			listToEdit->next_person_node = newList;
			newList->next_person_node = NULL;
			newList->first_item = NULL;

			char name[MAX_LEN];

			printf("\nEnter the Name of the New Item: ");
			fgets(name, MAX_LEN, stdin);

			name[strlen(name) - 1] = '\0';

			strcpy(newList->name, name);
		}
		else if (user_choice == 3) {
			printf("\nEnter the Name of the List to Delete: ");
			int found = DeleteList(header);
			while (!found) {
				printf(RED "\n#ERROR: LIST NOT FOUND PLEASE TRY AGAIN\n"
							 "        (OR ENTER NOTHING TO EXIT)#\n\n" RESET);
				found = DeleteList(header);
			}
		}
	}

	// Clears the screen for cleaner UI
	ClearScreen();

	DisplayBoard(header);
}

void SaveBoard(person_node header)
{
	// Pointer to the text file
	FILE *fp;

	// Allowing the user to name the file
	char fileName[MAX_LEN + 6] = "Saves/";
	printf("\nEnter the Name of the File: ");
	scanf("%s", fileName + 6);

	// Ensuring there was no errors opening the file
	while ((fp = fopen(fileName, "wb")) == NULL) {
		puts(RED "\n#ERROR: FILE COULD NOT BE SAVED TO#\n" RESET);
		printf("\nEnter the Name of the File: ");
		scanf("%s", fileName);
	};

	printf("\n");

	// Otherwise we can write to the file as it was opened sucessfully
	person_node current_person = header;
	while (current_person != NULL)
	{
		// Writes the current person's name to the file
		fwrite("-", sizeof(char), strlen("-"), fp);
		fwrite(current_person->name, sizeof(char), strlen(current_person->name), fp);
		fwrite("\n", sizeof(char), strlen("\n"), fp);

		// Iterates thru the sublist of items for each person
		item_node current_item = current_person->first_item;
		while (current_item != NULL)
		{
			// Writes the current item to the file
			fwrite("\t", sizeof(char), strlen("\t"), fp);
			fwrite(current_item->name, sizeof(char), strlen(current_item->name), fp);
			fwrite("\n", sizeof(char), strlen("\n"), fp);
			// Moves to the next item
			current_item = current_item->next_item_node;
		}
		// Moves to the next person
		current_person = current_person->next_person_node;
	}

	// Closing the file
	fclose(fp);
}

/*
		  _____                    _____                    _____                   _______                   _____
		 /\    \                  /\    \                  /\    \                 /::\    \                 /\    \
		/::\    \                /::\    \                /::\    \               /::::\    \               /::\    \
	   /::::\    \              /::::\    \              /::::\    \             /::::::\    \             /::::\    \
	  /::::::\    \            /::::::\    \            /::::::\    \           /::::::::\    \           /::::::\    \
	 /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \         /:::/~~\:::\    \         /:::/\:::\    \
	/:::/__\:::\    \        /:::/__\:::\    \        /:::/__\:::\    \       /:::/    \:::\    \       /:::/__\:::\    \
   /::::\   \:::\    \      /::::\   \:::\    \      /::::\   \:::\    \     /:::/    / \:::\    \     /::::\   \:::\    \
  /::::::\   \:::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \   /:::/____/   \:::\____\   /::::::\   \:::\    \
 /:::/\:::\   \:::\    \  /:::/\:::\   \:::\____\  /:::/\:::\   \:::\____\ |:::|    |     |:::|    | /:::/\:::\   \:::\____\
/:::/__\:::\   \:::\____\/:::/  \:::\   \:::|    |/:::/  \:::\   \:::|    ||:::|____|     |:::|    |/:::/  \:::\   \:::|    |
\:::\   \:::\   \::/    /\::/   |::::\  /:::|____|\::/   |::::\  /:::|____| \:::\    \   /:::/    / \::/   |::::\  /:::|____|
 \:::\   \:::\   \/____/  \/____|:::::\/:::/    /  \/____|:::::\/:::/    /   \:::\    \ /:::/    /   \/____|:::::\/:::/    /
  \:::\   \:::\    \            |:::::::::/    /         |:::::::::/    /     \:::\    /:::/    /          |:::::::::/    /
   \:::\   \:::\____\           |::|\::::/    /          |::|\::::/    /       \:::\__/:::/    /           |::|\::::/    /
	\:::\   \::/    /           |::| \::/____/           |::| \::/____/         \::::::::/    /            |::| \::/____/
	 \:::\   \/____/            |::|  ~|                 |::|  ~|                \::::::/    /             |::|  ~|
	  \:::\    \                |::|   |                 |::|   |                 \::::/    /              |::|   |
	   \:::\____\               \::|   |                 \::|   |                  \::/____/               \::|   |
		\::/    /                \:|   |                  \:|   |                   ~~                      \:|   |
		 \/____/                  \|___|                   \|___|                                            \|___|
										 ___   ___   ________   ___   ___                 
										|\  \ |\  \ |\   __  \ |\  \ |\  \                
										\ \  \\_\  \\ \  \|\  \\ \  \\_\  \               
										 \ \______  \\ \  \\\  \\ \______  \              
										  \|_____|\  \\ \  \\\  \\|_____|\  \             
												 \ \__\\ \_______\      \ \__\            
												  \|__| \|_______|       \|__|       


 */
