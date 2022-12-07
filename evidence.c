#include "defs.h"

/*
  Function:  cleanupGhostData
  Purpose:   initializes an evidence list
       in:   a pointer to an EvidenceListType
   return:   a pointer to an EvidenceListType that has been initialized 
*/
void initEvidenceList(EvidenceListType * evidenceList) {
	evidenceList->head = NULL;
	evidenceList->tail = NULL;
	evidenceList->size = 0;
	
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	evidenceList -> mutex = mutex;
}

/*
  Function:  initRoom
  Purpose:   initializes the evidence data with a random float in a range depending on the evidenceClassType
       in:   a double pointer to an EvidenceType
       in:   an evidenceClassType to tell what type of evidence it is
   return:   a pointer to an EvidenceType with initialized contents
*/
void initEvidence(EvidenceClassType evClass, EvidenceType ** evidence) {
	// Allocate memory to the evidence 
	EvidenceType * tempEv = (EvidenceType*) calloc(1, sizeof(EvidenceType));
	
	if (evClass == EMF) {
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
	
	tempEv -> type = evClass;
	
	*evidence = tempEv;
}

EvidenceType * getRandEvidence(EvidenceListType * evidenceList, EvidenceClassType evidenceClass) {
	// Set the current evidence node to the head of the list
	EvidenceNodeType * currEvidenceNode = evidenceList -> head;
	
	// Loop through each element of the hunter's evidence linked list
	while (currEvidenceNode != NULL && evidenceList -> size > 0){
		// Retrieve the evidence type of the current node
		if (currEvidenceNode -> evidenceData != NULL) {
		
			EvidenceClassType currEvidenceType = currEvidenceNode -> evidenceData -> type;
			
			// Compare against the evidence type you are trying to add
			if (evidenceClass == currEvidenceType) {
				EvidenceType * returnEvidence = currEvidenceNode -> evidenceData;
				return returnEvidence;
			}
		}
		
		// Set the current node to the next node
		currEvidenceNode = currEvidenceNode -> next;
	}
	return NULL;
}

/*
  Function:  getEvidenceAtIndex
  Purpose:   gets the evidence data from a specific index
       in:   a pointer to an EvidenceListType
       in:   an integer that points to which index to retrieve the evidence from
   return:   an address to an EvidenceType at the specified index, or an error message
*/
EvidenceType * getEvidenceAtIndex(EvidenceListType * evidenceList, int index){
	int evidenceListSize = evidenceList -> size;					// Size of the evidence list
	int i = 0;									// Current index in the list
	
	// If the index is valid:
	if (index <= evidenceListSize && evidenceListSize > 0) {
		// Make two temporary nodes to iterate through the linked list
		struct EvidenceNodeType * currNode = evidenceList -> head;
		
		// Keep looping till you find a matching index
		while (currNode != NULL){
			i++;
			if (i == index) break;
			currNode = currNode -> next;
		}
		
		if (i != index) printf("SOMETHING WENT WRONG WHILE SEARCHING FOR THE EVIDENCE!\n");
		else return currNode -> evidenceData;
	}
	else printf("Invalid Index!\n");
	return NULL;
}

/*
  Function:  addEvidence
  Purpose:   adds evidence to the linked list
       in:   a pointer to the EvidenceListType
       in:   a pointer to the EvidenceType you want stored
   return:   a pointer to an EvidenceListType with a pointer to the evidence data stored inside of it
*/
void addEvidence(EvidenceListType * list, EvidenceType * evidence){
	//A temporary node to store the information of the new tail to be added
	EvidenceNodeType * newNode;
	
	//Assign memory to the new node
	newNode = (EvidenceNodeType*)calloc(1, sizeof(EvidenceNodeType));
	
	//The data of the new node is the evidence data
	newNode -> evidenceData = evidence;
	
	//The new tail does not have any nodes after it, so make sure to mark it as so
	newNode -> next = NULL;
	
	// If list is empty:
	if (list -> head == NULL) {
		list -> head = newNode;
		list -> tail = newNode;
    	}
    	
    	// If list is not empty
    	else {
		list -> tail -> next = newNode;
		list -> tail = list -> tail -> next;
		
   	}
   	list -> size++;
}

/*
  Function:  cleanupEvidenceData
  Purpose:   cleans the data of the evidence node
       in:   a pointer to the EvidenceNodeType
   return:   a pointer to an EvidenceNodeType with no data inside of it
*/
void cleanupEvidenceData(EvidenceNodeType * evNode) {
	free(evNode -> evidenceData);
}	
	
/*
  Function:  cleanupEvidenceNode
  Purpose:   frees the evidence node from memory
       in:   a pointer to the EvidenceNodeType
   return:   free memory
*/
void cleanupEvidenceNode(EvidenceNodeType * evNode) {
    free(evNode);
} 	
	
/*
  Function:  cleanupEvidenceList
  Purpose:   cleans up the entire evidence list
       in:   a pointer to the EvidenceListType
   return:   an empty evidence list
*/
void cleanupEvidenceList(EvidenceListType * evList) {
	int evListSize = evList -> size;
	
	// Make two temporary nodes to iterate through the linked list
	EvidenceNodeType * currEv = evList -> head;
	EvidenceNodeType * nextEv;
	
	// Clean up every evidence in the evidence list
	while (currEv != NULL){
		nextEv = currEv -> next;
		cleanupEvidenceData(currEv);
        	cleanupEvidenceNode(currEv);
		currEv = nextEv;
	}
	free(evList);
}	


void cleanupEvidenceListNodes(EvidenceListType * evList) {
	// Make two temporary nodes to iterate through the linked list
	EvidenceNodeType * currEv = evList -> head;
	EvidenceNodeType * nextEv;
	
	// Clean up every evidence in the evidence list
	while (currEv != NULL){
		nextEv = currEv -> next;
        	cleanupEvidenceNode(currEv);
		currEv = nextEv;
	}
	free(evList);
}	
