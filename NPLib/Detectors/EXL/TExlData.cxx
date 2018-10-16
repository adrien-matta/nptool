/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $GANIL2ROOT/Licence                           *
 * For the list of contributors see $GANIL2ROOT/README/CREDITS               *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                    							                                         *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds EXL Raw data                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include "TExlData.h"

ClassImp(TExlData)

TExlData::TExlData()
{
}

TExlData::~TExlData()
{
}

void TExlData::Clear()
{
    fExlE_Energy.clear();
    fExlE_Number.clear();
    fExlE_Crystal.clear();
    
    fExlT_Time.clear();
    fExlT_Number.clear();
    fExlT_Crystal.clear();

}

void TExlData::Dump() const
{
    cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

    cout << "Energy:" << endl;
    for(unsigned short i = 0 ; i<fExlE_Energy.size() ; i ++)
    {
        cout << "Exl Number " << fExlE_Number[i]
            << " Crystal Number " << fExlE_Crystal[i]
            << " Energy: " << fExlE_Energy[i] << endl;
    }

    cout << "Time:" << endl;
    for(unsigned short i = 0 ; i<fExlT_Time.size() ; i ++)
    {
        cout << "Exl Number " << fExlT_Number[i]
            << " Crystal Number " << fExlT_Crystal[i]
            << " Time: " << fExlT_Time[i] << endl;
    }

}

double TExlData::GetEnergyCrystalNumber(const int crystal){
  double energy = -1000;
  unsigned int sizeE = GetEMult();
  for (unsigned int i = 0; i < sizeE; i++){
    if (GetE_CrystalNumber(i) == crystal){
      energy = GetEnergy(i);
      // return GetEnergy(i); 
    }
  }
  return energy;
} 

double TExlData::GetTimeCrystalNumber(const int crystal){
  double time = -1000;
  unsigned int sizeT = GetTMult();
  for (unsigned int i = 0; i < sizeT; i++){
    if (GetT_CrystalNumber(i)==crystal){
      time = GetTime(i);
      // return GetTime(i); 
    }
  }
  return time;
} 

