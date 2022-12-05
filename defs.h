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

// Building type struct
typedef struct {
	struct GhostType * ghost;
	struct RoomListType * rooms;
	struct HunterArrayType * hunters;
} BuildingType;


// Room list, nodes, and type structs
typedef struct {
	struct RoomNodeType * head;
	struct RoomNodeType * tail;
	int size;
} RoomListType;

typedef struct {
	struct RoomType * roomData;
	struct RoomNodeType * next;
} RoomNodeType;

typedef struct {
 	char name[MAX_STR];
 	struct GhostType * ghost;
 	struct EvidenceListType * evidence;
  	struct RoomListType * attached;
  	struct HunterArrayType * hunters;
} RoomType;
 
// Ghost type struct
typedef struct {
	GhostClassType ghostType;
	RoomType * room;
	int boredom;
} GhostType;


// Evidence list, node, and type structs
typedef struct {
	struct EvidenceNodeType * head;
	struct EvidenceNodeType * tail;
	int size;
} EvidenceListType;

typedef struct {
	struct EvidenceType * evidenceData;
	struct EvidenceNodeType * next;
} EvidenceNodeType;

typedef struct {
	int ghostliness;
	EvidenceClassType type;
	float value;
} EvidenceType;

// Hunter array and type structs
typedef struct {
	struct HunterType * hunters[MAX_HUNTERS];
	int size;
} HunterArrayType;

typedef struct {
	char name[MAX_STR];
	RoomType * room;
	EvidenceClassType evidenceType;
	struct EvidenceListType * evidence;
	int fear;
	int boredom;
    	int id;
} HunterType;

// Helper functions
int randInt(int, int);							// Generates a pseudorandom integer between the parameters
float randFloat(float, float);						// Generates a pseudorandom float between the parameters


// Building functions
void populateRooms(BuildingType *);					// Populates the building with sample data for rooms
void initBuilding(BuildingType *, struct GhostType *);				// Initiates a BuildingType 
void cleanupBuilding(BuildingType *);					// Frees all data associated with a building


// Room functions
void initRoomList(struct RoomListType *);					// Initializes a RoomListType 
void initRoom(char *, RoomType **);					// Initializes a RoomType 
void addRoom(RoomListType *, RoomType *);				// Adds a room to the end of a RoomListType
void connectRooms(RoomType *, RoomType *);				// Connects two RoomTypes
void cleanupRoom(RoomType *);						// Frees all data in a room
void cleanupRoomList(struct RoomListType *);					// Frees all nodes in a RoomList
RoomType * randRoom(struct RoomListType *, int);           // Gets a random room in a list

// Ghost functions
void initGhost(GhostClassType, RoomType *, GhostType **);		// Initializes a GhostType
void moveGhost(GhostType *);						// Moves a ghost to another room
void cleanupGhostData(GhostType *);					// Frees all data associated with a ghost
void spawnGhost(GhostType *, BuildingType *);                       // Adds the ghost to a random room that is not the van

// Evidence functions
void initEvidenceList(struct EvidenceListType *);				// Initializes an EvidenceListType
void initEvidence(EvidenceClassType, EvidenceType **);			// Initializes an EvidenceType
EvidenceType * getEvidenceAtIndex(EvidenceListType*, int);		// Returns the evidence at the index specified
void addEvidence(struct EvidenceListType *, EvidenceType *);			// Adds an EvidenceType to and EvidenceTypeList
void cleanupEvidenceData(struct EvidenceNodeType *);				// Frees all data associated with a piece of evidence
void cleanupEvidenceNode(struct EvidenceNodeType *);				// Frees an evidence node
void cleanupEvidenceList(struct EvidenceListType *);				// Frees all data from an evidence list


// Hunter functions
void initHunter(EvidenceClassType, char *, RoomType *, HunterType **, int);	// Initializes a HunterType
void initHunterArray(HunterArrayType *);
void collectEvidence(struct HunterType *);					// Collects all evidence from a room
void shareEvidence(struct HunterType *);					// Shares evidence with another hunter
int containsEvidenceType(EvidenceClassType , struct HunterType *);			// Checks if hunter's evidence array already has the evidence type we're adding
void transferEvidenceData(struct HunterType *, struct EvidenceType *);		// Transfers evidence data to the hunter 
void cleanupHunterData(struct HunterType *);					// Frees all data from a hunter
void cleanupHunterArray(struct HunterArrayType *);				// Frees all data in a hunter array
void moveHunter(HunterType *);                          // Moves the hunter to an adjacent room
void removeHunterFromRoom(HunterType *, RoomType *);
void addHunterToRoom(HunterType *, RoomType *);

// main threading control
void hunterAction (struct HunterType *);
void ghostAction (struct GhostType *);