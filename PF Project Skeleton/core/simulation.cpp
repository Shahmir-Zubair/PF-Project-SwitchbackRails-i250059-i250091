#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================
int TICK;
// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() 
{
    TICK = 0;
    
    initializeSimulationState();
    initializeLogFiles();
    loadLevelFile();
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick(int tick_no) {

    cout<<"Tick: "<<tick_no<<endl;
    print_grid();
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {

}
