/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra GIRON  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : april 2011                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold S2    treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TS2Physics.h"
using namespace S2_LOCAL;
	
//	STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//	NPL
#include "../include/RootInput.h"
#include "../include/RootOutput.h"

//	ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TS2Physics)
///////////////////////////////////////////////////////////////////////////
TS2Physics::TS2Physics() 
{ 
  EventMultiplicity 	= 0 		;
  m_EventData 		= new TS2Data	;
  m_PreTreatedData	= new TS2Data	;
  m_EventPhysics 	= this		;
  
  m_NumberOfAnnular	= 0	        ;
  m_NumberOfStripsTheta = 64            ; 
  m_NumberOfStripsPhi   = 16            ;
  m_NumberOfQuadrants   = 4             ;
    
  m_MaximumStripMultiplicityAllowed = 10       ;
  m_StripEnergyMatchingSigma        = 0.020    ;  // (in MeV) be careful to redefine if calibrated energy are in keV
  m_StripEnergyMatchingNumberOfSigma= 30       ; 

  // Raw Threshold
  m_S2_Theta_E_RAW_Threshold = 8200	;
  m_S2_Phi_E_RAW_Threshold   = 8200	;

  // Calibrated Threshold
  m_S2_Theta_E_Threshold     = 0	;
  m_S2_Phi_E_Threshold       = 0	;
  m_S2_Theta_E_Threshold_sup = 100000	;
  m_S2_Phi_E_Threshold_sup   = 100000	;

 		
  m_Take_E_Phi=false;
  m_Take_T_Phi=true;
		
}

		
///////////////////////////////////////////////////////////////////////////
void TS2Physics::BuildSimplePhysicalEvent()
{ 
  
  PreTreat();
  /*
  //	Theta
  //	E
  for(int i = 0 ; i < m_PreTreatedData->GetS2ThetaEMult() ; i++)
    {
      NTheta   .push_back(  m_PreTreatedData->GetS2ThetaEDetectorNbr(i) )	;
      StrTheta .push_back(  m_PreTreatedData->GetS2ThetaEStripNbr(i) )	;
      ETheta   .push_back(  m_PreTreatedData->GetS2ThetaEEnergy(i))	;

      // T
      for(int j = 0 ; j < m_PreTreatedData->GetS2ThetaTMult() ; j++)
	{
	  if(m_PreTreatedData->GetS2ThetaEDetectorNbr(i) == m_PreTreatedData->GetS2ThetaTDetectorNbr(j) && m_PreTreatedData->GetS2ThetaEStripNbr(i) == m_PreTreatedData->GetS2ThetaTStripNbr(j))
	    TTheta.push_back( m_PreTreatedData->GetS2ThetaTTime(j)	)	;
	}
    }
				
  //	Phi
  //	E
  for(int i = 0 ; i < m_PreTreatedData->GetS2PhiEMult() ; i++)
    {
      NPhi.push_back(m_PreTreatedData->GetS2PhiEDetectorNbr(i) )	;
      StrPhi .push_back(  m_PreTreatedData->GetS2PhiEStripNbr(i) )		;
      EPhi    .push_back(  m_PreTreatedData->GetS2PhiEEnergy(i)	)	;
      
      //	T
      for(int j = 0 ; j < m_PreTreatedData->GetS2PhiTMult() ; j++)
	{
	  if(m_PreTreatedData->GetS2PhiEDetectorNbr(i) ==  m_PreTreatedData->GetS2PhiTDetectorNbr(j) &&  m_PreTreatedData->GetS2PhiEStripNbr(i) == m_PreTreatedData->GetS2PhiTStripNbr(j))
	    TPhi .push_back(  m_PreTreatedData->GetS2PhiTTime(j)	)	;
	}
    }
  
  return;
  */
}
	
///////////////////////////////////////////////////////////////////////////
	
