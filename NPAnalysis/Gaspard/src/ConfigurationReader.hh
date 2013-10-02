#ifndef ConfigurationReader_h
#define ConfigurationReader_h 1

// STL 
#include <string>
#include <iostream>
#include <fstream>

// NPL 
#include "NPReaction.h"
using namespace NPL ;

#include "must.hh"

void ReadConfiguration ( string Path , array* myArray )	;
Reaction* ReadReaction ( string Path )					;


#endif

