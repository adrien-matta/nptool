/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold SiRes  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TSiResPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;

//   ROOT
#include "TChain.h"

//   tranform an integer to a string
string itoa(int value)
{
   char buffer [33];
   sprintf(buffer,"%d",value);
   return buffer;
}

ClassImp(TSiResPhysics)
///////////////////////////////////////////////////////////////////////////
TSiResPhysics::TSiResPhysics()
   {      
      NumberOfDetector = 0 ;
      EventData = new TSiResData ;
      PreTreatedData    = new TSiResData ;     
      EventPhysics = this ;
      m_SiRes_E_Threshold   = 0;   
      m_SiRes_RAW_Threshold   = 0;     
      m_SiRes_EBack_Threshold   = 0;   
      m_SiRes_RAWBack_Threshold   = 0;     
   }
   
///////////////////////////////////////////////////////////////////////////
TSiResPhysics::~TSiResPhysics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::Clear()
   {
      DetectorNumber.clear() ;
      ChannelNumber.clear() ;
      Energy.clear() ;
      Time.clear() ;
      EnergyBack.clear() ;
      x.clear() ;
      y.clear() ;
   }
   
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::ReadConfiguration(NPL::InputParser parser) {

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("SiRes");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"X","Y","Z","Width","Height","Thickness"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  SiRes " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");
      double Width = blocks[i]->GetDouble("Width","mm");
      double Height = blocks[i]->GetDouble("Height","mm");
      double Thickness = blocks[i]->GetDouble("Thickness","mm");
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }
}
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(int i = 0 ; i < NumberOfDetector ; i++)
         {
            for( int j = 0 ; j < 4 ; j++)
               {
                  Cal->AddParameter("SiRes", "Detector"+ NPL::itoa(i+1)+"_E","SiRes_Detector"+ NPL::itoa(i+1)+"_E")   ;
               }
               Cal->AddParameter("SiRes", "Detector"+ NPL::itoa(i+1)+"_EBack","SiRes_Detector"+ NPL::itoa(i+1)+"_EBack")   ;   
               Cal->AddParameter("SiRes", "Detector"+ NPL::itoa(i+1)+"_T","SiRes_Detector"+ NPL::itoa(i+1)+"_T")   ;   
      
         }
   }
   
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()     ;
      inputChain->SetBranchStatus ( "SiRes"       , true )        ;
      inputChain->SetBranchStatus ( "fSiRes_*"    , true )        ;
      inputChain->SetBranchAddress( "SiRes"       , &EventData )  ;
   }
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::InitializeRootInputPhysics()
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus ( "SiRes", true );
      inputChain->SetBranchStatus ( "DetectorNumber", true );
      inputChain->SetBranchStatus ( "ChannelNumber", true );
      inputChain->SetBranchStatus ( "Energy", true );
      inputChain->SetBranchStatus ( "EnergyBack", true );
      inputChain->SetBranchStatus ( "Time", true );
      inputChain->SetBranchAddress( "SiRes", &EventPhysics );
   }
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::InitializeRootOutput()
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
      outputTree->Branch( "SiRes" , "TSiResPhysics" , &EventPhysics ) ;
   }
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::InitSpectra(){  
   m_Spectra = new TSiResSpectra(NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::FillSpectra(){  
   m_Spectra -> FillRawSpectra(EventData);
   m_Spectra -> FillPreTreatedSpectra(PreTreatedData);
   m_Spectra -> FillPhysicsSpectra(EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TSiResPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
} 
///////////////////////////////////////////////////////////////////////////

void TSiResPhysics::PreTreat(){  

  //   X
  //   E
  ClearPreTreatedData();
  double E,T;
  E=-1000; T=-1000;
  int N=-1000;
  for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
    {
	if( EventData->GetEEnergy(i)>m_SiRes_RAW_Threshold )
    	{
		
		E=CalibrationManager::getInstance()->ApplyCalibration("SiRes/Detector" + NPL::itoa( EventData->GetEDetectorNumber(i) ) +"_Channel"+ NPL::itoa( EventData->GetEChannelNumber(i) )+"_E",EventData->GetEEnergy(i));
    		//if(E>m_SiRes_E_Threshold)
    		{
        		N=EventData->GetEDetectorNumber(i);
        		PreTreatedData->SetEDetectorNumber( N );
        		PreTreatedData->SetEChannelNumber( EventData->GetEChannelNumber(i) );
        		PreTreatedData->SetEEnergy( E );
    		}
    	}
    }
  for(unsigned int i = 0 ; i < EventData->GetEEnergyBackMult() ; i++)
    {
	if( EventData->GetEEnergyBack(i)>m_SiRes_RAWBack_Threshold && EventData->GetEEnergyBackDetectorNumber(i)==N )
	{
		E=CalibrationManager::getInstance()->ApplyCalibration("SiRes/Detector" + NPL::itoa( EventData->GetEEnergyBackDetectorNumber(i) ) +"_E",EventData->GetEEnergyBack(i));
		if(E>m_SiRes_EBack_Threshold)
		{    
			PreTreatedData->SetEEnergyBackDetectorNumber( EventData->GetEEnergyBackDetectorNumber(i) );
			PreTreatedData->SetEEnergyBack( E );
			if(EventData->GetTimeMult()>0)
			{
				T=CalibrationManager::getInstance()->ApplyCalibration("SiRes/Detector"+ NPL::itoa(EventData->GetTDetectorNumber(i))+"_T",EventData->GetTTime(i) ) ;      
			}            
			PreTreatedData->SetTTime( T );
		}
	}
    } 
}
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::BuildPhysicalEvent()
   {
      BuildSimplePhysicalEvent()   ;
   }


///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::BuildSimplePhysicalEvent()
   {
	PreTreat();
      for(unsigned int i = 0 ; i < PreTreatedData->GetEnergyMult() ; i++)
         {
            DetectorNumber.push_back( PreTreatedData->GetEDetectorNumber(i) )   ;
            ChannelNumber.push_back( PreTreatedData->GetEChannelNumber(i) )   ;
            Energy.push_back( PreTreatedData->GetEEnergy(i) );
          }
       for(unsigned int i = 0 ; i < EventData->GetEEnergyBackMult() ; i++)
         {
            EnergyBack.push_back( PreTreatedData->GetEEnergyBack(i) );
//to be corrected when we read time
         //}
       //for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; i++)
        //{
            Time.push_back( PreTreatedData->GetTTime(i) );
         }
     if(PreTreatedData->GetEnergyMult()==4)Treat();

   }
///////////////////////////////////////////////////////////////////////////
void TSiResPhysics::Treat()
   {
      double E1=-1000; double E2=-1000; double E3=-1000; double E4=-1000; 
      for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
         {
         	if(ChannelNumber[i]==0)E1=Energy[i];//DH
         	if(ChannelNumber[i]==1)E2=Energy[i];//DB
         	if(ChannelNumber[i]==2)E3=Energy[i];//GB
         	if(ChannelNumber[i]==3)E4=Energy[i];//GH
         }
      	x.push_back( 1+(E1+E2-E3-E4) / (E1+E2+E3+E4) ) ;
      	y.push_back( 1+(E1+E4-E2-E3) / (E1+E2+E3+E4) ) ;
   }
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TSiResPhysics::Construct(){
  return (NPL::VDetector*) new TSiResPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_sires{
  public:
    proxy_sires(){
      NPL::DetectorFactory::getInstance()->AddToken("SiRes","SiRes");
      NPL::DetectorFactory::getInstance()->AddDetector("SiRes",TSiResPhysics::Construct);
    }
};

proxy_sires p;
}

