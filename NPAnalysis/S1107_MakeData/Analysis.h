// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organizing we use Name Space.

/////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------- VARIOUS INCLUDE ---------------------------------------

// STL C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <time.h>
using namespace std;

// NPL
#include "TSharcData.h"
#include "TTigressData.h"

// Use CLHEP System of unit and Physical Constant
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"

// Global variable of the input tree

// Attached what is needed in the input tree
//int		tig_event_number;
int tig_num_chan;
int tig_event_id;

int* tig_midas_id;
int* tig_type; //0 for tig10 and 1 for tig 64, sound ok?  good.

int* channel_number;
int* channel_raw;
int* cfd_value;
int* led_value;
int* charge_raw;
float* charge_cal;


int* timestamp_low;
int* timestamp_high;
int* timestamp_live;
int* timestamp_tr;  // triggers requested
in*t timestamp_ta;  // triggers accepted

