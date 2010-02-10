/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold must2 treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  Only multiplicity one and multiplicity 2 are down.                       *
 *  Improvment needed                                                        *
 *                                                                           *
 *****************************************************************************/
#include "TMust2Physics.h"
using namespace LOCAL;
	
//	STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>

//	NPL
#include "RootInput.h"
#include "RootOutput.h"

//	ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TMust2Physics)
///////////////////////////////////////////////////////////////////////////
TMust2Physics::TMust2Physics() 
	{
		EventMultiplicity 	= 0 							;
		EventData 					= new TMust2Data	;
		EventPhysics 				= this						;
		NumberOfTelescope		= 0								;
	}
		
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::BuildSimplePhysicalEvent()
	{ 
		BuildPhysicalEvent(); 
	}
	
///////////////////////////////////////////////////////////////////////////
	
void TMust2Physics::BuildPhysicalEvent()
	{ 
		bool check_SILI = false ;
		bool check_CSI  = false ;
	
	
		if( CheckEvent() == 1 )
			{
				vector< TVector2 > couple = Match_X_Y() ;
				
				for(unsigned int i = 0 ; i < couple.size() ; i++)
					{
						check_SILI = false ;
						check_CSI = false ;
					
						int N = EventData->GetMMStripXEDetectorNbr(couple[i].X())		;
						
						int X = EventData->GetMMStripXEStripNbr(couple[i].X())			;
						int Y = EventData->GetMMStripYEStripNbr(couple[i].Y())			;
						
						double Si_X_E = fSi_X_E(EventData , couple[i].X())	;
						double Si_Y_E = fSi_Y_E(EventData , couple[i].Y())	;

						double Si_X_T = fSi_X_T(EventData , couple[i].X())		;
						double Si_Y_T = fSi_Y_T(EventData , couple[i].Y())		;
					
						Si_X.push_back(X) ; Si_Y.push_back(Y) ; TelescopeNumber.push_back(N) ;
						
						// Take maximum Energy
						if(Si_X_E >= Si_Y_E) Si_E.push_back(Si_X_E)	;
						else								 Si_E.push_back(Si_Y_E)	;
						
						// Take minimum Time
						if(Si_X_T >= Si_Y_T) Si_T.push_back(Si_Y_T)	;
						else								 Si_T.push_back(Si_X_T)	;
						
						for(unsigned int j = 0 ; j < EventData->GetMMSiLiEMult() ; j++)
							{
								if(EventData->GetMMSiLiEDetectorNbr(j)==N)
									{
										//	if SiLi energy is above threshold check the compatibility
										if( fSiLi_E(EventData , j)>SiLi_E_Threshold )
											{
												if( Match_Si_SiLi( X, Y , EventData->GetMMSiLiEPadNbr(j) ) )
												{
													SiLi_N.push_back(EventData->GetMMSiLiEPadNbr(j))	;
													SiLi_E.push_back(fSiLi_E(EventData , j))					;
													SiLi_T.push_back(fSiLi_T(EventData , j))		;
													check_SILI = true ;
													
												}
											}
									}
							}
							
						 for( int j = 0 ; j < EventData->GetMMCsIEMult() ; j++)
							{
								if(EventData->GetMMCsIEDetectorNbr(j)==N)
									{
										//	ifCsI energy is above threshold check the compatibility
										if( fCsI_T(EventData , j)>CsI_E_Threshold )
											{
												if( Match_Si_CsI( X, Y , EventData->GetMMCsIECristalNbr(j) ) )
													{
														CsI_N.push_back(EventData->GetMMCsIECristalNbr(j))	;
														CsI_E.push_back(fCsI_E(EventData , j))			;
														CsI_T.push_back(fCsI_T(EventData , j))				;
														check_CSI = true ;
													}
											}
									}
							}
					
					
						if(!check_SILI)
							{
								SiLi_N.push_back(0)	;
								SiLi_E.push_back(0)	;
								SiLi_T.push_back(0)	;
							}

						if(!check_CSI) 
							{
								CsI_N.push_back(0)	;
								CsI_E.push_back(0)	;
								CsI_T.push_back(0)	;
							}
					
					}
			}
		
		return;
	
	}	

///////////////////////////////////////////////////////////////////////////
int TMust2Physics :: CheckEvent()
	{
		// Check the size of the different elements
				 if(			EventData->GetMMStripXEMult() == EventData->GetMMStripYEMult() && EventData->GetMMStripYEMult() == EventData->GetMMStripXTMult() &&  EventData->GetMMStripXTMult() == EventData->GetMMStripYTMult()  )
	
					return 1 ; // Regular Event
	
		else if(			EventData->GetMMStripXEMult() == EventData->GetMMStripYEMult()+1 || EventData->GetMMStripXEMult() == EventData->GetMMStripYEMult()-1  )
	
					return 2 ; // Pseudo Event, potentially interstrip
		
		else 	return -1 ; // Rejected Event

	}

