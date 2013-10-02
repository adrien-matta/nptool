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

ClassImp(TMust2Physics)

TMust2Physics::TMust2Physics() 
	{ EventMultiplicity = 0 ;}

TMust2Physics::~TMust2Physics() {Clear();}

void TMust2Physics::BuildSimplePhysicalEvent(TMust2Data* Data)
	{
		BuildPhysicalEvent(Data);
	}
void TMust2Physics::BuildPhysicalEvent(TMust2Data* Data)
	{ 
		//	Check
		bool Check_Si = false ;bool Check_SiLi = false ; bool Check_CsI = false ;
		
		// Threshold
		double Si_X_E_Threshold = 0		;	double Si_X_T_Threshold = 0 	;
		double Si_Y_E_Threshold = 0		;	double Si_Y_T_Threshold = 0		;
		double SiLi_E_Threshold = 0		;	double SiLi_T_Threshold = 0		;
		double CsI_E_Threshold	= 0 	;	double CsI_T_Threshold	= 0		;
		
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
						TelescopeNumber.push_back(Data->GetMMStripXEDetectorNbr(0))	;
					
						//	Data->Get Max Energy
						if(Data->GetMMStripXEEnergy(0) > Data->GetMMStripYEEnergy(0))	Si_E.push_back( Data->GetMMStripXEEnergy(0) ) ;
						else															Si_E.push_back( Data->GetMMStripYEEnergy(0) ) ;
						
						//	Data->Get Min Time
						if(Data->GetMMStripXTTime(0) < Data->GetMMStripYTTime(0))		Si_T.push_back( Data->GetMMStripXTTime(0) ) ;
						else															Si_T.push_back( Data->GetMMStripYTTime(0) ) ;
						
						Si_X.push_back( Data->GetMMStripXEStripNbr(0) )	;
						Si_Y.push_back( Data->GetMMStripYEStripNbr(0) )	;	
						
						Check_Si = true ;			
						EventMultiplicity = 1;
								
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
							
					 
					 if      (!Check_SiLi && !Check_CsI ) TotalEnergy.push_back(   			     Si_E.at(0)					);
					 else if (Check_SiLi  && !Check_CsI ) TotalEnergy.push_back(               + Si_E.at(0) + SiLi_E.at(0)	);
					 else if (Check_CsI   && !Check_SiLi) TotalEnergy.push_back( CsI_E .at(0)  + Si_E.at(0)					);
					 else if (Check_CsI   &&  Check_SiLi) TotalEnergy.push_back( CsI_E .at(0)  + Si_E.at(0) + SiLi_E.at(0)	);
					 
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
												
												/*if (EX>EY)	Si_E.push_back(EX)	;
												else	  	Si_E.push_back(EY)	;*/ 
												Si_E.push_back(EX);
												
												
												if (TX>TY)	Si_T.push_back(TY)	;
												else	  	Si_T.push_back(TX)	; 
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
													SiLi_E.push_back(Data->GetMMSiLiEEnergy(i))	;
													SiLi_N.push_back(Data->GetMMSiLiEPadNbr(i))	;
													
													if (	Data->GetMMSiLiTDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
														&&	Data->GetMMSiLiTPadNbr(i)      == Data->GetMMSiLiEPadNbr(i) )
															{
															SiLi_T.push_back(Data->GetMMSiLiTTime(i))	;
															Check_SiLi = true ;
															}
													}
													
												else
													{
													SiLi_E.push_back(-1)	;
													SiLi_T.push_back(-1)	;
													SiLi_N.push_back(-1)	;
													}
											}
								
										//	CsI
										for (int i = 0 ; i < Data->GetMMCsIEMult() ; i++)
											{
												if (	Data->GetMMCsIEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
													&&	Data->GetMMCsIEEnergy(i) > CsI_E_Threshold	)
													{
													CsI_E.push_back(Data->GetMMCsIEEnergy(i))		;
													CsI_N.push_back(Data->GetMMCsIECristalNbr(i))	;
													if (	Data->GetMMCsITDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
														&&	Data->GetMMCsITCristalNbr(i)  == Data->GetMMCsIECristalNbr(i) )
															{
															CsI_T.push_back(Data->GetMMCsITTime(i))		;
															Check_CsI = true ;
															}
													}
													
												else
													{
													CsI_E.push_back(-1)	;
													CsI_T.push_back(-1)	;
													CsI_N.push_back(-1)	;
													}
											}
										
										TotalEnergy.push_back(Si_E.at(jj)) ;
										if (Check_SiLi) TotalEnergy.at(jj) += SiLi_E.at(jj)	;
										if (Check_CsI)  TotalEnergy.at(jj) += CsI_E.at(jj)	;
									}	
							}
						return;	
					}
					
				//	Same detector case :
				if(		Data->GetMMStripXEDetectorNbr(0) == Data->GetMMStripXEDetectorNbr(1)
					&&	Data->GetMMStripYEDetectorNbr(0) == Data->GetMMStripYEDetectorNbr(1))
					{
					
					
					return;
					}
			}
			
	}


void TMust2Physics::Clear()
	{
		EventMultiplicity= 0		;
		TelescopeNumber	.clear()	;
		EventType		.clear()	;
		TotalEnergy		.clear()	;
		
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

