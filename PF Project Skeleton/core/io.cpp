#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std;

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------

void print_grid()
{
    // cout<<"IN PRINTING"<<endl;
    // cout<<LVL_NAME<<endl;
    // cout<<ROWS<<endl;
    // cout<<COLS<<endl;
    // cout<<SEED<<endl;
    // cout<<WEATHER<<endl;
    // cout<<TRAIN_COUNT<<endl;
    // cout<<SWITCH_COUNT<<endl;

    for(int i=0; i<ROWS; i++)
    {
        cout<<"[";
        for(int j=0; j<COLS; j++)
            cout<<GRID[i][j];
        cout<<"]"<<endl;
    }
    // for(int i=0; i<TRAIN_COUNT; i++)
    // {
    //     for(int j=0; j<5; j++)
    //         cout<<TRAINS[i][j]<<" ";
    //     cout<<endl;
    // }
    // for(int i=0; i < SWITCH_COUNT; i++)
    // {
    //     cout<<LETTER[i]<<" "<<MODE[i]<<" "<<INIT[i]<<" "<<K_VALUES[i][0]<<" "<<K_VALUES[i][1]<<" "<<K_VALUES[i][2]<<" "<<K_VALUES[i][3]<<" "<<STATE0[i]<<" "<<STATE1[i]<<endl;
    // }
}


bool loadLevelFile(string file_name) 
{
    string path = "../data/levels/";
    // fast-forward to MAP:
    fstream c_rows(path + file_name);

    if (!c_rows.is_open()) 
    {
        cout<< "Error: could not open .lvl file"<<endl;
        return 1;
    }

    string line;
    while (getline(c_rows, line)) 
    {
        if (line == "MAP:") 
            break;
    }
    

    int rows_count = 0;
    int max_col = 0;
    while(getline(c_rows, line)) 
    {   
        if (line == "SWITCHES:")
            break;
        else if((int)line.size() > max_col)
            max_col = (int)line.size();
        rows_count++;
    }
    ROWS = rows_count;
    COLS = max_col;
    
    c_rows.close();
    // NEXT
    fstream file(path + file_name);

    if (!file.is_open()) 
    {
        cout<< "Error: could not open .lvl file"<<endl;
        return 1;
    }
    
    int line_number = 0;
    while(getline(file, line))
    {
        line_number++;

        if(line_number==2)
            LVL_NAME = line;
        if(line_number==11)
            SEED = stoi(line);
        if(line_number==14)
            WEATHER = line;
        if(line_number==16)
            break;
        
    }

    allocateGrid();
    for(int r = 0; r < ROWS; r++) 
    {   
        getline(file, line);
        if(line == "SWITCHES:")
            break;
        for(int c = 0; c < (int)line.size(); c++) 
        {
            GRID[r][c] = line[c];  
            if(isSpawnPoint(r, c))
                SPAWN_COUNT++;
            if(isDestinationPoint(r, c))
                DEST_COUNT++; 
        }
    }
    file.close();
    cout<<"________________"<<endl;

    initializeSpawnDest();

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
            if((int)line.size()==0)
                break;
            else
                SWITCH_COUNT++;
        }
    }

    getline(file1, line);
    while(getline(file1, line))
        TRAIN_COUNT++;
    SWITCH_COUNT--;

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

        for (int j = 0; j < (int)(int)line.size(); j++) 
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
        if (line == "TRAINS:") 
            break;
    }
    
    // Read actual train lines
    int idx = 0;
    while (getline(file2, line)) 
    {
        if (line == "") 
            continue;

        int col = 0;
        string token;
        bool hasToken = false;
        for (int j = 0; j <= (int)line.size(); j++) 
        {
            char c = (j < (int)line.size() ? line[j] : ' '); // add trailing space to flush last token
            if (c == ' ') 
            {
                if (token != "") 
                {
                    if (col < 5) 
                    {
                        TRAINS[idx][col] = stoi(token);
                        col++;
                    }
                    token = "";
                    hasToken = true;
                }
            } 
            else 
                token += c;
        }
        if(hasToken)
            idx++;
    }
    
    return 0;
}

// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------

void initializeLogFiles()
{
    // trace.csv
    ofstream traceFile("../data/CSVs/trace.csv", ios::trunc);
    traceFile.close();

    // switches.csv
    ofstream switchesFile("../data/CSVs/switches.csv", ios::trunc);
    switchesFile.close();

    // signals.csv
    ofstream signalsFile("../data/CSVs/signals.csv", ios::trunc);
    signalsFile.close();

    // metrics.txt
    ofstream metricsFile("../data/CSVs/metrics.txt", ios::trunc);
    metricsFile.close();
}


// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace() {
    
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState() {
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState() {
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
}