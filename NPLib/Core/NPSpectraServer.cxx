
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
#include "NPCore.h"
#include <cstdlib>
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
void NPL::SpectraServer::Destroy(){  
  delete instance ;
  instance = 0 ;
}
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraServer::SpectraServer(){
  m_Server= new TServerSocket(9092,true,100);
  if(!m_Server->IsValid())
    exit(1);

  m_Server->SetCompressionSettings(1);
  // Add erver socket to monitor so we are notified when a client needs to be
  // accepted
  m_Monitor  = new TMonitor();
  m_Monitor->Add(m_Server,TMonitor::kRead|TMonitor::kWrite);

  // Create a list to contain all client connections
  m_Sockets = new TList;

  // Create the list of Canvas
  m_Spectra = new TList;

  NPL::SendInformation("NPL::SpectraServer","Server started on port 9092");
}

///////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::CheckRequest(){
  if(m_Server && m_Monitor){
    TSocket* s ;
    m_Monitor->ResetInterrupt();
    if((s=m_Monitor->Select(1))!=(TSocket*)-1){
      HandleSocket(s);
      }
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
    m_Monitor->Add(socket,TMonitor::kRead|TMonitor::kWrite);
    m_Sockets->Add(socket);
    NPL::SendInformation("NPL::SpectraServer","Accepted new client connection");
  }
  else{
    // we only get string based requests from the spy
    char request[64];
    if (s->Recv(request, sizeof(request)) <= 0) {
      m_Monitor->Remove(s);
      m_Sockets->Remove(s);
      delete s;
      return;
    }

    // send requested object back
    static TMessage answer(kMESS_OBJECT);
    answer.SetCompressionLevel(1);
    answer.Reset();
    TObject* h =NULL;
   if (!strcmp(request, "RequestSpectra")){
     std::cout << "Prepare" << std::endl;
      answer.WriteObject(m_Spectra);
      std::cout << "Compress" << std::endl;
      answer.Compress();
      std::cout << "Send " << std::endl;
      s->Send(answer);
      std::cout << "done" << std::endl;
    }
  
    else if (h=m_Spectra->FindObject(request)){
     answer.WriteObject(h);
     s->Send(answer);
    }

    else if (!strcmp(request, "RequestClear")){
     // TO DO 
    }
    else{ // answer nothing
      std::cout << "Fail to respond to request : " << request << std::endl; 
      s->Send(answer);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::FillSpectra(const std::string& name,const double& valx){
  // Fill the local histo
  ((TH1*) m_Spectra->FindObject(name.c_str()))->Fill(valx);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::FillSpectra(const std::string& name,const double& valx,const double& valy){
  // Fill the local histo
  ((TH2*) m_Spectra->FindObject(name.c_str()))->Fill(valx,valy);
  
}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraServer::AddSpectra(TH1* h){
  m_Spectra->Add(h);
}
////////////////////////////////////////////////////////////////////////////////
//void NPL::SpectraServer::AddSpectra(TH2* h){
//  m_Spectra->Add(h);
//}
