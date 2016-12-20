/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date   : March 2009                                              *
 * Last update     : January 2011                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Two Body transfert Reaction                         *
 *  Physical parameter (Nuclei mass) are loaded from the nubtab03.asc file   *
 *  (2003 nuclear table of isotopes mass).                                   *
 *                                                                           *
 *  KineRelativistic: Used in NPSimulation                                   *
 *  A relativistic calculation is made to compute Light and Heavy nuclei     *
 *  angle given the Theta CM angle.                                          *
 *                                                                           *
 *  ReconstructRelativistic: Used in NPAnalysis                              *
 *  A relativistic calculation is made to compute Excitation energy given the*
 *  light angle and energy in Lab frame.                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + Redesign using LorentzVector by Pierre Morfouace                     *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                        *
 *    + Based on previous work by N.de Sereville                             *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <vector>

#include "NPReaction.h"
#include "NPOptionManager.h"
#include "NPFunction.h"

// Use CLHEP System of unit and Physical Constant
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// ROOT
#include"TF1.h"

ClassImp(Reaction)

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  Reaction::Reaction(){
    //------------- Default Constructor -------------

    // Need to be done before initializePrecomputeVariable
    fKineLine3 = 0 ;
    fKineLine4 = 0 ;
    fLineBrho3 = 0 ;
    fTheta3VsTheta4 = 0;
    fAngleLine = 0;

    //
    fBeamEnergy           = 0;
    fThetaCM              = 0;
    fExcitation3          = 0;
    fExcitation4          = 0;
    fQValue               = 0;
    fVerboseLevel         = NPOptionManager::getInstance()->GetVerboseLevel();
    initializePrecomputeVariable();

    fCrossSectionHist = NULL;
    fExcitationEnergyHist = NULL;
    fDoubleDifferentialCrossSectionHist = NULL ; 

    fshoot3=true;
    fshoot4=true;

  }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// This constructor aim to provide a fast way to instantiate a reaction without input file
