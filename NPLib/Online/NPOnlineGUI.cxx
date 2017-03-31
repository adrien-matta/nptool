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
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// NPL
#include "NPOnlineGUI.h"
#include "NPOptionManager.h"
#include "NPInputParser.h"
#include "NPCore.h"
// STL
#include <iostream>
#include <dirent.h>

// Root
#include "TROOT.h"
#include "TColor.h"
#include "TSystem.h"
#include "TString.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TMessage.h"
#include "TGSplitter.h"
#include "TG3DLine.h"
#include "TGDoubleSlider.h"
#include "TGTextEdit.h"
#include "TASImage.h"
#include "TH2.h"
ClassImp(NPL::OnlineGUI);
////////////////////////////////////////////////////////////////////////////////
void NPL::ExecuteMacro(string name){
  static DIR *dir;
  static struct dirent *ent;
  static string path; 
  path = "./online_macros/";
  name += ".C";
  if ((dir = opendir (path.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      if(ent->d_name==name)
        gROOT->ProcessLine(Form(".x online_macros/%s",name.c_str()));
    }
    closedir (dir);
  }
}
////////////////////////////////////////////////////////////////////////////////
NPL::OnlineGUI::OnlineGUI(NPL::SpectraClient* client){
  m_Client = client;
  m_Sock = 0;
  TString NPLPath = gSystem->Getenv("NPTOOL");
  gROOT->ProcessLine(Form(".x %s/NPLib/scripts/NPToolLogon.C+", NPLPath.Data()));
  gROOT->SetStyle("nponline");

  // Build the interface
  MakeGui();

  // Link the button slot to the function
  m_Quit->SetCommand("gApplication->Terminate()");
  m_Connect->Connect("Clicked()", "NPL::OnlineGUI", this, "Connect()");
  m_Update->Connect("Clicked()", "NPL::OnlineGUI", this, "Update()");
  m_Clock->Connect("Clicked()","NPL::OnlineGUI",this,"AutoUpdate()");
  m_Fit->Connect("Clicked()", "NPL::OnlineGUI", this, "Fit()");

  Connect();
}
////////////////////////////////////////////////////////////////////////////////
void NPL::OnlineGUI::Fit(){


  TCanvas* c = m_EmbeddedCanvas->GetCanvas();

  if(m_CheckFitAll->IsOn()){
    int size= ((TList*)c->GetListOfPrimitives())->GetSize();
    for(unsigned int i =  1 ; i < size ;i++){
      c->cd(i);
      TList* list = gPad->GetListOfPrimitives();
      int Hsize = list->GetSize();
      for(int h = 0 ; h < Hsize ; h++){
        TObject* obj = list->At(h);
        if(obj->InheritsFrom(TH1::Class()) && !obj->InheritsFrom(TH2::Class())){
          TF1* fit1 = new TF1("Gaussian","gaus");
          TF1* fit2 = new TF1("Gaussian+Background","gaus(0)+pol1(3)");
          fit2->SetParName(0,"Constant");
          fit2->SetParName(1,"Mean");
          fit2->SetParName(2,"Sigma");
          fit2->SetParName(3,"Offset");
          fit2->SetParName(4,"Slope");

          if(m_BackgroundFit->IsOn()){
            fit2->SetParameter(0,((TH1*)obj)->GetMaximum());
            fit2->SetParameter(1,((TH1*)obj)->GetMaximumBin()->GetBinCenter());
            fit2->SetParameter(2,5);
            fit2->SetParameter(3,10);fit2->SetParameters(4,0);
            ((TH1*) obj)->Fit(fit2,"Q");

          }
          else
            ((TH1*) obj)->Fit(fit1,"Q"); 

        }
      }
    }
  }

  else{
    TList* list = gPad->GetListOfPrimitives();
    int Hsize = list->GetSize();
    for(int h = 0 ; h < Hsize ; h++){
      TObject* obj = list->At(h);
      if(obj->InheritsFrom(TH1::Class()) && !obj->InheritsFrom(TH2::Class())){
        TF1* fit1 = new TF1("Gaussian","gaus");
        TF1* fit2 = new TF1("Gaussian+Background","gaus(0)+pol1(3)");
        fit2->SetParName(0,"Constant");
        fit2->SetParName(1,"Mean");
        fit2->SetParName(2,"Sigma");
        fit2->SetParName(3,"Offset");
        fit2->SetParName(4,"Slope");

        if(m_BackgroundFit->IsOn()){
          ((TH1*) obj)->Fit(fit1,"Q");
          fit2->SetParameters(fit1->GetParameters());
          ((TH1*) obj)->Fit(fit2,"Q");

        }
        else
          ((TH1*) obj)->Fit(fit1,"Q"); 

      }
    }
  }


  c ->Update();

}
////////////////////////////////////////////////////////////////////////////////
void NPL::OnlineGUI::MakeGui(){
  m_BgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_FgColor = gROOT->GetColor(kAzure+7)->GetPixel();
  m_TabBgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_TabFgColor = gROOT->GetColor(kAzure+7)->GetPixel();
  m_Timer = 0;

  // main frame
  m_Main = new TGMainFrame(gClient->GetRoot(),1500,500,kMainFrame | kVerticalFrame);
  m_Main->SetName("nponline");
  m_Main->SetBackgroundColor(m_BgColor);
  m_Main->SetForegroundColor(m_FgColor);
  m_Main->SetWindowName("nponline");
  // Button bar to hold the button
  m_ButtonBar= new TGVerticalFrame(m_Main,10000,42,kFixedSize);
  m_ButtonBar->SetBackgroundColor(m_BgColor);
  m_ButtonBar->SetForegroundColor(m_BgColor);
  m_ButtonBar->SetLayoutBroken(kTRUE);
  m_Main->AddFrame(m_ButtonBar,new TGLayoutHints(kLHintsLeft|kLHintsTop));

  string NPLPath = gSystem->Getenv("NPTOOL");  
  string path_quit = NPLPath+"/NPLib/Core/icons/power.xpm";
  m_Quit = new TGPictureButton(m_ButtonBar,gClient->GetPicture(path_quit.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Quit->SetBackgroundColor(m_BgColor);
  m_Quit->SetToolTipText("Quit");

  m_ButtonBar->AddFrame(m_Quit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Quit->MoveResize(10,5,32,32);

  string path_connect = NPLPath+"/NPLib/Core/icons/plugin.xpm";
  m_Connect = new TGPictureButton(m_ButtonBar,gClient->GetPicture(path_connect.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  string path_connected = NPLPath+"/NPLib/Core/icons/brightness.xpm"; 
  m_Connect->SetDisabledPicture(gClient->GetPicture(path_connected.c_str()));
  m_Connect->SetBackgroundColor(m_BgColor);
  m_Connect->SetBackgroundColor(m_BgColor);

  m_Connect->SetToolTipText("Connect to server");
  m_ButtonBar->AddFrame(m_Connect, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Connect->MoveResize(52,5,32,32);

  string path_update = NPLPath+"/NPLib/Core/icons/download.xpm";
  m_Update = new TGPictureButton(m_ButtonBar,gClient->GetPicture(path_update.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Update->SetBackgroundColor(m_BgColor);
  m_Update->SetForegroundColor(m_BgColor);
  m_Update->SetToolTipText("Update spectra");
  m_ButtonBar->AddFrame(m_Update, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Update->MoveResize(400,5,32,32);

  string path_clock= NPLPath+"/NPLib/Core/icons/clock.xpm";
  m_Clock = new TGPictureButton(m_ButtonBar,gClient->GetPicture(path_clock.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Clock->SetBackgroundColor(m_BgColor);
  m_Clock->SetForegroundColor(m_BgColor);

  m_Clock->SetToolTipText("AutoUpdate");
  m_ButtonBar->AddFrame(m_Clock, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Clock->MoveResize(442,5,32,32);

  TGFont* ufont;// will reflect user font changes
  ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

  TGGC  * uGC;// will reflect user GC changes
  // graphics context changes
  GCValues_t valress;
  valress.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
  gClient->GetColorByName("#000000",valress.fForeground);
  gClient->GetColorByName("#e7e7e7",valress.fBackground);
  valress.fFillStyle = kFillSolid;
  valress.fFont = ufont->GetFontHandle();
  valress.fGraphicsExposures = kFALSE;
  uGC = gClient->GetGC(&valress, kTRUE);
  m_Address = new TGTextEntry(m_ButtonBar, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kChildFrame | kOwnBackground);
  m_Address->SetMaxLength(4096);
  m_Address->SetAlignment(kTextLeft);
  m_Address->SetText(m_Client->GetAddress().c_str());
  m_Address->Resize(200,m_Address->GetDefaultHeight());
  m_ButtonBar->AddFrame(m_Address, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Address->MoveResize(90,10,200,20);

  m_Port = new TGNumberEntry(m_ButtonBar, (Double_t) m_Client->GetPort(),9,-1,(TGNumberFormat::EStyle) 5);
  m_Port->SetName("m_Port");
  m_Port->GetButtonUp()->SetStyle(1);
  m_Port->GetButtonDown()->SetStyle(1);
  m_ButtonBar->AddFrame(m_Port, new TGLayoutHints(kLHintsLeft));
  m_Port->MoveResize(300,10,80,20);

  m_TimerEntry = new TGNumberEntry(m_ButtonBar, (Double_t) 1,9,-1,(TGNumberFormat::EStyle) 5);
  m_TimerEntry->SetName("m_TimerEntry");
  m_TimerEntry->GetButtonUp()->SetStyle(1);
  m_TimerEntry->GetButtonDown()->SetStyle(1);
  m_ButtonBar->AddFrame(m_TimerEntry, new TGLayoutHints(kLHintsLeft));
  m_TimerEntry->MoveResize(484,10,40,20);

  // Create the splitted frame
  m_Split = new TGHorizontalFrame(m_Main, 50, 50);
  TGVerticalFrame* fV1 = new TGVerticalFrame(m_Split, 10, 10, kFixedWidth);
  TGVerticalFrame* fV2 = new TGVerticalFrame(m_Split, 10, 10);
  TGVerticalFrame* fV3 = new TGVerticalFrame(m_Split, 10, 10, kFixedWidth);

  m_Left   = new TGCompositeFrame(fV1, 10, 10, kChildFrame);
  m_Center = new TGCompositeFrame(fV2, 10, 10, kChildFrame);
  m_Right  = new TGCompositeFrame(fV3, 10, 10, kChildFrame); 
  fV1->AddFrame(m_Left,  new TGLayoutHints(kLHintsLeft |    kLHintsExpandX | kLHintsExpandY,0, 0, 5, 10));
  fV2->AddFrame(m_Center,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0, 0, 5, 10));
  fV3->AddFrame(m_Right, new TGLayoutHints(kLHintsRight |   kLHintsExpandX | kLHintsExpandY,0, 0, 5, 10));

  fV1->Resize(200, 400);
  fV2->Resize(400, 400);
  fV3->Resize(200, 400);


  // Change the size of the list tree
  m_Split->AddFrame(fV1, new TGLayoutHints(kLHintsLeft | kLHintsExpandY ));

  TGVSplitter* splitter1 = new TGVSplitter(m_Split,5,5);
  splitter1->SetFrame(fV1, kTRUE);
  m_Split->AddFrame(splitter1, new TGLayoutHints(kLHintsLeft| kLHintsTop |  kLHintsExpandY));
  m_Split->AddFrame(fV2,  new TGLayoutHints(kLHintsExpandX| kLHintsTop |  kLHintsExpandY));

  // Change the size of the tool bar
  TGVSplitter* splitter2 = new TGVSplitter(m_Split,5,5);
  splitter2->SetFrame(fV3, false);
  m_Split->AddFrame(splitter2, new TGLayoutHints(kLHintsLeft| kLHintsTop |  kLHintsExpandY));
  m_Split->AddFrame(fV3, new TGLayoutHints(kLHintsRight| kLHintsTop |  kLHintsExpandY));

  splitter1->SetBackgroundColor(m_BgColor);    
  splitter1->SetForegroundColor(m_BgColor);    

  splitter2->SetBackgroundColor(m_BgColor);    
  splitter2->SetForegroundColor(m_BgColor);    


  m_Split->SetBackgroundColor(m_BgColor); 
  m_Split->SetForegroundColor(m_BgColor); 

  m_Left->SetBackgroundColor(m_BgColor); 
  m_Right->SetBackgroundColor(m_FgColor);    
  m_Left->SetForegroundColor(m_BgColor); 
  m_Right->SetForegroundColor(m_BgColor);    
  fV1->SetBackgroundColor(m_BgColor); 
  fV2->SetBackgroundColor(m_BgColor);    
  fV1 ->SetForegroundColor(m_BgColor); 
  fV2->SetForegroundColor(m_BgColor);    
  fV3->SetBackgroundColor(m_BgColor);
  m_Main->AddFrame(m_Split, new TGLayoutHints(kLHintsRight | kLHintsExpandX |kLHintsExpandY));

  // Right
  // Create a fit tool bar
  //m_Right->SetLayoutBroken(kTRUE);

  // Navigation
  TGVerticalFrame* m_NavBar= new TGVerticalFrame(m_Right,10000,80);
  m_NavBar->SetBackgroundColor(m_FgColor);
  m_NavBar->SetForegroundColor(m_FgColor);
  m_Right->AddFrame(m_NavBar, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

  TGVerticalFrame* m_LogBar= new TGVerticalFrame(m_NavBar,10000,80);
  m_LogBar->SetBackgroundColor(m_FgColor);
  m_LogBar->SetForegroundColor(m_FgColor);
  m_LogBar->SetLayoutManager(new TGHorizontalLayout(m_LogBar));
  m_NavBar->AddFrame(m_LogBar, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

  TGCheckButton* m_CheckLogX = new TGCheckButton(m_LogBar,"LogX");
  m_CheckLogX->SetBackgroundColor(m_FgColor);
  m_CheckLogX->SetForegroundColor(m_BgColor);        
  m_LogBar->AddFrame(m_CheckLogX, new TGLayoutHints(kLHintsCenterX,2,2,2,2)); 
  m_CheckLogX->Move(0,0); 

  TGCheckButton* m_CheckLogY = new TGCheckButton(m_LogBar,"LogY");
  m_CheckLogY->SetBackgroundColor(m_FgColor);
  m_CheckLogY->SetForegroundColor(m_BgColor);        
  m_LogBar->AddFrame(m_CheckLogY, new TGLayoutHints(kLHintsCenterX,2,2,2,2)); 
  m_CheckLogY->Move(60,0); 

  TGCheckButton* m_CheckLogZ = new TGCheckButton(m_LogBar,"LogZ");
  m_CheckLogZ->SetBackgroundColor(m_FgColor);
  m_CheckLogZ->SetForegroundColor(m_BgColor);        
  m_LogBar->AddFrame(m_CheckLogZ, new TGLayoutHints(kLHintsCenterX,2,2,2,2)); 
  m_CheckLogZ->Move(120 ,0); 

  TGDoubleHSlider* m_ZoomAllH = new TGDoubleHSlider(m_NavBar,m_NavBar->GetWidth());
  m_NavBar->AddFrame(m_ZoomAllH, new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,2,2,2,2)); 
  m_ZoomAllH->SetBackgroundColor(m_FgColor); 
  m_ZoomAllH->SetRange(0,100); 
  m_ZoomAllH->SetPosition(0,100);

  TGDoubleHSlider* m_ZoomAllV = new TGDoubleHSlider(m_NavBar,m_NavBar->GetWidth());
  m_NavBar->AddFrame(m_ZoomAllV, new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,2,2,2,2)); 
  m_ZoomAllV->SetBackgroundColor(m_FgColor); 
  m_ZoomAllV->SetRange(0,100); 
  m_ZoomAllV->SetPosition(0,100);

  // Horizontal separator
  TGHorizontal3DLine* m_NavLine = new TGHorizontal3DLine(m_Right,408,8);
  m_NavLine->SetBackgroundColor(m_FgColor);
  m_NavLine->SetForegroundColor(m_BgColor);
  m_Right->AddFrame(m_NavLine, new TGLayoutHints(kLHintsExpandX,2,2,2,2));  


  // Fit Bar
  TGVerticalFrame* m_FitBar = new TGVerticalFrame(m_Right,10000,80);
  m_FitBar->SetBackgroundColor(m_FgColor);
  m_FitBar->SetForegroundColor(m_FgColor);
  m_FitBar->SetLayoutManager(new TGHorizontalLayout(m_FitBar));  
  m_Right->AddFrame(m_FitBar, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

  // Fit button
  string path_fit = "/scratch/nptool/NPLib/Core/icons/barchart.xpm";
  m_Fit= new TGPictureButton(m_FitBar,gClient->GetPicture(path_fit.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Fit->SetBackgroundColor(m_FgColor);
  m_Fit->SetToolTipText("Fit");
  m_FitBar->AddFrame(m_Fit, new TGLayoutHints(kLHintsTop|kLHintsLeft,10,10,10,10));

  // Check box container
  TGVerticalFrame* m_SelectBar = new TGVerticalFrame(m_FitBar,10000,80);
  m_SelectBar->SetBackgroundColor(m_FgColor);
  m_SelectBar->SetForegroundColor(m_FgColor);
  m_SelectBar->SetLayoutManager(new TGVerticalLayout(m_SelectBar));
  m_FitBar->AddFrame(m_SelectBar, new TGLayoutHints(kLHintsLeft|kLHintsExpandX));

  // Fit check box
  m_CheckFitAll = new TGCheckButton(m_SelectBar,"all");
  m_CheckFitAll->SetBackgroundColor(m_FgColor);
  m_CheckFitAll->SetForegroundColor(m_BgColor);        
  m_SelectBar->AddFrame(m_CheckFitAll, new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2)); 

  m_BackgroundFit = new TGCheckButton(m_SelectBar,"background");
  m_BackgroundFit->SetBackgroundColor(m_FgColor);
  m_BackgroundFit->SetForegroundColor(m_BgColor);        
  m_SelectBar->AddFrame(m_BackgroundFit, new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2)); 

  // Fit Slider
  TGDoubleHSlider* m_FitSlider = new TGDoubleHSlider(m_Right,m_NavBar->GetWidth());
  m_Right->AddFrame(m_FitSlider, new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,2,2,2,2)); 
  m_FitSlider->SetBackgroundColor(m_FgColor); 
  m_FitSlider->SetRange(0,100); 
  m_FitSlider->SetPosition(0,100);


  // Horizontal separator
  TGHorizontal3DLine* m_FitLine = new TGHorizontal3DLine(m_Right,408,8);
  m_FitLine->SetBackgroundColor(m_FgColor);
  m_FitLine->SetForegroundColor(m_BgColor);
  m_Right->AddFrame(m_FitLine, new TGLayoutHints(kLHintsExpandX,2,2,2,2));  

  // Print Bar
  TGVerticalFrame* m_PrintBar = new TGVerticalFrame(m_Right,10000,80);
  m_PrintBar->SetBackgroundColor(m_FgColor);
  m_PrintBar->SetForegroundColor(m_FgColor);
  m_PrintBar->SetLayoutManager(new TGHorizontalLayout(m_PrintBar));  
  m_Right->AddFrame(m_PrintBar, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

  // Print button
  string path_print= "/scratch/nptool/NPLib/Core/icons/print.xpm";
  TGPictureButton* m_Print= new TGPictureButton(m_PrintBar,gClient->GetPicture(path_print.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Print->SetBackgroundColor(m_FgColor);
  m_Print->SetToolTipText("SaveAs");
  m_PrintBar->AddFrame(m_Print, new TGLayoutHints(kLHintsTop|kLHintsLeft,10,10,10,10));

  // Check box container
  TGVerticalFrame* m_OptionBar = new TGVerticalFrame(m_PrintBar,10000,80);
  m_OptionBar->SetBackgroundColor(m_FgColor);
  m_OptionBar->SetForegroundColor(m_FgColor);
  m_OptionBar->SetLayoutManager(new TGVerticalLayout(m_OptionBar));
  m_PrintBar->AddFrame(m_OptionBar, new TGLayoutHints(kLHintsLeft|kLHintsExpandX));

  // Fit check box
  TGCheckButton* m_PrintColor = new TGCheckButton(m_OptionBar,"printer color");
  m_PrintColor->SetBackgroundColor(m_FgColor);
  m_PrintColor->SetForegroundColor(m_BgColor);        
  m_OptionBar->AddFrame(m_PrintColor, new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2)); 

  // Fit check box
  TGCheckButton* m_PrintPDF = new TGCheckButton(m_OptionBar,"pdf");
  m_PrintPDF->SetBackgroundColor(m_FgColor);
  m_PrintPDF->SetForegroundColor(m_BgColor);        
  m_OptionBar->AddFrame(m_PrintPDF, new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2)); 


  // Horizontal separator
  TGHorizontal3DLine* m_PrintLine = new TGHorizontal3DLine(m_Right,408,8);
  m_PrintLine->SetBackgroundColor(m_FgColor);
  m_PrintLine->SetForegroundColor(m_BgColor);
  m_Right->AddFrame(m_PrintLine, new TGLayoutHints(kLHintsExpandX,2,2,2,2));  
  // m_PrintLine->Move(0,offset+50);


  // Elog button
  string path_elog= "/scratch/nptool/NPLib/Core/icons/booklet.xpm";
  TGPictureButton* m_Elog= new TGPictureButton(m_Right,gClient->GetPicture(path_elog.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Elog->SetBackgroundColor(m_FgColor);
  m_Elog->SetToolTipText("Elog");
  m_Right->AddFrame(m_Elog, new TGLayoutHints(kLHintsTop|kLHintsLeft,10,10,10,10));
  // m_Elog->Move(10,offset); 

  // Elog entry
  TGTextEdit* m_ElogEntry = new TGTextEdit(m_Right,m_Right->GetWidth(),100);
  m_Right->AddFrame(m_ElogEntry, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY |kLHintsCenterY, 5, 5, 5, 5));
  //m_ElogEntry->Move(0,offset+40); 


  // Center //
  m_EmbeddedCanvas = new TRootEmbeddedCanvas("Display",m_Center,700,490,!kSunkenFrame);  
  m_EmbeddedCanvas->SetAutoFit(true);
  m_Center->AddFrame(m_EmbeddedCanvas,new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandX | kLHintsExpandY));
  TCanvas* c = new TCanvas("logo",0,0,0);
  TASImage* logo = new TASImage("/scratch/nptool/NPLib/Core/icons/nptoolLogo.png");
  logo->Draw("");
  m_EmbeddedCanvas->AdoptCanvas(c);
  // Left //
  // Create tabs for histo list and canvas list
  m_Tab = new TGTab(m_Left,700,500);
  m_Tab->Resize(m_Tab->GetDefaultSize());
  m_Tab->SetBackgroundColor(m_TabBgColor);
  m_Tab->SetForegroundColor(m_TabFgColor);
  m_Tab->ChangeSubframesBackground(m_BgColor);
  TGCompositeFrame* tfCanvas= m_Tab->AddTab("Canvas");
  TGCompositeFrame* tfHisto = m_Tab->AddTab("Histo");

  m_Left->AddFrame(m_Tab, new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandY | kLHintsExpandX));

  // canvas widget
  TGCanvas* m_ListCanvas = new TGCanvas(tfCanvas,120,500);
  m_ListCanvas->SetName("m_ListCanvas");

  m_ListCanvas->SetForegroundColor(m_BgColor); 
  m_ListCanvas->SetForegroundColor(m_BgColor);    

  // canvas viewport
  TGViewPort* CanvasViewPort = m_ListCanvas->GetViewPort();

  // list tree
  m_CanvasListTree = new CanvasList(m_Main,m_ListCanvas,m_EmbeddedCanvas);
  m_ListTree = m_CanvasListTree->GetListTree();

  CanvasViewPort->AddFrame(m_ListTree,new TGLayoutHints(kLHintsRight | kLHintsBottom | kLHintsExpandY | kLHintsExpandX));
  m_ListTree->SetLayoutManager(new TGHorizontalLayout(m_ListTree));
  m_ListTree->MapSubwindows();

  m_ListCanvas->SetContainer(m_ListTree);
  m_ListCanvas->MapSubwindows();
  tfCanvas->AddFrame(m_ListCanvas, new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandY | kLHintsExpandX));
  m_ListCanvas->MoveResize(10,50,120,500);

  m_Main->SetMWMHints(kMWMDecorAll,kMWMFuncAll,kMWMInputModeless);
  m_Main->MapSubwindows();

  m_Main->Resize(m_Main->GetDefaultSize());
  m_Main->MapWindow();
  m_Main->MoveResize(50,50,1250,800);

}

////////////////////////////////////////////////////////////////////////////////
NPL::OnlineGUI::~OnlineGUI(){
  delete m_Main; 
  delete m_ListCanvas;
  delete m_ListTree;
  delete m_Tab;
  delete m_Quit;
  delete m_Connect;
  delete m_Update;
  delete m_Sock;
  delete m_Port;
  delete m_Address;
  delete m_StatusBar;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::OnlineGUI::Connect(){
  m_Client->SetAddressAndPort((string) m_Address->GetDisplayText(),(int) m_Port->GetNumber());
  m_Client->Connect();
}

////////////////////////////////////////////////////////////////////////////////
void NPL::OnlineGUI::Update(){
  if(m_Client->Update()){
    // Do some stuff with the histo
  }

  else
    m_Connect->SetState(kButtonUp);  
}

////////////////////////////////////////////////////////////////////////////////
void NPL::OnlineGUI::AutoUpdate(){

  if(m_Timer){
    delete m_Timer;
    m_Timer = 0 ;
    return;
  }

  else if(m_TimerEntry->GetNumber()>0){
    m_Timer = new TTimer(m_TimerEntry->GetNumber()*1000);
    m_Timer->Connect("Timeout()", "NPL::OnlineGUI", this, "Update()");
    m_Timer->TurnOn();
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* CanvasList Class */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

NPL::CanvasList::CanvasList(TGMainFrame* main, TGCanvas* parent,TRootEmbeddedCanvas* canvas){
  string NPLPath = gSystem->Getenv("NPTOOL");  
  string path_icon = NPLPath+"/NPLib/Core/icons/polaroid.xpm";
  string path_icon_folder = NPLPath+"/NPLib/Core/icons/folder.xpm";

  m_popen = gClient->GetPicture(path_icon.c_str());
  m_pclose = gClient->GetPicture(path_icon.c_str());
  m_pfolder = gClient->GetPicture(path_icon_folder.c_str()); 

  m_BgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_FgColor = gROOT->GetColor(kWhite)->GetPixel();

  m_ListTree = new TGListTree(parent,kHorizontalFrame);
  m_ListTree->Connect("DoubleClicked(TGListTreeItem*,Int_t)","NPL::CanvasList",this,"OnDoubleClick(TGListTreeItem*,Int_t)");
  m_Main = main;
  m_EmbeddedCanvas = canvas;
  LoadCanvasList();
}
////////////////////////////////////////////////////////////////////////////////
NPL::CanvasList::~CanvasList(){
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::OnDoubleClick(TGListTreeItem* item, Int_t btn){
  TCanvas* c = m_Canvas[item->GetText()];

  if(c){
    m_EmbeddedCanvas->AdoptCanvas(c);
    TGDimension size = m_EmbeddedCanvas->GetContainer()->GetSize();
    m_EmbeddedCanvas->GetContainer()->Resize(0,0);
    m_EmbeddedCanvas->GetContainer()->Resize(size);
    c->SetHighLightColor(kAzure+7); // color of active pad
    c->Update();
    ExecuteMacro(c->GetName());
  }
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::AddItem(TCanvas* c,TGListTreeItem* parent){
  TGListTreeItem* item  = m_ListTree->AddItem(parent,c->GetName());
  item->SetPictures(m_popen, m_pclose);
  if(parent)
    parent->SetPictures(m_pfolder,m_pfolder);
  string path = c->GetName();
  m_Canvas[c->GetName()]=c;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::Clear(){
  m_Canvas.clear();
  TGListTreeItem* item =  m_ListTree->GetFirstItem() ;
  while(item){
    m_ListTree->DeleteItem(item);
    item = m_ListTree->GetFirstItem() ;
  }
}
////////////////////////////////////////////////////////////////////////////////
TGListTree* NPL::CanvasList::GetListTree(){
  return m_ListTree;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::SetTab(TGTab* tab){
  m_Tab=tab;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::SetStatusText(const char* txt, int pi){
  for(unsigned int i = 0 ; i < m_StatusBar.size(); i++)
    m_StatusBar[i]->SetText(txt,pi);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::EventInfo(int event,int px,int py,TObject* selected){
  const char *text0, *text1, *text3;
  char text2[50];
  text0 = selected->GetTitle();
  SetStatusText(text0,0);
  text1 = selected->GetName();
  SetStatusText(text1,1);
  if (event == kKeyPress)
    sprintf(text2, "%c", (char) px);
  else
    sprintf(text2, "%d,%d", px, py);
  SetStatusText(text2,2);
  text3 = selected->GetObjectInfo(px,py);
  SetStatusText(text3,3);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::LoadCanvasList(){
  NPL::InputParser parser("CanvasList.txt",false);
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Canvas");
  vector<std::string> token = {"Path","Divide","Histo"};
  NPOptionManager::getInstance()->SetVerboseLevel(0);
  gROOT->ProcessLine("gROOT->SetBatch(kTRUE)");
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      vector<std::string> path = blocks[i]->GetVectorString("Path");
      vector<int> divide = blocks[i]->GetVectorInt("Divide");
      vector<std::string> histo = blocks[i]->GetVectorString("Histo");
      string name = path[path.size()-1];
      TCanvas* c = new TCanvas(name.c_str(), 5000,5000,0);
      c->Divide(divide[0],divide[1]);

      static int pos = 0 ;
      pos++;
      TH1D* hpipo1 = new TH1D(Form("pipo%d",pos),Form("pipo%d",pos),1000,-100,100);
      TF1* f = new TF1("ff","gaus(0)+pol1(3)");
      f->SetParameter(0,100); f->SetParameter(1,-pos*2); f->SetParameter(2,2); f->SetParameter(3,10);
      hpipo1->FillRandom("ff",100000);
      c->cd(1);
      hpipo1->Draw();

      c->cd(2);
      pos++;
      TH1D* hpipo2 = new TH1D(Form("pipo%d",pos),Form("pipo%d",pos),1000,-100,100);
      f->SetParameter(0,100); f->SetParameter(1,pos*2); f->SetParameter(2,2);
      hpipo2->FillRandom("ff",100000);
      hpipo2->Draw();
      TGListTreeItem*  item  =  NULL;
      TGListTreeItem*  pitem =  NULL;

      string item_path="";
      for(unsigned int j = 0 ; j < path.size()-1 ; j++){
        item_path+="/"+path[j];
        item = m_ListTree->FindItemByPathname(item_path.c_str());
        if(!item){
          item= m_ListTree->AddItem(pitem,path[j].c_str());
          if(pitem)
            pitem->SetPictures(m_pfolder,m_pfolder); 
        }
        pitem = item;

      }

      if(item)
        AddItem(c,item);
      else
        AddItem(c);
    }
    else
      NPL::SendWarning("NPL::CanvasList","CanvasList.txt has incorrect formatting");
  }
  gROOT->ProcessLine("gROOT->SetBatch(kFALSE)");

}
