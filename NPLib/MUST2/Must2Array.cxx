#include "Must2Array.h"

//	STL
#include <cmath>
#include <iostream>	
#include <fstream>
#include <string>
#include <stdlib.h>

//	NPL
#include "RootInput.h"
#include "RootOutput.h"

//	Root
#include "TChain.h"

using namespace std ;	
	
//	Default Constructor

MUST2Array::MUST2Array()
	{
		NumberOfTelescope = 0				;
		EventData = new TMust2Data 			;
		EventPhysics = new TMust2Physics 	;
	}	

MUST2Array::~MUST2Array()
	{
		NumberOfTelescope = 0				;
		delete EventData 			;
		delete EventPhysics 	;
	}	
	
				
////	Innherited from VDetector Class	////				
				
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void MUST2Array::ReadConfiguration(string Path) 	
{
   ifstream ConfigFile           	;
   ConfigFile.open(Path.c_str()) 	;
   string LineBuffer          		;
   string DataBuffer          		;	

   // A:X1_Y1     --> X:1    Y:1
   // B:X128_Y1   --> X:128  Y:1
   // C:X1_Y128   --> X:1    Y:128
   // D:X128_Y128 --> X:128  Y:128

   double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz           	;
   TVector3 A , B , C , D                                          				;
   double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0    ;
   int SI = 0 , SILI = 0 , CSI = 0                                         		;	

   bool check_A = false 	;
   bool check_C = false  	;
   bool check_B = false 	;
   bool check_D = false  	;

   bool check_Theta = false ;
   bool check_Phi  	= false ;
   bool check_R     = false ;
   bool check_beta 	= false ;
   
   bool ReadingStatus = false ;
	

   while (!ConfigFile.eof()) 
   	{
      
      	getline(ConfigFile, LineBuffer);

		//	If line is a Start Up Must2 bloc, Reading toggle to true      
      	if (LineBuffer.compare(0, 11, "M2Telescope") == 0) 
	      	{
	        	 cout << "///" << endl           		;
	       		  cout << "Telescope found: " << endl   ;        
	        	 ReadingStatus = true 					;
	        	
		   	}
		
		//	Else don't toggle to Reading Block Status
		else ReadingStatus = false ;
		
		//	Reading Block
		while(ReadingStatus)
			{
				 
				ConfigFile >> DataBuffer ;
				//	Comment Line 
					if(DataBuffer.compare(0, 1, "%") == 0) {
						 	ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
							
						}
			
					//	Finding another telescope (safety), toggle out
				else if (DataBuffer.compare(0, 11, "M2Telescope") == 0) {
						cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
						ReadingStatus = false ;
						}
			
					//	Position method
					
		         else if (DataBuffer.compare(0, 6, "X1_Y1=") == 0) {
		            check_A = true;
		            ConfigFile >> DataBuffer ;
		            Ax = atof(DataBuffer.c_str()) ;
		            Ax = Ax  ;
		            ConfigFile >> DataBuffer ;
		            Ay = atof(DataBuffer.c_str()) ;
		            Ay = Ay  ;
		            ConfigFile >> DataBuffer ;
		            Az = atof(DataBuffer.c_str()) ;
		            Az = Az  ;

		            A = TVector3(Ax, Ay, Az);
		            cout << "X1 Y1 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
		            
		         }


		         else if (DataBuffer.compare(0, 8, "X128_Y1=") == 0) {
		            check_B = true;
		            ConfigFile >> DataBuffer ;
		            Bx = atof(DataBuffer.c_str()) ;
		            Bx = Bx  ;
		            ConfigFile >> DataBuffer ;
		            By = atof(DataBuffer.c_str()) ;
		            By = By  ;
		            ConfigFile >> DataBuffer ;
		            Bz = atof(DataBuffer.c_str()) ;
		            Bz = Bz  ;

		            B = TVector3(Bx, By, Bz);
		            cout << "X128 Y1 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
		            
		         }
		         

		         else if (DataBuffer.compare(0, 8, "X1_Y128=") == 0) {
		            check_C = true;
		            ConfigFile >> DataBuffer ;
		            Cx = atof(DataBuffer.c_str()) ;
		            Cx = Cx  ;
		            ConfigFile >> DataBuffer ;
		            Cy = atof(DataBuffer.c_str()) ;
		            Cy = Cy  ;
		            ConfigFile >> DataBuffer ;
		            Cz = atof(DataBuffer.c_str()) ;
		            Cz = Cz  ;

		            C = TVector3(Cx, Cy, Cz);
		            cout << "X1 Y128 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
		           
		         }

		         else if (DataBuffer.compare(0, 10, "X128_Y128=") == 0) {
		            check_D = true;
		            ConfigFile >> DataBuffer ;
		            Dx = atof(DataBuffer.c_str()) ;
		            Dx = Dx  ;
		            ConfigFile >> DataBuffer ;
		            Dy = atof(DataBuffer.c_str()) ;
		            Dy = Dy  ;
		            ConfigFile >> DataBuffer ;
		            Dz = atof(DataBuffer.c_str()) ;
		            Dz = Dz  ;

		            D = TVector3(Dx, Dy, Dz);
		            cout << "X128 Y128 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;
		           
		         }
			
				//	End Position Method

		         //	Angle method
		         else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
		            check_Theta = true;
		            ConfigFile >> DataBuffer ;
		            Theta = atof(DataBuffer.c_str()) ;
		            Theta = Theta ;
		            cout << "Theta:  " << Theta << endl;
		            
		         }

		         //Angle method
		         else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
		            check_Phi = true;
		            ConfigFile >> DataBuffer ;
		            Phi = atof(DataBuffer.c_str()) ;
		            Phi = Phi ;
		            cout << "Phi:  " << Phi << endl;
		          
		         }

		         //Angle method
		         else if (DataBuffer.compare(0, 2, "R=") == 0) {
		            check_R = true;
		            ConfigFile >> DataBuffer ;
		            R = atof(DataBuffer.c_str()) ;
		            R = R ;
		            cout << "R:  " << R << endl;
		          
		         }

		         //Angle method
		         else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
		            check_beta = true;
		            ConfigFile >> DataBuffer ;
		            beta_u = atof(DataBuffer.c_str()) ;
		            beta_u = beta_u    ;
		            ConfigFile >> DataBuffer ;
		            beta_v = atof(DataBuffer.c_str()) ;
		            beta_v = beta_v    ;
		            ConfigFile >> DataBuffer ;
		            beta_w = atof(DataBuffer.c_str()) ;
		            beta_w = beta_w    ;
		            cout << "Beta:  " << beta_u << " " << beta_v << " " << beta_w << endl  ;
		            
		         }
		              
		         /////////////////////////////////////////////////
		         //	If All necessary information there, toggle out
		         if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta)  ) 
		         	{ 
		         	ReadingStatus = false; 
		         	
		         	///Add The previously define telescope
         			//With position method
		         	if ( check_A && check_B && check_C && check_D ) 
		         		{
				            AddTelescope(	A   ,
				                  			B   ,
				                 			C   ,
				                  			D   ) ;
				         }
		         	
		         	    //with angle method
       				  else if ( check_Theta && check_Phi && check_R && check_beta ) 
       				  	{
				            AddTelescope(	Theta   ,
				                  			Phi   	,
				                  			R       ,
				                  			beta_u  ,
				                  			beta_v  ,
				                  			beta_w  );
      					}
								
			        	check_A = false 	;
						check_B = false 	;
		  				check_C = false  	;
		  				check_D = false  	;

		   				check_Theta = false   	;
		   				check_Phi  = false  	;
		   				check_R    = false   	;
		   				check_beta = false  	;
					
		         	}
		         
		}
		        
		        
		    
		         	
	}
	
	cout << endl << "/////////////////////////////" << endl<<endl;

}


