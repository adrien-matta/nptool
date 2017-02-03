#ifndef VDectector_H
#define VDectector_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA  contact address: matta@lpccaen.in2p3.fr  *
 *                                                                           *
 * Creation Date   :   June 2009                                             *
 * Last update     :   Nov 2013                                              *
 *---------------------------------------------------------------------------*
 * Decription:  Class VDetector (virtual) for NPAnalysis                     *
 * All detector class used in NPAnalysis should derived from this virtual    *
 *  class. Those VDetector daughter will deal with geometry, calibration and *
 *  basic data treatment.                                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   See MUST2 array for exemple of VDetector derived class                  *
 *                                                                           *  
 *   Adding Fill Spectra Method to fill control Histogramm                   *
 *****************************************************************************/

// ROOT
#include "TH1.h"
#include "TCanvas.h"
// STL 
#include <string>
#include <vector>
#include <map>

// NPL
#include "NPInputParser.h"

namespace NPL {
  std::string itoa(const int&); 

  class VDetector{
    public:

      //  Default Constructor and destructor
      VDetector()   ;
      virtual ~VDetector()   ;

      //  Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
      virtual void ReadConfiguration(NPL::InputParser) {} ;

      //  Add Parameter to the CalibrationManger
      virtual void AddParameterToCalibrationManager() {} ;      

      //  Activated associated Branches and link it to the private member DetectorData address
      //  In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
      virtual void InitializeRootInputRaw() {} ;

      //  Activated associated Branches and link it to the private member DetectorPhysics address
      //  In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
      virtual void InitializeRootInputPhysics() {} ;

      //  Create associated branches and associated private member DetectorPhysics address
      virtual void InitializeRootOutput() {} ;

      //  This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
      virtual void BuildPhysicalEvent() {} ;

      //  Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
      //  This method aimed to be used for analysis performed during experiment, when speed is requiered.
      //  NB: This method can eventually be the same as BuildPhysicalEvent.
      virtual void BuildSimplePhysicalEvent() {} ;

      //  Those two method all to clear the Event Physics or Data
      virtual void ClearEventPhysics() {} ;
      virtual void ClearEventData() {} ;

      // Method related to the TSpectra classes, aimed at providing a framework for online applications
      // Instantiate the Spectra class and the histogramm throught it
      virtual void InitSpectra() {};
      // Fill the spectra hold by the spectra class
      virtual void FillSpectra()  {};
      // Write the spectra to a file
      virtual void WriteSpectra()  {};
      // Used for Online mainly, perform check on the histo and for example change their color if issues are found
      virtual void CheckSpectra() {};
      // Used for Online only, clear all the spectra hold by the Spectra class
      virtual void ClearSpectra() {};
      // Used for Online only, get all the spectra hold by the Spectra class
      virtual  std::map< std::string , TH1*> GetSpectra() {std::map< std::string, TH1* > x; return x;};
      // Used for Online only, get all the canvases
      virtual std::vector<TCanvas*> GetCanvas(){std::vector<TCanvas*> x ; return x;};

    private:   //   The list below is here to help you building your own detector
      /*
      //   GRU and Simulated Tree output are made of Data Object wich contain all the RAW parameter output by Detectors during an experiment.
      //   You need to add a pointer to those kind of object in order to received data from the Tree at each getEntry call. Just replace Detector
      //   by your Detector name, like Must2Data for Must2

      DetectorData* EventData   ;

      //   Position of detector: An array which held geometric information of detector, such as Strip position,...
      //   Dimension must be suited for your need

      vector< vector <double > >   DetectorPosition   ;

      //   Calibration are passed to the EventPhysics Object, so you don't need to store them here

      nothing   noCalibration   ;

*/
  ClassDef(VDetector,1) 
  };

}

#endif
