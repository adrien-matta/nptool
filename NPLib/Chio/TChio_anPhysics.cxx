/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2011                                             *
 * Last update    : March   2011 by megumi                                   *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold chio treated data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/


#include "TChio_anPhysics.h"

// STL
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>

// NPL
#include "RootInput.h"
#include "RootOutput.h"

// ROOT
#include "TChain.h"
#include "TRandom.h"

ClassImp(TChio_anPhysics)
///////////////////////////////////////////////////////////////////////////
TChio_anPhysics::TChio_anPhysics()
{
  EventData    = new TChio_anData();
  EventPhysics = this;
  fdecay = new TF1("fdecay","-[0]*exp(-x/[1])");
}



TChio_anPhysics::~TChio_anPhysics()
{
}



// Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TChio_anPhysics::ReadConfiguration(string Path)
{
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer              ;
  string DataBuffer              ;

  double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz      ;
  TVector3 A , B , C , D                                             ;

  bool check_A = false  ;
  bool check_B = false     ;
  bool check_C = false  ;
  bool check_D = false  ;

  bool ReadingStatus = false ;

  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);

    //If line is a Start Up chio bloc, Reading toggle to true
    if (LineBuffer.compare(0, 4, "Chio") == 0) {
      cout << "///" << endl                ;
      cout << "Chio Detector found: " << endl   ;
      ReadingStatus = true              ;
    }

    // Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    // Reading Block
    while (ReadingStatus) {
      ConfigFile >> DataBuffer ;
      //  Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {
	ConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }

      //  Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, 4, "Chio") == 0) {
	cout << "WARNING: Another chio is found before standard sequence of Token, Error may have occured in chio definition" << endl ;
	ReadingStatus = false ;
      }

      //  Corner Position method

      else if (DataBuffer.compare(0, 2, "A=") == 0) {
	check_A = true;
	ConfigFile >> DataBuffer ;
	Ax = atof(DataBuffer.c_str()) ;
	Ax = Ax  ;
	ConfigFile >> DataBuffer ;
	Ay = atof(DataBuffer.c_str()) ;
	Ay = Ay  ;
	ConfigFile >> DataBuffer ;
	Az = atof(DataBuffer.c_str()) ;
	Az = Az  ;

	A = TVector3(Ax, Ay, Az);
	cout << "A corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
      }

      else if (DataBuffer.compare(0, 2, "B=") == 0) {
	check_B = true;
	ConfigFile >> DataBuffer ;
	Bx = atof(DataBuffer.c_str()) ;
	Bx = Bx  ;
	ConfigFile >> DataBuffer ;
	By = atof(DataBuffer.c_str()) ;
	By = By  ;
	ConfigFile >> DataBuffer ;
	Bz = atof(DataBuffer.c_str()) ;
	Bz = Bz  ;

	B = TVector3(Bx, By, Bz);
	cout << "B corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
      }

      else if (DataBuffer.compare(0, 2, "C=") == 0) {
	check_C = true;
	ConfigFile >> DataBuffer ;
	Cx = atof(DataBuffer.c_str()) ;
	Cx = Cx  ;
	ConfigFile >> DataBuffer ;
	Cy = atof(DataBuffer.c_str()) ;
	Cy = Cy  ;
	ConfigFile >> DataBuffer ;
	Cz = atof(DataBuffer.c_str()) ;
	Cz = Cz  ;

	C = TVector3(Cx, Cy, Cz);
	cout << "C corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
      }

      else if (DataBuffer.compare(0, 2, "D=") == 0) {
	check_D = true;
	ConfigFile >> DataBuffer ;
	Dx = atof(DataBuffer.c_str()) ;
	Dx = Dx  ;
	ConfigFile >> DataBuffer ;
	Dy = atof(DataBuffer.c_str()) ;
	Dy = Dy  ;
	ConfigFile >> DataBuffer ;
	Dz = atof(DataBuffer.c_str()) ;
	Dz = Dz  ;

	D = TVector3(Dx, Dy, Dz);
	cout << "D corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;

      }

      //  End Corner Position Method

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out
      if (check_A && check_B && check_C && check_D) {
	ReadingStatus = false;

	///Add The previously define telescope

	//      AddChio(  A   ,
	//    B   ,
	//    C   ,
	//    D   );

	check_A = false;
	check_B = false;
	check_C = false;
	check_D = false;
      }
    }

  }

  cout << endl << "/////////////////////////////" << endl << endl;
}



