#ifndef __LASSAPHYSICS__
#define __LASSAPHYSICS__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold theHira Detector  Physics                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   STL
#include <vector>
#include <map>
using namespace std ;

//   ROOT
#include "TObject.h"
#include "TVector2.h"
#include "TVector3.h"


//   NPL
#include "TLassaData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"

class TLassaPhysics : public TObject, public NPL::VDetector
{
public:   //   Constructor and Destructor
    TLassaPhysics();
    ~TLassaPhysics();
    
public:
    void  Clear();
    void  Clear(const Option_t*) {};
    
public:
    vector < TVector2 > Match_EF_EB() ;
    bool Match_Si_CsI(int EF, int EB, int CristalNbr);
    int CheckEvent();
    
public:   //   Calibrated Data
    // Provide Physical Multiplicity
    Int_t EventMultiplicity;
    // Telescope
    vector<int> TelescopeNumber ;
    
    
    // Thick Si
    vector<double> ThickSi_E;
    vector<double> ThickSi_EX;
    vector<double> ThickSi_EY;
    vector<double> ThickSi_T;
    vector<int> ThickSi_StripNumberX;
    vector<int> ThickSi_StripNumberY;
    
    // CsI
    vector<double> CsI_E;
    vector<double> CsI_CrystalNumber;
    
public:   //   inherrited from VDetector
    //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
    void ReadConfiguration(NPL::InputParser);
    
    
    //   Add Parameter to the CalibrationManger
    void AddParameterToCalibrationManager();
    
    //   Activated associated Branches and link it to the private member DetectorData address
    //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
    void InitializeRootInputRaw() ;
    
    //   Activated associated Branches and link it to the private member DetectorPhysics address
    //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
    void InitializeRootInputPhysics() ;
    
    //   Create associated branches and associated private member DetectorPhysics address
    void InitializeRootOutput();
    
    //   This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter.
    void BuildPhysicalEvent();
    
    //   Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
    //   This method aimed to be used for analysis performed during experiment, when speed is requiered.
    //   NB: This method can eventually be the same as BuildPhysicalEvent.
    void BuildSimplePhysicalEvent();
    
    // Same as above but for online analysis
    void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};
    
    //   Add a Telescope using Corner Coordinate information
    void AddTelescope(TVector3 A, TVector3 B, TVector3 C, TVector3 D);
    
    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}
    
    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();
    
    //   Return false if the channel is disabled by user
    //   Frist argument is either "X","Y","SiLi","CsI"
    bool IsValidChannel(const string DetectorType, const int telescope , const int channel);

    
    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();
    
    void InitializeStandardParameter();
    
    // Use to access the strip position
    //    double GetStripPositionX( const int N , const int X , const int Y )   const{ return m_StripPositionX[N-1][X-1][Y-1] ; }  ;
    //    double GetStripPositionY( const int N , const int X , const int Y )   const{ return m_StripPositionY[N-1][X-1][Y-1] ; }  ;
    //    double GetStripPositionZ( const int N , const int X , const int Y )   const{ return m_StripPositionZ[N-1][X-1][Y-1] ; }  ;
    
    double GetStripPositionX( const int N , const int X , const int Y )   const{ return m_StripPositionX[N][X][Y] ; }  ;
    double GetStripPositionY( const int N , const int X , const int Y )   const{ return m_StripPositionY[N][X][Y] ; }  ;
    double GetStripPositionZ( const int N , const int X , const int Y )   const{ return m_StripPositionZ[N][X][Y] ; }  ;
    
    
    TVector3 GetPositionOfInteraction(const int i) const;
    
    unsigned int m_ThickSi_EXMult;//!
    unsigned int m_ThickSi_EYMult;//!
    unsigned int m_CsI_EMult;//!
    
    // Events over this value after pretreatment are not treated / avoid long treatment time on spurious events
    //unsigned int m_MaximumStripMultiplicityAllowed ; //!
    //Give the allowance in percent of the difference in energy between X and Y
    //double m_StripEnergyMatchingSigma ; //!
    //double m_StripEnergyMatchingNumberOfSigma ; //!
    
    // Give and external THiraData object to THiraPhysics. Needed for online analysis for example.
    void SetRawDataPointer(TLassaData* rawDataPointer) {m_EventData = rawDataPointer;}
    
    //   Those two method all to clear the Event Physics or Data
    void ClearEventPhysics() {Clear();}
    void ClearEventData()    {m_EventData->Clear();}
    
private: // Map of activated channel
    map< int, vector<bool> > m_XChannelStatus;//!
    map< int, vector<bool> > m_YChannelStatus;//!
    map< int, vector<bool> > m_SiChannelStatus;//!
    map< int, vector<bool> > m_CsIChannelStatus;//!
    
private:   // Data not writted in the tree
    int                m_NumberOfTelescope ;//!
    int                m_NumberOfStrip;//!
    int                m_NumberOfCrystal;//!
    TLassaData*         m_EventData ;//!
    TLassaData*         m_PreTreatedData ;//!
    TLassaPhysics*      m_EventPhysics ;//!
    
    vector< vector < vector < double > > >   m_StripPositionX;//!
    vector< vector < vector < double > > >   m_StripPositionY;//!
    vector< vector < vector < double > > >   m_StripPositionZ;//!
    
    //   Give the allowance in percent of the difference in energy between X and Y
    double m_StripEnergyMatchingNumberOfSigma;//!
    double m_StripEnergyMatchingSigma;//!
    double m_MaximumStripMultiplicityAllowed;//!
    double m_Si_DE_E_RAW_Threshold;//!
    double m_Si_EF_E_RAW_Threshold;//!
    double m_Si_EB_E_RAW_Threshold;//!
    double m_CsI_E_RAW_Threshold;//!
    double m_Si_DE_E_Threshold;//!
    double m_Si_EF_E_Threshold;//!
    double m_Si_EB_E_Threshold;//!
    double m_CsI_E_Threshold;//!
    bool m_Take_E_EF;//!
    bool m_Take_E_EB;//!
    
    // size in strip of a crystal
    int m_CsI_Size;//!
    // center position of the cristal on X
    vector< int > m_CsI_MatchingX;//!
    // center position of the cristal on Y
    vector< int > m_CsI_MatchingY;//!
    
    // If set to true, all event that do not come in front of a cristal will be ignore all time (crossing or not),
    // Warning, this option reduce statistic, however it help eliminating unrealevent event that cross the DSSD
    // And go between pad or cristal.
    bool m_Ignore_not_matching_CsI;//!

    
public:// Static constructor to be passed to the Detector Factory
    static NPL::VDetector* Construct();
    ClassDef(TLassaPhysics,1)  // HiraPhysics structure
};

#endif
