#include "defs.h"

/*
  Function:  initGhost
  Purpose:   initialize a ghost by setting it's fields to the corresponding values provided
       in:   a double pointer to a ghost
       in:   a pointer to a RoomType
       in:   a GhostClassType that describes the type of ghost
   return:   a pointer to a GhostType with initialized contents
*/
void initGhost(GhostClassType gc, RoomType * room, GhostType ** ghost, BuildingType * building){
	// Assign the ghost some memory
	GhostType * ghostInit = (GhostType*)calloc(1, sizeof(GhostType));
	
	// Initialize it's ghost class, the current room it's in, and the boredom
	ghostInit -> ghostType = gc;
	ghostInit -> room = room;
	ghostInit -> boredom = BOREDOM_MAX;
	ghostInit -> building = building;
	
	*ghost = ghostInit; // sets ghost double pointer
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
	RoomType * newRoom = randRoom(roomList, C_FALSE);
	
	if (sem_trywait(&newRoom -> mutex) == 0) {
		
		ghost -> room -> ghost = NULL;
		sem_post(&ghost -> room -> mutex);
		// Change the ghost's room pointer to a random room from that room's connected rooms
		ghost -> room = newRoom;
		ghost -> room -> ghost = ghost;
	
	}	

}

/*
  Function:  addGhostEvidence
  Purpose:   add evidence to the current room the ghost is in
       in:   a pointer to a ghost
   return:   a pointer to a room with evidence inside of it's evidence list
*/
void addGhostEvidence(GhostType * ghost){
	GhostClassType ghostType = ghost -> ghostType;		// The Type of Ghost
	EvidenceListType * roomEvidenceList = ghost -> room -> evidence;		// The Room's Evidence List
	EvidenceType * tempEvidence = (EvidenceType*)calloc(1, sizeof(EvidenceType));				// A Temporary Evidence Variable
	int ghostTypeNum = randInt(1,3);			// A random number b/w one to three, used to determine which type of evidence to drop
	
	//Print out Ghost Type:
	switch(ghostType){
		case POLTERGEIST :
			// Choose b/w EMF, Temperature and Fingerprints
			if(ghostTypeNum == 1) { 
				initEvidence(EMF, &tempEvidence);
				printf("A menacing aura emanates from %s.\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(TEMPERATURE, &tempEvidence);
				printf("The air of the %s chills suddenly.\n", ghost -> room -> name);
			}
			else {
				initEvidence(FINGERPRINTS, &tempEvidence);
				printf("Fingerprints start to appear on the walls of the %s.\n", ghost -> room -> name);
			}
			break;
			
		case PHANTOM:
			// Choose b/w Temperature, Fingerprints and Sound
			if(ghostTypeNum == 1) {
				initEvidence(TEMPERATURE, &tempEvidence);
				printf("Frost crackles on the walls in the %s.\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(FINGERPRINTS, &tempEvidence);
				printf("In the %s, a mug slowly starts to levitate.\n", ghost -> room -> name);
			}
			else { 
				initEvidence(SOUND, &tempEvidence);
				printf("A scream can be heard coming from the %s.\n", ghost -> room -> name);
			}
			break;
			
		case BULLIES:
			// Choose b/w EMF, Fingerprints and Sound
			if(ghostTypeNum == 1) { 
				initEvidence(EMF, &tempEvidence);
				printf("The air seems different in the %s.\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(FINGERPRINTS, &tempEvidence);
				printf("Several things are moved arund in the %s.\n", ghost -> room -> name);
			}
			else {
				initEvidence(SOUND, &tempEvidence);
				printf("A baby crying is heard coming from the %s.\n", ghost -> room -> name);
			}
			break;
			
		case BANSHEE:
			// Choose b/w EMF, Temperature and Sound
			if(ghostTypeNum == 1) {
				initEvidence(EMF, &tempEvidence);
				printf("Lights in the %s shift and fray.\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(TEMPERATURE, &tempEvidence);
				printf("Ice crackles underfoot in the %s.\n", ghost -> room -> name);
			}
			else {
				initEvidence(SOUND, &tempEvidence);
				printf("Something is scratching the walls of the %s.\n", ghost -> room -> name);
			}
			break;
			
		default:
			printf("UNKNOWN GHOST TYPE!");
			break;	
	}
	
	// Add the evidence data to the room's evidence linked list
	addEvidence(roomEvidenceList, tempEvidence);
	addEvidence(ghost -> building -> evidence, tempEvidence);
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

/*
  Function:  cleanupGhostData
  Purpose:   frees the ghost from memory
       in:   a pointer to a GhostType
   return:   free memory
*/
void cleanupGhostData(GhostType * ghost) {
	free(ghost);
}
