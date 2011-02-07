/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : january 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class allows to store any ascii file in a ROOT TTree.            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// Class headers
#include "TAsciiFile.h"

// C++ headers
#include <fstream>
#include <iostream>
using namespace std;

ClassImp(TAsciiFile)


TAsciiFile::TAsciiFile()
   : TNamed()
{
}



TAsciiFile::TAsciiFile(const char* name, const char* title, const char* inputAsciiFile)
   : TNamed(name, title)
{
   ReadFile(inputAsciiFile);
}



TAsciiFile::~TAsciiFile()
{
}



void TAsciiFile::ReadFile(const char* inputAsciiFile)
{
   ifstream inputStream(inputAsciiFile);
   if (!inputStream) {
      cout << "Problem opening file " << inputAsciiFile << endl;
   }
   else {
      string line;
      while (!inputStream.eof()) {
         getline(inputStream, line);
         fLines.push_back(line);
      }
   }

   inputStream.close();
}



bool TAsciiFile::IsEmpty() const
{
   if (fLines.size() == 0) return true;
   else return false;
}



void TAsciiFile::Append(const char* inputAsciiFile)
{
   ReadFile(inputAsciiFile);
}


void TAsciiFile::AppendLine(const char* AsciiLine)
{
  string line = AsciiLine ;
  
  fLines.push_back(line);
}


void TAsciiFile::WriteToFile(const char* outputAsciiFile) const
{
   ofstream outputStream(outputAsciiFile);

   for (UInt_t i = 0; i < fLines.size(); i++) outputStream << fLines[i] << endl;

   outputStream.close();
}



void TAsciiFile::WriteToFile(const char* outputAsciiFile, UInt_t begin, UInt_t end) const
{
   ofstream outputStream(outputAsciiFile);

   for (UInt_t i = 0; i < fLines.size(); i++) {
      if (i < begin || i > end) continue;
      outputStream << fLines[i] << endl;
   }

   outputStream.close();
}



void TAsciiFile::Print() const
{
   for (UInt_t i = 0; i < fLines.size(); i++) cout << fLines[i] << endl;
}



void TAsciiFile::Print(UInt_t begin, UInt_t end) const
{
   for (UInt_t i = 0; i < fLines.size(); i++) {
      if (i < begin || i > end) continue;
      cout << fLines[i] << endl;
   }
}

