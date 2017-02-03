#ifndef __SpegPhysics__
#define __SpegPhysics__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                                                                           *
 * Creation Date  : October 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the SPEG Spectrometer Physics                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 
//   STL
#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std ;

//   ROOT
#include "TObject.h"
#include "TVector2.h" 
#include "TVector3.h" 

//   NPL
#include "TSpegDCData.h"
#include "TSpegCHIOData.h"
#include "TSpegPlasticData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"

class TSpegPhysics : public TObject, public NPL::VDetector{
   public:   //   Constructor and Destructor
      TSpegPhysics();
      ~TSpegPhysics();

   public:
      void  Clear();
      void  Clear(const Option_t*) {};
   
   public:   //   Main Data
      vector <double> Q_S11;
      vector <double> Q_S12;
      vector <double> Q_S21;
      vector <double> Q_S22;
      vector <vector<double> > Charge;//!
      vector <int> Strip_S11;
      vector <int> Strip_S12;
      vector <int> Strip_S21;
      vector <int> Strip_S22;
      vector <vector <int> > Strip;//!
      vector<double> M_over_Q;
      vector<double> Z;
      vector<double> X_FocalPlan;
      vector<double> Y_FocalPlan;
      vector<double> Theta_FocalPlan;
      vector<double> Phi_FocalPlan;
      double Time_Plastic_Right;
      double Time_Plastic_Left;
      double xfoc_cor_thetafoc;//!
      double xfoc_cor_phifoc;//!
      double thetafoc_cor;//!
      double phifoc_cor;//!
      double xfoc_calibrated;//!
      double tfoc_calibrated;//!
      double phifoc_calibrated;//!

   public :
	//Variables for the first token which have parameters for trajectory calculation for Drift Chamber of SPEG
	int nax;//!
	int nbx;//! 
	int ichix;//! 
	int nfx;//! 
	int nmesx;//!
	double fax;//! 
	double zax;//! 
	double fbx;//!
	double zbx;//! 
	double fchix;//! 
	double anfocx;//! 
	double zfocx;//! 
	double ffx;//!
	double zfx;//!
	double tanfox;//!
	vector <double> Vector_zx;//!
	vector <int> Vector_nievcx;//!
	vector <double>	Vector_sigx;//!
	vector <double>	Vector_x0f;//!
	vector <double>	Vector_etalx;//!
	vector <int> Vector_iedevx;//!
	vector <double>	Vector_zdevx;//!
	vector <double>	Vector_fdevx;//!
	vector <double> Vector_sigx2;//! 

	//Variables for the second token which have parameters for trajectory calculation for Drift Chamber of SPEG
	int nay;//!
	int nby;//! 
	int ichiy;//! 
	int nfy;//! 
	int nmesy;//!
	double fay;//! 
	double zay;//! 
	double fby;//! 
	double zby;//! 
	double fchiy;//!
	double anfocy;//! 
	double zfocy;//! 
	double ffy;//! 
	double zfy;//!
	double tanfoy;//!
	vector <string> Vector_nomy;//!
	vector <double> Vector_zy;//!
	vector <int> Vector_nievcy;//!
	vector <double>	Vector_sigy;//!
	vector <double>	Vector_y0f;//!
	vector <double>	Vector_etaly;//!
	vector <int> Vector_iedevy;//!
	vector <double>	Vector_zdevy;//!
	vector <double>	Vector_fdevy;//!
	vector <double> Vector_sigy2;//!

	//Variables for the third token (Correction for the channel normalisation (COG Methode))
	double alpha[4];//!
	//Correction value
	double tema;//!
	double temb;//!
	double tcora;//!
	double tcorb;//!
	double cmsurqa;//!
	double cmsurqb;//!
	double za;//!
	double zb;//!

