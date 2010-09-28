/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 01/09/10                                                 *
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

#ifndef TSHIELDPHYSICS_H
#define TSHIELDPHYSICS_H

#include <vector>
#include "TObject.h"
#include "TShieldData.h"
#include <cstdlib>

using namespace std ;

class TShieldPhysics : public TObject
{
public:
   TShieldPhysics();
   ~TShieldPhysics();

public: 
   void Clear();	
   void Clear(const Option_t*) {};	
   void BuildPhysicalEvent(TShieldData* Data);
   void BuildSimplePhysicalEvent(TShieldData* Data);

public:
   // Provide Physical Multiplicity
   Int_t		ShieldEventMult;

   // Provide a Classification of Event
   //vector<int>		EventType;

   // Telescope
   //vector<int>		ModuleNumber;

   //	FirstStage
   vector<double>	ShieldCsI_E;
   //vector<double>	FirstStage_T;
   //vector<int>		FirstStage_X;
   //vector<int>		FirstStage_Y;

   /*
   //	SecondStage
   vector<double>	SecondStage_E;
   //vector<double>	SecondStage_T;
   //vector<int>		SecondStage_N;

   
   //	ThirdStage
   vector<double>	ThirdStage_E;
   vector<double>	ThirdStage_T;
   vector<int>		ThirdStage_N;
   */

   // Physical Value  
   vector<double>	ShieldTotalEnergy;

   ClassDef(TShieldPhysics,1)  // ShieldPhysics structure
};

#endif
