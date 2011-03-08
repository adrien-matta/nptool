/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : march 2011                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Must2                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TMust2Spectra.h"

#include "TString.h"


ClassImp(TMust2Spectra)

TMust2Spectra::TMust2Spectra()
   : fMust2Data(new TMust2Data()),
     fMust2PreTreatedData(new TMust2Data()),
     fMust2Physics(new TMust2Physics())
{
   fMust2PreTreatedData = fMust2Physics->GetPreTreatedData();
}



TMust2Spectra::~TMust2Spectra()
{
}



void TMust2Spectra::InitRawUserSpectra()
{
   TString name;
   for (Int_t i = 0; i < NUMBERMUST2; ++i) {   // loop on number of detectors
      // STRX_MULT
      name = Form("MM%d_STRX_MULT", i+1);
      MM_STRX_MULT[i] = AddTH1(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, "MUST2/BRUT/MULT");

      // STRY_MULT
      name = Form("MM%d_STRY_MULT", i+1);
      MM_STRY_MULT[i] = AddTH1(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, "MUST2/BRUT/MULT");

      // SILI_MULT
      name = Form("MM%d_SILI_MULT", i+1);
      MM_SILI_MULT[i] = AddTH1(name, name, NUMBERSILI, 1, NUMBERSILI+1, "MUST2/BRUT/MULT");

      // CSI_MULT
      name = Form("MM%d_CSI_MULT", i+1);
      MM_CSI_MULT[i] = AddTH1(name, name, NUMBERCSI, 1, NUMBERCSI+1, "MUST2/BRUT/MULT");
   } // end loop on number of detectors
}



void TMust2Spectra::InitUserSpectra()
{
   TString name;
   for (Int_t i = 0; i < NUMBERMUST2; i++) { // loop on number of detectors
      // STRX_E_BRU
      name = Form("MM%d_STRX_E_BRU", i+1);
      MM_STRX_E_BRU[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 512, 0, 8192, "MUST2/BRUT/STRXE");

      // STRY_E_BRU
      name = Form("MM%d_STRY_E_BRU", i+1);
      MM_STRY_E_BRU[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 512, 0, 8192, "MUST2/BRUT/STRYE");

      // STRX_T_BRU
      name = Form("MM%d_STRX_T_BRU", i+1);
      MM_STRX_T_BRU[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 512, 0, 8192, "MUST2/BRUT/STRXT");

      // STRY_T_BRU
      name = Form("MM%d_STRY_T_BRU", i+1);
      MM_STRY_T_BRU[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 512, 0, 8192, "MUST2/BRUT/STRYT");

      // SILI_E_BRU
      name = Form("MM%d_SILI_E_BRU", i+1);
      MM_SILI_E_BRU[i] = AddTH2(name, name, NUMBERSILI, 1, NUMBERSILI+1, 512, 0, 8192, "MUST2/BRUT/SILIE");

      // SILI_T_BRU
      name = Form("MM%d_SILI_T_BRU", i+1);
      MM_SILI_T_BRU[i] = AddTH2(name, name, NUMBERSILI, 1, NUMBERSILI+1, 512, 0, 8192, "MUST2/BRUT/SILIT");

      // CSI_E_BRU
      name = Form("MM%d_CSI_E_BRU", i+1);
      MM_CSI_E_BRU[i] = AddTH2(name, name, NUMBERCSI, 1, NUMBERCSI+1, 512, 0, 8192, "MUST2/BRUT/CSIE");

      // CSI_T_BRU
      name = Form("MM%d_CSI_T_BRU", i+1);
      MM_CSI_T_BRU[i] = AddTH2(name, name, NUMBERCSI, 1, NUMBERCSI+1, 512, 0, 8192, "MUST2/BRUT/CSIT");

      // STRX_E_CAL
      name = Form("MM%d_STRX_E_CAL", i+1);
      MM_STRX_E_CAL[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 500, 0, 50, "MUST2/CAL/STRXE");

      // STRY_E_CAL
      name = Form("MM%d_STRY_E_CAL", i+1);
      MM_STRY_E_CAL[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 500, 0, 50, "MUST2/CAL/STRYE");

      // STRX_T_CAL
      name = Form("MM%d_STRX_T_CAL", i+1);
      MM_STRX_T_CAL[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 500, 0, 500, "MUST2/CAL/STRXT");

      // STRY_T_CAL
      name = Form("MM%d_STRY_T_CAL", i+1);
      MM_STRY_T_CAL[i] = AddTH2(name, name, NUMBERSTRIPS, 1, NUMBERSTRIPS+1, 500, 0, 500, "MUST2/CAL/STRYT");

      // SILI_E_CAL
      name = Form("MM%d_SILI_E_CAL", i+1);
      MM_SILI_E_CAL[i] = AddTH2(name, name, NUMBERSILI, 1, NUMBERSILI+1, 500, 0, 50, "MUST2/CAL/SILIE");

      // SILI_T_CAL
      name = Form("MM%d_SILI_T_CAL", i+1);
      MM_SILI_T_CAL[i] = AddTH2(name, name, NUMBERSILI, 1, NUMBERSILI+1, 500, 0, 50, "MUST2/CAL/SILIT");

      // CSI_E_CAL
      name = Form("MM%d_CSI_E_CAL", i+1);
      MM_CSI_E_CAL[i] = AddTH2(name, name, NUMBERCSI, 1, NUMBERCSI+1, 500, 0, 50, "MUST2/CAL/CSIE");

      // CSI_T_CAL
      name = Form("MM%d_CSI_T_CAL", i+1);
      MM_CSI_T_CAL[i] = AddTH2(name, name, NUMBERCSI, 1, NUMBERCSI+1, 500, 0, 50, "MUST2/CAL/CSIT");
   }  // end loop on number of detectors
}



