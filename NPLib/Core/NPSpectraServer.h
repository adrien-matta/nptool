#ifndef NPSPECTRASERVER_H
#define NPSPECTRASERVER_H

#include "TSocket.h"
#include "TServerSocket.h"
#include "TMonitor.h"
#include "TMessage.h"
#include "TList.h"
#include "TCanvas.h"
namespace NPL{
  class SpectraServer{
    public:
      static SpectraServer* getInstance();
      void Destroy(); 
    private:
      SpectraServer();
      ~SpectraServer();

    private:
      static SpectraServer* instance;
   
    public:
      void HandleSocket(TSocket* s);
      void AddCanvas(TCanvas* c);
      void CheckRequest();
   private:
    bool m_stop;
    TServerSocket* m_Server;     
    TMonitor* m_Monitor;     
    TList* m_Sockets;
    TList* m_Canvas;
  };
}
#endif