// Add Parameter to the CalibrationManger
void TChio_anPhysics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  Cal->AddParameter("CHIO", "AN_AMPLITUDE", "CHIO_AN_AMPLITUDE");
}



// Activated associated Branches and link it to the private member DetectorData address
// In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TChio_anPhysics::InitializeRootInputRaw()
{
  TChain* inputChain = RootInput::getInstance()->GetChain() ;
  inputChain->SetBranchStatus("CHIO_AN" , true)        ;
  inputChain->SetBranchStatus("fChio_an_*" , true)     ;
  inputChain->SetBranchAddress("CHIO_AN" , &EventData)           ;
}

//   Activated associated Branches and link it to the private member DetectorPhysics address
//   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
void TChio_anPhysics::InitializeRootInputPhysics()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("CHIO_AN" , true);
  // inputChain->SetBranchStatus("Ampl" , true);
  // inputChain->SetBranchStatus("Ampl_sscorr" , true);
  // inputChain->SetBranchStatus("Sum" , true);
  // inputChain->SetBranchStatus("Sum_sscorr" , true);
  // inputChain->SetBranchStatus("Peak" , true);
  // inputChain->SetBranchStatus("Peak_base" , true);
  // inputChain->SetBranchStatus("Peak_dist" , true);
  // inputChain->SetBranchStatus("Peak_stop" , true);
  // inputChain->SetBranchStatus("PileupNbr" , true);
  inputChain->SetBranchStatus("Npileup" , true);
  inputChain->SetBranchStatus("peak_start" , true);
  inputChain->SetBranchStatus("peak_stop" , true);
  inputChain->SetBranchStatus("risetime" , true);
  inputChain->SetBranchStatus("rawAmplitude" , true);
  inputChain->SetBranchStatus("calAmplitude" , true);
  inputChain->SetBranchStatus("Energy" , true);
  inputChain->SetBranchAddress("CHIO_AN" , &EventPhysics);

}



// Create associated branches and associated private member DetectorPhysics address
void TChio_anPhysics::InitializeRootOutput()
{
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("CHIO_AN" , "TChio_anPhysics" , &EventPhysics);
}



void TChio_anPhysics::Clear()
{
  // --- variables used in Jo's program
  // Ampl.clear();
  // Ampl_sscorr.clear();
  // Sum.clear();
  // Sum_sscorr.clear();
  // Peak.clear();
  // Peak_base.clear();
  // Peak_dist.clear();
  // Peak_stop.clear();
  // for(int i=0;i<350;i++)
  //   Energy[i]=0;
  Energy.clear();
  // PileupNbr = 0;
  
  // -- modified by megumi
  Npileup = 0;
  peak_start.clear();
  peak_stop.clear();
  risetime.clear();
  rawAmplitude.clear();
  calAmplitude.clear();
}



