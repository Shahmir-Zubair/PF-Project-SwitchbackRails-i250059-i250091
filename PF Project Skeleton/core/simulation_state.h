#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H
#include <string>

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================

// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------
extern int ROWS, COLS;
extern char** GRID;
// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
extern const int DIR_NONE;
extern const int DIR_UP;
extern const int DIR_RIGHT;
extern const int DIR_DOWN;
extern const int DIR_LEFT;

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------




// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------
extern int** TRAINS;
extern int TRAIN_COUNT;
bool* TRAIN_CAN_MOVE;
bool* TRAIN_CRASHED;
// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------
extern int SWITCH_COUNT;
extern char* LETTER;
extern std::string* MODE;
extern int* INIT;
extern int** K_VALUES;
extern std::string* STATE0;
extern std::string* STATE1;
extern int** SWITCH_COUNTER_DIR;
extern int* SWITCH_COUNTER_GLOBAL;  
extern int* SWITCH_FLIP_QUEUE;
extern int* SWITCH_STATE;
extern int** SWITCH_STATE_DIR;

// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------
extern int SPAWN_COUNT;
extern int** SPAWN_POINTS;


// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------
extern int DEST_COUNT;
extern int** DEST_POINTS;

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int TICK;
extern int SPAWNED_TRAINS;
extern int next_x[TRAIN_COUNT];
extern int next_y[TRAIN_COUNT];
extern int next_dir[TRAIN_COUNT];

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------
extern int SEED;
extern std::string WEATHER, LVL_NAME;

// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void allocateGrid();
void allocateSwitchesTrains();
void initializeSpawnDest();
void initializeSimulationState();

#endif
