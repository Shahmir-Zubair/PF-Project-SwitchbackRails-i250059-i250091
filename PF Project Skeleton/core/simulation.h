#ifndef SIMULATION_H
#define SIMULATION_H
#include <string>

// ============================================================================
// SIMULATION.H - Simulation tick logic
// ============================================================================
extern int TICK;
// ----------------------------------------------------------------------------
// MAIN SIMULATION FUNCTION
// ----------------------------------------------------------------------------
// Run one simulation tick.
void simulateOneTick();

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// Initialize the simulation after loading a level.
void initializeSimulation(std::string filename="easy_level.lvl");

// ----------------------------------------------------------------------------
// UTILITY
// ----------------------------------------------------------------------------
// True if all trains are delivered or crashed.
bool isSimulationComplete();

#endif


