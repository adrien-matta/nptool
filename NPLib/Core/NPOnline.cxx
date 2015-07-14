// NPL
#include "NPOnline.h"

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
ClassImp(NPL::NPOnline);
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
NPL::NPOnline::NPOnline(string address,int port){
  m_Sock = 0;
  TString NPLPath = gSystem->Getenv("NPTOOL");
  gROOT->ProcessLine(Form(".x %s/NPLib/scripts/NPToolLogon.C+", NPLPath.Data()));
  gROOT->SetStyle("nponline");

  // Build the interface
  MakeGui(address,port);

  // Link the button slot to the function
  m_Quit->SetCommand("gApplication->Terminate()");
  m_Connect->Connect("Clicked()", "NPL::NPOnline", this, "Connect()");
  m_Update->Connect("Clicked()", "NPL::NPOnline", this, "Update()");
  m_Clock->Connect("Clicked()","NPL::NPOnline",this,"AutoUpdate()");

  Connect();
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::MakeGui(string address,int port){
  m_BgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_FgColor = gROOT->GetColor(kAzure+7)->GetPixel();
  m_TabBgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_TabFgColor = gROOT->GetColor(kAzure+7)->GetPixel();
  m_Timer = 0;

  // main frame
  m_Main = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
  m_Main->SetName("nponline");
  m_Main->SetBackgroundColor(m_BgColor);
  m_Main->SetForegroundColor(m_FgColor);

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

  TGFont* ufont;         // will reflect user font changes
  ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

  TGGC  * uGC;           // will reflect user GC changes
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
  m_Address->SetText(address.c_str());
  m_Address->Resize(200,m_Address->GetDefaultHeight());
  m_ButtonBar->AddFrame(m_Address, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Address->MoveResize(90,10,200,20);

  m_Port = new TGNumberEntry(m_ButtonBar, (Double_t) port,9,-1,(TGNumberFormat::EStyle) 5);
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
  m_Left = new TGCompositeFrame(fV1, 10, 10, kChildFrame);
  m_Right = new TGCompositeFrame(fV2, 10, 10, kChildFrame);
  fV1->AddFrame(m_Left, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0, 0, 5, 10));
  fV2->AddFrame(m_Right, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0, 0, 5, 10));

  fV1->Resize(m_Left->GetDefaultWidth()+200, fV1->GetDefaultHeight());
  fV2->Resize(m_Right->GetDefaultWidth(), fV1->GetDefaultHeight());
  m_Split->AddFrame(fV1, new TGLayoutHints(kLHintsLeft | kLHintsExpandY ));

  TGVSplitter* splitter = new TGVSplitter(m_Split,5,5);
  splitter->SetFrame(fV1, kTRUE);
  m_Split->AddFrame(splitter, new TGLayoutHints(kLHintsLeft| kLHintsTop |  kLHintsExpandY));
  m_Split->SetBackgroundColor(m_BgColor); 
  splitter->SetBackgroundColor(m_BgColor);    
  m_Split->SetForegroundColor(m_BgColor); 
  splitter->SetForegroundColor(m_BgColor);    
  m_Left->SetBackgroundColor(m_BgColor); 
  m_Right->SetBackgroundColor(m_BgColor);    
  m_Left->SetForegroundColor(m_BgColor); 
  m_Right->SetForegroundColor(m_BgColor);    
  fV1->SetBackgroundColor(m_BgColor); 
  fV2->SetBackgroundColor(m_BgColor);    
  fV1 ->SetForegroundColor(m_BgColor); 
  fV2->SetForegroundColor(m_BgColor);    

  m_Split->AddFrame(fV2, new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY));
  m_Main->AddFrame(m_Split, new TGLayoutHints(kLHintsRight | kLHintsExpandX |kLHintsExpandY));

  // canvas widget
  TGCanvas* m_ListCanvas = new TGCanvas(m_Left,120,500);
  m_ListCanvas->SetName("m_ListCanvas");

  m_ListCanvas ->SetForegroundColor(m_BgColor); 
  m_ListCanvas->SetForegroundColor(m_BgColor);    


  // canvas viewport
  TGViewPort* fViewPort669 = m_ListCanvas->GetViewPort();

  // list tree
  m_CanvasListTree = new CanvasList(m_Main,m_ListCanvas);
  m_ListTree = m_CanvasListTree->GetListTree();

  fViewPort669->AddFrame(m_ListTree,new TGLayoutHints(kLHintsRight | kLHintsBottom | kLHintsExpandY | kLHintsExpandX));
  m_ListTree->SetLayoutManager(new TGHorizontalLayout(m_ListTree));
  m_ListTree->MapSubwindows();

  m_ListCanvas->SetContainer(m_ListTree);
  m_ListCanvas->MapSubwindows();
  m_Left->AddFrame(m_ListCanvas, new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandY | kLHintsExpandX));
  m_ListCanvas->MoveResize(10,50,120,500);

  // tab widget
  m_Tab = new TGTab(m_Right,700,500);

  m_Tab->Resize(m_Tab->GetDefaultSize());
  m_Tab->SetBackgroundColor(m_TabBgColor);
  m_Tab->SetForegroundColor(m_TabFgColor);
  m_Tab->ChangeSubframesBackground(m_BgColor);

  m_Right->AddFrame(m_Tab,new TGLayoutHints(kLHintsRight | kLHintsBottom | kLHintsExpandX | kLHintsExpandY));
  m_CanvasListTree->SetTab(m_Tab);

  m_Main->SetMWMHints(kMWMDecorAll,kMWMFuncAll,kMWMInputModeless);
  m_Main->MapSubwindows();

  m_Main->Resize(m_Main->GetDefaultSize());
  m_Main->MapWindow();
  m_Main->MoveResize(0,0,1000,500);

}

