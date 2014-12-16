/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:  M. Labiche    contact address: marc.labiche@stfc.ac.uk  *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    : 02/07/2014                                                         *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the FATIMA detector.                                       *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef TFATIMAPHYSICS_H
#define TFATIMAPHYSICS_H

#include <vector>
#include "TObject.h"
#include "TFatimaData.h"
#include <cstdlib>

using namespace std ;

class TFatimaPhysics : public TObject
{
public:
   TFatimaPhysics();
   ~TFatimaPhysics();

public: 
   void Clear();	
   void Clear(const Option_t*) {};	
   void BuildPhysicalEvent(TFatimaData* Data);
   void BuildSimplePhysicalEvent(TFatimaData* Data);

public:
   // Provide Physical Multiplicity
   Int_t		FatimaEventMult;

   // Provide a Classification of Event
   //vector<int>		EventType;


   // Telescope
   //vector<int>		ModuleNumber;  // Id of cluster

   vector<int>		DetectorNumber;  // Id of Detector

   //	FirstStage
   vector<double>	FatimaLaBr_E;
   //vector<double>	FirstStage_T;
   vector<int>		FirstStage_X;   // column # of LaBr crystal in 3x3 cluster
   vector<int>		FirstStage_Y;   // raw # of LaBr crystal in 3x3 cluster

   /* 
  //	SecondStage
   vector<double>	FatimaCsI_E;
   //vector<double>	SecondStage_T;
   //vector<int>		SecondStage_N;
   vector<int>		SecondStage_X; // column # of CsI crystal in 3x3 cluster
   vector<int>		SecondStage_Y; // raw # of CsI crystal in 3x3 cluster
   */


   /*
   //	ThirdStage
   vector<double>	ThirdStage_E;
   vector<double>	ThirdStage_T;
   vector<int>		ThirdStage_N;
   */

   // Physical Value  
   vector<double>	FatimaTotalEnergy;
   vector<double>	FatimaInTotalEnergy;
   vector<double>	FatimaOutTotalEnergy;


   ClassDef(TFatimaPhysics,1)  // GaspardTrackerPHysics structure
};

#endif
