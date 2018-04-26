/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    : December 2016 m.moukaddam@surrey.ac.uk                   *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold GeTAMU treated data                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// STL
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPL
#include "TGeTAMUPhysics.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPSystemOfUnits.h"
#include "NPOptionManager.h"
using namespace NPUNITS;

//   ROOT
#include "TChain.h"
#include "TRandom3.h"

TRandom3 *Random = new TRandom3();
///////////////////////////////////////////////////////////////////////////

ClassImp(TGeTAMUPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TGeTAMUPhysics::TGeTAMUPhysics()  {
    m_EventData         = new TGeTAMUData ;
    m_PreTreatedData    = new TGeTAMUData ;
    m_EventPhysics      = this ;
    //m_Spectra           = NULL;

    //Raw Thresholds
    m_Cry_E_Threshold = 10 ;
    m_Seg_E_Threshold = 10 ;
    //Calibrated Threshold
    m_Cry_E_Raw_Threshold = 100 ;
    m_Seg_E_Raw_Threshold = 100 ;
    //Add Back mode
    m_AddBackMode = 1;

  };



void TGeTAMUPhysics::InitializeStandardParameter(){

//Set high gain as default
  m_LowGainCryIsSet = false;
  m_LowGainSegIsSet = false;
  m_CryChannelStatus.clear() ;
  m_SegChannelStatus.clear() ;

  //enable all channels 
  vector< bool > CryChannelStatus;
  vector< bool > SegChannelStatus;
  CryChannelStatus.resize(4,true);
  SegChannelStatus.resize(3,true);
  for (unsigned iDet = 0 ; iDet < m_CloverPosition.size() ; iDet++){
    m_CryChannelStatus[iDet] = CryChannelStatus;
    m_SegChannelStatus[iDet] = SegChannelStatus;
  }

  return;
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigGeTamu.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigGeTamu.dat found: Default parameters loaded for Analysis " << FileName << endl;
    return;
  }
  cout << " Loading user parameters for Analysis from ConfigGeTamu.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigGeTamu.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 12, "ConfigGeTamu") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {

      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo== "LOW_GAIN_ENERGY_CRY") {
        m_LowGainCryIsSet  = true ; 
        cout << whatToDo << " " << m_LowGainCryIsSet << endl; // e.g. DataBuffer = CLOVER03
      }

      else if (whatToDo== "LOW_GAIN_ENERGY_SEG") {
        m_LowGainSegIsSet  = true ; 
        cout << whatToDo << " " << m_LowGainSegIsSet << endl; // e.g. DataBuffer = CLOVER03
      }

      else if (whatToDo== "DISABLE_ALL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl; // e.g. DataBuffer = CLOVER03
        int detector = atoi(DataBuffer.substr(6,2).c_str());
        vector< bool > CryChannelStatus;
        vector< bool > SegChannelStatus;
        CryChannelStatus.resize(4,false);
        SegChannelStatus.resize(3,false);
        m_CryChannelStatus[detector-1] = CryChannelStatus;
        m_SegChannelStatus[detector-1] = SegChannelStatus;
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl; // e.g. DataBuffer = CLOVER03_CRY01, CLOVER03_SEG01
        int detector = atoi(DataBuffer.substr(6,2).c_str());
        int channel = -1;
        if (DataBuffer.compare(9,3,"CRY") == 0) {
          channel = atoi(DataBuffer.substr(12,2).c_str());
          m_CryChannelStatus[detector-1][channel-1] = false;
        }
        else if (DataBuffer.compare(9,3,"SEG") == 0) {
          channel = atoi(DataBuffer.substr(12,2).c_str());
          m_SegChannelStatus[detector-1][channel-1] = false;
        }
        else cout << "Warning: detector type for GeTAMU unknown!" << endl;
      }

      else if (whatToDo=="ADD_BACK_CLOVER") {
        m_AddBackMode = 1;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="ADD_BACK_FACING") {
        m_AddBackMode = 2;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="ADD_BACK_ARRAY") {
        m_AddBackMode = 3;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="CRY_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Cry_E_Raw_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Cry_E_Raw_Threshold << endl;
      }

      else if (whatToDo=="SEG_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Seg_E_Raw_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Seg_E_Raw_Threshold << endl;
      }

      else if (whatToDo=="CRY_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Cry_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Cry_E_Threshold << endl;
      }

      else if (whatToDo== "SEG_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Seg_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Seg_E_Threshold << endl;
      }

      else if (whatToDo== "ADC_RANDOM_BIN") {
        AnalysisConfigFile >> DataBuffer;
        m_ADCRandomBinIsSet  = true ; 
        cout << whatToDo << " " << m_ADCRandomBinIsSet << endl;
      }

      else {
        ReadingStatus = false;
      }

    }
  }
}


