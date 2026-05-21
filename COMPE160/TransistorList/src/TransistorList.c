/*
** Assignment: Laboratory Assignment 08 "TransistorList"
**
** Filename: TransistorList.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 10/17/2023, 9PM
**
** Description: Prompts user to enter a menu option choice, creates a linked list of transistors given specifications: model name, max voltage, max current
** can edit list by adding transistors, removing transistors, changing the order of transistors, and the option to quit the menu
**
** Input: a title for the transistor list, a character for menu choice, transistor model name, real numbers for max voltage and current, position of transistor
**
** Output: A menu, a list of transistors
**
** Algorithm: Prompts user for a title, prompts user for menu choice, calls functions from TransistorListNode.c using TransistorListNode struct
**
** Caveats: couldn't really tell the difference between InsertTransistorListNodeAfter and SetNextTransistor - sounds too similar
** Version: 1.0
**
*/



/*
 * Example Transistors
 * Transistor - NPN, 50V 800mA (BC337)
 * Transistor - NPN, 60V 200mA (2N3904)
 * Transistor - NPN, 50V 4A (2N51926)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TransistorListNode.h"

void PrintMenu(char titleListTitle[]) {
	printf("%s MENU\n", titleListTitle);
	printf("a - Add transistor\n");
	printf("r - Remove transistor\n");
	printf("c - Change position of transistor\n");
	printf("o - Output full transistor list\n");
	printf("q - Quit\n");
	printf("\n");
}

TransistorListNode* ExecuteMenu(char menuOp, char titleListTitle[], TransistorListNode *headPtr) {

	if (menuOp == 'a' || menuOp == 'r' || menuOp == 'c' || menuOp == 'o') {
		TransistorListNode* headNode = (TransistorListNode*)malloc(sizeof(TransistorListNode));
		headNode = headPtr;
		TransistorListNode* thisNode = (TransistorListNode*)malloc(sizeof(TransistorListNode));
		thisNode = headPtr;

		char model[50] = "";
		double transistorVoltage = 0.0;
		double transistorCurrent = 0.0;
		int transistorPosition = 0;

		switch(menuOp) {
		case 'a':
			printf("ADD TRANSISTOR\n");

			printf("Enter transistor's unique model:\n");
			fflush(stdout);
			scanf("%s", model);

			printf("Enter maximum rated voltage (V):\n");
			fflush(stdout);
			scanf("%lf", &transistorVoltage);

			printf("Enter maximum rated current (mA):\n");
			fflush(stdout);
			scanf("%lf", &transistorCurrent);

			if (headPtr == NULL) {
				headNode = CreateTransistorListNode(headNode, model, transistorVoltage, transistorCurrent);
				headPtr = headNode;
			}
			else {
				TransistorListNode* newNode = (TransistorListNode*)malloc(sizeof(TransistorListNode));
				newNode = CreateTransistorListNode(newNode, model, transistorVoltage, transistorCurrent);
				TransistorListNode* thisNode = headPtr;
				while (thisNode->nextNodePtr != NULL) {
					thisNode = thisNode->nextNodePtr;
				}
				InsertTransistorListNodeAfter(thisNode, newNode);
			}
			break;

		case 'r':
			int removed = 0;
			if (thisNode == NULL) {
				printf("No Nodes!\n");
				break;
			}

			printf("REMOVE TRANSISTOR\n");
			printf("Enter transistor's unique model:\n");
			fflush(stdout);
			scanf("%s", model);

			if (strcmp(thisNode->model, model) == 0) {
				if (thisNode->nextNodePtr != NULL) {
					headPtr = thisNode->nextNodePtr;
				}
				else {
					headPtr = NULL;
				}
				free(thisNode);
				removed = 1;
			}
			else {
				while (thisNode->nextNodePtr != NULL) {
					if (strcmp(thisNode->nextNodePtr->model, model) == 0) {
						TransistorListNode* removeNode = thisNode->nextNodePtr;
						thisNode->nextNodePtr = thisNode->nextNodePtr->nextNodePtr;

						free(removeNode);
						removed = 1;
						break;
					}
					thisNode = thisNode->nextNodePtr;
				}


			}
			if (removed == 0) {
			printf("No node with that model\n");
			break;
			}

			printf("\"%s\" removed\n", model);
			fflush(stdout);

			break;

		case 'c':
			fflush(stdout);
			printf("CHANGE POSITION OF TRANSISTOR\n");
			if (headPtr == NULL) {
				fflush(stdout);
				printf("Transistor list is empty.\n");
			}
			else {
				int transistorPosition, newTransistorPosition;
				int numNodes = 0;
				TransistorListNode* current = headPtr;
				while (current != NULL) {
                    numNodes++;
                    current = current->nextNodePtr;
				}
				fflush(stdout);

				printf("Enter transistor's current position (1-%d): ", numNodes);
				fflush(stdout);
				scanf("%d", &transistorPosition);
				fflush(stdout);
				printf("Enter new position for transistor (1-%d): ", numNodes);
				fflush(stdout);
				scanf("%d", &newTransistorPosition);

				if (transistorPosition < 1)
					transistorPosition = 1;
				if (newTransistorPosition < 1)
					newTransistorPosition = 1;
				if (transistorPosition > numNodes)
					transistorPosition = numNodes;
				if (newTransistorPosition > numNodes)
					newTransistorPosition = numNodes;

				if (transistorPosition != newTransistorPosition) {
					TransistorListNode* current = headPtr;
					TransistorListNode* prev = NULL;
					for (int i = 1; i < transistorPosition; i++) {
						prev = current;
						current = current->nextNodePtr;
					}

					if (prev == NULL) {
						headPtr = current->nextNodePtr;
					}
					else {
						prev->nextNodePtr = current->nextNodePtr;
					}

					if (newTransistorPosition == 1) {
						current->nextNodePtr = headPtr;
						headPtr = current;
					}
					else {
						TransistorListNode* insertAfter = headPtr;
						for (int i = 1; i < newTransistorPosition - 1; i++) {
							insertAfter = insertAfter->nextNodePtr;
						}
						current->nextNodePtr = insertAfter->nextNodePtr;
						insertAfter->nextNodePtr = current;
					}
					fflush(stdout);

					printf("\"%s\" moved to position %d.\n", current->model, newTransistorPosition);
				}
				else {
					fflush(stdout);
					printf("Transistor is already in the specified position.\n");
				}
			}
			break;

			case 'o':
			if (headPtr == NULL) {
				printf("%s list is empty\n", titleListTitle);
			}
			else {
				transistorPosition = 1;
				while (thisNode != NULL) {
					printf("%d.\n", transistorPosition);
					PrintTransistorListNode(thisNode);
					thisNode = thisNode->nextNodePtr;
					++transistorPosition;
				}
			}

			break;

		default:
			printf("shouldn't be accessible...\n");
			break;
		}

	}
	printf("\n");
	return headPtr;
}


int main(void) {

	char transistorTitle[50];
	printf("Enter the transistor list's title:\n");
	fflush(stdout);
	fgets(transistorTitle, sizeof(transistorTitle), stdin);

	transistorTitle[strlen(transistorTitle)-2] = '\0';



	char menuOp = '0';
	TransistorListNode* headNode = NULL;

	while(menuOp != 'q') {
		PrintMenu(transistorTitle);
		printf("Choose an option:\n");
		fflush(stdout);
		scanf(" %c", &menuOp);

		headNode = ExecuteMenu(menuOp, transistorTitle, headNode);
	}

	return 0;
}
