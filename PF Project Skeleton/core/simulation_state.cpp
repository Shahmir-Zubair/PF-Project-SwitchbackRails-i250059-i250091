#include "simulation_state.h"
#include <iostream>
#include <fstream>
#include <cstring>
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

// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------

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


int initializeSimulationState() 
{
    string path = "../data/levels/", file_name = "easy_level.lvl";
    
    fstream file(path + file_name);

    if (!file.is_open()) 
    {
        cout<< "Error: could not open .lvl file"<<endl;
        return 1;
    }
    
    string line;
    int line_number = 0;
    while(getline(file, line))
    {
        line_number++;

        if(line_number==2)
            LVL_NAME = line;
        if(line_number==5)
            ROWS = stoi(line);
        if(line_number==8)
            COLS = stoi(line);
        if(line_number==11)
            SEED = stoi(line);
        if(line_number==14)
            WEATHER = line;
        if(line_number==17)
            break;
        
    }
    allocateGrid();

    for (int r = 0; r < ROWS; r++) 
    {
        if (!getline(file, line)) line = "";

        // pad line if shorter
        while (line.size() < COLS) line += ' ';

        for (int c = 0; c < COLS; c++) 
        {
            GRID[r][c] = line[c];
        }
    }
    file.close();

    ifstream file1(path + file_name);
    line_number = 0;
    bool switch_flag = false;
    while(getline(file1, line))
    {
        line_number++;
        if(line=="SWITCHES:")
        {
            line_number = 0;
            switch_flag = true;
        }
        if(switch_flag)
        {
            if(line.size()==0)
                break;
            else
                SWITCH_COUNT++;
        }
    }

    getline(file1, line);
    while(getline(file1, line))
        TRAIN_COUNT++;
    SWITCH_COUNT--;
    TRAIN_COUNT--;

    file1.close();
    allocateSwitchesTrains();
    ifstream file2(path + file_name);

    // fast-forward to SWITCHES:
    while (getline(file2, line)) 
    {
        if (line == "SWITCHES:") break;
    }
for (int i = 0; i < SWITCH_COUNT; i++) 
{
    getline(file2, line);   // read one switch line

    // --- Split line into 9 tokens manually ---
    string tokens[9];
    int wordCount = 0;
    string current = "";

    for (int j = 0; j < (int)line.size(); j++) 
    {
        char c = line[j];
        if (c == ' ') 
        {
            if (current != "") 
            {
                tokens[wordCount++] = current;
                current = "";
            }
        } 
        else 
            current += c;
    }
    if (current != "" && wordCount < 9) 
        tokens[wordCount++] = current;

    LETTER[i] = tokens[0][0];                        // Store Switch
    MODE[i] = tokens[1];                           // "PER_DIR" or "GLOBAL"
    INIT[i] = stoi(tokens[2]);                     // initial state (0 or 1)

    K_VALUES[i][0] = stoi(tokens[3]);                  // K_UP
    K_VALUES[i][1] = stoi(tokens[4]);                  // K_RIGHT
    K_VALUES[i][2] = stoi(tokens[5]);                  // K_DOWN
    K_VALUES[i][3] = stoi(tokens[6]);                  // K_LEFT

    STATE0[i]   = tokens[7];                           // label for State0
    STATE1[i]   = tokens[8];                           // label for State1
    }

    // fast-forward to TRAINS:
    while (getline(file2, line)) 
    {
        if (line == "TRAINS:") break;
    }

    // Read actual train lines
    int idx = 0;
    while (getline(file2, line)) 
    {
        if (line.length() == 0) continue;

        for(int i=0; i<=8; i++)
            if(i%2==0)
            {
                TRAINS[idx][i/2] = line[i] - '0';
            }

        idx++;
    }
    return 0;
}

int main()
{
    initializeSimulationState();
    cout<<LVL_NAME<<endl;
    cout<<ROWS<<endl;
    cout<<COLS<<endl;
    cout<<SEED<<endl;
    cout<<WEATHER<<endl;
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
            cout<<GRID[i][j];
        cout<<endl;
    }
    for(int i=0; i<TRAIN_COUNT; i++)
    {
        for(int j=0; j<5; j++)
            cout<<TRAINS[i][j]<<" ";
        cout<<endl;
    }
    for(int i=0; i < SWITCH_COUNT; i++)
    {
        cout<<LETTER[i]<<" "<<MODE[i]<<" "<<INIT[i]<<" "<<K_VALUES[i][0]<<" "<<K_VALUES[i][1]<<" "<<K_VALUES[i][2]<<" "<<K_VALUES[i][3]<<" "<<STATE0[i]<<" "<<STATE1[i]<<endl;
    }

}