///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: ResolvePseudoEvent()
	{
		return false;
	}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TMust2Physics :: Match_X_Y()
	{
		vector < TVector2 > ArrayOfGoodCouple ;
		
		// Prevent code from treating very high multiplicity Event
		// Those event are not physical anyway and that improve speed.
		if( EventData->GetMMStripXEMult()>6 || EventData->GetMMStripYEMult()>6 )
			return ArrayOfGoodCouple;
		
		for(int i = 0 ; i < EventData->GetMMStripXEMult(); i++)
			{
				//	if X value is above threshold, look at Y value
				if( fSi_X_E(EventData , i) > Si_X_E_Threshold )
					{
					
						for(int j = 0 ; j < EventData->GetMMStripYEMult(); j++)
							{
								//	if Y value is above threshold look if detector match
								if( fSi_Y_E(EventData , j) > Si_Y_E_Threshold )							
									{
										//	if same detector check energy
										if ( EventData->GetMMStripXEDetectorNbr(i) == EventData->GetMMStripYEDetectorNbr(j) )
											{
													//	Look if energy match
													if( ( fSi_X_E(EventData , i) - fSi_Y_E(EventData , j) ) / fSi_X_E(EventData , i) < 0.1	)
														ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;	
											}
									}
							}
					}
			}
	
		if( ArrayOfGoodCouple.size() > EventData->GetMMStripXEMult() ) ArrayOfGoodCouple.clear() ;
		
		return ArrayOfGoodCouple;	
	}
	
	
///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: Match_Si_SiLi(int X, int Y , int PadNbr)
	{
							if( 	 PadNbr == 1 
									&& X<=121 && X>=93
									&& Y<=128 && Y>=95 ) 

						return true ;


				else	if( 	 PadNbr == 2 
									&& X<=121 && X>=93 
									&& Y<=100 && Y>=65 ) 

						return true ;


				else	if( 	 PadNbr == 3 
									&& X<=96 && X>=61 
									&& Y<=128 && Y>=95 ) 

						return true ;

				else	if( 	 PadNbr == 4 
									&& X<=96 && X>=61
									&& Y<=100 && Y>=65 ) 

						return true ;

				else	if( 	 PadNbr == 5 
									&& X<=67 && X>=30 
									&& Y<=100 && Y>=65) 

						return true ;

				else	if( 	 PadNbr == 6 
									&& X<=67 && X>=30 
									&& Y<=128 && Y>=95 ) 

						return true ;

				else	if( 	 PadNbr == 7 
									&& X<=35 && X>=6 
									&& Y<=100 && Y>=65 ) 

						return true ;

				else	if( 	 PadNbr == 8 
									&& X<=35 && X>=6 
									&& Y<=128 && Y>=95 ) 

						return true ;

				else	if( 	 PadNbr == 9 
									&& X<=121 && X>=93 
									&& Y<=31 && Y>=1 ) 

						return true ;

				else	if( 	 PadNbr == 10 
									&& X<=121 && X>=93 
									&& Y<=60 && Y>=26 ) 

						return true ;

				else	if( 	 PadNbr == 11 
									&& X<=96 && X>=61
									&& Y<=31 && Y>=1 ) 

						return true ;

				else	if( 	 PadNbr == 12 
									&& X<=96 && X>=61
									&& Y<=60 && Y>=26) 

						return true ;

				else	if( 	 PadNbr == 13 
									&& X<=67 && X>=30 
									&& Y<=60 && Y>=26 ) 

						return true ;

				else	if( 	 PadNbr == 14 
									&& X<=67 && X>=30 
									&& Y<=31 && Y>=1 ) 

						return true ;

				else	if( 	 PadNbr == 15 
									&& X<=35 && X>=6
									&& Y<=60 && Y>=26 ) 

						return true ;

				else	if( 	 PadNbr == 16 
									&& X<=35 && X>=6
									&& Y<=31 && Y>=1 ) 

						return true ;		

				else
						return false;
	}


