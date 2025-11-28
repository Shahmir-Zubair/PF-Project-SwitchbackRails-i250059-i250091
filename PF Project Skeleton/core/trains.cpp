#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <iostream>
#include <cstdlib>
using namespace std;
// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() 
{
    for(int i=0; i<TRAIN_COUNT; i++)
    {
        
        if(TRAINS[i][0] == TICK)
        {  
            cout<<"Available Spawn Points:"<<endl; 
            for(int j=0; j<SPAWN_COUNT; j++)
            {
                cout<<"("<<SPAWN_POINTS[j][0]<<", "<<SPAWN_POINTS[j][1]<<")"<<endl;
            }
            cout<<"Train "<<i<<" spawned at ("<<TRAINS[i][1]<<", "<<TRAINS[i][2]<<")"<<endl<<endl;
        }
    }
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition() {
    return true;
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection() {
    return 0;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing() {
    return 0;
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
    for(int i=0; i< TRAIN_COUNT; i++)
    {
        for(int j=0; j<DEST_COUNT; j++)
        {
            if(TRAINS[i][1] == DEST_POINTS[j][0] && TRAINS[i][2] == DEST_POINTS[j][1])
                cout<<"Train "<<i<<" arrived at destination point ("<<DEST_POINTS[j][0]<<", "<<DEST_POINTS[j][1]<<")"<<endl;
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
}
