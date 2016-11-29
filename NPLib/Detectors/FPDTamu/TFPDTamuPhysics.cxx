/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: m.moukaddam@surrey.ac.uk                                 *
 *                                                                           *
 * Creation Date  : November 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold FPDTamu Treated  data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TFPDTamuPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"

//   ROOT
#include "TChain.h"

ClassImp(TFPDTamuPhysics)


///////////////////////////////////////////////////////////////////////////
TFPDTamuPhysics::TFPDTamuPhysics()
   : m_EventData(new TFPDTamuData),
     m_PreTreatedData(new TFPDTamuData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0), 
     m_NumberOfDelta(0),
     m_NumberOfMicro(0),
     m_NumberOfAWire(0),
     m_NumberOfPlast(0){
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  //Delta
  // match the energy and time together (not implemented yet) and fill the vectors
  unsigned int mysizeE = m_PreTreatedData->Get_Delta_Energy_Mult();
  for (UShort_t e = 0; e < mysizeE ; e++) {
        DeltaDetNumber.push_back(m_PreTreatedData->Get_Delta_E_DetectorNbr(e));
        DeltaEnergy.push_back(m_PreTreatedData->Get_Delta_Energy(e));
      }

  //Micro
  // match the energy and time together (not implemented yet) and fill the vectors    
  mysizeE = m_PreTreatedData->Get_Micro_Energy_Mult();
  for (UShort_t e = 0; e < mysizeE ; e++) {
        MicroRowNumber.push_back(m_PreTreatedData->Get_Micro_E_RowNbr(e));
        MicroColNumber.push_back(m_PreTreatedData->Get_Micro_E_ColNbr(e));
        //Calculate position in X and Z for each of the pads
        MicroPositionX.push_back((m_PreTreatedData->Get_Micro_E_ColNbr(e)-3)*0.20); // 0.20 is the Col pitch
        MicroPositionZ.push_back((m_PreTreatedData->Get_Micro_E_RowNbr(e)-3)*0.50); // 0.20 is the Col pitch
        //Pass the corresponding Energy, Charge 
        MicroEnergy.push_back(m_PreTreatedData->Get_Micro_Energy(e)); //calibrated
        MicroCharge.push_back(m_EventData->Get_Micro_Energy(e)); //uncalibrated
      }    

  //AWire
  //separate Left and right detectors 
  vector<double> awireLeftDetNumber, awireRightDetNumber;
  vector<double> awireLeftCharge, awireRightCharge;
  mysizeE = m_PreTreatedData->Get_AWire_Energy_Mult();
  for (UShort_t e = 0; e < mysizeE ; e++) {
    int side = m_PreTreatedData->Get_AWire_E_DetectorSide(e);
    int det = m_PreTreatedData->Get_AWire_E_DetectorNbr(e);
    double charge = m_PreTreatedData->Get_AWire_Energy(e);
    if (side==1) { 
      awireRightDetNumber.push_back(det);
      awireRightCharge.push_back(charge);
    }
    else {
      awireLeftDetNumber.push_back(det);
      awireLeftCharge.push_back(charge);
    }
  }   
  // match the left and right
  unsigned int mysizeL = awireLeftDetNumber.size();
  unsigned int mysizeR = awireRightDetNumber.size();
  //cout << " " << mysizeL << " " << mysizeR << endl ;
  if (mysizeL==mysizeR){ 
    for (UShort_t l = 0; l < mysizeL ; l++) {
      for (UShort_t r = 0; r < mysizeR ; r++) {
        if (awireLeftDetNumber.at(l) == awireRightDetNumber.at(r)) {
          //Pass the detector number
          int det = awireLeftDetNumber.at(l);
          AWireDetNumber.push_back(det);
          // Pass the left charge, right charge
          double EnergyL = awireLeftCharge.at(l);
          double EnergyR = awireRightCharge.at(r); 
          AWireLeftCharge.push_back(EnergyL);
          AWireRightCharge.push_back(EnergyR);
          // calculate position in X and Z
          double wire_length = 1.0; //check me!
          AWirePositionX.push_back(wire_length*(EnergyL-EnergyR)/(EnergyL+EnergyR)); //check me!
          AWirePositionZ.push_back(det*10+0.5); //check me!, or directly from configuration
        }
      }
    }
  // Calculate beam direction from X and Z position //check me!
    double a = 1, b = 0, Zplast = 30; // X = aZ + b // check me!
    BeamDirection.SetXYZ(1,0,a); //(1,a) is the direction vector in the plane X,Z. no information about Y 
    BeamDirection.Unit();  
  //Calculate position on Plastic from AWire data provided Z of the Plastic
    PositionOnPlasticX = a*Zplast + b;
    //cout << " AWire Left and Right sizes are in agreement,  L: " << mysizeL << "  R: "<< mysizeR<<endl;
    //m_PreTreatedData->Dump();
    //cin.get();
  }
  else { 
    //cout << " AWire Left and Right sizes are not in agreement,  L: " << mysizeL << "  R: "<< mysizeR<<endl;
    //m_PreTreatedData->Dump();
    //cin.get();
  }

  //Plast
  //separate Left and right detectors 
  vector<double> plastLeftCharge, plastRightCharge;
  mysizeE = m_PreTreatedData->Get_Plast_Energy_Mult();
  for (UShort_t e = 0; e < mysizeE ; e++) {
    int side = m_PreTreatedData->Get_Plast_E_DetectorSide(e);
    double charge = m_PreTreatedData->Get_Plast_Energy(e);
    if (side==1) { 
      plastRightCharge.push_back(charge);
    }
    else {
      plastLeftCharge.push_back(charge);
    }
  }   
  // match the left and right
  mysizeL = plastLeftCharge.size();
  mysizeR = plastRightCharge.size();
  //cout << " " << mysizeL << " " << mysizeR << endl ;
  if (mysizeL==mysizeR){ 
    for (UShort_t l = 0; l < mysizeL ; l++) {
      for (UShort_t r = 0; r < mysizeR ; r++) {
          // Pass the left charge, right charge
          double EnergyL = plastLeftCharge.at(l);
          double EnergyR = plastRightCharge.at(r); 
          PlastLeftCharge.push_back(EnergyL);
          PlastRightCharge.push_back(EnergyR);
          // calculate position in X, Z is known
          double plast_length = 1.0; //check me!
          PlastPositionX.push_back(plast_length*(EnergyL-EnergyR)/(EnergyL+EnergyR));
          PlastPositionZ.push_back(99); //check me!, directly from configuration
      }
    }
  }
  else{ 
    //cout << " Plastic Left and Right sizes are not in agreement,  L: " << mysizeL << "  R: "<< mysizeR<<endl;
    //m_PreTreatedData->Dump();
    //cin.get();
  }
/*
  //model
  // match energy and time together
  unsigned int mysizeE = m_PreTreatedData->Get_Delta_Energy_Mult();
  unsigned int mysizeT = m_PreTreatedData->Get_Delta_Time_Mult();
  cout << " " << mysizeE << " " << mysizeT << endl ; 
  for (UShort_t e = 0; e < mysizeE ; e++) {
    for (UShort_t t = 0; t < mysizeT ; t++) {
      if (m_PreTreatedData->Get_Delta_E_DetectorNbr(e) == m_PreTreatedData->Get_Delta_T_DetectorNbr(t)) {
        DeltaDetNumber.push_back(m_PreTreatedData->Get_Delta_E_DetectorNbr(e));
        DeltaEnergy.push_back(m_PreTreatedData->Get_Delta_Energy(e));
        DeltaTime.push_back(m_PreTreatedData->Get_Delta_Time(t));
      }
    }
  }
  */

}

