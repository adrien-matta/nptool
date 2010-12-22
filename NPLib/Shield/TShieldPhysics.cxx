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
 *             for the Shield part of the Paris detector.                    *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TShieldPhysics.h"

//  STL
#include <vector>
#include <iostream>
#include <cstdlib>
#include <limits>

ClassImp(TShieldPhysics)


TShieldPhysics::TShieldPhysics() 
{
  //EventMultiplicity = 0;
}



TShieldPhysics::~TShieldPhysics()
{
   Clear();
}



void TShieldPhysics::BuildSimplePhysicalEvent(TShieldData* Data)
{
   BuildPhysicalEvent(Data);
}



void TShieldPhysics::BuildPhysicalEvent(TShieldData* Data)
{

  int multE = Data->GetPARISCsIShieldEMult();
  // cout << "multShield = " << multE << endl;

  ShieldEventMult=multE;


            // get energy from strips and store it
  if(multE>=1){
    //cout << "cava1b" <<endl;
    //cout <<  Data->GetPARISCsIShieldEEnergy(0) <<endl;
    //cout << "cava1b" <<endl;

    double EnergyTot=0.;
    double EnergyStripFront;
    double EnergyStrip;

    for(int j=0;j<multE;j++)
      {
	EnergyStripFront= Data->GetPARISCsIShieldEEnergy(j);
	
	EnergyStrip  = EnergyStripFront;
	ShieldCsI_E.push_back(EnergyStrip);

	EnergyTot += EnergyStrip; // addback between CsIShield 
      }

    // Fill total energy
    ShieldTotalEnergy.push_back(EnergyTot);
  }
}



void TShieldPhysics::Clear()
{
   ShieldEventMult=0;
   //ModuleNumber.clear();
   //EventType.clear();
   ShieldTotalEnergy.clear();

   // CsI
   ShieldCsI_E.clear();
   //FirstStage_T.clear();
   //FirstStage_X.clear();
   //FirstStage_Y.clear();

}
