#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // You may change this code; this is for demonstration purposes
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    return 0;
}

int hunterAction (hunterType * hunter) {

    while (C_TRUE) {
        int actionChoice;

        if (hunter -> room -> ghost != NULL) {
            hunter -> fear += FEAR_RATE;
        }


        if (hunter -> room -> hunters -> size != 1) {
            actionChoice = randInt(1,3);
        } else {
            actionChoice = randInt(1,2);
        }

        if (actionChoice == 1) {
            moveHunter(hunter);
        } else if (actionChoice == 2) {
            collectEvidence(hunter);
        } else if (actionChoice == 3) {
            shareEvidence(hunter);
        }


        if (hunter -> evidence -> size >= 3) {
            break;
        }
        if (hunter -> boredom <= 0) {
            break;            
        }
        if (hunter -> fear >= MAX_FEAR) {
            break;
        }

        sleep(1);
    }
}

int ghostAction (ghostType * ghost) {

    while (C_TRUE) {
        int actionChoice;

        if (ghost -> room -> hunters -> size == 0) {
            ghost -> boredom--;
        } else {
            ghost -> boredom = BOREDOM_MAX;
        }


        if (ghost -> room -> hunters -> size >= 1) {
            actionChoice = randInt(1,2);
        } else {
            actionChoice = randInt(1,3);
        }

        if (actionChoice == 1) {
            addGhostEvidence(ghost);
        } else if (actionChoice == 2) {
            pritnf("THE GHOST TWIDDLES IT'S THUMBS")
        } else if (actionChoice == 3) {
            moveGhost(ghost);
        }


        if (ghost -> boredom <= 0) {
            break;            
        }

        sleep(1);
    }
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
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