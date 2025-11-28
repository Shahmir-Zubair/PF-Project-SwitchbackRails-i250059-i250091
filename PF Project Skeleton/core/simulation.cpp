#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation(string filename) 
{
    TICK = 0;
    
    initializeSimulationState();
    initializeLogFiles();
    loadLevelFile(filename);
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {

    cout<<"Tick: "<<TICK<<endl;
    print_grid();
    spawnTrainsForTick();
    
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {
    if(TICK == TRAINS[TRAIN_COUNT-1][0] + 1)
        return true;
    else
        return false;
}