///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();
  static string name; // channel name

  // Delta
  // Energy
  unsigned int mysize = m_EventData->Get_Delta_Energy_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Delta_Energy(i) > m_E_RAW_Threshold) {
      name = "FPDTamu/Delta_R" ;
      name+= NPL::itoa( m_EventData->Get_Delta_E_DetectorNbr(i)+1) ;
      name+= "_C1_E" ;
      Double_t Energy = Cal->ApplyCalibration(name, m_EventData->Get_Delta_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->Set_Delta_E(m_EventData->Get_Delta_E_DetectorNbr(i), Energy);
      }
    }
  }
  // Time 
  mysize = m_EventData->Get_Delta_Time_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
      name = "FPDTamu/Delta_R" ;
      name+= NPL::itoa( m_EventData->Get_Delta_T_DetectorNbr(i)+1) ;
      name+= "_C1_T" ;
      Double_t Time = Cal->ApplyCalibration(name, m_EventData->Get_Delta_Time(i));
      m_PreTreatedData->Set_Delta_T(m_EventData->Get_Delta_T_DetectorNbr(i), Time);
      }
  
  // Micromega
  // Energy
  mysize = m_EventData->Get_Micro_Energy_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Micro_Energy(i) > m_E_RAW_Threshold) {
      name = "FPDTamu/Micro_R" ;
      name+= NPL::itoa( m_EventData->Get_Micro_E_RowNbr(i)+1) ;
      name+= "_C" ;
      name+= NPL::itoa( m_EventData->Get_Micro_E_ColNbr(i)+1) ;
      name+= "_E" ;
      Double_t Energy = Cal->ApplyCalibration(name, m_EventData->Get_Micro_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->Set_Micro_E(m_EventData->Get_Micro_E_RowNbr(i),m_EventData->Get_Micro_E_ColNbr(i), Energy);
      }
    }
  }
  // Time 
  mysize = m_EventData->Get_Micro_Time_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Micro_Time(i) > m_T_RAW_Threshold) {
      name = "FPDTamu/Micro_R" ;
      name+= NPL::itoa( m_EventData->Get_Micro_T_RowNbr(i)+1) ;
      name+= "_C" ;
      name+= NPL::itoa( m_EventData->Get_Micro_T_ColNbr(i)+1) ;
      name+= "_T" ;
      Double_t Time = Cal->ApplyCalibration(name, m_EventData->Get_Micro_Energy(i));
      if (Time > m_E_Threshold) {
        m_PreTreatedData->Set_Micro_T(m_EventData->Get_Micro_T_RowNbr(i),m_EventData->Get_Micro_T_ColNbr(i), Time);
      }
    }
  }

