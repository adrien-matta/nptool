#ifndef TVamosPHYSICS_H
#define TVamosPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : octobre 2018                                             *
 * Last update    : 09/01/2019                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Vamos Treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// C++ headers 
#include <vector>
#include <map>
#include <string>
using namespace std;

// ROOT headers
#include "TObject.h"
#include "TH1.h"
#include "TVector3.h"
// NPTool headers
#include "TVamosData.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
#include"NPEnergyLoss.h"
#include"NPReaction.h"


class TVamosPhysics : public TObject, public NPL::VDetector {
    //////////////////////////////////////////////////////////////
    // constructor and destructor
    public:
        TVamosPhysics();
        ~TVamosPhysics() {};


        //////////////////////////////////////////////////////////////
        // Inherited from TObject and overriden to avoid warnings
    public: 
        void Clear();   
        void Clear(const Option_t*) {};


        //////////////////////////////////////////////////////////////
        // data obtained after BuildPhysicalEvent() and stored in
        // output ROOT file
    public:
        vector<int>      DetectorNumber;
        vector<int>      DriftDetectorNumber;
        vector<double>   Energy;
        vector<double>   Time;
        vector<double>   Drift_X;
        vector<double>   Drift_Y;
        vector<double>   X_target;
        vector<double>   Y_target;
        vector<double>   Theta_target;
        vector<double>   Phi_target;
        vector<double>   TargetThickness;

        //////////////////////////////////////////////////////////////
        // methods inherited from the VDetector ABC class
    public:
        // read stream from ConfigFile to pick-up detector parameters
        void ReadConfiguration(NPL::InputParser);

        // add parameters to the CalibrationManger
        void AddParameterToCalibrationManager();

        // method called event by event, aiming at extracting the 
        // physical information from detector
        void BuildPhysicalEvent();

        // same as BuildPhysicalEvent() method but with a simpler
        // treatment
        void BuildSimplePhysicalEvent();

        // same as above but for online analysis
        void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

        // activate raw data object and branches from input TChain
        // in this method mother branches (Detector) AND daughter leaves 
        // (fDetector_parameter) have to be activated
        void InitializeRootInputRaw();

        // activate physics data object and branches from input TChain
        // in this method mother branches (Detector) AND daughter leaves 
        // (fDetector_parameter) have to be activated
        void InitializeRootInputPhysics();

        // create branches of output ROOT file
        void InitializeRootOutput();

        // clear the raw and physical data objects event by event
        void ClearEventPhysics() {Clear();}      
        void ClearEventData()    {m_EventData->Clear();}   

        //////////////////////////////////////////////////////////////
        // specific methods to Vamos array
    public:

        // read the user configuration file. If no file is found, load standard one
        void ReadAnalysisConfig();

        // objects are not written in the TTree
    private:
        TVamosData*         m_EventData;        //!
        /* TVamosData*         m_PreTreatedData;   //! */
        TVamosPhysics*      m_EventPhysics;     //!
        /* NPL::Reaction myReaction; //! */

        double m_TargetThickness;
        double DistanceTargetVamos;
        vector<double> Z_DriftChambers;//
        double Z1, Z2 ;

        // getters for raw and pre-treated data object
    public:
        TVamosData* GetRawData()        const {return m_EventData;}

        // parameters used in the analysis
    private:
        // thresholds
        double m_E_RAW_Threshold; //!
        double m_E_Threshold;     //!

    private:
        int m_NumberOfDetectors;  //!

        // Static constructor to be passed to the Detector Factory
    public:
        static NPL::VDetector* Construct();

        ClassDef(TVamosPhysics,1)  // VamosPhysics structure
};
#endif
