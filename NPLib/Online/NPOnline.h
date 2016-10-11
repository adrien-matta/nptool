#ifndef NPONLINE_H
#define NPONLINE_H
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


#include "TGLayout.h"
#include "TGButton.h"
#include "TGTab.h"
#include "TRootEmbeddedCanvas.h"
#include "TH1.h"
#include "TSocket.h"
#include "TGListTree.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TTimer.h"
//#include "TGCanvasContainer.h"
#include "RQ_OBJECT.h"
#include<map>
using namespace std;

namespace NPL{
   void ExecuteMacro(string name);
  
  class CanvasList {
    RQ_OBJECT("CanvasList")
    protected:
      TGMainFrame* m_Main;
      TGListTree* m_ListTree;
      TGTab* m_Tab;
      map<string,TCanvas*> m_Canvas;
      const TGPicture* m_popen;     
      const TGPicture* m_pclose;   
      Pixel_t m_BgColor;
      Pixel_t m_FgColor;

   
    public:
      CanvasList(TGMainFrame* main, TGCanvas* parent);
      virtual ~CanvasList();

      // slots
      void OnDoubleClick(TGListTreeItem* item, Int_t btn);

      // Interface with NPOnline
      void SetTab(TGTab* tab);
      void AddItem(TCanvas* c);
      void Clear();
      TGListTree* GetListTree();
      // Add a new Tab to the interface
      void AddTab(std::string name="default",TCanvas* c=0);
  };

  class NPOnline{
    RQ_OBJECT("NPOnline")

    public:
      NPOnline(string address="localhost", int port=9090);
      ~NPOnline();

      void MakeGui(string address="localhost", int port=9090);
      void Connect();
      void Update();
      void AutoUpdate();
    
    private: // Server client
      TSocket* m_Sock;
      TList* m_CanvasList;

    private: // GUI stuff
      // Main window
      TGMainFrame* m_Main;
      // Menu bar
      TGVerticalFrame* m_ButtonBar;
      // Splitted frame for Tree (l) and Tab (r)
      TGHorizontalFrame* m_Split; 

      // left view port
      TGCompositeFrame* m_Left;
      // right view port
      TGCompositeFrame* m_Right;

      TGTab* m_Tab;
      TGPictureButton* m_Quit;
      TGPictureButton* m_Connect;
      TGPictureButton* m_Update;
      TGPictureButton* m_Clock;
      TGNumberEntry* m_TimerEntry;
      TTimer* m_Timer;
      

      TGTextEntry* m_Address; 
      TGNumberEntry* m_Port; 
      TGListTree* m_ListTree;
      CanvasList* m_CanvasListTree;

      TGCanvas* m_ListCanvas;
    
    private: // Style
      Pixel_t m_BgColor;
      Pixel_t m_FgColor;
      Pixel_t m_TabBgColor;
      Pixel_t m_TabFgColor;

      TH1* m_hist; 

      ClassDef(NPOnline,1);
  };

}
#endif
