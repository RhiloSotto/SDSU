/*
 * TransistorListNode.h
 *
 *  Created on: Oct 13, 2023
 *      Author: rnsot
 */

#ifndef TRANSISTORLISTNODE_H_
#define TRANSISTORLISTNODE_H_

typedef struct TransistorListNode_struct {
	char model[50];
	double maxVoltage;
	double maxCurrent;
	struct TransistorListNode_struct* nextNodePtr;
} TransistorListNode;


TransistorListNode* CreateTransistorListNode(TransistorListNode* thisNode, char modelInit[], double maxVoltageInit, double maxCurrentInit);

void InsertTransistorListNodeAfter(TransistorListNode* thisNode, TransistorListNode* newNode);

void SetNextTransistor(TransistorListNode* thisNode, TransistorListNode* newNode);

TransistorListNode* GetNextTransistorListNode(TransistorListNode* thisNode);

void PrintTransistorListNode(TransistorListNode* thisNode);



#endif /* TRANSISTORLISTNODE_H_ */