//	Read stream at Path and pick-up calibration parameter using Token
//	If argument is "Simulation" no change calibration is loaded
void MUST2Array::ReadCalibrationFile(string Path)	
	{
		
		//	Order of Polynom function used for calibration
		int Calibration_Si_E_Order 		;
		int Calibration_Si_T_Order 		;
		int Calibration_SiLi_E_Order 	;
		int Calibration_CsI_E_Order 	;
		
		//	Calibration_Si_X_E[DetectorNumber][StripNumber][Order of Coeff]
		vector< vector< vector< double > > >	Calibration_Si_X_E	;
		vector< vector< vector< double > > >	Calibration_Si_X_T	;
		
		vector< vector< vector< double > > >	Calibration_Si_Y_E	;
		vector< vector< vector< double > > >	Calibration_Si_Y_T	;
	
		//	Calibration_SiLi_E[DetectorNumber][PadNumber][Order of Coeff]
		vector< vector< vector< double > > >	Calibration_SiLi_E	;
		
		//	Calibration_SiLi_E[DetectorNumber][CrystalNumber][Order of Coeff]
		vector< vector< vector< double > > >	Calibration_CsI_E	;
	
		if(Path == "Simulation")	//	Simulation case: data already calibrated
			{
				Calibration_Si_E_Order		=	1 		;
				Calibration_Si_T_Order 		=	1		;
				Calibration_SiLi_E_Order 	=	1		;
				Calibration_CsI_E_Order 	=	1		;
				
				vector<double> Coef;
				//	Order 0				Order 1
				Coef.push_back(0) ; Coef.push_back(1) 	;
				
				vector< vector<double> > StripLine 		;
				StripLine.resize( 128 , Coef)			;
				
				Calibration_Si_X_E.resize( NumberOfTelescope , StripLine)	;
				Calibration_Si_X_T.resize( NumberOfTelescope , StripLine)	;
				
				Calibration_Si_Y_E.resize( NumberOfTelescope , StripLine)	;
				Calibration_Si_Y_T.resize( NumberOfTelescope , StripLine)	;
				
				Calibration_SiLi_E.resize( NumberOfTelescope , StripLine)	;
				Calibration_CsI_E .resize( NumberOfTelescope , StripLine)	;
			}
			
		else
			{
				
			
			}
	}		
	

