/*
   Program 4: Database of Students
   SYSTEM: Windows 11 - Visual Studio Community 2022 (ARM) / replit
   spring 2023
   Alex Carrillo - aec10@uic.edu
   Prof. Kidane 1pm
*/

// using this to compile deprecated C functions which are "unsafe" 
#pragma warning(disable : 4996)

// program includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// structs for the program
typedef struct
{
	char* name;
	char* id;
	double gpa;
	int creditHours;
} Student;

typedef struct StudentNode
{
	Student* pStudent;
	struct StudentNode* next;
} StudentNode;

typedef struct
{
	StudentNode* pIDList;
	StudentNode* pHonorRollList;
	StudentNode* pAcademicProbationList;
	StudentNode* pFreshmanList;
	StudentNode* pSophomoreList;
	StudentNode* pJuniorList;
	StudentNode* pSeniorList;
} Database;

// prototype function declarations for the program functions that are below the main
void loadDatabaseFile(Database* database, const char* filename);
void addStudent(Database* database);
void displayHead(const Database* database);
void outputStudents(const Database* database, int option);
StudentNode* findStudentByIdentifier(const Database* database, const char* student_id);
bool removeStudentFromDatabase(Database* database, const char* student_id);

// the main for the program 
int main(void)
{
	Database database = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	char choice; // User's choice from the menu

	// Print the header of the program to console
	puts("CS 211, Spring 2023");
	puts("Program 4: Database of Students\n");

	puts("Enter E to start with an empty database, ");
	puts("or F to start with a database that has information on students from a file.");

	for (;;)
	{
		printf("Your choice --> ");
		scanf(" %c", &choice);
		getchar(); // Clear newline character from input buffer

		if (choice == 'E' || choice == 'F')
		{
			break;
		}
		printf("Sorry, that input was invalid. Please try again.\n");
	}

	if (choice == 'F')
	{
		char filename[100];
		printf("Enter the name of the file you would like to use: ");
		fgets(filename, sizeof(filename), stdin);

		// Remove newline character
		filename[strcspn(filename, "\n")] = 0;

		loadDatabaseFile(&database, filename);
	}

	for (;;)
	{
		puts("\nEnter: \tC to create a new student and add them to the database,");
		puts("\tR to read from the database,");
		puts("\tD to delete a student from the database, or");
		puts("\tX to exit the program.");
		printf("Your choice --> ");

		scanf(" %c", &choice);
		getchar(); // Clear newline character from input buffer

		if (choice == 'X')
		{
			puts("\nThanks for playing!");
			puts("Exiting...");
			break;
		}
		if (choice == 'C')
		{
			addStudent(&database);
		}
		else if (choice == 'R')
		{
			int option;
			puts("Select one of the following: ");
			puts("\t1) Display the head (first 10 rows) of the database");
			puts("\t2) Display students on the honor roll, in order of their GPA");
			puts("\t3) Display students on academic probation, in order of their GPA");
			puts("\t4) Display freshmen students, in order of their name");
			puts("\t5) Display sophomore students, in order of their name");
			puts("\t6) Display junior students, in order of their name");
			puts("\t7) Display senior students, in order of their name");
			puts("\t8) Display the information of a particular student");
			for (;;)
			{
				printf("Your choice --> ");
				scanf("%d", &option);
				getchar(); // Clear newline character from input buffer

				if (option == 1)
				{
					displayHead(&database);
					break;
				}
				if (option >= 2 && option <= 7)
				{
					outputStudents(&database, option - 1);
					break;
				}
				if (option == 8)
				{
					char student_id[100];
					printf("Enter the id of the student to find: ");
					fgets(student_id, sizeof(student_id), stdin);
					student_id[strcspn(student_id, "\n")] = 0; // Removes newline character

					const StudentNode* found_student_node = findStudentByIdentifier(&database, student_id);
					if (found_student_node)
					{
						printf("%s: \n", found_student_node->pStudent->name);
						printf("\tID - %s\n", found_student_node->pStudent->id);
						printf("\tGPA - %.2f\n", found_student_node->pStudent->gpa);
						printf("\tCredit Hours - %d\n\n", found_student_node->pStudent->creditHours);
					}
					else
					{
						printf("Sorry, there is no student in the database with the id %s.\n", student_id);
					}
					break;
				}
				puts("Sorry, that input was invalid. Please try again.");
			}
		}
		else if (choice == 'D')
		{
			char student_id[100];
			printf("Enter the id of the student to be removed: ");
			fgets(student_id, sizeof(student_id), stdin);
			student_id[strcspn(student_id, "\n")] = 0; // Remove newline character

			const bool student_deleted = removeStudentFromDatabase(&database, student_id);

			if (!student_deleted)
			{
				printf("Sorry, there is no student in the database with the id %s.\n", student_id);
			}
		}
		else
		{
			puts("Invalid option. Try again.");
		}
	}
	
	// Free all student nodes in the linked lists
	StudentNode* current = database.pIDList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp->pStudent->name);
		free(temp->pStudent->id);
		free(temp->pStudent);
		free(temp);
	}

	current = database.pHonorRollList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp);
	}

	current = database.pAcademicProbationList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp);
	}

	current = database.pFreshmanList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp);
	}

	current = database.pSophomoreList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp);
	}

	current = database.pJuniorList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp);
	}

	current = database.pSeniorList;
	while (current != NULL)
	{
		StudentNode* temp = current;
		current = current->next;
		free(temp);
	}

	// Set all pointers in the Database struct to NULL
	database.pIDList = NULL;
	database.pHonorRollList = NULL;
	database.pAcademicProbationList = NULL;
	database.pFreshmanList = NULL;
	database.pSophomoreList = NULL;
	database.pJuniorList = NULL;
	database.pSeniorList = NULL;

	return 0;
}

