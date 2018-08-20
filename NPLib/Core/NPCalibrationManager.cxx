/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
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
#include "NPCalibrationManager.h"
#include "NPOptionManager.h"
#include "TAsciiFile.h"
#include "RootOutput.h"
#include "NPCore.h"
//   STL
#include <cstdlib>
#include <limits>
#include <cmath>
#include <sstream>
//   ROOT
#include <TSystem.h>

//////////////////////////////////////////////////////////////////
CalibrationManager* CalibrationManager::instance = 0;

CalibrationManager* CalibrationManager::getInstance(const std::string& configFileName){
  // A new instance of CalibrationManager is created if it does not exist:
  if (instance == 0) {
    instance = new CalibrationManager(configFileName);
  }

  // The instance of CalibrationManager is returned:
  return instance;
}

//////////////////////////////////////////////////////////////////
CalibrationManager::CalibrationManager(std::string configFileName){
  // Read configuration file Buffer
  std::string lineBuffer, dataBuffer;

  // Open file
  std::ifstream inputConfigFile;
  inputConfigFile.open(gSystem->ExpandPathName(configFileName.c_str()));

  if(!NPOptionManager::getInstance()->IsDefault("Calibration")){
    std::cout << std::endl;
    std::cout << "/////////// Calibration Information ///////////" << std::endl;
    std::cout << "Getting list of calibration files" << std::endl;
  }
 
  if (!inputConfigFile) {
      if(!NPOptionManager::getInstance()->IsDefault("Calibration"))
        std::cout << "Calibration Path file: " << configFileName << " not found" << std::endl; 
    return;
  }

  else { 
    std::cout << "Reading list of files from: " << configFileName << std::endl; 
    while (!inputConfigFile.eof()) {
      getline(inputConfigFile, lineBuffer);

      // search for token giving the list of Root files to treat
      if ( lineBuffer.compare(0, 19, "CalibrationFilePath") == 0 ) {
        while (!inputConfigFile.eof()) {
          inputConfigFile >> dataBuffer;

          // ignore comment Line 
          if (dataBuffer.compare(0, 1, "%") == 0) {
            inputConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          }

          else if (!inputConfigFile.eof()) {
						dataBuffer = gSystem->ExpandPathName(dataBuffer.c_str());
            AddFile(dataBuffer);
            std::cout << "Adding file " << dataBuffer << " to Calibration" << std::endl;
          }
        }
      }
    }
  }
  std::cout << "/////////////////////////////////" << std::endl;
}

//////////////////////////////////////////////////////////////////
CalibrationManager::~CalibrationManager()
{}

//////////////////////////////////////////////////////////////////
bool CalibrationManager::AddParameter(std::string DetectorName , std::string ParameterName , std::string Token ){
  std::string ParameterPath = DetectorName + "/" + ParameterName ;
  fToken[Token] = ParameterPath ;
  return true;
}

/////////////////////////////////////////////////////////////////
void CalibrationManager::ClearCalibration(){
  fCalibrationCoeff.clear();
}

/////////////////////////////////////////////////////////////////
std::vector<double> CalibrationManager::GetCorrection(const std::string& ParameterPath) const {
  std::vector<double> Coeff ;
  std::map< std::string , std::vector<double> >::const_iterator it ;
  it = fCalibrationCoeff.find(ParameterPath)  ;

  if(it == fCalibrationCoeff.end() )
  {
    /* std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << std::endl ;
       std::cout << " ERROR: PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << std::endl ;
       std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << std::endl ;*/

    return Coeff ;
  }

  Coeff = it->second  ;

  return(Coeff);
}


