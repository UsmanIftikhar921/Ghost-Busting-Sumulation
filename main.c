#include "defs.h"

int main(int argc, char *argv[])
{
     	// Initialize a random seed for the random number generators
       	srand(time(NULL));
	
       	BuildingType building;
       	initBuilding(&building);
       	populateRooms(&building);

       	// Ghost
       	GhostType * ghost;
       	RoomType * ghostSpawnPoint = randRoom(building.rooms, C_TRUE);

       	initGhost(BANSHEE, ghostSpawnPoint, &ghost);


       	// Hunters
       	HunterType * hunter1, * hunter2, * hunter3, * hunter4;
       	initHunter(EMF, "Bill", building.rooms -> head -> roomData, &hunter1, 1);
       	initHunter(TEMPERATURE, "Bob", building.rooms -> head -> roomData, &hunter2, 2);
       	initHunter(FINGERPRINTS, "Bonzo", building.rooms -> head -> roomData, &hunter3, 3);
       	initHunter(SOUND, "Bib", building.rooms -> head -> roomData, &hunter4, 4);

	pthread_t  ht1, ht2, ht3, ht4, gt1;

	pthread_create(&ht1, NULL, hunterAction, hunter1);
	pthread_create(&ht2, NULL, hunterAction, hunter2);
	pthread_create(&ht3, NULL, hunterAction, hunter3);
	pthread_create(&ht4, NULL, hunterAction, hunter4);
	pthread_create(&gt1, NULL, ghostAction, ghost);

	pthread_join(ht1, NULL);
	printf("\nh1 is back\n");
	pthread_join(ht2, NULL);
	printf("h2 is back\n");
	pthread_join(ht3, NULL);
	printf("h3 is back\n");
	pthread_join(ht4, NULL);
	printf("h4 is back\n");
	pthread_join(gt1, NULL);
	printf("gt1 is back\n");


    	return 0;
}

void * hunterAction (void * hunter) {

	HunterType * gameHunter = (HunterType*)hunter;

    	for (int i = 0; i < 100; i++) {
        	int actionChoice;

        	if (gameHunter -> room -> ghost != NULL) {
            		gameHunter -> fear += FEAR_RATE;
        	}


        	if (gameHunter -> room -> hunters -> size != 1) {
            		actionChoice = randInt(1,2);
        	} else {
            		actionChoice = randInt(1,2);
        	}

        	if (actionChoice == 1) {
			if (sem_trywait(&gameHunter -> room -> mutex) == 0) {

				// Check the rooms that are attached to the room the hunter is currently 	in:
				RoomListType * roomList = gameHunter -> room -> attached;
				RoomType * newRoom = randRoom(roomList, C_FALSE);
	
	
				if (sem_trywait(&newRoom -> mutex) == 0) {	
					// Decrease the number of hunters in the current room
					gameHunter -> room -> hunters -> size--;
		
			
					// Remove hunter from linked list
					removeHunterFromRoom(gameHunter, gameHunter -> room);
		
					sem_post(&gameHunter -> room -> mutex);
			
					// Change the hunters's room pointer to a random room from that room's connected rooms
					gameHunter -> room = newRoom;
		
					// Add hunter to the linked list
					addHunterToRoom(gameHunter, gameHunter -> room);
			
					// Increase the number of hunters in the current room
					gameHunter -> room -> hunters -> size++;
				}
				sem_post(&gameHunter -> room -> mutex);
			
			}
            		
            		
            		
            		
            		//moveHunter(gameHunter);
            		printf("Hunter Move %s\n", gameHunter -> room -> name);
			
        	} else if (actionChoice == 2) {
			sem_wait(&gameHunter -> room -> mutex);
            		collectEvidence(gameHunter);
            		printf("Hunter Collect Evidence\n");
			sem_post(&gameHunter -> room -> mutex);
        	} else if (actionChoice == 3) {
			sem_wait(&gameHunter -> room -> mutex);
           	 	shareEvidence(gameHunter);
           	 	printf("Hunter Share Evidence\n");
			sem_post(&gameHunter -> room -> mutex);
        	}


        	if (gameHunter -> evidence -> size >= 3) {
        		printf("EvidenceLimit");
            		break;
        	}
        	if (gameHunter -> boredom <= 0) {
        		printf("BoredomLimit");
            		break;            
        	}
        	if (gameHunter -> fear >= MAX_FEAR) {
            		printf("FearLimit");
            		break;
        	}

        	sleep(1);
    	}
    	return 0;
}

void * ghostAction (void * ghost) {
	GhostType * gameGhost = (GhostType*)ghost;
    	for (int i = 0; i < 100; i++) {
        	int actionChoice;
	
        	if (gameGhost -> room -> hunters -> size == 0) {
        		gameGhost -> boredom--;
        	} else {
            		gameGhost -> boredom = BOREDOM_MAX;
        	}


        	if (gameGhost -> room -> hunters -> size >= 1) {
            		actionChoice = randInt(1,2);
        	} else {
	            	actionChoice = randInt(1,3);
        	}

        	if (actionChoice == 1) {
			sem_wait(&gameGhost -> room -> mutex);
            		addGhostEvidence(gameGhost);
            		printf("Ghost Haunt\n");
			sem_post(&gameGhost -> room -> mutex);
        	} else if (actionChoice == 2) {
            		printf("Ghost Wait\n");
        	} else if (actionChoice == 3) {
        		if (sem_trywait(&gameGhost -> room -> mutex) == 0) {
        			
				// Check the rooms that are attached to the room the ghost is currently in:
				RoomListType * roomList = gameGhost -> room -> attached;
				RoomType * newRoom = randRoom(roomList, C_FALSE);
	
				if (sem_trywait(&newRoom -> mutex) == 0) {
		
					gameGhost -> room -> ghost = NULL;
					sem_post(&gameGhost -> room -> mutex);
					// Change the ghost's room pointer to a random room from that room's connected rooms
					gameGhost -> room = newRoom;
					gameGhost -> room -> ghost = gameGhost;
		
		}	
        			
        			//moveGhost(gameGhost);
        			sem_post(&gameGhost -> room -> mutex);
        		}
               		printf("Ghost Move %s\n", gameGhost -> room -> name);
        	} else {
        		printf("Error\n");
        	}


        	if (gameGhost -> boredom <= 0) {
            		break;            
        	}

        	sleep(1);
    	}
    	return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max) {
 	
    	return rand() % (max - min + 1) + min;
 	//srand(time(0));
	
 	//int val = (rand() % (max - min + 1)) + min;
	
 	//return val;
 }

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}