void TS2Physics::BuildPhysicalEvent()
{
  PreTreat();
  
  
   //   if( CheckEvent() == 1 )
     {
   vector< TVector2 > couple = Match_Theta_Phi() ;
   EventMultiplicity = couple.size();

   // cout << EventMultiplicity << endl;

   for(unsigned int i = 0 ; i < couple.size() ; i++)
     {
       //  cout << "CheckEvent = 1, Multiplicity = " << EventMultiplicity << endl;
       
       int N = m_PreTreatedData->GetS2ThetaEDetectorNbr(couple[i].X()) 	;
       
       int Theta = m_PreTreatedData->GetS2ThetaEStripNbr(couple[i].X())		;
       int Phi = m_PreTreatedData->GetS2PhiEStripNbr(couple[i].Y())			;
						
       double S2_Theta_E = m_PreTreatedData->GetS2ThetaEEnergy( couple[i].X() ) 	;
       double S2_Phi_E   = m_PreTreatedData->GetS2PhiEEnergy( couple[i].Y() ) 	;  
       
       //cout << N << " " << Theta << " " << Phi << " " << S2_Theta_E << " " << S2_Phi_E << endl;
       
       //  Search for associate Time
       double S2_Theta_T = -1000 ;
       for(unsigned int t = 0 ; t < m_PreTreatedData->GetS2ThetaTMult() ; t++ )
	 {
	   if(  m_PreTreatedData->GetS2ThetaTStripNbr( couple[i].X() ) == m_PreTreatedData->GetS2ThetaTStripNbr(t)
		|| m_PreTreatedData->GetS2ThetaTDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetS2ThetaTDetectorNbr(t))
	     { 
	       S2_Theta_T = m_PreTreatedData->GetS2ThetaTTime(t);
	       // cout <<  "Theta_T = " << S2_Theta_T << endl;
	     }
	 }
       
       double S2_Phi_T = -1000 ;
       for(unsigned int t = 0 ; t < m_PreTreatedData->GetS2PhiTMult() ; t++ )
	 {
	   if(  m_PreTreatedData->GetS2PhiTStripNbr( couple[i].Y() ) == m_PreTreatedData->GetS2PhiTStripNbr(t)
		   || m_PreTreatedData->GetS2PhiTDetectorNbr( couple[i].Y() ) == m_PreTreatedData->GetS2PhiTDetectorNbr(t)) 
	     {
	       S2_Phi_T = m_PreTreatedData->GetS2PhiTTime(t);
	       // cout <<  "Phi_T  = " << S2_Phi_T << endl;
	     }
	 }
       
       Si_Theta.push_back(Theta) ; Si_Phi.push_back(Phi) ; AnnularNumber.push_back(N) ;
       
       if(m_Take_E_Phi) Si_E.push_back(S2_Phi_E);
       else             Si_E.push_back(S2_Theta_E);
       
       if(m_Take_T_Phi) Si_T.push_back(S2_Phi_T)	;
       else             Si_T.push_back(S2_Theta_T)	;
       
       // Store the other value for checking purpose
       Si_ETheta.push_back(S2_Theta_E);				;
       Si_TTheta.push_back(S2_Theta_T);				;
       
       Si_EPhi.push_back(S2_Phi_E);				;
       Si_TPhi.push_back(S2_Phi_T);				;
       
       //  cout << N << " " << Theta << " " << Phi << " " << S2_Theta_E << " " << S2_Phi_E << endl;
       
        }  // check event
     }

  
	
}	

///////////////////////////////////////////////////////////////////////////
void TS2Physics::PreTreat()
{
  ClearPreTreatedData();

  //	Theta
  //	E
  for(int i = 0 ; i < m_EventData->GetS2ThetaEMult() ; i++)
    {
      
       if(m_EventData->GetS2ThetaEEnergy(i)> m_S2_Theta_E_RAW_Threshold && IsValidChannel("Theta", m_EventData->GetS2ThetaEDetectorNbr(i), m_EventData->GetS2ThetaEStripNbr(i)) )
	{
	  double ETheta = fS2_Theta_E(m_EventData , i); 
	 
	  if( ETheta > m_S2_Theta_E_Threshold && ETheta < m_S2_Theta_E_Threshold_sup)
	    {
	      m_PreTreatedData->SetS2ThetaEDetectorNbr( m_EventData->GetS2ThetaEDetectorNbr(i) )	;
	      m_PreTreatedData->SetS2ThetaEStripNbr( m_EventData->GetS2ThetaEStripNbr(i) )				;
	      m_PreTreatedData->SetS2ThetaEEnergy( ETheta )																					;
	    }
	}
    }
				
  //	T
  for(int i = 0 ; i < m_EventData->GetS2ThetaTMult() ; i++)
    {
      if(IsValidChannel("Theta", m_EventData->GetS2ThetaTDetectorNbr(i), m_EventData->GetS2ThetaTStripNbr(i)))
	{
	  m_PreTreatedData->SetS2ThetaTDetectorNbr( m_EventData->GetS2ThetaTDetectorNbr(i) )	;
	  m_PreTreatedData->SetS2ThetaTStripNbr( m_EventData->GetS2ThetaTStripNbr(i) )				;
	  m_PreTreatedData->SetS2ThetaTTime( fS2_Theta_T(m_EventData , i) )												;
	}
    }
				
				
  //	Phi
  //	E
  for(int i = 0 ; i < m_EventData->GetS2PhiEMult() ; i++)
    {
        if( m_EventData->GetS2PhiEEnergy(i)< m_S2_Phi_E_RAW_Threshold && IsValidChannel("Phi", m_EventData->GetS2PhiEDetectorNbr(i), m_EventData->GetS2PhiEStripNbr(i)))
	{
	  double EPhi = fS2_Phi_E(m_EventData , i); 
	
	  if( EPhi > m_S2_Phi_E_Threshold && EPhi < m_S2_Phi_E_Threshold_sup)
	    {
	      m_PreTreatedData->SetS2PhiEDetectorNbr( m_EventData->GetS2PhiEDetectorNbr(i) )	;
	      m_PreTreatedData->SetS2PhiEStripNbr( m_EventData->GetS2PhiEStripNbr(i) )				;
	      m_PreTreatedData->SetS2PhiEEnergy( EPhi )																					;
	    }
	}
    }
				
  //	T
  for(int i = 0 ; i < m_EventData->GetS2PhiTMult() ; i++)
    {
       if(IsValidChannel("Phi", m_EventData->GetS2PhiTDetectorNbr(i), m_EventData->GetS2PhiTStripNbr(i)))
	{
	  m_PreTreatedData->SetS2PhiTDetectorNbr( m_EventData->GetS2PhiTDetectorNbr(i) )	;
	  m_PreTreatedData->SetS2PhiTStripNbr( m_EventData->GetS2PhiTStripNbr(i) )				;
	  m_PreTreatedData->SetS2PhiTTime( fS2_Phi_T(m_EventData , i) )												;
	}
    }

//  if (m_EventData->GetS2ThetaEEnergy(0)) {
  m_EventData->Dump();
  m_PreTreatedData->Dump();
//  }
	
  return;
}


