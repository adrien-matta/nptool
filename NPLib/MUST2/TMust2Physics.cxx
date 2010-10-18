/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    : october 2010                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold must2 treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
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
		PreTreatedData			= new TMust2Data	;
		EventPhysics 				= this						;
		NumberOfTelescope		  = 0								;
		m_MaximumStripMultiplicityAllowed = 0	;
		m_StripEnergyMatchingTolerance    = 0 ;	
//                ReadAnalysisConfig();
	}
		
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::BuildSimplePhysicalEvent()
	{ 
	  BuildPhysicalEvent();
	}
	
///////////////////////////////////////////////////////////////////////////
	
void TMust2Physics::BuildPhysicalEvent()
	{ 
		PreTreat();
		
		bool check_SILI = false ;
		bool check_CSI  = false ;
	
		if( CheckEvent() == 1 )
			{
				vector< TVector2 > couple = Match_X_Y() ;
				EventMultiplicity = couple.size();
				for(unsigned int i = 0 ; i < couple.size() ; i++)
					{
						check_SILI = false ;
						check_CSI = false ;
					
						int N = PreTreatedData->GetMMStripXEDetectorNbr(couple[i].X())	;
						
						int X = PreTreatedData->GetMMStripXEStripNbr(couple[i].X())			;
						int Y = PreTreatedData->GetMMStripYEStripNbr(couple[i].Y())			;
						
						double Si_X_E = PreTreatedData->GetMMStripXEEnergy( couple[i].X() ) 	;
						double Si_Y_E = PreTreatedData->GetMMStripYEEnergy( couple[i].Y() ) 	;  
						
						
						//  Search for associate Time
						  double Si_X_T = -1000 ;
						  for(unsigned int t = 0 ; t < PreTreatedData->GetMMStripXTMult() ; t++ )
						    {
						      if(  PreTreatedData->GetMMStripXTStripNbr( couple[i].X() ) == PreTreatedData->GetMMStripXTStripNbr(t)
						         ||PreTreatedData->GetMMStripXTDetectorNbr( couple[i].X() ) == PreTreatedData->GetMMStripXTDetectorNbr(t)) 
						        Si_X_T = PreTreatedData->GetMMStripXTTime(t);
						    }
						    
						  double Si_Y_T = -1000 ;
						  for(unsigned int t = 0 ; t < PreTreatedData->GetMMStripYTMult() ; t++ )
						    {
						      if(  PreTreatedData->GetMMStripYTStripNbr( couple[i].Y() ) == PreTreatedData->GetMMStripYTStripNbr(t)
						         ||PreTreatedData->GetMMStripYTDetectorNbr( couple[i].Y() ) == PreTreatedData->GetMMStripYTDetectorNbr(t)) 
						        Si_Y_T = PreTreatedData->GetMMStripYTTime(t);
						    }
						
						Si_X.push_back(X) ; Si_Y.push_back(Y) ; TelescopeNumber.push_back(N) ;
						
						//	Take maximum Energy
						if(Si_X_E >= Si_Y_E) Si_E.push_back(Si_X_E)	;
						else								 Si_E.push_back(Si_Y_E)	;
						
						//	Take Y Time, better resolution than X.							
						Si_T.push_back(Si_Y_T)	;
						
						for(unsigned int j = 0 ; j < PreTreatedData->GetMMSiLiEMult() ; j++)
							{
								if(PreTreatedData->GetMMSiLiEDetectorNbr(j)==N)
									{
												// pad vs strip number match
												if( Match_Si_SiLi( X, Y , PreTreatedData->GetMMSiLiEPadNbr(j) ) )
												{
													SiLi_N.push_back(PreTreatedData->GetMMSiLiEPadNbr(j))	;
													SiLi_E.push_back(PreTreatedData->GetMMSiLiEEnergy(j))	;
													
													// Look for associate time
													// Note: in case of use of SiLi "Orsay" time is not coded.
													for(int k =0 ; k  < PreTreatedData->GetMMSiLiTMult() ; k ++)
														{
															// Same Pad, same Detector
															if( PreTreatedData->GetMMSiLiEPadNbr(j)==PreTreatedData->GetMMSiLiEPadNbr(k) && PreTreatedData->GetMMSiLiEDetectorNbr(j)==PreTreatedData->GetMMSiLiTDetectorNbr(k) )
																{ SiLi_T.push_back( PreTreatedData->GetMMSiLiTTime(k) ) ; break ;}
														}
													
													check_SILI = true ;
													
												}
											
									}
							}
							
						 for( int j = 0 ; j < PreTreatedData->GetMMCsIEMult() ; j++)
							{
											
								if(PreTreatedData->GetMMCsIEDetectorNbr(j)==N)
									{
											if(Match_Si_CsI( X, Y , PreTreatedData->GetMMCsIECristalNbr(j) ) )
													{
														 CsI_N.push_back( PreTreatedData->GetMMCsIECristalNbr(j) ) ;
														 CsI_E.push_back( PreTreatedData->GetMMCsIEEnergy(j) ) ;
														
														//	Look for associate Time																										
														for(int k =0 ; k  < PreTreatedData->GetMMCsITMult() ; k ++)
															{
																// Same Cristal; Same Detector
																if( PreTreatedData->GetMMCsIECristalNbr(j)==PreTreatedData->GetMMCsITCristalNbr(k) && PreTreatedData->GetMMCsIEDetectorNbr(j)==PreTreatedData->GetMMCsITDetectorNbr(k) )
																	{ CsI_T.push_back( PreTreatedData->GetMMCsITTime(j) ) ; break ;}
															}
														
														check_CSI = true ;
													}
											
										}
								}
							
						
					if(!check_SILI)
							{
								SiLi_N.push_back(0)			;
								SiLi_E.push_back(-1000)	;
								SiLi_T.push_back(-1000)	;
							}

						if(!check_CSI) 
							{
								CsI_N.push_back(0)			;
								CsI_E.push_back(-1000)	;
								CsI_T.push_back(-1000)	;
							}
					
					}
			}

		return;
	
	}	

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::PreTreat()
	{
		ClearPreTreatedData();
	
		//	X
			//	E
			for(int i = 0 ; i < EventData->GetMMStripXEMult() ; i++)
				{
							if(IsValidChannel("X", EventData->GetMMStripXEDetectorNbr(i), EventData->GetMMStripXEStripNbr(i)))
								{
									double EX = fSi_X_E(EventData , i); 
									if( EX > Si_X_E_Threshold )
										{
											PreTreatedData->SetMMStripXEDetectorNbr( EventData->GetMMStripXEDetectorNbr(i) )	;
											PreTreatedData->SetMMStripXEStripNbr( EventData->GetMMStripXEStripNbr(i) )				;
											PreTreatedData->SetMMStripXEEnergy( EX )																					;
										}
									
								}
						
					
					else
						{
						
						}
				
				}
				
			//	T
			for(int i = 0 ; i < EventData->GetMMStripXTMult() ; i++)
				{
							if(IsValidChannel("X", EventData->GetMMStripXTDetectorNbr(i), EventData->GetMMStripXTStripNbr(i)))
								{
									PreTreatedData->SetMMStripXTDetectorNbr( EventData->GetMMStripXTDetectorNbr(i) )	;
									PreTreatedData->SetMMStripXTStripNbr( EventData->GetMMStripXTStripNbr(i) )				;
									PreTreatedData->SetMMStripXTTime( fSi_X_T(EventData , i) )												;
								}
					
					else
						{
						
						}
				
				}
				
				
		//	Y
			//	E
			for(int i = 0 ; i < EventData->GetMMStripYEMult() ; i++)
				{
							if(IsValidChannel("Y", EventData->GetMMStripYEDetectorNbr(i), EventData->GetMMStripYEStripNbr(i)))
								{
									double EY = fSi_Y_E(EventData , i); 
									if( EY > Si_Y_E_Threshold )
										{
											PreTreatedData->SetMMStripYEDetectorNbr( EventData->GetMMStripYEDetectorNbr(i) )	;
											PreTreatedData->SetMMStripYEStripNbr( EventData->GetMMStripYEStripNbr(i) )				;
											PreTreatedData->SetMMStripYEEnergy( EY )																					;
										}
								}
					
					else
						{
						
						}
				
				}
				
			//	T
			for(int i = 0 ; i < EventData->GetMMStripYTMult() ; i++)
				{
							if(IsValidChannel("Y", EventData->GetMMStripYTDetectorNbr(i), EventData->GetMMStripYTStripNbr(i)))
								{
									PreTreatedData->SetMMStripYTDetectorNbr( EventData->GetMMStripYTDetectorNbr(i) )	;
									PreTreatedData->SetMMStripYTStripNbr( EventData->GetMMStripYTStripNbr(i) )				;
									PreTreatedData->SetMMStripYTTime( fSi_Y_T(EventData , i) )												;
								}
					
					else
						{
						
						}
				
				}


		//	CsI
			//	E
			for(int i = 0 ; i < EventData->GetMMCsIEMult() ; i++)
				{
				
							if(IsValidChannel("CsI", EventData->GetMMCsIEDetectorNbr(i), EventData->GetMMCsIECristalNbr(i)))
								{
									double ECsI = fCsI_E(EventData , i); 
									if( ECsI > CsI_E_Threshold )
										{
											PreTreatedData->SetMMCsIEDetectorNbr( EventData->GetMMCsIEDetectorNbr(i) )		;
											PreTreatedData->SetMMCsIECristalNbr( EventData->GetMMCsIECristalNbr(i) )			;
											PreTreatedData->SetMMCsIEEnergy( ECsI )																				;
										}
								}
						
					else
						{
						
						}
				
				}
				
			//	T
			for(int i = 0 ; i < EventData->GetMMCsITMult() ; i++)
				{
							if(IsValidChannel("CsI", EventData->GetMMCsITDetectorNbr(i), EventData->GetMMCsITCristalNbr(i)))
								{
									PreTreatedData->SetMMCsITDetectorNbr( EventData->GetMMCsITDetectorNbr(i) )		;
									PreTreatedData->SetMMCsITCristalNbr( EventData->GetMMCsITCristalNbr(i) )			;
									PreTreatedData->SetMMCsITTime( fCsI_T(EventData , i) )												;
								}	
						
					else
						{
						
						}
				
				}
				
				
		//	SiLi
			//	E
			for(int i = 0 ; i < EventData->GetMMSiLiEMult() ; i++)
				{
							if(IsValidChannel("SiLi", EventData->GetMMSiLiEDetectorNbr(i), EventData->GetMMSiLiEPadNbr(i)))
								{
									double ESiLi = fSiLi_E(EventData , i); 
									if( ESiLi > SiLi_E_Threshold )
										{
											PreTreatedData->SetMMSiLiEDetectorNbr( EventData->GetMMSiLiEDetectorNbr(i) )	;
											PreTreatedData->SetMMSiLiEPadNbr( EventData->GetMMSiLiEPadNbr(i) )						;
											PreTreatedData->SetMMSiLiEEnergy( ESiLi )										;
										}
								}
					
					else
						{
						
						}
				
				}
				
			//	T
			for(int i = 0 ; i < EventData->GetMMSiLiTMult() ; i++)
				{
							if(IsValidChannel("SiLi", EventData->GetMMSiLiTDetectorNbr(i), EventData->GetMMSiLiTPadNbr(i)))
								{
									PreTreatedData->SetMMSiLiTDetectorNbr( EventData->GetMMSiLiTDetectorNbr(i) )	;
									PreTreatedData->SetMMSiLiTPadNbr( EventData->GetMMSiLiTPadNbr(i) )						;
									PreTreatedData->SetMMSiLiTTime( fSiLi_T(EventData , i) )											;
								}
					
					else
						{
						
						}
				
				}
	
	
		return;
	}


