/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    : october 2010                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Mugast treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TMugastPhysics.h"
using namespace MUGAST_LOCAL;

//   STL
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdlib.h>

//   NPL
#include "NPDetectorFactory.h"
#include "NPInputParser.h"
#include "NPOptionManager.h"
#include "NPSystemOfUnits.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
using namespace NPUNITS;
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TMugastPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TMugastPhysics::TMugastPhysics() {
    EventMultiplicity                  = 0;
    m_EventData                        = new TMugastData;
    m_PreTreatedData                   = new TMugastData;
    m_EventPhysics                     = this;
    m_Spectra                          = NULL;
    m_NumberOfTelescope                = 0;
    m_MaximumStripMultiplicityAllowed  = 10;
    m_StripEnergyMatching = 0.050;
    // Raw Threshold
    m_DSSD_X_E_RAW_Threshold = 8200;
    m_DSSD_Y_E_RAW_Threshold = 8200;
    m_SecondLayer_E_RAW_Threshold = 8200;
    // Calibrated Threshold
    m_DSSD_X_E_Threshold = 0;
    m_DSSD_Y_E_Threshold = 0;
    m_SecondLayer_E_Threshold  = 0;

    m_Take_E_Y = false;
    m_Take_T_Y = true;
  }

///////////////////////////////////////////////////////////////////////////
TMugastPhysics::~TMugastPhysics() {}
///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::BuildSimplePhysicalEvent() { BuildPhysicalEvent(); }

//////////////////////////////////////////////////////////////////////////
void TMugastPhysics::PreTreat() {
  ClearPreTreatedData();
  static unsigned int DSSDX_EMult, DSSDY_EMult, SecondLayer_EMult;
  static unsigned int DSSDX_TMult, DSSDY_TMult, SecondLayer_TMult;
  DSSDX_EMult = m_EventData->GetDSSDXEMult();
  DSSDY_EMult = m_EventData->GetDSSDYEMult();
  DSSDX_TMult = m_EventData->GetDSSDXTMult();
  DSSDY_TMult = m_EventData->GetDSSDYTMult();
  SecondLayer_EMult = m_EventData->GetSecondLayerEMult();
  SecondLayer_TMult = m_EventData->GetSecondLayerTMult();

  //   X
  //   E
  for (unsigned int i = 0; i < DSSDX_EMult; ++i) {
    if (m_EventData->GetDSSDXEEnergy(i) > m_DSSD_X_E_RAW_Threshold
        && IsValidChannel(0, m_EventData->GetDSSDXEDetectorNbr(i),
          m_EventData->GetDSSDXEStripNbr(i))) {
      double EX = fDSSD_X_E(m_EventData, i);
      if (EX > m_DSSD_X_E_Threshold)
        m_PreTreatedData->SetDSSDXE(MG_NOCHANGE,
        m_EventData->GetDSSDXEDetectorNbr(i),
        m_EventData->GetDSSDXEStripNbr(i), EX);
    }
  }
  //   T
  for (unsigned int i = 0; i < DSSDX_TMult; ++i) {
    if (IsValidChannel(0, m_EventData->GetDSSDXTDetectorNbr(i),
          m_EventData->GetDSSDXTStripNbr(i)))
      m_PreTreatedData->SetDSSDXT(MG_NOCHANGE,
m_EventData->GetDSSDXTDetectorNbr(i),
          m_EventData->GetDSSDXTStripNbr(i),
          fDSSD_X_T(m_EventData, i));
  }

  //   Y
  //   E
  for (unsigned int i = 0; i < DSSDY_EMult; ++i) {
    if (m_EventData->GetDSSDYEEnergy(i) < m_DSSD_Y_E_RAW_Threshold
        && IsValidChannel(1, m_EventData->GetDSSDYEDetectorNbr(i),
          m_EventData->GetDSSDYEStripNbr(i))) {
      double EY = fDSSD_Y_E(m_EventData, i);
      if (EY > m_DSSD_Y_E_Threshold)
        m_PreTreatedData->SetDSSDYE(MG_NOCHANGE,
m_EventData->GetDSSDYEDetectorNbr(i),
            m_EventData->GetDSSDYEStripNbr(i), EY);
    }
  }

  //   T
  for (unsigned int i = 0; i < DSSDY_TMult; ++i) {
    if (IsValidChannel(1, m_EventData->GetDSSDYTDetectorNbr(i),
          m_EventData->GetDSSDYTStripNbr(i)))
      m_PreTreatedData->SetDSSDYT(MG_NOCHANGE,
m_EventData->GetDSSDYTDetectorNbr(i),
          m_EventData->GetDSSDYTStripNbr(i),
          fDSSD_Y_T(m_EventData, i));
  }

  //   SecondLayer
  //   E
  for (unsigned int i = 0; i < SecondLayer_EMult; ++i) {
    if (m_EventData->GetSecondLayerEEnergy(i) > m_SecondLayer_E_RAW_Threshold
        && IsValidChannel(2, m_EventData->GetSecondLayerEDetectorNbr(i),
          m_EventData->GetSecondLayerEStripNbr(i))) {
      double ESecondLayer = fSecondLayer_E(m_EventData, i);
      if (ESecondLayer > m_SecondLayer_E_Threshold)
        m_PreTreatedData->SetSecondLayerE(m_EventData->GetSecondLayerEDetectorNbr(i),
            m_EventData->GetSecondLayerEStripNbr(i), ESecondLayer);
    }
  }

  //   T
  for (unsigned int i = 0; i < SecondLayer_TMult; ++i) {
    if (IsValidChannel(2, m_EventData->GetSecondLayerTDetectorNbr(i),
          m_EventData->GetSecondLayerTStripNbr(i)))
      m_PreTreatedData->SetSecondLayerT(m_EventData->GetSecondLayerTDetectorNbr(i),
          m_EventData->GetSecondLayerTStripNbr(i),
          fSecondLayer_T(m_EventData, i));
  }

  return;
}


