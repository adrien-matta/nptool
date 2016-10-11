#ifndef THELIOSPHYSICS_H
#define THELIOSPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche   contact address: marc.labiche@stfc.ac.uk  *
 *                                                                           *
 * Creation Date  : 30/01/12                                                 *
 * Last update    : 31/08/15                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the tracker part of the Gaspard detector.                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "THeliosData.h"
#include "NPVDetector.h"

// c++
#include <cstdlib>
#include <vector>
//ROOT
#include "TObject.h"

using namespace std ;

class THeliosPhysics : public TObject{
public:
   THeliosPhysics();
   ~THeliosPhysics();

public: 
   void Clear();	
   void Clear(const Option_t*) {};
   	
   void BuildPhysicalEvent(THeliosData* Data);
   void BuildSimplePhysicalEvent(THeliosData* Data);

public:
   // Provide Physical Multiplicity
   Int_t		EventMultiplicity;

   // Provide a Classification of Event
   vector<Int_t>		fEventType;

   // Telescope
   vector<Int_t>		fModuleNumber;

   //	FirstStage
    // First stage
    vector<Double_t>  fFirstStage_Energy;
    vector<Double_t>  fFirstStage_Time;
    vector<Int_t>	  fFirstStage_X;
    vector<Int_t>     fFirstStage_Y;

/*
   vector<double>	FirstStage_E;
   vector<double>	FirstStage_T;
   vector<int>		FirstStage_X;
   vector<int>		FirstStage_Y;
*/

   // Physical Value  
   vector<Double_t>	fTotalEnergy;
   vector<Double_t>	fTotalTime;

      
     
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
    void SetFirstStageXPosition(Int_t x)   {fFirstStage_X.push_back(x);}
    void SetFirstStageYPosition(Int_t y)   {fFirstStage_Y.push_back(y);}
   
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
    Int_t   GetFirstStageXPosition(Int_t i)   {return fFirstStage_X.at(i);}
    Int_t   GetFirstStageYPosition(Int_t i)   {return fFirstStage_Y.at(i);}
   
     
     
     
     ClassDef(THeliosPhysics,1)  // HeliosPHysics structure
};

#endif
