/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra GIRON  contact address: giron@ipno.in2p3.fr       *
 *                  Benjamin LE CROM		   lecrom@ipno.in2p3.fr                                                        *
 * Creation Date  : march 2014                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold exogam treated data                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TExogamPhysics.h"
using namespace EXOGAM_LOCAL;
	
//	STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>

//	NPL
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "NPVDetector.h"
#include "NPOptionManager.h"
//	ROOT
#include "TChain.h"


///////////////////////////////////////////////////////////////////////////
ClassImp(TExogamPhysics)
///////////////////////////////////////////////////////////////////////////
TExogamPhysics::TExogamPhysics() 
{
  // cout << "coconutsssssssssssssssssssssssssssss " << endl;

  EventMultiplicity 	= 0 			;  
  ECC_Multiplicity      = 0                     ;
  GOCCE_Multiplicity    = 0                     ;
  NumberOfHitClover     = 0                     ;
  NumberOfHitCristal    = 0                     ;
  m_Spectra             = NULL;
  NumberOfClover=0;

  PreTreatedData	= new TExogamData	;
  EventData 		= new TExogamData	;
  EventPhysics 		= this			;
  NumberOfClover	= 0			;
  CloverMult            = 0                     ; 

}
		
///////////////////////////////////////////////////////////////////////////
void TExogamPhysics::BuildSimplePhysicalEvent()
{ 
 
  BuildPhysicalEvent(); 
}
///////////////////////////////////////////////////////////////////////////
void TExogamPhysics::PreTreat()	
{
  ClearPreTreatedData();

  //E 

  for(unsigned int i = 0 ; i < EventData -> GetECCEMult(); i++) {
    UShort_t cristal_E = 10000 ; UShort_t cristal_T = 2000;
    //if(IsValidChannel)
    {
      int clover  = EventData -> GetECCEClover(i);
      int cristal = EventData -> GetECCECristal(i);
     
      if(EventData -> GetECCEEnergy(i) < 3000)  cristal_E = CalibrationManager::getInstance()-> ApplyCalibration("EXOGAM/Cl"+ NPL::itoa(clover)+"_Cr"+ NPL::itoa(cristal)+"_Elow", EventData -> GetECCEEnergy(i));
      else                                      cristal_E = CalibrationManager::getInstance()-> ApplyCalibration("EXOGAM/Cl"+ NPL::itoa(clover)+"_Cr"+ NPL::itoa(cristal)+"_Ehigh", EventData -> GetECCEEnergy(i));

    
      if(cristal_E > Threshold_ECC)
	{      

	PreTreatedData->SetECCEClover ( clover )        ;
	PreTreatedData->SetECCECristal( cristal )	;
	PreTreatedData->SetECCEEnergy ( cristal_E )	;

	      bool checkT = false;
	      for(unsigned int k = 0; k < EventData -> GetECCTMult(); k++){
		  if(clover == EventData -> GetECCTClover(k) && cristal == EventData -> GetECCTCristal(k)){
		      // cout << EventData -> GetECCTTime(k) << endl;

		      if(EventData -> GetECCTTime(k) < 16383)  cristal_T = CalibrationManager::getInstance()-> ApplyCalibration("EXOGAM/Cl"+ NPL::itoa(clover)+"_Cr"+ NPL::itoa(cristal)+"_T", EventData -> GetECCTTime(k));
		      else                                     cristal_T = 2500;  
		  
		      //if(cristal_T >5000 && cristal_T !=25000 ) cout << "PreTreat " << cristal_T << " " << EventData -> GetECCTTime(k) << " " << clover << " " << cristal << " " << EventData->GetECCTMult() << endl;
		  	      
		     checkT=true;
		      PreTreatedData->SetECCTClover (clover )        ;
		      PreTreatedData->SetECCTCristal( cristal )	;
		      PreTreatedData->SetECCTTime   ( cristal_T )	;

		      ECC_Multiplicity ++;
		      GOCCE_Multiplicity++;
		    }
		 
		}

 		if(!checkT) {
 			PreTreatedData->SetECCTClover (clover )        ;
		      	PreTreatedData->SetECCTCristal( cristal )	;
		      	PreTreatedData->SetECCTTime   ( -1000 )	;
	 	}

	    
	}
    }
  }

  //cout << PreTreatedData-> GetECCTMult() << " " << PreTreatedData-> GetECCEMult() << endl;

 
  //GOCCE

  //E 
  
  for(unsigned int i = 0 ; i < EventData -> GetGOCCEEMult(); i++) {
    UShort_t segment_E = 25000;   

    //if(IsValidChannel)
    {
      int clover  = EventData -> GetGOCCEEClover(i);
      int cristal = EventData -> GetGOCCEECristal(i);
      int segment = EventData -> GetGOCCEESegment(i);
      
      if(EventData -> GetGOCCEEEnergy(i) > RawThreshold_GOCCE)
	{
	  segment_E = CalibrationManager::getInstance()->ApplyCalibration("EXOGAM/Cl"+ NPL::itoa(clover)+"_Cr"+ NPL::itoa(cristal)+"_Seg"+ NPL::itoa(segment)+"_E", EventData -> GetGOCCEEEnergy(i));
	  	  
	  if(segment_E > Threshold_GOCCE)
	    {
	      PreTreatedData->SetGOCCEEClover ( clover )        ;
	      PreTreatedData->SetGOCCEECristal( cristal )	;
	      PreTreatedData->SetGOCCEESegment( segment )	;
	      PreTreatedData->SetGOCCEEEnergy ( segment_E )	;
	      
	    }
	}
      else
	{
	  
	}
    }
  }

  //cout << "EXOGAM pretreat ok!" << endl;
  return;
  
}
///////////////////////////////////////////////////////////////////////////
	