///////////////////////////////////////////////////////////////////////////
int TS2Physics :: CheckEvent()
{
  // m_PreTreatedData -> Dump();

  // Check the size of the different elements
  if(m_PreTreatedData->GetS2ThetaEMult() == m_PreTreatedData->GetS2PhiEMult() )
     // && m_PreTreatedData->GetS2PhiEMult() == m_PreTreatedData->GetS2ThetaTMult() &&  m_PreTreatedData->GetS2ThetaTMult() == m_PreTreatedData->GetS2PhiTMult())
    {
      //      cout << "CHECK 1" << endl;
    return 1 ; // Regular Event
    }

  else if(m_PreTreatedData->GetS2ThetaEMult() == m_PreTreatedData->GetS2PhiEMult()+1 || m_PreTreatedData->GetS2ThetaEMult() == m_PreTreatedData->GetS2PhiEMult()-1  )
    {
      //      cout << "CHECK 2" << endl;
      return 2 ; // Pseudo Event, potentially interstrip
    }
		
  else 	{
    //    cout << "CHECK -1" << endl;
    return -1 ; // Rejected Event
  }
  
}

///////////////////////////////////////////////////////////////////////////
bool TS2Physics :: ResolvePseudoEvent()
{
  return false;
}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TS2Physics :: Match_Theta_Phi()
{
 
  vector < TVector2 > ArrayOfGoodCouple ;
  
  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_PreTreatedData->GetS2ThetaEMult() > m_MaximumStripMultiplicityAllowed || m_PreTreatedData->GetS2PhiEMult() > m_MaximumStripMultiplicityAllowed )
    {
      // cout << "too high multiplicity : ThetaEMult = " <<  m_PreTreatedData->GetS2ThetaEMult()<< " PhiEMult  = " <<  m_PreTreatedData->GetS2PhiEMult() << endl;
      return ArrayOfGoodCouple;
    }
    
  for(int i = 0 ; i < m_PreTreatedData->GetS2ThetaEMult(); i++)
    {
      for(int j = 0 ; j < m_PreTreatedData->GetS2PhiEMult(); j++)
	{
	  //	if same detector check energy
	  
	  // cout << "inside loop" <<endl;

	  if ( m_PreTreatedData->GetS2ThetaEDetectorNbr(i) == m_PreTreatedData->GetS2PhiEDetectorNbr(j) )
	    {
	      if(Match_Theta_Phi_Position(m_PreTreatedData->GetS2ThetaEStripNbr(i), m_PreTreatedData->GetS2PhiEStripNbr(j)))
		{
		  //cout << "new condition : same detector phi and theta " << endl;
		  
		  //	Look if energy match
		  double ETheta = m_PreTreatedData->GetS2ThetaEEnergy(i) ;
		  double EPhi   = m_PreTreatedData->GetS2PhiEEnergy(j) ;
		  double mean   = ( ETheta + EPhi ) /2. ;
		  double distTheta = abs(ETheta-mean);
		  double distPhi   = abs(EPhi-mean);
		  
		  //  cout << ETheta << " " << EPhi << " " << mean << " " << distTheta << " " << distPhi << endl;
		  
		  // cout << m_StripEnergyMatchingNumberOfSigma << " " << m_StripEnergyMatchingSigma << endl;
		  // cout << m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma << endl;
		  
		  if( distTheta < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma && distPhi< m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma )
		    {
		      ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;	
		      //		  cout << "same energy theta phi" << endl;
		    }
		}
	    }
	}
    }
	
  //	Prevent to treat event with ambiguous matching beetween Theta and Phi
  if( ArrayOfGoodCouple.size() > m_PreTreatedData->GetS2ThetaEMult() ) 
    {
      // cout << "clear event at the end"<< endl;;
	ArrayOfGoodCouple.clear() ;
    }
  
  return ArrayOfGoodCouple;	
}
	