//////////////////////////////////////////////////////////////////
void CalibrationManager::LoadParameterFromFile(){
  std::ifstream  CalibFile;
  std::string    DataBuffer;
  std::string    LineBuffer;

  // Get pointer to the TAsciifile CalibrationFile in RootOuput
  TAsciiFile* AcsiiCalibration = RootOutput::getInstance()->GetAsciiFileCalibration();


  unsigned int sizeF = fFileList.size();
  for(unsigned int i = 0 ; i < sizeF ; i++){
    CalibFile.open( fFileList[i].c_str() );
    std::map<std::string,std::string>::iterator it ;

    if(!CalibFile){
      std::ostringstream message;
      message << "file " << fFileList[i] << " is missing " ;
      NPL::SendWarning ("NPL::CalibrationManager" , message.str());
    }

    else {
      // Append the Calibration File to the RootOuput for Back-up
      std::string comment = "%%% From File " + fFileList[i] + "%%%";
      AcsiiCalibration->AppendLine(comment.c_str());
      AcsiiCalibration->Append(fFileList[i].c_str());

      while( !CalibFile.eof() ){
        // Read the file Line by line
        getline(CalibFile, LineBuffer);

        // Create a istd::stringstream to manipulate the line easely
        std::istringstream theLine (LineBuffer,std::istringstream::in);
        theLine >> DataBuffer ;

        // Comment support, comment symbole is %
        if(DataBuffer.compare(0, 1, "%") == 0) {
          CalibFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

        //   Search word in the token list
        it=fToken.find(DataBuffer);
        //   if the word is find, values are read
        if( it!=fToken.end() ){ 
          std::vector<double> Coeff ;
          while( theLine >> DataBuffer ){
            Coeff.push_back( atof(DataBuffer.c_str()) ) ;
          }

          //   Check this parameter is not already define
          if( fCalibrationCoeff.find(it->second) != fCalibrationCoeff.end() ) 
            std::cout << "WARNING: Parameter " << it->second << " Already found. It will be overwritten. " << std::endl;

          //   Add the list of Coeff to the Coeff std::map using Parameter Path as index
          fCalibrationCoeff[ it->second ] = Coeff ;
        }
      }
    }
    CalibFile.close() ;
  }
}

//////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyCalibration(const std::string& ParameterPath , const double& RawValue) const {
  std::map< std::string , std::vector<double> >::const_iterator it ;
  static std::map< std::string , std::vector<double> >::const_iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;
  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
//by Shuya 170222
//std::cout << ParameterPath << "!" << std::endl;
    return RawValue ;
  }

  // The std::vector size give the degree of calibration
  // We just apply the coeff it->second and returned the calibrated value
  double CalibratedValue = 0 ;
  unsigned int mysize = it->second.size(); 
  for(unsigned int i = 0 ; i < mysize ; i++){
    CalibratedValue += it->second[i]*pow(RawValue, (double)i);
  }

  return CalibratedValue ;

}
//////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyCalibrationDebug(const std::string& ParameterPath , const double& RawValue) const{
  std::map< std::string , std::vector<double> >::const_iterator it ;
  static std::map< std::string , std::vector<double> >::const_iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
           std::cout << " PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << std::endl ;
    return RawValue ;
  }

  // Else we take the second part of the element (first is index, ie: parameter path)
  // Second is the std::vector of Coeff
  std::cout << it->first << " :  raw = " << RawValue << " coeff = "  ;
  std::vector<double> Coeff = it->second  ;

  // The std::vector size give the degree of calibration
  // We just apply the coeff and returned the calibrated value

  double CalibratedValue = 0 ;
  for(unsigned int i = 0 ; i < Coeff.size() ; i++){
    std::cout << Coeff[i] << " " ;
    CalibratedValue += Coeff[i]*pow(RawValue, (double)i);
  }
  std::cout << "results = " << CalibratedValue << std::endl ;
  return CalibratedValue ;
}
////////////////////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyResistivePositionCalibration(const std::string& ParameterPath , const double& DeltaRawValue) const{
  std::map< std::string , std::vector<double> >::const_iterator it ;
  static std::map< std::string , std::vector<double> >::const_iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
    return DeltaRawValue ;
  }

  // Check that the number of coeff is ok
  if(it->second.size()!=2) 
    return DeltaRawValue ; 

  double CalibratedValue = (DeltaRawValue-it->second[0])/(it->second[1]);

  return CalibratedValue ;
}

////////////////////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyResistivePositionCalibrationDebug(const std::string& ParameterPath , const double& DeltaRawValue) const {
  std::map< std::string , std::vector<double> >::const_iterator it ;
  static std::map< std::string , std::vector<double> >::const_iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;
	
  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
      std::cout << " PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << std::endl ;
    return DeltaRawValue ;
  }

	std::vector<double> Coeff = it->second  ;
	
  // Check that the number of coeff is ok
  if(Coeff.size()!=2){
      std::cout << " NUMBER OF COEFF " << Coeff.size() << " IS DIFFERENT THAN TWO " << std::endl ;
    return DeltaRawValue ; 
  }

  double CalibratedValue = (DeltaRawValue-Coeff[0])/(Coeff[1]);
  std::cout << it->first << " :  raw = " << DeltaRawValue << " coeff = "  ;
  std::cout << Coeff[0] << " " << Coeff[1] << std::endl ;
  std::cout << "results = " << CalibratedValue << std::endl ;

  return CalibratedValue ;
}

//////////////////////////////////////////////////////////////////
bool CalibrationManager::ApplyThreshold(const std::string& ParameterPath, const double& RawValue) const{
  std::map< std::string , std::vector<double> >::const_iterator it ;
  static std::map< std::string , std::vector<double> >::const_iterator ite = fCalibrationCoeff.end();
  
  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
    return false;
  }

  // The std::vector size give the degree of calibration
  // We just apply the coeff and returned the calibrated value

  double ThresholdValue ;

  if(it->second.size()==2){ // CATS style
    ThresholdValue = it->second[0] + 3*it->second[1];
  }
  else{ // Standard Threshold
    ThresholdValue = it->second[0];
  }


  if(RawValue > ThresholdValue)
    return true;
  else 
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
double CalibrationManager::GetPedestal(const std::string& ParameterPath) const{
  return GetValue(ParameterPath,0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
double CalibrationManager::GetValue(const std::string& ParameterPath,const unsigned int& order) const{
  std::map< std::string , std::vector<double> >::const_iterator it ;
  static std::map< std::string , std::vector<double> >::const_iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
    return 0;
  }

  // The std::vector size give the degree of calibration
  double Value = 0 ;
  if(it->second.size()>order){
     Value = it->second[order];
  }

  return Value;
}