///////////////////////////////////////////////////////////////////////////
int TMust2Physics :: CheckEvent()
	{
		// Check the size of the different elements
				 if(			PreTreatedData->GetMMStripXEMult() == PreTreatedData->GetMMStripYEMult() /*&& PreTreatedData->GetMMStripYEMult() == PreTreatedData->GetMMStripXTMult() &&  PreTreatedData->GetMMStripXTMult() == PreTreatedData->GetMMStripYTMult()*/  )
	
					return 1 ; // Regular Event
	
		else if(			PreTreatedData->GetMMStripXEMult() == PreTreatedData->GetMMStripYEMult()+1 || PreTreatedData->GetMMStripXEMult() == PreTreatedData->GetMMStripYEMult()-1  )
	
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
		if( PreTreatedData->GetMMStripXEMult() > m_MaximumStripMultiplicityAllowed || PreTreatedData->GetMMStripYEMult() > m_MaximumStripMultiplicityAllowed )
			return ArrayOfGoodCouple;
		
		for(int i = 0 ; i < PreTreatedData->GetMMStripXEMult(); i++)
			{
						for(int j = 0 ; j < PreTreatedData->GetMMStripYEMult(); j++)
							{
										//	if same detector check energy
										if ( PreTreatedData->GetMMStripXEDetectorNbr(i) == PreTreatedData->GetMMStripYEDetectorNbr(j) )
											{
													//	Look if energy match (within 10%)
													if( abs((  PreTreatedData->GetMMStripXEEnergy(i) - PreTreatedData->GetMMStripYEEnergy(j) ) / PreTreatedData->GetMMStripXEEnergy(i)) < m_StripEnergyMatchingTolerance/100.	)
														ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;	
											}
							}
			}
	
		//	Prevent to treat event with ambiguous matchin beetween X and Y
		if( ArrayOfGoodCouple.size() > PreTreatedData->GetMMStripXEMult() ) ArrayOfGoodCouple.clear() ;
		
		return ArrayOfGoodCouple;	
	}
	
	
