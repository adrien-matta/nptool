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

#include"NPSpectraClient.h" 
#include<iostream>
#include<sstream>
#include"NPCore.h"
#include"TH2.h"
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraClient::SpectraClient(){
  m_Sock =NULL;
  m_Spectra=NULL; 
  m_Address = "localhost";
  m_Port = 9092;
}
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraClient::SpectraClient(std::string address, int port){
  m_Sock =NULL;
  m_Spectra=NULL; 
  m_Address = address;
  m_Port = port;
}
////////////////////////////////////////////////////////////////////////////////
NPL::SpectraClient::~SpectraClient(){
  if(m_Sock){
    m_Sock->Close("force");
    delete m_Sock;
    m_Sock = NULL;
  } 
  if(m_Spectra){
    m_Spectra->Delete();
    delete m_Spectra;
    m_Spectra=NULL;
  }
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::SpectraClient::Connect(){
  // Connect to SpectraServer
  if(m_Sock){
    m_Sock->Close("force");
    delete m_Sock;
    m_Sock = NULL;
  }
  m_Sock = new TSocket(m_Address.c_str(),m_Port);
  if(m_Sock->IsValid()){
    NPL::SendInformation("NPL::SpectraClient","Successful connection to spectra server");
    Sync();
    return true;
  }
  else{
    std::ostringstream message;
    message << "Connection to " << m_Address << ":" << m_Port << " Failed";
    NPL::SendWarning("NPL::SpectraClient",message.str());
    return false;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
bool NPL::SpectraClient::Sync(){
  if(!m_Sock || !(m_Sock->IsValid())){
    if(m_Sock){
      m_Sock->Close("force");
      delete m_Sock;
      m_Sock = NULL;
    }
    NPL::SendWarning("NPL::SpectraClient","Sync failed: Connection lost");

    return false;
  }

  TMessage* message=NULL;
  m_Sock->Send("RequestSpectra");

  if(m_Sock->Recv(message)<=0){
    if(m_Sock){
      m_Sock->Close("force");
      delete m_Sock;
      m_Sock = NULL;
    }
    NPL::SendWarning("NPL::SpectraClient","Sync failed: message return unreadable");

    return false ;
  }

  if(message){
    if(m_Spectra){
      m_Spectra->Delete();
      delete m_Spectra;
      m_Spectra = NULL;
    }
   
    //message->Uncompress();
    m_Spectra = (TList*) message->ReadObject(message->GetClass());
    if(m_Spectra){
      NPL::SendInformation("NPL::SpectraClient","Successful sync of spectra list");
      return true;
    }
    
    else{
      NPL::SendInformation("NPL::SpectraClient","Sync return a NULL spectra list");
      return false;
    }
  }

  else{
    NPL::SendInformation("NPL::SpectraClient","Server returned an empty message");
    return false;
  }

}

////////////////////////////////////////////////////////////////////////////////
bool NPL::SpectraClient::Update(std::string name){
  if(!m_Sock || !(m_Sock->IsValid())){
    if(m_Sock){
      m_Sock->Close("force");
      delete m_Sock;
      m_Sock = NULL;
    }
    NPL::SendWarning("NPL::SpectraClient","Update failed: Connection lost");

    return false;
  }

  TMessage* message=NULL;
  m_Sock->Send(name.c_str());

  if(m_Sock->Recv(message)<=0){
    if(m_Sock){
      m_Sock->Close("force");
      delete m_Sock;
      m_Sock = NULL;
    }

    NPL::SendWarning("NPL::SpectraClient","Update failed: message return unreadable");

    return false;
  }

  if(message){
    // Get the current spectra
    TH1* h = (TH1*) m_Spectra->FindObject(name.c_str());
    // Get the new one
    TH1* n = (TH1*) message->ReadObject(message->GetClass());
    UpdateTH1(h,n); 
    delete n;  
    return true;
  }
  
  else{
    NPL::SendInformation("NPL::SpectraClient","Server return empty update list");
    return true;
  }
}

////////////////////////////////////////////////////////////////////////////////
TList* NPL::SpectraClient::GetSpectra(){
  return m_Spectra;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::SpectraClient::UpdateTH1(TH1* Old , TH1* New){
  // Save the Ranges on the different axis
  double minX = Old->GetXaxis()->GetBinLowEdge(Old->GetXaxis()->GetFirst());
  double maxX = Old->GetXaxis()->GetBinUpEdge(Old->GetXaxis()->GetLast());
  double minY = Old->GetYaxis()->GetBinLowEdge(Old->GetYaxis()->GetFirst());
  double maxY = Old->GetYaxis()->GetBinUpEdge(Old->GetYaxis()->GetLast());

  // Put new stuff in old object
  New->Copy(*Old) ;    
  // Reset the axis range
  Old->SetAxisRange(minX,maxX);
  if(Old->GetDimension()==2)
    Old->SetAxisRange(minY,maxY,"Y");
   
  // Refresh style
  Old->UseCurrentStyle();      
 }

