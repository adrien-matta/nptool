/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : 9 May 2017                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class make an interface to the elog command line                     *
 * The elog CLI must be installed on the system for this class to work       *
 * The interface is designed to help create elog entry from within nptool    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#include <iostream>
#include "NPElog.h"
#include "NPCore.h"
#include "NPInputParser.h"
#include "NPOptionManager.h"

////////////////////////////////////////////////////////////////////////////////
NPL::Elog::Elog(){
}
////////////////////////////////////////////////////////////////////////////////
NPL::Elog::~Elog(){
}
////////////////////////////////////////////////////////////////////////////////
void NPL::Elog::ReadConfiguration(std::string filename){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();
  NPL::InputParser parser(filename);

  std::vector<NPL::InputBlock*> block = parser.GetAllBlocksWithToken("elog");
  if(block.size()!=1){
    NPL::SendWarning("NPL::Elog","Elog configuration file has wrong formatting");
    return;
  }

  std::vector<std::string> token = {"host","port","port","ssl","logbook","username","password"};
  if(!block[0]->HasTokenList(token)){
    NPL::SendWarning("NPL::Elog","Elog configuration file has wrong formatting");
    return;
  }
  if(verbose)
    std::cout << "//// Elog Configuration " << std::endl;
  m_host = block[0]->GetString("host");
  m_port = block[0]->GetString("port");
  m_ssl  = block[0]->GetString("ssl");
  m_logbook = block[0]->GetString("logbook");
  m_username = block[0]->GetString("username");
  m_password = block[0]->GetString("password");

  std::vector<NPL::InputBlock*> attributes = parser.GetAllBlocksWithToken("attribute");
  unsigned int size = attributes.size();
  for(unsigned int i = 0 ; i < size ; i++){
    if(attributes[i]->HasToken("values"))
      m_AttributesValues[attributes[i]->GetMainValue()]=attributes[i]->GetVectorString("values");
  }

}
////////////////////////////////////////////////////////////////////////////////
bool NPL::Elog::CheckServer(){
  // to be done
  return true;
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::Elog::CheckEntry(std::vector<std::string>& attributes, std::vector<std::string>& val){
  return true;
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::Elog::CreateEntry(std::vector<std::string>& attributes, std::vector<std::string>& val,std::string message, std::vector<std::string> attachement){
  
  if(CheckEntry(attributes,val)){
    std::string cmd = "elog -h " + m_host + " -p " + m_port + " -s " + m_ssl; 
    cmd +=  " -u " + m_username +  " " + m_password;
    cmd +=  " -l " + m_logbook;
    unsigned int size = attributes.size();
    for(unsigned int i = 0 ; i < size ; i++)
      cmd += " -a " + attributes[i] + "=" + val[i];
  
    cmd += " \""+message+"\"";

    size = attachement.size();
    for(unsigned int i = 0 ; i < size ; i++){
      cmd +=  " -f " + attachement[i]; 
    }

    int code = system(cmd.c_str());
  }

  return true;
}
