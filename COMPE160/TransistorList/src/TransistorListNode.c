/*
 * TransistorListNode.c
 *
 *  Created on: Oct 13, 2023
 *      Author: rnsot
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TransistorListNode.h"

TransistorListNode* CreateTransistorListNode(TransistorListNode* thisNode, char modelInit[], double maxVoltageInit, double maxCurrentInit) {
	thisNode = (TransistorListNode*)malloc(sizeof(TransistorListNode));
	strcpy(thisNode->model, modelInit);
	thisNode->maxVoltage = maxVoltageInit;
	thisNode->maxCurrent = maxCurrentInit;
	thisNode->nextNodePtr = NULL;
	return thisNode;
}

void InsertTransistorListNodeAfter(TransistorListNode* thisNode, TransistorListNode* newNode) {
	TransistorListNode *temp;
	temp = thisNode->nextNodePtr;
	thisNode->nextNodePtr = newNode;
	newNode->nextNodePtr = temp;
}

void SetNextTransistor(TransistorListNode* thisNode, TransistorListNode* newNode) {
	TransistorListNode *temp;
	temp = thisNode->nextNodePtr;
	thisNode->nextNodePtr = newNode;
	newNode->nextNodePtr = temp;
}

TransistorListNode* GetNextTransistorListNode(TransistorListNode* thisNode) {
	return thisNode->nextNodePtr;
}

void PrintTransistorListNode(TransistorListNode* thisNode) {
	printf("model: %s\n", thisNode->model);
	printf("maximum rated voltage (V): %0.1lf\n", thisNode->maxVoltage);
	printf("maximum rated current (mA): %0.1lf\n", thisNode->maxCurrent);
}
