/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ComptonTelescope treated data                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TComptonTelescopePhysics.h"
using namespace ComptonTelescope_LOCAL;

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPDetectorFactory.h"

//   ROOT
#include "TChain.h"


ClassImp(TComptonTelescopePhysics)


  ///////////////////////////////////////////////////////////////////////////
TComptonTelescopePhysics::TComptonTelescopePhysics()
  : m_EventData(new TComptonTelescopeData),
  m_PreTreatedData(new TComptonTelescopeData),
  m_EventPhysics(this),
  m_MaximumStripMultiplicityAllowed(10),
  m_StripEnergyMatchingSigma(0.060),      // MeV
  m_StripEnergyMatchingNumberOfSigma(3),  // MeV
  m_StripFront_E_RAW_Threshold(0),
  m_StripFront_E_Threshold(0),
  m_StripBack_E_RAW_Threshold(0),
  m_StripBack_E_Threshold(0),
  m_Take_E_Front(true), // p-side
  m_NumberOfDetectors(0),
  m_NumberOfStrips(32)
{
  EventMultiplicity   = 0;
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::BuildPhysicalEvent()
{
  BuildSimplePhysicalEvent();
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::BuildSimplePhysicalEvent()
{
  // select active channels and apply threhsolds
  PreTreat();

  if (CheckEvent() == 1) {   // case where multiplicity front = multiplicity back
    vector<TVector2> couple = Match_Front_Back();
    EventMultiplicity = couple.size();

    for (UShort_t i = 0; i < couple.size(); ++i) { // loop on selected events
      Int_t    N       = m_PreTreatedData->GetCTTrackerFrontEDetectorNbr(couple[i].X());
      Int_t    Front   = m_PreTreatedData->GetCTTrackerFrontEStripNbr(couple[i].X());
      Int_t    Back    = m_PreTreatedData->GetCTTrackerBackEStripNbr(couple[i].Y());
      Double_t Front_E = m_PreTreatedData->GetCTTrackerFrontEEnergy(couple[i].X());
      Double_t Back_E  = m_PreTreatedData->GetCTTrackerBackEEnergy(couple[i].Y());

      // Fill TComptonTelescopePhysics private members
      DetectorNumber.push_back(N);
      StripFront_E.push_back(Front_E);
      StripBack_E.push_back(Back_E);

      if (m_Take_E_Front)
        Strip_E.push_back(Front_E);
      else
        Strip_E.push_back(Back_E);

      Strip_Front.push_back(Front);
      Strip_Back.push_back(Back);
    }
  }

  //   if (DetectorNumber.size() == 1) return;
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::PreTreat()
{
  // Clear pre treated object
  ClearPreTreatedData();

  // Front, energy
  for (UShort_t i = 0; i < m_EventData->GetCTTrackerFrontEMult(); ++i) {
    if (m_EventData->GetCTTrackerFrontEEnergy(i) > m_StripFront_E_RAW_Threshold && 
        IsValidChannel("Front", m_EventData->GetCTTrackerFrontEDetectorNbr(i), m_EventData->GetCTTrackerFrontEStripNbr(i))) {
      Double_t E = fStrip_Front_E(m_EventData, i);
      if (E > m_StripFront_E_Threshold) {
        m_PreTreatedData->SetCTTrackerFrontETowerNbr(m_EventData->GetCTTrackerFrontETowerNbr(i));
        m_PreTreatedData->SetCTTrackerFrontEDetectorNbr(m_EventData->GetCTTrackerFrontEDetectorNbr(i));
        m_PreTreatedData->SetCTTrackerFrontEStripNbr(m_EventData->GetCTTrackerFrontEStripNbr(i));
        m_PreTreatedData->SetCTTrackerFrontEEnergy(E);

      }
    }
  }

  // Back, energy
  for (UShort_t i = 0; i < m_EventData->GetCTTrackerBackEMult(); ++i) {
    if (m_EventData->GetCTTrackerBackEEnergy(i) > m_StripBack_E_RAW_Threshold && 
        IsValidChannel("Back", m_EventData->GetCTTrackerBackEDetectorNbr(i), m_EventData->GetCTTrackerBackEStripNbr(i))) {
      Double_t E = fStrip_Back_E(m_EventData, i);
      if (E > m_StripBack_E_Threshold) {
        m_PreTreatedData->SetCTTrackerBackETowerNbr(m_EventData->GetCTTrackerBackETowerNbr(i));
        m_PreTreatedData->SetCTTrackerBackEDetectorNbr( m_EventData->GetCTTrackerBackEDetectorNbr(i));
        m_PreTreatedData->SetCTTrackerBackEStripNbr( m_EventData->GetCTTrackerBackEStripNbr(i));
        m_PreTreatedData->SetCTTrackerBackEEnergy(E);
      }
    }
  }

  // DSSSD time information and LaBr3 still have to be done...
}



///////////////////////////////////////////////////////////////////////////
int TComptonTelescopePhysics::CheckEvent()
{
  // same multiplicity on front and back side 
  if (m_PreTreatedData->GetCTTrackerBackEMult() == m_PreTreatedData->GetCTTrackerFrontEMult())
    return 1 ; // Regular Event

  else
    return -1 ; // Rejected Event
}



///////////////////////////////////////////////////////////////////////////
vector<TVector2> TComptonTelescopePhysics::Match_Front_Back()
{
  vector<TVector2> ArrayOfGoodCouple;

  // Select allowed multiplicity events. If multiplicity is too 
  // high, then return "empty" vector
  if (m_PreTreatedData->GetCTTrackerFrontEMult() > m_MaximumStripMultiplicityAllowed || 
      m_PreTreatedData->GetCTTrackerBackEMult() > m_MaximumStripMultiplicityAllowed)
    return ArrayOfGoodCouple;

  // Loop on front multiplicity
  for (UShort_t i = 0; i < m_PreTreatedData->GetCTTrackerFrontEMult(); i++) {
    // Loop on back multiplicity
    for (UShort_t j = 0; j < m_PreTreatedData->GetCTTrackerBackEMult(); j++) {
      // if same tower and same detector check energy
      if ((m_PreTreatedData->GetCTTrackerFrontETowerNbr(i) == m_PreTreatedData->GetCTTrackerBackETowerNbr(j)) && 
          (m_PreTreatedData->GetCTTrackerFrontEDetectorNbr(i) == m_PreTreatedData->GetCTTrackerBackEDetectorNbr(j))) {
        // equal energy
        if (abs((m_PreTreatedData->GetCTTrackerFrontEEnergy(i) - m_PreTreatedData->GetCTTrackerBackEEnergy(j))/2.) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma) {
          ArrayOfGoodCouple.push_back(TVector2(i,j));
        } // end test energy
      } // end test same tower and detector
    } // end loop back multiplicity
  } // end loop front multiplicity

  // prevent treating event with ambiguous matchin beetween X and Y
  if (ArrayOfGoodCouple.size() > m_PreTreatedData->GetCTTrackerFrontEMult()) ArrayOfGoodCouple.clear();

  return ArrayOfGoodCouple;
}



////////////////////////////////////////////////////////////////////////////
bool TComptonTelescopePhysics::IsValidChannel(const string DetectorType, const int detector, const int channel)
{
  if (DetectorType == "Front")
    return *(m_FrontChannelStatus[detector-1].begin()+channel);

  else if (DetectorType == "Back")
    return *(m_BackChannelStatus[detector-1].begin()+channel);

  else return false;
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::ReadAnalysisConfig()
{
  bool ReadingStatus = false;

  cout << "\t/////////// Reading ConfigComptonTelescope.dat file ///////////" << endl;

  // path to file
  string FileName = "./configs/ConfigComptonTelescope.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << "\tNo ConfigComptonTelescope.dat found: default parameters loaded for Analysis " << FileName << endl;
    return;
  }
  cout << "\tLoading user parameters from ConfigComptonTelescope.dat " << endl;

  // storing config file in the ROOT output file
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigComptonTelescope.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");

  // read analysis config file
  string LineBuffer, DataBuffer, whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 22, "ConfigComptonTelescope") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus) {
      whatToDo = "";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo == "MAX_STRIP_MULTIPLICITY") {
        AnalysisConfigFile >> DataBuffer;
        m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str());
        cout << "\t" << whatToDo << "\t" << m_MaximumStripMultiplicityAllowed << endl;
      }

      else if (whatToDo == "FRONT_BACK_ENERGY_MATCHING_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingSigma = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << "\t" << m_StripEnergyMatchingSigma << endl;
      }

      else if (whatToDo == "FRONT_BACK_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str());
        cout << "\t" << whatToDo << "\t" << m_StripEnergyMatchingNumberOfSigma << endl;
      }

      else if (whatToDo == "DISABLE_ALL") {
        AnalysisConfigFile >> DataBuffer;
        cout << "\t" << whatToDo << "\t" << DataBuffer << endl;
        Int_t Detector = atoi(DataBuffer.substr(2,1).c_str());
        vector< bool > ChannelStatus;
        ChannelStatus.resize(m_NumberOfStrips, false);
        m_FrontChannelStatus[Detector-1] = ChannelStatus;
        m_BackChannelStatus[Detector-1]  = ChannelStatus;
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << "\t" << whatToDo << "\t" << DataBuffer << endl;
        Int_t Detector = atoi(DataBuffer.substr(2,1).c_str());
        Int_t channel = -1;
        if (DataBuffer.compare(3,4,"FRONT") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_FrontChannelStatus[Detector-1].begin()+channel) = false;
        }
        else if (DataBuffer.compare(3,4,"BACK") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_BackChannelStatus[Detector-1].begin()+channel) = false;
        }
      }

      else if (whatToDo=="TAKE_E_FRONT") {
        m_Take_E_Front = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_E_BACK") {
        m_Take_E_Front = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="STRIP_FRONT_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripFront_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripFront_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_BACK_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripBack_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripBack_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_FRONT_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripFront_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripFront_E_Threshold << endl;
      }

      else if (whatToDo=="STRIP_BACK_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripBack_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripBack_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }
    }
  }
  cout << "\t/////////////////////////////////////////////////" << endl;
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::Clear()
{
  EventMultiplicity = 0;

  //   Provide a Classification of Event
  EventType.clear();

  // Detector
  DetectorNumber.clear();

  //   DSSD
  Strip_E.clear();
  Strip_T.clear();
  StripFront_E.clear();
  StripFront_T.clear();
  StripBack_E.clear();
  StripBack_T.clear();
  Strip_Front.clear();
  Strip_Back.clear();
}


