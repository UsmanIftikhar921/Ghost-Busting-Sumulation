#include "defs.h"

/*
  Function:  initBuilding
  Purpose:   initialize an building by initializing it's room list, it's hunter list, and assigning a ghost to NULL
       in:   a pointer to a BuildingType
   return:   a pointer to a BuildingType that has been initialized
*/
void initBuilding(BuildingType * building){
	// Initialize The Rooms List and Hunter Array
	RoomListType * rooms = (RoomListType*)calloc(1, sizeof(RoomListType));
	initRoomList(rooms);
	building -> rooms = rooms;
	// Set the ghost pointer to NULL for now
	building -> ghost = NULL;
	
	// Allocate memory to the hunter array, initialize it after
	HunterArrayType * hunters = (HunterArrayType*)calloc(1,sizeof(HunterArrayType));
	initHunterArray(hunters);
	hunters -> size = 4;
	building -> hunters = hunters;
	
	// Allocate memory to the evidence array, initialize it after
	EvidenceListType * evidence = (EvidenceListType*)calloc(1, sizeof(EvidenceListType));
	initEvidenceList(evidence);
	building -> evidence = evidence;
}

/*
  Function:  cleanupBuilding
  Purpose:   cleans the contained room list and hunter array inside the building
       in:   a pointer to a BuildingType
   return:   a pointer to a BuildingType with no elements within it's room list and hunter array
*/
void cleanupBuilding(BuildingType * building){
	// Clean up the Hunters Array
	cleanupHunterArray(building -> hunters);
 	
	// Clean up room list
	cleanupRoomList(building -> rooms);	

	// Clean up the evidence List	
	cleanupEvidenceList(building -> evidence);	
	
	// Clean up ghost data
	cleanupGhostData(building -> ghost);
	
	// Free the hunters from memory
	free(building -> hunters);
}

/*
  Function:  populateRooms
  Purpose:   fills the building with rooms
       in:   a pointer to a BuildingType
   return:   a pointer to a BuildingType that is populated with rooms
*/
void populateRooms(BuildingType* building) {
    RoomType* van;
    initRoom(&van, "Van");
    RoomType* hallway;
    initRoom(&hallway, "Hallway");
    RoomType* master_bedroom;
    initRoom(&master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom;
    initRoom(&boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom;
    initRoom(&bathroom, "Bathroom");
    RoomType* basement;
    initRoom(&basement, "Basement Staircase");
    RoomType* basement_hallway;
    initRoom(&basement_hallway, "Basement Hallway");
    RoomType* right_storage_room;
    initRoom(&right_storage_room, "Right Storage Room");
    RoomType* left_storage_room;
    initRoom(&left_storage_room, "Left Storage Room");
    RoomType* kitchen;
    initRoom(&kitchen, "Kitchen");
    RoomType* living_room;
    initRoom(&living_room, "Living Room");
    RoomType* garage;
    initRoom(&garage, "Garage");
    RoomType* utility_room;
    initRoom(&utility_room, "Utility Room");


    addRoom(building->rooms, van);
    addRoom(building->rooms, hallway);
    addRoom(building->rooms, master_bedroom);
    addRoom(building->rooms, boys_bedroom);
    addRoom(building->rooms, bathroom);
    addRoom(building->rooms, basement);
    addRoom(building->rooms, basement_hallway);
    addRoom(building->rooms, right_storage_room);
    addRoom(building->rooms, left_storage_room);
    addRoom(building->rooms, kitchen);
    addRoom(building->rooms, living_room);
    addRoom(building->rooms, garage);
    addRoom(building->rooms, utility_room);

    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}
