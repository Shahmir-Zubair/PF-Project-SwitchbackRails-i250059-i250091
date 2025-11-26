#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "grid.h"
#include "io.h"
#include <iostream>
using namespace std;

int main()
{

    initializeSimulation();
    simulateOneTick(TICK);


}

// loadLevelFile();
// print_level();

// cout<<endl<<"Spawns: "<<endl;
// for(int i=0; i<SPAWN_COUNT; i++)
// {
//     cout<<SPAWN_POINTS[i][0]<<", "<<SPAWN_POINTS[i][1]<<endl;
// }

// cout<<endl<<"Destinations: "<<endl;
// for(int i=0; i<DEST_COUNT; i++)
// {
//     cout<<DEST_POINTS[i][0]<<", "<<DEST_POINTS[i][1]<<endl;
// }