//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void MUST2Array::InitializeRootInput() 		
	{
		TChain* inputChain = RootInput::getInstance()->GetChain()	;
		inputChain->SetBranchStatus( "MUST2" , true )				;
		inputChain->SetBranchStatus( "fMM_*" , true )				;
		inputChain->SetBranchAddress( "MUST2" , &EventData )		;
	}


//	Create associated branches and associated private member DetectorPhysics address
void MUST2Array::InitializeRootOutput() 	
	{
		TTree* outputTree = RootOutput::getInstance()->GetTree()		;
		outputTree->Branch( "MUST2" , "TMust2Physics" , &EventPhysics )	;
	}


//	This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
void MUST2Array::BuildPhysicalEvent()		
	{
		EventPhysics -> BuildPhysicalEvent(EventData)	;
	}


//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
//	NB: This method can eventually be the same as BuildPhysicalEvent.
void MUST2Array::BuildSimplePhysicalEvent()	
	{
		EventPhysics -> BuildSimplePhysicalEvent(EventData)	;
	}



/////	Specific to MUST2Array	////

void MUST2Array::AddTelescope(	TVector3 C_X1_Y1 		,
			 					TVector3 C_X128_Y1 		, 
			 					TVector3 C_X1_Y128 		, 
			 					TVector3 C_X128_Y128	)
	{
		NumberOfTelescope++;
	
		//	Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
		TVector3 U = C_X1_Y1 - C_X128_Y1 				;	
		U = U.Unit()									;
		//	Vector V on Telescope Face (parallele to X Strip)
		TVector3 V = C_X128_Y128 - C_X128_Y1 				;
		V = V.Unit()									;

		//	Position Vector of Strip Center
		TVector3 StripCenter = TVector3(0,0,0)			;
		//	Position Vector of X=1 Y=1 Strip 
		TVector3 Strip_1_1 								;		

		//	Geometry Parameter
		double Face = 98 					  	; //mm
		double NumberOfStrip = 128 				; 
		double StripPitch = Face/NumberOfStrip	; //mm

		//	Buffer object to fill Position Array
		vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

		vector< vector< double > >	OneTelescopeStripPositionX	;
		vector< vector< double > >	OneTelescopeStripPositionY	;
		vector< vector< double > >	OneTelescopeStripPositionZ	;
		
		//	Moving StripCenter to 1.1 corner:
		Strip_1_1 = C_X128_Y1 + (U+V) * (StripPitch/2.) 	;

		for( int i = 0 ; i < 128 ; i++ )
			{
				lineX.clear()	;
				lineY.clear()	;
				lineZ.clear()	;
				
				for( int j = 0 ; j < 128 ; j++ )
					{
						StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  )	;
						//StripCenter += -TargetPosition		;
						
						lineX.push_back( StripCenter.X() )	;
						lineY.push_back( StripCenter.Y() )	;
						lineZ.push_back( StripCenter.Z() )	;	
					}
					
				OneTelescopeStripPositionX.push_back(lineX)	;
				OneTelescopeStripPositionY.push_back(lineY)	;
				OneTelescopeStripPositionZ.push_back(lineZ)	;
			 	
			}
	
		
		StripPositionX.push_back( OneTelescopeStripPositionX ) ;
		StripPositionY.push_back( OneTelescopeStripPositionY ) ;
		StripPositionZ.push_back( OneTelescopeStripPositionZ ) ;	

	}
				
				
