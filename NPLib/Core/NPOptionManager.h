#ifndef NPOPTIONMANAGER_HH
#define NPOPTIONMANAGER_HH

/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. MATTA  contact address: a.matta@surrey.ac.uk*
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

class NPOptionManager{
   public:
      // The analysis class is designed to be a singleton (i.e. only one instance
      // can exist). A member function called Instance is defined, which allows
      // the user to get a pointer to the existing instance or to create it if
      // it does not yet exist:
      // (see the constructor for an explanation of the arguments)
      static NPOptionManager* getInstance(int argc = 0, char** argv = NULL);
  
      // Added for compatibility with pROOF 
      static NPOptionManager* getInstance(string arg);

      // The analysis class instance can be deleted by calling the Destroy
      // method (NOTE: The class destructor is protected, and can thus not be
      // called directly):
      static void Destroy();

   protected:
      // Constructor (protected)
      NPOptionManager(int argc, char** argv);
      NPOptionManager(string arg);

      // Destructor (protected)
      ~NPOptionManager() {};

      // Prevent copying
      NPOptionManager(const NPOptionManager& only);
      const NPOptionManager& operator=(const NPOptionManager& only);

  private:
    // Read the input argument
    void ReadTheInputArgument(int argc = 0, char** argv = NULL);
  
   private:
      // The static instance of the NPOptionManager class:
      static NPOptionManager* instance;

   private:
      void DisplayHelp();
      void DisplayVersion();
      void CheckArguments();
      void CheckEventGenerator();
      void CheckDetectorConfiguration();
      void CheckG4Macro();

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
      string GetDefaultG4MacroPath()      {return fDefaultG4MacroPath;}

      // assigned values
      string GetReactionFile()             {return fReactionFileName;}
      string GetDetectorFile()             {return fDetectorFileName;}
      string GetRunToReadFile()            {return fRunToReadFileName;}
      string GetCalibrationFile()          {return fCalibrationFileName;}
      string GetOutputFile()               {return fOutputFileName;}
      string GetOutputTreeName()           {return fOutputTreeName;}
      string GetG4MacroPath()              {return fG4MacroPath;}     

      bool   GetDisableAllBranchOption()   {return fDisableAllBranchOption;}
      bool   GetInputPhysicalTreeOption()  {return fInputPhysicalTreeOption;}
      bool   GetGenerateHistoOption()      {return fGenerateHistoOption;}
      bool   GetCheckHistoOption()         {return fCheckHistoOption;}
      bool   GetOnline()                   {return fOnline;}
      bool   GetPROOF()                    {return fPROOFMode;}
      bool   GetG4BatchMode()              {return fG4BatchMode;}
      int    GetVerboseLevel()             {return fVerboseLevel;}
      int    GetNumberOfEntryToAnalyse()   {return fNumberOfEntryToAnalyse;} 
      string GetSharedLibExtension()       {return fSharedLibExtension;}     
      string GetLastFile();                 
      
      // Setters
      void SetReactionFile(string name)       {fReactionFileName = name;CheckEventGenerator();}
      void SetDetectorFile(string name)       {fDetectorFileName = name;CheckDetectorConfiguration();}
      void SetRunToReadFile(string name)      {fRunToReadFileName = name;}
      void SetVerboseLevel(int VerboseLevel)  {fVerboseLevel = VerboseLevel;}
  
   private:
      // default values
      string fDefaultReactionFileName;
      string fDefaultDetectorFileName;
      string fDefaultRunToReadFileName;
      string fDefaultCalibrationFileName;
      string fDefaultOutputFileName;
      string fDefaultOutputTreeName;
      string fDefaultG4MacroPath;

      // assigned values
      string fReactionFileName;
      string fDetectorFileName;
      string fRunToReadFileName;
      string fCalibrationFileName;
      string fOutputFileName;
      string fOutputTreeName;
      bool   fDisableAllBranchOption;
      bool   fInputPhysicalTreeOption;
      bool   fGenerateHistoOption;
      bool   fCheckHistoOption;
      bool   fOnline; // true if spectra server is started
      bool   fPROOFMode; // if true, the system run in a pROOF environment
      bool   fLastSimFile;
      bool   fLastPhyFile;
      bool   fLastResFile;
      bool   fLastAnyFile;
      int    fVerboseLevel; // 0 for not talk, 1 for talking
      int    fNumberOfEntryToAnalyse; // use to limit the number of analysed in NPA
      string fSharedLibExtension; // lib extension is platform dependent
      string fG4MacroPath; // Path to a geant4 macro to execute at start of nps
      bool fG4BatchMode; // Execute geant4 in batch mode, running the given macro
};

#endif
