#ifndef __TASCIIFILE__
#define __TASCIIFILE__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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

// ROOT headers
#include "TNamed.h"

// C++ headers
#include <vector>
#include <string>
using namespace std;


class TAsciiFile : public TNamed {
 private :
   vector<string>   fLines;

 protected :
   void ReadFile(const char* inputAsciiFile);

 public :
   TAsciiFile();
   TAsciiFile(const char* name, const char* title, const char* inputAsciiFile);
   virtual ~TAsciiFile();

   void Append(const char* inputAsciiFile);
   void AppendLine(const char* AsciiLine);
   void WriteToFile(const char* outputAsciiFile) const;
   void WriteToFile(const char* outputAsciiFile, UInt_t begin, UInt_t end) const;
   void Print(const Option_t*) const {}; 
   void Print() const;
   void Print(UInt_t begin, UInt_t end) const;
   bool IsEmpty() const;

   ClassDef(TAsciiFile, 1);   // Class TAsciiFile for storing ascii text
};

#endif
