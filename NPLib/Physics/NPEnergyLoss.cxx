/*****************************************************************************
 * Copyright (C) 2009    this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date   : April 2009                                              *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Deal with energy loss on basis of a dEdX input file, from SRIM or LISE++ *
 *   The class can be used to evaluate energy loss in material or to Evaluate* 
 *   initial energy before crossing the material.                            *
 *                                                                           *
 *   The use of Interpolator derived form the GSL insure a very good speed of*
 *   execution.                                                              *
 *                                                                           *
 *   Table Should come in the following unit:                                *
 *   Particle Energy: MeV/u                                                  *
 *   dEdX:            MeV/micrometer      | SRIM MeV/mm                      *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


//  STL
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>

using namespace std;

#include "NPEnergyLoss.h"
#include "TAxis.h"

//   NPL
using namespace NPL;

//ClassImp(EnergyLoss)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EnergyLoss::EnergyLoss() {
  fInter = NULL   ;
  fSpline = NULL;
  fMax = -10000; 
  fMin = 1e12;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EnergyLoss::~EnergyLoss() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EnergyLoss::EnergyLoss(string Path , string Source, int NumberOfSlice=100 ,  int LiseColumn , int NumberOfMass) { 
  fMax = -10000; 
  fMin = 1e12;

  fNumberOfSlice = NumberOfSlice ; 
  fNumberOfMass  = NumberOfMass  ;

  string globalPath = getenv("NPTOOL");
  string StandardPath = globalPath + "/Inputs/EnergyLoss/" + Path;

  cout << endl;
  cout << "/////////// Energy loss ///////////" << endl ;
  cout << "Initializing an EnergyLoss object " << endl ;

  ifstream TableFile   ;
  TableFile.open(StandardPath.c_str())   ;

  // Opening dE/dX file
  if(TableFile.is_open())    cout << "Reading Energy Loss File (standard path): " << Path << endl ; 
  // In case the file is not found in the standard path, the programm try to interpret the file name as an absolute or relative file path.
  else {
    TableFile.open( Path.c_str() );
    if(TableFile.is_open()) { cout << "Reading Energy Loss File (absolute path): " << Path << endl ;}

    else { cout << "ERROR: TABLE FILE "<< Path << " NOT FOUND" << endl; exit(1); }
  }


  if (Source == "G4Table" || Source == "G4table" ){
    // Reading Data
    double energy, total;
    string dummy;
    //skipped first line
    getline(TableFile,dummy);
    while ( TableFile >> energy >> total ){
      fEnergy.push_back ( energy*MeV );
      if(energy*MeV > fMax){
        fMax = energy*MeV ;
      }
      if(energy*MeV < fMin){
        fMin = energy*MeV ;
      }
      fdEdX_Total.push_back ( total*(MeV/micrometer) );
    }

    // Close File
    TableFile.close();

  }

  else if (Source == "SRIM")   {
    // Reading Data
    double energy, nuclear, electronic;
    string unit, dummy;

    while (  TableFile >> energy >> unit 
        >> electronic >> nuclear 
        >> dummy >> dummy >> dummy 
        >> dummy >> dummy >> dummy ){
      if ( unit == "keV" ) energy = energy*keV;
      if ( unit == "MeV" ) energy = energy*MeV;
      if ( unit == "GeV" ) energy = energy*GeV;
      fEnergy.push_back ( energy );
      if(energy*MeV > fMax)
        fMax = energy*MeV ;
      else if(energy*MeV < fMin || fMin <0)
        fMin = energy*MeV ;
      
      fdEdX_Nuclear     .push_back ( nuclear*MeV/mm );
      fdEdX_Electronic  .push_back ( electronic*MeV/mm );
      fdEdX_Total       .push_back ( nuclear*MeV/mm + electronic*MeV/mm );
    }

    // Close File
    TableFile.close();
  }

  else if(Source == "LISE"){
    // Reading Data
    double energy=0, energyloss=0;
    string dummy;            
    // skipping comment first line      
    getline(TableFile,dummy);

    while ( TableFile >> energy ){ 
      for (int k = 0 ; k < 11 ; k++ ){
        TableFile >> dummy ;
        if (k+1==LiseColumn) energyloss = atof(dummy.c_str()) ;
      }
      fEnergy.push_back (energy*MeV) ;
      if(energy*MeV > fMax)
        fMax = energy*MeV ;
      else if(energy*MeV < fMin || fMin <0)
        fMin = energy*MeV ;

      fdEdX_Total.push_back(energyloss*MeV/micrometer);
    }

    // Close File
    TableFile.close();
  }

  else{
    cout << "ERROR : EnergyLoss Wrong Source Type" << endl ;
    exit(1);
  }

  fInter = new TGraph(fEnergy.size(), &fEnergy[0], &fdEdX_Total[0]);
  fInter->Sort();
  fSpline = new TSpline3 ("Energy Loss Spline", fInter->GetX(), fInter->GetY(), fInter->GetN());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void EnergyLoss::Draw(string option) const{
    fInter->Draw(option.c_str());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EvaluateNuclearLoss(double Energy) const{
  if (fEnergy.size()==0 || fdEdX_Nuclear.size()==0){
    cout << "No Nuclear table for this Energy loss" ;
    return -1000;         
  }

  return Eval(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EvaluateElectronicLoss(double Energy) const{
  if (fEnergy.size()==0 || fdEdX_Electronic.size()==0){
    cout << "No Electronic table for this Energy loss" ;
    return -1000;
  }

  return Eval(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EvaluateTotalLoss(double Energy) const{
  if (fEnergy.size()==0 || fdEdX_Total.size()==0){
    cout << "No Total table for this Energy loss" ;
    return -1000;
  }

  return Eval(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void EnergyLoss::Print() const{
  cout << "Total Energy Loss : " << endl;
  int size = fdEdX_Total.size() ;
  for( int i = 0 ; i < size ; i++)
    cout << fEnergy[i]/MeV << " MeV " << fdEdX_Total[i]/MeV*micrometer << " MeV/um " << endl;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EnergyLossCalculation( double Energy          , // Energy of the detected particle
    double TargetThickness , // Target Thickness at 0 degree
    double Angle           ) // Particle Angle
  const
{
  return(Energy - Slow(Energy,TargetThickness,Angle) ) ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::Slow(   double Energy          , // Energy of the detected particle
    double TargetThickness , // Target Thickness at 0 degree
    double Angle           ) // Particle Angle
  const
{
  //   Lise file are given in MeV/u
  //   For SRIM and geant4 file fNumberOfMass = 1 whatever is the nucleus, file are given in MeV
  Energy = Energy / (double) fNumberOfMass ;

  if (Angle > halfpi) Angle = pi-Angle;
  TargetThickness = TargetThickness / ( cos(Angle) );

  double SliceThickness = TargetThickness / (double)fNumberOfSlice;

  for (int i = 0; i < fNumberOfSlice ; i++) 
  {
    double de = Eval(Energy) * SliceThickness;
    Energy    -= de/fNumberOfMass;

    if(Energy<0) {Energy=0;break;}
  }

  return (Energy*fNumberOfMass)   ;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double EnergyLoss::EvaluateInitialEnergy( double Energy           , // Energy of the detected particle
    double TargetThickness  , // Target Thickness at 0 degree
    double Angle            ) // Particle Angle
  const
{
  if(TargetThickness == 0 )
    return Energy;
  //   Lise file are given in MeV/u
  //   For SRIM and geant4 file fNumberOfMass = 1 whatever is the nucleus, file are given in MeV
  Energy = Energy / (double) fNumberOfMass ;
  if (Angle > halfpi) Angle = pi-Angle;
  TargetThickness = TargetThickness / ( cos(Angle) );
  double SliceThickness = TargetThickness / (double)fNumberOfSlice;
  for (int i = 0; i < fNumberOfSlice ; i++){
    double de = Eval(Energy) * SliceThickness ;
    Energy   += de/fNumberOfMass ;
  }
  return (Energy*fNumberOfMass) ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double   EnergyLoss::EvaluateMaterialThickness( double InitialEnergy    , // Energy of the detected particle
    double FinalEnergy      ,
    double ThicknessLimit   ,
    double ThicknessStep    ) // Target Thickness at 0 degree
  const 
{
  double Thickness = ThicknessStep ;
  double Energy = InitialEnergy;
  while(Energy<FinalEnergy && Thickness > 0){
    Energy = EvaluateInitialEnergy(Energy,ThicknessStep,0);
    Thickness+=ThicknessStep;
    if (Thickness>ThicknessLimit)
      Thickness= -1 ;

  }

  return Thickness ;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//   Evaluate Total Energy of particle from Energy loss in a giver thickness
double   EnergyLoss::EvaluateEnergyFromDeltaE(  double DeltaE           , // Energy of the detected particle
    double TargetThickness  , // Target Thickness at 0 degree
    double Angle            , // Particle Angle
    double EnergyMin        , // Starting Energy
    double EnergyMax        , // Maximum Energy allowed
    double EnergyResolution , // Resolution at which function stop
    int    MaxStep          ) // Stop after MaxStep Whatever Precision is reached
  const 
{

  double step_size = 10.*MeV;
  double Energy = EnergyMax;
  double DE = 0 ;
  bool check_low = false;
  bool check_high = false ;

  for(int i = 0 ; i < MaxStep ; i++){
    if(Energy>0)
      DE = Energy - Slow(Energy,TargetThickness,Angle) ;
    else
      return 0;
    
    if(abs(DeltaE-DE)<EnergyResolution) return Energy;
    else if (DeltaE-DE > 0){
      if(Energy - step_size > EnergyMin){
        Energy = Energy - step_size ;
        check_low = true ;
      }
      else {
        step_size = step_size / 10.;
        Energy = Energy - step_size ;
      }

    }

    else if (DeltaE-DE < 0){
      if(Energy + step_size < EnergyMax)
      {
        Energy = Energy + step_size ;
        check_high = true ;
      }
      else {
        step_size = step_size / 10.;
        Energy = Energy + step_size ;
      }
    }

    if(check_high && check_low){
      step_size = step_size/10.;
      check_high = false;
      check_low  = false;
    }

    if(step_size < EnergyResolution) return Energy;

  }

  return Energy;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double EnergyLoss::Eval(double energy) const {
  if(energy < 0){
    cout << "WARNING: negative energy given to NPL::EnergyLoss" << endl;
    return 0;
  }
  else if (energy!=energy){
    cout << "WARNING: nan energy given to NPL::EnergyLoss" << endl;
    return 0;
  }
    
  
  return fInter->Eval(energy,fSpline);
}










