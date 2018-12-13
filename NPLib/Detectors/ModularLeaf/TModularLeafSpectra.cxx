/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Valerian alcindor  contact address: girardalcindor@ganil.fr
 *                                                      alcindor@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : May 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ModularLeaf Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TModularLeafSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"
// #include "NPVSpectra.h"

////////////////////////////////////////////////////////////////////////////////
TModularLeafSpectra::TModularLeafSpectra() 
{
     SetName("ModularLeaf");
}

////////////////////////////////////////////////////////////////////////////////
TModularLeafSpectra::TModularLeafSpectra(std::vector<std::string> leafs, std::vector<string> leafs_X, std::vector <string> leafs_Y) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TModularLeafSpectra : Initalizing control spectra for " 
      <<  leafs.size() << " Leafs" << endl
      << "************************************************" << endl ;
  SetName("ModularLeaf");
  fleafs = leafs;
  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra(leafs_X, leafs_Y);
}

////////////////////////////////////////////////////////////////////////////////
TModularLeafSpectra::~TModularLeafSpectra() {
  delete f_ModularHisto;
}

////////////////////////////////////////////////////////////////////////////////
void TModularLeafSpectra::InitRawSpectra() {
  static string name;
  unsigned int leafs_size = fleafs.size();
  for (unsigned int i = 0; i < leafs_size; i++) { // loop on number of detectors
    name = "NP_" + fleafs[i];
    if (fleafs[i] == "GATCONFMASTER" || fleafs[i] == "GATCONFSLAVE"){
      AddHisto1D(name, name, 8400, 0, 8400, "ModularLeaf/RAW");
    }
    else{
      AddHisto1D("NP_" + fleafs[i], "NP_" + fleafs[i], 512, 0, 16384, "ModularLeaf/RAW");
    }
    // cout << fleafs[i] << endl;
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TModularLeafSpectra::InitPreTreatedSpectra() {
  static string name;
  unsigned int leafs_size = fleafs.size();
  for (unsigned int i = 0; i < leafs_size; i++) { // loop on number of detectors
    name = "NP_" + fleafs[i]+ "_CAL";
    char T = 'T'; char E = 'E'; char X = 'X'; 
    if (fleafs[i].at(0) == T){
      AddHisto1D(name, name, 10000, 0, 1000, "ModularLeaf/CAL");
    }
    else{
      AddHisto1D(name, name, 1000, 0, 16000, "ModularLeaf/CAL");
    }
  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TModularLeafSpectra::InitPhysicsSpectra(std::vector <string>& leafs_X, std::vector <string>& leafs_Y) {
  static string name;

  int leafs_size;
  if(leafs_X.size() == leafs_Y.size()){
   leafs_size = leafs_X.size(); 
  }
  else{
    cout << "ERROR X SIZE DIFFERENT OF Y SIZE" << endl;
    return exit(1); 
  }
  for(int i = 0; i < leafs_size ; i++){
    name = leafs_Y[i] + ":" + leafs_X[i];
    if(leafs_X[i] == "T_PPT_CATS" && leafs_Y[i] == "PPT_Q"){
    AddHisto2D(name, name, 1000, 0, 10, 10000, 0, 1000, "ModularLeaf/PHY");
    } 
    else{
    AddHisto2D(name, name, 512, 0, 16384, 512, 0, 16384, "ModularLeaf/PHY");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void TModularLeafSpectra::FillRawSpectra(std::map<std::string, short>& RawData) {
  static string name;
  static string family;
  short value;
  std::map<std::string, short>::iterator it;
  for (it = RawData.begin(); it != RawData.end(); it++){
    name = "NP_" + it->first;
    value = it->second;
    family = "ModularLeaf/RAW";
    FillSpectra(family, name, value);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TModularLeafSpectra::FillPreTreatedSpectra(std::map<std::string, double>& CalibratedData) {
  static string name;
  static string family;
  double value;

  std::map<std::string, double>::iterator it;
  for (it = CalibratedData.begin(); it != CalibratedData.end(); it++){
    name = "NP_" + it->first + "_CAL";
    value = it->second;
    family = "ModularLeaf/CAL";
    FillSpectra(family, name, value);
  }

  // // Time
  // unsigned int sizeT = PreTreatedData->GetMultTime();
  // for (unsigned int i = 0; i < sizeT; i++) {
  //   name = "ModularLeaf"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
  //   family = "ModularLeaf/CAL";

  //   GetHisto(family,name) -> Fill(PreTreatedData->Get_Time(i));
  // }
}

////////////////////////////////////////////////////////////////////////////////
void TModularLeafSpectra::FillPhysicsSpectra(std::vector<string>& leafs_X,
    std::vector<string>& leafs_Y,
    std::map<std::string, double>& CalibratedData) {

  static string name;
  static string family;
  double x = 0.;
  double y = 0.;
  int leafs_size;
  if(leafs_X.size() == leafs_Y.size()){
   leafs_size = leafs_X.size(); 
  }
  else{
    cout << "ERROR X SIZE DIFFERENT OF Y SIZE" << endl;
    return exit(1); 
  }

  for(int i = 0; i < leafs_size ; i++){
    std::map<std::string, double>::iterator it1;
    for (it1 = CalibratedData.begin(); it1 != CalibratedData.end(); it1++){
      if(it1->first == leafs_X[i]){
        x = it1->second;
      }
    }
    std::map<std::string, double>::iterator it2;
    for (it2 = CalibratedData.begin(); it2 != CalibratedData.end(); it2++){
      if(it2->first == leafs_Y[i]){
        y = it2->second;
      }
    }

    name = leafs_Y[i] + ":" + leafs_X[i];
    family = "ModularLeaf/PHY";
    FillSpectra(family, name, x, y);
  }
}

