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
  Function:  moveHunter
  Purpose:   moves a hunter from one room to the next
       in:   a pointer to a HunterType
   return:   a pointer to a HunterType with an updated room pointer
*/
void moveHunter (HunterType * hunter) {
	// Check the rooms that are attached to the room the hunter is currently in:
	RoomListType * roomList = hunter -> room -> attached;
	RoomType * newRoom = randRoom(roomList, C_FALSE);


	if (sem_trywait(&newRoom -> mutex) == 0) {	
		// Decrease the number of hunters in the current room
		hunter -> room -> hunters -> size--;

	
		// Remove hunter from linked list
		removeHunterFromRoom(hunter, hunter -> room);

		sem_post(&hunter -> room -> mutex);
	
		// Change the hunters's room pointer to a random room from that room's connected rooms
		hunter -> room = newRoom;

		// Add hunter to the linked list
		addHunterToRoom(hunter, hunter -> room);
		
		// Increase the number of hunters in the current room
		hunter -> room -> hunters -> size++;
	}
}

// Remove hunter from the room's hunter list
void removeHunterFromRoom(HunterType * hunter, RoomType * room){
	// Find the hunter with the same ID in the hunters array
	room -> hunters -> hunters[hunter -> id] = NULL;
	room -> hunters -> size--;
}

// Add hunter to the room's hunter list
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
				printf("%s scans the %s and finds standard readings.\n", hunter -> name, hunter -> room -> name);
			}
			//else printf("THE EVIDENCE IS NOT OF INTEREST!\n");
		} else {
			printf("%s hits his reader, something seems off about it.\n", hunter -> name);
		}
	} else {
		printf("The %s seems perfectly normal to %s. Too normal...\n", hunter -> room -> name, hunter -> name);
	}
	//else printf("THE EVIDENCE LIST IS EMPTY\n");
}

/*
  Function:  shareEvidence
  Purpose:   Share the input hunter's evidence with another random hunter in the same room
       in:   a pointer to a HunterType
   return:   a randomly chosen second HunterType with evidence inside of it's evidence list
*/
void shareEvidence(HunterType * hunter){
	// Note: Only transfer evidence that is not in standard values
	HunterArrayType * huntersInRoom = hunter -> room -> hunters;			// An array of hunters currently in the room
	int numOfHuntersInRoom = huntersInRoom -> size;					// The number of hunters currently in the room
	int numOfEvidence = hunter -> evidence -> size;					// The number of evidence in the hunter's evidence list
	
	// If there are at least two hunters in the room:
	if (numOfHuntersInRoom >= 2){
		// Choose a second random hunter by index
		int otherHunterIndex = randInt(1,numOfHuntersInRoom);			// A randomly chosen index in the hunter's array
        	int count = 0;

		struct HunterType * currHunter;
		
		// In a for loop, transfer all evidence to the other hunter's evidence list
		for (int i = 0; i < numOfHuntersInRoom; i++){
			currHunter = huntersInRoom -> hunters[i];
			// If they are not the same hunter
			if (currHunter -> id != hunter -> id){	
                		count++;
            		}
            		if (count == otherHunterIndex) {
                		EvidenceNodeType * evidenceNode = hunter -> evidence -> head;
                		for (int i = 0; i < numOfEvidence; i++){
                    			transferEvidenceData(huntersInRoom -> hunters[i], evidenceNode -> evidenceData);
                    			evidenceNode = evidenceNode -> next;
                		}
                		break;
            		}
		}
	}
}

// Tranfers evidence data to the hunter
void transferEvidenceData(HunterType * hunter, EvidenceType * evidence){
	// Check the type of evidence we are about to send
	EvidenceClassType evidenceClass = evidence -> type;
	
	// See if the hunter in question already has that type of evidence
	if (containsEvidenceType(evidenceClass, hunter) == C_FALSE ) {
		addEvidence(hunter -> evidence, evidence);
	}
	//else printf("HUNTER ALREADY HAS EVIDENCE OF THIS TYPE!\n");
}


// Checks if the hunter's evidence array already contains the evidence class
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

// Cleans up the hunter's data and frees it from memory
void cleanupHunterData(HunterType * hunter){
	// Clean up the evidence list
	//cleanupEvidenceListNodes(hunter -> evidence);
	// Free the hunter from memory
	//free(hunter);
}

// Cleans up the entire hunter array from memory
void cleanupHunterArray(HunterArrayType * hunterArray){
	int hunterArraySize = hunterArray -> size;
	
	for (int i = 0; i < hunterArraySize; i++){
		cleanupHunterData(hunterArray -> hunters[i]);
		
		free(hunterArray -> hunters[i]);
	}
}