bool TS2Physics::Match_Theta_Phi_Position(int theta, int phi)
{
  bool status = false;

  if(phi > 2 && phi < 7)
    {
      if(theta > 48 && theta < 65)  status = true;
    }
  
  else if(phi > 6 && phi < 11)
    {
      if(theta > 16 && theta < 33)  status = true;
    }
  
  else if(phi > 10 && phi < 15)
    {
      if(theta > 0 && theta < 16)  status = true;
    }

  else if((phi > 14 && phi < 17) || (phi > 0 && phi < 3))
    {
      if(theta > 32 && theta < 49)  status = true;
    }

  // if(status == true)  cout << theta << " " << phi << endl;

  return status;
}
	
////////////////////////////////////////////////////////////////////////////
bool TS2Physics :: IsValidChannel(string DetectorType,  int telescope , int channel)
{
  vector<bool>::iterator it ;
  if(DetectorType == "Theta")
    return *(m_ThetaChannelStatus[telescope-1].begin()+channel-1);
			
  else if(DetectorType == "Phi")
    return *(m_PhiChannelStatus[telescope-1].begin()+channel-1);
			
  else return false;
}
	
///////////////////////////////////////////////////////////////////////////
void TS2Physics::ReadAnalysisConfig()
{
  
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigS2.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigS2.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigS2.dat " << endl;

  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 8, "ConfigS2") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {

      whatToDo="";
      AnalysisConfigFile >> whatToDo;
         
      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
	AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }
         
      //         else if (whatToDo.compare(0, 22, "MAX_STRIP_MULTIPLICITY") == 0) {
      else if (whatToDo=="MAX_STRIP_MULTIPLICITY") { 
	AnalysisConfigFile >> DataBuffer;
	m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
	cout << "MAXIMUM STRIP MULTIPLICITY " << m_MaximumStripMultiplicityAllowed << endl;
      }
         
      //         else if (whatToDo.compare(0, 27, "STRIP_ENERGY_MATCHING_SIGMA") == 0) {
      else if (whatToDo=="STRIP_ENERGY_MATCHING_SIGMA") {
	AnalysisConfigFile >> DataBuffer;
	m_StripEnergyMatchingSigma = atof(DataBuffer.c_str() );
	cout << "STRIP ENERGY MATCHING SIGMA " << m_StripEnergyMatchingSigma <<endl;
      }
         
      else if (whatToDo=="STRIP_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
	AnalysisConfigFile >> DataBuffer;
	m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str() );
	cout << "STRIP ENERGY MATCHING NUMBER OF SIGMA " << m_StripEnergyMatchingNumberOfSigma << endl;
      }
         
      else if (whatToDo== "DISABLE_ALL") {
	AnalysisConfigFile >> DataBuffer;
	cout << whatToDo << "  " << DataBuffer << endl;
	int telescope = atoi(DataBuffer.substr(1,1).c_str());
	vector< bool > ChannelStatus;
	ChannelStatus.resize(64,false);
	m_ThetaChannelStatus[telescope-1] = ChannelStatus;
	ChannelStatus.resize(16,false);
	m_PhiChannelStatus[telescope-1] = ChannelStatus;
      }
            
      else if (whatToDo == "DISABLE_CHANNEL") {
	AnalysisConfigFile >> DataBuffer;
	cout << whatToDo << "  " << DataBuffer << endl;
	int telescope = atoi(DataBuffer.substr(1,1).c_str());
	int channel = -1;
	if (DataBuffer.compare(2,5,"THETA") == 0) {
	  channel = atoi(DataBuffer.substr(7).c_str());
	  // cout << "disable channel theta " << telescope << " " << channel << endl;
	  *(m_ThetaChannelStatus[telescope-1].begin()+channel-1) = false;
	}
           
	else if (DataBuffer.compare(2,3,"PHI") == 0) {
	  channel = atoi(DataBuffer.substr(5).c_str());
	  //cout << "disable channel phi " << telescope << " " << channel << endl;
	  *(m_PhiChannelStatus[telescope-1].begin()+channel-1) = false;
	}
           
	else cout << "Warning: detector type for S2 unknown!" << endl;
       
      } 
            
      else if (whatToDo=="TAKE_E_Phi") {
	m_Take_E_Phi = true;
	cout << whatToDo << endl;
      } 
        
      else if (whatToDo=="TAKE_T_Phi") {
	m_Take_T_Phi = true;
	cout << whatToDo << endl;
      }
        
      else if (whatToDo=="TAKE_E_Theta") {
	m_Take_E_Phi = false;
	cout << whatToDo << endl;
      }
        
      else if (whatToDo=="TAKE_T_Theta") {
	m_Take_T_Phi = false;
	cout << whatToDo << endl;
      }
           
      else if (whatToDo=="S2_Theta_E_RAW_THRESHOLD") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Theta_E_RAW_Threshold = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Theta_E_RAW_Threshold << endl;
      }  
         
      else if (whatToDo=="S2_Phi_E_RAW_THRESHOLD") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Phi_E_RAW_Threshold = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Phi_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="S2_Theta_E_THRESHOLD") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Theta_E_Threshold = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Theta_E_Threshold << endl;
      }  
         
      else if (whatToDo== "S2_Phi_E_THRESHOLD") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Phi_E_Threshold = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Phi_E_Threshold << endl;
      }

      else if (whatToDo=="S2_Theta_E_THRESHOLD_SUP") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Theta_E_Threshold_sup = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Theta_E_Threshold_sup << endl;
      }  
         
      else if (whatToDo== "S2_Phi_E_THRESHOLD_SUP") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Phi_E_Threshold_sup = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Phi_E_Threshold_sup << endl;
      }

      else if (whatToDo=="S2_Theta_T_THRESHOLD") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Theta_T_Threshold = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Theta_T_Threshold << endl;
      }  
         
      else if (whatToDo== "S2_Phi_T_THRESHOLD") {
	AnalysisConfigFile >> DataBuffer;
	m_S2_Phi_T_Threshold = atoi(DataBuffer.c_str());
	cout << whatToDo << " " << m_S2_Phi_T_Threshold << endl;
      }
      
      else {
	ReadingStatus = false;
      }
       
    }
  }
}	
	

