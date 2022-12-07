#include "defs.h"

int main(int argc, char *argv[])
{
     	// Initialize a random seed for the random number generators
       	srand(time(NULL));
	
       	BuildingType * building = (BuildingType*)calloc(1, sizeof(BuildingType));
       	initBuilding(building);
       	populateRooms(building);

       	// Ghost
       	GhostType * ghost;
       	RoomType * ghostSpawnPoint = randRoom(building -> rooms, C_TRUE);

	GhostClassType ghostClass;
	int ghostSelector = randInt(1, 4);
	if (ghostSelector == 1) {
		ghostClass = POLTERGEIST;
	} else if ( ghostSelector == 2) {
		ghostClass = BANSHEE;
	} else if (ghostSelector == 3) {
		ghostClass = BULLIES;
	} else if (ghostSelector == 4) {
		ghostClass = PHANTOM;
	}
	
       	initGhost(ghostClass, ghostSpawnPoint, &ghost, building);

	char hunter1Name[MAX_STR] = {'\0'};;
	char hunter2Name[MAX_STR] = {'\0'};;
	char hunter3Name[MAX_STR] = {'\0'};;
	char hunter4Name[MAX_STR] = {'\0'};;

	getHunterNames(hunter1Name, hunter2Name, hunter3Name, hunter4Name);
	
       	// Hunters
       	HunterType * hunter1, * hunter2, * hunter3, * hunter4;
       	RoomType * van = building -> rooms -> head -> roomData;
       	initHunter(EMF, hunter1Name, van, &hunter1, 0);
       	initHunter(TEMPERATURE, hunter2Name, van, &hunter2, 1);
       	initHunter(FINGERPRINTS, hunter3Name, van, &hunter3, 2);
       	initHunter(SOUND, hunter4Name, van, &hunter4, 3);



       	HunterArrayType * vanHunters = van -> hunters;
       	vanHunters -> hunters[0] = hunter1; building -> hunters -> hunters[0] = hunter1;
       	vanHunters -> hunters[1] = hunter2; building -> hunters -> hunters[1] = hunter2;
       	vanHunters -> hunters[2] = hunter3; building -> hunters -> hunters[2] = hunter3;
       	vanHunters -> hunters[3] = hunter4; building -> hunters -> hunters[3] = hunter4;

	pthread_t ht1, ht2, ht3, ht4, gt1;

	printf("The mystery crew pull up to the haunted house. Time for some ghostbusting!\n\n");

	pthread_create(&ht1, NULL, hunterAction, hunter1);
	pthread_create(&ht2, NULL, hunterAction, hunter2);
	pthread_create(&ht3, NULL, hunterAction, hunter3);
	pthread_create(&ht4, NULL, hunterAction, hunter4);
	pthread_create(&gt1, NULL, ghostAction, ghost);

	pthread_join(ht1, NULL);
	pthread_join(ht2, NULL);
	pthread_join(ht3, NULL);
	pthread_join(ht4, NULL);
	pthread_join(gt1, NULL);
	
	if ((hunter1 -> fear >= MAX_FEAR || hunter1 -> boredom <= 0) && (hunter2 -> fear >= MAX_FEAR || hunter2 -> boredom <= 0) && (hunter3 -> fear >= MAX_FEAR || hunter3 -> boredom <= 0) && (hunter4 -> fear >= MAX_FEAR || hunter4 -> boredom <= 0)) {
		printf("All the hunters have gotten bored or been frightened off by the ghost! The ghost wins!\n");
	} else {
		printf("Your hunters have found out the ghost's type and won!\n");
		if (hunter1 -> evidence -> size == 3) {
			typeCalculator(hunter1 -> evidence);
		} else if (hunter2 -> evidence -> size == 3) {
			typeCalculator(hunter2 -> evidence);
		} else if (hunter3 -> evidence -> size == 3) {
			typeCalculator(hunter3 -> evidence);
		} else if (hunter4 -> evidence -> size == 3) {
			typeCalculator(hunter4 -> evidence);
		}	
	}
	
	cleanupBuilding(building);
	free(building);
    	return 0;
}