void TExogamPhysics::BuildPhysicalEvent()
{ 
  PreTreat();
  
  if(PreTreatedData -> GetECCEMult() != PreTreatedData -> GetECCTMult()) cout << PreTreatedData -> GetECCEMult() << " " <<  PreTreatedData -> GetECCTMult() << endl;

   
  for(unsigned int i = 0 ; i < PreTreatedData -> GetECCEMult(); i++) {
    
    // cout << i << " " << cristal_E << endl;
    // if(PreTreatedData->GetECCTTime(i) > 0) 
      {
    ECC_E.push_back(PreTreatedData->GetECCEEnergy(i));
    ECC_T.push_back(PreTreatedData->GetECCTTime(i));
    ECC_CloverNumber.push_back(PreTreatedData->GetECCEClover(i));
    ECC_CristalNumber.push_back(PreTreatedData->GetECCECristal(i));

    //    cout << "BuildPhys " << PreTreatedData->GetECCEClover(i) << " " <<  PreTreatedData->GetECCECristal(i)<< " " << PreTreatedData->GetECCTTime(i) << " " << endl;
      }
  }

  
  for(unsigned int j = 0 ; j < PreTreatedData -> GetGOCCEEMult(); j++) {
    GOCCE_E.push_back(PreTreatedData->GetGOCCEEEnergy(j));
    GOCCE_CloverNumber.push_back(PreTreatedData->GetGOCCEEClover(j));
    GOCCE_CristalNumber.push_back(PreTreatedData->GetGOCCEECristal(j));
    GOCCE_SegmentNumber.push_back(PreTreatedData->GetGOCCEESegment(j));
  }
  

  //int NumberOfHitClover = 0;
  
  int DetectorID = -1;
   
  for( unsigned short i = 0 ; i < PreTreatedData->GetECCEMult() ; i++ )
    { 
      // cout << PreTreatedData->GetECCEClover(i) << endl;
      if( PreTreatedData->GetECCEClover(i) != DetectorID)
	{
	  if(i==0) 
	    {
	      NumberOfHitClover++;
	    }
	  else if(PreTreatedData->GetECCEClover(i)!= PreTreatedData->GetECCEClover(i-1) )  
	    {  
	      NumberOfHitClover++; 
	    }
	}
      if(NumberOfHitClover == 4) break;	
      //clover_mult -> Fill(NumberOfHitClover);
      
    }
  
  //cout << "NumberOfHitClover " << NumberOfHitClover << endl;
  
  map<int, vector<int> > MapCristal;
  map<int, vector<int> > MapSegment; 
  
  map<int, vector<int> > :: iterator it;    // iterator used with MapCristal
  map<int, vector<int> > :: iterator at;    // iterator used with MapSegment

  vector<int> PositionOfCristal_Buffer_ECC;
  vector<int> PositionOfSegment_Buffer_GOCCE;
 

  //Fill map Cristal
  for(int clo = 0; clo < NumberOfClover; clo++)
    {
      for(unsigned int k = 0; k < ECC_CloverNumber.size(); k++)
	{
	  if(ECC_CloverNumber.at(k) == clo) // && ECC_CristalNumber.at(k)== cri )  
	    PositionOfCristal_Buffer_ECC.push_back(k);
	}
      if(PositionOfCristal_Buffer_ECC.size() != 0) MapCristal[clo] = PositionOfCristal_Buffer_ECC;
      
      PositionOfCristal_Buffer_ECC.clear();
      
    }
  

  //Fill map Segment
  for(int clo = 0; clo < NumberOfClover; clo++)
    {
      for(int cri = 0; cri < 4 ; cri++)
	{
	  //  for(int seg = 0; seg < 4 ; seg++)
	    {
	      for(unsigned int m = 0; m < GOCCE_CloverNumber.size(); m++)
		{
		  if(GOCCE_CloverNumber.at(m) == clo && GOCCE_CristalNumber.at(m) == cri)// && GOCCE_SegmentNumber.at(m) == seg) 
		    {
		      // PositionOfSegment_Buffer_GOCCE.push_back(4*clo+cri);
		      PositionOfSegment_Buffer_GOCCE.push_back(m);
		    }
		}
	    }
	    if(PositionOfSegment_Buffer_GOCCE.size() != 0) MapSegment[4*clo+cri] = PositionOfSegment_Buffer_GOCCE;
	    
	    PositionOfSegment_Buffer_GOCCE.clear();
	}
    }

  // Treatment 
  for(int clo = 0; clo < NumberOfClover ; clo++)
    {
      double E = 0; double T = 0;
      int mult_cristal = 0;
      int cristal = -1 , segment;

      int cristal_Emax = 0; int cristal_Emin = 0;
      int Emax = 0, Emin = 1000000;
      int Tmin = 0, Tmax = 0;

      //ADD-BACK 
      it = MapCristal.find(clo); 
      
      int cristal_cond = 0;
      
      if(it != MapCristal.end())  
	{
	  vector<int> PositionOfCristal = it -> second;     
	  	      		
	  mult_cristal = PositionOfCristal.size();
	  //if(mult_cristal!=0) cristal_mult -> Fill(mult_cristal);
		
	  // ADD-BACK
	  //cout << "boucle" << endl;

	  for(unsigned int k = 0; k < PositionOfCristal.size(); k++)        
	    {
	      int indice = PositionOfCristal.at(k);

	      cristal_cond += ECC_CristalNumber.at(indice);
	      // cout <<  ECC_CristalNumber.at(k) << " " ECC_E.at(k) << endl;
	      
	      if(mult_cristal < 3)         
		{
		  E+= ECC_E.at(indice); 
		  
		  if(ECC_E.at(indice) < Emin) {
		    cristal_Emin = ECC_CristalNumber.at(indice);
		    Emin = ECC_E.at(indice);
		    Tmin = ECC_T.at(indice);
		  }
		  
		  if(ECC_E.at(indice) > Emax) {
		    cristal_Emax = ECC_CristalNumber.at(indice);
		    Emax = ECC_E.at(indice);
		    Tmax = ECC_T.at(indice);
		  } 
		}

	      else // case of multiplicity = 3 or 4
		{
		  E = -1; cristal_Emax = -1; cristal_Emin = -1; Tmax = -1; Tmin = -1;
		}
	      
	      // cout << ECC_E.at(indice) << " " << Emax << " " << cristal_Emax << " " << Emin << " " << cristal_Emin << endl;

	    }

	  if( (mult_cristal==1) || (mult_cristal ==2  && cristal_cond %2 == 1) )
	    { 
	      // cout << cristal_cond << endl;

	      //cristal = cristal_Emax; T = Tmax;
	      //cout << Emax << " " << cristal_Emax << " " << Emin << " " << cristal_Emin << endl;

	      if(E > 500) { cristal = cristal_Emax; T = Tmax; }
	      else        { cristal = cristal_Emin; T = Tmin; }
	    	
	      
	      // DOPPLER CORRECTION
	      
	      at = MapSegment.find(4*clo+cristal);
	      segment = -1;
	      
	      if(at != MapSegment.end())
		{
		  vector<int> PositionOfSegment = at -> second;     // position of segment k in the vector
		  
		  int segment_max = -1, E_temp = -1;
		  
		  for(unsigned int m = 0; m < PositionOfSegment.size(); m++)             // loop on hit segments of cristal cri of clover clo
		    {
		      int indice = PositionOfSegment.at(m);

		      if(GOCCE_E.at(indice) > 0 && GOCCE_CristalNumber.at(indice) == cristal)
			{
			  if( GOCCE_E.at(indice) > E_temp ) 
			    {
			      segment_max = GOCCE_SegmentNumber.at(indice) ;
			      E_temp = GOCCE_E.at(indice);
			    }
			}
		    }
		  segment = segment_max;
		}
	      
	    }     
	 

	  if(E > 0 && cristal != -1 && segment != -1)
	    {
	      TotalEnergy_lab.push_back(E);
	      Time.push_back(T);
	      CloverNumber.push_back(clo);
	      CristalNumber.push_back(cristal);
	      SegmentNumber.push_back(segment);
	      
	      double theta = GetSegmentAngleTheta(clo, cristal, segment);
	      
	      Theta.push_back(theta);
	      
	      double doppler_E = DopplerCorrection(E, theta);
	      DopplerCorrectedEnergy.push_back(doppler_E);

	      //  cout << E  << " " << clo << " " << cristal << " " << segment << " " << theta << " " << doppler_E << endl;
	      
	    }

	}  // end of condition over CristalMap

    } // loop over NumberOfClover

  CloverMult = GetClover_Mult();

  //cout << "Exogam fine" << endl;	
}	


