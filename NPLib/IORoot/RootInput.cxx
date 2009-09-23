/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is a singleton class which deals with the ROOT     *
 *             input file and tree both for NPSimulation and NPAnalysis.     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <fstream>

#include "RootInput.h"


RootInput *RootInput::instance = 0;

RootInput* RootInput::getInstance(string configFileName)
{
   // A new instance of RootInput is created if it does not exist:
   if (instance == 0) {
      instance = new RootInput(configFileName);
   }

   // The instance of RootInput is returned:
   return instance;
}


void RootInput::Destroy()
{
   if (instance != 0) {
      delete instance;
      instance = 0;
   }
}


// fileNameBase doit etre le nom du TChain.
RootInput::RootInput(string configFileName)
{
   bool CheckTreeName     = false;
   bool CheckRootFileName = false;

   // Read configuration file Buffer
   string lineBuffer, dataBuffer;

   // Open file
   ifstream inputConfigFile;
   inputConfigFile.open(configFileName.c_str());
	
   pRootChain = new TChain();
   
   cout << "/////////////////////////////////" << endl;
   cout << "Initializing input TChain" << endl;

   if (!inputConfigFile) {
      cout << "Run to Read file :" << configFileName << " not found " << endl; 
      return;
   }
   else {
      while (!inputConfigFile.eof()) {
         getline(inputConfigFile, lineBuffer);
			      
         // search for token giving the TTree name
         if (lineBuffer.compare(0, 9, "TTreeName") == 0) {
            inputConfigFile >> dataBuffer;
            // initialize pRootChain
            pRootChain->SetName(dataBuffer.c_str());
            CheckTreeName = true ;
         }
     
         // search for token giving the list of Root files to treat
         else if (lineBuffer.compare(0, 12, "RootFileName") == 0  &&  pRootChain) {
            CheckRootFileName = true ;

            while (!inputConfigFile.eof()) {
               inputConfigFile >> dataBuffer;

               // ignore comment Line 
               if (dataBuffer.compare(0, 1, "%") == 0) {
                  inputConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
               }

               else if (!inputConfigFile.eof()) {
                  pRootChain->Add(dataBuffer.c_str());
                  cout << "Adding file " << dataBuffer << " to TChain" << endl;
               }
            }
         }
      }
   }
		   
   if (!CheckRootFileName || !CheckTreeName) 
      cout << "WARNING: Token not found for InputTree Declaration : Input Tree may not be instantiate properly" << endl;

   cout << "/////////////////////////////////" << endl;
}



RootInput::~RootInput()
{
   delete pRootChain;
}