///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: Match_Si_CsI(int X, int Y , int CristalNbr)
	{
							if( 	 CristalNbr == 1 
									&& X<=71 && X>=27 
									&& Y<=101 && Y>=59 ) 

						return true ;


				else	if( 	 CristalNbr == 2 
									&& X<=71 && X>=27 
									&& Y<=128 && Y>=90 ) 

						return true ;


				else	if( 	 CristalNbr == 3 
									&& X<=35 && X>=1 
									&& Y<=101 && Y>=59 ) 

						return true ;

				else	if( 	 CristalNbr == 4 
									&& X<=35 && X>=1 
									&& Y<=128 && Y>=90 ) 

						return true ;

				else	if( 	 CristalNbr == 5 
									&& X<=104 && X>=60 
									&& Y<=71 && Y>=30 ) 

						return true ;

				else	if( 	 CristalNbr == 6 
									&& X<=104 && X>=60 
									&& Y<=41 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 7 
									&& X<=128 && X>=90 
									&& Y<=71 && Y>=30 )

						return true ;

				else	if( 	 CristalNbr == 8 
									&& X<=128 && X>=90 
									&& Y<=41 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 9 
									&& X<=71 && X>=27 
									&& Y<=71 && Y>=40 ) 

						return true ;

				else	if( 	 CristalNbr == 10 
									&& X<=71 && X>=27 
									&& Y<=41 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 11 
									&& X<=35 && X>=1 
									&& Y<=71 && Y>=30 ) 

						return true ;

				else	if( 	 CristalNbr == 12 
									&& X<=35 && X>=1 
									&& Y<=41 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 13 
									&& X<=104 && X>=60 
									&& Y<=101 && Y>=59 ) 

						return true ;

				else	if( 	 CristalNbr == 14 
									&& X<=104 && X>=60 
									&& Y<=128 && Y>=90 ) 

						return true ;

				else	if( 	 CristalNbr == 15 
									&& X<=128 && X>=90 
									&& Y<=101 && Y>=59 ) 

						return true ;

				else	if( 	 CristalNbr == 16 
									&& X<=128 && X>=90 
									&& Y<=128 && Y>=90 ) 

						return true ;

				else
						return false;
	}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::Clear()
	{
		EventMultiplicity= 0		;
		
		TelescopeNumber	.clear()	;
		EventType				.clear()	;
		TotalEnergy			.clear()	;
		
		// Si X
		Si_E.clear()	;
		Si_T.clear()	;
		Si_X.clear()	;
		Si_Y.clear()	;
		
		// Si(Li)
		SiLi_E.clear()	;
		SiLi_T.clear()	;
		SiLi_N.clear()	;
		
		// CsI	
		CsI_E.clear()	;
		CsI_T.clear()	;
		CsI_N.clear()	;
	}
///////////////////////////////////////////////////////////////////////////

////	Innherited from VDetector Class	////				
				
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TMust2Physics::ReadConfiguration(string Path) 	
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

//	Add Parameter to the CalibrationManger
void TMust2Physics::AddParameterToCalibrationManager()	
	{
		CalibrationManager* Cal = CalibrationManager::getInstance();
		
		for(int i = 0 ; i < NumberOfTelescope ; i++)
			{
			
				for( int j = 0 ; j < 128 ; j++)
					{
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_E")	;
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_E")	;
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_T")	;
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_T")	;	
					}
		
				for( int j = 0 ; j < 16 ; j++)
					{
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_E")	;
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_T")	;
					}
			
				for( int j = 0 ; j < 16 ; j++)
					{
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_E")		;
						Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_T")		;
					}
			}
	
	
	}

//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TMust2Physics::InitializeRootInput() 		
	{
		TChain* inputChain = RootInput::getInstance()->GetChain()	;
		inputChain->SetBranchStatus( "MUST2" , true )				;
		inputChain->SetBranchStatus( "fMM_*" , true )				;
		inputChain->SetBranchAddress( "MUST2" , &EventData )		;
	}


//	Create associated branches and associated private member DetectorPhysics address
void TMust2Physics::InitializeRootOutput() 	
	{
		TTree* outputTree = RootOutput::getInstance()->GetTree()		;
		outputTree->Branch( "MUST2" , "TMust2Physics" , &EventPhysics )	;
	}


/////	Specific to MUST2Array	////