///////////////////////////////////////////////////////////////////////////
void TS2Physics::Clear()
{
  EventMultiplicity= 0		;
  
  AnnularNumber	.clear()	;	
  EventType	.clear()	;
  		
  // Si Theta
  
  Si_E.clear()	;
  Si_T.clear()	;
  Si_Theta.clear()	;
  Si_Phi.clear()	;
  	
  
  Si_ETheta.clear()	;
  Si_TTheta.clear()	;
  Si_EPhi.clear()	;
  Si_TPhi.clear()	;
  
  AnnularNumber_Theta.clear()	;
  AnnularNumber_Phi.clear()	;
  
  /*
  NTheta.clear()    ;
  StrTheta.clear()  ;
  ETheta.clear()     ;
  TTheta.clear()     ;
  
  NPhi.clear()    ;
  StrPhi.clear()  ;
  EPhi.clear()     ;
  TPhi.clear()     ;
  */
}
///////////////////////////////////////////////////////////////////////////

void TS2Physics::ReadCalibrationRun()
{
  //	Theta
  //	E	
  
  for(int i = 0 ; i < m_EventData->GetS2ThetaEMult();i++)
    {
      AnnularNumber_Theta.push_back(m_EventData->GetS2ThetaEDetectorNbr(i));
      Si_ETheta.push_back( fS2_Theta_E( m_EventData , i) )				;
      Si_Theta.push_back(m_EventData->GetS2ThetaEStripNbr(i));
    }

  //	T
  for(int i = 0 ; i < m_EventData->GetS2ThetaTMult();i++)
    {
      AnnularNumber_Theta.push_back(m_EventData->GetS2ThetaTDetectorNbr(i));
      Si_TTheta.push_back( fS2_Theta_T( m_EventData , i) )				;
      Si_Theta.push_back(m_EventData->GetS2ThetaTStripNbr(i));
    }
		
  //	Phi
  //	E
  for(int i = 0 ; i < m_EventData->GetS2PhiEMult();i++)
    {
      AnnularNumber_Phi.push_back(m_EventData->GetS2PhiEDetectorNbr(i));
      Si_EPhi.push_back( fS2_Phi_E( m_EventData , i) )				;
      Si_Phi.push_back(m_EventData->GetS2PhiEStripNbr(i));
    }
			
  //	T
  for(int i = 0 ; i < m_EventData->GetS2PhiTMult();i++)
    { 
      AnnularNumber_Phi.push_back(m_EventData->GetS2PhiTDetectorNbr(i));
      Si_TPhi.push_back( fS2_Phi_T( m_EventData , i) )				;
      Si_Phi.push_back(m_EventData->GetS2PhiTStripNbr(i));
    }
   		
}

