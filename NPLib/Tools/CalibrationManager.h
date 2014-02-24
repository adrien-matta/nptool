#ifndef ConfigManager_h 
#define ConfigManager_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : october 2009                                             *
 * Last update    : december 2012                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class manage the calibration coefficient of the detector in NPA      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   This class is a singleton                                               *
 *                                                                           *
 *****************************************************************************/
//   STL
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>

using namespace std ;

class CalibrationManager
   {
   
      protected:   //   Constructor and Destructor are protected because the class is a singleton
         CalibrationManager(string configFileName);
         ~CalibrationManager();
   
      public: // Accessor
         //   return a pointer to the calibration manager instance.
         //   if the instance does not exist it is created.
         static CalibrationManager* getInstance(string configFileName="XXX");
   
      private: // the instance
         //   Hold a pointer on itself
         static CalibrationManager* instance ;
   
      public:   //   File Management
         inline void AddFile(string Path) { fFileList.push_back(Path) ;} ;
         
         
      public:   // Calibration Parameter Related
   
         // call like : myCalibrationManager->AddParameter( "MUST2" ,"Telescope5_Si_X38_E", "T5_Si_X38_E" )
         // return false if the token is not found in the file list
         bool AddParameter(string DetectorName , string ParameterName , string Token)    ;      
         
         // call like : myCalibrationManager->ApplyCalibration( "MUST2/Telescope5_Si_X38_E" , RawEnergy )
         // return the Calibrated value
         double ApplyCalibration(const string& ParameterPath , const double& RawValue);
         // Same but with debug information outputs
         double ApplyCalibrationDebug(const string& ParameterPath , const double& RawValue);
         double ApplyResistivePositionCalibration(const string& ParameterPath , const double& RawValue);
         bool ApplyThreshold(const string& ParameterPath, const double& RawValue);
         double GetPedestal(const string& ParameterPath);
      
      public:   //   To be called after initialisation
         //   Loop over the file list and catch the file used for calibration
         void LoadParameterFromFile();

      public: //Clear calibration if we have some calibration files to read 
         void ClearCalibration();

      public: //Get correction coefficient vector
         vector<double> GetCorrection(const string& ParameterPath);



      private:
         //   This map hold a vector of the calibration coefficient. Index is the Parameter path, like "MUST2/Telescope5_Si_X38_E"
         map< string , vector<double> >   fCalibrationCoeff ;
         
         //   Hold the path of all the registered file of coeff
         vector<string>   fFileList   ;
         
         //   Hold The list of Token. Index is the Token, value the parameter path.
         map< string , string >   fToken      ;
      
   };
   
#endif