void freeStudentNode(StudentNode* node)
{
	// Free the student pointer first
	free(node->pStudent);

	// Then free the node itself
	free(node);
}

StudentNode* createStudentNode(Student* student)
{
	// Allocate memory for the new node and check for errors
	StudentNode* node = malloc(sizeof(StudentNode));
	if (node == NULL)
	{
		return NULL;
	}

	// Assign the student pointer and set the next node to NULL
	node->pStudent = student;
	node->next = NULL;

	// Return the new node
	return node;
}

// Define a function pointer type for comparing students
typedef bool (*StudentComparator)(StudentNode*, StudentNode*);

// Comparator function for comparing students by GPA
bool compareByGPA(const StudentNode* a, const StudentNode* b)
{
	return a->pStudent->gpa <= b->pStudent->gpa;
}

// Comparator function for comparing students by name
bool compareByName(const StudentNode* a, const StudentNode* b)
{
	return strcmp(a->pStudent->name, b->pStudent->name) <= 0;
}

// Comparator function for comparing students by ID
bool compareByID(const StudentNode* a, const StudentNode* b)
{
	return strcmp(a->pStudent->id, b->pStudent->id) <= 0;
}

// Add a new student node to the given list in the appropriate position according to the specified comparator
void addStudentToList(StudentNode** list_head, StudentNode* new_student_node, const StudentComparator compare, bool free_node)
{
	// If the list is empty or the new node should come before the head, insert the new node at the head
	if (*list_head == NULL || compare(new_student_node, *list_head))
	{
		new_student_node->next = *list_head;
		*list_head = new_student_node;
	}
	else
	{
		// Find the appropriate position in the list for the new node
		StudentNode* current = *list_head;
		while (current->next != NULL && !compare(new_student_node, current->next))
		{
			// If the new node and the current node are equivalent, do not insert the new node
			if (compare(new_student_node, current) && compare(current, new_student_node))
			{
				if (free_node)
				{
					free(new_student_node);
				}
				return;
			}
			current = current->next;
		}

		// Insert the new node into the list at the appropriate position
		new_student_node->next = current->next;
		current->next = new_student_node;
	}
}