////////////////////////////////////////////////////////////////////////////////
NPL::NPOnline::~NPOnline(){
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
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::Connect(){
  // Connect to SpectraServer
  if(m_Sock){
    m_Sock->Close("force");
    delete m_Sock;
    m_Sock = NULL;
  }

  m_Sock = new TSocket(m_Address->GetDisplayText(),(Int_t) m_Port->GetNumber());
  if(m_Sock->IsValid()){
    m_Connect->SetState(kButtonDisabled);
    Update();
  }
  else{
    cout << "Connection to " << m_Address->GetDisplayText() << " " <<(Int_t) m_Port->GetNumber() << " Failed" << endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::Update(){
  if(!m_Sock || !(m_Sock->IsValid())){
    if(m_Sock){
      m_Sock->Close("force");
      delete m_Sock;
      m_Sock = NULL;
    }
    m_Connect->SetState(kButtonUp);
    return;
  }

  TMessage* message;
  m_Sock->Send("RequestSpectra");

  if(m_Sock->Recv(message)<=0){
    if(m_Sock){
      m_Sock->Close("force");
      delete m_Sock;
      m_Sock = NULL;
    }
    m_Connect->SetState(kButtonUp);
    return;
  }

  m_CanvasListTree->Clear();
  m_CanvasList = (TList*) message->ReadObject(message->GetClass());

  TGCompositeFrame* tf; 
  TRootEmbeddedCanvas* canvas;
  for(TCanvas* c = (TCanvas*) m_CanvasList->First() ; c !=0 ; c = (TCanvas*) m_CanvasList->After(c)){
    m_CanvasListTree->AddItem(c);

    TGCompositeFrame*  tab =  m_Tab->GetTabContainer(c->GetName());
    if(tab){
      tab->RemoveAll();
      TRootEmbeddedCanvas* canvas = new TRootEmbeddedCanvas(c->GetName(),tab,700,500,!kSunkenFrame); 

      c->UseCurrentStyle();
      c->SetMargin(0,0,0,0);
      canvas->AdoptCanvas(c);
      tab->SetLayoutManager(new TGHorizontalLayout(tab));
      tab->AddFrame(canvas,new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandX | kLHintsExpandY));
      ExecuteMacro(c->GetName());
    }
  }

  m_Main->MapSubwindows();
  m_Main->Layout();
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::AutoUpdate(){

  if(m_Timer){
    delete m_Timer;
    m_Timer = 0 ;
    return;
  }

  else if(m_TimerEntry->GetNumber()>0){
    m_Timer = new TTimer(m_TimerEntry->GetNumber()*1000);
    m_Timer->Connect("Timeout()", "NPL::NPOnline", this, "Update()");
    m_Timer->TurnOn();
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* CanvasList Class */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

NPL::CanvasList::CanvasList(TGMainFrame* main, TGCanvas* parent){
  string NPLPath = gSystem->Getenv("NPTOOL");  
  string path_icon = NPLPath+"/NPLib/Core/icons/polaroid.xpm";

  m_popen = gClient->GetPicture(path_icon.c_str());
  m_pclose = gClient->GetPicture(path_icon.c_str());

  m_BgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_FgColor = gROOT->GetColor(kWhite)->GetPixel();

  m_ListTree = new TGListTree(parent,kHorizontalFrame);
  m_ListTree->Connect("DoubleClicked(TGListTreeItem*,Int_t)","NPL::CanvasList",this,"OnDoubleClick(TGListTreeItem*,Int_t)");
  m_Main = main;
}
////////////////////////////////////////////////////////////////////////////////
NPL::CanvasList::~CanvasList(){
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::OnDoubleClick(TGListTreeItem* item, Int_t btn){
  AddTab(item->GetText(),m_Canvas[item->GetText()]);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::AddItem(TCanvas* c){
  TGListTreeItem*  item  = m_ListTree->AddItem(NULL,c->GetName());
  item->SetPictures(m_popen, m_pclose);
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
void NPL::CanvasList::AddTab(std::string name,TCanvas* c){
  // If the tab exist, activate
  if(m_Tab->GetTabTab(name.c_str())){
    m_Tab->SetTab(name.c_str());
    return;
  }

  TGCompositeFrame* tf = m_Tab->AddTab(name.c_str());
  TRootEmbeddedCanvas* canvas = new TRootEmbeddedCanvas("Canvas",tf,700,490,!kSunkenFrame); 

  if(c){
    c->UseCurrentStyle();
    c->SetMargin(0,0,0,0);
    canvas->AdoptCanvas(c);
    ExecuteMacro(c->GetName());
  }

  tf->SetBackgroundColor(m_BgColor);
  tf->SetForegroundColor(m_FgColor);
  tf->SetLayoutManager(new TGVerticalLayout(tf));
  tf->AddFrame(canvas,new TGLayoutHints(kLHintsTop | kLHintsLeft |kLHintsExpandX | kLHintsExpandY));
  m_Tab->Resize(m_Tab->GetDefaultSize());
  m_Tab->MoveResize(144,50,700,500);
  m_Tab->SetTab(name.c_str());
  m_Main->MapSubwindows();
  m_Main->MapWindow();
  m_Main->Layout();
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::SetTab(TGTab* tab){
  m_Tab=tab;
}

