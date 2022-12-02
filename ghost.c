#include "defs.h"

typedef struct {
	GhostClassType ghostClass;		// The type of Ghost
	RoomType * room;			// Pointer to the room it's in
	int boredom;
} GhostType;

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
	// Choose a random evidence class
	
	// Add the evidence data to the room's evidence linked list
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