////////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: IsValidChannel(string DetectorType, int telescope , int channel)
	{
		vector<bool>::iterator it ;
		if(DetectorType == "X")
			return *(XChannelStatus[telescope].begin()+channel);
			
		else if(DetectorType == "Y")
			return *(YChannelStatus[telescope].begin()+channel);
			
		else if(DetectorType == "SiLi")
			return *(SiLiChannelStatus[telescope].begin()+channel);
			
		else if(DetectorType == "CsI")
			return *(CsIChannelStatus[telescope].begin()+channel);
			
		else return false;
	}
	
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;
   bool check_mult    = false;
   bool check_match   = false;

   // path to file
   string FileName = "./configs/ConfigMust2.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigMust2.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigMust2.dat " << endl;

   // read analysis config file
   string LineBuffer,DataBuffer;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 11, "ConfigMust2") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {
         AnalysisConfigFile >> DataBuffer;

         // Search for comment symbol (%)
         if (DataBuffer.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }
         else if (DataBuffer.compare(0, 22, "MAX_STRIP_MULTIPLICITY") == 0) {
            check_mult = true;
            AnalysisConfigFile >> DataBuffer;
            m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
            cout << "MAX_STRIP_MULTIPLICITY= " << m_MaximumStripMultiplicityAllowed << endl;
         }
         else if (DataBuffer.compare(0, 31, "STRIP_ENERGY_MATCHING_TOLERANCE") == 0) {
            check_match = true;
            AnalysisConfigFile >> DataBuffer;
            m_StripEnergyMatchingTolerance = atoi(DataBuffer.c_str() );
            cout << "STRIP_ENERGY_MATCHING_TOLERANCE= " << m_StripEnergyMatchingTolerance << endl;
         }
         else if (DataBuffer.compare(0, 5, "MUST2") == 0) {
            AnalysisConfigFile >> DataBuffer;
            string whatToDo = DataBuffer;
            if (whatToDo.compare(0, 11, "DISABLE_ALL") == 0) {
               AnalysisConfigFile >> DataBuffer;
               cout << whatToDo << "  " << DataBuffer << endl;
               int telescope = atoi(DataBuffer.substr(2,1).c_str());
               vector< bool > ChannelStatus;
               ChannelStatus.resize(128,false);
               XChannelStatus[telescope] = ChannelStatus;
               YChannelStatus[telescope] = ChannelStatus;
               ChannelStatus.resize(16,false);
               SiLiChannelStatus[telescope] = ChannelStatus;
               CsIChannelStatus[telescope]  = ChannelStatus;
            }
            else if (whatToDo.compare(0, 15, "DISABLE_CHANNEL") == 0) {
               AnalysisConfigFile >> DataBuffer;
               cout << whatToDo << "  " << DataBuffer << endl;
               int telescope = atoi(DataBuffer.substr(2,1).c_str());
               int channel = -1;
               if (DataBuffer.compare(4,4,"STRX") == 0) {
                  channel = atoi(DataBuffer.substr(9).c_str());
                  *(XChannelStatus[telescope].begin()+channel) = false;
               }
               else if (DataBuffer.compare(4,4,"STRY") == 0) {
                  channel = atoi(DataBuffer.substr(9).c_str());
                  *(YChannelStatus[telescope].begin()+channel) = false;
               }
               else if (DataBuffer.compare(4,4,"SILI") == 0) {
                  channel = atoi(DataBuffer.substr(9).c_str());
                  *(SiLiChannelStatus[telescope].begin()+channel) = false;
               }
               else if (DataBuffer.compare(4,3,"CSI") == 0) {
                  channel = atoi(DataBuffer.substr(8).c_str());
                  *(CsIChannelStatus[telescope].begin()+channel) = false;
               }
               else {
                  cout << "Warning: detector type for Must2 unknown!" << endl;
               }
            }
            else {
               cout << "Warning: don't know what to do (lost in translation)" << endl;
            }
         }
         else {
            ReadingStatus = false;
//            cout << "WARNING: Wrong Token Sequence" << endl;
         }
      }
   }
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
									&& X<=128 && X>=97 
									&& Y<=128 && Y>=97 ) 

						return true ;


				else	if( 	 CristalNbr == 2 
									&& X<=128 && X>=97 
									&& Y<=96 && Y>=65 ) 

						return true ;


				else	if( 	 CristalNbr == 3 
									&& X<=128 && X>=97 
									&& Y<=64 && Y>=33 ) 

						return true ;

				else	if( 	 CristalNbr == 4 
									&& X<=128 && X>=7 
									&& Y<=32 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 5 
									&& X<=96 && X>=65 
									&& Y<=32 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 6 
									&& X<=96 && X>=65 
									&& Y<=64 && Y>=33 ) 

						return true ;

				else	if( 	 CristalNbr == 7 
									&& X<=96 && X>=65 
									&& Y<=96 && Y>=65 )

						return true ;

				else	if( 	 CristalNbr == 8 
									&& X<=96 && X>=65 
									&& Y<=128 && Y>=97 ) 

						return true ;

				else	if( 	 CristalNbr == 9 
									&& X<=64 && X>=33 
									&& Y<=32 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 10 
									&& X<=64 && X>=33 
									&& Y<=64 && Y>=33 ) 

						return true ;

				else	if( 	 CristalNbr == 11 
									&& X<=64 && X>=33 
									&& Y<=96 && Y>=65 ) 

						return true ;

				else	if( 	 CristalNbr == 12 
									&& X<=64 && X>=33 
									&& Y<=128 && Y>=97 ) 

						return true ;

				else	if( 	 CristalNbr == 13 
									&& X<=32 && X>=1 
									&& Y<=32 && Y>=1 ) 

						return true ;

				else	if( 	 CristalNbr == 14 
									&& X<=32 && X>=1 
									&& Y<=64 && Y>=33 ) 

						return true ;

				else	if( 	 CristalNbr == 15 
									&& X<=32 && X>=1 
									&& Y<=96 && Y>=65 ) 

						return true ;

				else	if( 	 CristalNbr == 16 
									&& X<=32 && X>=1 
									&& Y<=128 && Y>=97 ) 

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
		
		Si_EX.clear()				;
		Si_TX.clear()				;
		Si_EY.clear()			;
		Si_TY.clear()				;
		TelescopeNumber_X.clear()				;
		TelescopeNumber_Y.clear()				;
	}