///////////////////////////////////////////////////////////////////////////

void TMugastPhysics::BuildPhysicalEvent() {

  PreTreat();

  bool check_SILI = false;
  bool check_SecondLayer  = false;
  static unsigned int DSSDXEMult, DSSDYEMult, DSSDXTMult, DSSDYTMult,SecondLayerEMult,SecondLayerTMult; 
  DSSDXEMult = m_PreTreatedData->GetDSSDXEMult();
  DSSDYEMult = m_PreTreatedData->GetDSSDYEMult();
  DSSDXTMult = m_PreTreatedData->GetDSSDXTMult();
  DSSDYTMult = m_PreTreatedData->GetDSSDYTMult();
  SecondLayerEMult    = m_PreTreatedData->GetSecondLayerEMult();
  SecondLayerTMult    = m_PreTreatedData->GetSecondLayerTMult();

  if (1 /*CheckEvent() == 1*/) {
    vector<TVector2> couple = Match_X_Y();

    EventMultiplicity = couple.size();
    for (unsigned int i = 0; i < couple.size(); ++i) {
      check_SILI = false;
      check_SecondLayer  = false;

      int N = m_PreTreatedData->GetDSSDXEDetectorNbr(couple[i].X());

      int X = m_PreTreatedData->GetDSSDXEStripNbr(couple[i].X());
      int Y = m_PreTreatedData->GetDSSDYEStripNbr(couple[i].Y());

      double DSSD_X_E = m_PreTreatedData->GetDSSDXEEnergy(couple[i].X());
      double DSSD_Y_E = m_PreTreatedData->GetDSSDYEEnergy(couple[i].Y());

      //  Search for associate Time
      double DSSD_X_T = -1000;
      for (unsigned int t = 0; t < DSSDXTMult; ++t) {
        if (m_PreTreatedData->GetDSSDXTStripNbr(couple[i].X())
            == m_PreTreatedData->GetDSSDXTStripNbr(t)
            && m_PreTreatedData->GetDSSDXTDetectorNbr(couple[i].X())
            == m_PreTreatedData->GetDSSDXTDetectorNbr(t)) {
          DSSD_X_T = m_PreTreatedData->GetDSSDXTTime(t);
          break;
        }
      }

      double DSSD_Y_T = -1000;
      for (unsigned int t = 0; t < DSSDYTMult; ++t) {
        if (m_PreTreatedData->GetDSSDYTStripNbr(couple[i].Y())
            == m_PreTreatedData->GetDSSDYTStripNbr(t)
            && m_PreTreatedData->GetDSSDYTDetectorNbr(couple[i].Y())
            == m_PreTreatedData->GetDSSDYTDetectorNbr(t)) {
          DSSD_Y_T = m_PreTreatedData->GetDSSDYTTime(t);
          break;
        }
      }

      DSSD_X.push_back(X);
      DSSD_Y.push_back(Y);
      TelescopeNumber.push_back(N);

      if (m_Take_E_Y)
        DSSD_E.push_back(DSSD_Y_E);
      else
        DSSD_E.push_back(DSSD_X_E);

      if (m_Take_T_Y)
        DSSD_T.push_back(DSSD_Y_T);
      else
        DSSD_T.push_back(DSSD_X_T);

      for (unsigned int j = 0; j < SecondLayerEMult; ++j) {
        if (m_PreTreatedData->GetSecondLayerEDetectorNbr(j) == N) {
          if (Match_SecondLayer(X, Y, m_PreTreatedData->GetSecondLayerEStripNbr(j))) {
            SecondLayer_N.push_back(m_PreTreatedData->GetSecondLayerEStripNbr(j));
            SecondLayer_E.push_back(m_PreTreatedData->GetSecondLayerEEnergy(j));
            SecondLayer_T.push_back(-1000);
            //   Look for associate Time
            for (unsigned int k = 0; k < SecondLayerTMult; ++k) {
              // Same DSSD, Same Detector
              if (m_PreTreatedData->GetSecondLayerEStripNbr(j)
                  == m_PreTreatedData->GetSecondLayerTStripNbr(k)
                  && m_PreTreatedData->GetSecondLayerEDetectorNbr(j)
                  == m_PreTreatedData->GetSecondLayerTDetectorNbr(k)) {
                SecondLayer_T[SecondLayer_T.size() - 1] = m_PreTreatedData->GetSecondLayerTTime(j);
                break;
              }
            }

            check_SecondLayer = true;
          }
        }
      }

      if (!check_SecondLayer) {
        SecondLayer_N.push_back(0);
        SecondLayer_E.push_back(-1000);
        SecondLayer_T.push_back(-1000);
      }
    }
  }
  return;
}

