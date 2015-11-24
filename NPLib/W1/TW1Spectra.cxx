/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
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
 *  This class holds all the online spectra needed for W1                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// class header
#include "TW1Spectra.h"

// C++ headers
#include <iostream>  
#include <string>
using namespace std;

// NPTool headers
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TW1Spectra::TW1Spectra()
   : fNumberOfDetectors(0),
     fNumberOfStripsFront(16),
     fNumberOfStripsBack(16)
{
   SetName("W1");
}



////////////////////////////////////////////////////////////////////////////////
TW1Spectra::TW1Spectra(unsigned int NumberOfDetectors)
{
   if (NPOptionManager::getInstance()->GetVerboseLevel() > 0)
      cout << "******************************************************" << endl
           << "TW1Spectra : Initalising control spectra for " 
           << NumberOfDetectors << " Detectors" << endl
           << "******************************************************" << endl ;
   SetName("W1");
   fNumberOfDetectors = NumberOfDetectors;
   fNumberOfStripsFront = 16;
   fNumberOfStripsBack  = 16;

   InitRawSpectra();
   InitPreTreatedSpectra();
   InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TW1Spectra::~TW1Spectra()
{
}



////////////////////////////////////////////////////////////////////////////////
void TW1Spectra::InitRawSpectra()
{
   static string name;
   for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
      // STR_FRONT_E_RAW
      name = "W1_D"+NPL::itoa(i+1)+"_STR_FRONT_E_RAW";
      AddHisto2D(name, name, fNumberOfStripsFront, 0, fNumberOfStripsFront, 4096, 0, 4096, "W1/RAW/STR_FRONT_E");

      // STR_BACK_E_RAW
      name = "W1_D"+NPL::itoa(i+1)+"_STR_BACK_E_RAW";
      AddHisto2D(name, name, fNumberOfStripsBack, 0, fNumberOfStripsBack, 4096, 0, 4096, "W1/RAW/STR_BACK_E");

      // STR_FRONT_EMAX_RAW
      name = "W1_D"+NPL::itoa(i+1)+"_STR_FRONT_EMAX_RAW";
      AddHisto2D(name, name, fNumberOfStripsFront, 0, fNumberOfStripsFront, 4096, 0, 4096, "W1/RAW/STR_FRONT_EMAX");

      // STR_BACK_EMAX_Raw
      name = "W1_D"+NPL::itoa(i+1)+"_STR_BACK_EMAX_RAW";
      AddHisto2D(name, name, fNumberOfStripsBack, 0, fNumberOfStripsBack, 4096, 0, 4096, "W1/RAW/STR_BACK_EMAX");

      // STR_FRONT_RAW_MULT
      name = "W1_D"+NPL::itoa(i+1)+"_STR_FRONT_RAW_MULT";
      AddHisto1D(name, name, fNumberOfStripsFront, 1, fNumberOfStripsFront+1, "W1/RAW/MULT");

      // STR_BACK_RAW_MULT
      name = "W1_D"+NPL::itoa(i+1)+"_STR_BACK_RAW_MULT";
      AddHisto1D(name, name, fNumberOfStripsFront, 1, fNumberOfStripsFront+1, "W1/RAW/MULT");
   } // end loop on number of detectors

   // One single STR_E_RAW spectrum
   name = "W1_STR_E_RAW";
   Int_t ntot = (fNumberOfStripsFront+fNumberOfStripsBack) * fNumberOfDetectors;
   AddHisto2D(name, name, ntot, 0, ntot, 4096, 0, 4096, "W1/RAW");
}



////////////////////////////////////////////////////////////////////////////////
void TW1Spectra::InitPreTreatedSpectra()
{
   static string name;
   for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
      // STR_FRONT_E_CAL
      name = "W1_D"+NPL::itoa(i+1)+"_STR_FRONT_E_CAL";
      AddHisto2D(name, name, fNumberOfStripsFront, 0, fNumberOfStripsFront, 4000, 0, 8, "W1/CAL/STR_FRONT_E");

      // STR_BACK_E_CAL
      name = "W1_D"+NPL::itoa(i+1)+"_STR_BACK_E_CAL";
      AddHisto2D(name, name, fNumberOfStripsBack, 0, fNumberOfStripsBack, 4000, 0, 8, "W1/CAL/STR_BACK_E");

      // STR_FRONT_CAL_MULT
      name = "W1_D"+NPL::itoa(i+1)+"_STR_FRONT_CAL_MULT";
      AddHisto1D(name, name, fNumberOfStripsFront, 1, fNumberOfStripsFront+1, "W1/CAL/MULT");

      // STR_BACK_CAL_MULT
      name = "W1_D"+NPL::itoa(i+1)+"_STR_BACK_CAL_MULT";
      AddHisto1D(name, name, fNumberOfStripsFront, 1, fNumberOfStripsFront+1, "W1/CAL/MULT");

      // Front-Back Energy Correlation
      name = "W1_D"+NPL::itoa(i+1)+"_FB_COR";
      AddHisto2D(name, name, 2000, 0, 8, 2000, 0, 8, "W1/CAL/FB"); 
   }  // end loop on number of detectors

   // One single STR_E_CAL spectrum
   name = "W1_STR_E_CAL";
   Int_t ntot = (fNumberOfStripsFront+fNumberOfStripsBack) * fNumberOfDetectors;
   AddHisto2D(name, name, ntot, 0, ntot, 2000, 0, 8, "W1/CAL");
}