// AWire
  // Energy
  mysize = m_EventData->Get_AWire_Energy_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_AWire_Energy(i) > m_E_RAW_Threshold) {
      name = "FPDTamu/AWire_R" ;
      name+= NPL::itoa( m_EventData->Get_AWire_E_DetectorNbr(i)+1) ;
      name+= "_C" ;
      name+= NPL::itoa( m_EventData->Get_AWire_E_DetectorSide(i)+1) ;
      name+= "_E" ;
      Double_t Energy = Cal->ApplyCalibration(name, m_EventData->Get_AWire_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->Set_AWire_E(m_EventData->Get_AWire_E_DetectorNbr(i),m_EventData->Get_AWire_E_DetectorSide(i), Energy);
      }
    }
  }
  // Time 
  mysize = m_EventData->Get_AWire_Time_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_AWire_Time(i) > m_T_RAW_Threshold) {
      name = "FPDTamu/AWire_R" ;
      name+= NPL::itoa( m_EventData->Get_AWire_T_DetectorNbr(i)+1) ;
      name+= "_C" ;
      name+= NPL::itoa( m_EventData->Get_AWire_T_DetectorSide(i)+1) ;
      name+= "_T" ;
      Double_t Time = Cal->ApplyCalibration(name, m_EventData->Get_AWire_Time(i));
      if (Time > m_T_Threshold) {
        m_PreTreatedData->Set_AWire_T(m_EventData->Get_AWire_T_DetectorNbr(i),m_EventData->Get_AWire_T_DetectorSide(i), Time);
      }
    }
  }