///////////////////////////////////////////////////////////////////////////
int TMugastPhysics::CheckEvent() {
  // Check the size of the different elements
  if (m_PreTreatedData->GetDSSDXEMult()
      == m_PreTreatedData->GetDSSDYEMult())
    return 1; // Regular Event

  // INterstrip management is not coded, so waste of time to make this test
  /*  else if(   m_PreTreatedData->GetMMStripXEMult() ==
      m_PreTreatedData->GetMMStripYEMult()+1
      || m_PreTreatedData->GetMMStripXEMult() ==
      m_PreTreatedData->GetMMStripYEMult()-1  )
      return 2 ; // Pseudo Event, potentially interstrip*/

  else
    return -1; // Rejected Event
}

///////////////////////////////////////////////////////////////////////////
bool TMugastPhysics::ResolvePseudoEvent() { return false; }

///////////////////////////////////////////////////////////////////////////
vector<TVector2> TMugastPhysics::Match_X_Y() {
  vector<TVector2> ArrayOfGoodCouple;
  static unsigned int m_DSSDXEMult,m_DSSDYEMult;
  m_DSSDXEMult = m_PreTreatedData->GetDSSDXEMult();
  m_DSSDYEMult = m_PreTreatedData->GetDSSDYEMult();

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if (m_DSSDXEMult > m_MaximumStripMultiplicityAllowed
      || m_DSSDYEMult > m_MaximumStripMultiplicityAllowed) {
    return ArrayOfGoodCouple;
  }

  for (unsigned int i = 0; i < m_DSSDXEMult; i++) {
    for (unsigned int j = 0; j < m_DSSDYEMult; j++) {

      // Declaration of variable for clarity
      double DSSDXDetNbr = m_PreTreatedData->GetDSSDXEDetectorNbr(i);
      double DSSDYDetNbr = m_PreTreatedData->GetDSSDYEDetectorNbr(j);

      //   if same detector check energy
      if (DSSDXDetNbr == DSSDYDetNbr) {

        // Declaration of variable for clarity
        double DSSDXEnergy = m_PreTreatedData->GetDSSDXEEnergy(i);
        double DSSDXNbr    = m_PreTreatedData->GetDSSDXEStripNbr(i);
        double DSSDYEnergy = m_PreTreatedData->GetDSSDYEEnergy(j);
        double DSSDYNbr    = m_PreTreatedData->GetDSSDYEStripNbr(j);

        //   Look if energy match
        if (abs((DSSDXEnergy - DSSDYEnergy) / 2.)
            < m_StripEnergyMatching) {
          // Gives a unique ID for every telescope and strip combination
          int IDX = m_NumberOfTelescope * DSSDXNbr + DSSDXDetNbr;
          int IDY = m_NumberOfTelescope * DSSDYNbr + DSSDYDetNbr;

          m_HitDSSDX[IDX]++;
          m_HitDSSDY[IDY]++;

          ArrayOfGoodCouple.push_back(TVector2(i, j));
        }
      }
    }
  }

  // Prevent to treat event with ambiguous matching beetween X and Y
  map<int, int>::iterator itX = m_HitDSSDX.begin();
  for (; itX != m_HitDSSDX.end(); itX++) {
    if (itX->second > 1) {
      ArrayOfGoodCouple.clear();
    }
  }

  map<int, int>::iterator itY = m_HitDSSDY.begin();
  for (; itY != m_HitDSSDY.end(); itY++) {
    if (itY->second > 1) {
      ArrayOfGoodCouple.clear();
    }
  }

  m_HitDSSDX.clear();
  m_HitDSSDY.clear();

  return ArrayOfGoodCouple;
}