void TMust2Spectra::FillRawUserSpectra()
{
   
}



void TMust2Spectra::FillUserSpectra()
{
   // STRX_E 
   for (Int_t i = 0; i < fMust2Data->GetMMStripXEMult(); i++) {
      MM_STRX_E_BRU[fMust2Data->GetMMStripXEDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMStripXEStripNbr(i), fMust2Data->GetMMStripXEEnergy(i) - 8192);
   }

   // STRY_E
   for (Int_t i = 0; i < fMust2Data->GetMMStripYEMult(); i++) {
      MM_STRY_E_BRU[fMust2Data->GetMMStripYEDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMStripYEStripNbr(i), 8192 - fMust2Data->GetMMStripYEEnergy(i));
   }

   // STRX_T
   for (Int_t i = 0; i < fMust2Data->GetMMStripXTMult(); i++) {
      MM_STRX_T_BRU[fMust2Data->GetMMStripXTDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMStripXTStripNbr(i), fMust2Data->GetMMStripXTTime(i) - 8192);
   }
   // STRY_T
   for (Int_t i = 0; i < fMust2Data->GetMMStripYTMult(); i++) {
      MM_STRY_T_BRU[fMust2Data->GetMMStripYTDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMStripYTStripNbr(i), 8192 - fMust2Data->GetMMStripYTTime(i));
   }

   // SILI_E
   for (Int_t i = 0; i < fMust2Data->GetMMSiLiEMult(); i++) {
      MM_SILI_E_BRU[fMust2Data->GetMMSiLiEDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMSiLiEPadNbr(i), fMust2Data->GetMMSiLiEEnergy(i));
   }

   // SILI_T
   for (Int_t i = 0; i < fMust2Data->GetMMSiLiTMult(); i++) {
      MM_SILI_T_BRU[fMust2Data->GetMMSiLiTDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMSiLiTPadNbr(i), fMust2Data->GetMMSiLiTTime(i));
   }

   // CSI_E
   for (Int_t i = 0; i < fMust2Data->GetMMCsIEMult(); i++) {
      MM_CSI_E_BRU[fMust2Data->GetMMCsIEDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMCsIECristalNbr(i), fMust2Data->GetMMCsIEEnergy(i));
   }

   // CSI_T
   for (Int_t i = 0; i < fMust2Data->GetMMCsITMult(); i++) {
      MM_CSI_T_BRU[fMust2Data->GetMMCsITDetectorNbr(i) - 1] -> Fill(fMust2Data->GetMMCsITCristalNbr(i), fMust2Data->GetMMCsITTime(i));
   }

   // fill calibrated user spectra
   // STRX_E
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMStripXEMult(); i++) {
      MM_STRX_E_CAL[fMust2PreTreatedData->GetMMStripXEDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMStripXEStripNbr(i), fMust2PreTreatedData->GetMMStripXEEnergy(i));
   }
   // STRY_E
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMStripYEMult(); i++) {
      MM_STRY_E_CAL[fMust2PreTreatedData->GetMMStripYEDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMStripYEStripNbr(i), fMust2PreTreatedData->GetMMStripYEEnergy(i));
   }
   // STRX_T
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMStripXTMult(); i++) {
      MM_STRX_T_CAL[fMust2PreTreatedData->GetMMStripXTDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMStripXTStripNbr(i), fMust2PreTreatedData->GetMMStripXTTime(i));
   }
   // STRY_T
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMStripYTMult(); i++) {
      MM_STRY_T_CAL[fMust2PreTreatedData->GetMMStripYTDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMStripYTStripNbr(i), fMust2PreTreatedData->GetMMStripYTTime(i));
   }
   // SILI_E
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMSiLiEMult(); i++) {
      MM_SILI_E_CAL[fMust2PreTreatedData->GetMMSiLiEDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMSiLiEPadNbr(i), fMust2PreTreatedData->GetMMSiLiEEnergy(i));
   }
   // SILI_T
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMSiLiTMult(); i++) {
      MM_SILI_T_CAL[fMust2PreTreatedData->GetMMSiLiTDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMSiLiTPadNbr(i), fMust2PreTreatedData->GetMMSiLiTTime(i));
   }
   // CSI_E
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMCsIEMult(); i++) {
      MM_CSI_E_CAL[fMust2PreTreatedData->GetMMCsIEDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMCsIECristalNbr(i), fMust2PreTreatedData->GetMMCsIEEnergy(i));
   }
   // CSI_T
   for (Int_t i = 0; i < fMust2PreTreatedData->GetMMCsITMult(); i++) {
         MM_CSI_T_CAL[fMust2PreTreatedData->GetMMCsITDetectorNbr(i) - 1] -> Fill(fMust2PreTreatedData->GetMMCsITCristalNbr(i), fMust2PreTreatedData->GetMMCsITTime(i));
      }
}



TH1* TMust2Spectra::AddTH1(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, const char* family)
{
   // create histo
   TH1 *hist = new TH1I(name, title, nbinsx, xlow, xup);

   // fill map
   fMapHisto[family].push_back(hist);

   return hist;
}



TH1* TMust2Spectra::AddTH2(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, const char* family)
{
   // create histo
   TH1 *hist = new TH2I(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);

   // fill map
   fMapHisto[family].push_back(hist);

   return hist;
}
