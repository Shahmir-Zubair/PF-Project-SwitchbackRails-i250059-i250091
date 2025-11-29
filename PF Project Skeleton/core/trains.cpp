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

// Return the manhattan distance between two points
int manhattan(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    if (dx < 0) 
        dx = -dx;

    int dy = y1 - y2;
    if (dy < 0) 
        dy = -dy;

    return dx + dy;
}

int getClosestDestinationManhattan(int x, int y)
{
    int shortest = 999999;

    for(int d=0; d<DEST_COUNT; d++) {
        int dx = DEST_POINTS[d][0];
        int dy = DEST_POINTS[d][1];

        int dist = manhattan(x, y, dx, dy);

        if(dist < shortest)
            shortest = dist;
    }
    return shortest;
}

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
    next_dir[train_i] = getNextDirection(train_i);
    int dir = next_dir[train_i]
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
        x--;

    // bounds
    if(!isInBounds(x, y))
        return false;

    // store next position
    next_x[train_i] = x;
    next_y[train_i] = y;

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
        return getSmartDirectionAtCrossing();

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
int getSmartDirectionAtCrossing(int train_i) {
    int x = TRAINS[train_i][1];
    int y = TRAINS[train_i][2];

    int best_dir = DIR_NONE;
    int best_dist = 999999;

    // Try UP
    if(isInBounds(x, y-1) && isTrackTile(GRID[y-1][x])) {
        int dist = getClosestDestinationManhattan(x, y-1);
        if(dist < best_dist) {
            best_dist = dist;
            best_dir = DIR_UP;
        }
    }

    // Try RIGHT
    if(isInBounds(x+1, y) && isTrackTile(GRID[y][x+1])) {
        int dist = getClosestDestinationManhattan(x+1, y);
        if(dist < best_dist) {
            best_dist = dist;
            best_dir = DIR_RIGHT;
        }
    }

    // Try DOWN
    if(isInBounds(x, y+1) && isTrackTile(GRID[y+1][x])) {
        int dist = getClosestDestinationManhattan(x, y+1);
        if(dist < best_dist) {
            best_dist = dist;
            best_dir = DIR_DOWN;
        }
    }

    // Try LEFT
    if(isInBounds(x-1, y) && isTrackTile(GRID[y][x-1])) {
        int dist = getClosestDestinationManhattan(x-1, y);
        if(dist < best_dist) {
            best_dist = dist;
            best_dir = DIR_LEFT;
        }
    }

    return best_dir;
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
void moveAllTrains()
{
    for(int i=0; i<SPAWNED_TRAINS; i++)
    {
        if(TRAIN_CRASHED[i])
            continue;

        if(TRAIN_CAN_MOVE[i])
        {
            TRAINS[i][1] = next_x[i];
            TRAINS[i][2] = next_y[i];
        }
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions()
{
    // Mark all trains as initially allowed to move
    for(int i=0; i<SPAWNED_TRAINS; i++)
        TRAIN_CAN_MOVE[i] = true;

    // ------------------------------------------------------------
    // 1. SAME-TILE COLLISIONS
    // ------------------------------------------------------------
    for(int t1=0; t1<SPAWNED_TRAINS; t1++)
    {
        for(int t2=t1+1; t2<SPAWNED_TRAINS; t2++)
        {
            if(next_x[t1] == next_x[t2] && next_y[t1] == next_y[t2])
            {
                int d1 = getClosestDestinationManhattan(TRAINS[t1][1], TRAINS[t1][2]);
                int d2 = getClosestDestinationManhattan(TRAINS[t2][1], TRAINS[t2][2]);

                if(d1 > d2) {
                    TRAIN_CAN_MOVE[t2] = false;
                }
                else if(d2 > d1) {
                    TRAIN_CAN_MOVE[t1] = false;
                }
                else {
                    // Equal → crash both
                    TRAIN_CRASHED[t1] = true;
                    TRAIN_CRASHED[t2] = true;
                }
            }
        }
    }

    // ------------------------------------------------------------
    // 2. HEAD-ON SWAP COLLISIONS (A→B and B→A)
    // ------------------------------------------------------------
    for(int t1=0; t1<SPAWNED_TRAINS; t1++)
    {
        for(int t2=t1+1; t2<SPAWNED_TRAINS; t2++)
        {
            int x1 = TRAINS[t1][1], y1 = TRAINS[t1][2];
            int x2 = TRAINS[t2][1], y2 = TRAINS[t2][2];

            // Swap if next positions cross
            if(next_x[t1] == x2 && next_y[t1] == y2 &&
               next_x[t2] == x1 && next_y[t2] == y1)
            {
                int d1 = getClosestDestinationManhattan(x1, y1);
                int d2 = getClosestDestinationManhattan(x2, y2);

                if(d1 > d2) {
                    TRAIN_CAN_MOVE[t2] = false;
                }
                else if(d2 > d1) {
                    TRAIN_CAN_MOVE[t1] = false;
                }
                else {
                    TRAIN_CRASHED[t1] = true;
                    TRAIN_CRASHED[t2] = true;
                }
            }
        }
    }

    // ------------------------------------------------------------
    // 3. CROSSING TILE ('+') COLLISIONS
    // ------------------------------------------------------------
    for(int t1=0; t1<SPAWNED_TRAINS; t1++)
    {
        // Check if target is a crossing
        int nx = next_x[t1];
        int ny = next_y[t1];

        if(GRID[ny][nx] == '+')
        {
            // Compare with all others entering same crossing
            for(int t2=t1+1; t2<SPAWNED_TRAINS; t2++)
            {
                if(next_x[t2] == nx && next_y[t2] == ny)
                {
                    int d1 = getClosestDestinationManhattan(TRAINS[t1][1], TRAINS[t1][2]);
                    int d2 = getClosestDestinationManhattan(TRAINS[t2][1], TRAINS[t2][2]);

                    if(d1 > d2) {
                        TRAIN_CAN_MOVE[t2] = false;
                    }
                    else if(d2 > d1) {
                        TRAIN_CAN_MOVE[t1] = false;
                    }
                    else {
                        TRAIN_CRASHED[t1] = true;
                        TRAIN_CRASHED[t2] = true;
                    }
                }
            }
        }
    }

    // ------------------------------------------------------------
    // 4. FINALIZE MOVES: Prevent crashed trains from moving
    // ------------------------------------------------------------
    for(int i=0; i<SPAWNED_TRAINS; i++)
    {
        if(TRAIN_CRASHED[i])
            TRAIN_CAN_MOVE[i] = false;  // crashed → no movement
    }
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