////////////////////////////////////////////////////////////////////////////
bool TMugastPhysics::IsValidChannel(const int& DetectorType,
    const int& telescope, const int& channel) {
  if (DetectorType == 0)
    return *(m_XChannelStatus[m_DetectorNumberIndex[telescope] - 1].begin() + channel - 1);

  else if (DetectorType == 1)
    return *(m_YChannelStatus[m_DetectorNumberIndex[telescope] - 1].begin() + channel - 1);

  else if (DetectorType == 2)
    return *(m_SecondLayerChannelStatus[m_DetectorNumberIndex[telescope] - 1].begin() + channel - 1);

  else
    return false;
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::ReadAnalysisConfig() {

  NPL::InputParser parser("./configs/ConfigMugast.dat");
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("ConfigMugast");

  for (unsigned int i = 0; i < blocks.size(); i++) {
    if(blocks[i]->HasToken("MAX_STRIP_MULTIPLICITY"))
      m_MaximumStripMultiplicityAllowed = blocks[i]->GetInt("MAX_STRIP_MULTIPLICITY");
    else if(blocks[i]->HasToken("STRIP_ENERGY_MATCHING"))
      m_StripEnergyMatching = blocks[i]->GetDouble("STRIP_ENERGY_MATCHING","MeV");
    else if(blocks[i]->HasToken("DISABLE_CHANNEL")){
      vector<int> v = blocks[i]->GetVectorInt("DISABLE_CHANNEL");
      *(m_XChannelStatus[v[0] - 1].begin() + v[1] - 1) = false;
    }
    else if(blocks[i]->HasToken("DISABLE_ALL")){
      int telescope = blocks[i]->GetInt("DISABLE_ALL");
      vector<bool> ChannelStatus;
      ChannelStatus.resize(128, false);
      m_XChannelStatus[telescope - 1] = ChannelStatus;
      m_YChannelStatus[telescope - 1] = ChannelStatus;
      ChannelStatus.resize(16, false);
      m_SecondLayerChannelStatus[telescope - 1]  = ChannelStatus;
    }
    else if (blocks[i]->HasToken("TAKE_E_Y"))
      m_Take_E_Y = blocks[i]->GetInt("TAKE_E_Y");

    else if (blocks[i]->HasToken("TAKE_T_Y"))
      m_Take_T_Y = blocks[i]->GetInt("TAKE_T_Y");

    else if (blocks[i]->HasToken("TAKE_E_X"))
      m_Take_E_Y = !(blocks[i]->GetInt("TAKE_E_X"));

    else if (blocks[i]->HasToken("TAKE_T_X"))
      m_Take_T_Y = !(blocks[i]->GetInt("TAKE_T_X"));

    else if (blocks[i]->HasToken("DSSD_X_E_RAW_THRESHOLD"))
      m_DSSD_X_E_RAW_Threshold = blocks[i]->GetInt("DSSD_X_E_RAW_THRESHOLD");

    else if (blocks[i]->HasToken("DSSD_Y_E_RAW_THRESHOLD"))
      m_DSSD_Y_E_RAW_Threshold = blocks[i]->GetInt("DSSD_Y_E_RAW_THRESHOLD");

    else if (blocks[i]->HasToken("SECONDLAYER_E_RAW_THRESHOLD"))
      m_SecondLayer_E_RAW_Threshold = blocks[i]->GetInt("SECONDLAYER_E_RAW_THRESHOLD");

    else if (blocks[i]->HasToken("DSSD_X_E_THRESHOLD"))
      m_DSSD_X_E_Threshold = blocks[i]->GetDouble("DSSD_X_E_THRESHOLD","MeV");

    else if (blocks[i]->HasToken("DSSD_Y_E_THRESHOLD"))
      m_DSSD_Y_E_Threshold = blocks[i]->GetDouble("DSSD_Y_E_THRESHOLD","MeV");

    else if (blocks[i]->HasToken("SECONDLAYER_E_THRESHOLD"))
      m_SecondLayer_E_Threshold = blocks[i]->GetDouble("SECONDLAYER_E_THRESHOLD","MeV");
  }
}

///////////////////////////////////////////////////////////////////////////
bool TMugastPhysics::Match_SecondLayer(int X, int Y, int StripNbr) {
  /*
     if (abs(m_CsI_MatchingX[CristalNbr - 1] - X) < (double)m_CsI_Size / 2.
     && abs(m_CsI_MatchingY[CristalNbr - 1] - Y) < (double)m_CsI_Size / 2.)
     return true;

     else
     return false;*/
  return true;
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::Clear() {
  EventMultiplicity = 0;

  TelescopeNumber.clear();
  EventType.clear();
  TotalEnergy.clear();

  // DSSD X
  DSSD_E.clear();
  DSSD_T.clear();
  DSSD_X.clear();
  DSSD_Y.clear();
  // SecondLayer
  SecondLayer_E.clear();
  SecondLayer_T.clear();
  SecondLayer_N.clear();

}

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Mugast");
  if (NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " Telescope found " << endl;

  unsigned int det=0;
  // Cartesian Case
  vector<string> cart
    = {"DetectorNumber","X1_Y1", "X1_Y128", "X128_Y1", "X128_Y128"};
  // Spherical Case
  vector<string> sphe = {"DetectorNumber","R", "THETA", "PHI", "BETA"};
  // Annular Case
  vector<string> annular = {"DetectorNumber","Center"};
  string Type; 

  for (unsigned int i = 0; i < blocks.size(); i++) {
   
    if (blocks[i]->HasTokenList(cart)) {
      if (NPOptionManager::getInstance()->GetVerboseLevel())
      
      Type = blocks[i]->GetMainValue();
      cout << endl << "////  Mugast Telescope " << Type << " " << i + 1 << endl;
      
      int detectorNbr = blocks[i]->GetInt("DetectorNumber");
      if(Type=="Square") DetectorType[detectorNbr]=MG_SQUARE;
      else if(Type=="Trapezoid") DetectorType[detectorNbr]=MG_TRAPEZE;
      else if(Type=="Annular") DetectorType[detectorNbr]=MG_ANNULAR;
   
      det = i+1;
      m_DetectorNumberIndex[detectorNbr]=det;
      TVector3 A = blocks[i]->GetTVector3("X1_Y1", "mm");
      TVector3 B = blocks[i]->GetTVector3("X128_Y1", "mm");
      TVector3 C = blocks[i]->GetTVector3("X1_Y128", "mm");
      TVector3 D = blocks[i]->GetTVector3("X128_Y128", "mm");
      AddTelescope(A, B, C, D);
    }
    
    else if (blocks[i]->HasTokenList(annular)) {
      if (NPOptionManager::getInstance()->GetVerboseLevel())
      Type = blocks[i]->GetMainValue();
      cout << endl << "////  Mugast Telescope " << Type << " " << i + 1 << endl;
      int detectorNbr = blocks[i]->GetInt("DetectorNumber");
      if(Type=="Square") DetectorType[detectorNbr]=MG_SQUARE;
      else if(Type=="Trapezoid") DetectorType[detectorNbr]=MG_TRAPEZE;
      else if(Type=="Annular") DetectorType[detectorNbr]=MG_ANNULAR;
      if(Type!="Annular"){
        cout << "ERROR: Using Mugast Annular Token for Square or Trapezoid detector " << endl;
        exit(1);
        }

      det = i+1;
      m_DetectorNumberIndex[detectorNbr]=det;
      TVector3 Center = blocks[i]->GetTVector3("Center", "mm");
      AddTelescope(Center);
    }


    else if (blocks[i]->HasTokenList(sphe)) {
      if (NPOptionManager::getInstance()->GetVerboseLevel())
      Type = blocks[i]->GetMainValue();
      cout << endl << "////  Mugast Telescope " << Type << " " << i + 1 << endl;
      int detectorNbr = blocks[i]->GetInt("DetectorNumber");
      if(Type=="Square") DetectorType[detectorNbr]=MG_SQUARE;
      else if(Type=="Trapezoid") DetectorType[detectorNbr]=MG_TRAPEZE;
      else if(Type=="Annular") DetectorType[detectorNbr]=MG_ANNULAR;
  
      det = i+1;
      m_DetectorNumberIndex[detectorNbr]=det;
      double         Theta = blocks[i]->GetDouble("THETA", "deg");
      double         Phi   = blocks[i]->GetDouble("PHI", "deg");
      double         R     = blocks[i]->GetDouble("R", "mm");
      vector<double> beta  = blocks[i]->GetVectorDouble("BETA", "deg");
      AddTelescope(Theta, Phi, R, beta[0], beta[1], beta[2]);
    }

    else {
      cout << "ERROR: Missing token for Mugast, check your input "
        "file"
        << endl;
      exit(1);
    }
  }

  InitializeStandardParameter();
  // Create a file to be read by Ganil2Root telling which detector
  // is which shape
  std::ofstream shapeFile(".MugastShape");
  for(auto& it:DetectorType){
    shapeFile << it.first << " " << it.second << endl;
    }
  shapeFile.close();
  //ReadAnalysisConfig();
}
//////////////////////////////////////////////////////////////////////////
void TMugastPhysics::InitSpectra() {
  m_Spectra = new TMugastSpectra(m_DetectorNumberIndex);
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::FillSpectra() {
  m_Spectra->FillRawSpectra(m_EventData);
  m_Spectra->FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra->FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::CheckSpectra() { /*m_Spectra->CheckSpectra();*/ }
///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::ClearSpectra() {
  // To be done
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::WriteSpectra() {
  if (m_Spectra)
    m_Spectra->WriteSpectra();
}

///////////////////////////////////////////////////////////////////////////
map<string, TH1*> TMugastPhysics::GetSpectra() {
 if(m_Spectra)
  return m_Spectra->GetMapHisto();
  else {
    map<string, TH1*> empty;
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  // Good for simulation, close to typical values
  vector<double> standardX    = {-63, 63. / 8192.};
  vector<double> standardY    = {63, -63. / 8192.};
  vector<double> standardSecondLayer  = {-250, 250. / 8192.};
  vector<double> standardT    = {-1000, 1000. / 8192.};

  for (int i = 0; i < m_NumberOfTelescope; ++i) {

    for (int j = 0; j < 128; ++j) {
      Cal->AddParameter(
          "Mugast", "T" + NPL::itoa(i + 1) + "_DSSD_X" + NPL::itoa(j + 1) + "_E",
          "Mugast_T" + NPL::itoa(i + 1) + "_DSSD_X" + NPL::itoa(j + 1) + "_E",
          standardX);
      Cal->AddParameter(
          "Mugast", "T" + NPL::itoa(i + 1) + "_DSSD_Y" + NPL::itoa(j + 1) + "_E",
          "Mugast_T" + NPL::itoa(i + 1) + "_DSSD_Y" + NPL::itoa(j + 1) + "_E",
          standardY);
      Cal->AddParameter(
          "Mugast", "T" + NPL::itoa(i + 1) + "_DSSD_X" + NPL::itoa(j + 1) + "_T",
          "Mugast_T" + NPL::itoa(i + 1) + "_DSSD_X" + NPL::itoa(j + 1) + "_T",
          standardT);
      Cal->AddParameter(
          "Mugast", "T" + NPL::itoa(i + 1) + "_DSSD_Y" + NPL::itoa(j + 1) + "_T",
          "Mugast_T" + NPL::itoa(i + 1) + "_DSSD_Y" + NPL::itoa(j + 1) + "_T",
          standardT);
    }

    for (int j = 0; j < 16; ++j) {
      Cal->AddParameter(
          "Mugast", "T" + NPL::itoa(i + 1) + "_SecondLayer" + NPL::itoa(j + 1) + "_E",
          "Mugast_T" + NPL::itoa(i + 1) + "_SecondLayer" + NPL::itoa(j + 1) + "_E",
          standardSecondLayer);
      Cal->AddParameter(
          "Mugast", "T" + NPL::itoa(i + 1) + "_SecondLayer" + NPL::itoa(j + 1) + "_T",
          "Mugast_T" + NPL::itoa(i + 1) + "_SecondLayer" + NPL::itoa(j + 1) + "_T",
          standardT);
    }
  }

  return;
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Mugast", true);
  inputChain->SetBranchStatus("fMM_*", true);
  inputChain->SetBranchAddress("Mugast", &m_EventData);
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Mugast", true);
  inputChain->SetBranchStatus("EventMultiplicity", true);
  inputChain->SetBranchStatus("EventType", true);
  inputChain->SetBranchStatus("TelescopeNumber", true);
  inputChain->SetBranchStatus("Si_E", true);
  inputChain->SetBranchStatus("Si_T", true);
  inputChain->SetBranchStatus("Si_X", true);
  inputChain->SetBranchStatus("Si_Y", true);
  inputChain->SetBranchStatus("Si_EX", true);
  inputChain->SetBranchStatus("Si_TX", true);
  inputChain->SetBranchStatus("Si_EY", true);
  inputChain->SetBranchStatus("Si_TY", true);
  inputChain->SetBranchStatus("TelescopeNumber_X", true);
  inputChain->SetBranchStatus("TelescopeNumber_Y", true);
  inputChain->SetBranchStatus("SiLi_E", true);
  inputChain->SetBranchStatus("SiLi_T", true);
  inputChain->SetBranchStatus("SiLi_N", true);
  inputChain->SetBranchStatus("CsI_E", true);
  inputChain->SetBranchStatus("CsI_T", true);
  inputChain->SetBranchStatus("CsI_N", true);
  inputChain->SetBranchStatus("TotalEnergy", true);
  inputChain->SetBranchAddress("Mugast", &m_EventPhysics);
}

///////////////////////////////////////////////////////////////////////////
void TMugastPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("Mugast", "TMugastPhysics", &m_EventPhysics);
}

/////   Specific to MugastArray   ////

void TMugastPhysics::AddTelescope(TVector3 C_X1_Y1, TVector3 C_X128_Y1,
    TVector3 C_X1_Y128, TVector3 C_X128_Y128) {
  // To avoid warning
  C_X128_Y128 *= 1;

  m_NumberOfTelescope++;

  // Vector U parallel to BaseLarge
  TVector3 U = C_X128_Y1 - C_X1_Y1;
  U = U.Unit();

  // Vector V parallel to height
  TVector3 V = 0.5 * (C_X1_Y128 + C_X128_Y128 - C_X1_Y1 - C_X128_Y1);
  V = V.Unit();

  //   Position Vector of Strip Center
  TVector3 StripCenter = TVector3(0, 0, 0);
  //   Position Vector of X=1 Y=1 Strip
  TVector3 Strip_1_1;

  //   Geometry Parameter
  double Face          = 98; // mm
  double NumberOfStrip = 128;
  double StripPitch    = Face / NumberOfStrip; // mm
  //   Buffer object to fill Position Array
  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector<vector<double>> OneTelescopeStripPositionX;
  vector<vector<double>> OneTelescopeStripPositionY;
  vector<vector<double>> OneTelescopeStripPositionZ;

  //   Moving StripCenter to 1.1 corner:
  Strip_1_1 = C_X1_Y1 + (U + V) * (StripPitch / 2.);
  Strip_1_1 += U + V ;

  for (int i = 0; i < 128; ++i) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < 128; ++j) {
      StripCenter = Strip_1_1 + StripPitch * (i * U + j * V);
      lineX.push_back(StripCenter.X());
      lineY.push_back(StripCenter.Y());
      lineZ.push_back(StripCenter.Z());
    }

    OneTelescopeStripPositionX.push_back(lineX);
    OneTelescopeStripPositionY.push_back(lineY);
    OneTelescopeStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back(OneTelescopeStripPositionX);
  m_StripPositionY.push_back(OneTelescopeStripPositionY);
  m_StripPositionZ.push_back(OneTelescopeStripPositionZ);
}

void TMugastPhysics::InitializeStandardParameter() {
  //   Enable all channel
  vector<bool> ChannelStatus;
  m_XChannelStatus.clear();
  m_YChannelStatus.clear();
  m_SecondLayerChannelStatus.clear();

  ChannelStatus.resize(128, true);
  for (int i = 0; i < m_NumberOfTelescope; ++i) {
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(16, true);
  for (int i = 0; i < m_NumberOfTelescope; ++i) {
    m_SecondLayerChannelStatus[i]  = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfTelescope;

  return;
}


void TMugastPhysics::AddTelescope(TVector3 C_Center) {
  // To avoid warning
  m_NumberOfTelescope++;
  double Z = C_Center.Z();

  double R_Min = 24;
  double R_Max = 48;

  double Phi_Min = 0  ;
  double Phi_Max = 360;

  int NumberOfQuadrant = 4 ;
  int NumberofRing = 16 ; //Per Quadrant
  int NumberofSector = 16 ; //Per detector, ( 4 in each Quad)

  double StripPitchSector = (Phi_Max-Phi_Min)/(NumberofSector) ; //radial strip spacing in rad
  double StripPitchRing = (R_Max-R_Min)/NumberofRing  ; // ring strip spacing in mm

  double Phi_0 = (8)*StripPitchSector; //Phi Offset: 1st sector starts at 180 degrees and ends at 180-22.5 degrees in the lab frame, numbering goes clockwise
  TVector3 Strip_1_1=TVector3(0,0,Z);
  TVector3 StripCenter = Strip_1_1;

  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;
  vector< vector< double > >   OneStripPositionX   ;
  vector< vector< double > >   OneStripPositionY   ;
  vector< vector< double > >   OneStripPositionZ   ;
  
 /* The logic behind the strip numbering of S1 in NPTOOL: 
 The number of rings goes from 1->64, the number of sectors goes from 1->16 
 (4 per quadrant). There's a redundancy in the fact that 1->64 already contain 
 the information about the quadrant and the majority of these positions are 
 indeed not physical. Example: 
 A hit combining Ring 17 (first ring in Quadrant 2) and 
 Sector 4 (last sector in Quadrant 1) is not possible due to physical mismatch 
 of the detector frontside-backside layout. 
 The possible (allowed hits) are R(1-16)S(1-4), R(17-32)S(5-8), R(33-49)S(9-12),
 R(50-64)S(13-16). 
 The three loops however takes all the possible combintation that an analysis
 can produce. This works perfectly for cases where the detector does not have 
 "Quadrants", e.g. S3 type. For the S1 an extra condition is added to flag the
 non physical hit combinations. 
 */
  for(int iQuad = 0 ; iQuad < NumberOfQuadrant ; iQuad++){
    for(int iRing = 0 ; iRing < NumberofRing; iRing++){
      lineX.clear() ;
      lineY.clear() ;
      lineZ.clear() ;
      for(int iSector = 0 ; iSector < NumberofSector ; iSector++){
        //Build vector
        StripCenter = Strip_1_1;
        StripCenter = TVector3(R_Min+(iRing+0.5)*StripPitchRing,0, Z);
        StripCenter.RotateZ( ( Phi_0 - (iSector+0.5)*StripPitchSector ) *M_PI/180.);

        // if the hit is not "allowed" (see comment above) use a default value
        if ( (iRing+(iQuad*NumberofRing))/NumberofSector != (iSector/NumberOfQuadrant) ) 
          StripCenter.SetXYZ(-100,-100, Z-100);
        
        lineX.push_back( StripCenter.X() );// these vectors will contain 16x4 = 64 elements
        lineY.push_back( StripCenter.Y() );
        lineZ.push_back( StripCenter.Z() );
      }

      OneStripPositionX.push_back(lineX);
      OneStripPositionY.push_back(lineY);
      OneStripPositionZ.push_back(lineZ);

    }
  }
  m_StripPositionX.push_back( OneStripPositionX ) ;
  m_StripPositionY.push_back( OneStripPositionY ) ;
  m_StripPositionZ.push_back( OneStripPositionZ ) ;

  return;

}


void TMugastPhysics::AddTelescope(double theta, double phi, double distance,
    double beta_u, double beta_v, double beta_w) {

  m_NumberOfTelescope++;

  double Pi = 3.141592654;

  // convert from degree to radian:
  theta = theta * Pi / 180.;
  phi   = phi * Pi / 180.;

  // Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y
  // strip are allong X axis)
  TVector3 U;
  // Vector V on Telescope Face (parallele to X Strip)
  TVector3 V;
  // Vector W normal to Telescope Face (pointing CsI)
  TVector3 W;
  // Vector position of Telescope Face center
  TVector3 C;

  C = TVector3(distance * sin(theta) * cos(phi),
      distance * sin(theta) * sin(phi), distance * cos(theta));

  TVector3 P
    = TVector3(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta));

  W = C.Unit();
  U = W.Cross(P);
  V = W.Cross(U);

  U = U.Unit();
  V = V.Unit();

  U.Rotate(beta_u * Pi / 180., U);
  V.Rotate(beta_u * Pi / 180., U);

  U.Rotate(beta_v * Pi / 180., V);
  V.Rotate(beta_v * Pi / 180., V);

  U.Rotate(beta_w * Pi / 180., W);
  V.Rotate(beta_w * Pi / 180., W);

  double Face          = 98; // mm
  double NumberOfStrip = 128;
  double StripPitch    = Face / NumberOfStrip; // mm

  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector<vector<double>> OneTelescopeStripPositionX;
  vector<vector<double>> OneTelescopeStripPositionY;
  vector<vector<double>> OneTelescopeStripPositionZ;

  double X, Y, Z;

  // Moving C to the 1.1 corner:
  C.SetX(C.X() - (Face / 2 - StripPitch / 2) * (V.X() + U.X()));
  C.SetY(C.Y() - (Face / 2 - StripPitch / 2) * (V.Y() + U.Y()));
  C.SetZ(C.Z() - (Face / 2 - StripPitch / 2) * (V.Z() + U.Z()));

  for (int i = 0; i < 128; ++i) {

    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < 128; ++j) {
      X = C.X() + StripPitch * (U.X() * i + V.X() * j);
      Y = C.Y() + StripPitch * (U.Y() * i + V.Y() * j);
      Z = C.Z() + StripPitch * (U.Z() * i + V.Z() * j);

      lineX.push_back(X);
      lineY.push_back(Y);
      lineZ.push_back(Z);
    }

    OneTelescopeStripPositionX.push_back(lineX);
    OneTelescopeStripPositionY.push_back(lineY);
    OneTelescopeStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back(OneTelescopeStripPositionX);
  m_StripPositionY.push_back(OneTelescopeStripPositionY);
  m_StripPositionZ.push_back(OneTelescopeStripPositionZ);
}

TVector3 TMugastPhysics::GetPositionOfInteraction(const int i) {
  TVector3 Position
    = TVector3(GetStripPositionX(TelescopeNumber[i], DSSD_X[i], DSSD_Y[i]),
        GetStripPositionY(TelescopeNumber[i], DSSD_X[i], DSSD_Y[i]),
        GetStripPositionZ(TelescopeNumber[i], DSSD_X[i], DSSD_Y[i]));

  return (Position);
}

TVector3 TMugastPhysics::GetTelescopeNormal(const int i) {
  TVector3 U = TVector3(GetStripPositionX(TelescopeNumber[i], 128, 1),
      GetStripPositionY(TelescopeNumber[i], 128, 1),
      GetStripPositionZ(TelescopeNumber[i], 128, 1))

    - TVector3(GetStripPositionX(TelescopeNumber[i], 1, 1),
        GetStripPositionY(TelescopeNumber[i], 1, 1),
        GetStripPositionZ(TelescopeNumber[i], 1, 1));

  TVector3 V = TVector3(GetStripPositionX(TelescopeNumber[i], 128, 128),
      GetStripPositionY(TelescopeNumber[i], 128, 128),
      GetStripPositionZ(TelescopeNumber[i], 128, 128))

    - TVector3(GetStripPositionX(TelescopeNumber[i], 128, 1),
        GetStripPositionY(TelescopeNumber[i], 128, 1),
        GetStripPositionZ(TelescopeNumber[i], 128, 1));

  TVector3 Normal = U.Cross(V);

  return (Normal.Unit());
}

///////////////////////////////////////////////////////////////////////////
namespace MUGAST_LOCAL {
  //   DSSD
  //   X
  double fDSSD_X_E(const TMugastData* m_EventData, const int& i) {
    static string name;
    name = "Mugast/T";
    name += NPL::itoa(m_EventData->GetDSSDXEDetectorNbr(i));
    name += "_DSSD_X";
    name += NPL::itoa(m_EventData->GetDSSDXEStripNbr(i));
    name += "_E";
    return CalibrationManager::getInstance()->ApplyCalibration(
        name, m_EventData->GetDSSDXEEnergy(i));
  }

  double fDSSD_X_T(const TMugastData* m_EventData, const int& i) {
    static string name;
    name = "Mugast/T";
    name += NPL::itoa(m_EventData->GetDSSDXTDetectorNbr(i));
    name += "_DSSD_X";
    name += NPL::itoa(m_EventData->GetDSSDXTStripNbr(i));
    name += "_T";
    return CalibrationManager::getInstance()->ApplyCalibration(
        name, m_EventData->GetDSSDXTTime(i));
  }

  //   Y
  double fDSSD_Y_E(const TMugastData* m_EventData, const int& i) {
    static string name;
    name = "Mugast/T";
    name += NPL::itoa(m_EventData->GetDSSDYEDetectorNbr(i));
    name += "_DSSD_Y";
    name += NPL::itoa(m_EventData->GetDSSDYEStripNbr(i));
    name += "_E";
    return CalibrationManager::getInstance()->ApplyCalibration(
        name, m_EventData->GetDSSDYEEnergy(i));
  }

  double fDSSD_Y_T(const TMugastData* m_EventData, const int& i) {
    static string name;
    name = "Mugast/T";
    name += NPL::itoa(m_EventData->GetDSSDYTDetectorNbr(i));
    name += "_DSSD_Y";
    name += NPL::itoa(m_EventData->GetDSSDYTStripNbr(i));
    name += "_T";
    return CalibrationManager::getInstance()->ApplyCalibration(
        name, m_EventData->GetDSSDYTTime(i));
  }

  //   SecondLayer
  double fSecondLayer_E(const TMugastData* m_EventData, const int& i) {
    static string name;
    name = "Mugast/T";
    name += NPL::itoa(m_EventData->GetSecondLayerEDetectorNbr(i));
    name += "_SecondLayer";
    name += NPL::itoa(m_EventData->GetSecondLayerEStripNbr(i));
    name += "_E";
    return CalibrationManager::getInstance()->ApplyCalibration(
        name, m_EventData->GetSecondLayerEEnergy(i));
  }

  double fSecondLayer_T(const TMugastData* m_EventData, const int& i) {
    static string name;
    name = "Mugast/T";
    name += NPL::itoa(m_EventData->GetSecondLayerTDetectorNbr(i));
    name += "_SecondLayer";
    name += NPL::itoa(m_EventData->GetSecondLayerTStripNbr(i));
    name += "_T";
    return CalibrationManager::getInstance()->ApplyCalibration(
        name, m_EventData->GetSecondLayerTTime(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TMugastPhysics::Construct() {
  return (NPL::VDetector*)new TMugastPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C" {
  class proxy_Mugast {
    public:
      proxy_Mugast() {
        NPL::DetectorFactory::getInstance()->AddToken("Mugast", "Mugast");
        NPL::DetectorFactory::getInstance()->AddDetector("Mugast",
            TMugastPhysics::Construct);
      }
  };

  proxy_Mugast p_Mugast;
}
