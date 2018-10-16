/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre    contact address: lefebvrl@ipno.in2p3.fr   *
 * Update : V. Girard-Alcindor contact address: girardalcindor@ganil.fr      *
 *                                                                           *
 * Creation Date  : October 2011                                             *
 * Last update    : 2018                                                     *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Exl Detector  Physics                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: Due to the way the detector is build, one has to take into       *
 *          account the cross-talk between two neighboring crystals.         *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TExlPhysics.h"
#include "TExlData.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   ROOT
#include "TChain.h"

//   tranform an integer to a string
string itoa(int value){
    char buffer [33];
    sprintf(buffer,"%d",value);
    return buffer;
}

ClassImp(TExlPhysics);

///////////////////////////////////////////////////////////////////////////
TExlPhysics::TExlPhysics(){
    m_NumberOfDetector=0;
    m_NumberOfCrystal = 18;
    m_EventData = new TExlData;
    m_EventPhysics = this;
    m_E_RAW_Threshold = 0;

    A = 0.;
    B = 0.;
    a = 0.;
    b = 0.;
    v_A.clear();
    v_B.clear();
    v_a.clear();
    v_b.clear();
   
    m_Crystal_i = 0;
    m_NeighborCrystal_i = 0;
    m_Channel_i = 0.;
    m_NeighborChannel_i = 0.;
    m_Energy_i = 0.;

    // std::ifstream pre_calibration_coeff;
    // pre_calibration_coeff.open("/home/alcindor/Documents/Projects/EXL/Analysis/Calibration/pre_calibration_coeff2.txt");

    // std::ifstream calibration_coeff;
    // calibration_coeff.open("/home/alcindor/Documents/Projects/EXL/Analysis/Calibration/Co60_EXL_Calibration.txt");

    // if(v_A.size()==0){
    //     while(pre_calibration_coeff >> B >> A){
    //         v_A.push_back(A);
    //         v_B.push_back(B);
    //     }
    // }
    // if(v_a.size()==0){
    //     while(calibration_coeff >> Channel_name >> b >> a){
    //         v_a.push_back(a);
    //         v_b.push_back(b);
    //     }
    // }
    
    for (int i = 0; i < 30 ; i++){
            v_A.push_back(1);
            v_B.push_back(1);
            v_a.push_back(1);
            v_b.push_back(1);
    }
}

