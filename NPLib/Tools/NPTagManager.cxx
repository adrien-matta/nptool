/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
 *   This class will held a set of string, that can be used as a TAG manager *
 * Users can write macro and add different TAG to that object based on users *
 * condition. Then the TAG branch can be open and close alone to select event*
 * without loading the whole tree.                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 
#include "NPTagManager.h"

ClassImp(NPTagManager)

////////////////////////////////////////////
NPTagManager::NPTagManager() {}
NPTagManager::~NPTagManager(){}


////////////////////////////////////////////
bool NPTagManager::Is(string condition)
   {
     // return True is the element is find, false other wise
   return !( fTAG.find(condition)==fTAG.end() );
  }
////////////////////////////////////////////
void NPTagManager::AddCondition(string condition)
   {
    fTAG.insert(condition);
   }

////////////////////////////////////////////
void NPTagManager::PrintCondition()
   {
    set<string>::iterator it ;   

    cout << "------------------  Event Condition  ------------------" << endl ;
    
     for ( it=fTAG.begin() ; it!=fTAG.end() ; it++ )
      {
        cout << "  " << *it << endl ;
      }

    cout << "-------------------------------------------------------" << endl ;
    
   }

////////////////////////////////////////////
void NPTagManager::PrintConditionToFile(string filename)
   {
    
    ofstream file;
    file.open(filename.c_str());
    
    if(!file) {cout << "Warning: file " << filename << " not found " << endl ; return ;}
    
    else
      {
        set<string>::iterator it ;   
    
        file << "------------------  Event Condition  ------------------" << endl ;
        
         for ( it=fTAG.begin() ; it!=fTAG.end() ; it++ )
          {
            file << "  " << *it << endl ;
          }

        file << "-------------------------------------------------------" << endl ;
    
      } 
    
   }

        
