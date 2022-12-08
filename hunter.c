#include "defs.h"

/*
  Function:  initHunter
  Purpose:   initialize a hunter by setting it's fields to the corresponding values provided
       in:   a double pointer to a HunterType
       in:   a pointer to a RoomType
       in:   a pointer to a char
       in:   the EvidenceClassType
   return:   a pointer to a HunterType with initialized contents
*/
void initHunter(EvidenceClassType ec, char * name, RoomType * room, HunterType ** hunterPtr, int id){
	// Assign some memory to the hunter
	HunterType * hunterInit = (HunterType*)calloc(1,sizeof(HunterType));
	
	// Initialize the hunter's name, the current room he is in and the type of evidence they can collect
	strcpy(hunterInit -> name, name);
	hunterInit -> room = room;
	room -> hunters -> size++;
	
	hunterInit -> evidenceType = ec;
	
	// Initialize the hunter's evidence linked list
	EvidenceListType * evidence = (EvidenceListType*)calloc(1, sizeof(EvidenceListType));
	initEvidenceList(evidence);
	hunterInit -> evidence = evidence;
	
	// Set the hunter's fear and boredom level to 0
	hunterInit -> fear = 0;
	hunterInit -> boredom = BOREDOM_MAX;
	hunterInit -> id = id;

	*hunterPtr = hunterInit;
}


void initHunterArray(HunterArrayType * hunterArray) {
	for (int i = 0; i < MAX_HUNTERS; i++){
		hunterArray -> hunters[i] = NULL;
	}
	
	hunterArray -> size = 0;
}

/*
  Function:  removeHunterFromRoom
  Purpose:   remove hunter from the room's hunter list
       in:   a pointer to a hunter
       in:   a pointer to a room
   return:   a pointer to a room without the hunter that we wanted to remove
*/
void removeHunterFromRoom(HunterType * hunter, RoomType * room){
	// Find the hunter with the same ID in the hunters array
	room -> hunters -> hunters[hunter -> id] = NULL;
	room -> hunters -> size--;
}

/*
  Function:  addHunterToRoom
  Purpose:   add hunter to the room's hunter list
       in:   a pointer to a hunter
       in:   a pointer to a room
   return:   a pointer to a room with the hunter that we wanted to add
*/
void addHunterToRoom(HunterType * hunter, RoomType * room){
	// Add hunter 
	room -> hunters -> hunters[hunter -> id] = hunter;
	// Increase the number of hunters in the room
	room -> hunters -> size++;
}

/*
  Function:  collectEvidence
  Purpose:   add evidence from the room to the hunter's evidence list
       in:   a pointer to a HunterType
   return:   a pointer to a HunterType with evidence inside of it's evidence list
*/
void collectEvidence(HunterType * hunter){
	RoomType * currRoom = hunter -> room;				// Current room hunter is in
	EvidenceListType * evidenceList = currRoom -> evidence;		// Said room's evidence List
	int evidenceSize = evidenceList -> size;			// Size of the evidence list
	EvidenceType * tempEvidence;					// Temporarily store the evidence in this variable
	
	//printf("COLLECTING EVIDENCE...\n");
	
	// Does the room have any evidence in it's evidence linked list?
	if(evidenceSize > 0){
		// If yes, collect a random piece of evidence (choose an int between 0 and size of the linked list)
		tempEvidence = getRandEvidence(evidenceList, hunter -> evidenceType);

		if (tempEvidence != NULL) {
			// Check if the evidence is ghostly or not:
			if (tempEvidence -> ghostliness == GHOSTLY){
				if (containsEvidenceType(tempEvidence -> type, hunter) == C_FALSE) {
					addEvidence(hunter -> evidence, tempEvidence);
				
					printf("%s scans the %s and finds a new piece of ghostly evidence!\n", hunter -> name, hunter -> room -> name);
				}
				// Store the evidence data by adding it to your own linked list
				printf("%s scans the %s and finds evidence that reinforces their findings!\n", hunter -> name, hunter -> room -> name);
				
				hunter -> boredom = BOREDOM_MAX;
			} else {
                hunter -> boredom -= BOREDOM_RATE;
				printf("%s scans the %s and finds standard readings.\n", hunter -> name, hunter -> room -> name);
			}
			//else printf("THE EVIDENCE IS NOT OF INTEREST!\n");
		} else {
			printf("%s hits his reader, something seems off about it.\n", hunter -> name);
		}
	} else {
		printf("The %s seems perfectly normal to %s. Too normal...\n", hunter -> room -> name, hunter -> name);
	}
}


/*
  Function:  transferEvidenceData
  Purpose:   tranfers evidence data to the hunter
       in:   a pointer to a HunterType
       in:   a pointer to an EvidenceType
   return:   a pointer to a HunterType with evidence inside it
*/
void transferEvidenceData(HunterType * hunter, EvidenceType * evidence){
	// Check the type of evidence we are about to send
	EvidenceClassType evidenceClass = evidence -> type;
	
	// See if the hunter in question already has that type of evidence
	if (containsEvidenceType(evidenceClass, hunter) == C_FALSE ) {
		addEvidence(hunter -> evidence, evidence);
	}
	//else printf("HUNTER ALREADY HAS EVIDENCE OF THIS TYPE!\n");
}

/*
  Function:  containsEvidenceType
  Purpose:   checks if the hunter's evidence array already contains the evidence class
       in:   a pointer to a HunterType
       in:   an EvidenceClassType
   return:   a pointer to a HunterType with evidence inside it
*/
int containsEvidenceType (EvidenceClassType evidenceClass, HunterType * hunter){
	EvidenceListType * evidenceList = hunter -> evidence;		// Temporary evidence list
	
	// Set the current evidence node to the head of the list
	EvidenceNodeType * currEvidenceNode = evidenceList -> head;
	
	// Loop through each element of the hunter's evidence linked list
	while (currEvidenceNode != NULL){
		// Retrieve the evidence type of the current node
		if (currEvidenceNode -> evidenceData != NULL) {
		
			EvidenceClassType currEvidenceType = currEvidenceNode -> evidenceData -> type;
			
			// Compare against the evidence type you are trying to add
			if (evidenceClass == currEvidenceType) return C_TRUE;
		}

		
		// Set the current node to the next node
		currEvidenceNode = currEvidenceNode -> next;
	}
	return C_FALSE;
}

/*
  Function:  cleanupHunterArray
  Purpose:   cleans up the entire hunter array from memory
       in:   a pointer to a HunterType
   return:   an empty hunter array
*/
void cleanupHunterArray(HunterArrayType * hunterArray){
	for (int i = 0; i < MAX_HUNTERS; i++){
		cleanupEvidenceListNodes(hunterArray -> hunters[i] -> evidence);
		//free(hunterArray -> hunters[i] -> evidence);
		
		free(hunterArray -> hunters[i]);
	}
}
