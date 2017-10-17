#ifndef NPELOG_H
#define NPELOG_H
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
#include <string>
#include <vector>
#include <map>

namespace NPL{
  class Elog{
    public:
      Elog();
      ~Elog();

    public:
      // Read the elog configuration file in nptool format
      void ReadConfiguration(std::string filename);
      // check if the server is reachable
      bool CheckServer();
      // Check the submitted entry match the required grammar (all attribute filled correctly)
      bool CheckEntry(std::vector<std::string>& attributes, std::vector<std::string>& val);
      // Submit the entry
      bool CreateEntry(std::vector<std::string>& attributes, std::vector<std::string>& val, std::string message , std::vector<std::string> attachement=std::vector<std::string>());
     
    private: // Login configuration
      std::string m_host;
      std::string m_port;
      std::string m_ssl;
      std::string m_logbook;
      std::string m_username;
      std::string m_password;

    private: // attribute and their possible values
     std::map<std::string, std::vector<std::string> > m_AttributesValues;
    public: // Accessort to the define attributes and their default value
     inline std::map<std::string, std::vector<std::string> > GetAttributesValues() { return m_AttributesValues;};
  };
}
#endif