	//Declaration of some variables
	double seuil_haut;//! 
	double pas;//! 
	double a1_1;//! 
	double x11;//!   
	double x12;//!   
 	double x21;//!   
 	double x22;//!   
	double xPL;//! 
	double y11;//!   
	double y12;//!    
	double y21;//!   
	double y22;//!    
	double z;//! 
	double x_c;//!  
	double x3cg[4];//! 
	double xg[4];//! 
	double xsh[4];//!
	double Tab_xij[4];//!
	double Tab_yij[4];//!
	double qmax[4];//! 
	int imax[4];//!
	double SPEG_CHIO_anode;
	double phia1;//! 
	double phib1;//! 
	double sigmaphi1;//! 
	double sigmay1;//!
	double phi1;//! 
	double xa;//! 
	double xb;//! 
	double theta2;//! 
	double delta2;//! 
	double x1;//! 
	double x2;//! 
	double xt;//! 
	double yt;//! 
	double xfoc;//!
	double yfoc;//! 
	double tfoc;//! 
	double phifoc;//! 
	double Khi2;
	double bx11;//! 
	double bx12;//! 
	double bx21;//! 
	double bx22;//! 
	double det;//! 
	double ax11;//! 
	double ax12;//! 
	double ax21;//! 
	double ax22;//!
	double by11;//! 
	double by12;//! 
	double by21;//!
	double by22;//! 
	double ay11;//! 
	double ay12;//! 
	double ay21;//! 
	double ay22;//! 
	double SPEG_PLASTIC_tplg;//!  
	double SPEG_PLASTIC_tpld;//!  
	double q_c;//!
	double q_r;//!
	double q_l;//!
	int i_c;//!
	int i_l;//!
	int i_r;//!
	double temp;//!
	double cosih;//! 
	double a3;//! 
	double temp2;//! 
	double sinuh;//! 
	double temp3;//! 
	double a2;//! 
	double b1;//!
	double b2;//!
	double ax;//!
	double bx;//!
	double dx;//!
	double ay;//!
	double by;//!
	double dy;//!
	double tem;//! 
	double tcor;//!
	double cmsurq;//!
	double param_theta;//!
	double param_phi;//!
	int degree_of_calibration_angle_with_Brho;//!
	int degree_of_correction_angle_with_Brho;//!
	vector <double> cor_xfoc_thetafoc;//!
	vector <double> cor_xfoc_phifoc;//!
	vector <double> cor_theta;//!
	vector <double> cor_phi;//!
	vector <double> correction_theta_with_Brho;//!
	vector <double> correction_phi_with_Brho;//!
	vector <double> cal_theta;//!
	vector <double> cal_phi;//!
	vector <double> calibration_theta_with_Brho;//!
	vector <double> calibration_phi_with_Brho;//!
	vector <double> Vector_x;//! 
	vector <double> Vector_y;//!

   public:   //   inherrited from VDetector
      //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
      void ReadConfiguration(string Path);
      

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

      // Give and external TSpegDCData object to TSpegPhysics. Needed for online analysis for example.
      void SetRawDCDataPointer(TSpegDCData* rawDCDataPointer) {EventDCData = rawDCDataPointer;}

      // Give and external TSpegCHIOData object to TSpegPhysics. Needed for online analysis for example.
      void SetRawCHIODataPointer(TSpegCHIOData* rawCHIODataPointer) {EventCHIOData = rawCHIODataPointer;}

      // Give and external TSpegPlasticData object to TSpegPhysics. Needed for online analysis for example.
      void SetRawPlasticDataPointer(TSpegPlasticData* rawPlasticDataPointer) {EventPlasticData = rawPlasticDataPointer;}

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventDCData()    {EventDCData->Clear();}      
      void ClearEventCHIOData()    {EventCHIOData->Clear();} 
      void ClearEventPlasticData()    {EventPlasticData->Clear();} 

      // Read Config
      void ReadAnalysisConfig();

   private :
	int m_BadStrip_SPEG_DC11[128];//!
	int m_BadStrip_SPEG_DC12[128];//!
	int m_BadStrip_SPEG_DC21[128];//!
	int m_BadStrip_SPEG_DC22[128];//!
	double m_CorrectionCoef_SPEG_DC11[128];//!
	double m_CorrectionCoef_SPEG_DC12[128];//!
	double m_CorrectionCoef_SPEG_DC21[128];//!
	double m_CorrectionCoef_SPEG_DC22[128];//!
  	bool bool_CORRECTION_COEF_DC_11;//!
   	bool bool_CORRECTION_COEF_DC_12;//!
   	bool bool_CORRECTION_COEF_DC_21;//!
   	bool bool_CORRECTION_COEF_DC_22;//!

   private:   //   Root Input and Output tree classes
      TSpegDCData*         EventDCData ;//!
      TSpegCHIOData*         EventCHIOData ;//!
      TSpegPlasticData*         EventPlasticData ;//!
      TSpegPhysics*      EventPhysics ;//!

      public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TSpegPhysics,1)  // SpegPhysics structure
};

#endif
