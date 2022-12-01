#include "defs.h"

// Initialize the hunter
void initHunter(EvidenceClassType ec, char * name, RoomType * room, HunterType ** hunterPtr){
	// Assign some memory to the hunter
	(*hunterPtr) = calloc(1, sizeof(HunterType));
	
	// Initialize the hunter's name, the current room he is in and the type of evidence they can collect
	strcpy((*hunterPtr) -> name, name);
	(*hunterPtr) -> room = room;
	(*hunterPtr) -> evidenceType = ec;
	
	// Initialize the hunter's evidence linked list
	initEvidenceList((*hunterPtr) -> evidence);
	
	// Set the hunter's fear and boredom level to 0
	(*hunterPtr) -> fear = 0;
	(*hunterPtr) -> boredom = 0;
}


// Collect Evidence From a room
void collectEvidence(HunterType * hunter){
	RoomType * currRoom = hunter -> room;				// Current room hunter is in
	EvidenceListType * evidenceList = currRoom -> evidence;		// Said room's evidence List
	int evidenceSize = evidenceList -> size;			// Size of the evidence list
	EvidenceListType * tempEvidence;				// Temporarily store the evidence in this variable
	
	printf("COLLECTING EVIDENCE...\n");
	
	// Does the room have any evidence in it's evidence linked list?
	if(evidenceSize > 0){
		// If yes, collect a random piece of evidence (choose an int between 0 and size of the linked list)
		int randomEvidenceIndex = randint(0,(evidenceSize-1));
		tempEvidence = getEvidenceAtIndex(evidenceList, currRoomrandomEvidenceIndex);
		
		// Check if the evidence is ghostly or not:
		if ( checkGhostlyEvidence(tempEvidence) > 0 ){
			// Store the evidence data by adding it to your own linked list
			addEvidence(tempEvidence, hunter -> evidence);
		}
		else printf("THE EVIDENCE IS NOT GHOSTLY!\n");
	}
	else printf("THE EVIDENCE LIST IS EMPTY\n");
}

// Share the input hunter's evidence with another random hunter in the same room
void shareEvidence(HunterType * hunter){
	// Note: Only transfer evidence that is not in standard values
	
	HunterArrayType * huntersInRoom = hunter -> room -> hunters		// An array of hunters currently in the room
	int numOfHuntersInRoom = huntersInRoom -> size;				// The number of hunters currently in the room
	int numOfEvidence = hunter -> evidence -> size;				// The number of evidence in the hunter's evidence list
	
	// If there are at least two hunters in the room:
	if (numOfHuntersInRoom >= 2){
		// Choose a second random hunter by index
		HunterType * otherHunter;
		int otherHunterIndex = randint(1,numOfHuntersInRoom);		// A randomly chosen index in the hunter's array
		int hunterId;							// A hunter's ID
		
		HunterType * currHunter = huntersInRoom[0]	
		
		// In a for loop, transfer all evidence to the other hunter's evidence list
		for (int i = 0; i < numOfHuntersInRoom; i++){
			// If they are not the same hunter
			if (currHunter -> id == hunter -> id){
				hunterId = i;
			}
		}
		
				
	}
	
	
	// If there are only two hunters in the room,
	// Transfer all the evidence in their array to the other
	
	// If there are multiple hunters in the room,
	// Pick a random hunter, and transfer all the evidence in their array
}

// Tranfers evidence data to the hunter
void transferEvidenceData(HunterType * hunter, EvidenceType * evidence){
	// Check the type of evidence we are about to send
	EvidenceClassType evidenceClass = evidence -> type;
	
	// See if the hunter in question already has that type of evidence
	if ( containsEvidenceType(evidenceClass,hunter) == C_FALSE ) {
		addEvidence(evidence, hunter -> evidence);
	}
	else printf("HUNTER ALREADY HAS EVIDENCE OF THIS TYPE!\n");
}

// Checks if the hunter's evidence array already contains the evidence class
int containsEvidenceType (EvidenceClassType evidenceClass, HunterType * hunter){
	EvidenceListType * evidenceList = hunter -> evidence;		// Temporary evidence list
	int numOfEvidence = evidenceList -> size;			// Number of evidence in list
	
	// Set the current evidence node to the head of the list
	EvidenceNodeType * currEvidenceNode = evidenceList -> head;
	
	// Loop through each element of the hunter's evidence linked list
	for (int i = 1; i < numOfEvidence; i++){
		// Retrieve the evidence type of the current node
		EvidenceClassType currEvidenceType = currEvidenceNode -> evidenceData -> type;
		
		// Compare against the evidence type you are trying to add
		if (evidenceClass == currEvidenceType) return C_TRUE;
		
		// Set the current node to the next node
		currEvidenceNode = currEvidenceNode -> next;
	}
	
	return C_FALSE;
}

// Cleans up the hunter's data and frees it from memory
void cleanupHunterData(HunterType * hunter){
	// Clean up the evidence list
	cleanupEvidenceList(hunter -> evidence);
	// Free the hunter from memory
	free(hunter);
}

// Cleans up the entire hunter array from memory
void cleanupHunterArray(HunterArrayType * hunterArray){
	hunterArraySize = hunterArray -> size;
	
	for (int i = 0, i < hunterArraySize; i++){
		cleanupHunterData(hunterArray[i]);
	}
}