// The string should be of the form "A(b,c)D@E" with E the ernegy of the beam in MeV
Reaction::Reaction(string reaction){
  // Instantiate the parameter to default  
  // Analyse the reaction and extract revelant information
  string A,b,c,D,E;
  unsigned int i=0;
  for(; i < reaction.length() ; i++){
    if(reaction.compare(i,1,"(")!=0) A.push_back(reaction[i]);
    else break;
  }

  i++;
  for(; i < reaction.length() ; i++){
    if(reaction.compare(i,1,",")!=0) b.push_back(reaction[i]);
    else break;
  }

  i++;
  for(; i < reaction.length() ; i++){
    if(reaction.compare(i,1,")")!=0) c.push_back(reaction[i]);
    else break;
  }

  i++;
  for(; i < reaction.length() ; i++){
    if(reaction.compare(i,1,"@")!=0) D.push_back(reaction[i]);
    else break;
  }

  i++;
  for(; i < reaction.length() ; i++){
    E.push_back(reaction[i]);
  }

  fKineLine3 = 0 ;
  fKineLine4 = 0 ;
  fLineBrho3 = 0;
  fTheta3VsTheta4 = 0;
  fAngleLine = 0;
  fNuclei1 = Beam(A);
  fNuclei2 = Nucleus(b);
  fNuclei3 = Nucleus(c);
  fNuclei4 = Nucleus(D);
  fBeamEnergy = atof(E.c_str());
  fThetaCM              = 0;
  fExcitation3          = 0;
  fExcitation4          = 0;
  fQValue               = 0;
  fVerboseLevel         = NPOptionManager::getInstance()->GetVerboseLevel();
  initializePrecomputeVariable();

  // do that to avoid warning from multiple Hist with same name...  int offset = 0;
  int offset = 0;
  while(gDirectory->FindObjectAny(Form("EnergyHist_%i",offset))!=0)
    ++offset;

  fCrossSectionHist = new TH1F(Form("EnergyHist_%i",offset),"Reaction_CS",1,0,180);
  fDoubleDifferentialCrossSectionHist = NULL ;

  fshoot3=true;
  fshoot4=true;

  initializePrecomputeVariable();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Reaction::~Reaction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool Reaction::CheckKinematic(){
  double theta = fThetaCM;
  if (m1 > m2) theta = M_PI - fThetaCM;

  fEnergyImpulsionCM_3	= TLorentzVector(pCM_3*sin(theta),0,pCM_3*cos(theta),ECM_3);
  fEnergyImpulsionCM_4	= fTotalEnergyImpulsionCM - fEnergyImpulsionCM_3;

  fEnergyImpulsionLab_3 = fEnergyImpulsionCM_3;
  fEnergyImpulsionLab_3.Boost(0,0,BetaCM);
  fEnergyImpulsionLab_4 = fEnergyImpulsionCM_4;
  fEnergyImpulsionLab_4.Boost(0,0,BetaCM);

  if ( fabs(fTotalEnergyImpulsionLab.E() - (fEnergyImpulsionLab_3.E()+fEnergyImpulsionLab_4.E()))> 1e-6){
    cout << "Problem with energy conservation" << endl;
    return false;
  }

  else{
    //cout << "Kinematic OK" << endl;
    return true;
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Reaction::ShootRandomThetaCM(){
  double theta;
  if(fDoubleDifferentialCrossSectionHist){
    // Take a slice in energy
    TAxis* Y = fDoubleDifferentialCrossSectionHist->GetYaxis();
    int binY;

    // Those test are there for the tail event of the energy distribution
    // In case the energy is outside the range of the 2D histo we take the 
    // closest available CS
    if(Y->FindBin(fBeamEnergy) > Y->GetLast())
      binY = Y->GetLast()-1;

    else if(Y->FindBin(fBeamEnergy) < Y->GetFirst())
      binY = Y->GetFirst()+1;

    else
      binY = Y->FindBin(fBeamEnergy);

    TH1D* Proj = fDoubleDifferentialCrossSectionHist->ProjectionX("proj",binY,binY);
    SetThetaCM( theta=Proj->GetRandom()*deg );
  }
  else
    SetThetaCM( theta=fCrossSectionHist->GetRandom()*deg );
  return theta;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Reaction::ShootRandomExcitationEnergy(){
  if(fExcitationEnergyHist){
    SetExcitation4(fExcitationEnergyHist->GetRandom());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Reaction::KineRelativistic(double& ThetaLab3, double& KineticEnergyLab3,
    double& ThetaLab4, double& KineticEnergyLab4){
  // 2-body relativistic kinematics: direct + inverse
  // EnergieLab3,4 : lab energy in MeV of the 2 ejectiles
  // ThetaLab3,4   : angles in rad

  // case of inverse kinematics
  double theta = fThetaCM;
  if (m1 > m2) theta = M_PI - fThetaCM;

  fEnergyImpulsionCM_3	= TLorentzVector(pCM_3*sin(theta),0,pCM_3*cos(theta),ECM_3);
  fEnergyImpulsionCM_4	= fTotalEnergyImpulsionCM - fEnergyImpulsionCM_3;

  fEnergyImpulsionLab_3 = fEnergyImpulsionCM_3;
  fEnergyImpulsionLab_3.Boost(0,0,BetaCM);
  fEnergyImpulsionLab_4 = fEnergyImpulsionCM_4;
  fEnergyImpulsionLab_4.Boost(0,0,BetaCM);

  // Angle in the lab frame
  ThetaLab3 = fEnergyImpulsionLab_3.Angle(fEnergyImpulsionLab_1.Vect());
  if (ThetaLab3 < 0) ThetaLab3 += M_PI;

  ThetaLab4 = fEnergyImpulsionLab_4.Angle(fEnergyImpulsionLab_1.Vect());
  if (fabs(ThetaLab3) < 1e-6) ThetaLab3 = 0;
  ThetaLab4 = fabs(ThetaLab4);
  if (fabs(ThetaLab4) < 1e-6) ThetaLab4 = 0;

  // Kinetic Energy in the lab frame
  KineticEnergyLab3 = fEnergyImpulsionLab_3.E() - m3;
  KineticEnergyLab4 = fEnergyImpulsionLab_4.E() - m4;

  // test for total energy conversion
  if (fabs(fTotalEnergyImpulsionLab.E() - (fEnergyImpulsionLab_3.E()+fEnergyImpulsionLab_4.E())) > 1e-6)
    cout << "Problem for energy conservation" << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Reaction::ReconstructRelativistic(double EnergyLab, double ThetaLab){
  // EnergyLab in MeV
  // ThetaLab in rad
  double E3 = m3 + EnergyLab;
  double p_Lab_3 = sqrt(E3*E3 - m3*m3);
  fEnergyImpulsionLab_3 = TLorentzVector(p_Lab_3*sin(ThetaLab),0,p_Lab_3*cos(ThetaLab),E3);
  fEnergyImpulsionLab_4 = fTotalEnergyImpulsionLab - fEnergyImpulsionLab_3;


  double Eex = fEnergyImpulsionLab_4.Mag() - fNuclei4.Mass();

  return Eex;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Return ThetaCM
double  Reaction::EnergyLabToThetaCM(double EnergyLab, double ThetaLab){
  double E3 = m3 + EnergyLab;
  double p_Lab_3 = sqrt(E3*E3 - m3*m3);

  fEnergyImpulsionLab_3 = TLorentzVector(p_Lab_3*sin(ThetaLab),0,p_Lab_3*cos(ThetaLab),E3);
  fEnergyImpulsionCM_3 = fEnergyImpulsionLab_3;
  fEnergyImpulsionCM_3.Boost(0,0,-BetaCM);

  double ThetaCM = M_PI - fEnergyImpulsionCM_1.Angle(fEnergyImpulsionCM_3.Vect());

  return(ThetaCM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Reaction::Print() const{
  // Print informations concerning the reaction

  cout << "Reaction : " << fNuclei2.GetName() << "(" << fNuclei1.GetName()
    << "," << fNuclei3.GetName() << ")" << fNuclei4.GetName() << "  @  "
    << fBeamEnergy << " MeV"
    << endl   ;

  cout << "Exc Nuclei 3 = " << fExcitation3 << " MeV" << endl;
  cout << "Exc Nuclei 4 = " << fExcitation4 << " MeV" << endl;
  cout << "Qgg = " << fQValue << " MeV" << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Reaction::ReadConfigurationFile(string Path){
  ifstream ReactionFile;
  string GlobalPath = getenv("NPTOOL");
  string StandardPath = GlobalPath + "/Inputs/EventGenerator/" + Path;
  ReactionFile.open(Path.c_str());
  if (!ReactionFile.is_open()) {
    ReactionFile.open(StandardPath.c_str());
    if(ReactionFile.is_open()) {
      Path = StandardPath;
    }
    else {cout << "Reaction File " << Path << " not found" << endl;exit(1);}
  }
  NPL::InputParser parser(Path);
  ReadConfigurationFile(parser);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Reaction::ReadConfigurationFile(NPL::InputParser parser){

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("TwoBodyReaction");
  if(blocks.size()>0 && NPOptionManager::getInstance()->GetVerboseLevel())
    cout << endl << "\033[1;35m//// Two body reaction found " << endl; 

  vector<string> token1 = {"Beam","Target","Light","Heavy"};
  vector<string> token2 = {"Beam","Target","Nuclei3","Nuclei4"};
  double CSHalfOpenAngleMin= 0*deg ;
  double CSHalfOpenAngleMax= 180*deg;
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token1)){
      int v = NPOptionManager::getInstance()->GetVerboseLevel();
      NPOptionManager::getInstance()->SetVerboseLevel(0);
      fNuclei1.ReadConfigurationFile(parser);
      NPOptionManager::getInstance()->SetVerboseLevel(v);

      fBeamEnergy= fNuclei1.GetEnergy();

      fNuclei2 = Nucleus(blocks[i]->GetString("Target"));
      fNuclei3 = Nucleus(blocks[i]->GetString("Light"));
      fNuclei4 = Nucleus(blocks[i]->GetString("Heavy"));
    }
    else if(blocks[i]->HasTokenList(token2)){
      fNuclei1.SetVerboseLevel(0);
      fNuclei1.ReadConfigurationFile(parser);
      fBeamEnergy= fNuclei1.GetEnergy();

      fNuclei2 = Nucleus(blocks[i]->GetString("Target"));
      fNuclei3 = Nucleus(blocks[i]->GetString("Nuclei3"));
      fNuclei4 = Nucleus(blocks[i]->GetString("Nuclei4"));
    }    
    else{
      cout << "ERROR: check your input file formatting \033[0m" << endl; 
      exit(1);
    }

    if(blocks[i]->HasToken("ExcitationEnergyLight"))
      fExcitation3 = blocks[i]->GetDouble("ExcitationEnergyLight","MeV"); 
    else if(blocks[i]->HasToken("ExcitationEnergy3"))
      fExcitation3 = blocks[i]->GetDouble("ExcitationEnergy3","MeV"); 

    if(blocks[i]->HasToken("ExcitationEnergyHeavy"))
      fExcitation4 = blocks[i]->GetDouble("ExcitationEnergyHeavy","MeV"); 
    else if(blocks[i]->HasToken("ExcitationEnergy4"))
      fExcitation4 = blocks[i]->GetDouble("ExcitationEnergy4","MeV"); 

    if(blocks[i]->HasToken("ExcitationEnergyDistribution")){
      vector<string> file = blocks[i]->GetVectorString("ExcitationEnergyDistribution");
      fExcitationEnergyHist = Read1DProfile(file[0],file[1]);
      fExcitation4 = 0 ;
    }

    if(blocks[i]->HasToken("CrossSectionPath")){
      vector<string> file = blocks[i]->GetVectorString("CrossSectionPath");
      TH1F* CStemp = Read1DProfile(file[0], file[1]);

      // multiply CStemp by sin(theta)
      TF1* fsin = new TF1("sin",Form("1/(sin(x*%f/180.))",M_PI),0,180);
      CStemp->Divide(fsin,1);
      SetCrossSectionHist(CStemp);
      delete fsin;
    }

    if(blocks[i]->HasToken("DoubleDifferentialCrossSectionPath")){
      vector<string> file = blocks[i]->GetVectorString("DoubleDifferentialCrossSectionPath");
      TH2F* CStemp = Read2DProfile(file[0],file[1]);

      // multiply CStemp by sin(theta)
      // X axis is theta CM
      // Y axis is beam energy
      // Division affect only X axis
      TF1* fsin = new TF1("sin",Form("1/(sin(x*%f/180.))",M_PI),0,180);
      CStemp->Divide(fsin,1);

      SetDoubleDifferentialCrossSectionHist(CStemp);
      delete fsin;
    }

    if(blocks[i]->HasToken("HalfOpenAngleMin")){
      CSHalfOpenAngleMin = blocks[i]->GetDouble("HalfOpenAngleMin","deg");
    }
    if(blocks[i]->HasToken("HalfOpenAngleMax")){
      CSHalfOpenAngleMax = blocks[i]->GetDouble("HalfOpenAngleMax","deg");
    }
    if(blocks[i]->HasToken("Shoot3")){
      fshoot3 = blocks[i]->GetInt("Shoot3");
    } 
    if(blocks[i]->HasToken("Shoot4")){
      fshoot4 = blocks[i]->GetInt("Shoot4");
    } 
    if(blocks[i]->HasToken("ShootHeavy")){
      fshoot4 = blocks[i]->GetInt("ShootHeavy");
    } 
    if(blocks[i]->HasToken("ShootLight")){
      fshoot3 = blocks[i]->GetInt("ShootLight");
    }
  }
  SetCSAngle(CSHalfOpenAngleMin,CSHalfOpenAngleMax);
  initializePrecomputeVariable();
  cout << "\033[0m" ;
}

////////////////////////////////////////////////////////////////////////////////////////////
void Reaction::initializePrecomputeVariable(){

  if(fBeamEnergy < 0)
    fBeamEnergy = 0 ;

  m1 = fNuclei1.Mass();
  m2 = fNuclei2.Mass();
  m3 = fNuclei3.Mass() + fExcitation3;
  m4 = fNuclei4.Mass() + fExcitation4;
  fQValue =m1+m2-m3-m4;

  s = m1*m1 + m2*m2 + 2*m2*(fBeamEnergy + m1);
  fTotalEnergyImpulsionCM = TLorentzVector(0,0,0,sqrt(s));

  ECM_1 = (s + m1*m1 - m2*m2)/(2*sqrt(s));
  ECM_2 = (s + m2*m2 - m1*m1)/(2*sqrt(s));
  ECM_3 = (s + m3*m3 - m4*m4)/(2*sqrt(s));
  ECM_4 = (s + m4*m4 - m3*m3)/(2*sqrt(s));

  pCM_1 = sqrt(ECM_1*ECM_1 - m1*m1);
  pCM_2 = sqrt(ECM_2*ECM_2 - m2*m2);
  pCM_3 = sqrt(ECM_3*ECM_3 - m3*m3);
  pCM_4 = sqrt(ECM_4*ECM_4 - m4*m4);

  fImpulsionLab_1 = TVector3(0,0,sqrt(fBeamEnergy*fBeamEnergy + 2*fBeamEnergy*m1));
  fImpulsionLab_2 = TVector3(0,0,0);

  fEnergyImpulsionLab_1 = TLorentzVector(fImpulsionLab_1,m1+fBeamEnergy);
  fEnergyImpulsionLab_2 = TLorentzVector(fImpulsionLab_2,m2);

  fTotalEnergyImpulsionLab = fEnergyImpulsionLab_1 + fEnergyImpulsionLab_2;

  BetaCM = fTotalEnergyImpulsionLab.Beta();

  fEnergyImpulsionCM_1 = fEnergyImpulsionLab_1;
  fEnergyImpulsionCM_1.Boost(0,0,-BetaCM);

  fEnergyImpulsionCM_2 = fEnergyImpulsionLab_2;
  fEnergyImpulsionCM_2.Boost(0,0,-BetaCM);
}

////////////////////////////////////////////////////////////////////////////////////////////
void Reaction::SetNuclei3(double EnergyLab, double ThetaLab){
  double p3 = sqrt(pow(EnergyLab,2) + 2*m3*EnergyLab);

  fEnergyImpulsionLab_3 = TLorentzVector(p3*sin(ThetaLab),0,p3*cos(ThetaLab),EnergyLab+m3);
  fEnergyImpulsionLab_4 = fTotalEnergyImpulsionLab - fEnergyImpulsionLab_3;

  fNuclei3.SetEnergyImpulsion(fEnergyImpulsionLab_3);
  fNuclei4.SetEnergyImpulsion(fEnergyImpulsionLab_4);

  fThetaCM = EnergyLabToThetaCM(EnergyLab, ThetaLab);
  fExcitation4 = ReconstructRelativistic(EnergyLab, ThetaLab);
}

////////////////////////////////////////////////////////////////////////////////////////////
TGraph* Reaction::GetKinematicLine3(double AngleStep_CM){

  vector<double> vx;
  vector<double> vy;
  double theta3,E3,theta4,E4;

  for (double angle=0 ; angle < 360 ; angle+=AngleStep_CM){
    SetThetaCM(angle*deg);
    KineRelativistic(theta3, E3, theta4, E4);
    fNuclei3.SetKineticEnergy(E3);

    if(E3>0){
      vx.push_back(theta3/deg);
      vy.push_back(E3);
    }
  }
  fKineLine3 = new TGraph(vx.size(),&vx[0],&vy[0]);

  return(fKineLine3);
}

////////////////////////////////////////////////////////////////////////////////////////////
TGraph* Reaction::GetKinematicLine4(double AngleStep_CM){

  vector<double> vx;
  vector<double> vy;
  double theta3,E3,theta4,E4;

  for (double angle=0 ; angle < 360 ; angle+=AngleStep_CM){
    SetThetaCM(angle*deg);
    KineRelativistic(theta3, E3, theta4, E4);
    fNuclei4.SetKineticEnergy(E4);
    if(E4>0){
      vx.push_back(theta4/deg);
      vy.push_back(E4);
    }
  }
  fKineLine4= new TGraph(vx.size(),&vx[0],&vy[0]);

  return(fKineLine4);
}

////////////////////////////////////////////////////////////////////////////////////////////   
TGraph* Reaction::GetTheta3VsTheta4(double AngleStep_CM)
{	

  vector<double> vx;
  vector<double> vy;
  double theta3,E3,theta4,E4;

  for (double angle=0 ; angle < 360 ; angle+=AngleStep_CM){
    SetThetaCM(angle*deg); 
    KineRelativistic(theta3, E3, theta4, E4);

    vx.push_back(theta3/deg); 
    vy.push_back(theta4/deg); 
  }
  fTheta3VsTheta4= new TGraph(vx.size(),&vx[0],&vy[0]);
  return(fTheta3VsTheta4);
}

////////////////////////////////////////////////////////////////////////////////////////////
TGraph* Reaction::GetBrhoLine3(double AngleStep_CM){

  vector<double> vx;
  vector<double> vy;
  double theta3,E3,theta4,E4;
  double Brho;

  for (double angle=0 ; angle < 360 ; angle+=AngleStep_CM){
    SetThetaCM(angle*deg);
    KineRelativistic(theta3, E3, theta4, E4);
    fNuclei3.SetKineticEnergy(E3);
    Brho = fNuclei3.GetBrho();

    vx.push_back(theta3/deg);
    vy.push_back(Brho);
  }
  fLineBrho3= new TGraph(vx.size(),&vx[0],&vy[0]);
  return(fLineBrho3);
}

////////////////////////////////////////////////////////////////////////////////////////////
TGraph* Reaction::GetThetaLabVersusThetaCM(double AngleStep_CM){

  vector<double> vx;
  vector<double> vy;
  double theta3,E3,theta4,E4;

  for (double angle=0 ; angle < 360 ; angle+=AngleStep_CM){
    SetThetaCM(angle*deg);
    KineRelativistic(theta3, E3, theta4, E4);

    vx.push_back(fThetaCM/deg);
    vy.push_back(theta3/deg);
  }

  fAngleLine= new TGraph(vx.size(),&vx[0],&vy[0]);
  return(fAngleLine);
}
////////////////////////////////////////////////////////////////////////////////////////////
TGraph* Reaction::GetELabVersusThetaCM(double AngleStep_CM){

  vector<double> vx;
  vector<double> vy;
  double theta3,E3,theta4,E4;

  for (double angle=0 ; angle < 360 ; angle+=AngleStep_CM){
    SetThetaCM(angle*deg);
    KineRelativistic(theta3, E3, theta4, E4);

    vx.push_back(E3);
    vy.push_back(fThetaCM/deg);
  }

  fAngleLine= new TGraph(vx.size(),&vx[0],&vy[0]);
  return(fAngleLine);
}


////////////////////////////////////////////////////////////////////////////////////////////
Double_t Reaction::GetTotalCrossSection() const {
  Double_t stot = fCrossSectionHist->Integral("width"); // take bin width into account (in deg!)
  stot *= M_PI/180; // correct so that bin width is in rad
  stot *= 2*M_PI;   // integration over phi

  return stot;
}



////////////////////////////////////////////////////////////////////////////////////////////
void Reaction::PrintKinematic(){
  int size = 360;
  double theta3,E3,theta4,E4,Brho3,Brho4;

  cout << endl;
  cout << "*********************** Print Kinematic ***********************" << endl;
  cout << "ThetaCM" << "	" << "ThetaLab" << " " << "EnergyLab3" << "	" << "Brho3" << "	" << "EnergyLab4" << "	" << "Brho4" << endl;
  for (int i = 0; i < size; ++i){
    SetThetaCM(((double)i)/2*deg);
    KineRelativistic(theta3, E3, theta4, E4);

    fNuclei3.SetKineticEnergy(E3);
    Brho3 = fNuclei3.GetBrho();

    fNuclei4.SetKineticEnergy(E4);
    Brho4 = fNuclei4.GetBrho();

    cout << (double)i/2 << "	" << theta3/deg << "	" << E3 << "	" << Brho3 << "		" << E4 << "	" << Brho4 << endl;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
void Reaction::SetCSAngle(double CSHalfOpenAngleMin,double CSHalfOpenAngleMax){
  if(fCrossSectionHist){
    for (int i = 0 ; i< fCrossSectionHist->GetNbinsX(); i++)
      if( fCrossSectionHist->GetBinCenter(i) > CSHalfOpenAngleMax && fCrossSectionHist->GetBinCenter(i) < CSHalfOpenAngleMin)
        fCrossSectionHist->SetBinContent(i,0);
  }
}

