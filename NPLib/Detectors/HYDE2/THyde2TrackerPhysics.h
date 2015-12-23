/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 31/08/09                                                 *
 * Last update    : 08/10/10                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the tracker part of the Hyde2 detector.                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 08/10/10: Change private member names and add setters and getters    *
 *                methods. BuildPhysicalEvent() method is moved to the       *
 *                THyde2Tracker*** classes (N. de Sereville)               *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef THYDE2TRACKERPHYSICS_H
#define THYDE2TRACKERPHYSICS_H

#include "THyde2TrackerData.h"
#include "NPVDetector.h"
// ROOT
#include "TObject.h"

// C++
#include <vector>

using namespace std ;


class THyde2TrackerPhysics : public TObject, public NPL::VDetector
{
public:
   THyde2TrackerPhysics();
   ~THyde2TrackerPhysics();

public: 
   void Clear();   
   void Clear(const Option_t*) {};


private:
   vector<Int_t>   fEventType;
   vector<Int_t>   fModuleNumber;

   // First stage
   vector<Double_t>  fFirstStage_Energy;
   vector<Double_t>  fFirstStage_Time;
   vector<Int_t>     fFirstStage_FrontPosition;
   vector<Int_t>     fFirstStage_BackPosition;

   // Second stage
   vector<Double_t>  fSecondStage_Energy;
   vector<Double_t>  fSecondStage_Time;
   vector<Int_t>     fSecondStage_Position;

   // Third stage
   vector<Double_t>  fThirdStage_Energy;
   vector<Double_t>  fThirdStage_Time;
   vector<Int_t>     fThirdStage_Position;

   // Fourth stage
   vector<Double_t>  fFourthStage_Energy;
   vector<Double_t>  fFourthStage_Time;
   vector<Int_t>     fFourthStage_Position;

   // Fifth stage
   vector<Double_t>  fFifthStage_Energy;
   vector<Double_t>  fFifthStage_Time;
   vector<Int_t>     fFifthStage_Position;

   // Sixth stage
   vector<Double_t>  fSixthStage_Energy;
   vector<Double_t>  fSixthStage_Time;
   vector<Int_t>     fSixthStage_Position;

   vector<Double_t>  fTotalEnergy;


public:
   /////////////////////////////////////
   //            SETTERS              //
   /////////////////////////////////////
   // General
   void SetEventType(Int_t evtType)      {fEventType.push_back(evtType);}
   void SetModuleNumber(Int_t moduleNbr)   {fModuleNumber.push_back(moduleNbr);}
   void SetTotalEnergy(Double_t ener)      {fTotalEnergy.push_back(ener);}

   // First stage
   void SetFirstStageEnergy(Double_t ener)   {fFirstStage_Energy.push_back(ener);}
   void SetFirstStageTime(Double_t time)   {fFirstStage_Time.push_back(time);}
   void SetFirstStageFrontPosition(Int_t x)   {fFirstStage_FrontPosition.push_back(x);}
   void SetFirstStageBackPosition(Int_t y)   {fFirstStage_BackPosition.push_back(y);}

   // Second stage
   void SetSecondStageEnergy(Double_t ener)   {fSecondStage_Energy.push_back(ener);}
   void SetSecondStageTime(Double_t time)   {fSecondStage_Time.push_back(time);}
   void SetSecondStagePosition(Int_t pos)   {fSecondStage_Position.push_back(pos);}

   // Third stage
   void SetThirdStageEnergy(Double_t ener)   {fThirdStage_Energy.push_back(ener);}
   void SetThirdStageTime(Double_t time)   {fThirdStage_Time.push_back(time);}
   void SetThirdStagePosition(Int_t pos)   {fThirdStage_Position.push_back(pos);}

   // Fourth stage
   void SetFourthStageEnergy(Double_t ener)   {fFourthStage_Energy.push_back(ener);}
   void SetFourthStageTime(Double_t time)   {fFourthStage_Time.push_back(time);}
   void SetFourthStagePosition(Int_t pos)   {fFourthStage_Position.push_back(pos);}

   // Fifth stage
   void SetFifthStageEnergy(Double_t ener)   {fFifthStage_Energy.push_back(ener);}
   void SetFifthStageTime(Double_t time)   {fFifthStage_Time.push_back(time);}
   void SetFifthStagePosition(Int_t pos)   {fFifthStage_Position.push_back(pos);}

   // Sixth stage
   void SetSixthStageEnergy(Double_t ener)   {fSixthStage_Energy.push_back(ener);}
   void SetSixthStageTime(Double_t time)   {fSixthStage_Time.push_back(time);}
   void SetSixthStagePosition(Int_t pos)   {fSixthStage_Position.push_back(pos);}

   /////////////////////////////////////
   //            GETTERS              //
   /////////////////////////////////////
   // General
   Int_t   GetEventMultiplicity()         {return fTotalEnergy.size();}
   Int_t   GetEventType(Int_t i)         {return fEventType.at(i);}
   Int_t   GetModuleNumber(Int_t i)      {return fModuleNumber.at(i);}
   Double_t   GetTotalEnergy(Int_t i)         {return fTotalEnergy.at(i);}

   // First stage
   Double_t   GetFirstStageEnergy(Int_t i)      {return fFirstStage_Energy.at(i);}
   Double_t   GetFirstStageTime(Int_t i)      {return fFirstStage_Time.at(i);}
   Int_t   GetFirstStageFrontPosition(Int_t i)   {return fFirstStage_FrontPosition.at(i);}
   Int_t   GetFirstStageBackPosition(Int_t i)   {return fFirstStage_BackPosition.at(i);}

   // Second stage
   Double_t   GetSecondStageEnergy(Int_t i)      {return fSecondStage_Energy.at(i);}
   Double_t   GetSecondStageTime(Int_t i)      {return fSecondStage_Time.at(i);}
   Int_t   GetSecondStagePosition(Int_t i)      {return fSecondStage_Position.at(i);}

   // Third stage
   Double_t   GetThirdStageEnergy(Int_t i)      {return fThirdStage_Energy.at(i);}
   Double_t   GetThirdStageTime(Int_t i)      {return fThirdStage_Time.at(i);}
   Int_t   GetThirdStagePosition(Int_t i)      {return fThirdStage_Position.at(i);}

   // Fourth stage
   Double_t   GetFourthStageEnergy(Int_t i)      {return fFourthStage_Energy.at(i);}
   Double_t   GetFourthStageTime(Int_t i)      {return fFourthStage_Time.at(i);}
   Int_t   GetFourthStagePosition(Int_t i)      {return fFourthStage_Position.at(i);}

   // Fifth stage
   Double_t   GetFifthStageEnergy(Int_t i)      {return fFifthStage_Energy.at(i);}
   Double_t   GetFifthStageTime(Int_t i)      {return fFifthStage_Time.at(i);}
   Int_t   GetFifthStagePosition(Int_t i)      {return fFifthStage_Position.at(i);}

   // Sixth stage
   Double_t   GetSixthStageEnergy(Int_t i)      {return fSixthStage_Energy.at(i);}
   Double_t   GetSixthStageTime(Int_t i)      {return fSixthStage_Time.at(i);}
   Int_t   GetSixthStagePosition(Int_t i)      {return fSixthStage_Position.at(i);}

   public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(THyde2TrackerPhysics,1)  // THyde2TrackerPhysics structure
};

#endif
