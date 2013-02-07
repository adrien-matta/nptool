/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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

#include "TTigressClover.h"


TTigressClover::TTigressClover(int CloverId, double R, double Theta, double Phi)  {
  Theta*=M_PI/180.;
  Phi*=M_PI/180.;
  
  m_CloverId = CloverId;
  m_Position = TVector3(0,0,1);
  R = R + 7; //7mm for detector wall thickness 
  m_Position.SetMag(R);
  m_Position.SetTheta(Theta);
  m_Position.SetPhi(Phi);
 
  TVector3 CrystalPosition;
  for(int i = 1 ; i<5 ; i++){
    switch(i){
      case 1:
        CrystalPosition=TVector3(-27.5,27.5,0);
      break;
      case 2:
       CrystalPosition=TVector3(27.5,27.5,0);
      break;
      case 3:
        CrystalPosition=TVector3(27.5,-27.5,0);
      break;
      case 4:
       CrystalPosition=TVector3(-27.5,-27.5,0);
      break;
      default:
        CrystalPosition = TVector3(0,0,0);
    };
    
    CrystalPosition+=TVector3(0,0,m_Position.Mag());
    
    if(m_Position.X()<0) CrystalPosition.RotateZ(M_PI);
    if(m_Position.Y()<0) CrystalPosition.RotateZ(M_PI/2.);
    
    CrystalPosition.RotateY(Theta);
    CrystalPosition.RotateZ(Phi);
    AddCrystals(CrystalPosition,m_Position);
  }

}

void TTigressClover::AddCrystals(TVector3 CrystalPosition,TVector3 CrystalNormal) {
  m_Crystal.push_back(TTigressCrystal(m_Crystal.size()+1,CrystalPosition,CrystalNormal));
}

void TTigressClover::Clear() {
  for(unsigned int i = 0 ; i < m_Crystal.size() ; i++)
    m_Crystal[i].Clear();
}