double TExogamPhysics::DopplerCorrection(double E, double Theta)
{
  double Pi = 3.141592654 ;
  TString filename = "configs/beta.txt";
  ifstream file;
  //cout << filename << endl;
  file.open(filename);
  if(!file) cout << filename << " was not opened" << endl;

  double E_corr = 0;
  double beta = 0.; 
  file>>beta;
  double gamma = 1./ sqrt(1-beta*beta);

  E_corr = gamma * E * ( 1. - beta * cos(Theta*Pi/180.)); 
  
  return(E_corr);

}


///////////////////////////////////////////////////////////////////////////


void TExogamPhysics::Clear()
{
  EventMultiplicity  = 0;
 ECC_Multiplicity   = 0;
  GOCCE_Multiplicity = 0;
  NumberOfHitClover  = 0;
  NumberOfHitCristal  = 0;

  ECC_CloverNumber	.clear()	;
  ECC_CristalNumber	.clear()	;
  GOCCE_CloverNumber	.clear()	;
  GOCCE_CristalNumber	.clear()	;
  GOCCE_SegmentNumber	.clear()	;

  // ECC
  ECC_E.clear()	;
  ECC_T.clear();

  // GOCCE
  GOCCE_E.clear()	;

  CristalNumber.clear()    ;
  SegmentNumber.clear()    ;
  CloverNumber .clear()    ;
  
  TotalEnergy_lab       .clear()    ;
  Time                  .clear()    ;
  DopplerCorrectedEnergy.clear()    ;
  Position              .clear()    ;
  Theta                 .clear()    ;



}
///////////////////////////////////////////////////////////////////////////

