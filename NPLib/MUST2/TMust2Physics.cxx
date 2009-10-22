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

///////////////////////////////////////////////////////////////////////////

ClassImp(TMust2Physics)
///////////////////////////////////////////////////////////////////////////
TMust2Physics::TMust2Physics() 
	{ EventMultiplicity = 0 ;}
	
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::BuildSimplePhysicalEvent(TMust2Data* Data)
	{ BuildPhysicalEvent(Data); }
	
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
						
						double Si_X_E = fSi_X_E(Data , couple[i].X())	;
						double Si_Y_E = fSi_Y_E(Data , couple[i].Y())	;

						double Si_X_T = fSi_X_T(Data , couple[i].X())		;
						double Si_Y_T = fSi_Y_T(Data , couple[i].Y())		;
					
						Si_X.push_back(X) ; Si_Y.push_back(Y) ; TelescopeNumber.push_back(N) ;
						
						// Take maximum Energy
						if(Si_X_E >= Si_Y_E) Si_E.push_back(Si_X_E)	;
						else								 Si_E.push_back(Si_Y_E)	;
						
						// Take minimum Time
						if(Si_X_T >= Si_Y_T) Si_T.push_back(Si_Y_T)	;
						else								 Si_T.push_back(Si_X_T)	;
						
						for(unsigned int j = 0 ; j < Data->GetMMSiLiEMult() ; j++)
							{
								if(Data->GetMMSiLiEDetectorNbr(j)==N)
									{
										//	if SiLi energy is above threshold check the compatibility
										if( fSiLi_E(Data , j)>SiLi_E_Threshold )
											{
												if( Match_Si_SiLi( X, Y , Data->GetMMSiLiEPadNbr(j) ) )
												{
													SiLi_N.push_back(Data->GetMMSiLiEPadNbr(j))	;
													SiLi_E.push_back(fSiLi_E(Data , j))	;
													SiLi_T.push_back(fSiLi_T(Data , j))		;
												}
											}
									}
							}
							
						 for( int j = 0 ; j < Data->GetMMCsIEMult() ; j++)
							{
								if(Data->GetMMCsIEDetectorNbr(j)==N)
									{
										//	ifCsI energy is above threshold check the compatibility
										if( fCsI_T(Data , j)>CsI_E_Threshold )
											{
												if( Match_Si_CsI( X, Y , Data->GetMMCsIECristalNbr(j) ) )
													{
														CsI_N.push_back(Data->GetMMCsIECristalNbr(j))	;
														CsI_E.push_back(fCsI_E(Data , j))			;
														CsI_T.push_back(fCsI_T(Data , j))				;
													}
											}
									}
							}
					}
			}
		
		return;
	
	}	

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
				//	if X value is above threshold, look at Y value
				if( fSi_X_E(Data , i) > Si_X_E_Threshold )
					{
					
						for(int j = 0 ; j < Data->GetMMStripYEMult(); j++)
							{
								//	if Y value is above threshold look if detector match
								if( fSi_Y_E(Data , j) > Si_Y_E_Threshold )							
									{
										//	if same detector check energy
										if ( Data->GetMMStripXEDetectorNbr(i) == Data->GetMMStripYEDetectorNbr(j) )
											{
													//	Look if energy match
													if( ( fSi_X_E(Data , i) - fSi_Y_E(Data , j) ) / fSi_X_E(Data , i) < 0.1	)
														ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;	
											}
									}
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
		double fSi_X_E(TMust2Data* Data , int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/" + itoa( Data->GetMMStripXEDetectorNbr(i) ) + "Si_X" + itoa( Data->GetMMStripXEStripNbr(i) ) + "_E",	
																																		Data->GetMMStripXEEnergy(i) );
																																	
			}
			
		double fSi_X_T(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMStripXTDetectorNbr(i) ) + "Si_X" + itoa( Data->GetMMStripXTStripNbr(i) ) +"_T",	
																																		Data->GetMMStripXTTime(i) );
			}
		
		//	Y	
		double fSi_Y_E(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMStripYEDetectorNbr(i) ) + "Si_Y" + itoa( Data->GetMMStripYEStripNbr(i) ) +"_E",	
																																		Data->GetMMStripYEEnergy(i) );
			}
			
		double fSi_Y_T(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMStripYTDetectorNbr(i) ) + "Si_Y" + itoa( Data->GetMMStripYTStripNbr(i) ) +"_T",	
																																		Data->GetMMStripYTTime(i) );
			}
			
			
		//	SiLi
		double fSiLi_E(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMSiLiEDetectorNbr(i) ) + "SiLi" + itoa( Data->GetMMSiLiEPadNbr(i) ) +"_E",	
																																		Data->GetMMSiLiEEnergy(i) );
			}
			
		double fSiLi_T(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMSiLiTDetectorNbr(i) ) + "SiLi" + itoa( Data->GetMMSiLiTPadNbr(i) )+"_T",	
																																		Data->GetMMSiLiTTime(i) );
			}
			
		//	CsI
		double fCsI_E(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMCsIEDetectorNbr(i) ) + "SiLi" + itoa( Data->GetMMCsIECristalNbr(i) ) +"_E",	
																																		Data->GetMMCsIEEnergy(i) );
			}
			
		double fCsI_T(TMust2Data* Data, int i)
			{
				return CalibrationManager::getInstance()->ApplyCalibration(	"MUST2/T" + itoa( Data->GetMMCsITDetectorNbr(i) ) + "SiLi" + itoa( Data->GetMMCsITCristalNbr(i) ) +"_T",	
																																		Data->GetMMCsITTime(i) );
			}
	
	}

