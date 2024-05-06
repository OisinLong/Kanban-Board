#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void PrintMenuOptions() {
	// Displaying the options for the user
	puts("\n=========================");
	puts("(1) Display Board");
	puts("(2) Load Board From File");
	puts("(3) Edit List");
	puts("(4) Edit Board");
	puts("(5) Save Board To A File");
	puts("(6) Quit");
	puts("=========================\n");
}

void PrintEditListOptions() {
	puts("\n=========================");
	puts("(1) Edit an Item");
	puts("(2) Add a New Item");
	puts("(3) Delete an Item");
	puts("(4) Return to Main Menu");
	puts("=========================\n");
}

void PrintEditBoardOptions() {
	puts("\n=========================");
	puts("(1) Edit the Name of a List");
	puts("(2) Add a New List");
	puts("(3) Delete a List");
	puts("(4) Return to Main Menu");
	puts("=========================\n");
}

// Clears screen depending on the OS
void ClearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