// Plastic
  // Energy
  mysize = m_EventData->Get_Plast_Energy_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Plast_Energy(i) > m_E_RAW_Threshold) {
      name = "FPDTamu/Plast_R1" ;
      name+= "_C" ;
      name+= NPL::itoa( m_EventData->Get_Plast_E_DetectorSide(i)+1) ;
      name+= "_E" ;
      Double_t Energy = Cal->ApplyCalibration(name, m_EventData->Get_Plast_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->Set_Plast_E(m_EventData->Get_Plast_E_DetectorSide(i), Energy);
      }
    }
  }
  // Time 
  mysize = m_EventData->Get_Plast_Time_Mult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Plast_Time(i) > m_T_RAW_Threshold) {
      name = "FPDTamu/Plast_R1" ;
      name+= "_C" ;
      name+= NPL::itoa( m_EventData->Get_Plast_T_DetectorSide(i)+1) ;
      name+= "_T" ;
      Double_t Time = Cal->ApplyCalibration(name, m_EventData->Get_Plast_Time(i));
      if (Time > m_T_Threshold) {
        m_PreTreatedData->Set_Plast_T(m_EventData->Get_Plast_T_DetectorSide(i), Time);
      }
    }
  }

}//end of function



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigFPDTamu.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigFPDTamu.dat found: Default parameter loaded for Analysis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigFPDTamu.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigFPDTamu.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigFPDTamu";
    if (LineBuffer.compare(0, name.length(), name) == 0) 
      ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {
      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo=="DELTA_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="DELTA_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }
    }
  }
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::Clear() {
  
 // Delta
  DeltaDetNumber.clear();
  DeltaCharge.clear();
  DeltaEnergy.clear();
  DeltaTime.clear();
  //Micromega
  MicroRowNumber.clear();
  MicroColNumber.clear();
  MicroPositionX.clear();
  MicroPositionZ.clear();
  MicroCharge.clear();
  MicroEnergy.clear();
  MicroTime.clear();
  //Avalanche wire
  AWireDetNumber.clear();
  AWireLeftCharge.clear();
  AWireRightCharge.clear();
  AWirePositionX.clear();
  AWirePositionZ.clear();
  //Plastic scintillator
  PlastDetNumber.clear();
  PlastLeftCharge.clear();
  PlastRightCharge.clear();
  PlastPositionX.clear();
  PlastPositionZ.clear();
  PlastTime.clear();
  //Calculated AWire and Plastic
  PositionOnPlasticX = -99 ; 
  BeamDirection.SetXYZ(0,0,0);
  
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::ReadConfiguration(string Path) {
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  
  double X, Y, Z; 
  TVector3 A , B , C , D;
  
  //Check if geometrical position read
  bool check_Delta_USL = false      ;
  bool check_Delta_USR = false      ;
  bool check_Micro_USL = false      ;
  bool check_Micro_USR = false      ;
  bool check_AWire_L = false        ;
  bool check_AWire_R = false        ;
  bool check_Plast_L = false        ;
  bool check_Plast_R = false        ;

  // Check if the sub-detectors are there
  bool check_Delta = false          ;
  bool check_Micro = false          ;
  bool check_AWire = false          ;
  bool check_Plast = false          ;

  bool check_Theta = false          ;
  bool check_Phi   = false          ;
  bool check_R     = false          ;
  bool check_Shape = false          ;
  bool check_X = false              ;
  bool check_Y = false              ;
  bool check_Z = false              ;

  //enabling readng several detectors
  bool ReadingStatus = false        ;
  bool ReadingDelta = false         ;

  while (!ConfigFile.eof()){

    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up FPDTamu bloc, Reading toggle to true
    string name="FPDTamu";
    string subname="";
    if (LineBuffer.compare(0, name.length(), name) == 0){
      cout << "///" << endl ;
      cout << "FPDTamu found: " << endl ;
      ReadingStatus = true ; 
    }

    //   Reading Block
    while(ReadingStatus){
      // Pickup Next Word
      ConfigFile >> DataBuffer ;

      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   
        ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }

      //   Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, name.length(), name) == 0) {
        cout << "\033[1;311mWARNING: Another detector is found before standard sequence of Token, Error may occured in detector definition\033[0m" << endl ;
        ReadingStatus = false ;
      }

      //DeltaE, Ionisation chamber
      else if (DataBuffer=="DELTA") {

            ReadingDelta = true;
            cout << "____//////" << endl ;
            cout << "____FPD TAMU DeltaE found:  "<< endl;
            
            //   Reading Delta Block
            while(ReadingDelta){
              ConfigFile >> DataBuffer ;
              subname="DELTA";
              //   Comment Line
              if (DataBuffer.compare(0, 1, "%") == 0) {   
                ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
              }
              //   Finding another telescope (safety), toggle out
              else if (DataBuffer.compare(0, subname.length(), subname) == 0) {
                cout << "\033[1;311mWARNING: Another DELTA is found before standard sequence of Token, Error may occured in detector definition\033[0m" << endl ;
                ReadingDelta = false ;
              }
              //Reading Upstream left point
              else if (DataBuffer=="UPSTREAM-LEFT="){
                  check_Delta_USL = true;
                  ConfigFile >> DataBuffer ;
                  X = atof(DataBuffer.c_str()) ;
                  ConfigFile >> DataBuffer ;
                  Y = atof(DataBuffer.c_str()) ;
                  ConfigFile >> DataBuffer ;
                  Z = atof(DataBuffer.c_str()) ;
                  A = TVector3(X, Y, Z);
                  cout << "____ ____ Upstream left corner position: ( " << A.X() << " ; " << A.Y() 
                  << " ; " << A.Z() << " )" << endl;
              }
              //Reading Upstream right point
              else if (DataBuffer=="UPSTREAM-RIGHT="){
                  check_Delta_USR = true;
                  ConfigFile >> DataBuffer ;
                  X = atof(DataBuffer.c_str()) ;
                  ConfigFile >> DataBuffer ;
                  Y = atof(DataBuffer.c_str()) ;
                  ConfigFile >> DataBuffer ;
                  Z = atof(DataBuffer.c_str()) ;
                  B = TVector3(X, Y, Z);
                  cout << "____ ____ Upstream right corner position: ( " << B.X() << " ; " << B.Y() 
                  << " ; " << B.Z() << " )" << endl;
              }
              //  If All necessary information there, toggle out
              if (check_Delta_USL && check_Delta_USR ){ 
                AddDelta( A, B );
                check_Delta = true ;
                //prepare for another Delta if necessary
                ReadingDelta = false;
                subname="";
                check_Delta_USL = false;
                check_Delta_USR = false;
                A.SetXYZ(-99,-99,-99);
                B.SetXYZ(-99,-99,-99);
                cout << "____FPD DeltaE configuration successful" << endl ;
                }
            }// end of while(ReadingDelta)
      }// end of if (DataBuffer=="DELTA")

      else if (DataBuffer=="MICRO") { // Micromega
        check_Micro = true;
        //ConfigFile >> DataBuffer ;
        cout << "____//////" << endl ;
        cout << "____FPD TAMU Micromega found "<< endl;
        cout << "____FPD Micromega configuration successful" << endl ;
      }

      else if (DataBuffer=="AWIRE") { // Avalanche wire
        check_AWire = true;
        cout << "____//////" << endl ;
        cout << "____FPD TAMU Avalanche wire found "<< endl;
        cout << "____FPD Avalanche wire configuration successful" << endl ;
      }

      else if (DataBuffer=="PLAST") { // Plastique
        check_Plast = true;
        cout << "____//////" << endl ;
        cout << "____FPD TAMU Plastic scintillator found "<< endl;
        cout << "____FPD Plastic scintillator configuration successful" << endl ;
      }
      /*
      //Angle method
      else if (DataBuffer=="THETA=") {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        cout << "Theta:  " << atof(DataBuffer.c_str()) << "deg" << endl;
      }

      else if (DataBuffer=="PHI=") {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        cout << "Phi:  " << atof( DataBuffer.c_str() ) << "deg" << endl;
      }

      else if (DataBuffer=="R=") {
        check_R = true;
        ConfigFile >> DataBuffer ;
        cout << "R:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
      }

      //Position method
      else if (DataBuffer=="X=") {
        check_X = true;
        ConfigFile >> DataBuffer ;
        cout << "X:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
      }

      else if (DataBuffer=="Y=") {
        check_Y = true;
        ConfigFile >> DataBuffer ;
        cout << "Y:  " << atof( DataBuffer.c_str() ) << "mm"<< endl;
      }

      else if (DataBuffer=="Z=") {
        check_Z = true;
        ConfigFile >> DataBuffer ;
        cout << "Z:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
      }


      //General
      else if (DataBuffer=="Shape=") {
        check_Shape = true;
        ConfigFile >> DataBuffer ;
        cout << "Shape:  " << DataBuffer << endl;
      }
    */
      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
        ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if ( (check_Delta && check_Micro && check_AWire && check_Plast) ){
        m_NumberOfDetectors++;
        //   Reinitialisation of Check Boolean
        check_Theta = false          ;
        check_Phi  = false           ;
        check_R     = false          ;
        check_Shape = false          ;
        check_X = false              ;
        check_Y = false              ;
        check_Z = false              ;
        ReadingStatus = false        ;
        check_Delta = false          ;
        check_Micro = false          ;
        check_AWire = false          ;
        check_Plast = false          ;
        cout << "///"<< endl         ;
      }
    }// end of while(ReadingStatus)
  }// end of configuration file 
}// enf of function ReadConfiguration



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::InitSpectra() {
  m_Spectra = new TFPDTamuSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TFPDTamuPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TFPDTamuPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::AddParameterToCalibrationManager() {

  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDelta; ++i) {
    cout << "Adding parameters " << m_NumberOfDelta << endl; 
    Cal->AddParameter("FPDTamu", "Delta_R"+ NPL::itoa(i+1)+"_C1_E",
                                 "Delta_R"+ NPL::itoa(i+1)+"_C1_E");
    Cal->AddParameter("FPDTamu", "Delta_R"+ NPL::itoa(i+1)+"_C1_T",
                                 "Delta_R"+ NPL::itoa(i+1)+"_C1_T");
  }

  for (int i = 0; i < m_NumberOfMicro; ++i) {
    for (int iRow = 0; iRow < 4; ++iRow) {
      for (int iCol = 0; iCol < 7; ++iCol) {
      Cal->AddParameter("FPDTamu", "Micro"+NPL::itoa(i+1)+"_R"+ NPL::itoa(iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_E",
                                   "Micro"+NPL::itoa(i+1)+"_R"+ NPL::itoa(iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_E");
      Cal->AddParameter("FPDTamu", "Micro"+NPL::itoa(i+1)+"_R"+ NPL::itoa(iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_T",
                                   "Micro"+NPL::itoa(i+1)+"_R"+ NPL::itoa(iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_T");      
      }
    }
  }

  for (int i = 0; i < m_NumberOfAWire; ++i) {
      for (int iCol = 0; iCol < 2; ++iCol) { // 2 cols for left and right
      Cal->AddParameter("FPDTamu", "AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E",
                           "FPDTamu_AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E");
      Cal->AddParameter("FPDTamu", "AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T",
                           "FPDTamu_AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T");      
      }
  }

  for (int i = 0; i < m_NumberOfPlast; ++i) { // Always 1 
      for (int iCol = 0; iCol < 2; ++iCol) { // 2 cols for left and right
      Cal->AddParameter("FPDTamu", "Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E",
                           "FPDTamu_Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E");
      Cal->AddParameter("FPDTamu", "Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T",
                           "FPDTamu_Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T");      
      }
  }

}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("FpdTAMU",  true );
  inputChain->SetBranchStatus("fFPD*" , true )   ;
  inputChain->SetBranchAddress("FpdTAMU", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "FPDTamu" , true );
  //Delta
  inputChain->SetBranchStatus( "DeltaDetNumber" , true );
  inputChain->SetBranchStatus( "DeltaCharge" , true );
  inputChain->SetBranchStatus( "DeltaEnergy" , true );
  //Micromega
  inputChain->SetBranchStatus( "MicroRowNumber" , true );
  inputChain->SetBranchStatus( "MicroColNumber" , true );
  inputChain->SetBranchStatus( "MicroPositionX" , true );
  inputChain->SetBranchStatus( "MicroPositionZ" , true );
  inputChain->SetBranchStatus( "MicroCharge" , true );
  inputChain->SetBranchStatus( "MicroEnergy" , true );
  //Avalanche wire
  inputChain->SetBranchStatus( "AWireDetNumber" , true );
  inputChain->SetBranchStatus( "AWireLeftCharge" , true );
  inputChain->SetBranchStatus( "AWireRightCharge" , true );
  inputChain->SetBranchStatus( "AWirePositionX" , true );
  inputChain->SetBranchStatus( "AWirePositionZ" , true );
  //Plastic scintillator
  inputChain->SetBranchStatus( "PlastDetNumber" , true );
  inputChain->SetBranchStatus( "PlastLeftCharge" , true );
  inputChain->SetBranchStatus( "PlastRightCharge" , true );
  inputChain->SetBranchStatus( "PlastPositionX" , true );
  inputChain->SetBranchStatus( "PlastPositionZ" , true );
  //Calculated AWire and Plastic
  inputChain->SetBranchStatus( "PositionOnPlasticX" , true );
  inputChain->SetBranchStatus( "BeamDirection" , true );

  inputChain->SetBranchAddress("FPDTamu", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("FPDTamu", "TFPDTamuPhysics", &m_EventPhysics);
}


void TFPDTamuPhysics::AddDelta(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfDelta++      ;

}

void TFPDTamuPhysics::AddMicro(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfMicro++      ;

}

void TFPDTamuPhysics::AddAWire(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfAWire++      ;

}

void TFPDTamuPhysics::AddPlast(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfPlast++      ;

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TFPDTamuPhysics::Construct() {
  return (NPL::VDetector*) new TFPDTamuPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_FPDTamu{
  public:
    proxy_FPDTamu(){
      NPL::DetectorFactory::getInstance()->AddToken("FPDTamu","FPDTamu");
      NPL::DetectorFactory::getInstance()->AddDetector("FPDTamu",TFPDTamuPhysics::Construct);
    }
};

proxy_FPDTamu p_FPDTamu;
}