///////////////////////////////////////////////////////////////////////////

void TMust2Physics::ReadCalibrationRun()
	{
		//	X
			//	E	
		for(int i = 0 ; i < EventData->GetMMStripXEMult();i++)
			{
						TelescopeNumber_X.push_back(EventData->GetMMStripXEDetectorNbr(i));
						Si_EX.push_back( fSi_X_E( EventData , i) )				;
						Si_X.push_back(EventData->GetMMStripXEStripNbr(i));
				
			}
			//	T
		for(int i = 0 ; i < EventData->GetMMStripXTMult();i++)
			{
						TelescopeNumber_X.push_back(EventData->GetMMStripXTDetectorNbr(i));
						Si_TX.push_back( fSi_X_T( EventData , i) )				;
						Si_X.push_back(EventData->GetMMStripXTStripNbr(i));
					
			}
		
		//	Y
			//	E
		for(int i = 0 ; i < EventData->GetMMStripYEMult();i++)
			{
						TelescopeNumber_Y.push_back(EventData->GetMMStripYEDetectorNbr(i));
						Si_EY.push_back( fSi_Y_E( EventData , i) )				;
						Si_Y.push_back(EventData->GetMMStripYEStripNbr(i));
					
						
			}
			
			//	T
		for(int i = 0 ; i < EventData->GetMMStripYTMult();i++)
			{ 
						TelescopeNumber_Y.push_back(EventData->GetMMStripYTDetectorNbr(i));
						Si_TY.push_back( fSi_Y_T( EventData , i) )				;
						Si_Y.push_back(EventData->GetMMStripYTStripNbr(i));
					
			}
			
	  //CsI Energy
		for( int j = 0 ; j < EventData->GetMMCsIEMult() ; j++)
		  {				
				   CsI_N.push_back(EventData->GetMMCsIECristalNbr(j))	;								
					 CsI_E.push_back(fCsI_E(EventData , j))							;
				 
		  }
		
		//CsI Time
		for( int j = 0 ; j < EventData->GetMMCsITMult() ; j++)
		  {			
				   //CsI_N.push_back(EventData->GetMMCsITCristalNbr(j))	;								
					 CsI_T.push_back(fCsI_T(EventData , j))							;
				 
		  }
	
	}

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

		   				check_Theta = false   ;
		   				check_Phi  = false  	;
		   				check_R    = false   	;
		   				check_beta = false  	;
					
		         	}
		         
		}
		        