void MUST2Array::AddTelescope(	double theta 	, 
								double phi 		, 
								double distance , 
								double beta_u 	, 
								double beta_v 	, 
								double beta_w	)
	{
	
		NumberOfTelescope++;
	
		double Pi = 3.141592654 ;

		// convert from degree to radian:
		theta = theta * Pi/180. ; 
		phi   = phi   * Pi/180. ;

		//Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
		TVector3 U ;	
		//Vector V on Telescope Face (parallele to X Strip)
		TVector3 V ;
		//Vector W normal to Telescope Face (pointing CsI)
		TVector3 W ;
		//Vector position of Telescope Face center
		TVector3 C ;
			
		C = TVector3 (	distance * sin(theta) * cos(phi) ,
						distance * sin(theta) * sin(phi) ,
						distance * cos(theta)			 );
		
    	TVector3 P = TVector3(	cos(theta ) * cos(phi)	, 
    							cos(theta ) * sin(phi)	,
    							-sin(theta)				);
		
		W = C.Unit() ;
		U = W .Cross ( P ) ;
	    V = W .Cross ( U );
		
		U = U.Unit();
		V = V.Unit();
		
		U.Rotate( beta_u * Pi/180. , U ) ;
		V.Rotate( beta_u * Pi/180. , U ) ;
		
		U.Rotate( beta_v * Pi/180. , V ) ;
		V.Rotate( beta_v * Pi/180. , V ) ;
		
		U.Rotate( beta_w * Pi/180. , W ) ;
		V.Rotate( beta_w * Pi/180. , W ) ;
		
		double Face = 98 					  	; //mm
		double NumberOfStrip = 128 				;
		double StripPitch = Face/NumberOfStrip	; //mm

		vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;
		
		vector< vector< double > >	OneTelescopeStripPositionX	;
		vector< vector< double > >	OneTelescopeStripPositionY	;
		vector< vector< double > >	OneTelescopeStripPositionZ	;
		
		double X , Y , Z  ;

		//Moving C to the 1.1 corner:
		C.SetX( C.X() - ( Face/2 - StripPitch/2 ) * ( V.X() + U.X() ) )  ; 
		C.SetY( C.Y() - ( Face/2 - StripPitch/2 ) * ( V.Y() + U.Y() ) )  ; 
		C.SetZ( C.Z() - ( Face/2 - StripPitch/2 ) * ( V.Z() + U.Z() ) )  ; 
	
		for( int i = 0 ; i < 128 ; i++ )
			{
				
				lineX.clear()	;
				lineY.clear()	;
				lineZ.clear()	;
				
				for( int j = 0 ; j < 128 ; j++ )
					{
						X = C.X() + StripPitch * ( U.X()*i + V.X()*j )	;
						Y = C.Y() + StripPitch * ( U.Y()*i + V.Y()*j )	;
						Z = C.Z() + StripPitch * ( U.Z()*i + V.Z()*j )	;
									
						lineX.push_back(X)	;
						lineY.push_back(Y)	;
						lineZ.push_back(Z)	;		
						
					}
				
				OneTelescopeStripPositionX.push_back(lineX)	;
				OneTelescopeStripPositionY.push_back(lineY)	;
				OneTelescopeStripPositionZ.push_back(lineZ)	;
			 	
			}
		StripPositionX.push_back( OneTelescopeStripPositionX ) ;
		StripPositionY.push_back( OneTelescopeStripPositionY ) ;
		StripPositionZ.push_back( OneTelescopeStripPositionZ ) ;
	}
	
