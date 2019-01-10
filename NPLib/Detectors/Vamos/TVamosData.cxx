/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : octobre 2018                                             *
 * Last update    : 09/01/2019                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Vamos Raw data                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *****************************************************************************/
#include "TVamosData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TVamosData)


    //////////////////////////////////////////////////////////////////////
    TVamosData::TVamosData() {
    }



//////////////////////////////////////////////////////////////////////
TVamosData::~TVamosData() {
}



//////////////////////////////////////////////////////////////////////
void TVamosData::Clear() {
    // Energy
    fVamos_E_DetectorNbr.clear();
    fVamos_Energy.clear();
    // Time
    fVamos_T_DetectorNbr.clear();
    fVamos_Time.clear();
    // Pos
    fVamos_Drift_DetectorNbr.clear();
    fVamos_DriftTime.clear();
    fVamos_X.clear();  
}


//////////////////////////////////////////////////////////////////////
void TVamosData::Dump() const {
    // This method is very useful for debuging and worth the dev.
    cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TVamosData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

    // Energy
    size_t mysize = fVamos_E_DetectorNbr.size();
    cout << "Vamos_E_Mult: " << mysize << endl;

    for (size_t i = 0 ; i < mysize ; i++){
        cout << "DetNbr: " << fVamos_E_DetectorNbr[i]
            << " Energy: " << fVamos_Energy[i];
    }

    // Time
    mysize = fVamos_T_DetectorNbr.size();
    cout << "Vamos_T_Mult: " << mysize << endl;

    for (size_t i = 0 ; i < mysize ; i++){
        cout << "DetNbr: " << fVamos_T_DetectorNbr[i]
            << " Time: " << fVamos_Time[i];
    }
}
