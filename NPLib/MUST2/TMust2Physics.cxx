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
#include <iostream>
#include <cmath>
///////////////////////////////////////////////////////////////////////////
namespace {
	// Threshold
	const double Si_X_E_Threshold = 0	;	const double Si_X_T_Threshold = 0 ;
	const double Si_Y_E_Threshold = 0	;	const double Si_Y_T_Threshold = 0	;
	const double SiLi_E_Threshold = 0	;	const double SiLi_T_Threshold = 0	;
	const double CsI_E_Threshold	= 0 ;	const double CsI_T_Threshold	= 0	;
}
///////////////////////////////////////////////////////////////////////////


ClassImp(TMust2Physics)
///////////////////////////////////////////////////////////////////////////
TMust2Physics::TMust2Physics() 
	{ EventMultiplicity = 0 ;}
	
///////////////////////////////////////////////////////////////////////////
TMust2Physics::~TMust2Physics() {Clear();}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::BuildSimplePhysicalEvent(TMust2Data* Data)
	{
		BuildPhysicalEvent(Data);
	}
	
///////////////////////////////////////////////////////////////////////////
	
void TMust2Physics::BuildPhysicalEvent(TMust2Data* Data)
	{ 
		if( CheckEvent(Data) == 1 )
			{
				vector< TVector2 > couple = Match_X_Y(Data) ;
				
				for(unsigned int i = 0 ; i < couple.size() ; i++)
					{
						int N = Data->GetMMStripXEDetectorNbr(couple[i].X())		;
						int X = Data->GetMMStripXEStripNbr(couple[i].X())				;
						int Y = Data->GetMMStripXEStripNbr(couple[i].Y())				;
						
						double Si_X_E = Data->GetMMStripXEEnergy(couple[i].X())	;
						double Si_Y_E = Data->GetMMStripYEEnergy(couple[i].Y())	;

						double Si_X_T = Data->GetMMStripXTTime(couple[i].X())		;
						double Si_Y_T = Data->GetMMStripYTTime(couple[i].Y())		;
					
						Si_X.push_back(X) ; Si_Y.push_back(Y) ; TelescopeNumber.push_back(N) ;
						
						// Take maximum Energy
						if(Si_X_E >= Si_Y_E) Si_E.push_back(Si_X_E)	;
						else								Si_E.push_back(Si_Y_E)	;
						
						// Take minimum Time
						if(Si_X_T >= Si_Y_T) Si_T.push_back(Si_Y_T)	;
						else								Si_T.push_back(Si_X_T)	;
						
						for(unsigned int j = 0 ; j < Data->GetMMSiLiEMult() ; j++)
							{
								if(Data->GetMMSiLiEDetectorNbr(j)==N)
									{
										if( Match_Si_SiLi( X, Y , Data->GetMMSiLiEPadNbr(j) ) )
										{
											SiLi_N.push_back(Data->GetMMSiLiEPadNbr(j))	;
											SiLi_E.push_back(Data->GetMMSiLiEEnergy(j))	;
											SiLi_T.push_back(Data->GetMMSiLiTTime(j))		;
										}
										
									}
								
							}
							
							
						 for( int j = 0 ; j < Data->GetMMCsIEMult() ; j++)
							{
								if(Data->GetMMCsIEDetectorNbr(j)==N)
									{
										if( Match_Si_CsI( X, Y , Data->GetMMCsIECristalNbr(j) ) )
										{
											CsI_N.push_back(Data->GetMMCsIECristalNbr(j))	;
											CsI_E.push_back(Data->GetMMCsIEEnergy(j))			;
											CsI_T.push_back(Data->GetMMCsITTime(j))				;
										}
									}
								
							}
					
					
					}
				
			}
		
		return;
	
	}	
	
	
	
	
	
	
	
	
	/*
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::BuildPhysicalEvent(TMust2Data* Data)
	{ 
		//	Check
		bool Check_Si = false ;bool Check_SiLi = false ; bool Check_CsI = false ;
	
		//	Multiplicity 1
		if( Data->GetMMStripXEMult()==1 && Data->GetMMStripYEMult()==1 && Data->GetMMStripXTMult()==1 && Data->GetMMStripXTMult()==1 )
			{
				
				if( //Same detector
						  Data->GetMMStripXEDetectorNbr(0) == Data->GetMMStripXTDetectorNbr(0)
					&&	Data->GetMMStripXTDetectorNbr(0) == Data->GetMMStripYTDetectorNbr(0)
					&&	Data->GetMMStripYTDetectorNbr(0) == Data->GetMMStripYEDetectorNbr(0) 
					
					// Same strip
					&&	Data->GetMMStripXEStripNbr(0) == Data->GetMMStripXTStripNbr(0)
					&&	Data->GetMMStripYEStripNbr(0) == Data->GetMMStripYTStripNbr(0)      )
					{
					
						EventMultiplicity = 1;
					
						TelescopeNumber.push_back(Data->GetMMStripXEDetectorNbr(0))	;
					
						//	Data->Get Max Energy
				//		if(Data->GetMMStripXEEnergy(0) > Data->GetMMStripYEEnergy(0))	Si_E.push_back( Data->GetMMStripXEEnergy(0) ) ;
				//		else															Si_E.push_back( Data->GetMMStripYEEnergy(0) ) ;
						Si_E.push_back( Data->GetMMStripXEEnergy(0) ) ;
					
						//	Data->Get Min Time
				//		if(Data->GetMMStripXTTime(0) < Data->GetMMStripYTTime(0))		Si_T.push_back( Data->GetMMStripXTTime(0) ) ;
				//		else															Si_T.push_back( Data->GetMMStripYTTime(0) ) ;
						Si_T.push_back( Data->GetMMStripXTTime(0) ) ;

						Si_X.push_back( Data->GetMMStripXEStripNbr(0) )	;
						Si_Y.push_back( Data->GetMMStripYEStripNbr(0) )	;	
						
						Check_Si = true ;			
								
					}
					
				
				// FIXME we have to resolve case where SiLi/CsI mult > Si mult by looking time? and Si XY vs Pad/crystal Nbr
				if (Check_Si)
					{
						
						//	Si(Li)
						for (int i = 0 ; i < Data->GetMMSiLiEMult() ; i++)
							{
								if (	Data->GetMMSiLiEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(0)
									&&	Data->GetMMSiLiEEnergy(i) > SiLi_E_Threshold	)
									{
										SiLi_E.push_back(Data->GetMMSiLiEEnergy(i))	;
										SiLi_N.push_back(Data->GetMMSiLiEPadNbr(i))	;
										
										if ( Data->GetMMSiLiTDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(0) )
											{
											SiLi_T.push_back(Data->GetMMSiLiTTime(i))	;
											Check_SiLi = true ;
											}
									}				
							}
						
						//	CsI
						for (int i = 0 ; i < Data->GetMMCsIEMult() ; i++)
							{
								if (	Data->GetMMCsIEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(0)
									&&	Data->GetMMCsIEEnergy(i) > CsI_E_Threshold	)
									{
										CsI_E.push_back(Data->GetMMCsIEEnergy(i))		;
										CsI_N.push_back(Data->GetMMCsIECristalNbr(i))	;
										
										if ( Data->GetMMCsITDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(0) )
											{
											CsI_T.push_back(Data->GetMMCsITTime(i))		;
											Check_CsI = true ;
											}
									}
							}
							
					 
					 			if(!Check_SiLi && !Check_CsI ) { 	TotalEnergy.push_back( Si_E.at(0) )									; 
					 																				CsI_E.push_back(0) 		;  CsI_T.push_back(0) 				; 
																									SiLi_E.push_back(0) 	;  SiLi_T.push_back(0) 				;	}
																									
					 else if (Check_SiLi  && !Check_CsI ) {	TotalEnergy.push_back( Si_E.at(0) + SiLi_E.at(0) )	;
					 																				CsI_E.push_back(0) 	;  CsI_T.push_back(0) 					; }
																																				
					 else if (Check_CsI   && !Check_SiLi) {	TotalEnergy.push_back( CsI_E .at(0)  + Si_E.at(0) )	; 
																									SiLi_E.push_back(0) 	;  SiLi_T.push_back(0) 				;	}
					 
					 TotalEnergy.push_back( CsI_E[0]  + Si_E[0] + SiLi_E[0]	);
					 return;
					}

				//FIXME: should built a pseudo event and then Check if particle could be identified with EDE method
				// Dump	
			}
		
		//	Multiplicity 2
		if( Data->GetMMStripXEMult()==2 && Data->GetMMStripYEMult()==2 && Data->GetMMStripXTMult()==2 && Data->GetMMStripXTMult()==2 )
			{
				// Different telescope case
				if (	Data->GetMMStripXEDetectorNbr(0) != Data->GetMMStripXEDetectorNbr(1)
					&&	Data->GetMMStripYEDetectorNbr(0) != Data->GetMMStripYEDetectorNbr(1) )
					{
						EventMultiplicity = 2 ;
						double EY, EX, TX, TY = 0;
						// loop on both event
						for (int jj = 0 ; jj < 2 ; jj++)
							{
								Check_Si = false ;Check_SiLi = false ;Check_CsI = false ;
							
								TelescopeNumber.push_back( Data->GetMMStripXEDetectorNbr(jj) )	;
								EX = Data->GetMMStripXEEnergy(jj) 				;
								Si_X.push_back( Data->GetMMStripXEStripNbr(jj))	;
								
								// Get Corresponding time
								for(int i = 0 ; i < 2 ; i++)
									{ 
										if(		Data->GetMMStripXTDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj) 
											&&	Data->GetMMStripXTStripNbr(i)    == Data->GetMMStripXEStripNbr(jj)   )
											{ TX = Data->GetMMStripXTTime(jj)	; }
									}
								
								// Get Corresponding Y strip
								for(int i = 0 ; i < 2 ; i++)
									{
										if(	Data->GetMMStripYEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj) )
											{
												Si_Y.push_back( Data->GetMMStripYEStripNbr(i))	;
												EY = Data->GetMMStripXEEnergy(i) 				;
												TY = Data->GetMMStripXTTime(i) 					;
												
												if (EX>EY)	Si_E.push_back(EX)	;
												else	  		Si_E.push_back(EY)	;
												
												
												if (TX>TY)	Si_T.push_back(TY)	;
												else	  		Si_T.push_back(TX)	; 
												Check_Si = true ;
											}
									}
							
								if (Check_Si)
									{ 
										//	Si(Li)
										for (int i = 0 ; i < Data->GetMMSiLiEMult() ; i++)
											{ 
												if (	Data->GetMMSiLiEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
													&&	Data->GetMMSiLiEEnergy(i) > SiLi_E_Threshold	)
													{
														Check_SiLi = true ;
														SiLi_E.push_back(Data->GetMMSiLiEEnergy(i))	;
														SiLi_N.push_back(Data->GetMMSiLiEPadNbr(i))	;
														
														if (	Data->GetMMSiLiTDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
															&&	Data->GetMMSiLiTPadNbr(i)      == Data->GetMMSiLiEPadNbr(i) )
																{
																	SiLi_T.push_back(Data->GetMMSiLiTTime(i))	;
																}
													}
													
											}
											
										if(!Check_SiLi)
											{
												SiLi_E.push_back(-1)	;
												SiLi_T.push_back(-1)	;
												SiLi_N.push_back(-1)	;
											}
								
										//	CsI
										for (int i = 0 ; i < Data->GetMMCsIEMult() ; i++)
											{
												if (	Data->GetMMCsIEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
													&&	Data->GetMMCsIEEnergy(i) > CsI_E_Threshold	)
													{
														Check_CsI = true ;
														CsI_E.push_back(Data->GetMMCsIEEnergy(i))			;
														CsI_N.push_back(Data->GetMMCsIECristalNbr(i))	;
														CsI_T.push_back(Data->GetMMCsITTime(i))				;
													}
													
											}
										
										if(!Check_CsI)
											{
												CsI_E.push_back(-200)	;
												CsI_T.push_back(-2)	;
												CsI_N.push_back(-2)	;
											}
										
										TotalEnergy.push_back(Si_E[jj]) ;
										if (Check_SiLi) TotalEnergy[jj] += SiLi_E[jj]	;
										if (Check_CsI)  TotalEnergy[jj] += CsI_E[jj]	;
									}	
							}
						return;	
					}
					
				//	Same detector case :
				if(		Data->GetMMStripXEDetectorNbr(0) == Data->GetMMStripXEDetectorNbr(1)
					&&	Data->GetMMStripYEDetectorNbr(0) == Data->GetMMStripYEDetectorNbr(1))
					{
					
						EventMultiplicity = -1 ;
				
						double EY1, EX1, TX1, TY1 = 0;
						double EY2, EX2, TX2, TY2 = 0;
						
						double Delta01, Delta00 = 0;
						Delta01 = fabs( Data->GetMMStripXEEnergy(0) - Data->GetMMStripYEEnergy(1) ) ;
						Delta00 = fabs( Data->GetMMStripXEEnergy(0) - Data->GetMMStripYEEnergy(0) ) ;
						
						if( Delta01 < Delta00 ) 
							{
								EX1 = Data->GetMMStripXEEnergy(0) 	;	 EY1 =  Data->GetMMStripYEEnergy(1) ;
								EX2 = Data->GetMMStripXEEnergy(1) 	;	 EY2 =  Data->GetMMStripYEEnergy(0) ;
								
								TX1 = Data->GetMMStripXTTime(0)		; 	TY1 =  Data->GetMMStripYTTime(1) ;
								TX2 = Data->GetMMStripXTTime(1) 	; 	TY2 =  Data->GetMMStripYTTime(0) ;
							}
							
						else
							{
								EX1 = Data->GetMMStripXEEnergy(0) 	;	 EY1 =  Data->GetMMStripYEEnergy(0) ;
								EX2 = Data->GetMMStripXEEnergy(1) 	;	 EY2 =  Data->GetMMStripYEEnergy(1) ;
								
								TX1 = Data->GetMMStripXTTime(0) 	;	 TY1 =  Data->GetMMStripYTTime(0) ;
								TX2 = Data->GetMMStripXTTime(1) 	;	 TY2 =  Data->GetMMStripYTTime(1) ;
							}
							
					
					return;
					}
			}
			
	}
*/
///////////////////////////////////////////////////////////////////////////
int TMust2Physics :: CheckEvent(TMust2Data* Data)
	{
		// Check the size of the different elements
				 if(			Data->GetMMStripXEMult() == Data->GetMMStripYEMult() && Data->GetMMStripYEMult() == Data->GetMMStripXTMult() &&  Data->GetMMStripXTMult() == Data->GetMMStripYTMult()  )
	
					return 1 ; // Regular Event
	
		else if(			Data->GetMMStripXEMult() == Data->GetMMStripYEMult()+1 || Data->GetMMStripXEMult() == Data->GetMMStripYEMult()-1  )
	
					return 2 ; // Pseudo Event, potentially interstrip
		
		else 	return -1 ; // Rejected Event

	}