double MUST2Array::GetEnergyDeposit()		
	{ 
		if(EventPhysics->TotalEnergy.size()>0)
		return EventPhysics->TotalEnergy[0] ; 
		else return -1000 ;
	}	
	
	
	
TVector3 MUST2Array::GetPositionOfInteraction()
	{
		TVector3 Position = TVector3(-1000,-1000,-1000);
		
		if(EventPhysics->TelescopeNumber.size()==1)
		{
				Position = TVector3 (	GetStripPositionX( EventPhysics->TelescopeNumber[0] , EventPhysics->Si_X[0] , EventPhysics->Si_Y[0] ) 	,
										GetStripPositionY( EventPhysics->TelescopeNumber[0] , EventPhysics->Si_X[0] , EventPhysics->Si_Y[0] )	,
										GetStripPositionZ( EventPhysics->TelescopeNumber[0] , EventPhysics->Si_X[0] , EventPhysics->Si_Y[0] )	) ;
		}
		
		return(Position) ;	
	
	}
	
TVector3 MUST2Array::GetTelescopeNormal()
	{
		TVector3 Normal = TVector3(-1000,-1000,-1000);
		
		if(EventPhysics->TelescopeNumber.size()==1)
		{
				TVector3 U = TVector3 (		GetStripPositionX( EventPhysics->TelescopeNumber[0] , 128 , 1 ) 	,
											GetStripPositionY( EventPhysics->TelescopeNumber[0] , 128 , 1 )		,
											GetStripPositionZ( EventPhysics->TelescopeNumber[0] , 128 , 1 )		)
											
							- TVector3 (	GetStripPositionX( EventPhysics->TelescopeNumber[0] , 1 , 1 ) 		,
											GetStripPositionY( EventPhysics->TelescopeNumber[0] , 1 , 1 )		,
											GetStripPositionZ( EventPhysics->TelescopeNumber[0] , 1 , 1 )		);
										
				TVector3 V = TVector3 (		GetStripPositionX( EventPhysics->TelescopeNumber[0] , 128 , 128 ) 	,
											GetStripPositionY( EventPhysics->TelescopeNumber[0] , 128 , 128 )	,
											GetStripPositionZ( EventPhysics->TelescopeNumber[0] , 128 , 128 )	)
											
							- TVector3 (	GetStripPositionX( EventPhysics->TelescopeNumber[0] , 128 , 1 ) 	,
											GetStripPositionY( EventPhysics->TelescopeNumber[0] , 128 , 1 )		,
											GetStripPositionZ( EventPhysics->TelescopeNumber[0] , 128 , 1 )		);
											
				Normal = U.Cross(V);
		}
		
		return(Normal.Unit()) ;	
	
	}	
	
	
void MUST2Array::Print()
	{
		cout << "Number of telescope : " << NumberOfTelescope << endl ;
		for( int f = 0 ; f < NumberOfTelescope ; f++)
			{
				cout << "Telescope " << f+1 << endl ;
			
				for( int i = 0 ; i < 128 ; i++ )
					{
			
						for( int j = 0 ; j < 128 ; j++ )
							{
							cout << i+1 << "  "<< j+1 << "  " 
							<< StripPositionX[f][i][j]   << "  " 
							<< StripPositionY[f][i][j]   << "  "
							<< StripPositionZ[f][i][j]   << "  " 
							<< endl ;
							}	
					}

			}
	}
									

