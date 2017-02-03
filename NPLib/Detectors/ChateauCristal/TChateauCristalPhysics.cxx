/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: E611                                                     *
 *                                                                           *
 * Creation Date  : May 2012                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds Chateau de Cristal Physics                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TChateauCristalPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
// C++
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <cmath>
using namespace std;
using namespace ChateauCristal_LOCAL;

//  ROOT
#include "TChain.h"
#include "TRandom.h"
//  transform an integer to a string
string itoa(int value)
{
/*          char buffer [33];
          sprintf(buffer,"%d",value);
          return buffer;
*/
   std::ostringstream o;
   if (!(o << value))
      return ""  ;
   return o.str();
}

ClassImp(TChateauCristalPhysics)

///////////////////////////////////////////////////////////////////////////
TChateauCristalPhysics::TChateauCristalPhysics()
	: m_NumberOfDetectors(0),
	  m_EventData(new TChateauCristalData),
	  m_PreTreatedData(new TChateauCristalData),
	  m_EventPhysics(this),
	  m_E_Threshold(0),
	  m_Pedestal_Threshold(0),
	  m_beta(0)
{    
 //  cout << "***************** constructeur CCPhys" << endl;
//   cout << "pointeur m_EventData " << m_EventData << endl;
  ReadAddbackConfFile((char *)"/home/e611/ganacq_manip/e611/GRU_e611/calibrations/BaFnear.txt");
}

///////////////////////////////////////////////////////////////////////////
TChateauCristalPhysics::~TChateauCristalPhysics()
{}


