#include "defs.h"

/*
  Function:  initRoomList
  Purpose:   initialize a list of rooms by setting it's head and tail to null, and it's size to 0
       in:   a pointer to a RoomListType
   return:   a pointer to a RoomListType that has been initialized
*/
void initRoomList(struct RoomListType * roomList){
	RoomNodeType * temp = NULL; 
	roomList -> head = temp;
	roomList -> tail = temp;
	roomList -> size = 0;
}

/*
  Function:  initRoom
  Purpose:   initialize a room by setting it's fields to the corresponding values provided
       in:   a double pointer to a room
       in:   a pointer to the name of the room
   return:   a pointer to a RoomType with initialized contents
*/
void initRoom(RoomType ** room, char * name){

	RoomType * roomInit = (RoomType*)calloc(1,sizeof(RoomType));
		
	//Initialize the room's Name
	strcpy(roomInit -> name, name);
	
	//Initialize The Evidence List, Room List and Hunter Array
	EvidenceListType * evidence = (EvidenceListType*)calloc(1, sizeof(EvidenceListType));
	initEvidenceList(evidence);
	roomInit -> evidence = evidence;

	RoomListType * rooms = (RoomListType*)calloc(1, sizeof(RoomListType));
	initRoomList(rooms);
	roomInit -> attached = rooms;
	
	HunterArrayType * hunters = (HunterArrayType*)calloc(1, sizeof(HunterArrayType));
	initHunterArray(hunters);
	roomInit -> hunters = hunters;
	
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	roomInit -> mutex = mutex;
	
	
	*room = roomInit;
}

/*
  Function:  addRoom
  Purpose:   adds a room to the room list
       in:   a pointer to a RoomListType
       in:   a pointer to a RoomType
   return:   a pointer to a RoomListType with a room element inside it
*/
void addRoom(RoomListType * list, RoomType * room){	
	//A temporary node to store the information of the new tail to be added
	RoomNodeType * newNode;
	
	//Assign memory to the new node
	newNode = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
	
	//The data of the new node is the room data
	newNode -> roomData = room;
	
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
   	list -> size++;
}

/*
  Function:  connectRooms
  Purpose:   connects two rooms together by adding the opposing room to each other's connected rooms list
       in:   a pointer to a RoomType for the first room
       in:   a pointer to a RoomType for the second room
   return:   pointers to two rooms that are connected to each other
*/
void connectRooms(RoomType * room1, RoomType * room2){
	// Add Room 2 to Room 1's attached roomList
	addRoom(room1->attached, room2);
	
	// Add Room 1 to Room 2's attached roomList
	addRoom(room2->attached, room1);
}

/*
  Function:  cleanupRoom
  Purpose:   clears all of the contents of the room
       in:   a pointer to a RoomType
   return:   a pointer to an empty RoomType
*/
void cleanupRoom(RoomType * room){
	// Free Evidence and Room Linked List
	cleanupEvidenceListNodes(room -> evidence);
	cleanupRoomListNodes(room -> attached);
	// Free hunter array
	free(room -> hunters);
	free(room);
}

/*
  Function:  cleanupRoomList
  Purpose:   clears all of the contents of the room list
       in:   a pointer to a RoomListType
   return:   a pointer to an empty RoomListType
*/
void cleanupRoomList(RoomListType * roomList){
	// Record the size of the room
	int roomListSize = roomList -> size;
	
	// Make two temporary nodes to iterate through the linked list
	RoomNodeType * currNode = roomList -> head;
	RoomNodeType * nextNode = roomList -> head -> next;
	
	// Clean up every room in the room list
	for(int i = 0; i < roomListSize; i++){
		nextNode = currNode -> next;
		cleanupRoom(currNode -> roomData);
		free(currNode);
		
		currNode = nextNode;
	}
	free(roomList);
}

/*
  Function:  cleanupRoomList
  Purpose:   clears all of the contents of the room list
       in:   a pointer to a RoomListType
   return:   a pointer to an empty RoomListType
*/
void cleanupRoomListNodes(RoomListType * roomList) {
	
	// Make two temporary nodes to iterate through the linked list
	RoomNodeType * currNode = roomList -> head;
	RoomNodeType * nextNode;
	
	// Clean up every room in the room list
	while(currNode != NULL){
		nextNode = currNode -> next;
		free(currNode);
		currNode = nextNode;
	}
	free(roomList);
}


/*
  Function:  randRoom
  Purpose:   get a random room from the room list
       in:   a pointer to a RoomListType
       in:   an integer that determines whether or not the ghost can move inside the van
   return:   a pointer to a room that has been randomly chosen
*/
RoomType * randRoom(RoomListType * roomList, int spawnGhostCheck) {
	int selectedRoom;
    	if (spawnGhostCheck == C_FALSE) {
        	selectedRoom = randInt(0, roomList -> size - 1);
    	} else {
        	selectedRoom = randInt(1, roomList -> size - 1);
    	}
	
    	RoomNodeType * currRoom = roomList -> head;

    	for (int i = 0; i < roomList -> size; i++) {
        	if (i == selectedRoom) {
            		return currRoom -> roomData;
        	} else {
            		currRoom = currRoom -> next;
        	}
    	}
    	return roomList -> head -> roomData;
}