////////////////////////////////////////////////////////////////////////////
bool TGeTAMUPhysics :: IsValidChannel(const int& DetectorType, const int& detector , const int& channel) {
  if(DetectorType == 0) //Cry
    return m_CryChannelStatus[detector-1][channel-1];

  else if(DetectorType == 1)
    return m_SegChannelStatus[detector-1][channel-1];

  else return false;
}

/////////////////////////////////////////////////
void TGeTAMUPhysics::BuildPhysicalEvent(){
  PreTreat();

  //match segments data to cores data,
  // Rule 0: If more than 1 core is fired, there are as many hits as there are fired cores
  // Rule 1: segments are auxilliary to cores, they only point out the reduced "pixel" if any, (pixel== overlap segment/core)
  //Ideas: try to kick one segment out by sum rule

  //cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
  //m_PreTreatedData->Dump();
  if(m_PreTreatedData->GetMultiplicityCoreE()==0) return;

  vector <int>    CryEN, SegEN;
  vector <double> CryE, SegE;
  for (unsigned iClover=0; iClover<4; iClover++){
    int clover = iClover+1;
    CryEN.clear();
    SegEN.clear();
    CryE.clear();
    SegE.clear();
    //Energy
    if(Singles_CloverMap_CryEN.find(iClover+1) != Singles_CloverMap_CryEN.end()){
      CryEN  = Singles_CloverMap_CryEN[iClover+1];
      CryE   = Singles_CloverMap_CryE[iClover+1];
    }
    else
      continue; // no need to go further if Cores energies are non existant

    if(Singles_CloverMap_SegEN.find(iClover+1) != Singles_CloverMap_SegEN.end()){
      SegEN   = Singles_CloverMap_SegEN[iClover+1];
      SegE    = Singles_CloverMap_SegE[iClover+1];
    }

    /*
    //calculate total energy for sum rules
    double totCryE =0;
    double totSegE =0;
    for (unsigned i=0 ; i < CryE.size();i++)    totCryE+=CryE[i];
    for (unsigned i=0 ; i < SegE.size();i++)   totSegE+=SegE[i];
    */

    //sort the crystal energies;
    int swapEN;
    double swapE;
    int size = (int) CryE.size();
    for (int i=0; i< (size -1); i++){    // element to be compared
      for(int j = (i+1); j < size; j++){   // rest of the elements
        if (CryE[i] < CryE[j]){          // descending order
          swapE= CryE[i];    CryE[i] = CryE[j];   CryE[j] = swapE;
          swapEN= CryEN[i];   CryEN[i] = CryEN[j];   CryEN[j] = swapEN;
        }
      }
    }
    //create hit matrix
    int pixel[4][3];
    memset(pixel,0,sizeof(pixel)); // filling the matrix with zeros
    //fill the cores
    for (unsigned iCry = 0 ; iCry < CryEN.size() ; iCry++){
      int Cry = CryEN[iCry] ;
      for (unsigned iSeg = 1 ; iSeg <= 3 ; iSeg++){
        // (Segment 3 (right) + Cry 1 or 4) or (Segment 1 (Left)  + Cry 2 or 3) are impossible
        if( ((Cry==1 || Cry==4) && iSeg!=3) ||  ((Cry==2 || Cry==3) && iSeg!=1) ){
          pixel[Cry-1][iSeg-1]++;
        }
      }
    }
    //fill the segments
    for (unsigned iSeg = 0 ; iSeg < SegEN.size() ; iSeg++){
      int Seg = SegEN[iSeg];
      for (unsigned iCry = 1 ; iCry <= 4 ; iCry++){
        if( pixel[iCry-1][Seg-1] != 0){ // Only if the pixel was filled by a Cry signal previously, access it
          pixel[iCry-1][Seg-1]++;
        }
      }
    }

    //Calculate the singles
    for (unsigned i = 0 ; i < CryEN.size() ; i++){
      int segment = -1;
      unsigned crystal = CryEN[i];
      unsigned segmentA = 2;
      unsigned segmentB = 3;
      if (crystal==1 || crystal==4){ // if Core 1 or 4 change the segments to segment 1 and 2
        segmentA = 1;
        segmentB = 2;
      }
      //pick between segment A or B for each case
      if (pixel[crystal-1][segmentA-1] == pixel[crystal-1][segmentB-1])
        segment = 0; // system can't be resolved
      else if (pixel[crystal-1][segmentA-1] > pixel[crystal-1][segmentB-1])
        segment = segmentA;
      else if (pixel[crystal-1][segmentA-1] < pixel[crystal-1][segmentB-1])
        segment = segmentB;

      //cout << i <<" picked: crystal " << crystal << "   segment " << segment << "  Energy " << CryE[i] << endl;
      Singles_Clover.push_back(clover);
      Singles_Crystal.push_back(CryEN[i]);
      Singles_Segment.push_back(segment);
      Singles_E.push_back(CryE[i]);
      TVector3 Pos = GetSegmentPosition(clover,CryEN[i],segment);
      Singles_X.push_back(Pos.X());
      Singles_Y.push_back(Pos.Y());
      Singles_Z.push_back(Pos.Z());
      //cout << " XYZ "<< Pos.X() << " "<< Pos.Y() << " "<< Pos.Z() << " Theta: " <<Pos.Theta()/deg<< endl ;
    }

  } // end of Clover loop on map


  //Fill the time OR
  for (unsigned i = 0 ; i < m_PreTreatedData->GetMultiplicityCoreT(); i++)
    GeTime.push_back(m_PreTreatedData->GetCoreTime(i));

} // end

