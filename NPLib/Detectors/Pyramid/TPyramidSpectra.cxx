/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: mohamad.moukaddam@iphc.cnrs.fr                           *
 *                                                                           *
 * Creation Date  : November 2018                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Pyramid Spectra                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
using namespace std;

// NPL
#include "TPyramidSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif


////////////////////////////////////////////////////////////////////////////////
TPyramidSpectra::TPyramidSpectra(){
  SetName("Pyramid");
  fNumberOfDetector= 0;
  fInnerStrip=4;
  fOuterStrip=4;
}

////////////////////////////////////////////////////////////////////////////////
TPyramidSpectra::TPyramidSpectra(unsigned int NumberOfDetector){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TPyramidSpectra : Initalising control spectra for " 
      << NumberOfDetector << " Detectors" << endl
      << "************************************************" << endl ;

  SetName("Pyramid");
  fNumberOfDetector= NumberOfDetector;
  fInnerStrip=4;
  fOuterStrip=4;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TPyramidSpectra::~TPyramidSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TPyramidSpectra::InitRawSpectra(){
  string name;
  string BaseFamily = "Pyramid/RAW/";

  //// HIT ////
  // Inner 
  // Hit Up Stream
  name = "INNER_US_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector*fInnerStrip, 1, fNumberOfDetector*fInnerStrip+1, BaseFamily+"HIT");
  //Hit Down Stream
  name = "INNER_DS_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector*fInnerStrip, 1, fNumberOfDetector*fInnerStrip+1, BaseFamily+"HIT");
  // Hit Back
  name = "INNER_BACK_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, BaseFamily+"HIT");

  // Outer 
  //Hit Strip
  name = "OUTER_STRIP_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector*fOuterStrip, 1, fNumberOfDetector*fOuterStrip+1, BaseFamily+"HIT");

  //// E ////
  // Inner 
  // E Up Stream
  name = "INNER_US_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fInnerStrip, 1, fNumberOfDetector*fInnerStrip+1, 512,0,16384,BaseFamily+"E");
  //E Down Stream
  name = "INNER_DS_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fInnerStrip, 1, fNumberOfDetector*fInnerStrip+1,512,0,16384, BaseFamily+"E");
  //E Back
  name = "INNER_BACK_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1,512,0,16384, BaseFamily+"E");

  // Outer 
  //E Strip
  name = "OUTER_STRIP_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fOuterStrip, 1, fNumberOfDetector*fOuterStrip+1,512,0,16384, BaseFamily+"E");

  //// VS ////
  // Inner 
  for(unsigned int i  = 0 ; i < fNumberOfDetector ; i++){
    for(unsigned int j = 0 ; j < fInnerStrip;j++){
      name = "IB"+NPL::itoa(i+1)+"_VS"+NPL::itoa(j+1)+"_RAW";
      AddHisto2D(name, name,1024,0,0,1024,0,0,BaseFamily+"VS");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void TPyramidSpectra::InitPreTreatedSpectra(){
  string BaseFamily = "Pyramid/CAL/";
  string name ;
  //// VS ////
  // Inner 
  for(unsigned int i  = 0 ; i < fNumberOfDetector ; i++){
    for(unsigned int j = 0 ; j < fInnerStrip;j++){
      name = "IB"+NPL::itoa(i+1)+"_VS"+NPL::itoa(j+1)+"_CAL";
      AddHisto2D(name,name,2048,0,0,2048,0,0,BaseFamily+"VS");
    }
    name = "IB"+NPL::itoa(i+1)+"_VS_BACK_CAL"; 
    AddHisto2D(name,name,2048,0,0,2048,0,0,BaseFamily+"VS");
  }
}

////////////////////////////////////////////////////////////////////////////////
void TPyramidSpectra::InitPhysicsSpectra(){
string BaseFamily = "Pyramid/PHY";
string name ;
  //// E POS ////
  // Inner 
  for(unsigned int i  = 0 ; i < fNumberOfDetector ; i++){
    for(unsigned int j = 0 ; j < fInnerStrip;j++){
      name = "IB"+NPL::itoa(i+1)+"_EPOS"+NPL::itoa(j+1)+"_CAL";
      AddHisto2D(name, name,1000,-0.5,1.5,1000,0,30,BaseFamily);
    }
  }
  //// E Theta ////
  // Inner 
  for(unsigned int i  = 0 ; i < fNumberOfDetector ; i++){
    for(unsigned int j = 0 ; j < fInnerStrip;j++){
      name = "IB"+NPL::itoa(i+1)+"_ETHETA"+NPL::itoa(j+1)+"_CAL";
      AddHisto2D(name, name,360,0,180,1000,0,30,BaseFamily);
    }
  }
  //// E Theta ////
  // Inner 
 name = "IB_ETHETA_CAL";
 AddHisto2D(name, name,360,0,180,1000,0,30,BaseFamily);

}

////////////////////////////////////////////////////////////////////////////////
void TPyramidSpectra::FillRawSpectra(TPyramidData* RawData){
  string name;
  string family;
  string BaseFamily = "Pyramid/RAW/";

  // INNER_US_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetFrontUpstreamEMult(); i++) {
    name = "INNER_US_HIT_RAW";
    family = BaseFamily+"HIT";

    FillSpectra(family,name
      ,(RawData->GetFrontUpstreamEDetectorNbr(i)-1)*fInnerStrip
          +RawData->GetFrontUpstreamEStripNbr(i));

    family = BaseFamily+"E";
    name = "INNER_US_E_RAW";
    FillSpectra(family,name
      ,(RawData->GetFrontUpstreamEDetectorNbr(i)-1)*fInnerStrip
          +RawData->GetFrontUpstreamEStripNbr(i),RawData->GetFrontUpstreamEEnergy(i));
  }
  // INNER_DS_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetFrontDownstreamEMult(); i++) {
    name = "INNER_DS_HIT_RAW";
    family = BaseFamily+"HIT";

    FillSpectra(family,name
      ,(RawData->GetFrontDownstreamEDetectorNbr(i)-1)*fInnerStrip
          +RawData->GetFrontDownstreamEStripNbr(i));

    name = "INNER_DS_E_RAW";
    family = BaseFamily+"E";
    FillSpectra(family,name
      ,(RawData->GetFrontDownstreamEDetectorNbr(i)-1)*fInnerStrip
          +RawData->GetFrontDownstreamEStripNbr(i),RawData->GetFrontDownstreamEEnergy(i));
  }

  // INNER_BACK_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetBackEMult(); i++) {
    name = "INNER_BACK_HIT_RAW";
    family = BaseFamily+"HIT";

    FillSpectra(family,name
      ,(RawData->GetBackEDetectorNbr(i)-1));

    name = "INNER_BACK_E_RAW";
    family = BaseFamily+"E";
    FillSpectra(family,name
      ,(RawData->GetBackEDetectorNbr(i)-1), RawData->GetBackEEnergy(i));
  }

  // OUTER_STRIP_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetOuterEMult(); i++) {
    name = "OUTER_STRIP_HIT_RAW";
    family = BaseFamily+"HIT";

    FillSpectra(family,name
      ,(RawData->GetOuterEDetectorNbr(i)-1)*fInnerStrip
          +RawData->GetOuterEStripNbr(i));
    
    family = BaseFamily+"E";
    name = "OUTER_STRIP_E_RAW";
    FillSpectra(family,name
      ,(RawData->GetOuterEDetectorNbr(i)-1)*fInnerStrip
          +RawData->GetOuterEStripNbr(i),RawData->GetOuterEEnergy(i));
  }
  
  // INNER_VS_RAW                    
  family = BaseFamily+"VS";
  for (unsigned int i = 0; i < RawData->GetFrontUpstreamEMult(); i++) {
    int UpStreamDetNbr = RawData->GetFrontUpstreamEDetectorNbr(i);
    int UpStreamStrNbr = RawData->GetFrontUpstreamEStripNbr(i);
    
    for (unsigned int j = 0; j < RawData->GetFrontDownstreamEMult(); j++) {
      int DoStreamDetNbr = RawData->GetFrontDownstreamEDetectorNbr(j);
      int DoStreamStrNbr = RawData->GetFrontDownstreamEStripNbr(j);
     if(UpStreamDetNbr==DoStreamDetNbr && UpStreamStrNbr==DoStreamStrNbr){
       name = "IB"+NPL::itoa(UpStreamDetNbr)+"_VS"+NPL::itoa(UpStreamStrNbr)+"_RAW"; 
       FillSpectra(family,name
        ,RawData->GetFrontUpstreamEEnergy(i),RawData->GetFrontDownstreamEEnergy(j));
      } 
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void TPyramidSpectra::FillPreTreatedSpectra(TPyramidData* PreTreatedData){
string BaseFamily = "Pyramid/CAL/";
// INNER_VS_CAL                 
  string family = BaseFamily+"VS";
  string name ;

  unsigned int sizeU = PreTreatedData->GetFrontUpstreamEMult();
  unsigned int sizeD = PreTreatedData->GetFrontDownstreamEMult();
  unsigned int sizeB = PreTreatedData->GetBackEMult(); 
  
  for (unsigned int i = 0; i < sizeU ; i++) {
    int UpStreamDetNbr = PreTreatedData->GetFrontUpstreamEDetectorNbr(i);
    int UpStreamStrNbr = PreTreatedData->GetFrontUpstreamEStripNbr(i);
    
    for (unsigned int j = 0; j < sizeD ; j++) {
      int DoStreamDetNbr = PreTreatedData->GetFrontDownstreamEDetectorNbr(j);
      int DoStreamStrNbr = PreTreatedData->GetFrontDownstreamEStripNbr(j);
     if(UpStreamDetNbr==DoStreamDetNbr && UpStreamStrNbr==DoStreamStrNbr){
      name = "IB"+NPL::itoa(UpStreamDetNbr)+"_VS"+NPL::itoa(UpStreamStrNbr)+"_CAL"; 
       FillSpectra(family,name
         ,PreTreatedData->GetFrontUpstreamEEnergy(i),PreTreatedData->GetFrontDownstreamEEnergy(j));
     
        for (unsigned int k = 0; k < sizeB; k++) {
          if(UpStreamDetNbr == PreTreatedData->GetBackEDetectorNbr(k)){     
            name = "IB"+NPL::itoa(UpStreamDetNbr)+"_VS_BACK_CAL"; 
            FillSpectra(family,name
              ,PreTreatedData->GetFrontUpstreamEEnergy(i)
                      +PreTreatedData->GetFrontDownstreamEEnergy(j),
                      PreTreatedData->GetBackEEnergy(k));
          }
        }
      } 
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void TPyramidSpectra::FillPhysicsSpectra(TPyramidPhysics* Physics){
  string family = "Pyramid/PHY";
  string name ;
  //// E POS ////
  // Inner 
  //unsigned int size = Physics->Strip_E.size();
  for(unsigned int i  = 0 ; i < Physics->Strip_E.size() ; i++){
    name ="IB"+NPL::itoa(Physics->Detector_N[i])+"_EPOS"+NPL::itoa(Physics->Strip_N[i])+"_CAL";
    FillSpectra(family,name
      ,Physics->Strip_Pos[i],Physics->Strip_E[i]);

    name = "IB"+NPL::itoa(Physics->Detector_N[i])+"_ETHETA"+NPL::itoa(Physics->Strip_N[i])+"_CAL";
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));

    FillSpectra(family,name
      ,Theta*rad/deg,Physics->Strip_E[i]);

    name = "IB_ETHETA_CAL";
    FillSpectra(family,name
      ,Theta*rad/deg,Physics->Strip_E[i]);
  }
  
  // Energy vs time
  /*unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "Pyramid_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
  */
}

