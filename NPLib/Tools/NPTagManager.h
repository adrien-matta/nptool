#ifndef __TAG__
#define __TAG__
/*****************************************************************************
 * Copyright (C) 2009 	this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   : November 2010                                           *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *	This class will held a set of string, that can be used as a TAG manager  *
 * Users can write macro and add different TAG to that object based on users *
 * condition. Then the TAG branch can be open and close alone to select event*
 * without loading the whole tree.                                           *
 *	                                                                         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

// ROOT header
#include "TObject.h"

namespace NPL 
	{
		class TagManager : public TObject{
    
    public:
     TagManager();
    ~TagManager();
    
    private:
     set<string> fTAG;
     
    public:
     // Return True if condition is in the set fTAG
     bool Is(string condition);
     
     // Add condition to the set fTAG
     void SetCondition(string condition);
     
     // Print all the condition that exist in fTAG
     void PrintCondition(); 
     inline void Print() {PrintCondition();};
     
     // Print to File filename all the condition that exist in fTAG
     void PrintConditionToFile(string filename);
        
     // Clear all the fTAG
     inline void Clear() {fTAG.clear();} ;
        
    };
  }
  
#endif