////	Innherited from VDetector Class	////				
				
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TS2Physics::ReadConfiguration(string Path) 	
{ 
  ifstream ConfigFile           	;
  ConfigFile.open(Path.c_str()) 	;
  string LineBuffer          		;
  string DataBuffer          		;	

  // A:Theta1_Phi1     --> Theta:1    Phi:1
  // B:Theta128_Phi1   --> Theta:128  Phi:1
  // C:Theta1_Phi128   --> Theta:1    Phi:128
  // D:Theta128_Phi128 --> Theta:128  Phi:128

  double Centerx, Centery, Centerz;
  double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz           	;
  TVector3 A , B , C , D, Center                       				;
  //  double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0    ;
  
  bool check_Center = false 	;
  bool check_A = false 	;
  bool check_C = false  	;
  bool check_B = false 	;
  bool check_D = false  	;

  bool ReadingStatus = false ;
	

  while (!ConfigFile.eof()) 
    {
      
      getline(ConfigFile, LineBuffer);

      //	If line is a Start Up S2 bloc, Reading toggle to true      
      if (LineBuffer.compare(0, 11, "AnnularS2")==0) 
	{
	  cout << "///" << endl           		;
	  cout << "Annular found: " << endl   ;        
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
	  else if (DataBuffer=="AnnularS2") {
	    cout << "WARNING: Another Annular is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
	    ReadingStatus = false ;
	  }
			
	  //	Position method
	  else if (DataBuffer=="CalculatedCenter=") {
	    check_Center = true;
	    ConfigFile >> DataBuffer ;
	    Centerx = atof(DataBuffer.c_str()) ;
	    Centerx = Centerx  ;
	    ConfigFile >> DataBuffer ;
	    Centery = atof(DataBuffer.c_str()) ;
	    Centery = Centery  ;
	    ConfigFile >> DataBuffer ;
	    Centerz = atof(DataBuffer.c_str()) ;
	    Centerz = Centerz  ;

	    Center = TVector3(Centerx, Centery, Centerz);
	    cout << "Center position : (" << Center.X() << ";" << Center.Y() << ";" << Center.Z() << ")" << endl;
	  }         
				
	  else if (DataBuffer=="Phi2_Phi3=") {
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
	    cout << "Phi2 Phi3 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
		            
	  }


	  else if (DataBuffer=="Phi6_Phi7=") {
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
	    cout << "Phi6 Phi7 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
		            
	  }
		         

	  else if (DataBuffer=="Phi10_Phi11=") {
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
	    cout << "Phi10 Phi11 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
		           
	  }

	  else if (DataBuffer=="Phi14_Phi15=") {
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
	    cout << "Phi14 Phi15 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;
		           
	  }
			
	  //	End Position Method

	
	  /////////////////////////////////////////////////
	    //	If All necessary information there, toggle out
	    if ( (check_Center && check_A && check_B && check_C && check_D)) // || (check_Theta && check_Phi && check_R && check_beta)  ) 
	      { 
		ReadingStatus = false; 
		         	
		///Add The previously define telescope
		  //With position method
		  if ( check_Center && check_A && check_B && check_C && check_D ) 
		    {
		      AddAnnular(Center, 
				   A   ,
				   B   ,
				   C   ,
				   D   ) ;
		    }
		         	
		  //with angle method
		 
		  check_Center = false  ;
		  check_A = false 	;
		  check_B = false 	;
		  check_C = false  	;
		  check_D = false  	;
	      }
		         
	}
    }
  InitializeStandardParameter();        
  ReadAnalysisConfig();
	
  cout << endl << "/////////////////////////////" << endl << endl;

}

//	Add Parameter to the CalibrationManger
void TS2Physics::AddParameterToCalibrationManager()	
{
  CalibrationManager* Cal = CalibrationManager::getInstance();
		
  for(int i = 0 ; i < m_NumberOfAnnular ; i++)
    {
			
      for( int j = 0 ; j < 64 ; j++)
	{
	  Cal->AddParameter("S2", "T"+itoa(i+1)+"_Theta"+itoa(j+1)+"_E","S"+itoa(i+1)+"_THETA"+itoa(j+1)+"_E")	;
	  Cal->AddParameter("S2", "T"+itoa(i+1)+"_Theta"+itoa(j+1)+"_T","S"+itoa(i+1)+"_THETA"+itoa(j+1)+"_T")	;
	}

      for( int j = 0 ; j < 16 ; j++)
	{
	  Cal->AddParameter("S2", "T"+itoa(i+1)+"_Phi"+itoa(j+1)+"_E","S"+itoa(i+1)+"_PHI"+itoa(j+1)+"_E")	;
	  Cal->AddParameter("S2", "T"+itoa(i+1)+"_Phi"+itoa(j+1)+"_T","S"+itoa(i+1)+"_PHI"+itoa(j+1)+"_T")	;	
	}
    }
			
  return;
	
}

//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TS2Physics::InitializeRootInput() 		
{
  TChain* inputChain = RootInput::getInstance()->GetChain()	;
  inputChain->SetBranchStatus( "S2_ANNULAR" , true )				      ;
  inputChain->SetBranchStatus( "fS2_*" , true )				      ;
  inputChain->SetBranchAddress( "S2_ANNULAR" , &m_EventData )		  ;
}


//	Create associated branches and associated private member DetectorPhysics address
void TS2Physics::InitializeRootOutput() 	
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()		;
  outputTree->Branch( "S2" , "TS2Physics" , &m_EventPhysics )	;
}


/////	Specific to S2Array	////

