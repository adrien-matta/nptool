#ifndef __HiraPhysics__
#define __HiraPhysics__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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
using namespace std ;

//   ROOT
#include "TObject.h"
#include "TVector3.h"


//   NPL
#include "THiraData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"


class THiraPhysics : public TObject, public NPL::VDetector
{
   public:   //   Constructor and Destructor
      THiraPhysics();
      ~THiraPhysics();

   public:
      void  Clear();
      void  Clear(const Option_t*) {};
   
   public:   //   Calibrated Data
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
    vector<int> ThickSi_StripNumberX;
    vector<int> ThickSi_StripNumberY;
    
    // CsI
    vector<double> CsI_E;
    vector<double> CsI_CrystalNumber;

   public:   //   inherrited from VDetector
    //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
    void ReadConfiguration(string);
      

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
    
    // Use to access the strip position
    double GetStripPositionX( const int N , const int X , const int Y )   const{ return m_StripPositionX[N-1][X-1][Y-1] ; }  ;
    double GetStripPositionY( const int N , const int X , const int Y )   const{ return m_StripPositionY[N-1][X-1][Y-1] ; }  ;
    double GetStripPositionZ( const int N , const int X , const int Y )   const{ return m_StripPositionZ[N-1][X-1][Y-1] ; }  ;
    
    TVector3 GetPositionOfInteraction(const int i) const;
    
    unsigned int m_ThinSi_EMult;//!
    unsigned int m_ThickSi_EXMult;//!
    unsigned int m_ThickSi_EYMult;//!
    unsigned int m_CsI_EMult;//!
    

      // Give and external THiraData object to THiraPhysics. Needed for online analysis for example.
      void SetRawDataPointer(THiraData* rawDataPointer) {m_EventData = rawDataPointer;}

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventData()    {m_EventData->Clear();}

   private:   // Data not writted in the tree
    int                m_NumberOfTelescope ;//!
    THiraData*         m_EventData ;//!
    THiraData*         m_PreTreatedData ;//!
    THiraPhysics*      m_EventPhysics ;//!
    
    vector< vector < vector < double > > >   m_StripPositionX;//!
    vector< vector < vector < double > > >   m_StripPositionY;//!
    vector< vector < vector < double > > >   m_StripPositionZ;//!

    public:// Static constructor to be passed to the Detector Factory
    static NPL::VDetector* Construct();
      ClassDef(THiraPhysics,1)  // HiraPhysics structure
};

#endif
