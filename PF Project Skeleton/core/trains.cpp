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
            SPAWNED_TRAINS++;
        }
    }
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int train_i) 
{
    int dir = getNextDirection(train_i);

    if(dir == DIR_NONE)
        return false;

    int x = TRAINS[train_i][1];
    int y = TRAINS[train_i][2];

    if(dir == DIR_UP)    
        y--;
    if(dir == DIR_RIGHT)
        x++;
    if(dir == DIR_DOWN)  
        y++;
    if(dir == DIR_LEFT)  
        y++;

    // bounds
    if(isInBounds(x, y))
        return false;

    // store next position
    TRAINS[train_i][1] = x;
    TRAINS[train_i][2] = y;

    return true;
}


// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int train_i) {
    int train_x = TRAINS[train_i][1];
    int train_y = TRAINS[train_i][2];

    int curr_direction = TRAINS[train_i][3]; // Later update with switches

    // TRAINS store positions as [x, y] where x is column and y is row.
    // GRID is indexed as GRID[row][col], so use [train_y][train_x].
    char curr_tile = GRID[train_y][train_x];
    if(curr_tile == 'S')
        return TRAINS[train_i][3];

    else if(curr_tile == '-' || curr_tile == '=')
        return curr_direction;

    else if(curr_tile == '|')
        return curr_direction;

    else if(curr_tile == '+')
        return curr_direction;
        // return getSmartDirectionAtCrossing();

    else if(isSwitchTile(train_y, train_x))
        return curr_direction;
        // return getSwitchStateForDirection();

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
    for(int i=0; i<SPAWNED_TRAINS; i++)
    {
        determineNextPosition(i);
    }
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
