/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold QQQ Raw data                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TQQQData.h"

ClassImp(TQQQData)

////////////////////////////////////////////////////////////////////////////////
TQQQData::TQQQData(){
   // Default constructor

   // QQQ
   // Energy
   fQQQ_StripE_DetectorNbr.clear();
   fQQQ_StripE_StripNbr.clear();
   fQQQ_StripE_Energy.clear();
   // Time
   fQQQ_StripT_DetectorNbr.clear();
   fQQQ_StripT_StripNbr.clear();
   fQQQ_StripT_Time.clear();

}

////////////////////////////////////////////////////////////////////////////////
TQQQData::~TQQQData(){
}

////////////////////////////////////////////////////////////////////////////////
void TQQQData::Clear(){
   // DSSD
   // Energy
   fQQQ_StripE_DetectorNbr.clear();
   fQQQ_StripE_StripNbr.clear();
   fQQQ_StripE_Energy.clear(); 
   // Time
   fQQQ_StripT_DetectorNbr.clear();
   fQQQ_StripT_StripNbr.clear();
   fQQQ_StripT_Time.clear();
}


////////////////////////////////////////////////////////////////////////////////
void TQQQData::Dump() const{
   cout << "XXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // QQQ
   // Energy
   cout << "QQQ_StripE_Mult = " << fQQQ_StripE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fQQQ_StripE_DetectorNbr.size(); i++)
      cout << "DetNbr: "  << fQQQ_StripE_DetectorNbr[i] 
           << " Strip: "  << fQQQ_StripE_StripNbr[i] 
           << " Energy: " << fQQQ_StripE_Energy[i] << endl;

   // Time
   cout << "QQQ_StripXT_Mult = " << fQQQ_StripT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fQQQ_StripT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fQQQ_StripT_DetectorNbr[i] 
           << " Strip: " << fQQQ_StripT_StripNbr[i] 
           << " Time: "  << fQQQ_StripT_Time[i] << endl;
  
}
