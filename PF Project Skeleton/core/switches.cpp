#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters() {
    for(int i=0;i<TRAIN_COUNT;i++)
    {
        int rows = TRAINS[i][1];
        int cols = TRAINS[i][2];

        if(!isSwitchTile(rows,cols))
        continue;

        char letter = GRID[rows][cols]; 
        int sindex = getSwitchIndex(letter);


        int direction = TRAINS[i][3];

        if(MODE[sindex] == "PER_DIR")
        {
            SWITCH_COUNTER_DIR[sindex][direction]++;
        }
        else
        {   
            SWITCH_COUNTER_GLOBAL[sindex]++;    
        }
    }
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
    for(int i=0; i<SWITCH_COUNT; i++)
    {
        if(MODE[i] == "PER_DIR")
        {
            for(int j=0; j<4; j++)
            {
                if(SWITCH_COUNTER_DIR[i][j] >= K_VALUES[i][j])
                {
                    SWITCH_FLIP_QUEUE[i] = 1;        
                    SWITCH_COUNTER_DIR[i][j] = 0;
                }
            }
        }
        else 
        {
            if(SWITCH_COUNTER_GLOBAL[i] >= K_VALUES[i][0]) 
            {
                SWITCH_FLIP_QUEUE[i] = 1;        
                SWITCH_COUNTER_GLOBAL[i] = 0;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
    for(int i=0; i<SWITCH_COUNT; i++)
    {
        if(SWITCH_FLIP_QUEUE[i] == 1)
        {
            // Toggle switch state
            // if(SWITCH_STATE[i] == 0)
            // {
            //     SWITCH_STATE[i] = 1;
            // }
            // else
            // {
            //     SWITCH_STATE[i] = 0;
            // }
            SWITCH_STATE[i] = 1 - SWITCH_STATE[i];
            SWITCH_FLIP_QUEUE[i] = 0;
        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(int sindex) {
    SWITCH_STATE[sindex] = 1 - SWITCH_STATE[sindex];
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection(int sindex, int direction) {
    if (MODE[sindex] == "PER_DIR") 
    {
        return SWITCH_STATE_DIR[sindex][direction];
    } 
    else 
    {
        return SWITCH_STATE[sindex];
    }
}
