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

//   STL
#include <cstdlib>
#include <limits>
#include <cmath>
#include <sstream>

//////////////////////////////////////////////////////////////////
CalibrationManager* CalibrationManager::instance = 0;

CalibrationManager* CalibrationManager::getInstance(string configFileName){
  // A new instance of CalibrationManager is created if it does not exist:
  if (instance == 0) {
    instance = new CalibrationManager(configFileName);
  }

  // The instance of CalibrationManager is returned:
  return instance;
}

//////////////////////////////////////////////////////////////////
CalibrationManager::CalibrationManager(string configFileName){
  // Read configuration file Buffer
  string lineBuffer, dataBuffer;

  // Open file
  ifstream inputConfigFile;
  inputConfigFile.open(configFileName.c_str());

  if(!NPOptionManager::getInstance()->IsDefault("Calibration")){
    cout << endl;
    cout << "/////////// Calibration Information ///////////" << endl;
    cout << "Getting list of calibration files" << endl;
  }
 
  if (!inputConfigFile) {
      if(!NPOptionManager::getInstance()->IsDefault("Calibration"))
        cout << "Calibration Path file: " << configFileName << " not found" << endl; 
    return;
  }

  else { 
    cout << "Reading list of files from: " << configFileName << endl; 
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
            AddFile(dataBuffer);
            cout << "Adding file " << dataBuffer << " to Calibration" << endl;
          }
        }
      }
    }
  }
  cout << "/////////////////////////////////" << endl;
}

//////////////////////////////////////////////////////////////////
CalibrationManager::~CalibrationManager()
{}

//////////////////////////////////////////////////////////////////
bool CalibrationManager::AddParameter(string DetectorName , string ParameterName , string Token ){
  string ParameterPath = DetectorName + "/" + ParameterName ;
  fToken[Token] = ParameterPath ;
  return true;
}

/////////////////////////////////////////////////////////////////
void CalibrationManager::ClearCalibration(){
  fCalibrationCoeff.clear();
}

/////////////////////////////////////////////////////////////////
vector<double> CalibrationManager::GetCorrection(const string& ParameterPath){
  vector<double> Coeff ;
  map< string , vector<double> >::iterator it ;
  it = fCalibrationCoeff.find(ParameterPath)  ;

  if(it == fCalibrationCoeff.end() )
  {
    /* cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
       cout << " ERROR: PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << endl ;
       cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;*/

    return Coeff ;
  }

  Coeff = it->second  ;

  return(Coeff);
}


//////////////////////////////////////////////////////////////////
void CalibrationManager::LoadParameterFromFile()
{
  ifstream CalibFile    ;
  string    DataBuffer   ;
  string   LineBuffer ;

  // Get pointer to the TAsciifile CalibrationFile in RootOuput
  TAsciiFile* AcsiiCalibration = RootOutput::getInstance()->GetAsciiFileCalibration();


  for(unsigned int i = 0 ; i < fFileList.size() ; i++)
  {
    CalibFile.open( fFileList[i].c_str() );
    map<string,string>::iterator it ;

    if(!CalibFile)
    {
      cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
      cout << " WARNING: FILE " << fFileList[i] << " IS MISSING "                                                        << endl ;
      cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
    }

    else 
    {
      // Append the Calibration File to the RootOuput for Back-up
      string comment = "%%% From File " + fFileList[i] + "%%%";
      AcsiiCalibration->AppendLine(comment.c_str());
      AcsiiCalibration->Append(fFileList[i].c_str());


      while( !CalibFile.eof() )
      {
        // Read the file Line by line
        getline(CalibFile, LineBuffer);

        // Create a istringstream to manipulate the line easely
        istringstream theLine (LineBuffer,istringstream::in);
        theLine >> DataBuffer ;

        // Comment support, comment symbole is %
        if(DataBuffer.compare(0, 1, "%") == 0) {
          CalibFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

        //   Search word in the token list
        it=fToken.find(DataBuffer);
        //   if the word is find, values are read
        if( it!=fToken.end() )
        { 
          vector<double> Coeff ;
          while( theLine >> DataBuffer ){
            Coeff.push_back( atof(DataBuffer.c_str()) ) ;
          }

          //   Check this parameter is not already define
          if( fCalibrationCoeff.find(it->second) != fCalibrationCoeff.end() ) 
            cout << "WARNING: Parameter " << it->second << " Already found. It will be overwritten. " << endl;

          //   Add the list of Coeff to the Coeff map using Parameter Path as index
          fCalibrationCoeff[ it->second ] = Coeff ;
        }

      }

    }


    CalibFile.close() ;
  }
}

//////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyCalibration(const string& ParameterPath , const double& RawValue){
  map< string , vector<double> >::iterator it ;
  static map< string , vector<double> >::iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;
  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
    return RawValue ;
  }

  // The vector size give the degree of calibration
  // We just apply the coeff it->second and returned the calibrated value
  double CalibratedValue = 0 ;
  unsigned int mysize = it->second.size(); 
  for(unsigned int i = 0 ; i < mysize ; i++){
    CalibratedValue += it->second[i]*pow(RawValue, (double)i);
  }

  return CalibratedValue ;

}
//////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyCalibrationDebug(const string& ParameterPath , const double& RawValue){
  map< string , vector<double> >::iterator it ;
  static map< string , vector<double> >::iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
           cout << " PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << endl ;
    return RawValue ;
  }

  // Else we take the second part of the element (first is index, ie: parameter path)
  // Second is the vector of Coeff
  cout << it->first << " :  raw = " << RawValue << " coeff = "  ;
  vector<double> Coeff = it->second  ;

  // The vector size give the degree of calibration
  // We just apply the coeff and returned the calibrated value

  double CalibratedValue = 0 ;
  for(unsigned int i = 0 ; i < Coeff.size() ; i++){
    cout << Coeff[i] << " " ;
    CalibratedValue += Coeff[i]*pow(RawValue, (double)i);
  }
  cout << "results = " << CalibratedValue << endl ;
  return CalibratedValue ;
}
//////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyResistivePositionCalibration(const string& ParameterPath , const double& DeltaRawValue){
  map< string , vector<double> >::iterator it ;
  static map< string , vector<double> >::iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;
  vector<double> Coeff = it->second  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
    return DeltaRawValue ;
  }

  // Check that the number of coeff is ok
  if(it->second.size()!=2) 
    return DeltaRawValue ; 

  double CalibratedValue = (DeltaRawValue-Coeff[0])/(Coeff[1]);

  return CalibratedValue ;
}

