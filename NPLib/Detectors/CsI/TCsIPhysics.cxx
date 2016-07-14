/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
 *  This class hold CsI  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TCsIPhysics.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPCalibrationManager.h"

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

ClassImp(TCsIPhysics)
///////////////////////////////////////////////////////////////////////////
TCsIPhysics::TCsIPhysics()
{
    NumberOfDetector = 0 ;
    EventData = new TCsIData ;
    EventPhysics = this ;
}

///////////////////////////////////////////////////////////////////////////
TCsIPhysics::~TCsIPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::Clear()
{
    DetectorNumber.clear() ;
    Energy.clear() ;
    Time.clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::ReadConfiguration(string Path)
{
    ifstream ConfigFile           ;
    ConfigFile.open(Path.c_str()) ;
    string LineBuffer          ;
    string DataBuffer          ;
    
    double Theta = 0 , Phi = 0 , R = 0 , Thickness = 0 , Radius = 0 ;
    double  LeadThickness = 0, X = 0 , Y = 0 , Z = 0 , FaceFront = 0 , FaceBack = 0 ;
    string Scintillator, Shape ;
    
    bool check_Theta = false ;
    bool check_Phi = false ;
    bool check_R = false ;
    bool check_Thickness = false ;
    bool check_Radius = false ;
    bool check_LeadThickness = false ;
    bool check_Scintillator = false ;
    bool check_FaceFront = false ;
    bool check_FaceBack = false ;
    bool check_Shape = false ;
    bool check_X = false ;
    bool check_Y = false ;
    bool check_Z = false ;
    bool ReadingStatus = false ;
    
    
    while (!ConfigFile.eof()) {
        getline(ConfigFile, LineBuffer);
        
        //   If line is a Start Up CsI bloc, Reading toggle to true
        if (LineBuffer.compare(0, 3, "CsI") == 0) {
            cout << "///" << endl           ;
            cout << "CsI found: " << endl   ;
            ReadingStatus = true ;
            
        }
        
        //   Else don't toggle to Reading Block Status
        else ReadingStatus = false ;
        
        //   Reading Block
        while(ReadingStatus){
            // Pickup Next Word
            ConfigFile >> DataBuffer ;
            
            //   Comment Line
            if (DataBuffer.compare(0, 1, "%") == 0) {
                ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
            }
            
            //   Finding another telescope (safety), toggle out
            else if (DataBuffer.compare(0, 3, "CsI") == 0) {
                cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
                ReadingStatus = false ;
            }
            
            //Angle method
            else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
                check_Theta = true;
                ConfigFile >> DataBuffer ;
                Theta = atof(DataBuffer.c_str()) ;
                Theta = Theta ;
                cout << "Theta:  " << Theta << " deg" << endl;
            }
            
            else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
                check_Phi = true;
                ConfigFile >> DataBuffer ;
                Phi = atof(DataBuffer.c_str()) ;
                Phi = Phi ;
                cout << "Phi:  " << Phi << " deg"  << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "R=") == 0) {
                check_R = true;
                ConfigFile >> DataBuffer ;
                R = atof(DataBuffer.c_str()) ;
                R = R ;
                cout << "R:  " << R << " mm" << endl;
            }
            
            //Position method
            else if (DataBuffer.compare(0, 2, "X=") == 0) {
                check_X = true;
                ConfigFile >> DataBuffer ;
                X = atof(DataBuffer.c_str()) ;
                X = X ;
                cout << "X:  " << X << " mm" << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "Y=") == 0) {
                check_Y = true;
                ConfigFile >> DataBuffer ;
                Y = atof(DataBuffer.c_str()) ;
                Y = Y ;
                cout << "Y:  " << Y << " mm" << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "Z=") == 0) {
                check_Z = true;
                ConfigFile >> DataBuffer ;
                Z = atof(DataBuffer.c_str()) ;
                Z = Z ;
                cout << "Z:  " << Z << " mm"  << endl;
            }
            
            
            //General
            else if (DataBuffer.compare(0, 6, "Shape=") == 0) {
                check_Shape = true;
                ConfigFile >> DataBuffer ;
                Shape = DataBuffer ;
                cout << "Shape:  " << Shape << endl;
            }
            
            // Cylindrical shape
            else if (DataBuffer.compare(0, 7, "Radius=") == 0) {
                check_Radius = true;
                ConfigFile >> DataBuffer ;
                Radius = atof(DataBuffer.c_str()) ;
                Radius = Radius ;
                cout << "CsI Radius:  " << Radius << " mm" << endl;
            }
            
            // Trapezoidal shape
            else if (DataBuffer.compare(0, 10, "FaceFront=") == 0) {
                check_FaceFront = true;
                ConfigFile >> DataBuffer ;
                FaceFront = atof(DataBuffer.c_str()) ;
                FaceFront = FaceFront;
                cout << "CsI FaceFront:  " << FaceFront << " mm" << endl;
            }
            
            else if (DataBuffer.compare(0, 9, "FaceBack=") == 0) {
                check_FaceBack = true;
                ConfigFile >> DataBuffer ;
                FaceBack = atof(DataBuffer.c_str()) ;
                FaceBack = FaceBack ;
                cout << "CsI FaceBack:  " << FaceBack << " mm" << endl;
            }
            
            // Common
            else if (DataBuffer.compare(0, 10, "Thickness=") == 0) {
                check_Thickness = true;
                ConfigFile >> DataBuffer ;
                Thickness = atof(DataBuffer.c_str()) ;
                Thickness = Thickness;
                cout << "CsI Thickness:  " << Thickness << " mm" << endl;
            }
            
            else if (DataBuffer.compare(0, 13, "Scintillator=") == 0) {
                check_Scintillator = true ;
                ConfigFile >> DataBuffer ;
                Scintillator = DataBuffer ;
                cout << "CsI Scintillator type:  " << Scintillator << endl;
            }
            
            else if (DataBuffer.compare(0, 14, "LeadThickness=") == 0) {
                check_LeadThickness = true;
                ConfigFile >> DataBuffer ;
                LeadThickness = atof(DataBuffer.c_str()) ;
                LeadThickness = LeadThickness ;
                cout << "Lead Thickness :  " << LeadThickness << " mm" << endl;
            }
            
            ///////////////////////////////////////////////////
            //   If no Detector Token and no comment, toggle out
            else{
                ReadingStatus = false;
                cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;
            }
            
            /////////////////////////////////////////////////
            //   If All necessary information there, toggle out
            
            if (( check_Theta && check_Phi && check_R && check_Thickness
                 && check_Radius && check_LeadThickness && check_Scintillator
                 && check_Shape) // Cylindrical case
                ||
                ( check_X && check_Y && check_Z && check_Thickness && check_Radius
                 && check_LeadThickness && check_Scintillator )
                ||
                ( check_Theta && check_Phi && check_R && check_Thickness
                 && check_FaceBack && check_FaceFront && check_LeadThickness
                 && check_Scintillator && check_Shape ) // Trapezoidal case
                ||
                ( check_X && check_Y && check_Z && check_Thickness && check_FaceBack
                 && check_FaceFront && check_LeadThickness && check_Scintillator )) {
                    
                    if (check_X && check_Y && check_Z){
                        R = sqrt (X*X+Y*Y+Z*Z);
                        Theta = acos(Z / (R) );
                        Phi = atan2(Y,X);
                    }
                    
                    /*if (Shape == "Cylinder")
                        AddCsI(   R                ,
                               Theta             ,
                               Phi               ,
                               Thickness         ,
                               Radius            ,
                               Scintillator   ,
                               LeadThickness   );
                    
                    else if (Shape == "Trapezoidal")
                        AddCsI(   R                ,
                               Theta             ,
                               Phi               ,
                               FaceFront            ,
                               FaceBack               ,
                               Thickness         ,
                               Scintillator   ,
                               LeadThickness   );*/
                    
                    //   Reinitialisation of Check Boolean 
                    
                    check_Theta = false ;
                    check_Phi = false ;
                    check_R = false ;
                    check_Thickness = false ;
                    check_Radius = false ;
                    check_LeadThickness = false ;
                    check_Scintillator = false ;
                    check_FaceFront = false ;
                    check_FaceBack = false ;
                    check_Shape = false ;
                    check_X = false ;
                    check_Y = false ;
                    check_Z = false ;
                    ReadingStatus = false ;   
                    cout << "///"<< endl ;            
                }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::AddParameterToCalibrationManager()
{
    CalibrationManager* Cal = CalibrationManager::getInstance();
    
    for(int i = 0 ; i < NumberOfDetector ; i++)
    {
        for( int j = 0 ; j < 16 ; j++)
        {
            Cal->AddParameter("CsI", "Detector"+itoa(i+1)+"_E","CsI_Detector"+itoa(i+1)+"_E")   ;
            Cal->AddParameter("CsI", "Detector"+itoa(i+1)+"_T","CsI_Detector"+itoa(i+1)+"_T")   ;
        }
        
    }
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::InitializeRootInputRaw()
{
    TChain* inputChain = RootInput::getInstance()->GetChain()     ;
    inputChain->SetBranchStatus ( "CsI"       , true )        ;
    inputChain->SetBranchStatus ( "fCsI_*"    , true )        ;
    inputChain->SetBranchAddress( "CsI"       , &EventData )  ;
}
///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::InitializeRootInputPhysics()
{
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus ( "CsI", true );
    inputChain->SetBranchStatus ( "DetectorNumber", true );
    inputChain->SetBranchStatus ( "Energy", true );
    inputChain->SetBranchStatus ( "Time", true );
    inputChain->SetBranchAddress( "CsI", &EventPhysics );
}
///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::InitializeRootOutput()
{
    TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
    outputTree->Branch( "CsI" , "TCsIPhysics" , &EventPhysics ) ;
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::BuildPhysicalEvent()
{
    BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::BuildSimplePhysicalEvent()
{
    for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
    {
        DetectorNumber.push_back( EventData->GetENumber(i) )   ;
        Energy.push_back( CalibrationManager::getInstance()->ApplyCalibration("CsI/Detector" + itoa( EventData->GetENumber(i) ) +"_E",EventData->GetEEnergy(i) ) );
        Time.push_back( CalibrationManager::getInstance()->ApplyCalibration(   "CsI/Detector" + itoa( EventData->GetENumber(i) ) +"_T",EventData->GetTTime(i) ) );
    }
    
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TCsIPhysics::Construct(){
    return (NPL::VDetector*) new TCsIPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_csi{
    public:
        proxy_csi(){
            NPL::DetectorFactory::getInstance()->AddToken("CsI","CsI");
            NPL::DetectorFactory::getInstance()->AddDetector("CsI",TCsIPhysics::Construct);
        }
    };
    
    proxy_csi p;
}