void loadDatabaseFile(Database* database, const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		printf("Error opening file: %s\n", filename);
		return;
	}

	char line[100];

	fgets(line, sizeof(line), file); // Skip the header line

	while (fgets(line, sizeof(line), file))
	{
		Student* student = malloc(sizeof(Student));
		student->name = (char*)malloc(100 * sizeof(char));
		student->id = (char*)malloc(100 * sizeof(char));

		sscanf(line, "%[^,],%[^,],%lf,%d", student->name, student->id, &student->gpa, &student->creditHours);

		StudentNode* new_student_node_id = createStudentNode(student);
		addStudentToList(&(database->pIDList), new_student_node_id, compareByID, false);


		// Check if student is eligible for honor roll or academic probation
		const int is_honor_roll = student->gpa >= 3.5;
		const int is_academic_probation = student->gpa < 2.0;

		if (is_honor_roll)
		{
			StudentNode* new_student_node_honor = createStudentNode(student);
			addStudentToList(&(database->pHonorRollList), new_student_node_honor, compareByGPA, true);
		}
		else if (is_academic_probation)
		{
			StudentNode* new_student_node_probation = createStudentNode(student);
			addStudentToList(&(database->pAcademicProbationList), new_student_node_probation, compareByGPA, true);
		}

		// Determine the student's class standing
		StudentNode** class_list = NULL;

		if (student->creditHours < 30)
		{
			class_list = &(database->pFreshmanList);
		}
		else if (student->creditHours < 60)
		{
			class_list = &(database->pSophomoreList);
		}
		else if (student->creditHours < 90)
		{
			class_list = &(database->pJuniorList);
		}
		else
		{
			class_list = &(database->pSeniorList);
		}

		// Add student to the appropriate class standing list
		if (class_list != NULL)
		{
			StudentNode* new_student_node_class = createStudentNode(student);
			addStudentToList(class_list, new_student_node_class, compareByName, true);
		}
	}

	fclose(file);
}

void outputStudent(const Student* student)
{
	// menu output
	printf("%s: \n", student->name);
	printf("\tID - %s\n", student->id);
	printf("\tGPA - %.2f\n", student->gpa);
	printf("\tCredit Hours - %d\n\n", student->creditHours);
}

void displayHead(const Database* database)
{
	StudentNode* temp_head = NULL;
	const StudentNode* current = database->pIDList;

	// Create a temporary copy of the ID list
	while (current)
	{
		Student* student = current->pStudent;
		StudentNode* new_node = createStudentNode(student);
		addStudentToList(&temp_head, new_node, compareByID, false);
		current = current->next;
	}

	// Print the first 10 students
	int count = 0;
	current = temp_head;
	while (current && count < 10)
	{
		outputStudent(current->pStudent);
		current = current->next;
		count++;
	}

	// Free the memory allocated for the temporary list
	while (temp_head)
	{
		StudentNode* temp = temp_head;
		temp_head = temp_head->next;
		free(temp);
	}
}

void outputStudents(const Database* database, const int option)
{
	const StudentNode* current = NULL;

	// Select the appropriate list based on the option
	switch (option)
	{
	case 1: // Honor Roll students
		current = database->pHonorRollList;
		break;
	case 2: // Academic Probation students
		current = database->pAcademicProbationList;
		break;
	case 3: // Freshman students
		current = database->pFreshmanList;
		break;
	case 4: // Sophomore students
		current = database->pSophomoreList;
		break;
	case 5: // Junior students
		current = database->pJuniorList;
		break;
	case 6: // Senior students
		current = database->pSeniorList;
		break;
	default: // invalid option
		printf("Invalid option.\n");
		return;
	}

	// Check if the selected list is empty
	if (current == NULL)
	{
		puts("There are no students matching that criteria.");
	}
	else
	{
		// Print the student details for each student in the selected list
		while (current)
		{
			const Student* student = current->pStudent;

			printf("%s: \n", student->name);
			printf("\tID - %s\n", student->id);
			printf("\tGPA - %.2f\n", student->gpa);
			printf("\tCredit Hours - %d\n\n", student->creditHours);
			current = current->next;
		}
	}
}

void addStudentToDatabase(Database* database, Student* student)
{
	// Add the student to the ID list
	StudentNode* new_student_node_id = createStudentNode(student);
	addStudentToList(&(database->pIDList), new_student_node_id, compareByID, false);

	// Add the student to the Honor Roll or Academic Probation lists if applicable
	if (student->gpa >= 3.5)
	{
		StudentNode* new_student_node_honor_roll = createStudentNode(student);
		addStudentToList(&(database->pHonorRollList), new_student_node_honor_roll, compareByGPA, false);
	}
	else if (student->gpa < 2.0)
	{
		StudentNode* new_student_node_probation = createStudentNode(student);
		addStudentToList(&(database->pAcademicProbationList), new_student_node_probation, compareByGPA, false);
	}

	// Add the student to the appropriate class list based on their credit hours
	StudentNode* new_student_node_class;
	if (student->creditHours < 30)
	{
		new_student_node_class = createStudentNode(student);
		addStudentToList(&(database->pFreshmanList), new_student_node_class, compareByName, false);
	}
	else if (student->creditHours < 60)
	{
		new_student_node_class = createStudentNode(student);
		addStudentToList(&(database->pSophomoreList), new_student_node_class, compareByName, false);
	}
	else if (student->creditHours < 90)
	{
		new_student_node_class = createStudentNode(student);
		addStudentToList(&(database->pJuniorList), new_student_node_class, compareByName, false);
	}
	else
	{
		new_student_node_class = createStudentNode(student);
		addStudentToList(&(database->pSeniorList), new_student_node_class, compareByName, false);
	}
}

