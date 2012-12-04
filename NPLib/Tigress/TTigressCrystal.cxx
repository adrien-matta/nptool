/*****************************************************************************
 * Copyright (C) 2009-2012   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                  Peter Bender  contact address: bender@triumf.ca          *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Tigress treated data                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TTigressCrystal.h"


TTigressCrystal::TTigressCrystal(int CrystalNbr, TVector3 CrystalPosition,TVector3 CrystalNormal)  {
  SetCrystalPosition(CrystalPosition);
  SetCrystalNormal(CrystalNormal);
  SetCrystalId(CrystalNbr);
  Clear();
}

void TTigressCrystal::Clear() {
  m_Core_Energy=-1000;
  m_Segment_Energy.clear();
  m_Segment_Time.clear();
  m_Segment_Number.clear();
}

void TTigressCrystal::SetCrystalId(int x)  { 
  m_CrystalId = x;  
  switch(x) {
    case 1:
      m_CrystalName = "Blue";
      break;
    case 2:
      m_CrystalName = "Green";
      break;
    case 3:
      m_CrystalName = "Red";
      break;
    case 4:
      m_CrystalName = "White";
      break;
    default:  
      m_CrystalName = "Bacon";
      break;  
    };
 }







