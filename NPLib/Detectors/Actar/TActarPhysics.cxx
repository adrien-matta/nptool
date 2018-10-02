/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Actar Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TActarPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPLNPSystemOfUnits.h
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
#include "NPSystemOfUnits.h"

//   ROOT
#include "TChain.h"
#include "TH2F.h"
#include "TCanvas.h"

ClassImp(TActarPhysics)


///////////////////////////////////////////////////////////////////////////
TActarPhysics::TActarPhysics()
: m_EventData(new TActarData),
m_EventReduced(new MEventReduced),
m_PreTreatedData(new TActarData),
m_EventPhysics(this),

m_Spectra(0),
fRecoRansac(1),
fRecoCluster(0),
fRecoVisu(0),
fHitThreshold(20),
fQ_Threshold(0),
fT_Threshold(0),
fXBeamMin(0),
fXBeamMax(128),
fYBeamMin(60),
fYBeamMax(67),
fNumberOfPadsX(128),
fNumberOfPadsY(128),
fPadSizeX(2),
fPadSizeY(2),
fDriftVelocity(40),
fPressure(100),
fGas("iC4H10"),
m_NumberOfPadSilicon(20),
m_NumberOfDetectors(0) {
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TActarPhysics::AddDetector(TVector3 , string ){
    // In That simple case nothing is done
    // Typically for more complex detector one would calculate the relevant
    // positions (stripped silicon) or angles (gamma array)
    m_NumberOfDetectors++;
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::AddDetector(double R, double Theta, double Phi, string shape){
    // Compute the TVector3 corresponding
    TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
    // Call the cartesian method
    AddDetector(Pos,shape);
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::BuildSimplePhysicalEvent() {
    BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::BuildPhysicalEvent() {

    PreTreat();

    //unsigned int mysize = m_PreTreatedData->GetPadMult();

    /*for (unsigned int e = 0; e < mysize; e++) {
        PadX.push_back(m_PreTreatedData->GetPadX(e));
        PadY.push_back(m_PreTreatedData->GetPadY(e));
        PadZ.push_back(m_PreTreatedData->GetPadZ(e));
        PadCharge.push_back(m_PreTreatedData->GetPadCharge(e));
    }*/
    if(fRecoRansac && PadX.size()>fHitThreshold){
        m_Ransac->Init(PadX, PadY, PadZ, PadCharge);
        m_Track = m_Ransac->SimpleRansac();
    }

    else if(fRecoCluster){
      if(PadX.size()>fHitThreshold){
        m_Cluster->Init(PadX, PadY, PadZ, PadCharge);
        m_Cluster->FindTracks();
      }

      if(BeamPadX.size()>fHitThreshold){
        m_Cluster->Init(BeamPadX, BeamPadY, BeamPadZ, BeamPadCharge);
        m_Cluster->FindTracks();
      }

      m_Track = m_Cluster->GetTracks();

      m_Cluster->Clear();
    }

    TrackMult = m_Track.size();
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::PreTreat() {
    // This method typically applies thresholds and calibrations
    // Might test for disabled channels for more complex detector

    // clear pre-treated object
    ClearPreTreatedData();

    CleanPads();

    // instantiate CalibrationManager
    static CalibrationManager* Cal = CalibrationManager::getInstance();


    unsigned int mysize = m_EventReduced->CoboAsad.size();

    for (unsigned int it = 0; it < mysize ; ++it) {
        int co=m_EventReduced->CoboAsad[it].globalchannelid>>11;
        int as=(m_EventReduced->CoboAsad[it].globalchannelid - (co<<11))>>9;
        int ag=(m_EventReduced->CoboAsad[it].globalchannelid - (co<<11)-(as<<9))>>7;
        int ch=m_EventReduced->CoboAsad[it].globalchannelid - (co<<11)-(as<<9)-(ag<<7);
        int where=co*NumberOfASAD*NumberOfAGET*NumberOfChannel + as*NumberOfAGET*NumberOfChannel + ag*NumberOfChannel + ch;

        if(co!=31){
            unsigned int vector_size = m_EventReduced->CoboAsad[it].peakheight.size();
            for(unsigned int hh=0; hh<vector_size; hh++){
                if(m_EventReduced->CoboAsad[it].peakheight[hh]>fQ_Threshold && m_EventReduced->CoboAsad[it].peaktime[hh]>fT_Threshold){
                    if(GoodHit(TABLE[4][where],TABLE[5][where])){
                    //if(Hit[TABLE[4][where]][TABLE[5][where]]<2){
                        if(fRecoCluster){
                            if(IsBeamZone(TABLE[4][where],TABLE[5][where])){
                                BeamPadCharge.push_back(m_EventReduced->CoboAsad[it].peakheight[hh]);
                                BeamPadX.push_back(TABLE[4][where]);
                                BeamPadY.push_back(TABLE[5][where]);
                                BeamPadZ.push_back(m_EventReduced->CoboAsad[it].peaktime[hh]);
                            }
                            else{
                                PadCharge.push_back(m_EventReduced->CoboAsad[it].peakheight[hh]);
                                PadX.push_back(TABLE[4][where]);
                                PadY.push_back(TABLE[5][where]);
                                PadZ.push_back(m_EventReduced->CoboAsad[it].peaktime[hh]);
                            }

                        }
                        else if(fRecoRansac){
                            PadCharge.push_back(m_EventReduced->CoboAsad[it].peakheight[hh]);
                            PadX.push_back(TABLE[4][where]);
                            PadY.push_back(TABLE[5][where]);
                            PadZ.push_back(m_EventReduced->CoboAsad[it].peaktime[hh]);
                        }
                        //}
                    }
                }
            }
        }
        else if(co==31){
            unsigned int vector_size = m_EventReduced->CoboAsad[it].peakheight.size();
            for(unsigned int hit=0;hit<vector_size;hit++){
                if(fInputTreeName=="ACTAR_TTree"){
                  int vxi_parameter = m_EventReduced->CoboAsad[it].peaktime[hit];

                  if(Si_map[vxi_parameter]<21 && Si_map[vxi_parameter]>0){
                    double RawEnergy = m_EventReduced->CoboAsad[it].peakheight[hit];

                    static string name;
                    name = "ActarSi/D" ;
                    name+= NPL::itoa( Si_map[vxi_parameter] - 1) ;
                    name+= "_E" ;
                    double CalEnergy = CalibrationManager::getInstance()->ApplyCalibration( name, RawEnergy );

                    Si_Number.push_back(Si_map[vxi_parameter]);
                    Si_E.push_back(CalEnergy);
                  }
                }
                else{
                  Si_Number.push_back(m_EventReduced->CoboAsad[it].peaktime[hit]);
                  Si_E.push_back(m_EventReduced->CoboAsad[it].peakheight[hit]);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
bool TActarPhysics::IsBeamZone(int X, int Y)
{
    bool isBeam=false;
    if( (X>=fXBeamMin && X<=fXBeamMax) && (Y>=fYBeamMin && Y<=fYBeamMax) ){
        isBeam=true;
    }

    return isBeam;
}

///////////////////////////////////////////////////////////////////////////
bool TActarPhysics::GoodHit(int iX, int iY)
{
    bool bHit = true;
    if(Hit[iX][iY]<2){
        if(Hit[iX+1][iY]>0 || Hit[iX+1][iY-1]>0 || Hit[iX+1][iY+1]>0){
            if(Hit[iX+2][iY]>0 || Hit[iX+2][iY-1]>0 || Hit[iX+2][iY+1]>0){
                bHit = true;
            }
        }
    }

    return bHit;
}


///////////////////////////////////////////////////////////////////////////
void TActarPhysics::CleanPads()
{
    unsigned int mysize = m_EventReduced->CoboAsad.size();

    for(unsigned int it=0; it<mysize; it++){
        int co=m_EventReduced->CoboAsad[it].globalchannelid>>11;
        int as=(m_EventReduced->CoboAsad[it].globalchannelid - (co<<11))>>9;
        int ag=(m_EventReduced->CoboAsad[it].globalchannelid - (co<<11)-(as<<9))>>7;
        int ch=m_EventReduced->CoboAsad[it].globalchannelid - (co<<11)-(as<<9)-(ag<<7);
        int where=co*NumberOfASAD*NumberOfAGET*NumberOfChannel + as*NumberOfAGET*NumberOfChannel + ag*NumberOfChannel + ch;


        if(co!=31){
            unsigned int vector_size = m_EventReduced->CoboAsad[it].peakheight.size();
            for(unsigned int hh=0; hh < vector_size; hh++){
                if(m_EventReduced->CoboAsad[it].peakheight[hh]>fQ_Threshold && m_EventReduced->CoboAsad[it].peaktime[hh]>fT_Threshold){
                    Hit[TABLE[4][where]][TABLE[5][where]] += 1;
                    /*
                    m_PreTreatedData->SetCharge(m_EventReduced->CoboAsad[it].peakheight[hh]);
                    m_PreTreatedData->SetPadX(TABLE[4][where]);
                    m_PreTreatedData->SetPadY(TABLE[5][where]);
                    m_PreTreatedData->SetPadZ(m_EventReduced->CoboAsad[it].peaktime[hh]);*/
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::ReadAnalysisConfig() {
    // VXI ACTION FILE //
    string VXI_FileName = "./configs/ACTION_Si_config.dat";
    ifstream VXIConfigFile;
    VXIConfigFile.open(VXI_FileName.c_str());
    if(!VXIConfigFile.is_open()){
        cout << "No VXI ACTION FILE ./configs/ACTION_Si_config.dat found!" << endl;
        return;
    }
    else{
        cout << "/// Using VXI ACTION FILE: " << VXI_FileName << " ///" << endl;
        string token;
        int vxi_param, si_nbr;
        for(int i=0; i<20; i++){
            VXIConfigFile >> token >> vxi_param >> si_nbr;
            Si_map[vxi_param] = si_nbr+1;
        }
    }
    VXIConfigFile.close();

    // Lookup table //
    bool ReadingLookupTable = false;
    string LT_FileName = "./configs/LT.dat";
    ifstream LTConfigFile;
    LTConfigFile.open(LT_FileName.c_str());
    if(!LTConfigFile.is_open()){
        cout << "No Lookup Table in ./configs/LT.dat found!" << endl;
        return;
    }
    else{
        cout << "/// Using LookupTable from: " << LT_FileName << " ///" << endl;
        for(int i=0;i<NumberOfCobo*NumberOfASAD*NumberOfAGET*NumberOfChannel;i++){
            LTConfigFile >> TABLE[0][i] >> TABLE[1][i] >> TABLE[2][i] >> TABLE[3][i] >> TABLE[4][i] >> TABLE[5][i];
        }
        ReadingLookupTable = true;
    }
    LTConfigFile.close();


    bool ReadingStatus = false;

    // ACTAR config file //
    string FileName = "./configs/ConfigActar.dat";

    // open analysis config file
    ifstream AnalysisConfigFile;
    AnalysisConfigFile.open(FileName.c_str());

    if (!AnalysisConfigFile.is_open()) {
        cout << " No ConfigActar.dat found: Default parameter loaded for Analayis " << FileName << endl;
        return;
    }
    cout << "/// Loading user parameter for Analysis from ConfigActar.dat ///" << endl;

    // Save it in a TAsciiFile
    TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
    asciiConfig->AppendLine("%%% ConfigActar.dat %%%");
    asciiConfig->Append(FileName.c_str());
    asciiConfig->AppendLine("");
    // read analysis config file
    string LineBuffer,DataBuffer,whatToDo;
    while (!AnalysisConfigFile.eof()) {
        // Pick-up next line
        getline(AnalysisConfigFile, LineBuffer);

        // search for "header"
        string name = "ConfigActar";
        if (LineBuffer.compare(0, name.length(), name) == 0){
            ReadingStatus = true;
            cout << "**** ConfigActar found" << endl;
        }

        // loop on tokens and data
        while (ReadingStatus ) {
            whatToDo="";
            AnalysisConfigFile >> whatToDo;

            // Search for comment symbol (%)
            if (whatToDo.compare(0, 1, "%") == 0) {
                AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
            }

            else if (whatToDo.compare(0,11,"RecoRansac=") == 0) {
                AnalysisConfigFile >> DataBuffer;
                fRecoRansac = atoi(DataBuffer.c_str());
                cout << "/// Reco using Ransac= " << " " << fRecoRansac << " ///" << endl;
            }

            else if (whatToDo.compare(0,12,"RecoCluster=") == 0) {
                AnalysisConfigFile >> DataBuffer;
                fRecoCluster = atoi(DataBuffer.c_str());
                cout << "/// Reco using Cluster= " << " " << fRecoCluster << " ///" << endl;
            }

            else if (whatToDo.compare(0,9,"RecoVisu=") == 0) {
                AnalysisConfigFile >> DataBuffer;
                fRecoVisu = atoi(DataBuffer.c_str());
                cout << "/// Visualisation= " << " " << fRecoVisu << " ///" << endl;
            }

            else if (whatToDo.compare(0,14,"HIT_THRESHOLD=") == 0) {
                AnalysisConfigFile >> DataBuffer;
                fHitThreshold = atoi(DataBuffer.c_str());
                cout << "/// Hit Threshold= " << " " << fHitThreshold << " ///" << endl;
            }

            else if (whatToDo.compare(0,12,"Q_THRESHOLD=") == 0) {
                AnalysisConfigFile >> DataBuffer;
                fQ_Threshold = atof(DataBuffer.c_str());
                cout << "/// Q Threshold= " << " " << fQ_Threshold << " ///" << endl;
            }

            else if (whatToDo.compare(0,12,"T_THRESHOLD=") == 0) {
                AnalysisConfigFile >> DataBuffer;
                fT_Threshold = atof(DataBuffer.c_str());
                cout << "/// T Threshold= " << " " << fT_Threshold << " ///" << endl;
            }
            else if(whatToDo.compare(0,14,"NumberOfPadsX=")==0){
                AnalysisConfigFile >> DataBuffer;
                fNumberOfPadsX = atoi(DataBuffer.c_str());
                //check_padsX=true;
                cout << "/// Number Of Pads X= " << fNumberOfPadsX << " ///" << endl;
            }

            else if(whatToDo.compare(0,14,"NumberOfPadsY=")==0){
                AnalysisConfigFile >> DataBuffer;
                fNumberOfPadsY = atoi(DataBuffer.c_str());
                //check_padsY=true;
                cout << "/// Number Of Pads Y= " << fNumberOfPadsY << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"PadSizeX=")==0){
                AnalysisConfigFile >> DataBuffer;
                fPadSizeX = atof(DataBuffer.c_str());
                //check_sizeX=true;
                cout << "/// Pad Size X= " << fPadSizeX << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"PadSizeY=")==0){
                AnalysisConfigFile >> DataBuffer;
                fPadSizeY = atof(DataBuffer.c_str());
                //check_sizeY=true;
                cout << "/// Pad Size Y= " << fPadSizeY << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"XBeamMin=")==0){
                AnalysisConfigFile >> DataBuffer;
                fXBeamMin = atof(DataBuffer.c_str());
                cout << "/// X Beam Min= " << fXBeamMin << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"XBeamMax=")==0){
                AnalysisConfigFile >> DataBuffer;
                fXBeamMax = atof(DataBuffer.c_str());
                cout << "/// X Beam Max= " << fXBeamMax << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"YBeamMin=")==0){
                AnalysisConfigFile >> DataBuffer;
                fYBeamMin = atof(DataBuffer.c_str());
                cout << "/// Y Beam Min= " << fYBeamMin << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"YBeamMax=")==0){
                AnalysisConfigFile >> DataBuffer;
                fYBeamMax = atof(DataBuffer.c_str());
                cout << "/// Y Beam Max= " << fYBeamMax << " ///" << endl;
            }

            else if(whatToDo.compare(0,19,"NumberOfPadSilicon=")==0){
                AnalysisConfigFile >> DataBuffer;
                m_NumberOfPadSilicon = atof(DataBuffer.c_str());
                //check_pressure=true;
                cout << "/// Number of Silicons= " << m_NumberOfPadSilicon << " ///" << endl;
            }

            else if(whatToDo.compare(0,9,"Pressure=")==0){
                AnalysisConfigFile >> DataBuffer;
                fPressure = atof(DataBuffer.c_str());
                //check_pressure=true;
                cout << "/// Pressure= " << fPressure << " ///" << endl;
            }

            else if(whatToDo.compare(0,14,"DriftVelocity=")==0){
                AnalysisConfigFile >> DataBuffer;
                fDriftVelocity = atof(DataBuffer.c_str());
                //check_driftvelocity=true;
                cout << "/// Drift Velocity= " << fDriftVelocity << " ///" << endl;
            }

            else if(whatToDo.compare(0,4,"Gas=")==0){
                AnalysisConfigFile >> DataBuffer;
                fGas = DataBuffer.c_str();
                //check_gas=true;
                cout << "/// Gas Type= " << fGas << " ///" << endl;
            }

            else {
                ReadingStatus = false;
            }
        }
    }

    if(fRecoRansac){
        m_Ransac = new NPL::Ransac(fNumberOfPadsX,fNumberOfPadsY,fRecoVisu);
    }
    else if(fRecoCluster){
        m_Cluster = new NPL::Cluster(fNumberOfPadsX,fNumberOfPadsY,fRecoVisu);
    }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::SetRansacParameter(string filename){
    if(fRecoRansac){
        m_Ransac->ReadParameterValue(filename);
    }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::SetClusterParameter(string filename){
    if(fRecoCluster){
        m_Cluster->ReadParameterValue(filename);
    }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::Clear() {
    BeamPadX.clear();
    BeamPadY.clear();
    BeamPadZ.clear();
    BeamPadCharge.clear();

    PadX.clear();
    PadY.clear();
    PadZ.clear();
    PadCharge.clear();
    m_Track.clear();
    Si_Number.clear();
    Si_E.clear();

    for(int i=0; i<fNumberOfPadsX; i++){
        for(int j=0; j<fNumberOfPadsY; j++){
            Hit[i][j] = 0;
        }
    }
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::ReadConfiguration(NPL::InputParser parser) {
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Actar");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl;

    vector<string> cart = {"POS","Shape"};
    vector<string> sphe = {"R","Theta","Phi","Shape"};

    for(unsigned int i = 0 ; i < blocks.size() ; i++){
        if(blocks[i]->HasTokenList(cart)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  Actar " << i+1 <<  endl;

            TVector3 Pos = blocks[i]->GetTVector3("POS","mm");
            string Shape = blocks[i]->GetString("Shape");
            AddDetector(Pos,Shape);
        }
        else if(blocks[i]->HasTokenList(sphe)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  Actar " << i+1 <<  endl;
            double R = blocks[i]->GetDouble("R","mm");
            double Theta = blocks[i]->GetDouble("Theta","deg");
            double Phi = blocks[i]->GetDouble("Phi","deg");
            string Shape = blocks[i]->GetString("Shape");
            AddDetector(R,Theta,Phi,Shape);
        }
        else{
            cout << "ERROR: check your input file formatting " << endl;
            exit(1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitSpectra() {
    m_Spectra = new TActarSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::FillSpectra() {
    m_Spectra -> FillRawSpectra(m_EventData);
    m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
    m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::CheckSpectra() {
    m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::ClearSpectra() {
    // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TActarPhysics::GetSpectra() {
    if(m_Spectra)
        return m_Spectra->GetMapHisto();
    else{
        map< string , TH1*> empty;
        return empty;
    }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TActarPhysics::GetCanvas() {
    //if(m_Spectra)
    //return m_Spectra->GetCanvas();
    //else{
    vector<TCanvas*> empty;
    return empty;
    //}
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::WriteSpectra() {
    m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::AddParameterToCalibrationManager() {
    CalibrationManager* Cal = CalibrationManager::getInstance();
    for (int i = 0; i < m_NumberOfDetectors; ++i) {
        Cal->AddParameter("Actar", "D"+ NPL::itoa(i+1)+"_CHARGE","Actar_D"+ NPL::itoa(i+1)+"_CHARGE");
        Cal->AddParameter("Actar", "D"+ NPL::itoa(i+1)+"_TIME","Actar_D"+ NPL::itoa(i+1)+"_TIME");
    }

    for(int i=0; i<m_NumberOfPadSilicon; i++){
      Cal->AddParameter("ActarSi","D"+NPL::itoa(i)+"_E","ActarSi_D"+NPL::itoa(i)+"_E");
    }
}



///////////////////////////////////////////////////////////////////////////
/*void TActarPhysics::InitializeRootInputRaw() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus("Actar",  true );
    inputChain->SetBranchAddress("Actar", &m_EventData );
}*/

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitializeRootInputRaw() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus("data",  true );
    inputChain->SetBranchStatus("ACTAR_TTree",  true );
    inputChain->SetBranchAddress("data", &m_EventReduced );

    fInputTreeName = inputChain->GetTree()->GetName();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitializeRootInputPhysics() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchAddress("Actar", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitializeRootOutput() {
    TTree* outputTree = RootOutput::getInstance()->GetTree();
    outputTree->Branch("Actar", "TActarPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TActarPhysics::Construct() {
    return (NPL::VDetector*) new TActarPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_Actar{
    public:
        proxy_Actar(){
            NPL::DetectorFactory::getInstance()->AddToken("Actar","Actar");
            NPL::DetectorFactory::getInstance()->AddDetector("Actar",TActarPhysics::Construct);
        }
    };

    proxy_Actar p_Actar;
}