///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::ReadConfiguration(NPL::InputParser parser){

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("ComptonTelescope");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"R","THETA","PHI","BETA","SIZE_DSSSD","NUMBER_DSSSD","DISTANCE_INTER_DSSSD","THICKNESS_DSSSD","NUMBER_STRIPS","DISTANCE_TRACKER_CALORIMETER","THICKNESS_CALORIMETER","TRACKER","CALORIMETER","VIS"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg"); 
      double     size = blocks[i]->GetDouble("SIZE_DSSSD","mm");
      int     nbr_det = blocks[i]->GetInt("NUMBER_DSSSD");
      double inter = blocks[i]->GetDouble("DISTANCE_INTER_DSSSD","mm");
      double thickness = blocks[i]->GetDouble("THICKNESS_DSSSD","mm");
      int    nbr_strip = blocks[i]->GetInt("NUMBER_STRIPS");
      double distance_cal = blocks[i]->GetDouble("DISTANCE_TRACKER_CALORIMETER","mm");
      double thickness_cal = blocks[i]->GetDouble("THICKNESS_CALORIMETER","mm");
      int    tracker = blocks[i]->GetInt("TRACKER");
      int    calorimeter = blocks[i]->GetInt("CALORIMETER");
      int    vis= blocks[i]->GetInt("VIS");
      AddComptonTelescope(R);
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
void TComptonTelescopePhysics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    for (int j = 0; j < m_NumberOfStrips; ++j) {
      Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j)+"_E", "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j)+"_E");
      Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j)+"_T", "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j)+"_T");
    }
    for (int j = 0; j < m_NumberOfStrips; ++j) {
      Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j)+"_E",  "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j)+"_E");
      Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j)+"_T",  "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j)+"_T");
    }
  }

  return;  
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeRootInputRaw()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("ComptonTelescope", true);
  inputChain->SetBranchStatus("fCT_*", true);
  inputChain->SetBranchAddress("ComptonTelescope", &m_EventData);
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeRootInputPhysics()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("EventMultiplicity", true);
  inputChain->SetBranchStatus("EventType",         true);
  inputChain->SetBranchStatus("DetectorNumber",    true);
  inputChain->SetBranchStatus("Strip_E",           true);
  inputChain->SetBranchStatus("Strip_T",           true);
  inputChain->SetBranchStatus("StripFront_E",      true);
  inputChain->SetBranchStatus("StripBack_T",       true);
  inputChain->SetBranchStatus("StripFront_E",      true);
  inputChain->SetBranchStatus("StripBack_T",       true);
  inputChain->SetBranchStatus("Strip_Front",       true);
  inputChain->SetBranchStatus("Strip_Back",        true);
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeRootOutput()
{
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("ComptonTelescope", "TComptonTelescopePhysics", &m_EventPhysics);
}


