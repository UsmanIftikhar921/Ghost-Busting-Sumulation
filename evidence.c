#include "defs.h"



void initEvidenceList(EvidenceListType * evidenceList) {
	evidenceList->head = NULL;
	evidenceList->tail = NULL;
	evidenceList->size = 0;
}

void initEvidence(evidenceClassType evClass, EvidenceType ** evidence) {
	EvidenceType tempEv = (EvidenceType*)calloc(1, sizeof(EvidenceType);
	
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


void cleanupEvidenceData(EvidenceNodeType * evNode) {
	free(evNode -> evidenceData);
}	
	
	
void cleanupEvidenceNode(EvidenceNodeType * evNode) {
    free(evNode);
} 	
	
	
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