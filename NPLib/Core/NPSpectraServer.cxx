#include "NPSpectraServer.h"
#include <unistd.h>
#include<iostream>
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


  m_stop = false;
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
    while(!m_stop){
      if(!m_Server)
        return;

      TSocket* s ;
      if((s=m_Monitor->Select(20))!=(TSocket*)-1)
        HandleSocket(s);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer::~SpectraServer(){
 std::cout << 1 << std::endl;
  // tell the socket to stop
  m_stop = true;
  // wait for it to be stopped
  sleep(100);
 std::cout << 2 << std::endl;

  m_Server->Close("force");

  std::cout << 3 << std::endl;

  delete m_Server;
 std::cout << 4 << std::endl;

  m_Server=0;
  delete m_Monitor;
 std::cout << 5 << std::endl;

  m_Monitor = 0;
  instance = 0 ;
}
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
