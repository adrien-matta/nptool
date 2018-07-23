/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author : Adrien Matta  contact: matta@lpccaen.in2p3.fr           *
 *                                                                           *
 * Creation Date   : Octobre 2017                                            *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *   This Class hold data for all decay scheme of a given nuclei             *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "NPDecay.h"
#include <iostream>
#include "NPOptionManager.h"
#include "NPFunction.h"
#include "NPCore.h"
#include "TF1.h"
#include "TRandom.h"
////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Single Decay //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

NPL::SingleDecay::SingleDecay(std::string mother, double threshold, std::vector<std::string> daughter,std::vector<double> Ex, TH1F* CrossSection){

  m_MotherName = mother;
  m_DaughterName = daughter;
  // Checking if there is a mixt of Gamma and particle
  bool hasGamma=false;
  bool hasOther=false;
  unsigned int sizeD = daughter.size();
  for(unsigned int d = 0 ; d < sizeD ; d++){
    if(daughter[d]=="Gamma"|| daughter[d]=="gamma")
      hasGamma=true;
    else if (daughter[d]!=m_MotherName)
      hasOther=true;
  }

  // If the same channel contain both, issue an error and exit
  if(hasOther&&hasGamma){
    NPL::SendErrorAndExit("NPL::SingleDecay","Cannot have particle/gamma mixt decay channel");
  }
  else if (hasGamma)
    m_GammaOnly=true;
  else
    m_GammaOnly=false;

  m_ExcitationEnergies = Ex;
  m_CrossSectionHist = CrossSection;
  m_Mother = NPL::Nucleus(m_MotherName);
  m_MotherMass = m_Mother.Mass();
  m_Threshold = threshold;
  unsigned int size = m_DaughterName.size();
  for(unsigned int i = 0 ; i < size ; i++){
    m_Daughter.push_back(NPL::Nucleus(m_DaughterName[i])) ;
    
    m_Daughter[i].SetExcitationEnergy(Ex[i]);
    if (m_DaughterName[i]=="Gamma" || m_DaughterName[i]=="gamma")
      m_DaughterMasses.push_back(0);
    else
      m_DaughterMasses.push_back(m_Daughter[i].Mass()/GeV);

  }

}
////////////////////////////////////////////////////////////////////////////////
bool NPL::SingleDecay::GenerateEvent(double MEx, double MEK, double MPX, double MPY, double MPZ, std::vector<double>& DEK, std::vector<double>& DPx, std::vector<double>& DPy, std::vector<double>& DPz){
  // Clear the output object
  DPx.clear();
  DPy.clear();
  DPz.clear();
  DEK.clear();
  if(MEx < m_Threshold)
    return false;

  double Effective_Mass= m_MotherMass+MEx;
  double NucleiEnergy= MEK+Effective_Mass;

  double NucleiMomentum=
    sqrt(NucleiEnergy*NucleiEnergy 
        - Effective_Mass*Effective_Mass);
  TVector3 Momentum(MPX,MPY,MPZ);
  Momentum = Momentum.Unit();

  if(m_CrossSectionHist){
    TLorentzVector NucleiLV( NucleiMomentum*Momentum.X(),
        NucleiMomentum*Momentum.Y(),
        NucleiMomentum*Momentum.Z(),
        NucleiEnergy);
    // Shoot the angle in Center of Mass (CM) frame
    double ThetaCM = m_CrossSectionHist->GetRandom()* deg;
    double phi     = gRandom->Uniform()*2.*pi;

    // Build daughter particule CM LV
    // Pre compute variable for the decay
    double m1 = m_DaughterMasses[0]*GeV;
    double m2 = m_DaughterMasses[1]*GeV;
    if(Effective_Mass<(m1+m2))
      return false;

    else {
      double Energy = ( 1./(2.*Effective_Mass) )*(Effective_Mass*Effective_Mass + m1*m1 - m2*m2);
      double Momentum1 = sqrt(Energy*Energy - m1*m1);

      TVector3 FirstDaughterMomentum = Momentum1 * TVector3( sin(ThetaCM) * cos(phi),
          sin(ThetaCM) * sin(phi),
          cos(ThetaCM));

      TLorentzVector FirstDaughterLV(FirstDaughterMomentum,Energy);

      FirstDaughterLV.Boost( NucleiLV.BoostVector() );
      TLorentzVector SecondDaughterLV = NucleiLV - FirstDaughterLV;

      DEK.push_back(FirstDaughterLV.E()-m1);
      DPx.push_back(FirstDaughterLV.X());
      DPy.push_back(FirstDaughterLV.Y());
      DPz.push_back(FirstDaughterLV.Z());

      DEK.push_back(SecondDaughterLV.E()-m2);
      DPx.push_back(SecondDaughterLV.X());
      DPy.push_back(SecondDaughterLV.Y());
      DPz.push_back(SecondDaughterLV.Z());
      return true;
    }
  }

  // Case of a TGenPhaseSpace
  else if (!m_GammaOnly){
    // TGenPhaseSpace require to input Energy and Momentum in GeV
    TLorentzVector NucleiLV( NucleiMomentum*Momentum.x()/GeV,
        NucleiMomentum*Momentum.y()/GeV,
        NucleiMomentum*Momentum.z()/GeV,
        NucleiEnergy/GeV);

    if( !m_TGenPhaseSpace.SetDecay(NucleiLV, m_Daughter.size(), &m_DaughterMasses[0]) )
      return false;

    else{
      // FIXME : weight has to be returned for normalisation
      double weight = m_TGenPhaseSpace.Generate();

      TLorentzVector* DaughterLV ;
      double KineticEnergy;

      for (unsigned int i = 0 ;  i < m_Daughter.size(); i++) {
        DaughterLV = m_TGenPhaseSpace.GetDecay(i);
        DEK.push_back(GeV*DaughterLV->E()-GeV*m_DaughterMasses[i]);
        DPx.push_back(GeV*DaughterLV->X());
        DPy.push_back(GeV*DaughterLV->Y());
        DPz.push_back(GeV*DaughterLV->Z());
      }
      return true;
    }
  }

  // Case of a Gamma Cascade
  else{
    TLorentzVector NucleiLV( NucleiMomentum*Momentum.X(),
        NucleiMomentum*Momentum.Y(),
        NucleiMomentum*Momentum.Z(),
        NucleiEnergy);

    TLorentzVector TotalGamma(0,0,0,0);
    unsigned int pos;

    unsigned int sizeM = m_DaughterMasses.size();
    for (unsigned int i = 0; i < sizeM; i++) {
      // One of the Gamma
      if(m_DaughterName[i]=="gamma"|| m_DaughterName[i]=="Gamma"){
         // Shoot flat in cos(theta) and Phi to have isotropic emission
        double cos_theta = -1+2*gRandom->Uniform();
        double theta     = acos(cos_theta);
        double phi       = gRandom->Uniform()*2.*pi;
        double gammaEnergy = m_ExcitationEnergies[i];
        TLorentzVector GammaLV( gammaEnergy*cos(phi)*sin(theta),
          gammaEnergy*sin(phi)*sin(theta),
          gammaEnergy*cos(theta),
          gammaEnergy);
        
       GammaLV.Boost(NucleiLV.BoostVector());
        TotalGamma+=GammaLV; 
        DEK.push_back(GammaLV.E());
        DPx.push_back(GammaLV.X());
        DPy.push_back(GammaLV.Y());
        DPz.push_back(GammaLV.Z());

        }
     else{
        pos = i;
        DEK.push_back(0);
        DPx.push_back(0);
        DPy.push_back(0);
        DPz.push_back(0);
       }
    }

    // The original nuclei get what is left
   NucleiLV -= TotalGamma;
   DEK[pos]=NucleiLV.E()-m_MotherMass;
   DPx[pos]=NucleiLV.X();
   DPy[pos]=NucleiLV.Y();
   DPz[pos]=NucleiLV.Z();
   return true;
  }
  return false;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Decay ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
NPL::Decay::Decay(std::string MotherName,std::string path){
  ReadConfiguration(MotherName,path);
}

////////////////////////////////////////////////////////////////////////////////
NPL::Decay::Decay(std::string MotherName, NPL::InputParser parser){
  ReadConfiguration(MotherName,parser);     
}

////////////////////////////////////////////////////////////////////////////////
void NPL::Decay::ReadConfiguration(std::string MotherName,std::string path){
  NPL::InputParser parser(path);
  ReadConfiguration(MotherName,parser);
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::Decay::AnyAboveThreshold(double MEx){
  unsigned int size = m_SingleDecay.size();
  for(unsigned int i = 0 ; i < size ; i++){
    if(MEx >=  m_SingleDecay[i].GetThreshold())
      return true;
  }
  return false;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::Decay::ReadConfiguration(std::string MotherName, NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithTokenAndValue("Decay",MotherName);
  m_BRTotal=0;
  m_MotherName=MotherName;

  if(NPOptionManager::getInstance()->GetVerboseLevel())
    std::cout << "//// " << blocks.size() << " decay path found for " << MotherName << std::endl; 

  std::vector<std::string> token = 
  {"Daughter","Threshold","ExcitationEnergy","Shoot","LifeTime","BranchingRatio"};
  std::vector<std::string> CStoken = 
  {"DifferentialCrossSection"};

  unsigned int size = blocks.size();
  for(unsigned int i = 0 ; i < size ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "//// "<< MotherName << " Decay " << i+1 <<  endl;

      std::vector<std::string> Daughter = blocks[i]->GetVectorString("Daughter");

      double Threshold = blocks[i]->GetDouble("Threshold","MeV");   
      std::vector<double> Ex = blocks[i]->GetVectorDouble("ExcitationEnergy","MeV");
      double BR = blocks[i]->GetDouble("BranchingRatio","void");
      m_BranchingRatio.push_back(BR);
      m_BRTotal+= BR;
      double LT = blocks[i]->GetDouble("LifeTime","ns");
      m_Shoot = blocks[i]->GetVectorInt("Shoot");

      TH1F* h = NULL; 
      std::vector<std::string> cs ;


      if(blocks[i]->HasTokenList(CStoken)){
        if(Ex.size()!=2){
          std::cout << "ERROR : Differential cross section is only used for two body decay" << std::endl;
          exit(1);
        }
        cs = blocks[i]->GetVectorString("DifferentialCrossSection");
      }

      // Load the Cross section  
      if(cs.size()==2){
        h = NPL::Read1DProfile(cs[0],cs[1]);
        TF1* sinus = new TF1("sinus","1/sin(x*3.141592653589793/180.)",0,180);
        h->Divide(sinus,1);
        delete sinus;
      }
      else if (cs.size()!=0){
        std::cout << "ERROR : You should provide 2 arguments for the cross section : path and name" << std::endl;
        exit(1);
      }

      m_SingleDecay.push_back(SingleDecay(m_MotherName,Threshold,Daughter,Ex,h));
    }

    else{
      cout << "ERROR: check your input file formatting" << endl;
      exit(1);
    }

  }
  // Shift the Branching ratio for faster shooting during event generation
  for (unsigned int i = 1; i < m_BranchingRatio.size(); i++) {
    m_BranchingRatio[i] = m_BranchingRatio[i-1]+m_BranchingRatio[i];
  } 
}


////////////////////////////////////////////////////////////////////////////////
bool NPL::Decay::GenerateEvent(double MEx,double MEK,double MPx,double MPy,double MPz, 
    std::vector<NPL::Nucleus>& Daughter, std::vector<double>& Ex,
    std::vector<double>& DEK,
    std::vector<double>& DPx,std::vector<double>& DPy,std::vector<double>& DPz){
  // Choose one of the Decay
  // if the decay is not allowed, then try again
  // FIXME: does take into account case where the decay is never possible
  bool worked = false;
  // Limit the number of attempt
  unsigned int count =0;
  while (!worked){
    if(count++ > 1000)
      break;
    double rand = m_BRTotal*gRandom->Uniform();
    unsigned int size = m_BranchingRatio.size();
    unsigned int ChoosenDecay = 0;
    for (unsigned int i = 1; i < size; i++) {
      if(rand > m_BranchingRatio[i-1] && rand< m_BranchingRatio[i])
        ChoosenDecay = i;
    }

    // Generate the event
    worked = m_SingleDecay[ChoosenDecay].GenerateEvent(MEx,MEK,MPx,MPy,MPz,DEK,DPx,DPy,DPz);
    if(worked){
      Daughter = m_SingleDecay[ChoosenDecay].GetDaughter();
      Ex = m_SingleDecay[ChoosenDecay].GetExcitationEnergies();  
    }
  }

  return worked;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Decay Store ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
NPL::DecayStore::DecayStore(std::string path){
  ReadConfiguration(path);  
}
////////////////////////////////////////////////////////////////////////////////
NPL::DecayStore::DecayStore(NPL::InputParser parser){
  ReadConfiguration(parser);  
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DecayStore::ReadConfiguration(std::string path){
  NPL::InputParser parser(path);  
  ReadConfiguration(parser);
}  
////////////////////////////////////////////////////////////////////////////////
void NPL::DecayStore::ReadConfiguration(NPL::InputParser parser){
  // Get all the Nucleus concern by a decay
  std::vector<std::string> nuclei = parser.GetAllBlocksValues("Decay");
  m_Store.clear();
  unsigned int size = nuclei.size();
  for(unsigned int i = 0 ; i < size ; i++){
    if(m_Store.find(nuclei[i])==m_Store.end()){
      m_Store[nuclei[i]] = Decay(nuclei[i],parser);
    } 
  }
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DecayStore::GenerateEvent(std::string MotherName, double MEx,
    double MEK,double MPx,double MPy,double MPz,
    std::vector<NPL::Nucleus>& Daughter, std::vector<double>& Ex,
    std::vector<double>& DEK,
    std::vector<double>& DPx,std::vector<double>& DPy,std::vector<double>& DPz){

  if(m_Store.find(MotherName)!=m_Store.end())
    m_Store[MotherName].GenerateEvent(MEx,MEK,MPx,MPy,MPz,Daughter,Ex,DEK,DPx,DPy,DPz);  
  else
    cout << "Warning: Decay for " << MotherName << " requested but not found" << endl;
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::DecayStore::AnyAboveThreshold(std::string MotherName, double MEx){
  if(m_Store.find(MotherName)!=m_Store.end()){
    m_Store[MotherName].AnyAboveThreshold(MEx);
    return true;
  }
    
  return false;
}
////////////////////////////////////////////////////////////////////////////////
std::set<std::string> NPL::DecayStore::GetAllMotherName(){
  std::map<std::string,NPL::Decay>::iterator it;
  std::set<std::string> result;
  for(it = m_Store.begin(); it != m_Store.end() ; it++)
    result.insert(it->first);   

  return result; 
} 


































