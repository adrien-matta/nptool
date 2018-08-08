#ifndef ConfigManager_h 
#define ConfigManager_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
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


class CalibrationManager{

  protected:   //   Constructor and Destructor are protected because the class is a singleton
    CalibrationManager(std::string configFileName);
    ~CalibrationManager();

  public: // Accessor
    //   return a pointer to the calibration manager instance.
    //   if the instance does not exist it is created.
    static CalibrationManager* getInstance(const std::string& configFileName="XXX");

  private: // the instance
    //   Hold a pointer on itself
    static CalibrationManager* instance ;

  public:   //   File Management
    inline void AddFile(std::string Path) { fFileList.push_back(Path) ;} ;


  public:   // Calibration Parameter Related

    // call like : myCalibrationManager->AddParameter( "MUST2" ,"Telescope5_Si_X38_E", "T5_Si_X38_E" )
    // return false if the token is not found in the file list
    bool AddParameter(std::string DetectorName , std::string ParameterName , std::string Token,std::vector<double> def=std::vector<double>())    ;      

    // call like : myCalibrationManager->ApplyCalibration( "MUST2/Telescope5_Si_X38_E" , RawEnergy )
    // return the Calibrated value
    double ApplyCalibration (const std::string& ParameterPath , const double& RawValue) const ;
    double ApplyResistivePositionCalibration (const std::string& ParameterPath , const double& RawValue) const ;
    // Same but with debug information outputs
    double ApplyCalibrationDebug (const std::string& ParameterPath , const double& RawValue) const ;
    double ApplyResistivePositionCalibrationDebug (const std::string& ParameterPath , const double& RawValue) const ;

    bool ApplyThreshold (const std::string& ParameterPath, const double& RawValue) const ;
    double GetPedestal  (const std::string& ParameterPath) const ;
    double GetValue     (const std::string& ParameterPath,const unsigned int& order) const ;

  public:   //   To be called after initialisation
    //   Loop over the file list and catch the file used for calibration
    void LoadParameterFromFile();

  public: //Clear calibration if we have some calibration files to read 
    void ClearCalibration();

  public: //Get correction coefficient std::vector
    std::vector<double> GetCorrection (const std::string& ParameterPath) const ;

  private:
    //   This std::map hold a std::vector of the calibration coefficient. Index is the Parameter path, like "MUST2/Telescope5_Si_X38_E"

    std::map< std::string , std::vector<double> >   fCalibrationCoeff ;  
    //   Hold the path of all the registered file of coeff
    std::vector<std::string>   fFileList   ;

    //   Hold The list of Token. Index is the Token, value the parameter path.
    std::map< std::string , std::string >   fToken      ;

};

#endif
