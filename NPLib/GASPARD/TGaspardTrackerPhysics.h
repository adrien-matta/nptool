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
 *             for the tracker part of the Gaspard detector.                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 08/10/10: Change private member names and add setters and getters    *
 *                methods. BuildPhysicalEvent() method is moved to the       *
 *                TGaspardTracker*** classes (N. de Sereville)               *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef TGASPARDTRACKERPHYSICS_H
#define TGASPARDTRACKERPHYSICS_H

#include "TGaspardTrackerData.h"

// ROOT
#include "TObject.h"

// C++
#include <vector>

using namespace std ;


class TGaspardTrackerPhysics : public TObject
{
public:
   TGaspardTrackerPhysics();
   ~TGaspardTrackerPhysics();

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

   ClassDef(TGaspardTrackerPhysics,1)  // TGaspardTrackerPhysics structure
};

#endif