void TChio_anPhysics::Dump() const
{
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

  // cout << "Number of pileup : " << PileupNbr << endl;

  // for (unsigned int i = 0; i < Ampl.size() ; i++) {
  //    cout << "Amplitude :  " << Ampl.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Ampl_sscorr.size() ; i++) {
  //    cout << "Amplitude w/o correction :  " << Ampl_sscorr.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Sum.size() ; i++) {
  //    cout << "Sum :  " << Sum.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Sum_sscorr.size() ; i++) {
  //    cout << "Sum w/o correction :  " << Sum_sscorr.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Peak.size() ; i++) {
  //    cout << "Peak position :  " << Peak.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Peak_base.size() ; i++) {
  //    cout << "Peak base :  " << Peak_base.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Peak_dist.size() ; i++) {
  //    cout << "Peak distance :  " << Peak_dist.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Peak_stop.size() ; i++) {
  //    cout << "Peak stop :  " << Peak_stop.at(i) << endl;
  // }

  // for (unsigned int i = 0; i < Energy.size() ; i++) {
  //    cout << "Energy :  " << Energy.at(i) << endl;
  // }
}

// This method is called at each event read from the Input Tree. Aim is to build treat Raw dat in order to extract physical parameter
void TChio_anPhysics::BuildPhysicalEvent()
{
  BuildSimplePhysicalEvent();
}


// Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorithm but less efficient ...).
// This method aimed to be used for analysis performed during experiment, when speed is requiered.
void TChio_anPhysics::BuildSimplePhysicalEvent()
{
  // put here corresponding code of TChio_digCal.
  
  // // ========================================================
  // // from here --- modified by megumi
  /*
  // set variables
  Int_t Nch = EventData->GetEsize(); // number of channels
    
  // define variables
  Int_t threshold = 7;
  Int_t Toffset = 4;
  Double_t decay_t = 1400.0/25.; // decay time of pre-Amp (ns)/25ns = ch

  // read digitizer data into "Energy" 
  for(int ch=0;ch<Nch;ch++){
    Energy.push_back((double)EventData->GetEnergy(ch));
    // Energy[ch] = (int)EventData->GetEnergy(ch);
    // cout << EventData->GetEnergy(ch) << endl;
  }

  if(Nch!=0){
    int ch = 0;
    while(ch<Nch-1){
      if(Energy[ch]-Energy[ch+1]>threshold){
	peak_start.push_back(ch-Toffset);
	while(Energy[ch]-Energy[ch+1]>0){
	  ch++;
	}
	peak_stop.push_back(ch);
	risetime.push_back(peak_stop[Npileup] - peak_start[Npileup]);
	Npileup++;
      }
      ch++;
    }
    
    // deduce amplitude
    // -- define decay function
    
    fdecay->SetParLimits(0,0,10000); // only positive limitation
    fdecay->FixParameter(1,decay_t); // all peaks should have same decay time -> fixed
    
    
    for(int pid=0;pid<Npileup;pid++){
      // base line estimation
      Int_t fit_debut; // begining channel of fit
      Int_t fit_fin;   // end channel of fit
      // -- base line range
      if(pid==0) fit_debut = max((int)peak_start[pid]-20,0);
      else       fit_debut = max((int)peak_start[pid]-20,(int)peak_stop[pid-1]);
      fit_fin = peak_start[pid];
      // -- base line data
      TGraph *gra_base = new TGraph();
      int tmpi = 0;
      for(ch=fit_debut;ch<fit_fin;ch++){
	gra_base->SetPoint(tmpi,ch,Energy[ch]);
	tmpi++;
      }
      if(fit_debut<fit_fin){
	// -- base line fit
	fdecay->SetRange(fit_debut,fit_fin);
	gra_base->Fit("fdecay","rq");
	// deduce amplitude
	rawAmplitude.push_back(Energy[peak_stop[pid]]-fdecay->Eval(peak_stop[pid]));
	// calibration
	calAmplitude.push_back(LOCAL_CHIO_AN::CalibRawAmplitude(rawAmplitude[pid]));
      }
    }
  }
  */
}

namespace LOCAL_CHIO_AN
{
  double CalibRawAmplitude(const double rawAmplitude) 
  {
    return CalibrationManager::getInstance()->ApplyCalibration("CHIO/CHIO_AN_AMPLITUDE", rawAmplitude);
  }
}
