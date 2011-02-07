/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
#include <sstream>
#include <fstream>
#include <limits>
#include <sys/stat.h>
#include <stdlib.h>

#include "RootInput.h"
#include "TAsciiFile.h"


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
   NumberOfFriend = 0;
   bool CheckTreeName     = false;
   bool CheckRootFileName = false;

   // Read configuration file Buffer
   string lineBuffer, dataBuffer;

   // Open file
   ifstream inputConfigFile;
   inputConfigFile.open(configFileName.c_str());
	
   pRootFile  = NULL;
   pRootChain = new TChain();
   
   cout << endl;
   cout << "/////////// ROOT Input files ///////////" << endl;
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
                  if (!pRootFile) pRootFile = new TFile(dataBuffer.c_str());
               }
            }
         }
      }
   }
		   
   if (!CheckRootFileName || !CheckTreeName) 
      cout << "WARNING: Token not found for InputTree Declaration : Input Tree may not be instantiate properly" << endl;
}



void RootInput::AddFriendChain(string RunToAdd)
{
   NumberOfFriend++;
   ostringstream suffix_buffer;
   suffix_buffer << "_" << NumberOfFriend ; 
   string suffix = suffix_buffer.str();
   bool CheckTreeName     = false;
   bool CheckRootFileName = false;

   // Read configuration file Buffer
   string lineBuffer, dataBuffer;

   // Open file
   ifstream inputConfigFile;
   inputConfigFile.open(RunToAdd.c_str());
	
   TChain* localChain = new TChain();
   
   cout << "/////////////////////////////////" << endl;
   cout << "Adding frien to current TChain" << endl;

   if (!inputConfigFile) {
      cout << "Run to Add file :" << RunToAdd << " not found " << endl; 
      return;
   }
   
   else {
      while (!inputConfigFile.eof()) {
         getline(inputConfigFile, lineBuffer);
			      
         // search for token giving the TTree name
         if (lineBuffer.compare(0, 9, "TTreeName") == 0) {
            inputConfigFile >> dataBuffer;
            // adding suffix to insure uniquity of the chain name
            dataBuffer+suffix;
            // initialize localChain
            localChain->SetName(dataBuffer.c_str());
            CheckTreeName = true ;
         }
     
         // search for token giving the list of Root files to treat
         else if (lineBuffer.compare(0, 12, "RootFileName") == 0  &&  localChain) {
            CheckRootFileName = true ;

            while (!inputConfigFile.eof()) {
               inputConfigFile >> dataBuffer;

               // ignore comment Line 
               if (dataBuffer.compare(0, 1, "%") == 0) {
                  inputConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
               }

               else if (!inputConfigFile.eof()) {
                  localChain->Add(dataBuffer.c_str());
                  cout << "Adding file " << dataBuffer << " to TChain" << endl;
               }
            }
         }
      }
   }
		   
   if (!CheckRootFileName || !CheckTreeName) 
      cout << "WARNING: Token not found for InputTree Declaration : Input Tree has not be Added to the current Chain" << endl;

   else
      pRootChain->AddFriend( localChain->GetName() );
      
   cout << "/////////////////////////////////" << endl;
}



string RootInput::DumpAsciiFile(const char* type, const char* folder)
{
   string name;

   string sfolder = folder;
   // create folder if not existing
   // first test if folder exist
   struct stat dirInfo;
   int res = stat(folder, &dirInfo);
   if (res != 0) {   // if folder does not exist, create it
      string cmd = "mkdir " + sfolder;
      if (system(cmd.c_str()) != 0) cout << "RootInput::DumpAsciiFile() problem creating directory" << endl;
   }

   string stype = type;
   if (stype == "EventGenerator") {
      TAsciiFile *aFile = (TAsciiFile*)pRootFile->Get(stype.c_str());
      // build file name
      string title = aFile->GetTitle();
      unsigned int pos = title.rfind("/");
      if (pos != string::npos) name = sfolder + title.substr(pos);
      else name = sfolder + "/" + title;
      aFile->WriteToFile(name.c_str());
   }
   else if (stype == "DetectorConfiguration") {
      TAsciiFile *aFile = (TAsciiFile*)pRootFile->Get(stype.c_str());
      // build file name
      string title = aFile->GetTitle();
      unsigned int pos = title.rfind("/");
      if (pos != string::npos) name = sfolder + title.substr(pos);
      else name = sfolder + "/" + title;
      aFile->WriteToFile(name.c_str());
   }
   else if (stype == "Calibration") {
   }
   else if (stype == "RunToTreat") {
   }
   else {
      cout << "RootInput::DumpAsciiFile() unkwown keyword" << endl;
   }

   return name;
}



RootInput::~RootInput()
{
   // delete default directory ./.tmp
   struct stat dirInfo;
   int res = stat("./.tmp", &dirInfo);
   if (res == 0) {   // if does exist, delete it
      if (system("rm -rf ./.tmp") != 0) cout << "RootInput::~RootInput() problem deleting ./.tmp directory" << endl; 
   }

   delete pRootFile;
   delete pRootChain;
}
