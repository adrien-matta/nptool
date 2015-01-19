#ifndef ROOTINPUT_HH
#define ROOTINPUT_HH
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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



// C++ header
#include <string>

// ROOT headers
#include "TFile.h"
#include "TChain.h"

using namespace std;


class RootInput
{
public:
   // The analysis class is designed to be a singleton (i.e. only one instance
   // can exist). A member function called Instance is defined, which allows
   // the user to get a pointer to the existing instance or to create it if
   // it does not yet exist:
   // (see the constructor for an explanation of the arguments)
   static RootInput* getInstance(string configFileName = "configFile");

   // The analysis class instance can be deleted by calling the Destroy
   // method (NOTE: The class destructor is protected, and can thus not be
   // called directly):
   static void Destroy();

protected:
   // Constructor (protected)
   RootInput(string configFileName);

   // Destructor (protected)
   virtual ~RootInput();

   // Prevent copying
   RootInput(const RootInput& only);
   const RootInput& operator=(const RootInput& only);

private:
   // The static instance of the RootInput class:
   static RootInput* instance;

public:
   string DumpAsciiFile(const char* type, const char* folder = "./.tmp");

public:
   // Return the private chain and file
   TChain*  GetChain()  {return pRootChain;}
   TFile*   GetFile()   {return pRootFile;}
   
   // Add a Friend chain to the input chain
   void     AddFriendChain(string RunToAdd);

private:
   TChain   *pRootChain;
   TFile    *pRootFile;
   int NumberOfFriend;
  
};

// A convenient function related to Root Input, coded Here so it can be called within ROOT CINT
TChain* MakeFriendTrees(string,string);

#endif // ROOTINPUT_HH
