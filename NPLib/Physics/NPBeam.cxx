/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   : January 2013                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Beam:                                               *
 * User can enter various parameter, such as emittance or use ASCII or root  *
 * TH1F distribution                                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <vector>

// NPL header
#include "NPBeam.h"
#include "NPFunction.h"

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace NPL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Beam::Beam()
{
  fBeamNucleus = new Nucleus();
  fEnergy = 0;
  fSigmaEnergy = -1 ;
  fMeanX = 0 ;
  fMeanY = 0 ;
  fSigmaX = -1;
  fSigmaY = -0;
  fMeanThetaX = 0 ;
  fMeanPhiY = 0 ;
  fSigmaThetaX = -1 ;
  fSigmaPhiY = -1 ;
  fTargetSize = 0 ;
  fEffectiveTargetSize = 0 ;
  fTargetThickness = 0 ;
  fEffectiveTargetThickness = 0 ;
  fTargetAngle = 0 ;
  fTargetZ = 0 ;
  
  // case of user given distribution
  fEnergyHist  = new TH1F("EnergyHist","EnergyHist",1,0,1);
  fXThetaXHist = new TH2F("XThetaXHis","XThetaXHis",1,0,1,1,0,1);
  fYPhiYHist   = new TH2F("YPhiYHist","YPhiYHist",1,0,1,1,0,1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Beam::~Beam()
{
  delete fBeamNucleus ;
  delete fEnergyHist  ;
  delete fXThetaXHist ;
  delete fYPhiYHist   ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Beam::ReadConfigurationFile(string Path){
  ////////General Reading needs////////
  string LineBuffer;
  string DataBuffer;
  //////////////////////////////////////////////////////////////////////////////////////////
  ifstream ReactionFile;
  ReactionFile.open(Path.c_str());
  
  bool ReadingStatus      = false ;
  bool check_BeamName     = false ;
  bool check_Energy       = false ;
  bool check_SigmaEnergy  = false ;
  bool check_MeanX        = false ;
  bool check_MeanY        = false ;
  bool check_SigmaX       = false ;
  bool check_SigmaY       = false ;
  bool check_MeanThetaX   = false ;
  bool check_MeanPhiY     = false ;
  bool check_SigmaThetaX  = false ;
  bool check_SigmaPhiY    = false ;
  
  bool check_EnergyProfilePath  = false ;
  bool check_XThetaXPath        = false ;
  bool check_YPhiYPath          = false ;
  
  bool check_AllEnergy = false;
  bool check_AllEmittance = false;
  
  if (!ReactionFile.is_open()) {
    cout << "Error: File " << Path << " not found" << endl ;
    exit(1);
  }

  while (!ReactionFile.eof()) {
    //Pick-up next line
    getline(ReactionFile, LineBuffer);
    
    if (LineBuffer.compare(0, 4, "Beam") == 0) {
      cout << "Beam Found" << endl ;
      ReadingStatus = true ;
    }
    
    while(ReadingStatus){
      
      ReactionFile >> DataBuffer;
      
      //Search for comment Symbol %
      if (DataBuffer.compare(0, 1, "%") == 0) {   ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      else if (DataBuffer == "Particle=") {
        check_BeamName = true ;
        ReactionFile >> DataBuffer;
        delete fBeamNucleus;
        fBeamNucleus = new Nucleus(DataBuffer);
        cout << "Beam Particle: " << fBeamNucleus->GetName() << endl;
      }
      
      else if (DataBuffer == "Energy=") {
        check_Energy = true ;
        ReactionFile >> DataBuffer;
        fEnergy = atof(DataBuffer.c_str()) * MeV;
        cout << "Beam Energy: " << fEnergy / MeV << " MeV" << endl;
      }
      
      else if (DataBuffer == "SigmaEnergy=") {
        check_SigmaEnergy = true ;
        ReactionFile >> DataBuffer;
        fSigmaEnergy= atof(DataBuffer.c_str()) * MeV;
        cout << "Beam Energy Sigma: " << fSigmaEnergy / MeV << " MeV" << endl;
      }
      
      else if (DataBuffer=="MeanX=") {
        check_MeanX = true ;
        ReactionFile >> DataBuffer;
        fMeanX = atof(DataBuffer.c_str()) * mm;
        cout << "Mean X: " << fMeanX / mm << " mm" << endl;
      }
      
      else if (DataBuffer=="MeanY=") {
        check_MeanY = true ;
        ReactionFile >> DataBuffer;
        fMeanY = atof(DataBuffer.c_str()) * mm;
        cout << "Mean Y: " << fMeanY / mm << " mm" << endl;
      }
      
      else if (DataBuffer=="SigmaX=") {
        check_SigmaX = true ;
        ReactionFile >> DataBuffer;
        fSigmaX = atof(DataBuffer.c_str()) * mm;
        cout << "Sigma X: " << fSigmaX / mm << " mm" << endl;
      }
      
      else if (DataBuffer=="SigmaY=") {
        check_SigmaY = true ;
        ReactionFile >> DataBuffer;
        fSigmaY = atof(DataBuffer.c_str()) * mm;
        cout << "Sigma Y: " << fSigmaY / mm << " mm" << endl;
      }
      
      else if (DataBuffer == "MeanThetaX=" ) {
        check_MeanThetaX = true ;
        ReactionFile >> DataBuffer;
        fMeanThetaX = atof(DataBuffer.c_str()) * deg;
        cout << "Mean Theta X: " << fMeanThetaX / deg << " deg" << endl;
      }
      
      else if (DataBuffer == "MeanPhiY=") {
        check_MeanPhiY = true ;
        ReactionFile >> DataBuffer;
        fMeanPhiY = atof(DataBuffer.c_str()) * deg;
        cout << "Mean Phi Y: " << fMeanPhiY / deg << " deg" << endl;
      }
      
      else if (DataBuffer == "SigmaThetaX=" ) {
        check_SigmaThetaX = true ;
        ReactionFile >> DataBuffer;
        fSigmaThetaX = atof(DataBuffer.c_str()) * deg;
        cout << "Sigma Theta X: " << fSigmaThetaX / deg << " deg" << endl;
      }
      
      else if (DataBuffer == "SigmaPhiY=") {
        check_SigmaPhiY = true ;
        ReactionFile >> DataBuffer;
        fSigmaPhiY = atof(DataBuffer.c_str()) * deg;
        cout << "Sigma Phi Y: " << fSigmaPhiY / deg << " deg" << endl;
      }
      
      else if (DataBuffer == "EnergyProfilePath=") {
        check_EnergyProfilePath = true ;
        string FileName,HistName;
        ReactionFile >> FileName >> HistName;
        fEnergyHist = Read1DProfile(FileName, HistName );
      }
      
      else if (DataBuffer == "XThetaXProfilePath=") {
        check_XThetaXPath = true ;
        string FileName,HistName;
        ReactionFile >> FileName >> HistName;
        fXThetaXHist = Read2DProfile(FileName, HistName );
      }
      
      else if (DataBuffer == "YPhiYProfilePath=") {
        check_YPhiYPath = true ;
        string FileName,HistName;
        ReactionFile >> FileName >> HistName;
        fYPhiYHist = Read2DProfile(FileName, HistName );
      }
      
      
      ///////////////////////////////////////////////////
      //   If no Beam Token and no comment, toggle out
      else
          {ReadingStatus = false; cout << "WARNING : Wrong Token Sequence: Getting out " << endl ;}
      
      ///////////////////////////////////////////////////
      if(  ( check_MeanX && check_MeanY && check_SigmaX && check_SigmaY && check_SigmaThetaX && check_SigmaPhiY && check_MeanThetaX && check_MeanPhiY) || ( check_XThetaXPath && check_YPhiYPath ) ){
        check_AllEmittance = true ;
      }
      
      if(  ( check_Energy && check_SigmaEnergy ) || ( check_EnergyProfilePath ) ){
        check_AllEnergy = true ;
      }
      
      //   If all Token found toggle out
      if( check_BeamName && check_AllEnergy && check_AllEmittance )
        ReadingStatus = false ;
    }
  }
  
  if( !check_BeamName || !check_AllEnergy || !check_AllEnergy )
      {cout << "WARNING : Token Sequence Incomplete, Beam definition could not be Fonctionnal" << endl ;}
  
  cout << "///////////////////////////////////////////////////" << endl << endl ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Beam::GenerateRandomEvent(double& E, double& X, double& Y, double& Z, double& ThetaX, double& PhiY )
{
  X = Y = 1000000*cm;
  
  if(fSigmaEnergy!=-1)
    E = gRandom->Gaus(fEnergy,fSigmaEnergy);
  else
    E = fEnergyHist->GetRandom();
  
  if(fSigmaX!=-1){
    
  // Shoot within the target unless target size is null (no limit)
    while(sqrt(X*X+Y*Y>fEffectiveTargetSize) || fEffectiveTargetSize == 0){
      NPL::RandomGaussian2D(fMeanX, fMeanThetaX, fSigmaX, fSigmaThetaX, X, ThetaX);
      NPL::RandomGaussian2D(fMeanY, fMeanPhiY, fSigmaY, fSigmaPhiY, Y, PhiY);
    }
  }
  
  else{
    while(sqrt(X*X+Y*Y>fEffectiveTargetSize) || fEffectiveTargetSize == 0){
      fXThetaXHist->GetRandom2(X,ThetaX);
      fYPhiYHist->GetRandom2(Y,PhiY);
    }
  }
  
  Z = fTargetZ + fEffectiveTargetThickness*(gRandom->Uniform()-0.5);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Beam::Print() const {
  
  
}