//		InitializeStandardParameter();        
//                ReadAnalysisConfig();
	}
		InitializeStandardParameter();        
                ReadAnalysisConfig();
	
	cout << endl << "/////////////////////////////" << endl << endl;

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
			
		return;
	
	}

//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TMust2Physics::InitializeRootInput() 		
	{
		TChain* inputChain = RootInput::getInstance()->GetChain()	;
		inputChain->SetBranchStatus( "MUST2" , true )				;
		//added for compatibility with Riken file
		inputChain->SetBranchStatus( "Must2" , true )				;
		inputChain->SetBranchStatus( "fMM_*" , true )				;
		inputChain->SetBranchAddress( "MUST2" , &EventData )		;
		inputChain->SetBranchAddress( "Must2" , &EventData )		;
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
		TVector3 U = C_X128_Y1 - C_X1_Y1 				;	
		double Ushift = (U.Mag()-98)/2. ;
		U = U.Unit()									;
		//	Vector V on Telescope Face (parallele to X Strip)
		TVector3 V = C_X1_Y128 - C_X1_Y1 				;
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
		Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2.) 	;
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
	
	
void TMust2Physics::InitializeStandardParameter()
	{
		//	Enable all channel
		vector< bool > ChannelStatus;

		ChannelStatus.resize(128,true);
		for(int i = 0 ; i < NumberOfTelescope ; i ++)		
			{
				XChannelStatus[i+1] = ChannelStatus;
				YChannelStatus[i+1] = ChannelStatus;
			}
		
		ChannelStatus.resize(16,true);
		for(int i = 0 ; i < NumberOfTelescope ; i ++)		
			{
				SiLiChannelStatus[i+1] = ChannelStatus;
				CsIChannelStatus[i+1]  = ChannelStatus;
			}
			
			
		m_MaximumStripMultiplicityAllowed = NumberOfTelescope	;
		m_StripEnergyMatchingTolerance    = 10 								;	// %	
		
		return;
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
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripXEDetectorNbr(i) ) + "_Si_X" + itoa( EventData->GetMMStripXEStripNbr(i) ) + "_E",	
																																		EventData->GetMMStripXEEnergy(i) );
																																	
			}
			
		double fSi_X_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripXTDetectorNbr(i) ) + "_Si_X" + itoa( EventData->GetMMStripXTStripNbr(i) ) +"_T",	
																																		EventData->GetMMStripXTTime(i) );
			}
		
		//	Y	
		double fSi_Y_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripYEDetectorNbr(i) ) + "_Si_Y" + itoa( EventData->GetMMStripYEStripNbr(i) ) +"_E",	
																																		EventData->GetMMStripYEEnergy(i) );
			}
			
		double fSi_Y_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMStripYTDetectorNbr(i) ) + "_Si_Y" + itoa( EventData->GetMMStripYTStripNbr(i) ) +"_T",	
																																		EventData->GetMMStripYTTime(i) );
			}
			
			
		//	SiLi
		double fSiLi_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMSiLiEDetectorNbr(i) ) + "_SiLi" + itoa( EventData->GetMMSiLiEPadNbr(i) ) +"_E",	
																																		EventData->GetMMSiLiEEnergy(i) );
			}
			
		double fSiLi_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMSiLiTDetectorNbr(i) ) + "_SiLi" + itoa( EventData->GetMMSiLiTPadNbr(i) )+"_T",	
																																		EventData->GetMMSiLiTTime(i) );
			}
			
		//	CsI
		double fCsI_E(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMCsIEDetectorNbr(i) ) + "_CsI" + itoa( EventData->GetMMCsIECristalNbr(i) ) +"_E",	
																																		EventData->GetMMCsIEEnergy(i) );
			}
			
		double fCsI_T(TMust2Data* EventData , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( EventData->GetMMCsITDetectorNbr(i) ) + "_CsI" + itoa( EventData->GetMMCsITCristalNbr(i) ) +"_T",	
																																		EventData->GetMMCsITTime(i) );
			}
	
	}

