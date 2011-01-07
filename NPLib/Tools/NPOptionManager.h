#ifndef NPOPTIONMANAGER_HH
#define NPOPTIONMANAGER_HH

/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. MATTA  contact address: matta@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is a singleton class which deals with input        *
 * arguments of the different NPTool programm (NPS and NPA)                  *
 *---------------------------------------------------------------------------*
 * Comment: The singleton form allow users to call the object from anywhere  *
 * in the code                                                               *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL headers
#include <iostream>
#include <string>
using namespace std;

class NPOptionManager
  {
    public:
       // The analysis class is designed to be a singleton (i.e. only one instance
       // can exist). A member function called Instance is defined, which allows
       // the user to get a pointer to the existing instance or to create it if
       // it does not yet exist:
       // (see the constructor for an explanation of the arguments)
       static NPOptionManager* getInstance(int argc=0,char** argv=NULL);

       // The analysis class instance can be deleted by calling the Destroy
       // method (NOTE: The class destructor is protected, and can thus not be
       // called directly):
       static void Destroy();

    protected:
       // Constructor (protected)
       NPOptionManager(int argc,char** argv);

       // Destructor (protected)
       ~NPOptionManager() {};

       // Prevent copying
       NPOptionManager(const NPOptionManager& only);
       const NPOptionManager& operator=(const NPOptionManager& only);

    private:
       // The static instance of the NPOptionManager class:
       static NPOptionManager* instance;

    private:
      void DisplayHelp();

    public:
      string GetReactionFilePath()        { return fReactionFileName        ; } ;
      string GetDetectorFilePath()        { return fDetectorFileName        ; } ;
      string GetRunToReadFilePath()       { return fRunToReadFileName       ; } ;
      string GetCalibrationFilePath()     { return fCalibrationFileName     ; } ;
      string GetOutputFilePath()          { return fOutputFileName          ; } ;
      bool   GetDisableAllBranchOption()  { return fDisableAllBranchOption  ; } ;
      
    private:
      string fReactionFileName        ;
      string fDetectorFileName        ;
      string fRunToReadFileName       ;
      string fCalibrationFileName     ;
      string fOutputFileName          ;
      bool   fDisableAllBranchOption  ;
    
  };

#endif
