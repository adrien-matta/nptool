/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : 11 May 2017                                              *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class hold the record of pas filling of an histo server             *
 *  It is used in Online module to guarantee a faster Histo Sync             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "NPDeltaSpectra.h"
////////////////////////////////////////////////////////////////////////////////
void NPL::DeltaSpectra::Fill(std::string name, double valx){
  m_Delta[name].first.push_back(valx);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DeltaSpectra::Fill(std::string name,double valx,double valy){
 m_Delta[name].first.push_back(valx);
 m_Delta[name].second.push_back(valy);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DeltaSpectra::Clear(){
  m_Delta.clear();
}
