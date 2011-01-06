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

#ifndef ROOTINPUT_HH
#define ROOTINPUT_HH

// C++ header
#include <string>

// ROOT headers
#include "TFile.h"
#include "TChain.h"
#include "TList.h"

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
   // Return the private chain
   TChain*	GetChain() {return pRootChain;};
   
   // Add a Friend chain to the input chain
   void     AddFriendChain( string RunToAdd);

private:
   TChain	*pRootChain;
   int NumberOfFriend;
};

#endif // ROOTINPUT_HH
