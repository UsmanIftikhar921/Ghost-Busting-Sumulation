#include "defs.h"

void initRoomList(RoomListType * roomList){
	roomList->head = NULL;
	roomList->tail = NULL;
	roomList->size = 0;
}

void initRoom(char * name, RoomType ** room){
	//Assign memory for the room data
	*room = calloc(1, sizeof(RoomType));
	
	//Initialize the room's Name
	strcpy((*room) -> name, name);
	
	//Initialize The Evidence List, Room List and Hunter Array
	initEvidenceList((*room) -> evidence);
	initRoomList((*room) -> attached);
	initHunterArray((*room) -> hunters);
}

// Adds a room to the end of a RoomListType
void addRoom(RoomListType * list, RoomType * room){	
	//A temporary node to store the information of the new tail to be added
	RoomNodeType * newNode;
	
	//Assign memory to the new node
	newNode = calloc(1, sizeof(RoomNodeType));
	
	//The data of the new node is the room data
	newNode -> data = room;
	
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
		list -> tail = newNode;
   	}
}

void connectRooms(RoomType * room1, RoomType * room2){
	// Add Room 2 to Room 1's attached roomList
	addRoom(room1->attached, room2);
	
	// Add Room 1 to Room 2's attached roomList
	addRoom(room2->attached, room1);
}

void cleanupRoom(RoomType * room){
	// Free Evidence and Room Linked List
	cleanupEvidenceList(room -> evidence);
	cleanupRoomList(room -> attached);
	// Free hunter array
	cleanupHunterArray(room -> hunters);
}

void cleanupRoomList(RoomListType * roomList){
	// Record the size of the room
	int roomListSize = roomList -> size;
	
	// Make two temporary nodes to iterate through the linked list
	RoomNodeType * currNode = roomList -> head;
	RoomNodeType * nextNode;
	
	// Clean up every room in the room list
	for(int i = 0; i < roomListSize; i++){
		nextNode = currNode -> next;
		cleanupRoom(currNode -> data);
		free(currNode);
		currNode = nextNode;
	}
}

RoomType * randRoom(RoomListType * roomList, int spawnGhostCheck) {
    if (spawnGhostCheck == C_FALSE) {
        int selectedRoom = randInt(0, roomList -> size - 1);
    } else {
        int selectedRoom = randInt(1, roomList -> size - 1);
    }

    RoomNodeType currRoom = roomList -> head;

    for (int i = 0; i < roomList -> size; i++) {
        if (i == selected) {
            return currRoom -> roomData;
        } else {
            currRoom = currRoom -> next;
        }
    }
}





