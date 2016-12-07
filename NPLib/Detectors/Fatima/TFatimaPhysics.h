#ifndef TFATIMAPHYSICS_H
#define TFATIMAPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:  M. Labiche    contact address: marc.labiche@stfc.ac.uk  *
 *                                                                           *
 * Creation Date  : 04/12/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the FATIMA detector.                                       *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STD
#include <vector>
#include <cstdlib>
using namespace std ;

// NPL
#include "NPVDetector.h"
#include "TFatimaData.h"
#include "NPInputParser.h"
// Root
#include "TObject.h"
#include "TVector3.h"


class TFatimaPhysics : public TObject, public NPL::VDetector{
  public:
    TFatimaPhysics();
    ~TFatimaPhysics();

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
    void ClearEventPhysics()  {m_EventPhysics->Clear();}
    void ClearEventData()     {m_EventData->Clear();}

  public:
    ////////////////////////////////
    // Specific to Fatima //
    ////////////////////////////////
    // Case of a Square module
    // Add a Module using Corner Coordinate information
    void AddDetector(TVector3 A,TVector3 B,TVector3 C,TVector3 D);//!

    // Add a Module using R Theta Phi of Si center information
    void AddDetector(double theta,
        double phi,
        double r,
        double beta_u,
        double beta_v,
        double beta_w);//!

    // Getters to retrieve the (X,Y,Z) coordinates of a pixel defined by strips (X,Y)
    double GetPositionX(int N ,int X ,int Y)   { return m_PositionX[N-1][X-1][Y-1]; }
    double GetPositionY(int N ,int X ,int Y)   { return m_PositionY[N-1][X-1][Y-1]; }
    double GetPositionZ(int N ,int X ,int Y)   { return m_PositionZ[N-1][X-1][Y-1]; }
    double GetNumberOfModule()             { return m_NumberOfModule; }

    // Get Root input and output objects
    TFatimaData*    GetEventData()      {return m_EventData;}//!
    TFatimaPhysics*   GetEventPhysics()   {return m_EventPhysics;}//!

    // To be called after a build Physical Event
    double   GetEnergyDeposit();
    double   GetEnergyInDeposit();
    double   GetEnergyOutDeposit();
    TVector3   GetPositionOfInteraction();

    void      Print();


  private:
    ////////////////////////////////////////
    // Root Input and Output tree classes //
    ////////////////////////////////////////
    TFatimaData*      m_EventData;//!
    TFatimaPhysics*   m_EventPhysics;//!


  private:
    // Spatial Position of Strip Calculated on basis of detector position
    int m_NumberOfModule;
    vector< vector < vector < double > > >   m_PositionX;//!
    vector< vector < vector < double > > >   m_PositionY;//!
    vector< vector < vector < double > > >   m_PositionZ;//!


  public: // Data Member
    // Provide Physical Multiplicity
    Int_t      FatimaEventMult;

    //   FirstStage
    vector<double>   FatimaLaBr_E;

    //   SecondStage
    vector<double>   FatimaNaI_E;

    // Physical Value  
    vector<double>   FatimaTotalEnergy;
    vector<double>   FatimaInTotalEnergy;
    vector<double>   FatimaOutTotalEnergy;

    public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TFatimaPhysics,1)
};

#endif
