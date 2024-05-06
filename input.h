// Constants
#define MAX_LEN 100

// Declaring the inner linked list for the items associated with each user
struct Item
{
	char name[MAX_LEN];
	struct Item* next_item_node;
};

// Declaring the struct above to be called a node for better comprehension when creating the list
typedef struct Item* item_node;

// Declaring the struct for the outer linked list, the owner names and their respective item lists
struct Person
{
	char name[MAX_LEN];
	item_node first_item;
	struct Person* next_person_node;
};
// Renaming the above struct for easier comprehension
typedef struct Person* person_node;

short int GetNumInRange(short int min, short int max);

void FlushInput();