///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::Clear()
{

  f_Energy_addback.clear();
  f_Time_addback.clear();

   DetectorNumber  .clear() ;
   Energy          .clear() ;
   EnergyDCgeom    .clear() ;
   EnergyDCreal    .clear() ;
   Time            .clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("ChateauCristal");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"ANGLES_FILE"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      string AngleFile = blocks[i]->GetString("ANGLE_FILE");
      AddModule(AngleFile);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();

   for(int i = 0 ; i < m_NumberOfDetectors ; ++i){
      Cal->AddParameter("ChateauCristal", "Detector"+ NPL::itoa(i)+"_E","ChateauCristal_DETECTOR_"+ NPL::itoa(i)+"_E")  ;
      Cal->AddParameter("ChateauCristal", "Detector"+ NPL::itoa(i)+"_T","ChateauCristal_DETECTOR_"+ NPL::itoa(i)+"_T")  ;  
   }
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeRootInputRaw()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus ( "ChateauCristal"     , true );
   inputChain->SetBranchStatus ( "fChateauCristal_*"  , true );
   inputChain->SetBranchAddress( "ChateauCristal"     , &m_EventData );
}     

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeRootInputPhysics()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus ( "ChateauCristal"          , true );
   inputChain->SetBranchStatus ( "DetectorNumber", true );
   inputChain->SetBranchStatus ( "Energy"        , true );
   inputChain->SetBranchStatus ( "Time"          , true );
   inputChain->SetBranchAddress( "ChateauCristal"          , &m_EventPhysics );
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeRootOutput()
{
   TTree* outputTree = RootOutput::getInstance()->GetTree()      ;
   outputTree->Branch( "ChateauCristal" , "TChateauCristalPhysics" , &m_EventPhysics )  ;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::BuildPhysicalEvent()
{
   BuildSimplePhysicalEvent()  ;
   if( f_addback_possible) CalculateAddback();
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::BuildSimplePhysicalEvent()
{
//	cout << "TCCPhys : BuildSimplePHysicalEvent" << endl;
   PreTreat();

   for(unsigned int i = 0 ; i <   m_PreTreatedData->GetChateauCristalEnergyMult() ; ++i)
   {
      UShort_t det  = m_PreTreatedData->GetChateauCristalEDetectorNbr(i);
      Double_t ener = m_PreTreatedData->GetChateauCristalEnergy(i);

      if (ener > 0) {
         DetectorNumber.push_back(det);
	 Energy        .push_back(ener); 
//         DetectorNumber.push_back(m_PreTreatedData->GetChateauCristalEDetectorNbr(i));
//	 Energy        .push_back(m_PreTreatedData->GetChateauCristalEnergy(i)); 

         // Doppler correction
//         Double_t EDCgeom = DopplerCorrection(Energy.at(i), m_DetectorAngleGeom(DetectorNumber.at(i)));
         Double_t EDCgeom = DopplerCorrection(ener, m_DetectorAngleGeom[det]);
         Double_t EDCreal = DopplerCorrection(ener, m_DetectorAngleReal[det]);
//	 cout << det << "\t" << ener << "\t" << m_beta << "\t" << m_DetectorAngleGeom[det] << "\t" << EDCgeom << "\t" << m_DetectorAngleReal[det] << "\t" << EDCreal << endl;
         EnergyDCgeom.push_back(EDCgeom);
         EnergyDCreal.push_back(EDCreal);
      }

      // Look for associated time
      for(unsigned int j = 0 ; j <   m_PreTreatedData->GetChateauCristalTimeMult() ; ++j )
      {
         if(m_PreTreatedData->GetChateauCristalEDetectorNbr(i) == m_PreTreatedData->GetChateauCristalTDetectorNbr(j))
            Time.push_back(m_PreTreatedData->GetChateauCristalTime(j));
      }                        
   }
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::PreTreat()
{
//	cout << "TCCPhys : PreTreat" << endl;
   ClearPreTreatedData();

   //  E
//   cout << "mult = " << m_EventData->GetChateauCristalEnergyMult() << "\t" << m_EventData <<  endl;
   for(int i = 0 ; i < m_EventData->GetChateauCristalEnergyMult() ; ++i)
   {
   
      if(m_EventData->GetChateauCristalEnergy(i) > m_Pedestal_Threshold && ChannelStatus[m_EventData->GetChateauCristalEDetectorNbr(i)-1])
      {
         double E = fChateauCristal_E(m_EventData , i); 
         if( E > m_E_Threshold )
         {
            m_PreTreatedData->SetChateauCristalEDetectorNbr( m_EventData->GetChateauCristalEDetectorNbr(i) )  ;
            m_PreTreatedData->SetChateauCristalEnergy( E )                                              ;
         }
      } 
   }

   //  T
   for(int i = 0 ; i < m_EventData->GetChateauCristalTimeMult() ; ++i)
   {
      if(ChannelStatus[m_EventData->GetChateauCristalTDetectorNbr(i)-1])
      {
         m_PreTreatedData->SetChateauCristalTDetectorNbr( m_EventData->GetChateauCristalTDetectorNbr(i) )  ;
         m_PreTreatedData->SetChateauCristalTime( fChateauCristal_T(m_EventData , i) )                         ;
      }
   }
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeStandardParameter()
{
   //  Enable all channels
   bool TempChannelStatus;
   ChannelStatus.clear();
   TempChannelStatus=true;
   for(int i = 0 ; i < m_NumberOfDetectors ; ++i)   
      ChannelStatus[i] = TempChannelStatus;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;

   // path to file
   string FileName = "./configs/ConfigChateauCristal.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << "No ConfigChateauCristal.dat found: default parameters loaded for analysis " << FileName << endl;
      return;
   }
   cout << "Loading user parameters for analysis from ConfigChateauCristal.dat " << endl;

   // Save it in a TAsciiFile
   TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiConfig->AppendLine("%%% ConfigChateauCristal.dat %%%");
   asciiConfig->Append(FileName.c_str());
   asciiConfig->AppendLine("");

   // read analysis config file
   string LineBuffer,DataBuffer,whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 20, "ConfigChateauCristal") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {

         whatToDo= "" ;
         AnalysisConfigFile >> whatToDo;

         // Search for comment symbol (%)
         if (whatToDo.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }

         else if (whatToDo=="PEDESTAL_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_Pedestal_Threshold = atof(DataBuffer.c_str() );
            cout << "PEDESTAL THRESHOLD  " << m_Pedestal_Threshold << endl;
         }

         else if (whatToDo=="ENERGY_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_E_Threshold = atof(DataBuffer.c_str() );
            cout << "ENERGY THRESHOLD  " << m_E_Threshold << endl;
         }

         else if (whatToDo=="BETA") {
            AnalysisConfigFile >> DataBuffer;
            m_beta = atof(DataBuffer.c_str() );
            cout << "BETA  " << m_beta << endl;
         }

         else if (whatToDo=="DISABLE") {
            AnalysisConfigFile >> DataBuffer;
            int Detector = atoi(DataBuffer.substr(5).c_str());
            cout << whatToDo << "\t" << DataBuffer << "\t" << Detector << endl;
            bool ChannelStatusBuffer;
            ChannelStatusBuffer=false;
            ChannelStatus[Detector-1] = ChannelStatusBuffer;
         }

         else {
            ReadingStatus = false;
         }
      }
   }
}   



void TChateauCristalPhysics::AddModule(string AngleFile)
{
   ifstream file;

   string path = "/home/e611/ganacq_manip/e611/GRU_e611/calibrations/";
   TString filename = path + AngleFile;

   file.open(filename);
   if (!file) cout << filename << " was not opened" << endl;

   Double_t angle_correc, angle_geo; 
   string buffer;
   UShort_t det, dummy;

   while (file >> buffer >> det >> angle_correc >> angle_geo >> dummy) {
      cout << buffer << "\t" << det << "\t" << angle_correc << "\t" << angle_geo << "\t" << dummy << endl;
      m_DetectorAngleReal[det] = angle_correc; 
      m_DetectorAngleGeom[det] = angle_geo; 
      m_NumberOfDetectors++;
   }

   file.close();

//   cout << mapDetectorAngleCorrec[2] << "\t" << mapDetectorAngleGeo[2] << endl;
}

int TChateauCristalPhysics::ReadAddbackConfFile(char *file_name)
{
  ifstream in_f(file_name);
  if (!in_f){
    cout<<"\n No addback configuration found^\n";
    f_addback_possible= false;
    return -1;
  }
  cout<<"\n File "<<file_name<<" opened! Reading CdC detector configuration";
  string line;
  Short_t i=0,j=0,det,det_nr;
  vector<UShort_t> det_cor;
  det_cor.resize(6);
  while(!in_f.eof()){
    stringstream line_st;
    getline(in_f,line);
    line_st<<line;
    line_st>>det_nr;
    cout<<"\n Det "<< det_nr<<" has around ";
    for(i=0;i<6;i++){
      line_st>>det;
      det_cor[i]=det;
      cout<<det_cor[i]<<"\t";
    }
    
    f_CdC_nearDet[j++]= det_cor;
  }
  f_addback_possible=true;
  return 0;
}

void TChateauCristalPhysics::CalculateAddback()
{
  UShort_t i,j,k,l;
  UShort_t index=-1;
  Double_t ene,ene2,ene_s=0,t1=-100,t2=-2000;
  for(i=0;i<DetectorNumber.size();i++){
    ene=Energy[i];
    for(j=0;j<Time.size();j++)
      if( DetectorNumber[i]==m_EventData->GetChateauCristalTDetectorNbr(j))
	t1=m_EventData->GetChateauCristalTime(j);
    ene_s=ene;
    if( ene< 400.) {
      f_Energy_addback.push_back(0.);
      f_Time_addback.push_back(0.);
    }
    else{
      for(j=0;j<6;j++){
	if(ene==0) break;
	//	cout<<"\nbefore mat ene="<<ene<<" ene_s= "<<ene_s<<"index= "<<index<<"\n";
	index=*(f_CdC_nearDet[DetectorNumber[i]].begin()+j);
	//cout<<"\nafter mat\n";
	ene2=0.;
	for(k=0;k<DetectorNumber.size();k++){
	  if(index==DetectorNumber[k]) {
	    for(l=0;l<Time.size();l++)
	      if( DetectorNumber[k]==m_EventData->GetChateauCristalTDetectorNbr(l))
		t2=m_EventData->GetChateauCristalTime(l);
	    if(abs(t1-t2)<2.) ene2=Energy[k];
	    // cout<<"\nene2="<<ene2;
	    if(ene2>ene) {
	      ene_s=0.;
	      ene=0.;
	      ene2=0.;
	      break;
	    }
	    if( ene2> 400 ){
	      ene_s+=ene2;
	      ene2=0.;
	    }
	    
	    // cout<<"\n k= "<<k<<" max size = "<< DetectorNumber.size()<<"\n";
	  }
	}
	
      }
      // cout<<"\nbefore addback i= "<<i<<" size max "<<f_Energy_addback.size()<<"ene_s "<<ene_s<<"\n";
      f_Energy_addback.push_back(ene_s);
      f_Time_addback.push_back(t1);
      //      cout<<"index "<<i<<" energy "<<Energy[i]<<"addback = "<<f_Energy_addback[i]<<"ene2 "<<ene2<<endl;
      //cout<<"\nafter addback\n";
    }
  }
}



double TChateauCristalPhysics::DopplerCorrection(double E, double Theta)
{
   double Pi = 3.141592654 ;

   double E_corr = 0;
   double gamma = 1./ sqrt(1 - m_beta*m_beta);
   E_corr = gamma * E * (1 - m_beta * cos(Theta*Pi/180.));

   return E_corr;
}

///////////////////////////////////////////////////////////////////////////
double ChateauCristal_LOCAL::fChateauCristal_E( const TChateauCristalData* m_EventData , const int i )
{
   return CalibrationManager::getInstance()->ApplyCalibration("ChateauCristal/Detector" + NPL::itoa( m_EventData->GetChateauCristalEDetectorNbr(i) ) +"_E",  
							      m_EventData->GetChateauCristalEnergy(i)+gRandom->Uniform()-0.5 );
}

double ChateauCristal_LOCAL::fChateauCristal_T( const TChateauCristalData* m_EventData , const int i )
{
   return CalibrationManager::getInstance()->ApplyCalibration("ChateauCristal/Detector" + NPL::itoa( m_EventData->GetChateauCristalTDetectorNbr(i) ) +"_T",  
         m_EventData->GetChateauCristalTime(i) +gRandom->Uniform()-0.5);
}  



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TChateauCristalPhysics::Construct(){
  return (NPL::VDetector*) new TChateauCristalPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_chateaucristal{
  public:
    proxy_chateaucristal(){
      NPL::DetectorFactory::getInstance()->AddToken("ChateauCristal","ChateauCristal");
      NPL::DetectorFactory::getInstance()->AddDetector("ChateauCristal",TChateauCristalPhysics::Construct);
    }
};

proxy_chateaucristal p;
}

