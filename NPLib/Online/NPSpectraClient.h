#ifndef NPSPECTRACLIENT_H
#define NPSPECTRACLIENT_H
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
// ROOT
#include "TSocket.h"
#include "TMonitor.h"
#include "TMessage.h"
#include "TList.h"
#include "TH1.h"
#include "TTree.h"
#include "string"

namespace NPL{
  class SpectraClient{
    public:
      SpectraClient();
      SpectraClient(std::string address, int port);
      ~SpectraClient();

    public: // Server Interface
      // Open connection with the server
      bool Connect();
      // Get the full copy of Spectrum (erase local one first)
      bool Sync();
      // Update a single spectra
      bool Update(std::string name);
      void UpdateTH1(TH1* Old, TH1* New );
      // Access the tree
      TTree* GetTree();
    private: // The sochet use for connection
      TSocket* m_Sock;
      std::string m_Address;
      int m_Port;

    private: // The spectrum list
      TList* m_Spectra; 
    public: // GUI Interface
      TList* GetSpectra();
 
    public:
      inline std::string GetAddress(){return m_Address;};
      inline int         GetPort()   {return m_Port;}
      inline void SetAddress(std:: string add){m_Address=add;};
      inline void SetPort(int port)           {m_Port=port;}
      inline void SetAddressAndPort(std::string add,int port){
        m_Address=add; m_Port= port;
      };

  };
}
#endif
