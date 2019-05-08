/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 ****************************************************************************/

/*****************************************************************************
 * Original Author: R. Wilkinson   contact address: r.wilkinson@surrey.ac.uk *
 * Creation Date  : June 2017                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for EXL                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TExlSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// STL
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
TExlSpectra::TExlSpectra(){
  SetName("Exl");
  fNumberOfDetector = 0;
  fNumberOfCrystal = 18;
}

////////////////////////////////////////////////////////////////////////////////
TExlSpectra::TExlSpectra(unsigned int NumberOfDetector){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TExlSpectra : Initalising control spectra for "
      << NumberOfDetector << " EXL Detectors" << endl
      << "************************************************" << endl ;

  SetName("Exl");
  fNumberOfDetector = NumberOfDetector;
  fNumberOfCrystal = 18;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TExlSpectra::~TExlSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TExlSpectra::InitRawSpectra(){
  string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    TString nbr = NPL::itoa(i+1);

    // CSI_E_RAW
    // name = "EXL"+NPL::itoa(i+1)+"_CSI_E_RAW";
    // AddHisto2D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, 512, 0, 16384, "EXL/RAW/CSIE");

    // // CSI_T_RAW
    // name = "EXL"+NPL::itoa(i+1)+"_CSI_T_RAW";
    // AddHisto2D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, 512, 0, 8192, "EXL/RAW/CSIT");

    // CSI_RAW_MULT
    name = "EXL"+NPL::itoa(i+1)+"_CSI_RAW_MULT";
    AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "EXL/RAW/MULT");

    // // CSI_RAW_DETECTOR_HITMAP
    // name = "EXL"+NPL::itoa(i+1)+"_CSI_RAW_DETECTOR_HITMAP";
    // AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "EXL/RAW/CSIE");

    for (unsigned int j = 0; j < fNumberOfCrystal; j++) { // loop on number of crystals
      // CSI_E_RAW
      name = "EXL"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_E_RAW";
      AddHisto1D(name, name, 512, 0, 16384, "EXL/RAW/CSIE");

      // CSI_T_RAW
      name = "EXL"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_T_RAW";
      AddHisto1D(name, name, 512, 0, 16384, "EXL/RAW/CSIT");
    }
     name = "EXL_CSIE_CrystalNumber_Raw";
     AddHisto2D(name, name,18, 1, 19, 512, 0, 16384, "EXL/RAW");

     name = "EXL_CSIT_CrystalNumber_Raw";
     AddHisto2D(name, name,18, 1, 19, 512, 0, 16384, "EXL/RAW");
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TExlSpectra::InitPreTreatedSpectra()
{
  // string name;
  // for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
  //   // CSI_E_CAL
  //   name = "EXL"+NPL::itoa(i+1)+"_CSI_E_CAL";
  //   AddHisto2D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, 500, 0, 500, "EXL/CAL/CSIE");

  //   // CSI_T_CAL
  //   name = "EXL"+NPL::itoa(i+1)+"_CSI_T_CAL";
  //   AddHisto2D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, 500, 0, 50, "EXL/CAL/CSIT");

  //   // CSI_CAL_MULT
  //   name = "EXL"+NPL::itoa(i+1)+"_CSI_CAL_MULT";
  //   AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "EXL/CAL/MULT");

  //   // CSI_RAW_DETECTOR_HITMAP
  //   name = "EXL"+NPL::itoa(i+1)+"_CSI_CAL_DETECTOR_HITMAP";
  //   AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "EXL/CAL/CSIE");

  //   for (unsigned int j = 0; j < fNumberOfCrystal; j++) { // loop on number of crystals
  //     // CSI(X)_E_CAL
  //     name = "EXL"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_E_CAL";
  //     AddHisto1D(name, name, 500, 0, 500, "EXL/CAL/CSIE");

  //     // CSI(X)_T_CAL
  //     name = "EXL"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_T_CAL";
  //     AddHisto1D(name, name, 500, 0, 50, "EXL/CAL/CSIT");
  //   }
  // }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TExlSpectra::InitPhysicsSpectra(){
  string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop over number of detectors
    TString nbr = NPL::itoa(i+1);
    for (unsigned int j = 0; j < fNumberOfCrystal; j++) { // loop on number of crystals
      // CSI_E
      name = "EXL"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_E_CAL";
      AddHisto1D(name, name, 1000, 0, 10, "EXL/PHY/CSIE");

      // CSI_T
      name = "EXL"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_T_CAL";
      AddHisto1D(name, name, 10000, 0, 1000, "EXL/PHY/CSIT");
    }
     name = "EXL_CSIE_CrystalNumber";
     AddHisto2D(name, name,18, 1, 19, 1000, 0, 10, "EXL/PHY");

     name = "EXL_CSIT_CrystalNumber";
     AddHisto2D(name, name,18, 1, 19, 1000, 0, 10, "EXL/PHY");

     // Hit map
     name = "EXL_CSI_2D_HITMAP";
     AddHisto2D(name, name,6,0,6,3,0,3, "EXL/PHY");

    // AddBack
    // name = "EXL_CSI_ADDBACK";
    // AddHisto1D(name, name, 500, 0, 500, "EXL/PHY");
  } // loop on number of detectors
}


////////////////////////////////////////////////////////////////////////////////
void TExlSpectra::FillRawSpectra(TExlData* RawData){
  static string name;
  static string family;

  // // CSI_E_RAW
  // for (unsigned int i = 0; i < RawData->GetMult(); i++) {
  //   name = "EXL"+NPL::itoa(RawData->GetExlNumber(i))+"_CSI_E_RAW";
  //   family = "EXL/RAW/CSIE";

  //   FillSpectra(family,name
  //     ,RawData->GetCrystalNumber(i),
  //         RawData->GetEnergy(i));
  //   // cout << RawData->GetEnergy(i) << endl;
  // }

  // // CSI_T_RAW
  // for (unsigned int i = 0; i < RawData->GetMult(); i++) {
  //   name = "EXL"+NPL::itoa(RawData->GetExlNumber(i))+"_CSI_T_RAW";
  //   family = "EXL/RAW/CSIT";

  //   FillSpectra(family,name
  //     ,RawData->GetCrystalNumber(i),
  //         RawData->GetTime(i));
  // }

  // CSI_RAW_MULT
  int myMULT[fNumberOfDetector];
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ;

  for(unsigned int i = 0 ; i < RawData->GetEMult();i++){
    myMULT[RawData->GetE_ExlNumber(i)-1] += 1;
  }

  // for( unsigned int i = 0; i < fNumberOfDetector; i++){
  //   name = "EXL"+NPL::itoa(i+1)+"_CSI_RAW_MULT";
  //   family= "EXL/RAW/MULT";
  //   FillSpectra(family,name
  //     ,myMULT[i]);
  // }

  // CSI_RAW_DETECTOR_HITMAP
  // for (unsigned int i = 0; i < RawData->GetMult(); i++) {
  //   name = "EXL"+NPL::itoa(RawData->GetExlNumber(i))+"_CSI"+NPL::itoa(RawData->GetCrystalNumber(i))+"_E_RAW";
  //   family = "EXL/RAW/CSIE";

  //   FillSpectra(family,name
  //     ,RawData->GetCrystalNumber(i));
  // }

  // CSI(X)_E_RAW
   for (unsigned int i = 0; i < RawData->GetEMult(); i++) {
     name = "EXL"+NPL::itoa(RawData->GetE_ExlNumber(i))+"_CSI"+NPL::itoa(RawData->GetE_CrystalNumber(i))+"_E_RAW";
     family = "EXL/RAW/CSIE";

     FillSpectra(family,name
       ,RawData->GetEnergy(i));
   }

  // CSI(X)_T_RAW
  for (unsigned int i = 0; i < RawData->GetEMult(); i++) {
    name = "EXL"+NPL::itoa(RawData->GetE_ExlNumber(i))+"_CSI"+NPL::itoa(RawData->GetE_CrystalNumber(i))+"_T_RAW";
    family = "EXL/RAW/CSIT";

    FillSpectra(family,name
      ,RawData->GetTime(i));
  }

  // CsIE vs CrystalNumber
   for (unsigned int i = 0; i < RawData->GetEMult(); i++) {
     name = "EXL_CSIE_CrystalNumber_Raw";
     family = "EXL/RAW/";
     FillSpectra(family,name,
       RawData->GetE_CrystalNumber(i), RawData->GetEnergy(i));
   }
  // CsIT vs CrystalNumber
   for (unsigned int i = 0; i < RawData->GetTMult(); i++) {
     name = "EXL_CSIT_CrystalNumber_Raw";
     family = "EXL/RAW/";
     FillSpectra(family,name,
       RawData->GetT_CrystalNumber(i), RawData->GetTime(i));
   }
}

////////////////////////////////////////////////////////////////////////////////
void TExlSpectra::FillPreTreatedSpectra(TExlData* PreTreatedData){
  // string name ;
  // string family;

  // // CSI_E_CAL
  // for (unsigned int i = 0; i < PreTreatedData->GetMult(); i++) {
  //   name = "EXL_CSI"+NPL::itoa(PreTreatedData->GetExlNumber(i))+"_CSI_E_CAL";
  //   family = "EXL/CAL/CSIE";

  //   FillSpectra(family,name
  //     ,PreTreatedData->GetCrystalNumber(i),
  //         PreTreatedData->GetEnergy(i));
  // }

  // // CSI_T_CAL
  // for (unsigned int i = 0; i < PreTreatedData->GetMult(); i++) {
  //   name = "EXL_CSI"+NPL::itoa(PreTreatedData->GetExlNumber(i))+"_CSI_T_CAL";
  //   family = "EXL/CAL/CSIT";

  //   FillSpectra(family,name
  //     ,PreTreatedData->GetCrystalNumber(i),
  //         PreTreatedData->GetTime(i));
  // }

  // // CSI_CAL_MULT
  // int myMULT[fNumberOfDetector];
  // for( unsigned int i = 0; i < fNumberOfDetector; i++)
  //   myMULT[i] = 0 ;

  // for(unsigned int i = 0 ; i < PreTreatedData->GetMult();i++){
  //   myMULT[PreTreatedData->GetExlNumber(i)-1] += 1;
  // }

  // for( unsigned int i = 0; i < fNumberOfDetector; i++){
  //   name = "EXL"+NPL::itoa(i+1)+"_CSI_CAL_MULT";
  //   family= "EXL/CAL/MULT";
  //   FillSpectra(family,name
  //     ,myMULT[i]);
  // }

  // // CSI_CAL_DETECTOR_HIMAP
  // for (unsigned int i = 0; i < PreTreatedData->GetMult(); i++) {
  //   name = "EXL"+NPL::itoa(PreTreatedData->GetExlNumber(i))+"_CSI"+NPL::itoa(PreTreatedData->GetCrystalNumber(i))+"_E_CAL";
  //   family = "EXL/CAL/CSIE";

  //   FillSpectra(family,name
  //     ,PreTreatedData->GetCrystalNumber(i));
  // }

  // // CSI(X)_E_CAL
  // for (unsigned int i = 0; i < PreTreatedData->GetMult(); i++) {
  //   name = "EXL_CSI"+NPL::itoa(PreTreatedData->GetCrystalNumber(i))+"_E_CAL";
  //   family = "EXL/CAL/CSIE";

  //   FillSpectra(family,name
  //     ,PreTreatedData->GetEnergy(i));
  // }

  // // CSI(X)_T_CAL
  // for (unsigned int i = 0; i < PreTreatedData->GetMult(); i++) {
  //   name = "EXL_CSI"+NPL::itoa(PreTreatedData->GetCrystalNumber(i))+"_T_CAL";
  //   family = "EXL/CAL/CSIT";

  //   FillSpectra(family,name
  //     ,PreTreatedData->GetTime(i));
  // }
}

////////////////////////////////////////////////////////////////////////////////
void TExlSpectra::FillPhysicsSpectra(TExlPhysics* Physics){
  // string name;
  // string family= "EXL/PHY";
  
  static string name;
  static string family;

  // CSI_E
   for (unsigned int i = 0; i < Physics->GetMult(); i++) {
     name = "EXL"+NPL::itoa(Physics->GetExlNumber(i))+"_CSI"+NPL::itoa(Physics->GetCrystalNumber(i))+"_E_CAL";
     family = "EXL/CAL/CSIE";

     FillSpectra(family,name
       ,Physics->GetEnergy(i));
  }

  // // CSI_T
  // for (unsigned int i = 0; i < Physics->GetMult(); i++) {
  //   name = "EXL"+NPL::itoa(Physics->GetExlNumber(i))+"_CSI"+NPL::itoa(Physics->GetCrystalNumber(i))+"_T_CAL";
  //   family = "EXL/CAL/CSIT";

  //   FillSpectra(family,name
  //         Physics->GetTime(i));
  // }

  // CsIE vs CrystalNumber
   for (unsigned int i = 0; i < Physics->GetMult(); i++) {
     name = "EXL_CSIE_CrystalNumber";
     family = "EXL/PHY/";
     FillSpectra(family,name,
       Physics->GetCrystalNumber(i), Physics->GetEnergy(i));
   }

  // // CsIT vs CrystalNumber
  //  for (unsigned int i = 0; i < Physics->GetMult(); i++) {
  //    name = "EXL_CSIT_CrystalNumber";
  //    family = "EXL/PHY/";
  //    FillSpectra(family,name,
  //      Physics->GetCrystalNumber(i), Physics->GetTime(i));
  //  }
  
  double E = 0;
  double x = -1;
  double y = -1;

  for (unsigned int i = 0 ; i < Physics->Energy.size(); i++){
    E += Physics->Energy[i];
    name = "EXL_CSI_2D_HITMAP";
    family = "EXL/PHY";
    switch (Physics->CrystalNumber[i]) {
      case 1:
        x =  0;
        y =  2;
        FillSpectra(family,name,x,y);
        break;
      case 2:
        x =  1;
        y =  2;
        FillSpectra(family,name,x,y);
        break;
      case 3:
        x =  2;
        y =  2;
        FillSpectra(family,name,x,y);
        break;
      case 4:
        x =  3;
        y =  2;
        FillSpectra(family,name,x,y);
        break;
      case 5:
        x =  4;
        y =  2;
        FillSpectra(family,name,x,y);
        break;
      case 6:
        x =  5;
        y =  2;
        FillSpectra(family,name,x,y);
        break;
      case 7:
        x =  0;
        y =  1;
        FillSpectra(family,name,x,y);
        break;
      case 8:
        x =  1;
        y =  1;
        FillSpectra(family,name,x,y);
        break;
      case 9:
        x =  2;
        y =  1;
        FillSpectra(family,name,x,y);
        break;
      case 10:
        x =  3;
        y =  1;
        FillSpectra(family,name,x,y);
        break;
      case 11:
        x =  4;
        y =  1;
        FillSpectra(family,name,x,y);
        break;
      case 12:
        x =  5;
        y =  1;
        FillSpectra(family,name,x,y);
        break;
      case 13:
        x =  0;
        y =  0;
        FillSpectra(family,name,x,y);
        break;
      case 14:
        x =  1;
        y =  0;
        FillSpectra(family,name,x,y);
        break;
      case 15:
        x =  2;
        y =  0;
        FillSpectra(family,name,x,y);
        break;
      case 16:
        x =  3;
        y =  0;
        FillSpectra(family,name,x,y);
        break;
      case 17:
        x =  4;
        y =  0;
        FillSpectra(family,name,x,y);
        break;
      case 18:
        x =  5;
        y =  0;
        FillSpectra(family,name,x,y);
        break;
      default:
        FillSpectra(family,name,-1,-1); // fill the underflow in case of problems
        break;
    }
  }

  // // AddBack
  // name = "EXL_CSI_ADDBACK";
  // FillSpectra(family, name, E);
}