void addStudent(Database* database)
{
	// Allocate memory for the new student
	Student* student = malloc(sizeof(Student));
	student->name = (char*)malloc(100 * sizeof(char));
	student->id = (char*)malloc(100 * sizeof(char));

	// Get the student's information from the user
	printf("Enter the name of the new student: ");
	fgets(student->name, 100, stdin);
	student->name[strcspn(student->name, "\n")] = 0;

	printf("Enter the ID of the new student: ");
	fgets(student->id, 100, stdin);
	student->id[strcspn(student->id, "\n")] = 0;

	printf("Enter the GPA of the new student: ");
	scanf("%lf", &student->gpa);

	printf("Enter the credit hours of the new student: ");
	scanf("%d", &student->creditHours);
	getchar();

	// Add the student to the database
	addStudentToDatabase(database, student);

	// Print confirmation and student's information
	puts("Successfully added the following student to the database!");
	printf("%s:\n", student->name);
	printf("\tID - %s\n", student->id);
	printf("\tGPA - %.2f\n", student->gpa);
	printf("\tCredit Hours - %d\n\n", student->creditHours);
}

// Remove a student with the specified ID from the given list and return the removed student
Student* removeStudentFromList(StudentNode** list_head, const char* student_id)
{
	// Start searching from the head of the list
	StudentNode* currNode = *list_head;
	StudentNode* prevNode = NULL;
	Student* removedStudent = NULL;

	// Traverse the list until the student is found or the end is reached
	while (currNode)
	{
		if (strcmp(currNode->pStudent->id, student_id) == 0)
		{
			// Update the previous node's next pointer to skip the removed node
			if (prevNode)
			{
				prevNode->next = currNode->next;
			}
			else // Update the head pointer if the removed node is the head
			{
				*list_head = currNode->next;
			}

			// Save the removed student and free the removed node
			removedStudent = currNode->pStudent;
			free(currNode);
			break;
		}

		// Move on to the next node
		prevNode = currNode;
		currNode = currNode->next;
	}

	// Return the removed student, which could be NULL if the student was not found
	return removedStudent;
}

bool removeStudentFromDatabase(Database* database, const char* student_id)
{
	Student* removed_student = NULL;

	Student* temp = removeStudentFromList(&(database->pIDList), student_id);
	if (temp)
		removed_student = temp;

	temp = removeStudentFromList(&(database->pHonorRollList), student_id);
	if (temp)
		removed_student = temp;

	temp = removeStudentFromList(&(database->pAcademicProbationList), student_id);
	if (temp)
		removed_student = temp;

	temp = removeStudentFromList(&(database->pFreshmanList), student_id);
	if (temp)
		removed_student = temp;

	temp = removeStudentFromList(&(database->pSophomoreList), student_id);
	if (temp)
		removed_student = temp;

	temp = removeStudentFromList(&(database->pJuniorList), student_id);
	if (temp)
		removed_student = temp;

	temp = removeStudentFromList(&(database->pSeniorList), student_id);
	if (temp)
		removed_student = temp;

	if (removed_student)
	{
		if (removed_student->name)
		{
			free(removed_student->name);
		}
		if (removed_student->id)
		{
			free(removed_student->id);
		}
		free(removed_student);
		return true;
	}

	return false;
}

// Find a student in the database by ID and return the corresponding StudentNode
StudentNode* findStudentByIdentifier(const Database* database, const char* student_id)
{
	// Start searching from the head of the ID list
	StudentNode* current = database->pIDList;

	// Traverse the ID list until the student is found or the end is reached
	while (current != NULL)
	{
		// Compare the current student's ID with the target ID
		if (strcmp(current->pStudent->id, student_id) == 0)
		{
			// Return the StudentNode if the student is found
			return current;
		}
		current = current->next;
	}

	// Free current since it's NULL at this point
	free(current);

	// Return NULL if the student is not found in the database
	return NULL;
}