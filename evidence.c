#include "defs.h"

// Initializes the evidence list
void initEvidenceList(EvidenceListType * evidenceList) {
	evidenceList->head = NULL;
	evidenceList->tail = NULL;
	evidenceList->size = 0;
}

// Initializes the evidence data with a random variable in the range of each class' values
void initEvidence(evidenceClassType evClass, EvidenceType ** evidence) {
	// Allocate memory to the evidence 
	EvidenceType tempEv = (EvidenceType*) calloc(1, sizeof(EvidenceType);
	
	if (evClass == EMfF) {
		tempEv->value = randFloat(4.7, 5);
		if (tempEv->value >= 4.9) {
			tempEv->ghostliness = GHOSTLY;
		} else {tempEv->ghostliness = NORMAL;}
	} else if (evClass == TEMPERATURE) {
		tempEv->value = randFloat(-10,1);
		if (tempEv->value <= 0) {
			tempEv->ghostliness = GHOSTLY;
		} else {tempEv->ghostliness = NORMAL;}
	} else if (evClass == FINGERPRINTS) {
		tempEv->value = 1;
		tempEv->ghostliness = GHOSTLY;
	} else if (evClass == SOUND) {
		tempEv->value = randInt(65,75);
		if (tempEv->value >= 70) {
			tempEv->ghostliness = GHOSTLY;
		} else {tempEv->ghostliness = NORMAL;}
	}
	
	*evidence = tempEv;
}

EvidenceType * getEvidenceAtIndex(EvidenceListType * evidenceList, int index){
	evidenceListSize = evidenceList -> size;					// Size of the evidence list
	int i = 0;									// Current index in the list
	
	// If the index is valid:
	if (index <= evidenceListSize) {
		// Make two temporary nodes to iterate through the linked list
		EvidenceNodeType * currNode = evidenceList -> head;
		EvidenceNodeType * nextNode;
		
		// Keep looping till you find a matching index
		while (currNode != NULL){
			if (i == index) break;
			currNode = currNode -> next;
			i++;
		}
		
		if (i != index) printf("SOMETHING WENT WRONG WHILE SEARCHING FOR THE EVIDENCE!\n");
		else return currNode;
	}
	else printf("Invalid Index!\n");
}

// Cleans up the evidene data
void cleanupEvidenceData(EvidenceNodeType * evNode) {
	free(evNode -> evidenceData);
}	
	
// Cleans up the evidence Node	
void cleanupEvidenceNode(EvidenceNodeType * evNode) {
    free(evNode);
} 	
	
// Cleans up the entire evidence list
void cleanupEvidenceList(EvidenceListType * evList) {
	int evListSize = evList -> size;
	
	// Make two temporary nodes to iterate through the linked list
	EvidenceNodeType * currEv = evList -> head;
	EvidenceNodeType * nextEv;
	
	// Clean up every evidence in the evidence list
	for(int i = 0; i < evListSize; i++){
		nextEv = currEv -> next;
		cleanupEvidenceData(currEv);
        cleanupEvidenceNode(currEv);
		currRoom = nextRoom;
	}
}	