////////////////////////////////////////////////////////////////////////////////
void TW1Spectra::InitPhysicsSpectra()
{
   static string name;
   // Kinematic Plot 
   name = "W1_THETA_E";
   AddHisto2D(name, name,360,0,180,500,0,50,"W1/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TW1Spectra::FillRawSpectra(TW1Data* RawData)
{
   static string index;

   // STR_FRONT_E 
   unsigned int mysize = RawData->GetFrontEMult();
   double EFMAX = 0 ;
   int SFMAX = 0;
   int DFMAX = 0 ;
   for (unsigned int i = 0; i < mysize; i++) {
      index = "W1/RAW/STR_FRONT_E/W1_D"+NPL::itoa(RawData->GetFrontEDetectorNbr(i))+"_STR_FRONT_E_RAW";
      if(RawData->GetFrontEEnergy(i) > EFMAX){
         EFMAX = RawData->GetFrontEEnergy(i);
         SFMAX = RawData->GetFrontEStripNbr(i);
         DFMAX = RawData->GetFrontEDetectorNbr(i);
      }
      GetHisto(index)-> Fill(RawData->GetFrontEStripNbr(i), RawData->GetFrontEEnergy(i));
   }

   if (DFMAX != 0) {
      index = "W1/RAW/STR_FRONT_EMAX/W1_D"+NPL::itoa(DFMAX)+"_STR_FRONT_EMAX_RAW";
      GetHisto(index)-> Fill(SFMAX, EFMAX);
   }

   // STR_BACK_E
   mysize = RawData->GetBackEMult();
   double EBMAX = 0 ;
   int SBMAX = 0;
   int DBMAX = 0 ;

   for (unsigned int i = 0; i < mysize; i++) {
      index = "W1/RAW/STR_BACK_E/W1_D"+NPL::itoa( RawData->GetBackEDetectorNbr(i) )+"_STR_BACK_E_RAW";
      if(RawData->GetBackEEnergy(i) > EBMAX){
         EBMAX = RawData->GetBackEEnergy(i);
         SBMAX = RawData->GetBackEStripNbr(i);
         DBMAX = RawData->GetBackEDetectorNbr(i);
      }
      GetHisto(index)-> Fill(RawData->GetBackEStripNbr(i), RawData->GetBackEEnergy(i));
   }

   if (DBMAX != 0) {
      index = "W1/RAW/STR_BACK_EMAX/W1_D"+NPL::itoa(DBMAX)+"_STR_BACK_EMAX_RAW";
      GetHisto(index)-> Fill(SBMAX, EBMAX);
   }

   // STR_E_RAW
   index = "W1/RAW/W1_STR_E_RAW";
   UShort_t multFront = RawData->GetFrontEMult();
   for (UShort_t i = 0; i < multFront; ++i) {   // loop on front strips
      GetHisto(index)->Fill(RawData->GetFrontEStripNbr(i) + 2*fNumberOfStripsFront*(RawData->GetFrontEDetectorNbr(i)-1), RawData->GetFrontEEnergy(i));
   } // end loop on front strips
   UShort_t multBack = RawData->GetBackEMult();
   for (UShort_t i = 0; i < multBack; ++i) {   // loop on front strips
      GetHisto(index)->Fill(RawData->GetBackEStripNbr(i) + (2*(RawData->GetBackEDetectorNbr(i)-1)+1)*fNumberOfStripsBack, RawData->GetBackEEnergy(i));
   } // end loop on front strips

   // STR_FRONT MULT
   int myMULT[fNumberOfDetectors];
   for (unsigned int i = 0; i < fNumberOfDetectors; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetFrontEMult();i++) {
      myMULT[RawData->GetFrontEDetectorNbr(i)-1] += 1;  
   }

   for (unsigned int i = 0; i < fNumberOfDetectors; i++) {
      index = "W1/RAW/MULT/W1_D"+NPL::itoa(i+1)+"_STR_FRONT_RAW_MULT";
      GetHisto(index)->Fill(myMULT[i]);
   }

   // STR_BACK MULT
   for (unsigned int i = 0; i < fNumberOfDetectors; i++) myMULT[i] = 0;

   mysize = RawData->GetBackEMult();
   for (unsigned int i = 0; i < mysize;i++) {
      myMULT[RawData->GetBackEDetectorNbr(i)-1] += 1;  
   }

   for (unsigned int i = 0; i < fNumberOfDetectors; i++) {
      index= "W1/RAW/MULT/W1_D"+NPL::itoa(i+1)+"_STR_BACK_RAW_MULT";
      GetHisto(index)->Fill(myMULT[i]);
   }
}



////////////////////////////////////////////////////////////////////////////////
void TW1Spectra::FillPreTreatedSpectra(TW1Data* PreTreatedData)
{
   static string index;

   // Front-Back
   unsigned int mysizeF = PreTreatedData->GetFrontEMult();
   unsigned int mysizeB = PreTreatedData->GetBackEMult();

   for (unsigned int i = 0; i < mysizeF; i++) {
      for (unsigned int j = 0; j < mysizeB; j++) {
         if(PreTreatedData->GetFrontEDetectorNbr(i)==PreTreatedData->GetBackEDetectorNbr(j)){
            index="W1/CAL/FB/W1_D"+NPL::itoa(PreTreatedData->GetFrontEDetectorNbr(i))+"_FB_COR";
            GetHisto(index)->Fill(PreTreatedData->GetFrontEEnergy(i), PreTreatedData->GetBackEEnergy(j));
         }
      }
   } 

   // STR_FRONT_E
   unsigned int mysize = PreTreatedData->GetFrontEMult();
   for (unsigned int i = 0; i < mysize; i++) {
      index = "W1/CAL/STR_FRONT_E/W1_D"+NPL::itoa(PreTreatedData->GetFrontEDetectorNbr(i))+"_STR_FRONT_E_CAL";
      GetHisto(index)->Fill(PreTreatedData->GetFrontEStripNbr(i), PreTreatedData->GetFrontEEnergy(i));
   }
   // STR_BACK_E
   mysize = PreTreatedData->GetBackEMult();
   for (unsigned int i = 0; i < mysize; i++) {
      index = "W1/CAL/STR_BACK_E/W1_D"+NPL::itoa( PreTreatedData->GetBackEDetectorNbr(i))+"_STR_BACK_E_CAL";
      GetHisto(index)->Fill(PreTreatedData->GetBackEStripNbr(i), PreTreatedData->GetBackEEnergy(i));
   }

   // STR_E_CAL
   index = "W1/CAL/W1_STR_E_CAL";
   UShort_t multFront = PreTreatedData->GetFrontEMult();
   for (UShort_t i = 0; i < multFront; ++i) {   // loop on front strips
      GetHisto(index)->Fill(PreTreatedData->GetFrontEStripNbr(i) + 2*fNumberOfStripsFront*(PreTreatedData->GetFrontEDetectorNbr(i)-1), PreTreatedData->GetFrontEEnergy(i));
   } // end loop on front strips
   UShort_t multBack = PreTreatedData->GetBackEMult();
   for (UShort_t i = 0; i < multBack; ++i) {   // loop on front strips
      GetHisto(index)->Fill(PreTreatedData->GetBackEStripNbr(i) + (2*(PreTreatedData->GetBackEDetectorNbr(i)-1)+1)*fNumberOfStripsBack, PreTreatedData->GetBackEEnergy(i));
   } // end loop on front strips


   // STR_FRONT MULT
   int myMULT[fNumberOfDetectors];
   for (unsigned int i = 0; i < fNumberOfDetectors; i++) myMULT[i] = 0;

   mysize = PreTreatedData->GetFrontEMult(); 
   for (unsigned int i = 0 ; i < mysize ;i++) {
      myMULT[PreTreatedData->GetFrontEDetectorNbr(i)-1] += 1;  
   }

   for (unsigned int i = 0; i < fNumberOfDetectors; i++) {
      index= "W1/CAL/MULT/W1_D"+NPL::itoa(i+1)+"_STR_FRONT_CAL_MULT";
      GetHisto(index)->Fill(myMULT[i]);
   }

   // STR_BACK MULT
   for (unsigned int i = 0; i < fNumberOfDetectors; i++) myMULT[i] = 0; 

   mysize = PreTreatedData->GetBackEMult();
   for (unsigned int i = 0 ; i < mysize ;i++) {
      myMULT[PreTreatedData->GetBackEDetectorNbr(i)-1] += 1;  
   }

   for (unsigned int i = 0; i < fNumberOfDetectors; i++) {
      index= "W1/CAL/MULT/W1_D"+NPL::itoa(i+1)+"_STR_BACK_CAL_MULT";
      GetHisto(index)->Fill(myMULT[i]);
   }
}



////////////////////////////////////////////////////////////////////////////////
void TW1Spectra::FillPhysicsSpectra(TW1Physics* Physics)
{
/*   static string index;

   // Kine plot
   unsigned int mysize = Physics->Strip_E.size();
   for(unsigned int i = 0 ; i < mysize ; i++){
      double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
      Theta = Theta/deg;
      double Etot=Physics->Strip_E[i];

      if(Physics->PAD_E[i]>0){
         index = "W1/PHY/W1_PAD_E_E";
         Etot += Physics->PAD_E[i];
         GetHisto(index)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);
         index = "W1/PHY/W1"+NPL::itoa(Physics->DetectorNumber[i])+"_PAD_E_E";
         GetHisto(index)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);

      }
      index = "W1/PHY/W1_THETA_E";
      GetHisto(index)-> Fill(Theta,Etot);
   }*/
}

