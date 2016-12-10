#ifndef __HIRAPHYSICS__
#define __HIRAPHYSICS__
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

// ROOT
#include "TVector2.h"
#include "TVector3.h"
#include "TObject.h"
#include "TH1.h"

//   NPL
#include "THiraData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"

class THiraPhysics : public TObject, public NPL::VDetector
{
public:   //   Constructor and Destructor
    THiraPhysics();
    ~THiraPhysics();
    
public:
    void  Clear();
    void  Clear(const Option_t*) {};
    
public:
    vector < TVector2 > Match_EF_EB() ;
    bool Match_Si_CsI(int EF, int EB, int CristalNbr);
    bool Match_Si_DE_E(int EF, int DEStipNumber);
    int CheckEvent();
    
public:   //   Calibrated Data
    // Provide Physical Multiplicty
    Int_t EventMultiplicity;
    // Telescope
    vector<int> TelescopeNumber ;
    
    // Thin Si
    vector<double> ThinSi_E;
    vector<double> ThinSi_T;
    vector<int> ThinSi_StripNumber;
    
    // Thick Si
    vector<double> ThickSi_E;
    vector<double> ThickSi_EX;
    vector<double> ThickSi_EY;
    vector<double> ThickSi_T;
    vector<double> ThickSi_TX;
    vector<double> ThickSi_TY;
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
    
    void InitializeStandardParameter();
    
    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();

    
    // Use to access the strip position
    double GetStripPositionX( const int N , const int X , const int Y )   const{ return m_StripPositionX[N][X][Y] ; }  ;
    double GetStripPositionY( const int N , const int X , const int Y )   const{ return m_StripPositionY[N][X][Y] ; }  ;
    double GetStripPositionZ( const int N , const int X , const int Y )   const{ return m_StripPositionZ[N][X][Y] ; }  ;
    
    TVector3 GetPositionOfInteraction(const int i) const;
    
    unsigned int m_ThinSi_EMult;//!
    unsigned int m_ThickSi_EXMult;//!
    unsigned int m_ThickSi_EYMult;//!
    unsigned int m_ThickSi_TXMult;//!
    unsigned int m_ThickSi_TYMult;//!
    unsigned int m_CsI_EMult;//!
    unsigned int m_CsI_TMult;//!
    
    
    // Give and external THiraData object to THiraPhysics. Needed for online analysis for example.
    void SetRawDataPointer(THiraData* rawDataPointer) {m_EventData = rawDataPointer;}
    
    //   Those two method all to clear the Event Physics or Data
    void ClearEventPhysics() {Clear();}
    void ClearEventData()    {m_EventData->Clear();}
    
private: // Map of activated channel
    map< int, vector<bool> > m_DEChannelStatus;//!
    map< int, vector<bool> > m_EFChannelStatus;//!
    map< int, vector<bool> > m_EBChannelStatus;//!
    map< int, vector<bool> > m_CsIChannelStatus;//!
    
    
private:   // Data not writted in the tree
    int                m_NumberOfTelescope ;//!
    int                m_NumberOfStrip;//!
    int                m_NumberOfCrystal;//!
    THiraData*         m_EventData ;//!
    THiraData*         m_PreTreatedData ;//!
    THiraPhysics*      m_EventPhysics ;//!
    
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
    bool m_build_ThinSi;//!
    bool m_build_ThickSi;//!
    bool m_build_CsI;//!
    
    
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
    ClassDef(THiraPhysics,1)  // HiraPhysics structure
};

#endif
