#include "TStyle.h"
#include "TColor.h"
void Style_nptool(){
  //-----------------------------------
  // standardised nptool root style
  // produce minimalist publication ready
  // outlook with nptool color scheme
  //-----------------------------------
  TStyle* style_nptool = new TStyle("nptool", "style_nptool");

  //General 
  style_nptool->SetFrameFillColor(kWhite);
  style_nptool->SetFrameBorderMode(0);
  style_nptool->SetFrameLineColor(kBlack);
  style_nptool->SetStatColor(kWhite);	
  style_nptool->SetFillColor(kWhite); 
  style_nptool->SetTitleFillColor(kWhite);

  // Canvas
  style_nptool->SetCanvasColor(kWhite);
  style_nptool->SetCanvasDefH(800);
  style_nptool->SetCanvasDefW(800);
  style_nptool->SetCanvasBorderMode(0);

  // Pad
  style_nptool->SetPadBottomMargin(0.15);
  style_nptool->SetPadLeftMargin(0.15);
  style_nptool->SetPadTopMargin(0.05);
  style_nptool->SetPadRightMargin(0.10);
  style_nptool->SetPadBorderMode(0);
  style_nptool->SetPadBorderSize(1);
  style_nptool->SetPadColor(kWhite);		
  style_nptool->SetPadTickX(1);
  style_nptool->SetPadTickY(1);

  // Desable stat title and fit  
  style_nptool->SetOptStat(0);
  //style_nptool->SetOptTitle(0);
  //style_nptool->SetOptFit(0);

  // x axis
  style_nptool->SetTitleXSize(0.06);
  style_nptool->SetTitleXOffset(1.0);     
  style_nptool->SetLabelOffset(0.1,"X");   
  style_nptool->SetLabelSize(0.04,"X");
  style_nptool->SetLabelOffset(0.006,"X");
    
    /*style_nptool->SetTitleBorderSize(0);
    style_nptool->SetTitleX(0.1f);
    style_nptool->SetTitleW(0.8f);
    style_nptool->SetTitleAlign(3);*/

  // y axis
    //style_nptool->GetYaxis()->CenterTitle();
  style_nptool->SetTitleYSize(0.06);  
  style_nptool->SetTitleYOffset(1.1);
  style_nptool->SetLabelOffset(0.1,"Y");
  style_nptool->SetLabelSize(0.04,"Y");
  style_nptool->SetLabelOffset(0.006,"Y");

  // z axis
  style_nptool->SetLabelSize(0.04,"Z");
  style_nptool->SetLabelOffset(0.006,"Z");


  // Histogramm
  style_nptool->SetHistLineColor(kAzure+7);
  style_nptool->SetHistFillStyle(1001);
  style_nptool->SetHistFillColor(kAzure+7);
  style_nptool->SetMarkerColor(kOrange+7);
  style_nptool->SetMarkerStyle(20);
  style_nptool->SetLineColor(kOrange+7);
  style_nptool->SetLineWidth(2);
  style_nptool->SetFuncColor(kOrange+7);
  style_nptool->SetFuncWidth(2);

  style_nptool->SetTitlePS("nptool");

  const UInt_t Number = 4;
  Double_t Red[Number]    = { 0,0.22,  0.5*0,     0   };
  Double_t Green[Number]  = { 0,0.22,  0.5*0.8,   0.8 };
  Double_t Blue[Number]   = { 0,0.22,  0.5*1.00,  1.00 };

  Double_t Length[Number] = { 0, 0.5,0.5,1.00 };
  Int_t nb=255;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
  style_nptool->SetNumberContours(99);
}
