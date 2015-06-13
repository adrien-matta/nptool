#ifndef NPONLINE_H
#define NPONLINE_H
#include "TGLayout.h"
#include "TGButton.h"
#include "TGTab.h"
#include "TRootEmbeddedCanvas.h"
#include "TH1.h"
#include "TSocket.h"
#include "RQ_OBJECT.h"
using namespace std;

namespace NPL{
  class NPOnline{
    RQ_OBJECT("NPOnline")

    public:
      NPOnline();
      ~NPOnline();

      void Connect();
      void Update();
     
      // Add a new Tab to the interface
      void AddTab(std::string name="default",TCanvas* c=0);
    
    private: // Server client
      TSocket* m_Sock;
      TList* m_CanvasList;

    private: // GUI stuff
      TGMainFrame* m_Main;
      TGTab* m_Tab;
      TGLayoutHints* m_Lcan;
      TGHorizontalFrame* m_Horz;
      TGLayoutHints* m_Lbut;
      TGLayoutHints* m_Lhorz;
      TGLayoutHints* m_Lresize;
      TGButton* m_Quit;
      TGButton* m_Connect;
      TGButton* m_Update;
      
    private: // Style
      Pixel_t m_BgColor;
      Pixel_t m_FgColor;

      TH1* m_hist; 

      ClassDef(NPOnline,1);
  };

}
#endif
