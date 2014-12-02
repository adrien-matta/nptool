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

#include "TFatimaPhysics.h"
#include <vector>
#include <iostream>
#include <cstdlib>


ClassImp(TFatimaPhysics)


TFatimaPhysics::TFatimaPhysics() 
{
  //EventMultiplicity = 0;
}



TFatimaPhysics::~TFatimaPhysics()
{
   Clear();
}



void TFatimaPhysics::BuildSimplePhysicalEvent(TFatimaData* Data)
{
   BuildPhysicalEvent(Data);
}



void TFatimaPhysics::BuildPhysicalEvent(TFatimaData* Data)
{

  int multLaBrE = Data->GetFATIMALaBr3StageEMult();
  //  int multCsIE = Data->GetFATIMACsIStageEMult();

  cout << "%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  cout << "multLaBr= " << multLaBrE << endl;
  //cout << "multCsI= " << multCsIE << endl;

  FatimaEventMult=multLaBrE;
  //FatimaEventMult=multLaBrE+multCsIE;

 // Get energy from strips and store it

  if(FatimaEventMult>=1)
    {

      double EnergyTot=0.;

      double HighestEnergy;
      int DetID_HighestE;
      double CrystID_HighestE;
      // Initialisation
      HighestEnergy=0;
      DetID_HighestE=-1;
      CrystID_HighestE=-1;

      /*
      double HighestEnergy, Outer_HighestEnergy;
      int DetID_HighestE, OuterDetID_HighestE;
      double CrystID_HighestE, OuterCrystID_HighestE;
     
      // Initialisation
      HighestEnergy=Outer_HighestEnergy=0;
      DetID_HighestE=OuterDetID_HighestE=-1;
      CrystID_HighestE=OuterCrystID_HighestE=-1;
      */

      if(multLaBrE>=1){
	//cout << "cava1b" <<endl;
	//cout <<  Data->GetFATIMALaBr3StageEEnergy(0) <<endl;
	//cout << "cava1b" <<endl;

	    double EnergyTotFirst;
            double EnergyStripFront;
	    double EnergyStrip;
	    //double HighestEnergyStrip;

	    for(int j=0;j<multLaBrE;j++)
	    {
	     EnergyStripFront= Data->GetFATIMALaBr3StageEEnergy(j);

             EnergyStrip  = EnergyStripFront;
 	     cout << "Energy LaBr=" << EnergyStrip << endl;
	     FatimaLaBr_E.push_back(EnergyStrip);
  
             EnergyTotFirst += EnergyStrip;
             EnergyTot += EnergyStrip;

	     //Let's find the first  detector hit:
	     if(HighestEnergy<EnergyStripFront)
	       {
		 HighestEnergy=EnergyStripFront;
		 DetID_HighestE=Data->GetFATIMALaBr3StageEDetectorNbr(j);
		 CrystID_HighestE=Data->GetFATIMALaBr3StageECrystalNbr(j);
	       }


	    }

      // Fill total energy in inner shell and the first detector/crystal hit

      FatimaInTotalEnergy.push_back(EnergyTotFirst);

      cout << " Id of Fatima detector hit= " << DetID_HighestE << endl;
      cout << " EnergyTotFirst= " <<  EnergyTotFirst << endl;
      cout << " EnergyTot= " <<  EnergyTot << endl;

      if(DetID_HighestE>100)// Cluster case
	{
	  //ModuleNumber.push_back(DetID_HighestE);
	  DetectorNumber.push_back(-1);
	}else               // Detector case
	  {
	    //ModuleNumber.push_back(-1);
           DetectorNumber.push_back(DetID_HighestE);
	  }

      if(DetID_HighestE<100)  // Detector case
	{
	  FirstStage_X.push_back(0);
	  FirstStage_Y.push_back(0);
	}


      /*
     if(DetID_HighestE>100)  // Cluster case
	{

	  if(CrystID_HighestE==0)
	    {
	      FirstStage_X.push_back(0);
	      FirstStage_Y.push_back(0);
	    }
	  if(CrystID_HighestE==1)
	    {
	      FirstStage_X.push_back(1);
	      FirstStage_Y.push_back(0);
	    }
	  if(CrystID_HighestE==2)
	    {
	      FirstStage_X.push_back(2);
	      FirstStage_Y.push_back(0);
	    }
	  if(CrystID_HighestE==3)
	    {
	      FirstStage_X.push_back(0);
	      FirstStage_Y.push_back(1);
	    }
	  if(CrystID_HighestE==4)
	    {
	      FirstStage_X.push_back(1);
	      FirstStage_Y.push_back(1);
	    }
	  if(CrystID_HighestE==5)
	    {
	      FirstStage_X.push_back(2);
	      FirstStage_Y.push_back(1);
	    }
	  if(CrystID_HighestE==6)
	    {
	      FirstStage_X.push_back(0);
	      FirstStage_Y.push_back(2);
	    }
	  if(CrystID_HighestE==7)
	    {
	      FirstStage_X.push_back(1);
	      FirstStage_Y.push_back(2);
	    }
	  if(CrystID_HighestE==8)
	    {
	      FirstStage_X.push_back(2);
	      FirstStage_Y.push_back(2);
	    }
	}  // end of cluster case
      */

      }

      /*
      if(multCsIE>=1){
	double EnergySecond;
	double EnergyTotSecond;
	    for(int j=0;j<multCsIE;j++)
	    {
	      EnergySecond = Data->GetFATIMACsIStageEEnergy(j);
	      FatimaCsI_E.push_back(EnergySecond);
	      EnergyTotSecond +=EnergySecond;

	      EnergyTot += EnergySecond;
	      cout << "Energy CsI=" << EnergySecond << endl;
	      //cout << "Energytot CsI=" << EnergyTot << endl;

	     //Let's find the first outer detector hit:
	     if(Outer_HighestEnergy<EnergySecond)
	       {
		 Outer_HighestEnergy=EnergySecond;
		 OuterDetID_HighestE=Data->GetFATIMACsIStageEDetectorNbr(j);
		 OuterCrystID_HighestE=Data->GetFATIMACsIStageECrystalNbr(j);
	       }
	    }

	    // Fill total energy in outer shell
	    FatimaOutTotalEnergy.push_back(EnergyTotSecond);
 
            cout << " Id of Fatima outer detector hit= " << OuterDetID_HighestE << endl;
            cout << " EnergyTotSecond= " <<  EnergyTotSecond << endl;
            cout << " EnergyTot= " <<  EnergyTot << endl;

	    if(multLaBrE==0 || (EnergyTotSecond>EnergyTot) ) {  // = only if Outer layer is hit first

	      FirstStage_X.push_back(-1);
	      FirstStage_Y.push_back(-1);

	      cout << " Id of Fatima (CsI) detector hit= " << OuterDetID_HighestE << endl;

	      if(OuterDetID_HighestE>100)
		{
		  ModuleNumber.push_back(OuterDetID_HighestE);
		  DetectorNumber.push_back(-1);
		}else
		  {
		    ModuleNumber.push_back(-1);
		    DetectorNumber.push_back(OuterDetID_HighestE);
		  }


	      if(OuterDetID_HighestE<100)  // Detector case
		{
		  SecondStage_X.push_back(0);
		  SecondStage_Y.push_back(0);
		}


	      if(OuterDetID_HighestE>100)  // Cluster case
		{
		  if(OuterCrystID_HighestE==0)
		    {
		      SecondStage_X.push_back(0);
		      SecondStage_Y.push_back(0);
		    }
		  if(OuterCrystID_HighestE==1)
		    {
		      SecondStage_X.push_back(1);
		      SecondStage_Y.push_back(0);
		    }
		  if(OuterCrystID_HighestE==2)
		    {
		      SecondStage_X.push_back(2);
		      SecondStage_Y.push_back(0);
		    }
		  if(OuterCrystID_HighestE==3)
		    {
		      SecondStage_X.push_back(0);
		      SecondStage_Y.push_back(1);
		    }
		  if(OuterCrystID_HighestE==4)
		    {
		      SecondStage_X.push_back(1);
		      SecondStage_Y.push_back(1);
		    }
		  if(OuterCrystID_HighestE==5)
		    {
		      SecondStage_X.push_back(2);
		      SecondStage_Y.push_back(1);
		    }
		  if(OuterCrystID_HighestE==6)
		    {
		      SecondStage_X.push_back(0);
		      SecondStage_Y.push_back(2);
		    }
		  if(OuterCrystID_HighestE==7)
		    {
		      SecondStage_X.push_back(1);
		      SecondStage_Y.push_back(2);
		    }
		  if(OuterCrystID_HighestE==8)
		    {
		      SecondStage_X.push_back(2);
		      SecondStage_Y.push_back(2);
		    }

		}
	    }

      }
      */

      // Fill total energy
      FatimaTotalEnergy.push_back(EnergyTot);
    }
}



void TFatimaPhysics::Clear()
{
   //EventMultiplicity= 0;
   FatimaEventMult= 0;
    DetectorNumber.clear();  // phoswich #
 
  //EventType.clear();
   FatimaInTotalEnergy.clear();   // inner shell
   //FatimaOutTotalEnergy.clear();  // outter shell
   FatimaTotalEnergy.clear();


   // LaBr
   FatimaLaBr_E.clear();
   //FatimaIn_1stDetId.clear();
   //FatimaIn_1stCrystId.clear();
   //FirstStage_T.clear();
   FirstStage_X.clear();
   FirstStage_Y.clear();

   /*   // CsI
   FatimaCsI_E.clear();
   //FatimaOut_1stDetId.clear();
   //FatimaOut_1stCrystId.clear();
   //SecondStage_T.clear();
   //SecondStage_N.clear();
   SecondStage_X.clear();
   SecondStage_Y.clear();
   */

   /*
   // CsI	
   ThirdStage_E.clear();
   ThirdStage_T.clear();
   ThirdStage_N.clear();
   */
}
