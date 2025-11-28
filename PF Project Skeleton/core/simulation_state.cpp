#include "simulation_state.h"
#include "grid.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================



// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int ROWS, COLS;
char** GRID = NULL;
// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
int** TRAINS = NULL;
int TRAIN_COUNT = 0;
const int DIR_UP = 0;
const int DIR_RIGHT = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;

// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
int SWITCH_COUNT = 0;
char* LETTER = NULL;
string* MODE = NULL;
int* INIT = NULL;
int** K_VALUES = NULL;
string* STATE0 = NULL;
string* STATE1 = NULL;
// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
int SPAWN_COUNT = 0;
int DEST_COUNT = 0;
int** SPAWN_POINTS = NULL;
int** DEST_POINTS = NULL;

// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int TICK;
int SPAWNED_TRAINS=0;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
int SEED;
string WEATHER, LVL_NAME;
// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void allocateGrid() 
{
    GRID = new char*[ROWS];
    for (int i = 0; i < ROWS; i++) 
    {
        GRID[i] = new char[COLS];
        // Initialize all cells to space
        for (int j = 0; j < COLS; j++) 
            GRID[i][j] = ' ';
    }

}
void allocateSwitchesTrains() 
{
    TRAINS = new int*[TRAIN_COUNT];
    for(int i = 0; i < TRAIN_COUNT; i++) 
    {
        TRAINS[i] = new int[5]; // tick, x, y, dir, color
    }
    LETTER = new char[SWITCH_COUNT];
    MODE = new string[SWITCH_COUNT];
    INIT = new int[SWITCH_COUNT];
    K_VALUES = new int*[SWITCH_COUNT];
    for(int i = 0; i < SWITCH_COUNT; i++) 
    {
        K_VALUES[i] = new int[4]; // up, right, down, left
    }
    STATE0 = new string[SWITCH_COUNT];
    STATE1 = new string[SWITCH_COUNT];
}
void initializeSpawnDest()
{

    SPAWN_POINTS = new int*[SPAWN_COUNT];
    for(int i = 0; i<SPAWN_COUNT; i++)
        SPAWN_POINTS[i] = new int[2]; // r and c

    DEST_POINTS = new int*[DEST_COUNT];
    for(int i = 0; i<DEST_COUNT; i++)
        DEST_POINTS[i] = new int[2]; // r and c

    int spawns = 0;
    int dests = 0;
    for(int r=0; r<ROWS; r++)
    {
        for(int c=0; c<COLS; c++)
        {
            if(isSpawnPoint(r, c))
            {
                SPAWN_POINTS[spawns][0] = c;
                SPAWN_POINTS[spawns][1] = r + 1;
                spawns++;
            }    
            if(isDestinationPoint(r, c))
            {
                DEST_POINTS[dests][0] = c;
                DEST_POINTS[dests][1] = r + 1;
                dests++;
            }      
        }
    }

}
int initializeSimulationState()
{
    // Free old allocations if they exist
    if(GRID) 
    {
        for (int i = 0; i < ROWS; i++) 
            delete[] GRID[i];

        delete[] GRID;
    }
    GRID = nullptr;

    if (TRAINS) 
    {
        for (int i = 0; i < TRAIN_COUNT; i++) 
            delete[] TRAINS[i];

        delete[] TRAINS;
    }
    TRAINS = nullptr;

    if(SPAWN_POINTS)
    {
        for(int i=0; i<SPAWN_COUNT; i++)
            delete[] SPAWN_POINTS[i];

        delete[] SPAWN_POINTS;
    }
    SPAWN_POINTS = nullptr;
    
    if(DEST_POINTS)
    {
        for(int i=0; i<DEST_COUNT; i++)
            delete[] DEST_POINTS[i];

        delete[] DEST_POINTS;
    }
    DEST_POINTS = nullptr;

    delete[] LETTER;   
    LETTER = nullptr;

    delete[] MODE;     
    MODE = nullptr;

    delete[] INIT;     
    INIT = nullptr;

    if (K_VALUES) 
    {
        for (int i = 0; i < SWITCH_COUNT; i++) 
            delete[] K_VALUES[i];
        delete[] K_VALUES;
    }
    K_VALUES = nullptr;

    delete[] STATE0;   
    STATE0 = nullptr;

    delete[] STATE1;   
    STATE1 = nullptr;

    TRAIN_COUNT = 0;
    SWITCH_COUNT = 0;
    int SPAWN_COUNT = 0;
    int DEST_COUNT = 0;


    return 0;
}