////	Innherited from VDetector Class	////				
				
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TExogamPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("EXOGAMClover");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"EXOGAMClover"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      string AngleFile = blocks[i]->GetString("ANGLE_FILE");
      AddClover(AngleFile);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TExogamPhysics::InitSpectra(){  
   m_Spectra = new TExogamSpectra(NumberOfClover);
}

///////////////////////////////////////////////////////////////////////////
void TExogamPhysics::FillSpectra(){  
   m_Spectra -> FillRawSpectra(EventData);
   m_Spectra -> FillPreTreatedSpectra(PreTreatedData);
   m_Spectra -> FillPhysicsSpectra(EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TExogamPhysics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TExogamPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TExogamPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
} 
//////////////////////////////////////////////////////////////////////////
void TExogamPhysics::AddClover(string AngleFile)
{
  ifstream file;
  //  TString filename = Form("posBaptiste/angles_exogam_clover%d.txt",NumberOfClover);
  //  TString filename = Form("posz42_simu50mm/angles_exogam_clover%d.txt",NumberOfClover);
  //  TString filename = Form("posz42_exp_stat_demiring/angles_exogam_clover%d.txt",NumberOfClover);
  
  string path = "configs/";
  TString filename = path + AngleFile;
  
  cout << filename << endl;
  file.open(filename);
  if(!file) cout << filename << " was not opened" << endl;

  vector <double> Angles;
  vector < vector <double> > Segment_angles;
  vector < vector < vector <double> > > Cristal_angles;
 
  Cristal_angles.clear();

  double angle; string buffer;

  for(int i = 0; i < 4; i++)
    {
      Segment_angles.clear();

      for(int j = 0; j < 4; j++)
	{
	  Angles.clear();

	  for(int k = 0; k < 2; k++)
	    {
	      file >> buffer >> angle;

	      Angles.push_back(angle);   // Theta (k = 0)   Phi (k = 1)

	      //cout << angle << endl;
	       if(Angles.size()==2)
	       cout << "Clover " << NumberOfClover << ": Theta=" << Angles[0] << " Phi=" << Angles[1]<< endl;
	      
	    }
	  
	  Segment_angles.push_back(Angles);
	}
      
      Cristal_angles.push_back(Segment_angles);
    }

  Clover_Angles_Theta_Phi.push_back(Cristal_angles);

  file.close();

  NumberOfClover++;

}	


//	Add Parameter to the CalibrationManger
void TExogamPhysics::AddParameterToCalibrationManager()	
{
  
  CalibrationManager* Cal = CalibrationManager::getInstance();
		
  for(int i = 0 ; i < NumberOfClover ; i++)
    {
      for( int j = 0 ; j < 4 ; j++)
	{
	  Cal->AddParameter("EXOGAM", "Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_Elow" ,"EXOGAM_Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_Elow");
	  Cal->AddParameter("EXOGAM", "Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_Ehigh","EXOGAM_Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_Ehigh");
	  Cal->AddParameter("EXOGAM", "Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_T","EXOGAM_Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_T")	;
	  
	  for( int k = 0 ; k < 4 ; k++)
	    {
	      Cal->AddParameter("EXOGAM", "Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_Seg"+ NPL::itoa(k)+"_E","EXOGAM_Cl"+ NPL::itoa(i)+"_Cr"+ NPL::itoa(j)+"_Seg"+ NPL::itoa(k)+"_E")	;
	    }
	}
    }
}
	

//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TExogamPhysics::InitializeRootInputRaw() 		
{
  TChain* inputChain = RootInput::getInstance()->GetChain()	;
  inputChain->SetBranchStatus( "EXOGAM" , true )			;
  inputChain->SetBranchStatus( "fEXO_*" , true )			;
  inputChain->SetBranchAddress( "EXOGAM" , &EventData )		;

  /*
  TList* outputList = RootOutput::getInstance()->GetList();
   clover_mult = new TH1F("clover_mult","clover_mult",20,0,20);
    outputList->Add(clover_mult);

  cristal_mult = new TH1F("cristal_mult","cristal_mult",20,0,20);
  outputList->Add(cristal_mult);
  */
}

/////////////////////////////////////////////////////////////////////
//   Activated associated Branches and link it to the private member DetectorPhysics address
//   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
void TExogamPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "EventMultiplicty" , true );
  inputChain->SetBranchStatus( "ECC_Multiplicity" , true );
  inputChain->SetBranchStatus( "GOCCE_Multiplicity" , true );
  inputChain->SetBranchStatus( "ECC_CloverNumber" , true );
  inputChain->SetBranchStatus( "ECC_CristalNumber" , true );
  inputChain->SetBranchStatus( "GOCCE_CloverNumber" , true );
  inputChain->SetBranchStatus( "GOCCE_CristalNumber" , true );
  inputChain->SetBranchStatus( "GOCCE_SegmentNumber" , true );
  inputChain->SetBranchStatus( "ECC_E" , true );
  inputChain->SetBranchStatus( "ECC_T" , true );
  inputChain->SetBranchStatus( "GOCCE_E" , true );
  inputChain->SetBranchStatus( "CristalNumber" , true );
  inputChain->SetBranchStatus( "SegmentNumber" , true );
  inputChain->SetBranchStatus( "CloverNumber" , true );
  inputChain->SetBranchStatus( "CloverMult" , true );
  inputChain->SetBranchStatus( "TotalEnergy_lab" , true );
  inputChain->SetBranchStatus( "Time" , true );
  inputChain->SetBranchStatus( "DopplerCorrectedEnergy" , true );
  inputChain->SetBranchStatus( "Position" , true );
  inputChain->SetBranchStatus( "Theta" , true );
  inputChain->SetBranchAddress( "EXOGAM" , &EventPhysics );

}

/////////////////////////////////////////////////////////////////////

//	Create associated branches and associated private member DetectorPhysics address
void TExogamPhysics::InitializeRootOutput() 	
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()		;
  outputTree->Branch( "EXOGAM" , "TExogamPhysics" , &EventPhysics )	;

  // control histograms if needed
  /*  
  TList* outputList = RootOutput::getInstance()->GetList();
  controle = new TH1F("controle","histo de controle",20,0,20);
  outputList->Add(controle);
  */

}


///////////////////////////////////////////////////////////////////////////
namespace EXOGAM_LOCAL
{
  //	tranform an integer to a string
  string itoa(int value)
  {
    std::ostringstream o;
			
    if (!(o << value))
      return ""	;
			    
    return o.str();
  }
}
			
  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TExogamPhysics::Construct(){
  return (NPL::VDetector*) new TExogamPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_exogam{
  public:
    proxy_exogam(){
      NPL::DetectorFactory::getInstance()->AddToken("EXOGAMArray","Exogam");
      NPL::DetectorFactory::getInstance()->AddDetector("EXOGAMArray",TExogamPhysics::Construct);
    }
};

proxy_exogam p;
}

