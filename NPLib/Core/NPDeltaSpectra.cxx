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
#include <iostream>
#include "NPDeltaSpectra.h"
#include "TH1.h"

ClassImp(NPL::DeltaSpectra)

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
////////////////////////////////////////////////////////////////////////////////
void NPL::DeltaSpectra::UpdateLocalSpectra(TList* local){
  std::map<std::string , std::pair<std::vector<double> ,std::vector<double> > >::iterator it;
  for(it = m_Delta.begin() ; it != m_Delta.end() ; ++it){

    TH1* h = (TH1*)local->FindObject(it->first.c_str());  

    if(h){
     unsigned int sizex = it->second.first.size();
     unsigned int sizey = it->second.second.size();

     if(sizey==0) 
       for(unsigned int i = 0 ; i < sizex ; i++)
         h->Fill(it->second.first[i]);
         
     else
       for(unsigned int i = 0 ; i < sizex ; i++)
         h->Fill(it->second.first[i],it->second.second[i]);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DeltaSpectra::Print(){
  std::cout << "///// Delta Specta :" << std::endl;
  std::cout << "Size : " << m_Delta.size() << std::endl;
/*  std::map<std::string , std::pair<std::vector<double> ,std::vector<double> > >::iterator it;
  for(it = m_Delta.begin() ; it != m_Delta.end() ; it++){
    std::cout << "Name: " << it->first.c_str() <<std::endl;
    std::cout << " Val:  " << std::endl;

     unsigned int sizex = it->second.first.size();
     unsigned int sizey = it->second.second.size();

     if(sizey==0) 
       for(unsigned int i = 0 ; i < sizex ; i++)
        std::cout << it->second.second[i] << std::endl; 
     else
       for(unsigned int i = 0 ; i < sizex ; i++)
         std::cout << it->second.first[i]<< " " << it->second.second[i] << std::endl;;
    }
*/
}
