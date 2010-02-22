#ifndef __CATSCAL__
#define __CATSCAL__

#include <vector>

#include "TObject.h"
#include "TVector3.h"
#include "TCATSData.h"

enum reconstruction{NO,SECHS,GAUSS,BAR3,BAR4,BAR5};
//enum correction{BAR3cor,BAR4cor,NOcor,GAUSScor};
enum correction{NOcor,cor};

class TCATSPhysics : public TObject {
 public:
  //for a CATS
  //	Vector of dim = multiplicity of event on all detector
  vector<int>  		DetNumberX				; 
  vector<int>  		StripX					;
  vector<double>	ChargeX					; 
  
  //	Vector of dim = number of CATS
  vector<double>	ChargeSumX				;
  vector<int>		MultOverThreshX  		;  
  vector<int>		StripMaxX 				;
  
  
  //	Vector of dim = multiplicity of event on all detector
  vector<int>  		DetNumberY				; 
  vector<int>  		StripY					;
  vector<double>	ChargeY					;
  //  vector<double>	ChargeY_test				;
  
  //	Vector of dim = number of CATS
  vector<double>	ChargeSumY				;  
  vector<int> 		MultOverThreshY		        	;
  vector<int> 		StripMaxY 				;
  //  vector<int> 		StripMaxY_test 				;
  
  //	Calculate
  vector<int>           DetNumberX_Position                     ;
  vector<int>           DetNumberY_Position                     ;
  vector<int>           DetNumberZ_Position                     ;
  vector<double>        PositionX 				;
  vector<double>	PositionY 				;
  vector<double>	PositionZ 				;
  double 		PositionOnTargetX		;
  double 		PositionOnTargetY		;
  TVector3		BeamDirection			;

  double                Chargex[28];  //!
  double                Chargey[28];  //!
  //  double                Chargey_test[28];  //!
  int HitX;    //!
  int HitY;    //!

  vector<reconstruction>       ReconstructionMethodX;
  vector<reconstruction>       ReconstructionMethodY;
 
 public:
  TCATSPhysics();
  virtual ~TCATSPhysics();

  void	Clear();
  void	Dump();
  
  void BuildSimplePhysicalEvent(  TCATSData* 	                         Data			, 
				  vector< vector <double> > 	         &Ped_X 	        ,
				  vector< vector <double> > 	         &Ped_Y 	        ,
				  vector< vector< vector<double> > >    &OnlineCalib_X_E	,
				  vector< vector< vector<double> > >	 &OnlineCalib_Y_E 	,	
				  vector< vector <double> > 		 &Thresh_X              ,
				  vector< vector <double> > 		 &Thresh_Y 	        ,
				  vector< vector< vector<double> > >    &StripPositionX  	,
				  vector< vector< vector<double> > >    &StripPositionY   	,
				  vector<double>  			 &StripPositionZ   	     ) ;

  
  double AnalyseX(TCATSData* Data, 
		  vector< vector <double> > 	        &Ped_X 		,
		  vector< vector< vector<double> > >    &OnlineCalib_X_E,
		  vector< vector <double> > 		&Thresh_X,
		  vector< vector< vector<double> > >    &StripPositionX,
		  int ff,
		  int NumberOfDetector);

  double AnalyseY(TCATSData* Data, 
		  vector< vector <double> >              	&Ped_Y 		,
		  vector< vector< vector<double> > >    	&OnlineCalib_Y_E,
		  vector< vector <double> > 			&Thresh_Y,
		  vector< vector< vector<double> > >            &StripPositionY,
		  int ff,
		  int NumberOfDetector);

  double CalculatePositionX( vector< vector< vector<double> > >  &StripPositionX,
			     int          StripMaxX,
			     double*      Chargex,
			     double       CalculatedStripX, 
			     int          ff, 
			     correction   method);

  double CalculatePositionY( vector< vector< vector<double> > >  &StripPositionY,
			     int        StripMaxY,
			     double*    Chargey,
			     double     CalculatedStripY, 
			     int        ff, 
			     correction method);


  reconstruction ChooseReconstruction(int ff, int type, double * charge, int StripMax);

  //	Calculate Strip touch using an array of Charge on Strip and Strip with Maximum Charge
  
  double HyperbolicSequentMethod(double* Charge , int StripMax) ;
  double GaussianMethodX(int ff, double* Chargex , int StripMax, vector< vector< vector<double> > >  &StripPositionX);
  double GaussianMethodY(int ff, double* Chargey , int StripMax, vector< vector< vector<double> > >  &StripPositionY);
  double Barycentric5Method( double* Charge , int StripMax);
  double Barycentric4Method( double* Charge , int StripMax);
  double Barycentric3Method( double* Charge , int StripMax);
  
  double CorrectedPositionX3(int ff, double Position, int StripMax, double a, vector< vector< vector<double> > >  &StripPositionX);
  double CorrectedPositionY3(int ff, double Position, int StripMax, double a, vector< vector< vector<double> > >  &StripPositionY);
  double CorrectedPositionX4(int ff, double* Charge, double Position, int StripMax, double b, vector< vector< vector<double> > >  &StripPositionX);
  double CorrectedPositionY4(int ff, double* Charge, double Position, int StripMax, double b, vector< vector< vector<double> > >  &StripPositionY);
 
  // X

  //	Vector of dim = multiplicity of event on all detector
  int	    GetCATSDetNumberX(int i)       {return DetNumberX.at(i);}
  int	    GetCATSStripX(int i)           {return StripX.at(i);}
  double    GetCATSChargeX(int i)          {return ChargeX.at(i);}
  
  int	    GetCATSMultX()	           {return DetNumberX.size();}
 
  //	Vector of dim = number of CATS
  double    GetCATSChargeSumX(int i)       {return ChargeSumX.at(i);}
  int 	    GetCATSMultOverThreshX(int i)  {return MultOverThreshX.at(i);}
  int       GetCATSStripMaxX(int i)        {return StripMaxX.at(i); }
  int       GetCATSDetNumberX_Position(int i)  {return DetNumberX_Position.at(i);}
  double    GetCATSPositionX(int i)        {return PositionX.at(i);}

  double    GetPositionOnTargetX()         {return PositionOnTargetX;}

  // Y

  //	Vector of dim = multiplicity of event on all detector
  int	    GetCATSDetNumberY(int i)       {return DetNumberY.at(i);}
  int	    GetCATSStripY(int i)           {return StripY.at(i);}
  double    GetCATSChargeY(int i)          {return ChargeY.at(i);}

  int	    GetCATSMultY()	           {return DetNumberY.size();}
 
  //	Vector of dim = number of CATS
  double    GetCATSChargeSumY(int i)       {return ChargeSumY.at(i);}
  int 	    GetCATSMultOverThreshY(int i)  {return MultOverThreshY.at(i);}
  int       GetCATSStripMaxY(int i)        {return StripMaxY.at(i); }
  // int       GetCATSStripMaxY_test(int i)   {return StripMaxY_test.at(i); }
  int       GetCATSDetNumberY_Position(int i)  {return DetNumberY_Position.at(i);}
  double    GetCATSPositionY(int i)        {return PositionY.at(i);}
  
  double    GetPositionOnTargetY()         {return PositionOnTargetY;}

  int       GetCATSMult()                  {return PositionX.size();}

  ClassDef(TCATSPhysics,1)  // CATSPhysics structure
    };


#endif
