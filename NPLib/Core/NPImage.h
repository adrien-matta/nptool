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


// ROOT
#include"TASImage.h"

//STL
#include<string>

namespace NPL{

  class Image{

  public:
    Image();
    Image(std::string filename,double Xscalling,double Yscalling);
    ~Image();
  
  public:
    void Open(std::string filename);
    void Print(double scale=0); // print mockup in terminal
    void Draw(); // draw in a new canvas (interactive only)
    void Save(std::string filename); // save the image

  public: // Get the value based on coordinate
    unsigned int GetRedAtCoordinate(double x, double y);
    unsigned int GetGreenAtCoordinate(double x, double y);
    unsigned int GetBlueAtCoordinate(double x, double y);
    unsigned int GetAlphaAtCoordinate(double x, double y);
    unsigned int GetPixelAtCoordinate(double x, double y);


  public: // Get the value based on pixel
    unsigned int PixelToIndex(unsigned int x, unsigned int y);
    unsigned int GetRedAtPixel(unsigned int x, unsigned int y);
    unsigned int GetGreenAtPixel(unsigned int x, unsigned int y);
    unsigned int GetBlueAtPixel(unsigned int x, unsigned int y);
    unsigned int GetAlphaAtPixel(unsigned int x, unsigned int y);

  public:
    void GenerateStrip(unsigned int Nbr, double x, double y , double interstrip , unsigned int pixel);

  private:
    TASImage* m_Image; // the image itself
    unsigned int* m_ARGB; // the alpha red green blue array of pixel value
    double m_Xscaling; // pixel per mm
    double m_Yscaling; // pixel per mm
    unsigned int m_Width;
    unsigned int m_Height;
  };

}
