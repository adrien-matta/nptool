#ifndef NPDELTASPECTRA_H
#define NPDELTASPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : 11 May 2017                                              *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class hold the record of pas filling of an histo server             *
 *  It is used in Online module to guarantee a faster Histo Sync             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// STL
#include<string>
#include<map>
#include<vector>
#include<algorithm>

// ROOT
#include"TObject.h"
#include"TList.h"

namespace NPL{
  class DeltaSpectra:public TObject{
    public:
      DeltaSpectra():TObject(){};
      ~DeltaSpectra(){};

    private:
      // Hold the record of Filling 
      std::map<std::string , std::pair<std::vector<double> ,std::vector<double> > > m_Delta;

    public:
      // Record a Fill for TH1
      void Fill(std::string name, double valx);
      // Record a Fill for TH2
      void Fill(std::string name, double valx, double valy);
      // Erase all recorded data (typically called after a client update)
      void Clear();
      // Fill in the spectra using there name and the internal data
      void UpdateLocalSpectra(TList* local);
      // Print for debug
      void Print();

      ClassDef(NPL::DeltaSpectra,1)  // NPL::DeltaSpectra structure

  }; 
}


#endif
