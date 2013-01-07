#ifndef __CATSCAL__
#define __CATSCAL__
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2010                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold CATS treated data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

//   STL
#include <vector>
//   ROOT
#include "TObject.h"
#include "TVector3.h"
#include <TRandom1.h>
#include <TRandom2.h>
#include <TRandom3.h>
//   NPLib
#include "TCATSData.h"
#include "../include/VDetector.h"
#include "../include/CalibrationManager.h"
#include "../include/DetectorManager.h"

#define NBDETECTOR	2
#define	NBSTRIPS	28


using namespace std ;
enum reconstruction{NO,SECHS,GAUSS,BAR3,BAR4,BAR5};
enum correction{NOcor,cor};

class TCATSPhysics : public TObject, public NPA::VDetector
{
  
 public:   //   Constructor and Destructor
  TCATSPhysics();
  ~TCATSPhysics();

 private:   //   Root Input and Output tree classes
            
         TCATSData*         m_EventData;//!
         TCATSData*         m_PreTreatedData;//!
         TCATSPhysics*      m_EventPhysics;//!

 public :
 // marker of the cats used
  int ff ;

  //   Vector of dim = multiplicity of event on all detector
  vector<int>        DetNumberX; 
  vector<int>        StripX;
  vector<double>     ChargeX; 
     
  //   Vector of dim = number of CATS
  vector<int>      StripMaxX;
     
     
  //   Vector of dim = multiplicity of event on all detector
  vector<int>        DetNumberY; 
  vector<int>        StripY;
  vector<double>     ChargeY;
     
  //   Vector of dim = number of CATS  
  vector<int>       StripMaxY;
     
  //   Vector of dim = number of CATS
  vector<int>       DetNumberX_Position;
  vector<int>       DetNumberY_Position;
  vector<int>       DetNumberZ_Position;
  vector<double>    PositionX;
  vector<double>    PositionY;
  vector<double>    PositionZ;
	vector<double>	QsumX;
	vector<double>	QsumY;
  double            PositionOnTargetX;
  double            PositionOnTargetY;
     
  TVector3      BeamDirection      ;  //!

  double Buffer_X_Q[NBSTRIPS][NBDETECTOR];//!
  double Buffer_Y_Q[NBSTRIPS][NBDETECTOR];//!
      
  int HitX;    //!
  int HitY;    //!

  vector<reconstruction>   ReconstructionMethodX;
  vector<reconstruction>   ReconstructionMethodY;

	
		 private :
        vector< vector< vector<double> > >   StripPositionX;//!
        vector< vector< vector<double> > >   StripPositionY;//!
        vector<double>                       StripPositionZ;//!  
        int m_NumberOfCATS;
		double m_TargetAngle;
		double m_TargetThickness;
		double m_CorrectionCoef_CATS1X;//!
		double m_CorrectionCoef_CATS1Y;//!
		double m_CorrectionCoef_CATS2X;//!
		double m_CorrectionCoef_CATS2Y;//!
	
		
		string m_correction_CATS1X;//!
		string m_correction_CATS1Y;//!
		string m_correction_CATS2X;//!
		string m_correction_CATS2Y;//!
	
		string m_reconstruction_CATS1X;//!
		string m_reconstruction_CATS1Y;//!
		string m_reconstruction_CATS2X;//!
		string m_reconstruction_CATS2Y;//!
		reconstruction m_method_CATS1X;//!
		reconstruction m_method_CATS1Y;//!
		reconstruction m_method_CATS2X;//!
		reconstruction m_method_CATS2Y;//!

 private : 
       //   Map of activated channel
       map< int, vector<bool> > m_XChannelStatus;//!
       map< int, vector<bool> > m_YChannelStatus;//! 
       //   Map of inverted channel
       map< int, vector<int> > m_CATSXInversion;//!
       map< int, vector<int> > m_CATSYInversion;//! 
   
 public:   // Output data of interest
      //   for a CATS
	void SetTargetAngle(double m_TargetAngle) {m_TargetAngle = m_TargetAngle;}
	void SetTargetThickness(double m_TargetThickness) {m_TargetThickness = m_TargetThickness;}
	

      //   Remove bad channel, calibrate the data and apply threshold
      void PreTreat();

      //   Activated associated Branches and link it to the private member DetectorData address
      //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
      void InitializeRootInputRaw() ;

      //   Activated associated Branches and link it to the private member DetectorPhysics address
      //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
      void InitializeRootInputPhysics() ;
   
      //   Create associated branches and associated private member DetectorPhysics address
      void InitializeRootOutput() ;
    
      //   Clear The PreTeated object
      void ClearPreTreatedData()   {m_PreTreatedData->Clear();}
      
      void BuildPhysicalEvent();

      void BuildSimplePhysicalEvent();

      // Same as above but for online analysis
      void BuildOnlinePhysicalEvent()  {BuildSimplePhysicalEvent();};

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventData()    {m_EventData->Clear();}    

      void  Clear();
      void  Clear(const Option_t*) {};  
	
	  // Give and external TCATSData object to TCATSPhysics, needed for online analysis
	  void SetRawDataPointer(TCATSData* rawDataPointer) {m_EventData = rawDataPointer;}

      //   Return false if the channel is disabled by user
      bool IsValidChannel(const string DetectorType, const int Detector , const int channel);

      void InitializeStandardParameter();

      void AddParameterToCalibrationManager();

      void ReadAnalysisConfig();

      void ReadConfiguration(string);

      void AddCATS(TVector3 C_X1_Y1, TVector3 C_X28_Y1, TVector3 C_X1_Y28, TVector3 C_X28_Y28);

      double AnalyseX(int ff);

      double AnalyseY(int ff);

      double CalculatePositionX( double CalculatedStripX, correction method);

      double CalculatePositionY( double CalculatedStripY, correction method);

      reconstruction ChooseReconstruction(int ff, TString type);
	
	  reconstruction StringToEnum(string type);

      double CorrectedPositionX3(double Position, double a) ;
      double CorrectedPositionY3(double Position, double a) ;
      double CorrectedPositionX4(double Position, double b); 
      double CorrectedPositionY4(double Position, double b); 
	double Corrected3PointsX(double Position, double c);
	double Corrected3PointsY(double Position, double c);
	double Corrected4PointsX(double Position, double d);
	double Corrected4PointsY(double Position, double d);

	
      // Methode de reconstruction X
      double HyperbolicSequentMethodX();
      double GaussianMethodX();
      double Barycentric5MethodX(); 
      double Barycentric4MethodX(); 
      double Barycentric3MethodX(); 

      // Methode de Reconstruction Y
      double HyperbolicSequentMethodY();
      double GaussianMethodY();     
      double Barycentric5MethodY(); 
      double Barycentric4MethodY(); 
      double Barycentric3MethodY(); 

      TVector3 GetBeamDirection();
      TVector3 GetPositionOnTarget();
    
      double GetPositionOnTargetX()  {return PositionOnTargetX;}  
      double GetPositionOnTargetY()  {return PositionOnTargetY;}

       ClassDef(TCATSPhysics,1)  // CATSPhysics structure
};


namespace LOCAL_CATS
{
  //   tranform an integer to a string
  string itoa(int value);

  double fCATS_X_Q(const TCATSData* Data, const int i);
  double fCATS_Y_Q(const TCATSData* Data, const int i);
  bool fCATS_Threshold_X(const TCATSData* Data, const int i);
  bool fCATS_Threshold_Y(const TCATSData* Data, const int i);
  double fCATS_Ped_X(const TCATSData* m_EventData, const int i);
  double fCATS_Ped_Y(const TCATSData* m_EventData, const int i);
}

#endif