/////////////////////////////////////////////////
void TGeTAMUPhysics::PreTreat(){

ClearPreTreatedData();

static CalibrationManager* cal = CalibrationManager::getInstance();
static string name;
unsigned int mysizeE ;
unsigned int mysizeT ;
double Eraw,Energy;
double Traw,Time;
int clover, crystal, segment;
//by Shuya 170919
bool	LG_Opt;
bool	m_LowGainCryIsOpt = true;
bool	m_LowGainSegIsOpt = true;

//Crystal energy
if(m_LowGainCryIsSet)
  mysizeE = m_EventData->GetMultiplicityCoreELowGain();
else
  mysizeE = m_EventData->GetMultiplicityCoreE();

for(unsigned int i = 0 ; i < mysizeE ; i++){
  if(m_LowGainCryIsSet){
    clover = m_EventData->GetCoreCloverNbrELowGain(i);
    crystal = m_EventData->GetCoreCrystalNbrELowGain(i);
    Eraw = m_EventData->GetCoreEnergyLowGain(i);
  }
  else{
    clover = m_EventData->GetCoreCloverNbrE(i);
    crystal = m_EventData->GetCoreCrystalNbrE(i);
    Eraw = m_EventData->GetCoreEnergy(i);

//by Shuya 170919. Use low-gain data if it is available (for high-energy gammas which might be above high-gain data's saturation level).
//***************************************************************************************************
    if(m_LowGainCryIsOpt)
    {
	LG_Opt = false;

	int	m_Cry_E_Raw_Threshold_tmp = 2000;
	//try low-gain data if high-gain data is above this threshold. If set 0 -> all low-gain, set above 5000 -> all high gain.
        if(Eraw>=m_Cry_E_Raw_Threshold_tmp)
    	{
		unsigned int mysizeE2;
		double Eraw_tmp;
		int clover_tmp, crystal_tmp;

  		mysizeE2 = m_EventData->GetMultiplicityCoreELowGain();

		for(unsigned int j = 0 ; j < mysizeE2 ; j++)
		{
    			clover_tmp = m_EventData->GetCoreCloverNbrELowGain(j);
    			crystal_tmp = m_EventData->GetCoreCrystalNbrELowGain(j);
    			Eraw_tmp = m_EventData->GetCoreEnergyLowGain(j);

			if(clover_tmp == clover && crystal_tmp == crystal && Eraw_tmp>=m_Cry_E_Raw_Threshold)
			{
				Eraw = Eraw_tmp;
				LG_Opt = true;
				break;
			}
		}
	}
    }
//***************************************************************************************************
  }
  if(Eraw>=m_Cry_E_Raw_Threshold && IsValidChannel(0, clover, crystal)){
    name = "GETAMU/D"+ NPL::itoa(clover)+"_CRY"+ NPL::itoa(crystal);
    if(m_ADCRandomBinIsSet) 
      Eraw += Random->Rndm();
    //by Shuya 170919
    //Energy =  cal->ApplyCalibration(name+"_E_LG", Eraw);
    if(m_LowGainCryIsSet)
    Energy =  cal->ApplyCalibration(name+"_E_LG", Eraw);
    else
    {
    	if(m_LowGainCryIsOpt && LG_Opt)
    	Energy =  cal->ApplyCalibration(name+"_E_LG", Eraw);
	else
    	Energy =  cal->ApplyCalibration(name+"_E", Eraw);
    }
    if(Energy>=m_Cry_E_Threshold){
      Singles_CloverMap_CryEN[clover].push_back(crystal);
      Singles_CloverMap_CryE[clover].push_back(Energy);
      m_PreTreatedData->SetCoreE(clover,crystal,Energy);
    }
  }
}

//Crystal time
  mysizeT = m_EventData->GetMultiplicityCoreT();
  for(unsigned int i = 0 ; i < mysizeT ; i++){
    Traw = m_EventData->GetCoreTime(i);
    if(Traw>0){
      clover = m_EventData->GetCoreCloverNbrT(i);
      crystal = m_EventData->GetCoreCrystalNbrT(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_CRY"+ NPL::itoa(crystal);
      if(m_ADCRandomBinIsSet) 
        Traw += Random->Rndm();
      Time =  cal->ApplyCalibration(name+"_T", Traw);
      Singles_CloverMap_CryTN[clover].push_back(crystal);
      Singles_CloverMap_CryT[clover].push_back(Time);
      m_PreTreatedData->SetCoreT(clover,crystal,Time);
    }
  }

//Segment Energy
if(m_LowGainSegIsSet)
  mysizeE = m_EventData->GetMultiplicitySegmentELowGain();
else
  mysizeE = m_EventData->GetMultiplicitySegmentE();

for(unsigned int i = 0 ; i < mysizeE ; i++){
  if(m_LowGainSegIsSet){
    clover = m_EventData->GetSegmentCloverNbrELowGain(i);
    segment = m_EventData->GetSegmentSegmentNbrELowGain(i);
    Eraw = m_EventData->GetSegmentEnergyLowGain(i);
  }
  else{
    clover = m_EventData->GetSegmentCloverNbrE(i);
    segment = m_EventData->GetSegmentSegmentNbrE(i);
    Eraw = m_EventData->GetSegmentEnergy(i);

//by Shuya 170919. Use low-gain data if it is available (for high-energy gammas which might be above high-gain data's saturation level).
//***************************************************************************************************
    if(m_LowGainCryIsOpt)
    {
	LG_Opt = false;

	int	m_Seg_E_Raw_Threshold_tmp = 2000;
	//try low-gain data if high-gain data is above this threshold. If set 0 -> all low-gain, set above 5000 -> all high gain.
        if(Eraw>=m_Seg_E_Raw_Threshold_tmp)
    	{
		unsigned int mysizeE2;
		double Eraw_tmp;
		int clover_tmp, segment_tmp;

 		mysizeE2 = m_EventData->GetMultiplicitySegmentELowGain();

		for(unsigned int j = 0 ; j < mysizeE2 ; j++)
		{
    			clover_tmp = m_EventData->GetSegmentCloverNbrELowGain(j);
    			segment_tmp = m_EventData->GetSegmentSegmentNbrELowGain(j);
    			Eraw_tmp = m_EventData->GetSegmentEnergyLowGain(j);

			if(clover_tmp == clover && segment_tmp == segment && Eraw_tmp>=m_Cry_E_Raw_Threshold)
			{
				Eraw = Eraw_tmp;
				LG_Opt = true;
				break;
			}
		}
	}
    }
//***************************************************************************************************
  }
  if(Eraw>=m_Seg_E_Raw_Threshold && IsValidChannel(1, clover, segment)){
    name = "GETAMU/D"+ NPL::itoa(clover)+"_SEG"+ NPL::itoa(segment);
    if(m_ADCRandomBinIsSet) 
      Eraw += Random->Rndm();
    //by Shuya 170919
    //Energy =  cal->ApplyCalibration(name+"_E", Eraw);
    if(m_LowGainCryIsSet)
    Energy =  cal->ApplyCalibration(name+"_E_LG", Eraw);
    else
    {
	if(m_LowGainSegIsOpt && LG_Opt)
    	Energy =  cal->ApplyCalibration(name+"_E_LG", Eraw);
	else
    	Energy =  cal->ApplyCalibration(name+"_E", Eraw);
    }
    if(Energy>=m_Seg_E_Threshold){
      Singles_CloverMap_SegEN[clover].push_back(segment);
      Singles_CloverMap_SegE[clover].push_back(Energy);
      m_PreTreatedData->SetSegmentE(clover,segment,Energy);
    }
  }
}
//Segment time
  mysizeT = m_EventData->GetMultiplicitySegmentT();
  for(unsigned int i = 0 ; i < mysizeT ; i++){
    Traw = m_EventData->GetSegmentTime(i);
    if(Traw>0){
      clover = m_EventData->GetSegmentCloverNbrT(i);
      segment = m_EventData->GetSegmentSegmentNbrT(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_SEG"+ NPL::itoa(segment);
      if(m_ADCRandomBinIsSet) 
        Traw += Random->Rndm();
      Time =  cal->ApplyCalibration(name+"_T", Traw);
      Singles_CloverMap_CryTN[clover].push_back(segment);
      Singles_CloverMap_CryT[clover].push_back(Time);
      m_PreTreatedData->SetSegmentT(clover,segment,Time);
    }
  }

}


/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetPositionOfInteraction(unsigned int& i){
  //return TVector3();
  return GetSegmentPosition(Singles_Clover[i],Singles_Crystal[i],Singles_Segment[i]);
}

/////////////////////////////////////////////////
// original energy, position, beta
double TGeTAMUPhysics::GetDopplerCorrectedEnergy(double& energy , TVector3 GamLabDirection, TVector3& BeamBeta){

  // renormalise GamLabDirection vector
  GamLabDirection.SetMag(1); // gamma direction
  m_GammaLV.SetPx(energy*GamLabDirection.X());
  m_GammaLV.SetPy(energy*GamLabDirection.Y());
  m_GammaLV.SetPz(energy*GamLabDirection.Z());
  m_GammaLV.SetE(energy);
  m_GammaLV.Boost(-BeamBeta); // beam beta
  return m_GammaLV.Energy();
}

void TGeTAMUPhysics::DCSingles( TVector3& BeamBeta){

  for(unsigned int iPixel = 0 ; iPixel < Singles_E.size() ; iPixel++){
    int clv = Singles_Clover[iPixel];
    int cry = Singles_Crystal[iPixel];
    int seg = Singles_Segment[iPixel];
    double energy = Singles_E[iPixel];
    TVector3 GammaLabDirection = GetSegmentPosition(clv,cry,seg);   
    // Fill The doppler corrected singles  
    Singles_DC.push_back(GetDopplerCorrectedEnergy(energy, GammaLabDirection, BeamBeta)); // Doppler Corrected for highest energy
    Singles_Theta.push_back(GammaLabDirection.Angle(BeamBeta)); 
  }
}



void TGeTAMUPhysics::AddBack( TVector3& BeamBeta){

  if (m_AddBackMode==1){ // clover by clover add-back

//////////////////////////////////////////////////////////////////////////////// PART 1
   //Pick up the maximum energy from the core of each clover for Doppler Correction.
   // and sum up energies of the same clover 
      vector<double> tot_E(4, 0); // size 4, all has zero
      vector<double> max_E(4, -1); // maximum stored energy 
      vector<unsigned> max_E_pixel(4, 999); // pixel corresponding tot he maximum stored energy
      for(unsigned int iPixel = 0 ; iPixel < Singles_E.size() ; iPixel++){
        int clv = Singles_Clover[iPixel];
        int cry = Singles_Crystal[iPixel];
        int seg = Singles_Segment[iPixel];
        double energy = Singles_E[iPixel];
        tot_E[clv-1]+=energy; // add the enrgy to the right clover
        if(energy>max_E[clv-1]) {
            max_E[clv-1]       = energy; // update maximum energy for this clover
            max_E_pixel[clv-1] = iPixel; // select this pixel for this clover
        }
      }

/////////////////////////////////////////////////////////////////////////////// PART 2
    // fill the doppler corrected data
    for(unsigned int iClover = 0 ; iClover < tot_E.size() ; iClover++){
      //retrieve the total energy 
      double energy = tot_E[iClover];
      if(energy>0){
        //retrieve the maximum pixel 
        unsigned pixel = max_E_pixel[iClover];
        int clv = Singles_Clover[pixel];
        int cry = Singles_Crystal[pixel];
        int seg = Singles_Segment[pixel]; 
        // fill the addback data
        AddBack_Clover.push_back(clv);
        AddBack_Crystal.push_back(cry);
        AddBack_Segment.push_back(seg);
        TVector3 GammaLabDirection = GetSegmentPosition(clv,cry,seg);
        AddBack_Theta.push_back(GammaLabDirection.Angle(BeamBeta)); 
        AddBack_X.push_back(GammaLabDirection.X());
        AddBack_Y.push_back(GammaLabDirection.Y());
        AddBack_Z.push_back(GammaLabDirection.Z());
        AddBack_E.push_back(energy); 
        //Addback energy
        AddBack_DC.push_back(GetDopplerCorrectedEnergy(energy, GammaLabDirection, BeamBeta));  // DC(Etot)
      }
    }
  } // end of scheme 1
    
  else if (m_AddBackMode==2){ // facing clovers (1&3) or (2&4) add-back
      double max24 = -1; // maximum stored energy 
      double max13 = -1;
      double totE24 = 0; // total energy 
      double totE13 = 0; 
      unsigned max_E_pixel24 = -1;
      unsigned max_E_pixel13 = -1;

      ////////////////////////////////////////////////////////////////////////////////  PART 1
      // pick up the maximum energies
      for(unsigned int iPixel = 0 ; iPixel < Singles_E.size() ; iPixel++){
        int clv = Singles_Clover[iPixel];
        int cry = Singles_Crystal[iPixel];
        int seg = Singles_Segment[iPixel];
        double energy = Singles_E[iPixel];
        if(clv == 1 || clv ==3 ) {
          totE13+=energy;
          if(energy>max13) {
            max13 = energy;
            max_E_pixel13 = iPixel; // select this pixel for this clover souple 2,4
          }
        }
        else {
          totE24+=energy;
          if(energy>max24) {
            max24 = energy;
            max_E_pixel24 = iPixel; // select this pixel for this clover souple 2,4
          }
        }
      }

      ////////////////////////////////////////////////////////////////////////////////  PART 2
      //Fill the addback vectors
      if (totE13>0){
        int clv = Singles_Clover[max_E_pixel13];
        int cry = Singles_Crystal[max_E_pixel13];
        int seg = Singles_Segment[max_E_pixel13];
        AddBack_Clover.push_back(clv);
        AddBack_Crystal.push_back(cry);
        AddBack_Segment.push_back(seg);
        TVector3 GammaLabDirection = GetSegmentPosition(clv,cry,seg);
        AddBack_Theta.push_back(GammaLabDirection.Angle(BeamBeta)); 
        AddBack_X.push_back(GammaLabDirection.X());
        AddBack_Y.push_back(GammaLabDirection.Y());
        AddBack_Z.push_back(GammaLabDirection.Z());
        AddBack_E.push_back(totE13);
        //Doppler correction
        AddBack_DC.push_back(GetDopplerCorrectedEnergy(totE13, GammaLabDirection, BeamBeta)); // Doppler Corrected for highest energy
      }
      if (totE24>0){
        int clv = Singles_Clover[max_E_pixel24];
        int cry = Singles_Crystal[max_E_pixel24];
        int seg = Singles_Segment[max_E_pixel24];
        AddBack_Clover.push_back(clv);
        AddBack_Crystal.push_back(cry);
        AddBack_Segment.push_back(seg);
        TVector3 GammaLabDirection = GetSegmentPosition(clv,cry,seg);
        AddBack_Theta.push_back(GammaLabDirection.Angle(BeamBeta)); 
        AddBack_X.push_back(GammaLabDirection.X());
        AddBack_Y.push_back(GammaLabDirection.Y());
        AddBack_Z.push_back(GammaLabDirection.Z());
        AddBack_E.push_back(totE24);
        //Doppler correction
        AddBack_DC.push_back(GetDopplerCorrectedEnergy(totE24, GammaLabDirection, BeamBeta)); // Doppler Corrected for highest energy
      } 
    } // end of scheme 2
    else if (m_AddBackMode==3){ // all clovers add-back
        double maxE = -1; // maximum stored energy 
        double totE = 0; // total energy 
        unsigned max_E_pixel = -1;

      ////////////////////////////////////////////////////////////////////////////////  PART 1
      // pick up the maximum energies
        for(unsigned int iPixel = 0 ; iPixel < Singles_E.size() ; iPixel++){
          int clv = Singles_Clover[iPixel];
          int cry = Singles_Crystal[iPixel];
          int seg = Singles_Segment[iPixel];
          double energy = Singles_E[iPixel];
          totE+=energy;
          if(energy>maxE) {
            maxE = energy;
            max_E_pixel = iPixel; // select this pixel for this clover souple 2,4
          }
        }
        ////////////////////////////////////////////////////////////////////////////////  PART 2
        //Fill the addback vectors
        if (totE>0){
          int clv = Singles_Clover[max_E_pixel];
          int cry = Singles_Crystal[max_E_pixel];
          int seg = Singles_Segment[max_E_pixel];
          AddBack_Clover.push_back(clv);
          AddBack_Crystal.push_back(cry);
          AddBack_Segment.push_back(seg);
          TVector3 GammaLabDirection = GetSegmentPosition(clv,cry,seg);
          AddBack_Theta.push_back(GammaLabDirection.Angle(BeamBeta)); 
          AddBack_X.push_back(GammaLabDirection.X());
          AddBack_Y.push_back(GammaLabDirection.Y());
          AddBack_Z.push_back(GammaLabDirection.Z());
          AddBack_E.push_back(totE);
          //Doppler correction
          AddBack_DC.push_back(GetDopplerCorrectedEnergy(totE, GammaLabDirection, BeamBeta)); // Doppler Corrected for highest energy
        }
      } // end of scheme 3 


}

/////////////////////////////////////////////////
void TGeTAMUPhysics::AddClover(unsigned int ID ,double R, double Theta, double Phi){
  TVector3 Pos(0,0,1);
  Pos.SetTheta(Theta);
  Pos.SetPhi(Phi);
  Pos.SetMag(R);
  m_CloverPosition[ID] = Pos;
  return;
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetCloverPosition(int& CloverNbr){
  return m_CloverPosition[CloverNbr];
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetCorePosition(int& CloverNbr,int& CoreNbr){
  double offset = 20; // mm
  double depth = 40; // mm
  TVector3 Pos;
  TVector3 CloverPos = GetCloverPosition(CloverNbr);

  if(CoreNbr==1)
    Pos.SetXYZ(offset,offset,depth);
  else if(CoreNbr==2)
    Pos.SetXYZ(-offset,offset,depth);
  else if(CoreNbr==3)
    Pos.SetXYZ(-offset,-offset,depth);
  else if(CoreNbr==4)
    Pos.SetXYZ(offset,-offset,depth);
  else
    cout << "Warning in GetCorePosition: GeTAMU crystal number " << CoreNbr << " is out of range (1 to 4)" << endl;

  // Define reference axis as the clover direction
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos);
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetSegmentPosition(int& CloverNbr,int& CoreNbr, int& SegmentNbr){

  if (SegmentNbr==0) return GetCorePosition(CloverNbr,CoreNbr);

  double offsetX = 20; // 20mm in GeTAMU according to measurments, 33.4 mm in TIGRESS
  double offsetY = 20;
  double depth = 40;  // 40mm in GeTAMU according to measurments, 45 mm in TIGRESS

  // Changes signs with segment/core combinations
  if (CoreNbr==3||CoreNbr==2)
    offsetX = -offsetX;
  if (CoreNbr==3||CoreNbr==4)
    offsetY = -offsetY;

  TVector3 CloverPos = GetCloverPosition(CloverNbr);
  TVector3 Pos;

  if(SegmentNbr==1 || SegmentNbr==3){ // LEFT OR RIGHT
    offsetX = 1.5*offsetX ;
    Pos.SetXYZ(offsetX,offsetY,depth);
  }
  else if(SegmentNbr==2){ // MIDDLE
    offsetX = 0.5*offsetX ;
    Pos.SetXYZ(offsetX,offsetY,depth);
  }
  else

    cout << "Warning: GeTAMU segment number " << SegmentNbr
      << " is out of range\n accepted values: 0 (reserved for core) or 1-3 (Left, Middle, Right segment) " << endl;

  // Define reference axis as the Clover position, 
  // This is a special case to GeTAMU where segmentation is with respect to clover
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos);

}


/////////////////////////////////////////////////

void TGeTAMUPhysics::ReadConfiguration(NPL::InputParser parser)  {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GeTAMU");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " clovers found " << endl;

  vector<string> token = {"CloverID","R","Theta","Phi","Beta"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      int     id = blocks[i]->GetInt("CloverID");
      vector<double> Beta = blocks[i]->GetVectorDouble("Beta","deg");
      cout << "WARNING: beta not used, need to be fixed!" << endl;
      AddClover(id,R,Theta,Phi);
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
void TGeTAMUPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "GeTAMU" , true );
  if(inputChain->FindBranch( "fTIG_*" ))
    inputChain->SetBranchStatus( "fTIG_*" , true ); // CHECK
  inputChain->SetBranchAddress( "GeTAMU" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::InitializeRootOutput()    {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "GeTAMU" , "TGeTAMUPhysics" , &m_EventPhysics );
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::Clear() {

  Singles_CloverMap_CryEN.clear(); // cry number energy
  Singles_CloverMap_SegEN.clear(); // seg number
  Singles_CloverMap_CryE.clear(); // cry energy
  Singles_CloverMap_SegE.clear(); // seg energy
  Singles_CloverMap_CryTN.clear(); // cry number time
  Singles_CloverMap_SegTN.clear(); // seg number
  Singles_CloverMap_CryT.clear(); // cry energy
  Singles_CloverMap_SegT.clear(); // seg energy
  Singles_E.clear();
  Singles_DC.clear();
  Singles_Theta.clear();
  Singles_X.clear();
  Singles_Y.clear();
  Singles_Z.clear();
  Singles_Clover.clear();
  Singles_Crystal.clear();
  Singles_Segment.clear();

  AddBack_E.clear();
  AddBack_DC.clear();
  AddBack_Theta.clear();
  AddBack_X.clear();
  AddBack_Y.clear();
  AddBack_Z.clear();
  AddBack_Clover.clear();
  AddBack_Crystal.clear();
  AddBack_Segment.clear();
  AddBack_T.clear();

  GeTime.clear();
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int det = 0 ; det < 4; det++){ //4 detectors

    for(int cry = 0 ; cry < 4 ; cry++){ // 4 crystals
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E","GETAMU_D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E");
	//by Shuya 170919
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E_LG","GETAMU_D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E_LG");
	//by Shuya 170509
      //Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E","GETAMU_D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_T");
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_T","GETAMU_D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_T");
    }
    for( int seg = 0 ; seg < 3 ; seg++){ // 3 segments
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_SEG"+ NPL::itoa(seg+1)+"_E","GETAMU_D"+ NPL::itoa(det+1)+"_SEG"+NPL::itoa(seg+1)+"_E");
	//by Shuya 170919
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_SEG"+ NPL::itoa(seg+1)+"_E_LG","GETAMU_D"+ NPL::itoa(det+1)+"_SEG"+NPL::itoa(seg+1)+"_E_LG");
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_SEG"+ NPL::itoa(seg+1)+"_T","GETAMU_D"+ NPL::itoa(det+1)+"_SEG"+NPL::itoa(seg+1)+"_T");
    }

  }

  return;
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TGeTAMUPhysics::Construct(){
  return (NPL::VDetector*) new TGeTAMUPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_getamu{
  public:
    proxy_getamu(){
      NPL::DetectorFactory::getInstance()->AddToken("GeTAMU","GeTAMU");
      NPL::DetectorFactory::getInstance()->AddDetector("GeTAMU",TGeTAMUPhysics::Construct);
    }
};

proxy_getamu p;
}
