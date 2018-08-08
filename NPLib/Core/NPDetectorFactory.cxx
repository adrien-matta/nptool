
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


#include"NPDetectorFactory.h"
#include"NPCore.h"
#include<dlfcn.h>
#include<fstream>
#include<iostream>
#include<stdlib.h>

// NPTool
#include "NPOptionManager.h"

NPL::DetectorFactory* NPL::DetectorFactory::m_Instance = 0;
////////////////////////////////////////////////////////////////////////////////
NPL::DetectorFactory::DetectorFactory(){

}

/////////////////////////////////////////////////////////////////////////////////
NPL::DetectorFactory::~DetectorFactory(){
  m_Instance = 0 ;
}

////////////////////////////////////////////////////////////////////////////////
NPL::DetectorFactory* NPL::DetectorFactory::getInstance(){
  if(!m_Instance){
    m_Instance = new NPL::DetectorFactory();
  }

  return m_Instance;

}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorFactory::ReadClassList(std::string FileList){
    std::ifstream InFile(FileList.c_str());

    if(!InFile.is_open()){
      std::string error = "Detector Class List file " +FileList +" Not found";
      NPL::SendErrorAndExit("NPL::NPL::DetectorFactory",error);
      exit(1);
    }

    std::string Token, LibName;
    while(InFile >> Token >> LibName)
      m_TokenLib[Token] = LibName; 
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorFactory::CreateClassList(std::string FileList){
  std::ofstream outFile(FileList.c_str());
  std::map<std::string,std::string>::iterator it;

  for(it = m_TokenLib.begin();it!=m_TokenLib.end();it++){
    outFile << it->first << " " << it->second << std::endl;
  }


}
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* NPL::DetectorFactory::Construct(std::string Token){
  std::map<std::string,ClassDetectorFactoryFn>::iterator it;
  if(m_Construct.find(Token)!=m_Construct.end())
    return  m_Construct[Token]();

  else if( m_TokenLib.find(Token)!=m_TokenLib.end()){
   // Add absolute path to library name
    std::string path = getenv("NPTOOL"); 
    std::string libName = path+"/NPLib/lib/"+m_TokenLib[Token];
    dlopen(libName.c_str(),RTLD_NOW | RTLD_GLOBAL);
    char* LibError = dlerror();
	if(m_Construct.find(Token)!=m_Construct.end())
      		return  m_Construct[Token]();
  
    else{
      
      std::cout << "Warning: Detector with Token " << Token << " has no Constructor or no Library" << std::endl;
      if(LibError){
        std::cout << "Library loading failes with error: " << std::endl;
        std::cout << LibError << std::endl << std::endl;
      }
      
      return NULL;
    }
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorFactory::AddDetector(std::string Token, ClassDetectorFactoryFn fn){
  m_Construct[Token] = fn;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorFactory::AddToken(std::string Token, std::string LibName){
  // Make sure the lib name is correct:
  // Strip it from lib and NP and extension
  std::string remword = "lib";
  size_t pos;
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }

  remword = "NP";
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }
 
  remword = ".so";
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }

  remword = ".dylib";
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }


  // Create the full name:
  LibName = "libNP"+LibName + NPOptionManager::getInstance()->GetSharedLibExtension();
  
  m_TokenLib[Token] = LibName;
}
