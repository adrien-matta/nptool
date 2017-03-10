#ifndef NPINPUTPARSER
#define NPINPUTPARSER
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : December 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *   This class allow for parsing of tabulated input file with unit and token*
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


// STL
#include<string>
#include<vector>

// ROOT
#include"TVector3.h"


namespace NPL{

  static std::string token_separator = "=";
  std::string StripSpaces(std::string line);
  std::string ToLower(std::string line);
  double ApplyUnit(double value, std::string unit);
  unsigned int GetLevel(std::string line);

  class InputBlock{
    public:
      InputBlock(std::string line);
      ~InputBlock(){};

    private:
      unsigned int m_Level;
      std::string m_MainToken;
      std::string m_MainValue;
      std::vector<std::string> m_Token;
      std::vector<std::string> m_Value; 
      std::vector<NPL::InputBlock*> m_SubBlock;

    public:
      void AddLine(std::string line);  
      std::string ExtractToken(std::string line,std::string separator="");
      std::string ExtractValue(std::string line,std::string separator="");

    public:
      std::string GetToken(unsigned int i){return m_Token[i];};
      std::string GetValue(unsigned int i){return m_Value[i];};
      std::string GetValue(std::string Token);

    public:  
      bool HasTokenList(std::vector<std::string> TokenList);
      bool HasToken(std::string Token);

    public:
      std::string GetMainToken(){return m_MainToken;};
      std::string GetMainValue(){return m_MainValue;};

      unsigned int GetSize(){return m_Token.size();};
      double GetDouble(std::string Token,std::string default_unit);
      int GetInt(std::string Token);
      std::string GetString(std::string Token);
      TVector3 GetTVector3(std::string Token,std::string default_unit);
      std::vector<double> GetVectorDouble(std::string Token,std::string default_unit);
      std::vector<int> GetVectorInt(std::string Token);
      std::vector<std::string> GetVectorString(std::string Token);
      std::vector<NPL::InputBlock*> GetSubBlock(std::string Token);

    public:
      void Dump();
  };
  ////////////////////////////////////////////////////////////////////////////////
  class InputParser{
    public:
      InputParser(){};
      InputParser(std::string filename,bool ExitOnError=true) {ReadFile(filename,ExitOnError);}
      ~InputParser(){};

    private:
      std::vector<InputBlock*> m_Block;
      std::map<std::string, vector<std::string> > m_Aliases;

    private:
      int m_verbose;

    public:
      void ReadFile(std::string filename,bool ExitOnError=true);
      void TreatAliases();
      void TreatOneAlias();
      void Dump();
      void Print() {Dump();}
      void Clear();
      std::vector<InputBlock*> GetAllBlocksWithToken(std::string Token);
      std::vector<InputBlock*> GetAllBlocksWithTokenAndValue(std::string Token,std::string Value);

      std::vector<std::string> GetAllBlocksToken();


    private:
      bool IsNotComment(std::string line);
  };
}

#endif
