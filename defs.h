#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define C_FALSE			0
#define C_TRUE			1
#define NORMAL			0
#define GHOSTLY			1
#define MAX_STR			64
#define FEAR_RATE		1
#define MAX_FEAR		100
#define MAX_HUNTERS		4
#define USLEEP_TIME		50000
#define BOREDOM_MAX		99

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

// Hunder list
typedef struct HunterArrayType {
	struct HunterType * hunters[MAX_HUNTERS];
	int size;
} HunterArrayType;


// Evidence list
typedef struct EvidenceListType {
	struct EvidenceNodeType * head;
	struct EvidenceNodeType * tail;
	int size;
} EvidenceListType;

// Room list, nodes, and type structs
typedef struct RoomListType {
	struct RoomNodeType * head;
	struct RoomNodeType * tail;
	int size;
} RoomListType;

// Evidence node, and type structs
typedef struct EvidenceNodeType {
	struct EvidenceType * evidenceData;
	struct EvidenceNodeType * next;
} EvidenceNodeType;

typedef struct RoomNodeType {
	struct RoomType * roomData;
	struct RoomNodeType * next;
} RoomNodeType;

typedef struct RoomType {
 	char name[MAX_STR];
 	struct GhostType * ghost;
 	EvidenceListType * evidence;
  	RoomListType * attached;
  	HunterArrayType * hunters;
} RoomType;
 
// Ghost type struct
typedef struct GhostType {
	GhostClassType ghostType;
	RoomType * room;
	int boredom;
} GhostType;

typedef struct EvidenceType {
	int ghostliness;
	EvidenceClassType type;
	float value;
} EvidenceType;

// Hunter type
typedef struct HunterType {
	char name[MAX_STR];
	RoomType * room;
	EvidenceClassType evidenceType;
	EvidenceListType * evidence;
	int fear;
	int boredom;
    	int id;
} HunterType;

// Building type struct
typedef struct BuildingType {
	GhostType * ghost;
	RoomListType * rooms;
	HunterArrayType * hunters;
} BuildingType;


// Helper functions
int randInt(int, int);							// Generates a pseudorandom integer between the parameters
float randFloat(float, float);						// Generates a pseudorandom float between the parameters


// Building functions
void populateRooms(BuildingType *);					// Populates the building with sample data for rooms
void initBuilding(BuildingType *, struct GhostType *);				// Initiates a BuildingType 
void cleanupBuilding(BuildingType *);					// Frees all data associated with a building


// Room functions
void initRoomList(RoomListType *);					// Initializes a RoomListType 
void initRoom(char *, RoomType **);					// Initializes a RoomType 
void addRoom(RoomListType *, RoomType *);				// Adds a room to the end of a RoomListType
void connectRooms(RoomType *, RoomType *);				// Connects two RoomTypes
void cleanupRoom(RoomType *);						// Frees all data in a room
void cleanupRoomList(RoomListType *);					// Frees all nodes in a RoomList
RoomType * randRoom(RoomListType *, int);           // Gets a random room in a list

// Ghost functions
void initGhost(GhostClassType, RoomType *, GhostType **);		// Initializes a GhostType
void moveGhost(GhostType *);						// Moves a ghost to another room
void cleanupGhostData(GhostType *);					// Frees all data associated with a ghost
void spawnGhost(GhostType *, BuildingType *);                       // Adds the ghost to a random room that is not the van

// Evidence functions
void initEvidenceList(EvidenceListType *);				// Initializes an EvidenceListType
void initEvidence(EvidenceClassType, EvidenceType **);			// Initializes an EvidenceType
EvidenceType * getEvidenceAtIndex(EvidenceListType*, int);		// Returns the evidence at the index specified
void addEvidence(EvidenceListType *, EvidenceType *);			// Adds an EvidenceType to and EvidenceTypeList
void cleanupEvidenceData(struct EvidenceNodeType *);				// Frees all data associated with a piece of evidence
void cleanupEvidenceNode(struct EvidenceNodeType *);				// Frees an evidence node
void cleanupEvidenceList(EvidenceListType *);				// Frees all data from an evidence list


// Hunter functions
void initHunter(EvidenceClassType, char *, RoomType *, HunterType **, int);	// Initializes a HunterType
void initHunterArray(HunterArrayType *);
void collectEvidence(HunterType *);					// Collects all evidence from a room
void shareEvidence(HunterType *);					// Shares evidence with another hunter
int containsEvidenceType(EvidenceClassType , struct HunterType *);			// Checks if hunter's evidence array already has the evidence type we're adding
void transferEvidenceData(HunterType *, EvidenceType *);		// Transfers evidence data to the hunter 
void cleanupHunterData(HunterType *);					// Frees all data from a hunter
void cleanupHunterArray(HunterArrayType *);				// Frees all data in a hunter array
void moveHunter(HunterType *);                          // Moves the hunter to an adjacent room
void removeHunterFromRoom(HunterType *, RoomType *);
void addHunterToRoom(HunterType *, RoomType *);

// main threading control
void hunterAction (struct HunterType *);
void ghostAction (struct GhostType *);