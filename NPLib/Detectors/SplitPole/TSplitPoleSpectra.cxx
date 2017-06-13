/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 ****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville         address: deserevi@ipno.in2p3.fr  *
 *                                                                           *
 * Creation Date  : November 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for SplitPole             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// class header
#include "TSplitPoleSpectra.h"

// C++ headers
#include <iostream>  
#include <string>
using namespace std;

// NPTool headers
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TSplitPoleSpectra::TSplitPoleSpectra()
{
   if (NPOptionManager::getInstance()->GetVerboseLevel() > 0)
      cout << "*********************************************************" << endl
           << "TSplitPoleSpectra : Initializing control spectra         " << endl 
           << "*********************************************************" << endl;
   SetName("SplitPole");

   InitRawSpectra();
   InitPreTreatedSpectra();
   InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TSplitPoleSpectra::~TSplitPoleSpectra()
{
}



////////////////////////////////////////////////////////////////////////////////
void TSplitPoleSpectra::InitRawSpectra()
{
   static string name;
   // POSITION
   name = "SplitPole_POSITION";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // DELTAE
   name = "SplitPole_DELTAE";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // WIRE
   name = "SplitPole_WIRE";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // PLASTICP
   name = "SplitPole_PLASTICP";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // PLASTICG
   name = "SplitPole_PLASTICG";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // Time1
   name = "SplitPole_Time1";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // Time2
   name = "SplitPole_Time2";
   AddHisto1D(name, name, 4096, 0, 4096, "SplitPole/RAW");

   // Multiplicity Time!
   name = "SplitPole_Mult_Time1";
   AddHisto1D(name, name, 10, 0, 10, "SplitPole/RAW");

   // Multiplicity Time2
   name = "SplitPole_Mult_Time2";
   AddHisto1D(name, name, 10, 0, 10, "SplitPole/RAW");

   // POSITION vs DELTAE 
   name = "SplitPole_POSITION_DELTAE";
   AddHisto2D(name, name, 512, 0, 4096, 512, 0, 4096, "SplitPole/RAW");

   // POSITION vs WIRE 
   name = "SplitPole_POSITION_WIRE";
   AddHisto2D(name, name, 512, 0, 4096, 512, 0, 4096, "SplitPole/RAW");

   // DELTAE vs WIRE 
   name = "SplitPole_DELTAE_WIRE";
   AddHisto2D(name, name, 512, 0, 4096, 512, 0, 4096, "SplitPole/RAW");

   // POSITION vs PLASTICP 
   name = "SplitPole_POSITION_PLASTICP";
   AddHisto2D(name, name, 512, 0, 4096, 512, 0, 4096, "SplitPole/RAW");

   // POSITION vs PLASTICG 
   name = "SplitPole_POSITION_PLASTICG";
   AddHisto2D(name, name, 512, 0, 4096, 512, 0, 4096, "SplitPole/RAW");
}



////////////////////////////////////////////////////////////////////////////////
void TSplitPoleSpectra::InitPreTreatedSpectra()
{
   static string name;
   // POSITION
   name = "SplitPole_RHO";
   AddHisto1D(name, name, 4096, 0.62, 0.85, "SplitPole/CAL");

}



////////////////////////////////////////////////////////////////////////////////
void TSplitPoleSpectra::InitPhysicsSpectra()
{
   static string name;
   // Brho 
   name = "SplitPole_BRHO";
   AddHisto1D(name, name, 4096, 0.25, 0.56, "SplitPole/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TSplitPoleSpectra::FillRawSpectra(TSplitPoleData* RawData)
{
   static string index;

   // POSITION 
   index = "SplitPole/RAW/SplitPole_POSITION";
   FillSpectra(index,RawData->GetPosition());

   // DELTAE
   index = "SplitPole/RAW/SplitPole_DELTAE";
   FillSpectra(index,RawData->GetDeltaE());

   // WIRE
   index = "SplitPole/RAW/SplitPole_WIRE";
   FillSpectra(index,RawData->GetWire());

   // PLASTICP
   index = "SplitPole/RAW/SplitPole_PLASTICP";
   FillSpectra(index,RawData->GetPlasticP());

   // PLASTICG
   index = "SplitPole/RAW/SplitPole_PLASTICG";
   FillSpectra(index,RawData->GetPlasticG());

   // Time1
   index = "SplitPole/RAW/SplitPole_Time1";
   for (UShort_t i = 0; i < RawData->GetTime1Multiplicity(); ++i) {   // loop on multiplicity
      FillSpectra(index,RawData->GetTime1(i));
   } // end loop on multiplicity

   // Time2
   index = "SplitPole/RAW/SplitPole_Time2";
   for (UShort_t i = 0; i < RawData->GetTime2Multiplicity(); ++i) {   // loop on multiplicity
      FillSpectra(index,RawData->GetTime2(i));
   } // end loop on multiplicity

   // Multiplicity Time1
   index = "SplitPole/RAW/SplitPole_Mult_Time1";
   FillSpectra(index,RawData->GetTime1Multiplicity());

   // Multiplicity Time2
   index = "SplitPole/RAW/SplitPole_Mult_Time2";
   FillSpectra(index,RawData->GetTime2Multiplicity());

   // POSITION vs DELTAE
   index = "SplitPole/RAW/SplitPole_POSITION_DELTAE";
   FillSpectra(index,RawData->GetDeltaE(), RawData->GetPosition());

   // POSITION vs WIRE 
   index = "SplitPole/RAW/SplitPole_POSITION_WIRE";
   FillSpectra(index,RawData->GetWire(), RawData->GetPosition());

   // DELTAE vs WIRE 
   index = "SplitPole/RAW/SplitPole_DELTAE_WIRE";
   FillSpectra(index,RawData->GetWire(), RawData->GetDeltaE());

   // POSITION vs PLASTICP
   index = "SplitPole/RAW/SplitPole_POSITION_PLASTICP";
   FillSpectra(index,RawData->GetPlasticP(), RawData->GetPosition());

   // POSITION vs PLASTICG
   index = "SplitPole/RAW/SplitPole_POSITION_PLASTICG";
   FillSpectra(index,RawData->GetPlasticG(), RawData->GetPosition());
}



////////////////////////////////////////////////////////////////////////////////
void TSplitPoleSpectra::FillPreTreatedSpectra(TSplitPoleData* PreTreatedData)
{
   // RHO 
   static string index;
   index = "SplitPole/CAL/SplitPole_RHO";
   FillSpectra(index,PreTreatedData->GetPlasticG());
}



////////////////////////////////////////////////////////////////////////////////
void TSplitPoleSpectra::FillPhysicsSpectra(TSplitPolePhysics* Physics)
{
   static string index;

   // POSITION 
   index = "SplitPole/PHY/SplitPole_BRHO";
   FillSpectra(index,Physics->GetBrho());
}

