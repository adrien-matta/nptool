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

#include "TParisPhysics.h"

//  STL
#include <vector>
#include <iostream>
#include <cstdlib>
#include <limits>

ClassImp(TParisPhysics)


TParisPhysics::TParisPhysics() 
{
  //EventMultiplicity = 0;
}



TParisPhysics::~TParisPhysics()
{
   Clear();
}



void TParisPhysics::BuildSimplePhysicalEvent(TParisData* Data)
{
   BuildPhysicalEvent(Data);
}



void TParisPhysics::BuildPhysicalEvent(TParisData* Data)
{

  int multLaBrE = Data->GetPARISLaBr3StageEMult();
  int multCsIE = Data->GetPARISCsIStageEMult();

  //cout << "multLaBr= " << multLaBrE << endl;
  //cout << "multCsI= " << multCsIE << endl;

  ParisEventMult=multLaBrE+multCsIE;
            // get energy from strips and store it
  if(ParisEventMult>=1)
    {

      double EnergyTot=0.;

      if(multLaBrE>=1){
   //cout << "cava1b" <<endl;
   //cout <<  Data->GetPARISLaBr3StageEEnergy(0) <<endl;
   //cout << "cava1b" <<endl;

            double EnergyStripFront;
       double EnergyStrip;

       for(int j=0;j<multLaBrE;j++)
       {
        EnergyStripFront= Data->GetPARISLaBr3StageEEnergy(j);
  
             EnergyStrip  = EnergyStripFront;
             ParisLaBr_E.push_back(EnergyStrip);
  
             EnergyTot += EnergyStrip;
        //cout << "Energytot LaBr=" << EnergyTot << endl;
       }

      // Fill total energy in inner shell
      ParisInTotalEnergy.push_back(EnergyTot);
      }

      if(multCsIE>=1){
   double EnergySecond;
   double EnergyTotSecond;
       for(int j=0;j<multCsIE;j++)
       {
         EnergySecond = Data->GetPARISCsIStageEEnergy(j);
         ParisCsI_E.push_back(EnergySecond);
         EnergyTotSecond +=EnergySecond;

         EnergyTot += EnergySecond;
         //cout << "Energy CsI=" << EnergySecond << endl;
         //cout << "Energytot CsI=" << EnergyTot << endl;
       }

      // Fill total energy in outter shell
      ParisOutTotalEnergy.push_back(EnergyTotSecond);

      }

      // Fill total energy
      ParisTotalEnergy.push_back(EnergyTot);
    }
}



void TParisPhysics::Clear()
{
   //EventMultiplicity= 0;
   ParisEventMult= 0;
   //ModuleNumber.clear();
   //EventType.clear();
   ParisInTotalEnergy.clear();   // inner shell
   ParisOutTotalEnergy.clear();  // outter shell
   ParisTotalEnergy.clear();

   // LaBr
   ParisLaBr_E.clear();
   //FirstStage_T.clear();
   //FirstStage_X.clear();
   //FirstStage_Y.clear();

   // CsI
   ParisCsI_E.clear();
   //SecondStage_T.clear();
   //SecondStage_N.clear();

   /*
   // CsI   
   ThirdStage_E.clear();
   ThirdStage_T.clear();
   ThirdStage_N.clear();
   */
}