///////////////////////////////////////////////////////////////////////////
TExlPhysics::~TExlPhysics(){
}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::Clear(){
    CrystalNumber.clear() ;
    Energy.clear() ;
    RawEnergy.clear();
    Time.clear() ;
    AddBack.clear();
}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::ReadConfiguration(NPL::InputParser parser){
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("EXL");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl;

    vector<string> token = {"POS"};
    vector<TVector3> Center_CsI_Crystals;
    for(unsigned int i = 0; i < blocks.size(); i++){
        if(blocks[i]->HasTokenList(token)){
            TVector3 pos = blocks[i]->GetTVector3("POS","mm");
            Center_CsI_Crystals.push_back(pos);
        }
        else{
            cout << "ERROR: check your input file formatting " << endl;
            exit(1);
        }
    }
    AddEXL(Center_CsI_Crystals) ;
}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::AddEXL(vector <TVector3> Center_CsI_Crystals){
    m_NumberOfDetector++;
    // In progress
    // Needs the detectors dimensions
    // Crystals in this detector can be mapped differently for every module
    // Rows_y (0,1,2) Cols_x (0,1,2,3,4,5)
    // local zero is at row 1, col 2.5
    double length = 110; // detector arbit. face length and width, 100 mm
    double width  = 7.2;
    double height = 9.8;

    for(unsigned i=0; i<Center_CsI_Crystals.size(); i++){
        TVector3 direction = Center_CsI_Crystals.at(i).Unit();
        //place all 18 crystals in place
        for(unsigned iCry=0; iCry<18; iCry++){
            int Row_y = iCry/6;
            int Col_x = iCry%6;
            // Define Detector position localy
            TVector3 localPos((Row_y-1)*length,(Col_x-2.5)*length,0);
            // Rotate
            localPos.RotateUz(direction);
            TVector3 globalPos = Center_CsI_Crystals.at(i)+localPos;
            CsIPosition.push_back(globalPos);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
TVector3 TExlPhysics::GetPositionOfInteraction(int det, int cry){
    int N = det*18+cry; // hyper crystal number
    TVector3 Position = TVector3(CsIPosition.at(N).X(),
            CsIPosition.at(N).Y(),
            CsIPosition.at(N).Z());
    return(Position) ;

}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::AddParameterToCalibrationManager(){
    CalibrationManager* Cal = CalibrationManager::getInstance();
    for(unsigned int i = 0 ; i < m_NumberOfCrystal ; i++){
        Cal->AddParameter("EXL", "E_"+ NPL::itoa(i+1), "EXL_E_" + NPL::itoa(i+1));
        Cal->AddParameter("EXL", "T_"+ NPL::itoa(i+1), "EXL_T_" + NPL::itoa(i+1));
    }
}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("EXL",    true);
  inputChain->SetBranchStatus("fExl_*", true);
  inputChain->SetBranchAddress("EXL", &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::InitializeRootInputPhysics(){
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus("EXL",           true);
    inputChain->SetBranchStatus("CrystalNumber", true);
    inputChain->SetBranchStatus("EXL_Energy",    true);
    inputChain->SetBranchStatus("EXL_AddBack", true );
    inputChain->SetBranchStatus("EXL_Time", true );
    inputChain->SetBranchAddress("EXL", &m_EventPhysics);
    inputChain->SetBranchAddress("EXL", &m_EventData );
}
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::InitializeRootOutput(){
    TTree* outputTree = RootOutput::getInstance()->GetTree();
    outputTree->Branch("EXL", "TExlPhysics", &m_EventPhysics);
}

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::BuildPhysicalEvent(){

  unsigned int sizeE = m_EventData->GetEMult();
  for(unsigned int i = 0 ; i < sizeE ; i++){

    CrystalNumber.push_back(m_EventData->GetE_CrystalNumber(i));

    double energy = CalibrationManager::getInstance()->ApplyCalibration(
        "EXL/E_" + NPL::itoa(m_EventData->GetE_CrystalNumber(i)), 
        m_EventData->GetEnergy(i));

    Energy.push_back(energy);

    // look for associated time
    bool check = false;
    unsigned int sizeT = m_EventData->GetTMult();
    for(unsigned int j = 0 ; j < sizeT ; j++){
       if(m_EventData->GetT_CrystalNumber(j)==m_EventData->GetE_CrystalNumber(i)){

          double time = CalibrationManager::getInstance()->ApplyCalibration(
          "EXL/T_" + NPL::itoa(m_EventData->GetT_CrystalNumber(j)), 
          m_EventData->GetTime(j));
          // cout << time << endl;

         Time.push_back(time);

         check = true;
         break;
      }
    }
    if(!check)
      Time.push_back(-1000);

  }

    //     for(int m_Crystal_i = 1; m_Crystal_i < 19; m_Crystal_i++){

    //         // Getting NeighborCrystalNumber
    //         if(m_Crystal_i % 2 == 0){
    //             m_NeighborCrystal_i = m_Crystal_i - 1;     
    //         }
    //         else{
    //             m_NeighborCrystal_i = m_Crystal_i + 1; 
    //         }

    //         // Getting channel of both crystals 
    //         m_Channel_i = m_EventData->GetEnergyCrystalNumber(m_Crystal_i);
    //         m_NeighborChannel_i = m_EventData->GetEnergyCrystalNumber(m_NeighborCrystal_i);

    //         // Getting time
    //         Time.push_back(m_EventData->GetTimeCrystalNumber(m_Crystal_i));

    //         if (m_Channel_i != -10){
    //             if(m_NeighborChannel_i > 1){
    //                 m_Channel_i = m_Channel_i 
    //                     - v_A.at(m_NeighborCrystal_i - 1) * m_NeighborChannel_i 
    //                     - v_B.at(m_NeighborCrystal_i - 1);
    //             }
    //             if (m_Channel_i < 0){
    //                 m_Channel_i = 0.;
    //             }

    //             m_Energy_i = v_a.at(m_Crystal_i - 1) * m_Channel_i 
    //                 + v_b.at(m_Crystal_i - 1);

    //             CrystalNumber.push_back(m_Crystal_i);
    //             Energy.push_back(m_Energy_i);
    //         }
    //     }

    //     // AddBack
    //     // double E = 0;
    //     // unsigned int Energy_size = Energy.size();
    //     // for(unsigned int k = 0 ; k < Energy_size; k++){
    //     //     E += Energy.at(k);
    //     // }
    //     AddBack.push_back(0);
  }

  ///////////////////////////////////////////////////////////////////////////
  void TExlPhysics::BuildSimplePhysicalEvent(){
  }

  ///////////////////////////////////////////////////////////////////////////
  double TExlPhysics::DopplerCorrection(double E, double Theta, double beta){
    // double E_corr = 0;
    // double gamma = 1./sqrt(1-beta*beta);
    // E_corr = gamma*E*(1.-beta*cos(Theta*M_PI/180.));
    // E_corr = E;
    // return(E_corr);
    return 0.0;
  }

  /////////////////////////////////////////////////////////////////////////
  // Spectra function needed by the spectra server 
  //////////////////////////////////////////////////////////////////////////
  void TExlPhysics::InitSpectra(){
    m_Spectra = new TExlSpectra(m_NumberOfDetector);
  }

  ///////////////////////////////////////////////////////////////////////////
  void TExlPhysics::FillSpectra(){
    m_Spectra -> FillRawSpectra(m_EventData);
    // m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
    m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
  }
  ///////////////////////////////////////////////////////////////////////////
  void TExlPhysics::CheckSpectra(){
    m_Spectra->CheckSpectra();
  }
  ///////////////////////////////////////////////////////////////////////////
  void TExlPhysics::ClearSpectra(){
    // To be done
  }

  ///////////////////////////////////////////////////////////////////////////
  void TExlPhysics::WriteSpectra() {
    if(m_Spectra){
      m_Spectra->WriteSpectra();
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  map< string , TH1*> TExlPhysics::GetSpectra() {
    if(m_Spectra)
      return m_Spectra->GetMapHisto();
    else{
      map< string , TH1*> empty;
      return empty;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////
  //            Construct Method to be pass to the DetectorFactory              //
  ////////////////////////////////////////////////////////////////////////////////
  NPL::VDetector* TExlPhysics::Construct(){
    return (NPL::VDetector*) new TExlPhysics();
  }

  ////////////////////////////////////////////////////////////////////////////////
  //            Registering the construct method to the factory                 //
  ////////////////////////////////////////////////////////////////////////////////
  extern "C"{
    class proxy_exl{
      public:
        proxy_exl(){
          NPL::DetectorFactory::getInstance()->AddToken("EXL","EXL");
          NPL::DetectorFactory::getInstance()->AddDetector("EXL",TExlPhysics::Construct);
        }
    };

    proxy_exl p_exl;
  }
