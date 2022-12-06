#include "defs.h"

/*
  Function:  initGhost
  Purpose:   initialize a ghost by setting it's fields to the corresponding values provided
       in:   a double pointer to a ghost
       in:   a pointer to a RoomType
       in:   a GhostClassType that describes the type of ghost
   return:   a pointer to a GhostType with initialized contents
*/
void initGhost(GhostClassType gc, RoomType * room, GhostType ** ghost){
	// Assign the ghost some memory
	(*ghost) = (GhostType*)calloc(1, sizeof(GhostType));
	
	// Initialize it's ghost class, the current room it's in, and the boredom
	(*ghost) -> ghostType = gc;
	(*ghost) -> room = room;
	(*ghost) -> boredom = BOREDOM_MAX;
}

/*
  Function:  moveGhost
  Purpose:   moves a ghost from one room to the next
       in:   a pointer to a ghost
   return:   a pointer to a GhostType with an updated room pointer
*/
void moveGhost(GhostType * ghost){
	// Check the rooms that are attached to the room the ghost is currently in:
	RoomListType * roomList = ghost -> room -> attached;
	ghost -> room -> ghost = NULL;
	// Change the ghost's room pointer to a random room from that room's connected rooms
	ghost -> room = randRoom(roomList, C_FALSE);
	ghost -> room -> ghost = ghost;
}

/*
  Function:  addGhostEvidence
  Purpose:   add evidence to the current room the ghost is in
       in:   a pointer to a ghost
   return:   a pointer to a room with evidence inside of it's evidence list
*/
void addGhostEvidence(GhostType * ghost){
	//sem_wait(ghost -> room -> evidence -> mutex);

	GhostClassType ghostType = ghost -> ghostType;		// The Type of Ghost
	EvidenceListType * roomEvidenceList = ghost -> room -> evidence;		// The Room's Evidence List
	EvidenceType * tempEvidence = (EvidenceType*)calloc(1, sizeof(EvidenceType));;				// A Temporary Evidence Variable
	int ghostTypeNum = randInt(1,3);			// A random number b/w one to three, used to determine which type of evidence to drop
	
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
	//sem_post(ghost -> room -> evidence -> mutex);
}

/*
  Function:  spawnGhost
  Purpose:   spawns the ghost within a random room, except for the van
       in:   a pointer to a GhostType
       in:   a pointer to a BuildingType
   return:   a pointer to a GhostType with a room pointer within it
*/
void spawnGhost(GhostType * ghost, BuildingType * building){
	// Check the rooms that are attached to the room the ghost is currently in:
	RoomListType * roomList = building -> rooms;

	// Change the ghost's room pointer to a random room from that room's connected rooms
	ghost -> room = randRoom(roomList, C_TRUE);
}
