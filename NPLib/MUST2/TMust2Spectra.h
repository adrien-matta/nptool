#ifndef __MUST2SPECTRA__
#define __MUST2SPECTRA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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

// C++ STL headers
#include <map>
#include <string>

// ROOT headers
#include "TObject.h"
#include <TH1.h>
#include <TH2.h>

// NPLib headers
#include "TMust2Data.h"
#include "TMust2Physics.h"

using namespace std;

#define NUMBERMUST2  8
#define NUMBERSTRIPS 128
#define NUMBERSILI   16
#define NUMBERCSI    16


class TMust2Spectra : public TObject {
 public:
   // constructor and destructor
	TMust2Spectra();
	virtual ~TMust2Spectra();

   // Instantiate and register histo to maps
   TH1* AddTH1(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, const char* family = "MUST2");
   TH1* AddTH2(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, const char* family = "MUST2");

   // Initialization methods
   void InitRawUserSpectra();
   void InitUserSpectra();
   // Filling methods
   void FillRawUserSpectra();
   void FillUserSpectra();

	// getters and setters
   // getters
   TMust2Data*	   GetMust2Data()             const {return fMust2Data;}
   TMust2Data*	   GetMust2PreTreatedData()   const {return fMust2PreTreatedData;}
	TMust2Physics*	GetMust2Physics()          const {return fMust2Physics;}
   // setters
   void SetMust2Data(TMust2Data*           must2Data)           {fMust2Data           = must2Data;}
   void SetMust2PreTreatedData(TMust2Data* must2PreTreatedData) {fMust2PreTreatedData = must2PreTreatedData;}
   void SetMust2Data(TMust2Physics*        must2Physics)        {fMust2Physics        = must2Physics;}

   // get map histo which will be used for GSpectra in GUser
   map< string, vector<TH1*> > GetMapHisto() const {return fMapHisto;}


 private:
	// Data and physics class for Must2
	TMust2Data     *fMust2Data;
	TMust2Data     *fMust2PreTreatedData;
	TMust2Physics  *fMust2Physics;

 private:
   // map holding histo pointers and their family names
   map< string, vector<TH1*> > fMapHisto;

 private:
   // monodim
   TH1 *MM_STRX_MULT[NUMBERMUST2];  // dim = 128
   TH1 *MM_STRY_MULT[NUMBERMUST2];  // dim = 128
   TH1 *MM_SILI_MULT[NUMBERMUST2];  // dim = 16
   TH1 *MM_CSI_MULT[NUMBERMUST2];   // dim = 16

   // bidim
   TH1 *MM_STRX_E_BRU[NUMBERMUST2];   // dim = 128 x 512
   TH1 *MM_STRY_E_BRU[NUMBERMUST2];
   TH1 *MM_STRX_T_BRU[NUMBERMUST2];
   TH1 *MM_STRY_T_BRU[NUMBERMUST2];
   // SiLi E
   TH1 *MM_SILI_E_BRU[NUMBERMUST2];   // dim = 16 x 512
   TH1 *MM_SILI_T_BRU[NUMBERMUST2];   // dim = 16 x 512
   // CsI E
   TH1 *MM_CSI_E_BRU[NUMBERMUST2];    // dim = 16 x 512
   TH1 *MM_CSI_T_BRU[NUMBERMUST2];    // dim = 16 x 512
   // Strips (X,Y) (E.T)
   TH1 *MM_STRX_E_CAL[NUMBERMUST2];   // dim = 128 x 500 (50 MeV range)
   TH1 *MM_STRY_E_CAL[NUMBERMUST2];
   TH1 *MM_STRX_T_CAL[NUMBERMUST2];   // dim = 128 x 500 (500 ns range)
   TH1 *MM_STRY_T_CAL[NUMBERMUST2];
   // SiLi E
   TH1 *MM_SILI_E_CAL[NUMBERMUST2];   // dim = 16 x 512
   TH1 *MM_SILI_T_CAL[NUMBERMUST2];   // dim = 16 x 512
   // CsI E
   TH1 *MM_CSI_E_CAL[NUMBERMUST2];    // dim = 16 x 512
   TH1 *MM_CSI_T_CAL[NUMBERMUST2];    // dim = 16 x 512

	ClassDef(TMust2Spectra,1)  // Must2Spectra structure
};

#endif
