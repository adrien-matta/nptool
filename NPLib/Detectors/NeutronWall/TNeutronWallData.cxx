/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TNeutronWallData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

ClassImp(TNeutronWallData)


//////////////////////////////////////////////////////////////////////
TNeutronWallData::TNeutronWallData() {
}



//////////////////////////////////////////////////////////////////////
TNeutronWallData::~TNeutronWallData() {
}



//////////////////////////////////////////////////////////////////////
void TNeutronWallData::Clear() {
    // NeutronWall Energy
    fNeutronWall_E_DetectorNbr.clear();
    fNeutronWall_E_PadNbr.clear();
    fNeutronWall_Energy.clear();
    // NeutronWall Time
    fNeutronWall_T_DetectorNbr.clear();
    fNeutronWall_T_PadNbr.clear();
    fNeutronWall_Time.clear();
    // NeutronWall Position
    fNeutronWall_Position_X.clear();
    fNeutronWall_Position_Y.clear();
    fNeutronWall_Position_Z.clear();
    
    // VetoWall Energy
    fVetoWall_E_DetectorNbr.clear();
    fVetoWall_E_PadNbr.clear();
    fVetoWall_Energy.clear();
    // VetoWall Time
    fVetoWall_T_DetectorNbr.clear();
    fVetoWall_T_PadNbr.clear();
    fVetoWall_Time.clear();
    // VetoWall Position
    fVetoWall_Position_X.clear();
    fVetoWall_Position_Y.clear();
    fVetoWall_Position_Z.clear();
    
}



//////////////////////////////////////////////////////////////////////
void TNeutronWallData::Dump() const {
    // This method is very useful for debuging and worth the dev.
    cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TNeutronWallData::Dump()] XXXXXXXXXXXXXXXXX" << endl;
    
    // Energy
    size_t mysize = fNeutronWall_E_DetectorNbr.size();
    cout << "NeutronWall_E_Mult: " << mysize << endl;
    
    for (size_t i = 0 ; i < mysize ; i++){
        cout << "DetNbr: " << fNeutronWall_E_DetectorNbr[i]
        << " Energy: " << fNeutronWall_Energy[i];
    }
    
    // Time
    mysize = fNeutronWall_T_DetectorNbr.size();
    cout << "NeutronWall_T_Mult: " << mysize << endl;
    
    for (size_t i = 0 ; i < mysize ; i++){
        cout << "DetNbr: " << fNeutronWall_T_DetectorNbr[i]
        << " Time: " << fNeutronWall_Time[i];
    }
}
