#ifndef NPOPTIONMANAGER_HH
#define NPOPTIONMANAGER_HH

/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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

// C++ headers
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
      static NPOptionManager* getInstance(int argc = 0, char** argv = NULL);

      // The analysis class instance can be deleted by calling the Destroy
      // method (NOTE: The class destructor is protected, and can thus not be
      // called directly):
      static void Destroy();

   protected:
      // Constructor (protected)
      NPOptionManager(int argc, char** argv);

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
      void CheckArguments();
      void CheckEventGenerator();
      void CheckDetectorConfiguration();

   public:
      bool IsDefault(const char* type) const;
      void SendErrorAndExit(const char* type) const;

   public:
      // Getters
      // default values
      string GetDefaultReactionFile()     {return fDefaultReactionFileName;}
      string GetDefaultDetectorFile()     {return fDefaultDetectorFileName;}
      string GetDefaultRunToReadFile()    {return fDefaultRunToReadFileName;}
      string GetDefaultCalibrationFile()  {return fDefaultCalibrationFileName;}
      string GetDefaultOutputFile()       {return fDefaultOutputFileName;}
      // assigned values
      string GetReactionFile()             {return fReactionFileName;}
      string GetDetectorFile()             {return fDetectorFileName;}
      string GetRunToReadFile()            {return fRunToReadFileName;}
      string GetCalibrationFile()          {return fCalibrationFileName;}
      string GetOutputFile()               {return fOutputFileName;}
      bool   GetDisableAllBranchOption()   {return fDisableAllBranchOption;}
      bool   GetInputPhysicalTreeOption()  {return fInputPhysicalTreeOption;}
      int    GetVerboseLevel()             {return fVerboseLevel;}
      
      // Setters
      void SetReactionFile(string name)       {fReactionFileName = name;}
      void SetDetectorFile(string name)       {fDetectorFileName = name;}
      void SetRunToReadFile(string name)      {fRunToReadFileName = name;}
      void GetVerboseLevel(int VerboseLevel)  {fVerboseLevel = VerboseLevel;}
  
   private:
      // default values
      string fDefaultReactionFileName;
      string fDefaultDetectorFileName;
      string fDefaultRunToReadFileName;
      string fDefaultCalibrationFileName;
      string fDefaultOutputFileName;
      // assigned values
      string fReactionFileName;
      string fDetectorFileName;
      string fRunToReadFileName;
      string fCalibrationFileName;
      string fOutputFileName;
      bool   fDisableAllBranchOption;
      bool   fInputPhysicalTreeOption;
      int    fVerboseLevel; // 0 for not talk, 1 for talking
};

#endif