double CalibrationManager::ApplyResistivePositionCalibrationDebug(const string& ParameterPath , const double& DeltaRawValue){
  map< string , vector<double> >::iterator it ;
  static map< string , vector<double> >::iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;
  vector<double> Coeff = it->second  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
      cout << " PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << endl ;
    return DeltaRawValue ;
  }

  // Check that the number of coeff is ok
  if(Coeff.size()!=2){
      cout << " NUMBER OF COEFF " << Coeff.size() << " IS DIFFERENT THAN TWO " << endl ;
    return DeltaRawValue ; 
  }

  double CalibratedValue = (DeltaRawValue-Coeff[0])/(Coeff[1]);
  cout << it->first << " :  raw = " << DeltaRawValue << " coeff = "  ;
  cout << Coeff[0] << " " << Coeff[1] << endl ;
  cout << "results = " << CalibratedValue << endl ;

  return CalibratedValue ;
}

//////////////////////////////////////////////////////////////////
bool CalibrationManager::ApplyThreshold(const string& ParameterPath, const double& RawValue){
  map< string , vector<double> >::iterator it ;
  static map< string , vector<double> >::iterator ite = fCalibrationCoeff.end();
  
  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
    // cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
    // cout << " ERROR: PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << endl ;
    // cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;           
    return false;
  }

  // Else we take the second part of the element (first is index, ie: parameter path)
  // Second is the vector of Coeff
  vector<double> Coeff = it->second  ;

  // The vector size give the degree of calibration
  // We just apply the coeff and returned the calibrated value

  double ThresholdValue = 0 ;

  if(Coeff.size()==2){ // CATS style
    ThresholdValue = Coeff[0] + 3*Coeff[1];
  }
  else{ // Standard Threshold
    ThresholdValue = Coeff[0];
  }


  if(RawValue > ThresholdValue)
    return true;
  else 
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
double CalibrationManager::GetPedestal(const string& ParameterPath){
  return GetValue(ParameterPath,0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
double CalibrationManager::GetValue(const string& ParameterPath,const unsigned int& order){
  map< string , vector<double> >::iterator it ;
  static map< string , vector<double> >::iterator ite = fCalibrationCoeff.end();

  //   Find the good parameter in the Map
  // Using Find method of stl is the fastest way
  it = fCalibrationCoeff.find(ParameterPath)  ;

  // If the find methods return the end iterator it's mean the parameter was not found
  if(it == ite ){
   /* cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
    cout << " ERROR: PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << endl ;
    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
    */
    return 0;
  }

  // The vector size give the degree of calibration
  double Value = 0 ;
  if(it->second.size()>order){
     Value = it->second[order];
  }

  return Value;
}