///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: ResolvePseudoEvent(TMust2Data* Data)
	{
		return false;
	}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TMust2Physics :: Match_X_Y(TMust2Data* Data)
	{
		vector < TVector2 > ArrayOfGoodCouple ;
		
		for(int i = 0 ; i < Data->GetMMStripXEMult(); i++)
			{
				for(int j = 0 ; j < Data->GetMMStripYEMult(); j++)
					{
							if ( Data->GetMMStripXEDetectorNbr(i) == Data->GetMMStripYEDetectorNbr(j) )
									{
											if( fabs(Data->GetMMStripXEEnergy(i) - Data->GetMMStripYEEnergy(j))/Data->GetMMStripXEEnergy(i) < 0.1	)
												ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;	
									}
					}
			}
	
		if( ArrayOfGoodCouple.size() > Data->GetMMStripXEMult() ) ArrayOfGoodCouple.clear() ;
		
		return ArrayOfGoodCouple;	
	}
	
	
///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: Match_Si_SiLi(int X, int Y , int PadNbr)
	{
							if( 	 PadNbr == 1 
									&& X<121 && X>93
									&& Y<128 && Y>95 ) 

						return true ;


				else	if( 	 PadNbr == 2 
									&& X<121 && X>93 
									&& Y<100 && Y>65 ) 

						return true ;


				else	if( 	 PadNbr == 3 
									&& X<96 && X>61 
									&& Y<128 && Y>95 ) 

						return true ;

				else	if( 	 PadNbr == 4 
									&& X<96 && X>61
									&& Y<100 && Y>65 ) 

						return true ;

				else	if( 	 PadNbr == 5 
									&& X<67 && X>30 
									&& Y<100 && Y>65) 

						return true ;

				else	if( 	 PadNbr == 6 
									&& X<67 && X>30 
									&& Y<128 && Y>95 ) 

						return true ;

				else	if( 	 PadNbr == 7 
									&& X<35 && X>6 
									&& Y<100 && Y>65 ) 

						return true ;

				else	if( 	 PadNbr == 8 
									&& X<35 && X>6 
									&& Y<128 && Y>95 ) 

						return true ;

				else	if( 	 PadNbr == 9 
									&& X<121 && X>93 
									&& Y<31 && Y>1 ) 

						return true ;

				else	if( 	 PadNbr == 10 
									&& X<121 && X>93 
									&& Y<60 && Y>26 ) 

						return true ;

				else	if( 	 PadNbr == 11 
									&& X<96 && X>61
									&& Y<31 && Y>1 ) 

						return true ;

				else	if( 	 PadNbr == 12 
									&& X<96 && X>61
									&& Y<60 && Y>26) 

						return true ;

				else	if( 	 PadNbr == 13 
									&& X<67 && X>30 
									&& Y<60 && Y>26 ) 

						return true ;

				else	if( 	 PadNbr == 14 
									&& X<67 && X>30 
									&& Y<31 && Y>1 ) 

						return true ;

				else	if( 	 PadNbr == 15 
									&& X<35 && X>6
									&& Y<60 && Y>26 ) 

						return true ;

				else	if( 	 PadNbr == 16 
									&& X<35 && X>6
									&& Y<31 && Y>1 ) 

						return true ;		

				else
						return false;
	}


///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: Match_Si_CsI(int X, int Y , int CristalNbr)
	{
							if( 	 CristalNbr == 1 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;


				else	if( 	 CristalNbr == 2 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;


				else	if( 	 CristalNbr == 3 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 4 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 5 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 6 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 7 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 8 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 9 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 10 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 11 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 12 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 13 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 14 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 15 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

						return true ;

				else	if( 	 CristalNbr == 16 
									&& X<1 && X>1 
									&& Y<1 && Y>1 ) 

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



