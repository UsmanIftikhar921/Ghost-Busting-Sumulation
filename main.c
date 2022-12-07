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

       	initGhost(PHANTOM, ghostSpawnPoint, &ghost);

       	// Hunters
       	HunterType * hunter1, * hunter2, * hunter3, * hunter4;
       	RoomType * van = building.rooms -> head -> roomData;
       	initHunter(EMF, "Bill", van, &hunter1, 0);
       	initHunter(TEMPERATURE, "Bob", van, &hunter2, 1);
       	initHunter(FINGERPRINTS, "Bonzo", van, &hunter3, 2);
       	initHunter(SOUND, "Bib", van, &hunter4, 3);

       	HunterArrayType * vanHunters = van -> hunters;
       	vanHunters -> hunters[0] = hunter1;
       	vanHunters -> hunters[1] = hunter2;
       	vanHunters -> hunters[2] = hunter3;
       	vanHunters -> hunters[3] = hunter4;

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

    	while (C_TRUE) {
        	int actionChoice;

        	if (gameHunter -> room -> ghost != NULL) {
            		gameHunter -> fear += FEAR_RATE;
        	}

        	if (gameHunter -> room -> hunters -> size > 1) {
            		actionChoice = randInt(1,3);
        	} else {
            		actionChoice = randInt(1,2);
        	}

        	if (actionChoice == 1) {
            		printf("Hunter Move %s\n", gameHunter -> room -> name);
			if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
				// Check the rooms that are attached to the room the hunter is currently 	in:
				RoomListType * roomList = gameHunter -> room -> attached;
				RoomType * newRoom = randRoom(roomList, C_FALSE);
	
	
				if (sem_trywait(&newRoom -> mutex) == 0) {	
					// Remove hunter from linked list
					removeHunterFromRoom(gameHunter, gameHunter -> room);
		
					sem_post(&gameHunter -> room -> mutex);
			
					// Change the hunters's room pointer to a random room from that room's connected rooms
					gameHunter -> room = newRoom;
		
					// Add hunter to the linked list
					addHunterToRoom(gameHunter, gameHunter -> room);
				}
				sem_post(&gameHunter -> room -> mutex);
			}
        	} else if (actionChoice == 2) {
            		printf("Hunter Collect Evidence\n");
			if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
				sem_wait(&gameHunter -> evidence -> mutex);
				collectEvidence(gameHunter);
				sem_post(&gameHunter -> evidence -> mutex);
				sem_post(&gameHunter -> room -> mutex);
			}
        	} else if (actionChoice == 3) {
           	 	printf("Hunter Share Evidence\n");
			if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
				sem_wait(&gameHunter -> evidence -> mutex);
				// Note: Only transfer evidence that is not in standard values
				HunterArrayType * huntersInRoom = gameHunter -> room -> hunters;			// An array of hunters currently in the room
				int numOfHuntersInRoom = huntersInRoom -> size;					// The number of hunters currently in the room
				int numOfEvidence = gameHunter -> evidence -> size;					// The number of evidence in the hunter's evidence list
	
				// If there are at least two hunters in the room:
				if (numOfHuntersInRoom >= 2 && gameHunter -> evidence > 0){
					// Choose a second random hunter by index
					int otherHunterIndex = randInt(1,numOfHuntersInRoom);			// A randomly chosen index in the hunter's array
        				int count = 0;

					HunterType * currHunter;
		
					// In a for loop, transfer all evidence to the other hunter's evidence list
					for (int i = 0; i < numOfHuntersInRoom; i++){
						currHunter = huntersInRoom -> hunters[i];
						//If they are not the same hunter
						if (currHunter == NULL) {
							count++;
						} else if (currHunter -> id != gameHunter -> id){
							count++;
            						if (count == otherHunterIndex && otherHunterIndex <= numOfHuntersInRoom) {
            							HunterType * otherHunter = huntersInRoom -> hunters[i];
            							sem_wait(&otherHunter -> evidence -> mutex);
                						EvidenceNodeType * evidenceNode = gameHunter -> evidence -> head;
                						for (int i = 0; i < numOfEvidence; i++){
                    							transferEvidenceData(otherHunter, evidenceNode -> evidenceData);
                    							evidenceNode = evidenceNode -> next;
                						}
                						sem_post(&otherHunter -> evidence -> mutex);
                						break;
            						}
						}
					}
				}
				sem_post(&gameHunter -> evidence -> mutex);
				sem_post(&gameHunter -> room -> mutex);
			}

        	}


		if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
			int endCondition = C_FALSE;
        		if (gameHunter -> evidence -> size >= 3) {
        			printf("Your hunters have found out the ghost's type and won!\n");
				EvidenceNodeType * currNode = gameHunter -> evidence -> head;
		
				// Keep looping till you find a matching index
				while (currNode != NULL){
					if (currNode -> evidenceData -> type == EMF) {
						printf("EMF\n");
					}
					if (currNode -> evidenceData -> type == TEMPERATURE) {
						printf("TEMPERATURE\n");
					}
					if (currNode -> evidenceData -> type == FINGERPRINTS) {
						printf("FINGERPRINTS\n");
					}
					if (currNode -> evidenceData -> type == SOUND) {
						printf("SOUND\n");
					}
					currNode = currNode -> next;
				}
            			endCondition = C_TRUE;
            		}
      
        		if (gameHunter -> boredom <= 0) {
        			printf("Your hunter has gotten bored and exited the house!\n");
           			endCondition = C_TRUE;
            		}          
        
        		if (gameHunter -> fear >= MAX_FEAR) {
        			printf("Your hunter has gotten too scared and run from the house!\n");
            			endCondition = C_TRUE;
            		}  
            		if (endCondition == C_TRUE){
				gameHunter -> room -> hunters -> hunters[gameHunter -> id] = NULL;
				gameHunter -> room -> hunters -> size--;
            			sem_post(&gameHunter -> room -> mutex);  
            			break; 
            		} else {
            			sem_post(&gameHunter -> room -> mutex);
            		}
        	}

        	sleep(1);
    	}
    	return 0;
}

void * ghostAction (void * ghost) {
	GhostType * gameGhost = (GhostType*)ghost;
	
    	while (C_TRUE) {
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
            		printf("Ghost Haunt %d\n", gameGhost -> room -> hunters -> size);
			if(sem_trywait(&gameGhost -> room -> mutex) == 0) {
				addGhostEvidence(gameGhost);
				sem_post(&gameGhost -> room -> mutex);
			}
        	} else if (actionChoice == 2) {
            		printf("Ghost Wait\n");
        	} else if (actionChoice == 3) {
               		printf("Ghost Move %s\n", gameGhost -> room -> name);
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
        			
        			sem_post(&gameGhost -> room -> mutex);
        		}
        	} else {
        		printf("Error\n");
        	}


        	if (gameGhost -> boredom <= 0) {
        		printf("The ghost has gotten bored and exited the house!\n");
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