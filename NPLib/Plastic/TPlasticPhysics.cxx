/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Plastic  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//	NPL
#include "TPlasticPhysics.h"
#include "../include/RootOutput.h"
#include "../include/RootInput.h"

//	STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;

//	ROOT
#include "TChain.h"

//	tranform an integer to a string
		string itoa(int value)
			{
			  std::ostringstream o;
			
			  if (!(o << value))
			    return ""	;
			    
			  return o.str();
			}

ClassImp(TPlasticPhysics)
///////////////////////////////////////////////////////////////////////////
TPlasticPhysics::TPlasticPhysics()
	{		
		NumberOfDetector = 0 					;
		EventData = new TPlasticData	;
		EventPhysics = this						;
	}
	
///////////////////////////////////////////////////////////////////////////
TPlasticPhysics::~TPlasticPhysics()
	{}
	
///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::Clear()
	{
		DetectorNumber	.clear()	;
		Energy					.clear()	;
		Time						.clear()	;
	}
	
///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::ReadConfiguration(string Path) 
	{
		ifstream ConfigFile           ;
		ConfigFile.open(Path.c_str()) ;
		string LineBuffer          ;
		string DataBuffer          ;

		double Theta = 0 , Phi = 0 , R = 0 , Thickness = 0 , Radius = 0 , LeadThickness = 0;
		string Scintillator ;

		bool check_Theta = false   ;
		bool check_Phi  = false  ;
		bool check_R     = false   ;
		bool check_Thickness = false  		;
		bool check_Radius = false  			;
		bool check_LeadThickness = false		;
		bool check_Scintillator = false		;
		bool ReadingStatus = false ;
		

	 while (!ConfigFile.eof()) 
	 	{
	      
		      	getline(ConfigFile, LineBuffer);

				//	If line is a Start Up Plastic bloc, Reading toggle to true      
		      	if (LineBuffer.compare(0, 7, "Plastic") == 0) 
			      	{
			        	 cout << "///" << endl           ;
			       		  cout << "Platic found: " << endl   ;        
			        	 ReadingStatus = true ;
			        	
				   	}
				
				//	Else don't toggle to Reading Block Status
				else ReadingStatus = false ;
				
				//	Reading Block
				while(ReadingStatus)
					{
							// Pickup Next Word 
						ConfigFile >> DataBuffer ;

						//	Comment Line 
						if (DataBuffer.compare(0, 1, "%") == 0) {	ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

							//	Finding another telescope (safety), toggle out
						else if (DataBuffer.compare(0, 6, "Plastic") == 0) {
							cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
							ReadingStatus = false ;
						}
											
						//Angle method
						else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
							check_Theta = true;
							ConfigFile >> DataBuffer ;
							Theta = atof(DataBuffer.c_str()) ;
						}

						else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
							check_Phi = true;
							ConfigFile >> DataBuffer ;
							Phi = atof(DataBuffer.c_str()) ;
						}

						else if (DataBuffer.compare(0, 2, "R=") == 0) {
							check_R = true;
							ConfigFile >> DataBuffer ;
							R = atof(DataBuffer.c_str()) ;
						}
						
						else if (DataBuffer.compare(0, 7, "Radius=") == 0) {
							check_Radius = true;
							ConfigFile >> DataBuffer ;
							Radius = atof(DataBuffer.c_str()) ;
						}
						
						else if (DataBuffer.compare(0, 10, "Thickness=") == 0) {
							check_Thickness = true;
							ConfigFile >> DataBuffer ;
							Thickness = atof(DataBuffer.c_str()) ;
						}
						
						else if (DataBuffer.compare(0, 13, "Scintillator=") == 0) {
							check_Scintillator = true ;
							ConfigFile >> DataBuffer ;
							Scintillator = DataBuffer ;
						}
						
						else if (DataBuffer.compare(0, 14, "LeadThickness=") == 0) {
							check_LeadThickness = true;
							ConfigFile >> DataBuffer ;
							LeadThickness = atof(DataBuffer.c_str()) ;
						}
				      
				         	///////////////////////////////////////////////////
							//	If no Detector Token and no comment, toggle out
				         else 
				         	{ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
				         
				         	/////////////////////////////////////////////////
				         	//	If All necessary information there, toggle out
				         
				         if ( check_Theta && check_Phi && check_R && check_Thickness && check_Radius && check_LeadThickness && check_Scintillator) 
				         	{ 
			         		  NumberOfDetector++;
						         
						        //	Reinitialisation of Check Boolean 
						        
								check_Theta = false   			;
								check_Phi  = false  			;
								check_R     = false   			;
								check_Thickness = false  		;
								check_Radius = false  			;
								check_LeadThickness = false		;
								check_Scintillator = false 		;
								ReadingStatus = false 			;	
								cout << "///"<< endl ;	         
				         	}
				         	
					}
		}
	}


///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::AddParameterToCalibrationManager()
	{
		CalibrationManager* Cal = CalibrationManager::getInstance();
		
		for(int i = 0 ; i < NumberOfDetector ; i++)
			{
				for( int j = 0 ; j < 16 ; j++)
					{
						Cal->AddParameter("Plastic", "Detector"+itoa(i+1)+"_E","Plastic_Detector"+itoa(i+1)+"_E")	;
						Cal->AddParameter("Plastic", "Detector"+itoa(i+1)+"_T","Plastic_Detector"+itoa(i+1)+"_T")	;	
					}
		
			}
	}
	
///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::InitializeRootInput()
	{
		TChain* inputChain = RootInput::getInstance()->GetChain()			;
		inputChain->SetBranchStatus ( "Plastic" 		, true )					;
		inputChain->SetBranchStatus ( "fPlastic_*" 	, true )					;
		inputChain->SetBranchAddress( "Plastic" 		, &EventData )		;
	}	

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::InitializeRootOutput()
	{
		TTree* outputTree = RootOutput::getInstance()->GetTree()						;
		outputTree->Branch( "Plastic" , "TPlasticPhysics" , &EventPhysics )	;
	}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::BuildPhysicalEvent()
	{
		BuildSimplePhysicalEvent()	;
	}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::BuildSimplePhysicalEvent()
	{
				for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
					{
					
						DetectorNumber	.push_back( EventData->GetPlasticNumber(i) )	;
						
						Energy					.push_back(
							CalibrationManager::getInstance()->ApplyCalibration(	"Plastic/Detector" + itoa( EventData->GetPlasticNumber(i) ) +"_E",	
																																		EventData->GetEnergy(i) )
																			)	;
																			
						Time						.push_back(
							CalibrationManager::getInstance()->ApplyCalibration(	"Plastic/Detector" + itoa( EventData->GetPlasticNumber(i) ) +"_T",	
																																		EventData->GetTime(i) )
																			)	;
					}

	}