void getHunterNames(char * hunter1Name, char * hunter2Name, char * hunter3Name, char * hunter4Name) {
	printf("Enter hunter 1's name: ");
	fgets(hunter1Name, MAX_STR, stdin);
	while (getchar() != '\n');
	hunter1Name[strcspn(hunter1Name, "\n")] = 0;
	
	printf("Enter hunter 2's name: ");
	fgets(hunter2Name, MAX_STR, stdin);
	while (getchar() != '\n');
	hunter2Name[strcspn(hunter2Name, "\n")] = 0;
	
	printf("Enter hunter 3's name: ");
	fgets(hunter3Name, MAX_STR, stdin);
	while (getchar() != '\n');
	hunter3Name[strcspn(hunter3Name, "\n")] = 0;
	
	printf("Enter hunter 4's name: ");
	fgets(hunter4Name, MAX_STR, stdin);
	while (getchar() != '\n');
	hunter4Name[strcspn(hunter4Name, "\n")] = 0;
}

void typeCalculator (EvidenceListType * evidenceList) {

	EvidenceNodeType * currNode = evidenceList -> head;
		
	int poltergeist = 0, banshee = 0, bullies = 0, phantom = 0;
	char * ghostType;

	// Keep looping till you find a matching index
	while (currNode != NULL){
		if (currNode -> evidenceData -> type == EMF) {
			poltergeist++; banshee++; bullies++;
		}
		if (currNode -> evidenceData -> type == TEMPERATURE) {
			poltergeist++; banshee++; phantom++;
		}
		if (currNode -> evidenceData -> type == FINGERPRINTS) {
			poltergeist++; bullies++; phantom++;
		}
		if (currNode -> evidenceData -> type == SOUND) {
			banshee++; bullies++; phantom++;
		}
		currNode = currNode -> next;
	}
	if (poltergeist >= 3) ghostType = "Poltergeist";
	if (banshee >= 3) ghostType = "Banshee";
	if (bullies >= 3) ghostType = "Bullies";
	if (phantom >= 3) ghostType = "Phantom";
	
	printf("With careful teamwork and great detective skills, the hunters discovered that the ghost was really a %s!\n", ghostType);
	
}

