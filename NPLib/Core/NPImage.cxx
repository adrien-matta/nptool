/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *   This class is wrapper of root TASImage to manipulate and generate       *
 *   detector pixel map                                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//NPTool
#include "NPImage.h"

//STL
#include<iostream>

////////////////////////////////////////////////////////////////////////////////
NPL::Image::Image(){
  m_Image = NULL;
  m_ARGB = NULL;
}
////////////////////////////////////////////////////////////////////////////////
NPL::Image::Image(std::string filename,double Xscalling,double Yscalling){
  m_Image = new TASImage(filename.c_str());
  m_ARGB = m_Image->GetArgbArray();
  m_Xscaling = Xscalling;
  m_Yscaling = Yscalling;
  m_Width = m_Image->GetWidth();
  m_Height = m_Image->GetHeight();
}
////////////////////////////////////////////////////////////////////////////////
NPL::Image::~Image(){
  if(m_Image)
    delete m_Image;
  if(m_ARGB)
    delete m_ARGB;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::Image::Open(std::string filename){
  if(m_Image)
    delete m_Image;
  if(m_ARGB)
    delete m_ARGB;

  m_Image = new TASImage(filename.c_str());
  m_ARGB = m_Image->GetArgbArray();
  m_Width = m_Image->GetWidth();
  m_Height = m_Image->GetHeight();

}

////////////////////////////////////////////////////////////////////////////////
void NPL::Image::Print(double scale){

  m_Image->Scale(scale*m_Width,scale*m_Height);
  m_Width = m_Image->GetWidth();
  m_Height = m_Image->GetHeight();
  for(unsigned int i = 0 ; i < m_Width ; i++){
    for(unsigned int j = 0 ; j < m_Height ; j++){
      unsigned int index = i*m_Width+j;
      unsigned int b = GetBlueAtPixel(i,j);
      unsigned int g = GetGreenAtPixel(i,j);
      unsigned int r = GetRedAtPixel(i,j);
      unsigned int a = GetAlphaAtPixel(i,j);

      if(r||b||g)
        std::cout << "x"<< " " ;
      else
        std::cout << "."<<" " ; 
    }
    std::cout << std::endl;
  }
} 
////////////////////////////////////////////////////////////////////////////////
void NPL::Image::Save(std::string filename){
  m_Image->WriteImage(filename.c_str());
} 


////////////////////////////////////////////////////////////////////////////////
void NPL::Image::Draw(){
  m_Image->Draw();
} 

////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetRedAtCoordinate(double x, double y){
  unsigned int xp = x/m_Xscaling+m_Width/2 ;
  unsigned int yp = y/m_Yscaling+m_Height/2 ;
  return GetRedAtPixel(xp,yp);
}

////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetGreenAtCoordinate(double x, double y){
  unsigned int xp = x/m_Xscaling+m_Width/2 ;
  unsigned int yp = y/m_Yscaling+m_Height/2 ;
  return GetRedAtPixel(xp,yp);
}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetBlueAtCoordinate(double x, double y){
  unsigned int xp = x/m_Xscaling+m_Width/2 ;
  unsigned int yp = y/m_Yscaling+m_Height/2 ;
  return GetRedAtPixel(xp,yp);
}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetAlphaAtCoordinate(double x, double y){
  unsigned int xp = x/m_Xscaling+m_Width/2 ;
  unsigned int yp = y/m_Yscaling+m_Height/2 ;
  return GetRedAtPixel(xp,yp);
}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetPixelAtCoordinate(double x, double y){
  unsigned int xp = x/m_Xscaling+m_Width/2 ;
  unsigned int yp = y/m_Yscaling+m_Height/2 ;
  return m_ARGB[PixelToIndex(xp,yp)];
}


////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::PixelToIndex(unsigned int x, unsigned int y){
  unsigned int index = x*m_Width+y;
  if(index>m_Width*m_Height)// possible because of rounding
    index = m_Width*m_Height-1;
  return index;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetRedAtPixel(unsigned int x, unsigned int y){
  unsigned int v = m_ARGB[PixelToIndex(x,y)];  
  v= (v>>16)&0xff;
  return v;
}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetGreenAtPixel(unsigned int x, unsigned int y){
  unsigned int v = m_ARGB[PixelToIndex(x,y)];  
  v= (v>>8)&0xff;
  return v;

}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetBlueAtPixel(unsigned int x, unsigned int y){
  unsigned int v = m_ARGB[PixelToIndex(x,y)];  
  v= v&0xff;
  return v;

}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::Image::GetAlphaAtPixel(unsigned int x, unsigned int y){
  unsigned int v = m_ARGB[PixelToIndex(x,y)];  
  v= (v>>24)&0xff;
  return v;

}