void TS2Physics::AddAnnular(TVector3 C_Center           ,	
			    TVector3 C_Phi2_Phi3 	,
			    TVector3 C_Phi6_Phi7	, 
			    TVector3 C_Phi10_Phi11 	, 
			    TVector3 C_Phi14_Phi15	)
{
  
  // To avoid warning
  C_Phi2_Phi3 *= 1;
 
  m_NumberOfAnnular++;

  //	Geometry Parameter
  double rmin = 23.942; //with intern interstrip     //23990; without    // µm
  double rmax = 48.038; //with inter interstrip      //47990; without    // µm

  // Theta & phi strips pitch
  double thetaPitch = (rmax - rmin) / m_NumberOfStripsTheta * m_NumberOfQuadrants;
  double phiPitch   = 2*M_PI / m_NumberOfStripsPhi;

  //	Vector U on Annular Face (theta)
  TVector3 U = C_Phi2_Phi3 - C_Center 				;
  //  double Ushift = (U.Mag() -98)/2. ;
  U = U.Unit()			;

  //	Vector V on Annular Face (radius) 
  TVector3 V = C_Phi6_Phi7 - C_Center 				;	
  //double Vshift = (V.Mag()-98)/2. ;
  V = V.Unit()							;

  //  Coordinate system with ux vector on first quadrant
  U.RotateZ(2*phiPitch);
  V.RotateZ(2*phiPitch);

  
  //	Buffer object to fill Position Array
  vector<double> lineX ; 
  vector<double> lineY ; 
  vector<double> lineZ ;


  lineX.resize(64,-100);
  lineY.resize(64,-100);
  lineZ.resize(64,-100);

  
  vector< vector< double > >	OneAnnularStripPositionX	;
  vector< vector< double > >	OneAnnularStripPositionY	;
  vector< vector< double > >	OneAnnularStripPositionZ	;

  for (int i = 0; i < m_NumberOfStripsPhi; i++) 
    {
      OneAnnularStripPositionX.push_back(lineX);
      OneAnnularStripPositionY.push_back(lineY);
      OneAnnularStripPositionZ.push_back(lineZ);
    }

  ////////////////////////////////////////////////////////////////////////////
  // Construct strip mapping 
  //////////////////////////////////////////////////////////////////////////

  vector< vector<int> > Map;
  vector<int> line;
  line.resize(64,-1);

 for(int i = 0; i < m_NumberOfStripsPhi; i++)
    {
      Map.push_back(line);
    }

 for(int i = 0; i < m_NumberOfStripsTheta; i++)
    {
      int Strip_Theta_Number = i+1;

      for(int j = 0; j < m_NumberOfStripsPhi; j++)
	{
	  switch (j+1) 
	    {
	    case 1 : 
	    case 2 :
	    case 15:
	    case 16: if(Strip_Theta_Number <= 48 && Strip_Theta_Number > 32) Map[j][i] = 48 - Strip_Theta_Number ;
	      break;
	  
	    case 3 : 
	    case 4 :
	    case 5 :
	    case 6 : if(Strip_Theta_Number <= 64 && Strip_Theta_Number > 48) Map[j][i] =  64 - Strip_Theta_Number  ;
	      break;  
	  
	    case 7 : 
	    case 8 :
	    case 9 :
	    case 10: if(Strip_Theta_Number <= 32 && Strip_Theta_Number > 16) Map[j][i] =  32 - Strip_Theta_Number ;
	      break;  
	  
	    case 11: 
	    case 12:
	    case 13:
	    case 14: if( Strip_Theta_Number<= 16 && Strip_Theta_Number > 0) Map[j][i] =  Strip_Theta_Number - 1 ;
	      break; 
	    }

	  // cout << j+1 << " " << i+1 << " " <<  Map[j][i] << endl;
	}
    }

 //////////////////////////////////////////////////////////////////////////
  
  // loop on phi strips
 for(int phinb = 0; phinb <  m_NumberOfStripsPhi; phinb++)
   {
     double x = 0;
     double y = 0;
     // center of phi strips
     double phi = phiPitch/2 + phiPitch*phinb ;

     for(int thetanb = 0; thetanb <  m_NumberOfStripsTheta; thetanb++)
       {
	 int localtheta = Map[phinb][thetanb];
	 
	 if(localtheta != -1) 
	   {
	     // cout << "  cc  " <<thetanb+1 << " " <<phinb+1 << " "   << Map[phinb][thetanb] << endl;
	     TVector3 StripCenter = TVector3(0,0,0)                               ;
	     
	     double r = 0;
	     
	     r = rmax - thetaPitch/2 - thetaPitch*localtheta;

	     // calculate x and y projections
	     x = r * cos(phi);
	     y = r * sin(phi);
	     
	     StripCenter  = C_Center + ( x*U + y*V) ;
	     
	     lineX[thetanb] =  StripCenter.X();
	     lineY[thetanb] =  StripCenter.Y();
	     lineZ[thetanb] =  StripCenter.Z();
	     
	   }
       }
     
     OneAnnularStripPositionX[phinb] = lineX	;
     OneAnnularStripPositionY[phinb] = lineY	;
     OneAnnularStripPositionZ[phinb] = lineZ	;
     
   }

 m_StripPositionX.push_back(OneAnnularStripPositionX ) ;
 m_StripPositionY.push_back(OneAnnularStripPositionY ) ;
 m_StripPositionZ.push_back(OneAnnularStripPositionZ ) ;	

}



