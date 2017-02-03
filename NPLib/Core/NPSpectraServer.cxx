
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



#include "NPSpectraServer.h"
#include<cstdlib>
#include<unistd.h>
#include<iostream>
NPL::SpectraServer* NPL::SpectraServer::instance = 0 ;
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer* NPL::SpectraServer::getInstance(){
  if(!instance) 
    instance = new NPL::SpectraServer();

  return instance;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::Destroy(){  
  delete instance ;
  instance = 0 ;
}
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer::SpectraServer(){
  m_Server= new TServerSocket(9090,true);
  if(!m_Server->IsValid())
    exit(1);
  
  m_Server->SetCompressionSettings(1);
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
void NPL::SpectraServer::CheckRequest(){
  if(m_Server){
      TSocket* s ;
      if((s=m_Monitor->Select(1))!=(TSocket*)-1)
        HandleSocket(s);
  }
}

////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer::~SpectraServer(){
  // tell the socket to stop
  m_Server->Close("force");
  delete m_Server;
  m_Server=0;
  delete m_Monitor;
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
