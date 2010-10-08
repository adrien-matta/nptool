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
 * Last update    :                                                          *
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

#ifndef TGASPARDTRACKERPHYSICS_H
#define TGASPARDTRACKERPHYSICS_H

#include "TGaspardTrackerData.h"

#include "TObject.h"

#include <vector>
#include <cstdlib>

using namespace std ;

class TGaspardTrackerPhysics : public TObject
{
public:
   TGaspardTrackerPhysics();
   ~TGaspardTrackerPhysics();

public: 
   void Clear();	
   void Clear(const Option_t*) {};	
   void BuildPhysicalEvent(TGaspardTrackerData* Data);
   void BuildSimplePhysicalEvent(TGaspardTrackerData* Data);

public:
   // Provide Physical Multiplicity
   Int_t		EventMultiplicity;

   // Provide a Classification of Event
   vector<int>		EventType;

   // Telescope
   vector<int>		ModuleNumber;

   //	FirstStage
   vector<double>	FirstStage_E;
   vector<double>	FirstStage_T;
   vector<int>		FirstStage_X;
   vector<int>		FirstStage_Y;

   //	SecondStage
   vector<double>	SecondStage_E;
   vector<double>	SecondStage_T;
   vector<int>		SecondStage_N;

   //	ThirdStage
   vector<double>	ThirdStage_E;
   vector<double>	ThirdStage_T;
   vector<int>		ThirdStage_N;

   // Physical Value  
   vector<double>	TotalEnergy;

   ClassDef(TGaspardTrackerPhysics,1)  // GaspardTrackerPHysics structure
};

#endif
