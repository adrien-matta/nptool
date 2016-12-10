#ifndef GaspardTracker_H
#define GaspardTracker_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 31/08/09                                                 *
 * Last update    : 10/09/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class is mainly an interface to the                      *
 *             TGaspardTrackerPhysics class and it deals with the geometrical*
 *             correspondance between strip number and absolute coordinates  *
 *             (X, Y, Z) of interaction.                                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   + 10/09/09: Add support for Square and DummyShape shapes                *
 *               (N. de Sereville)                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// NPL
#include "NPVDetector.h"
#include "TGaspardTrackerData.h"
#include "TGaspardTrackerPhysics.h"
#include "GaspardTrackerModule.h"
#include "NPInputParser.h"

// C++
#include <map>
#include <vector>

// Root
#include "TObject.h"
#include "TVector3.h"

class GaspardTracker : public NPL::VDetector{
  public:
    GaspardTracker();
    virtual ~GaspardTracker();

  public:
    void Clear();
    void Clear(const Option_t*) {};

  public:
    /////////////////////////////////////
    // Innherited from VDetector Class //
    /////////////////////////////////////
    // Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
    void ReadConfiguration(NPL::InputParser);

    // Read stream at CalibFile and pick-up calibration parameter using Token
    // If argument is "Simulation" no change calibration is loaded
    void ReadCalibrationFile(string);

    // Activated associated Branches and link it to the private member DetectorData address
    // In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
    void InitializeRootInputRaw();

    // In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
    void InitializeRootInputPhysics();

    // Create associated branches and associated private member DetectorPhysics address
    void InitializeRootOutput();

    // This method is called at each event read from the Input Tree. 
    // The aim is to build treat Raw dat in order to extract physical parameter. 
    void BuildPhysicalEvent();

    // Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
    // This method aimed to be used for analysis performed during experiment, when speed is requiered.
    // NB: This method can eventually be the same as BuildPhysicalEvent.
    void BuildSimplePhysicalEvent();

    // Those two method all to clear the Event Physics or Data
    void ClearEventPhysics()   {m_EventPhysics->Clear();}
    void ClearEventData()      {m_EventData->Clear();}


  public:
    void DumpModulesMap();
    void DumpStrippingScheme(int moduleNumber);

  private:
    map<int, GaspardTrackerModule*>   m_ModulesMap;

  public:
    // Get Root input and output objects
    TGaspardTrackerData*    GetEventData()      {return m_EventData;}
    TGaspardTrackerPhysics* GetEventPhysics()   {return m_EventPhysics;}

    // To be called after a build Physical Event 
    double   GetEnergyDeposit();
    TVector3 GetPositionOfInteraction();


  private:
    ////////////////////////////////////////
    // Root Input and Output tree classes //
    ////////////////////////////////////////
    TGaspardTrackerData*      m_EventData;
    TGaspardTrackerPhysics*   m_EventPhysics;

  public: // Static constructor to be passed to the Detector Factory
    static NPL::VDetector* Construct();
};

#endif
