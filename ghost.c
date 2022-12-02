#include "defs.h"

void initGhost(GhostClassType gc, RoomType * room, GhostType ** ghost){
	// Assign the ghost some memory
	(*ghost) = (GhostType*)calloc(1, sizeof(GhostType));
	
	// Initialize it's ghost class, the current room it's in, and the boredom
	(*ghost) -> ghostClass = gc;
	(*ghost) -> room = room;
	(*ghost) -> boredom = 0;
}

void moveGhost(GhostType * ghost){
	// Check the rooms that are attached to the room the ghost is currently in:
	RoomListType * roomList = ghost -> room -> attached	
	
	// Change the ghost's room pointer to a random room from that room's connected rooms
	ghost -> room = randRoom(roomList, C_FALSE);
}

// Add evidence to the room the ghost is currently in
void addGhostEvidence(GhostType * ghost){
	GhostClassType ghostType = ghost -> ghostClass;		// The Type of Ghost
	roomEvidenceList = ghost -> room -> evidence;		// The Room's Evidence List
	EvidenceType * tempEvidence;				// A Temporary Evidence Variable
	int ghostTypeNum = randint(1,3);			// A random number b/w one to three, used to determine which type of evidence to drop
	
	//Print out Ghost Type:
	switch(ghostType){
		case POLTERGEIST :
			// Choose b/w EMF, Temperature and Fingerprints
			if(ghostTypeNum == 1) initEvidence(EMF, &tempEvidence);
			else if (ghostTypeNum == 2) initEvidence(TEMPERATURE, &tempEvidence);
			else initEvidence(FINGERPRINTS, &tempEvidence);
			break;
			
		case PHANTOM:
			// Choose b/w Temperature, Fingerprints and Sound
			if(ghostTypeNum == 1) initEvidence(TEMPERATURE, &tempEvidence);
			else if (ghostTypeNum == 2) initEvidence(FINGERPRINTS, &tempEvidence);
			else initEvidence(SOUND, &tempEvidence);
			break;
			
		case BULLIES:
			// Choose b/w EMF, Fingerprints and Sound
			if(ghostTypeNum == 1) initEvidence(EMF, &tempEvidence);
			else if (ghostTypeNum == 2) initEvidence(FINGERPRINTS, &tempEvidence);
			else initEvidence(SOUND, &tempEvidence);
			break;
			
		case BANSHEE:
			// Choose b/w EMF, Temperature and Sound
			if(ghostTypeNum == 1) initEvidence(EMF, &tempEvidence);
			else if (ghostTypeNum == 2) initEvidence(TEMPERATURE, &tempEvidence);
			else initEvidence(SOUND, &tempEvidence);
			break;
			
		default:
			printf("UNKNOWN GHOST TYPE!");
			break;	
	}
	
	// Add the evidence data to the room's evidence linked list
	addEvidence(roomEvidenceList, tempEvidence);
}

void cleanupGhostData(GhostType * ghost){
	free (ghost);
}

void spawnGhost(GhostType * ghost, BuildingType * building){
	// Check the rooms that are attached to the room the ghost is currently in:
	RoomListType * roomList = building -> rooms;

	// Change the ghost's room pointer to a random room from that room's connected rooms
	ghost -> room = randRoom(roomList, C_TRUE);
}
