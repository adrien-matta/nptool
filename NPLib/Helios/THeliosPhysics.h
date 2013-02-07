#ifndef THELIOSPHYSICS_H
#define THELIOSPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche   contact address: marc.labiche@stfc.ac.uk  *
 *                                                                           *
 * Creation Date  : 30/01/12                                                 *
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



#include <vector>
#include "TObject.h"
#include "THeliosData.h"
#include <cstdlib>

using namespace std ;

class THeliosPhysics : public TObject
{
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
   vector<int>		EventType;

   // Telescope
   vector<int>		ModuleNumber;

   //	FirstStage
   vector<double>	FirstStage_E;
   vector<double>	FirstStage_T;
   vector<int>		FirstStage_X;
   vector<int>		FirstStage_Y;


   // Physical Value  
   vector<double>	TotalEnergy;
   vector<double>	TotalTime;

   ClassDef(THeliosPhysics,1)  // HeliosPHysics structure
};

#endif
