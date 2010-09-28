/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:  M. Labiche    contact address: marc.labiche@stfc.ac.uk  *
 *                                                                           *
 * Creation Date  : 04/12/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the PARIS detector.                                       *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef TPARISPHYSICS_H
#define TPARISPHYSICS_H

#include <vector>
#include "TObject.h"
#include "TParisData.h"
#include <cstdlib>

using namespace std ;

class TParisPhysics : public TObject
{
public:
   TParisPhysics();
   ~TParisPhysics();

public: 
   void Clear();	
   void Clear(const Option_t*) {};	
   void BuildPhysicalEvent(TParisData* Data);
   void BuildSimplePhysicalEvent(TParisData* Data);

public:
   // Provide Physical Multiplicity
   Int_t		ParisEventMult;

   // Provide a Classification of Event
   //vector<int>		EventType;

   // Telescope
   //vector<int>		ModuleNumber;

   //	FirstStage
   vector<double>	ParisLaBr_E;
   //vector<double>	FirstStage_T;
   //vector<int>		FirstStage_X;
   //vector<int>		FirstStage_Y;

   //	SecondStage
   vector<double>	ParisCsI_E;
   //vector<double>	SecondStage_T;
   //vector<int>		SecondStage_N;

   /*
   //	ThirdStage
   vector<double>	ThirdStage_E;
   vector<double>	ThirdStage_T;
   vector<int>		ThirdStage_N;
   */

   // Physical Value  
   vector<double>	ParisTotalEnergy;
   vector<double>	ParisInTotalEnergy;
   vector<double>	ParisOutTotalEnergy;

   ClassDef(TParisPhysics,1)  // GaspardTrackerPHysics structure
};

#endif