void * hunterAction (void * hunter) {

	HunterType * gameHunter = (HunterType*)hunter;

    	while (C_TRUE) {
        	int actionChoice;

        	if (gameHunter -> room -> ghost != NULL) {
            		gameHunter -> fear += FEAR_RATE;
            		gameHunter -> boredom = BOREDOM_MAX;
        	} else {
        		gameHunter -> boredom -= BOREDOM_RATE;
        	}

        	if (gameHunter -> room -> hunters -> size > 1) {
            		actionChoice = randInt(1,3);
        	} else {
            		actionChoice = randInt(1,2);
        	}

        	if (actionChoice == 1) {
			if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
				// Check the rooms that are attached to the room the hunter is currently 	in:
				RoomListType * roomList = gameHunter -> room -> attached;
				RoomType * newRoom = randRoom(roomList, C_FALSE);
	
	
				if (sem_trywait(&newRoom -> mutex) == 0) {	
					// Remove hunter from linked list
					removeHunterFromRoom(gameHunter, gameHunter -> room);
		
					sem_post(&gameHunter -> room -> mutex);
					
					printf("%s leaves the %s and moves into the %s.\n", gameHunter -> name, gameHunter -> room -> name, newRoom -> name);
					
					// Change the hunters's room pointer to a random room from that room's connected rooms
					gameHunter -> room = newRoom;

					// Add hunter to the linked list
					addHunterToRoom(gameHunter, gameHunter -> room);
				} else {
					printf("%s starts heading for the %s, but his attention is drawn back to the %s.\n", gameHunter -> name, newRoom -> name, gameHunter -> room -> name);
				}
				sem_post(&gameHunter -> room -> mutex);
			} else {
				printf("%s dreams of a world of peace.\n", gameHunter -> name);
			}
        	} else if (actionChoice == 2) {
			if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
				sem_wait(&gameHunter -> evidence -> mutex);
				collectEvidence(gameHunter);
				sem_post(&gameHunter -> evidence -> mutex);
				sem_post(&gameHunter -> room -> mutex);
			} else {
				printf("%s's readings get drowned out by static.\n", gameHunter -> name);
			}
        	} else if (actionChoice == 3) {
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
								printf("%s tells %s about some exciting evidence that they found.\n", gameHunter -> name, otherHunter -> name);
                						sem_post(&otherHunter -> evidence -> mutex);
                						break;
            						} 
						}
					}
				} else {
					printf("%s wishes they could share evidence.\n", gameHunter -> name);
				}
				sem_post(&gameHunter -> evidence -> mutex);
				sem_post(&gameHunter -> room -> mutex);
			} else {
				printf("%s is overcome by social anxiety and doesn't want to talk to anyone.\n", gameHunter -> name);
			}
        	}


		if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
			int endCondition = C_FALSE;
        		if (gameHunter -> evidence -> size >= 3) {
        			printf("%s has found out the ghost's type and won!\n", gameHunter -> name);
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
        			printf("\n\n%s has gotten bored and exited the house!\n\n\n", gameHunter -> name);
           			endCondition = C_TRUE;
            		}          
        
        		if (gameHunter -> fear >= MAX_FEAR) {
        			printf("\n\n%s has gotten too scared and run from the house!\n\n\n", gameHunter -> name);
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
        		gameGhost -> boredom -= BOREDOM_RATE;
        	} else {
            		gameGhost -> boredom = BOREDOM_MAX;
        	}


        	if (gameGhost -> room -> hunters -> size >= 1) {
            		actionChoice = randInt(1,2);
        	} else {
	            	actionChoice = randInt(1,3);
        	}

        	if (actionChoice == 1) {
			if(sem_trywait(&gameGhost -> room -> mutex) == 0) {
				addGhostEvidence(gameGhost);
				sem_post(&gameGhost -> room -> mutex);
			} else {
				printf("An ominous presence looms over the %s.\n", gameGhost -> room -> name);
			}
        	} else if (actionChoice == 2) {
            		printf("The ghost twiddles its thumbs.\n");
        	} else if (actionChoice == 3) {
        		if (sem_trywait(&gameGhost -> room -> mutex) == 0) {
        			
				// Check the rooms that are attached to the room the ghost is currently in:
				RoomListType * roomList = gameGhost -> room -> attached;
				RoomType * newRoom = randRoom(roomList, C_FALSE);
	
				if (sem_trywait(&newRoom -> mutex) == 0) {
		
					gameGhost -> room -> ghost = NULL;
					sem_post(&gameGhost -> room -> mutex);
					printf("The ghost dissaparates from the %s and appears in the %s.\n", gameGhost -> room -> name, newRoom -> name);
					// Change the ghost's room pointer to a random room from that room's connected rooms
					gameGhost -> room = newRoom;
					gameGhost -> room -> ghost = gameGhost;
		
				} else {
					printf("The ghost peeks through the wall of the %s andinto the %s.\n", gameGhost -> room -> name, newRoom -> name);
				}
        			
        			sem_post(&gameGhost -> room -> mutex);
        		} else {
				printf("The ghost contemplates leaving the %s, but decides that it's comfy where it is.\n", gameGhost -> room -> name);
        		}
        	} else {
        		printf("Error\n");
        	}


        	if (gameGhost -> boredom <= 0) {
        		printf("\n\nThe ghost has gotten bored and exited the house!\n\n\n");
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