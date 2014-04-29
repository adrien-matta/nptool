/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
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

#include "THeliosPhysics.h"
#include <iostream>
#include <cstdlib>


ClassImp(THeliosPhysics)


THeliosPhysics::THeliosPhysics() 
{
   EventMultiplicity = 0;
}



THeliosPhysics::~THeliosPhysics()
{
   Clear();
}



void THeliosPhysics::BuildSimplePhysicalEvent(THeliosData* Data)
{
   BuildPhysicalEvent(Data);
}



void THeliosPhysics::BuildPhysicalEvent(THeliosData* Data)
{
   // indexes for each kind of detector
   // this should follow the indexes given in NPSimulation
   //    0: Square
   //  100: Trapezoid
   //  200: Annular
   // 1000: DummyShape
   const int dim = 4;
   int index[dim] = {0, 100, 200, 1000};

   // Check
   bool Check_FirstStage = false; if (Check_FirstStage) {};


   // Thresholds
/*
   double FirstStage_Front_E_Threshold = 0; double FirstStage_Front_T_Threshold = 0;
   double FirstStage_Back_E_Threshold  = 0; double FirstStage_Back_T_Threshold  = 0;
   double SecondStage_E_Threshold      = 0; double SecondStage_T_Threshold      = 0;
   double ThirdStage_E_Threshold       = 0; double ThirdStage_T_Threshold	= 0;
*/
   // calculate multipicity in the first stage
   int multXE = Data->GetHeliosFirstStageEMult();
   int multXT = Data->GetHeliosFirstStageTMult();


   //cout << multXE << " " <<multYE << " " <<multXT << " " <<multYT << endl;
 
   // Deal with multiplicity 1 for the first layer
   if (multXE==1 && multXT==1) {
      // calculate detector number
      int det_ref = Data->GetHeliosFirstStageEDetectorNbr(0);
      int detecXE = Data->GetHeliosFirstStageEDetectorNbr(0) / det_ref;
      int detecXT = Data->GetHeliosFirstStageTDetectorNbr(0) / det_ref;
      // convert detector number in order to it begins at 0
      // this is needed to access the strip position information
      // WARNING: this method may not work for an heteregneous detector 
      //          configuration, e.g. Trapezoid + Annular
      int good_index = -100;
      int diff_ref = 10000;
      for (int i = 0; i < dim; i++) {
         int diff = abs(det_ref - index[i]);
         if (diff < diff_ref) {
            diff_ref = diff;
            good_index = i;
         }
      }
      det_ref -= index[good_index];

      // case of same detector
      if (detecXE*detecXT == 1) {
         // store module number
         ModuleNumber.push_back(det_ref);
         // calculate strip number
         int stripXE = Data->GetHeliosFirstStageEStripNbr(0);
         int stripXT = Data->GetHeliosFirstStageTStripNbr(0);

         // case of same strips on X and Y
         if (stripXE == stripXT ) {        // here we have a good strip event
            // various
            Check_FirstStage = true;
            EventMultiplicity = 1;
            // store strip ID
            FirstStage_X.push_back(stripXE);
            // get energy from strips and store it
            double EnergyStrip = Data->GetHeliosFirstStageEEnergy(0);
            FirstStage_E.push_back(EnergyStrip);
            double EnergyTot = EnergyStrip;
	    //	    cout << "XXXXXXXXXXXXXXXXXXXXXXX" << endl;
	    //	    cout << "EnergyTot=" << EnergyTot << endl;


            // get time from strips and store it
            double TimeStrip = Data->GetHeliosFirstStageTTime(0);
             FirstStage_T.push_back(TimeStrip);
	    double TimeTot = TimeStrip;
	    //      cout << "TimeTot=" << TimeTot << endl;

            // Fill total energy
            TotalEnergy.push_back(EnergyTot);
            // Fill time detection
            TotalTime.push_back(TimeTot);
         }
         else {
            cout << "Not same strips" << endl;
         }
      }
      else {
         cout << "Not same detector" << endl;
      }
   }
   else {
/*      cout << "Multiplicity is not one, it is: " << endl;
      cout << "\tmultXE: " << multXE << endl;
      cout << "\tmultXT: " << multXT << endl;
      cout << "\tmultYE: " << multYE << endl;
      cout << "\tmultYT: " << multYT << endl;*/
   }
}



void THeliosPhysics::Clear()
{
   EventMultiplicity= 0;
   ModuleNumber.clear();
   EventType.clear();
   TotalEnergy.clear();
   TotalTime.clear();

   // Si X
   FirstStage_E.clear();
   FirstStage_T.clear();
   FirstStage_X.clear();
   FirstStage_Y.clear();

}
