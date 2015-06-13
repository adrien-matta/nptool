#include "NPSpectraServer.h"

NPL::SpectraServer* NPL::SpectraServer::instance = 0 ;
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer* NPL::SpectraServer::getInstance(){
  if(!instance) 
    instance = new NPL::SpectraServer();

  return instance;
}

////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer::SpectraServer(){
  m_Server= new TServerSocket(9090,true);
  if(!m_Server->IsValid())
    exit(1);

  // Add server socket to monitor so we are notified when a client needs to be
  // accepted
  m_Monitor  = new TMonitor;
  m_Monitor->Add(m_Server);

  // Create a list to contain all client connections
  m_Sockets = new TList;

  // Create the list of Canvas
  m_Canvas = new TList;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::AddCanvas(TCanvas* c){
  m_Canvas->Add(c);
}

////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::Start(){
  if(m_Server){
    while(1){
      TSocket* s ;
      if((s=m_Monitor->Select(20))!=(TSocket*)-1)
        HandleSocket(s);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer::~SpectraServer(){}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::HandleSocket(TSocket* s){
  if (s->IsA() == TServerSocket::Class()) {
    // accept new connection from spy
    TSocket* socket = ((TServerSocket*)s)->Accept();
    m_Monitor->Add(socket);
    m_Sockets->Add(socket);
  }
  else {
    // we only get string based requests from the spy
    char request[64];
    if (s->Recv(request, sizeof(request)) <= 0) {
      m_Monitor->Remove(s);
      m_Sockets->Remove(s);
      delete s;
      return;
    }

    // send requested object back
    TMessage answer(kMESS_OBJECT);
    if (!strcmp(request, "RequestSpectra"))
      answer.WriteObject(m_Canvas);

    s->Send(answer);
  }
}