/////   Specific to ComptonTelescopeArray   ////

void TComptonTelescopePhysics::AddComptonTelescope(double Z)
{
  m_NumberOfDetectors++;
  // empty at the moment
  // needed if solid angle analysis are needed
}



TVector3 TComptonTelescopePhysics::GetDetectorNormal( const int i) const{
  /*  TVector3 U =    TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionY( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionZ( DetectorNumber[i] , 24 , 1 ) )

      -TVector3 ( GetStripPositionX( DetectorNumber[i] , 1 , 1 ) ,
      GetStripPositionY( DetectorNumber[i] , 1 , 1 ) ,
      GetStripPositionZ( DetectorNumber[i] , 1 , 1 ) );

      TVector3 V =    TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 48 ) ,
      GetStripPositionY( DetectorNumber[i] , 24 , 48 ) ,
      GetStripPositionZ( DetectorNumber[i] , 24 , 48 ) )

      -TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionY( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionZ( DetectorNumber[i] , 24 , 1 ) );

      TVector3 Normal = U.Cross(V);

      return(Normal.Unit()) ;*/

  return (TVector3(0,0,i));

}

TVector3 TComptonTelescopePhysics::GetPositionOfInteraction(const int i) const{
  TVector3    Position = TVector3 (  GetStripPositionX( DetectorNumber[i] , Strip_Front[i] , Strip_Back[i] )    ,
      GetStripPositionY( DetectorNumber[i] , Strip_Front[i] , Strip_Back[i] )    ,
      GetStripPositionZ( DetectorNumber[i] , Strip_Front[i] , Strip_Back[i] )    ) ;
  return(Position) ;

}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeStandardParameter()
{
  // Enable all channels
  vector<bool> ChannelStatus;
  m_FrontChannelStatus.clear();
  m_BackChannelStatus.clear();

  ChannelStatus.resize(m_NumberOfStrips, true);
  for(Int_t i = 0; i < m_NumberOfDetectors; ++i) {
    m_FrontChannelStatus[i] = ChannelStatus;
    m_BackChannelStatus[i]  = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfDetectors;
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitSpectra()
{
  m_Spectra = new TComptonTelescopeSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::FillSpectra()
{
  m_Spectra->FillRawSpectra(m_EventData);
  m_Spectra->FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra->FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::CheckSpectra()
{
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::ClearSpectra()
{
  // To be done
}



///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::WriteSpectra()
{
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
map<string, TH1*> TComptonTelescopePhysics::GetSpectra() 
{
  if (m_Spectra)
    return m_Spectra->GetMapHisto();
  else {
    map<string, TH1*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
namespace ComptonTelescope_LOCAL
{
  // DSSD
  // Front
  Double_t fStrip_Front_E(const TComptonTelescopeData* m_EventData, const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration("COMPTONTELESCOPE/D" + NPL::itoa(m_EventData->GetCTTrackerFrontEDetectorNbr(i)) + "_STRIP_FRONT" + NPL::itoa(m_EventData->GetCTTrackerFrontEStripNbr(i)) + "_E", m_EventData->GetCTTrackerFrontEEnergy(i));
  }

  // Back
  Double_t fStrip_Back_E(const TComptonTelescopeData* m_EventData, const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration("COMPTONTELESCOPE/D" + NPL::itoa(m_EventData->GetCTTrackerBackEDetectorNbr(i)) + "_STRIP_BACK" + NPL::itoa(m_EventData->GetCTTrackerBackEStripNbr(i)) + "_E", m_EventData->GetCTTrackerBackEEnergy(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TComptonTelescopePhysics::Construct(){
  return (NPL::VDetector*) new TComptonTelescopePhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_comptontelescope{
  public:
    proxy_comptontelescope(){
      NPL::DetectorFactory::getInstance()->AddToken("ComptonTelescope","ComptonTelescope");
      NPL::DetectorFactory::getInstance()->AddDetector("ComptonTelescope",TComptonTelescopePhysics::Construct);
    }
};

proxy_comptontelescope p;
}

