/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
#include "NPSystemOfUnits.h"
#include "NPOptionManager.h"
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
  // Time and energy can't be matched since we have only an OR on time
  // fill the vectors, calculate positions   
  mysizeE = m_PreTreatedData->Get_Micro_Energy_Mult();
  for (UShort_t e = 0; e < mysizeE ; e++) {
        MicroRowNumber.push_back(m_PreTreatedData->Get_Micro_E_RowNbr(e));
        MicroColNumber.push_back(m_PreTreatedData->Get_Micro_E_ColNbr(e));
        //Calculate position in X and Z for each of the pads
        int col = m_PreTreatedData->Get_Micro_E_ColNbr(e) ;
        int row = m_PreTreatedData->Get_Micro_E_RowNbr(e) ; 
        int det = row/4; // 4 rows in the detector
        MicroPositionX.push_back(((col-3)*44.0)/NPUNITS::cm); // 0.20 is the Col pitch
        MicroPositionZ.push_back((((row+0.5)*32.5)+MicroLeftPos[det].Z())/NPUNITS::cm);
        //Pass the corresponding Energy, Charge 
        MicroEnergy.push_back(m_PreTreatedData->Get_Micro_Energy(e)); //calibrated
        MicroCharge.push_back(m_EventData->Get_Micro_Energy(e)); //uncalibrated
      } 
  unsigned int mysizeT = m_PreTreatedData->Get_Micro_Time_Mult();
  for (UShort_t t = 0; t < mysizeT ; t++) {
        //Pass the corresponding Time
        MicroTime.push_back(m_PreTreatedData->Get_Micro_Time(t));
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
          double wire_length = 2*fabs(AWireLeftPos[det].X())/NPUNITS::cm; 
          AWirePositionX.push_back(wire_length*(EnergyL-EnergyR)/(EnergyL+EnergyR));
          AWirePositionZ.push_back(AWireLeftPos[det].Z()/NPUNITS::cm);
        }
      }
    }
    // Calculate beam direction from X and Z position //check me!
    double a = 1, b = 0, Zplast = +547; // X = aZ + b // check me!
    IonDirection.SetXYZ(1,0,a); //(1,a) is the direction vector in the plane X,Z. no information about Y 
    IonDirection.Unit();  
    //Calculate position on Plastic from AWire data provided Z of the Plastic
    PlastPositionX_AW = a*Zplast + b;
    //cout << " AWire Left and Right sizes are in agreement,  L: " << mysizeL << "  R: "<< mysizeR<<endl;
    //m_PreTreatedData->Dump();
    //cin.get();
  }
  else { 
    //cout << " AWire Left and Right sizes are not in agreement,  L: " << mysizeL << "  R: "<< mysizeR<<endl;
    //m_PreTreatedData->Dump();
    //cin.get();
  }

  //Plastic
  //separate Left and right detectors 
  vector<double> plastLeftCharge, plastRightCharge;
  mysizeE = m_PreTreatedData->Get_Plast_Energy_Mult();
  for (UShort_t e = 0; e < mysizeE ; e++) {
    //collect info
    int side = m_PreTreatedData->Get_Plast_E_DetectorSide(e);
    double charge = m_PreTreatedData->Get_Plast_Energy(e);

    // skip values lower than a certain threshold
    if (charge<100) 
      continue;  

    //redistribute
    if (side==1)
      plastRightCharge.push_back(charge);
    else
      plastLeftCharge.push_back(charge);
  }  

  // match the left and right
  mysizeL = plastLeftCharge.size();
  mysizeR = plastRightCharge.size();
  if (mysizeL==mysizeR){ 
    for (UShort_t l = 0; l < mysizeL ; l++) {
      for (UShort_t r = 0; r < mysizeR ; r++) {
        // Pass the left charge, right charge
        double EnergyL = plastLeftCharge.at(l);
        double EnergyR = plastRightCharge.at(r); 
        PlastLeftCharge.push_back(EnergyL);
        PlastRightCharge.push_back(EnergyR);
        // calculate position in X, Z is known
        double plast_length = 2*PlastLeftPos.X()/NPUNITS::cm; //check me!
        PlastCharge.push_back(sqrt(EnergyL*EnergyR));
        PlastPositionX.push_back(plast_length*(EnergyL-EnergyR)/(EnergyL+EnergyR));
        PlastPositionZ.push_back(PlastLeftPos.Z()/NPUNITS::cm); //check me!, directly from configuration
      }
    }
  }
  else{ 
    //cout << " Plastic Left and Right sizes are not in agreement,  L: " << mysizeL << "  R: "<< mysizeR<<endl;
    //m_PreTreatedData->Dump();
    //cin.get();
  }

  //time: separate Left and right detectors 
  mysizeT = m_PreTreatedData->Get_Plast_Time_Mult();
  for (UShort_t t = 0; t < mysizeT ; t++) {
    //collect info
    int side = m_PreTreatedData->Get_Plast_T_DetectorSide(t);
    double time = m_PreTreatedData->Get_Plast_Time(t);
    // skip values lower than a certain threshold
    if (time<0) continue;  
    //redistribute
    if (side==1)
      PlastRightTime.push_back(time);
    else
      PlastLeftTime.push_back(time);
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

double TFPDTamuPhysics::GetMicroGroupEnergy(int lrow, int hrow, int lcol, int hcol){

  int dummy,row,col; 
  double energy = 0;  

  //avoid zeros
  if (lrow==0 || hrow==0 || lcol==0 || hcol==0)
    cout << " \033[1;311mWARNING: '0' value detected, TFPDTamuPhysics::GetMicroGroupEnergy() uses values >=1 " << endl;
  //check validity
  if (lrow>hrow) { 
    dummy = lrow;
    lrow = hrow;
    hrow = dummy;
  }
  if (lcol>hcol) { 
    dummy = lcol;
    lcol = hcol;
    hcol = dummy;
  }

  // group energies
  for (UShort_t e = 0; e < MicroRowNumber.size() ; e++) {
    row = MicroRowNumber.at(e)+1;
    col = MicroColNumber.at(e)+1;
    if ( (row>=lrow && row<=hrow) && (col>=lcol && col<=hcol) )
      energy += MicroEnergy.at(e);
  } 

  return energy ; 
}

double TFPDTamuPhysics::GetMicroRowGeomEnergy(int lrow, int hrow){

  int dummy; 
  double energy = 0;
  int sample = 0;   

  //avoid zeros
  if (lrow==0 || hrow==0 )
    cout << " \033[1;311mWARNING: '0' value detected, TFPDTamuPhysics::GetMicroRowGeomEnergy() uses values >=1 " << endl;
  //check validity
  if (lrow>hrow) { 
    dummy = lrow;
    lrow = hrow;
    hrow = dummy;
  }
  // group energies
  for (int r = lrow; r < hrow ; r++) {
    double esample = GetMicroGroupEnergy(r,r,1,7);
    if( esample > 0 ){
      sample++;  
      energy *= esample ;
    }
  } 

  return pow(energy,1./sample) ; 
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
      Double_t Time = Cal->ApplyCalibration(name, m_EventData->Get_Micro_Time(i));
      if (Time > m_T_Threshold) {
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
  PlastLeftCharge.clear();
  PlastRightCharge.clear();
  PlastLeftTime.clear();
  PlastRightTime.clear();
  PlastCharge.clear();
  PlastPositionX.clear();
  PlastPositionZ.clear();
  //Calculated 
  PlastPositionX_AW = -99 ; //from AWire and Plastic Z
  IonDirection.SetXYZ(0,0,0); // from AWire

}

//////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TFPDTamuPhysics::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  cout << "  ...oooOOOooo...   Delta  ...oooOOOooo...   " << endl;
  // Energy
  size_t mysize = DeltaEnergy.size();
  cout << "E Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << DeltaDetNumber[i]
      << " Charge: " << DeltaCharge[i]
      << " Energy: " << DeltaEnergy[i]
      <<endl;
  }
  // Time
  mysize = DeltaTime.size();
  cout << "T Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << DeltaDetNumber[i]
      << " Time: " << DeltaTime[i]
      <<endl;
  }

  cout << "  ...oooOOOooo...   Avalanche Wire  ...oooOOOooo...   " << endl;
  // Energy
  mysize = AWireLeftCharge.size();
  cout << "Charge Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << AWireDetNumber[i]
      << " Left: " << AWireLeftCharge[i]
      << " Right: " << AWireRightCharge[i]
      << " XPos: " << AWirePositionX[i]
      << " ZPos: " << AWirePositionZ[i]
      <<endl;
  }

  cout << "  ...oooOOOooo...   Micromega  ...oooOOOooo...   " << endl;
  // Energy
  mysize = MicroRowNumber.size();
  cout << " Row Charge:" <<endl; 
  for (size_t i = 0 ; i < MicroRowNumber.size() ; i++)
    cout << " " << MicroRowNumber[i];
  cout<<endl;
  cout << " Col Charge:" << endl;
  for (size_t i = 0 ; i < MicroColNumber.size() ; i++)
    cout << " " << MicroColNumber[i];
  cout<<endl;
  cout << " energy: "<<endl; 
  for (size_t i = 0 ; i < MicroColNumber.size() ; i++)
    cout << " " << MicroRowNumber[i];
  cout<<endl;
  cout << " energy: " << endl;
  for (size_t i = 0 ; i < MicroColNumber.size() ; i++)
    cout << " " << MicroRowNumber[i];
  cout<<endl;

  cout << "  ...oooOOOooo...   Plastic Scintillator  ...oooOOOooo...   " << endl;
  // Energy
  cout << " Left Charge:" ;
  for (size_t i = 0 ; i < PlastLeftCharge.size() ; i++)
    cout << " " << PlastLeftCharge[i];
  cout<<endl;
  cout << " Right Charge:" ; 
  for (size_t i = 0 ; i < PlastRightCharge.size() ; i++)
    cout << " " << PlastRightCharge[i];
  cout<<endl;
  cout << " total Charge:" ;
  for (size_t i = 0 ; i < PlastCharge.size() ; i++)
    cout << " " << PlastCharge[i];
  cout<<endl;
  cout << " XPos:"  ;
  for (size_t i = 0 ; i < PlastPositionX.size() ; i++)
    cout << " " << PlastPositionX[i];
  cout<<endl;
  cout << " ZPos:"  ;
  for (size_t i = 0 ; i < PlastPositionZ.size() ; i++)
    cout << " " << PlastPositionZ[i];
  cout<<endl;
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("FPDTamu");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token_delta = {"UPSTREAM-LEFT","UPSTREAM-RIGHT"};
  vector<string> token_micro = {"UPSTREAM-LEFT","UPSTREAM-RIGHT"};
  vector<string> token_awire = {"LEFT","RIGHT"};
  vector<string> token_plast = {"UPSTREAM-LEFT","UPSTREAM-RIGHT"};


  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->GetMainValue() == "DELTA"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "//// Delta " << i+1 << endl; 

      if(blocks[i]->HasTokenList(token_delta)){
        TVector3 left = blocks[i]->GetTVector3("UPSTREAM-LEFT","mm");
        TVector3 right = blocks[i]->GetTVector3("UPSTREAM-RIGHT","mm");  
        AddDelta(left,right);
      }

      else{
        cout << "Warning: check your input file formatting " << endl;
      }
    } 
    else if(blocks[i]->GetMainValue() == "MICRO"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "//// Micromegas " << i+1 << endl; 


      if(blocks[i]->HasTokenList(token_micro)){
        TVector3 left = blocks[i]->GetTVector3("UPSTREAM-LEFT","mm");
        TVector3 right = blocks[i]->GetTVector3("UPSTREAM-RIGHT","mm");  
        AddMicro(left,right);
      }

      else{
        cout << "Warning: check your input file formatting " << endl;
      }
    } 
    else if(blocks[i]->GetMainValue() == "AWIRE"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "//// Anode Wire " << i+1 << endl; 

      if(blocks[i]->HasTokenList(token_awire)){
        TVector3 left = blocks[i]->GetTVector3("LEFT","mm");
        TVector3 right = blocks[i]->GetTVector3("RIGHT","mm");  
        AddAWire(left,right);
      }

      else{
        cout << "Warning: check your input file formatting " << endl;
      }
    }
    else if(blocks[i]->GetMainValue() == "PLAST"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "//// Plastic " << i+1 << endl; 

      if(blocks[i]->HasTokenList(token_plast)){
        TVector3 left = blocks[i]->GetTVector3("UPSTREAM-LEFT","mm");
        TVector3 right = blocks[i]->GetTVector3("UPSTREAM-RIGHT","mm");  
        AddPlast(left,right);
      }

      else{
        cout << "ERROR: check your input file formatting " << endl;
        exit(1);
      }
    }
    else{
      cout << "ERROR: FPDTamu should be associated with another token : DELTA, MICRO, AWIRE ou PLAST" << endl;
      cout << "Given : " << blocks[i]->GetMainValue()  << endl;
      exit(1);
    }
  }

  ReadAnalysisConfig();

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
    Cal->AddParameter("FPDTamu", "Delta_R"+ NPL::itoa(i+1)+"_C1_E",
        "Delta_R"+ NPL::itoa(i+1)+"_C1_E");
    Cal->AddParameter("FPDTamu", "Delta_R"+ NPL::itoa(i+1)+"_C1_T",
        "Delta_R"+ NPL::itoa(i+1)+"_C1_T");
  }

  for (int i = 0; i < m_NumberOfMicro; ++i) { // in case there's 2 micromega add up the rows
    for (int iRow = 0; iRow < 4; ++iRow) {
      for (int iCol = 0; iCol < 7; ++iCol) {
        Cal->AddParameter("FPDTamu", "Micro_R"+ NPL::itoa((4*i)+iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_E",
            "Micro_R"+ NPL::itoa((4*i)+iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_E");
        Cal->AddParameter("FPDTamu", "Micro_R"+ NPL::itoa((4*i)+iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_T",
            "Micro_R"+ NPL::itoa((4*i)+iRow+1)+"_C"+ NPL::itoa(iCol+1)+"_T");      
      }
    }
  }

  for (int i = 0; i < m_NumberOfAWire; ++i) {
    for (int iCol = 0; iCol < 2; ++iCol) { // 2 cols for left and right
      Cal->AddParameter("FPDTamu", "AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E",
          "AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E");
      Cal->AddParameter("FPDTamu", "AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T",
          "AWire_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T");      
    }
  }

  for (int i = 0; i < m_NumberOfPlast; ++i) { // Always 1 
    for (int iCol = 0; iCol < 2; ++iCol) { // 2 cols for left and right
      Cal->AddParameter("FPDTamu", "Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E",
          "Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_E");
      Cal->AddParameter("FPDTamu", "Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T",
          "Plast_R"+ NPL::itoa(i+1)+"_C"+ NPL::itoa(iCol+1)+"_T");      
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
  inputChain->SetBranchStatus( "PlastLeftCharge" , true );
  inputChain->SetBranchStatus( "PlastRightCharge" , true );
  inputChain->SetBranchStatus( "PlastLeftTime" , true );
  inputChain->SetBranchStatus( "PlastRightTime" , true );
  inputChain->SetBranchStatus( "PlastCharge" , true );
  inputChain->SetBranchStatus( "PlastPositionX" , true );
  inputChain->SetBranchStatus( "PlastPositionZ" , true );
  //Calculated AWire and Plastic
  inputChain->SetBranchStatus( "PlastPositionX_AW" , true );
  inputChain->SetBranchStatus( "IonDirection" , true );

  inputChain->SetBranchAddress("FPDTamu", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TFPDTamuPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("FPDTamu", "TFPDTamuPhysics", &m_EventPhysics);
}


void TFPDTamuPhysics::AddDelta(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfDelta++      ;
  DeltaLeftPos.push_back(corner_UPL);
  DeltaRightPos.push_back(corner_UPR);
  if (m_NumberOfDelta>2) cout << " \033[1;311mWARNING: FPDTamu can have up to 2 Delta-E ion chambers  " << endl;

}

void TFPDTamuPhysics::AddMicro(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfMicro++      ;
  MicroLeftPos.push_back(corner_UPL);
  MicroRightPos.push_back(corner_UPR);
  if (m_NumberOfMicro>2) cout << " \033[1;311mWARNING: FPDTamu can have up to 2 Micormegas " << endl;
}

void TFPDTamuPhysics::AddAWire(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfAWire++      ;
  AWireLeftPos.push_back(corner_UPL);
  AWireRightPos.push_back(corner_UPR);
  if (m_NumberOfAWire>4) cout << " \033[1;311mWARNING: FPDTamu has 4 Avalanche Wires scintillator " << endl;
}

void TFPDTamuPhysics::AddPlast(TVector3 corner_UPL, TVector3 corner_UPR){
  m_NumberOfPlast++      ;
  PlastLeftPos= corner_UPL;
  PlastRightPos= corner_UPR;
  if (m_NumberOfPlast>1) cout << " \033[1;311mWARNING: FPDTamu has one plastic scintillator " << endl;

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