void TMust2Physics::AddTelescope(	TVector3 C_X1_Y1 		,
			 					TVector3 C_X128_Y1 		, 
			 					TVector3 C_X1_Y128 		, 
			 					TVector3 C_X128_Y128	)
	{
		// To avoid warning
		C_X1_Y128 *= 1;

		NumberOfTelescope++;
	
		//	Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
		TVector3 U = C_X1_Y1 - C_X128_Y1 				;	
		double Ushift = (U.Mag()-98)/2. ;
		U = U.Unit()									;
		//	Vector V on Telescope Face (parallele to X Strip)
		TVector3 V = C_X128_Y128 - C_X128_Y1 				;
		double Vshift = (V.Mag() -98)/2. ;
		V = V.Unit()									;

		//	Position Vector of Strip Center
		TVector3 StripCenter = TVector3(0,0,0)			;
		//	Position Vector of X=1 Y=1 Strip 
		TVector3 Strip_1_1 								;		

		//	Geometry Parameter
		double Face = 98					 					  	; //mm
		double NumberOfStrip = 128 							; 
		double StripPitch = Face/NumberOfStrip	; //mm
		//	Buffer object to fill Position Array
		vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

		vector< vector< double > >	OneTelescopeStripPositionX	;
		vector< vector< double > >	OneTelescopeStripPositionY	;
		vector< vector< double > >	OneTelescopeStripPositionZ	;
		
		//	Moving StripCenter to 1.1 corner:
		Strip_1_1 = C_X128_Y1 + (U+V) * (StripPitch/2.) 	;
    Strip_1_1+= U*Ushift+V*Vshift ;
    
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
				
				
void TMust2Physics::AddTelescope(	double theta 	, 
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
	
	
TVector3 TMust2Physics::GetPositionOfInteraction(int i)
	{
		TVector3 Position = TVector3 (	GetStripPositionX( TelescopeNumber[i] , Si_X[i] , Si_Y[i] ) 	,
																		GetStripPositionY( TelescopeNumber[i] , Si_X[i] , Si_Y[i] )		,
																		GetStripPositionZ( TelescopeNumber[i] , Si_X[i] , Si_Y[i] )		) ;
		
		return(Position) ;	
	
	}
	
TVector3 TMust2Physics::GetTelescopeNormal( int i)
	{
				TVector3 U = 	TVector3 (	GetStripPositionX( TelescopeNumber[i] , 128 , 1 ) 	,
																	GetStripPositionY( TelescopeNumber[i] , 128 , 1 )		,
																	GetStripPositionZ( TelescopeNumber[i] , 128 , 1 )		)
											
									- 	TVector3 (	GetStripPositionX( TelescopeNumber[i] , 1 , 1 ) 		,
																	GetStripPositionY( TelescopeNumber[i] , 1 , 1 )			,
																	GetStripPositionZ( TelescopeNumber[i] , 1 , 1 )			);
										
				TVector3 V = 	TVector3 (	GetStripPositionX( TelescopeNumber[i] , 128 , 128 ) ,
																	GetStripPositionY( TelescopeNumber[i] , 128 , 128 )	,
																	GetStripPositionZ( TelescopeNumber[i] , 128 , 128 )	)
											
										-	TVector3 (	GetStripPositionX( TelescopeNumber[i] , 128 , 1 ) 	,
																	GetStripPositionY( TelescopeNumber[i] , 128 , 1 )		,
																	GetStripPositionZ( TelescopeNumber[i] , 128 , 1 )		);
											
				TVector3 Normal = U.Cross(V);
		
		return(Normal.Unit()) ;	
	
	}	

///////////////////////////////////////////////////////////////////////////
namespace LOCAL
	{
		
		//	tranform an integer to a string
		string itoa(int value)
			{
			  std::ostringstream o;
			
			  if (!(o << value))
			    return ""	;
			    
			  return o.str();
			}
			
		//	DSSD
		//	X
		double fSi_X_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/" + itoa( EventData->GetMMStripXEDetectorNbr(i) ) + "Si_X" + itoa( EventData->GetMMStripXEStripNbr(i) ) + "_E",	
																																		EventData->GetMMStripXEEnergy(i) );
																																	
			}
			
		double fSi_X_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripXTDetectorNbr(i) ) + "Si_X" + itoa( EventData->GetMMStripXTStripNbr(i) ) +"_T",	
																																		EventData->GetMMStripXTTime(i) );
			}
		
		//	Y	
		double fSi_Y_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripYEDetectorNbr(i) ) + "Si_Y" + itoa( EventData->GetMMStripYEStripNbr(i) ) +"_E",	
																																		EventData->GetMMStripYEEnergy(i) );
			}
			
		double fSi_Y_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripYTDetectorNbr(i) ) + "Si_Y" + itoa( EventData->GetMMStripYTStripNbr(i) ) +"_T",	
																																		EventData->GetMMStripYTTime(i) );
			}
			
			
		//	SiLi
		double fSiLi_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMSiLiEDetectorNbr(i) ) + "SiLi" + itoa( EventData->GetMMSiLiEPadNbr(i) ) +"_E",	
																																		EventData->GetMMSiLiEEnergy(i) );
			}
			
		double fSiLi_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMSiLiTDetectorNbr(i) ) + "SiLi" + itoa( EventData->GetMMSiLiTPadNbr(i) )+"_T",	
																																		EventData->GetMMSiLiTTime(i) );
			}
			
		//	CsI
		double fCsI_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMCsIEDetectorNbr(i) ) + "SiLi" + itoa( EventData->GetMMCsIECristalNbr(i) ) +"_E",	
																																		EventData->GetMMCsIEEnergy(i) );
			}
			
		double fCsI_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMCsITDetectorNbr(i) ) + "SiLi" + itoa( EventData->GetMMCsITCristalNbr(i) ) +"_T",	
																																		EventData->GetMMCsITTime(i) );
			}
	
	}

