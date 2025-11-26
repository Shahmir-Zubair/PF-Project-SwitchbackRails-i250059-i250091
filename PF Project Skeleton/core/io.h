#ifndef IO_H
#define IO_H
#include <string>

// ============================================================================
// IO.H - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LEVEL LOADING
// ----------------------------------------------------------------------------
// Load a .lvl file.
void print_grid();
bool loadLevelFile(std::string file_name="easy_level.lvl");
// ----------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------
// Create/clear log files.
void initializeLogFiles();

// Append train movement to trace.csv.
void logTrainTrace();

// Append switch state to switches.csv.
void logSwitchState();

// Append signal state to signals.csv.
void logSignalState();

// Write final metrics to metrics.txt.
void writeMetrics();

#endif
