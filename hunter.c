#include "defs.h"

typedef struct {
	HunterType* hunters[MAX_HUNTERS];
	int size;
} HunterArrayType;

typedef struct {
	char name[MAX_STR];
	RoomType * room;
	EvidenceClassType evidenceType;
	EvidenceListType * evidence;
	int fear;
	int boredom;
} HunterType;

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


void collectEvidence(HunterType * hunter){
	// Take a look at the current room the hunter is in
	
	// Does the room have any evidence in it's evidence linked list?
	
	// If yes, collect a random piece of evidence (choose an int between 0 and size of the linked list)
	
	// Store this evidence data by adding it to your own linked list, and removing it from the room's linked list
}

typedef struct {
 	char name[MAX_STR];
 	GhostType * ghost;
 	struct EvidenceListType * evidence;
  	struct RoomListType * attached;
  	struct HunterArrayType * hunters;
} RoomType;

void shareEvidence(HunterType * hunter){
	// Note: Only transfer evidence that is not in standard values
	
	HunterArrayType * huntersInRoom = hunter -> room -> hunters
	int numOfHuntersInRoom = huntersInRoom -> size;
	
	int numOfEvidence = hunter -> evidence -> size;
	
	// Check if there are any hunters in the room
	if (numOfHuntersInRoom >= 2){
		// Choose a second random hunter by index
		HunterType * otherHunter;
		int otherHunterIndex = randint(0,numOfHuntersInRoom-1);
		int hunterId;
		
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

void transferEvidenceData(HunterType * hunter, EvidenceType * evidence){
	// Check the type of evidence we are about to send
	EvidenceClassType evidenceClass = evidence -> type;
	
	// Check if hunter already has that type of evidence
	int containsEvidenceType = 0;
	
	// See if the hunter in question already has that type of evidence
	if ( containsEvidenceType(evidenceClass,hunter) == C_FALSE ) {
		addEvidence(evidence, hunter -> evidence);
	}
}

int containsEvidenceType (EvidenceClassType evidenceClass, HunterType * hunter){
	// Create a temporary evidence list for simplification of code	
	EvidenceListType * evidenceList = hunter -> evidence;
	int numOfEvidence = evidenceList -> size;
	
	// Set the current evidence node to the head of the list
	EvidenceNodeType * currEvidenceNode = evidenceList -> head;
	
	// Loop through each element of the hunter's evidence linked list
	for (int i = 0; i < numOfEvidence; i++){
		// Retrieve the evidence type of the current node
		EvidenceClassType currEvidenceType = currEvidenceNode -> evidenceData -> type;
		
		// Compare against the evidence type you are trying to add
		if (evidenceClass == currEvidenceType) return C_TRUE;
		
		// Set the current node to the next node
		currEvidenceNode = currEvidenceNode -> next;
	}
	
	return C_FALSE;
}

void cleanupHunterData(HunterType *);			  	       // Frees all data from a hunter
void cleanupHunterArray(HunterArrayType *);			       // Frees all data in a hunter array