void TS2Physics::InitializeStandardParameter()
{
  //	Enable all channel
  vector< bool > ChannelStatus;
  m_ThetaChannelStatus.clear()    ;
  m_PhiChannelStatus.clear()    ;
      
  ChannelStatus.resize(64,true);
 
  for(int i = 0 ; i < m_NumberOfAnnular ; i ++)		
    {
      m_ThetaChannelStatus[i] = ChannelStatus;
    }
  
  ChannelStatus.resize(16,true);
  for(int i = 0 ; i < m_NumberOfAnnular ; i ++)		
    {
      m_PhiChannelStatus[i] = ChannelStatus;
    }
		
  m_MaximumStripMultiplicityAllowed = m_NumberOfAnnular	;
		
  return;
}	
	
void TS2Physics::Dump_Positions()
{
  double x = 0;
  double y = 0;
  double z = 0;

  for(int k = 0; k < m_NumberOfAnnular; k++)
    {
      for(int i = 0 ; i < m_NumberOfStripsPhi; i++)
	{
	  for(int j = 0; j < m_NumberOfStripsTheta; j++)
	    {
	      x = GetStripPositionX(k, i, j);
	      y = GetStripPositionY(k, i, j);
	      z = GetStripPositionZ(k, i, j);
	      
	      cout << k+1 << i+1 << " " << j+1 << " " << x << " " << y << " " << z << endl;
	    }
	}
    }

}
	
TVector3 TS2Physics::GetPositionOfInteraction(const int i) const 
{
  /*
  cout << endl;
  cout <<  AnnularNumber[i] << " " <<  Si_Phi[i] << " " << Si_Theta[i] << endl;
  cout << GetStripPositionX(AnnularNumber[i], Si_Phi[i] , Si_Theta[i] ) << " " << GetStripPositionY( AnnularNumber[i], Si_Phi[i] , Si_Theta[i] ) << " " << GetStripPositionZ( AnnularNumber[i], Si_Phi[i] , Si_Theta[i]) << endl;
  */  
  
  TVector3 Position = TVector3 (GetStripPositionX( AnnularNumber[i], Si_Phi[i] , Si_Theta[i] ) 	,
	 			GetStripPositionY( AnnularNumber[i], Si_Phi[i] , Si_Theta[i] )	,
				GetStripPositionZ( AnnularNumber[i], Si_Phi[i] , Si_Theta[i] )	) ;


  return(Position) ;	
  
}
	
TVector3 TS2Physics::GetAnnularNormal( const int i) const 
{
  TVector3 U = 	TVector3 (	GetStripPositionX( AnnularNumber[i] , 4 , 64 ) 	,
				GetStripPositionY( AnnularNumber[i] , 4 , 64 )		,
				GetStripPositionZ( AnnularNumber[i] , 4 , 64 )		)
											
           - 	TVector3 (GetStripPositionX( AnnularNumber[i] , 12 , 1 ) 		,
		  GetStripPositionY( AnnularNumber[i] , 12 , 1 )			,
		  GetStripPositionZ( AnnularNumber[i] , 12 , 1 )			);
										
  TVector3 V = 	TVector3 (GetStripPositionX( AnnularNumber[i] , 8, 32 ) ,
			  GetStripPositionY( AnnularNumber[i] , 8, 32 )	,
			  GetStripPositionZ( AnnularNumber[i] , 8, 32 )	)
											
            -	TVector3 (GetStripPositionX( AnnularNumber[i] , 16, 33) 	,
			  GetStripPositionY( AnnularNumber[i] , 16, 33)		,
			  GetStripPositionZ( AnnularNumber[i] , 16, 33)		);
											
  TVector3 Normal = U.Cross(V);
		
  return(Normal.Unit()) ;	
	
}	

///////////////////////////////////////////////////////////////////////////
namespace S2_LOCAL
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
  //	Theta
  double fS2_Theta_E(TS2Data* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(	"S2/T" + itoa( m_EventData->GetS2ThetaEDetectorNbr(i) ) + "_Theta" + itoa( m_EventData->GetS2ThetaEStripNbr(i) ) + "_E",	
								m_EventData->GetS2ThetaEEnergy(i) );
    
  }
			
  double fS2_Theta_T(TS2Data* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(	"S2/T" + itoa( m_EventData->GetS2ThetaTDetectorNbr(i) ) + "_Theta" + itoa( m_EventData->GetS2ThetaTStripNbr(i) ) +"_T",	
								m_EventData->GetS2ThetaTTime(i) );
    
  }
		
  //	Phi	
  double fS2_Phi_E(TS2Data* m_EventData , const int i)
  {
    
    return CalibrationManager::getInstance()->ApplyCalibration(	"S2/T" + itoa( m_EventData->GetS2PhiEDetectorNbr(i) ) + "_Phi" + itoa( m_EventData->GetS2PhiEStripNbr(i) ) +"_E",	
	     							m_EventData->GetS2PhiEEnergy(i) );
    
  }
			
  double fS2_Phi_T(TS2Data* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(	"S2/T" + itoa( m_EventData->GetS2PhiTDetectorNbr(i) ) + "_Phi" + itoa( m_EventData->GetS2PhiTStripNbr(i) ) +"_T",	
								m_EventData->GetS2PhiTTime(i) );
    
  }
			
			
}

