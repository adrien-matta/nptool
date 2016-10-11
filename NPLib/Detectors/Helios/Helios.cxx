/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche  contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 30/01/12                                                 *
 * Last update    : 31/08/15                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class is mainly an interface to the                      *
 *             THeliosPhysics class and it deals with the geometrical        *
 *             correspondance between strip number and absolute coordinates  *
 *             (X, Y, Z) of interaction.                                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "Helios.h"

// C++ headers
#include <iostream>	
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>

// NPL headers
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"

// ROOT headers
#include "TChain.h"

using namespace std ;	
	
//	Default Constructor

Helios::Helios()
{
   m_NumberOfModule = 0;
   m_NominalField = 0;
   m_TargThick = 0;
   m_EventData    = new THeliosData();
   m_EventPhysics = new THeliosPhysics();
}	



Helios::~Helios()
{
   m_NumberOfModule = 0;
   m_NominalField = 0;
   m_TargThick = 0;
   delete m_EventData;
   delete m_EventPhysics;
}	



// Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void Helios::ReadConfiguration(string Path) 	
{
   ifstream ConfigFile           	;
   ConfigFile.open(Path.c_str()) 	;
   string LineBuffer          		;
   string DataBuffer          		;	

   // A:X1_Y1     --> X:1    Y:1
   // B:X128_Y1   --> X:128  Y:1
   // C:X1_Y128   --> X:1    Y:128
   // D:X128_Y128 --> X:128  Y:128

   double   Ax, Bx, Cx, Dx, Ay, By, Cy, Dy, Az, Bz, Cz, Dz;
   TVector3 A, B, C, D; 
   double   Theta = 0, Phi = 0, R = 0, beta_u = 0 , beta_v = 0 , beta_w = 0;

   bool check_A = false;
   bool check_C = false;
   bool check_B = false;
   bool check_D = false;

   bool check_Theta = false;
   bool check_Phi   = false;
   bool check_R     = false;
   bool check_beta  = false;
   
   bool ReadingStatus = false;

   bool isSquare     = false;
   bool isDummyShape = false;
   bool isTrapezoid  = false;
   bool isAnnular    = false;

   bool isNomFieldInitialized = false; // nominal field value along beam axis
   bool isTargetThickness = false; // Target thickness 


   while (!ConfigFile.eof()) {

   //
   // Find Input thickness Target
   //
     if(!isTargetThickness)
       {
		ConfigFile >> DataBuffer;
		cout << DataBuffer << endl;
         if (DataBuffer.compare(0, 1, "%") == 0) {/*do nothing */;}
	
         // Position method
         else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
			ConfigFile >> DataBuffer ;
           
			m_TargThick = atof(DataBuffer.c_str()) ;
          
			cout << "/// Target Thickness is: " <<  m_TargThick << endl   ;
	       
			isTargetThickness = true;	   	   
			}
		}	

   //
   // Find Read Magneticfield nominal value
   //
     if(!isNomFieldInitialized)
       {
	 ConfigFile >> DataBuffer;
	 cout << DataBuffer << endl;
         if (DataBuffer.compare(0, 1, "%") == 0) {/*do nothing */;}
	
         // Position method
         else if (DataBuffer.compare(0, 7, "MField=") == 0) {
	   ConfigFile >> DataBuffer ;
           
	   m_NominalField = atof(DataBuffer.c_str()) ;
          
	   cout << "/// Magnetic field nominal value " <<  m_NominalField << endl   ;
	       
	   isNomFieldInitialized = true;	   	   
	 }
       }


     //
     // Then look for the all modules
     //

      getline(ConfigFile, LineBuffer);

     if (LineBuffer.compare(0, 16, "HeliosDummyShape") == 0) {
         cout << "///////////////////////" << endl;
         cout << "Module found:" << endl;        

	 // if (LineBuffer.compare(0,  12, "HeliosSquare")     == 0) isSquare     = true;
	 if (LineBuffer.compare(0, 16, "HeliosDummyShape") == 0) isDummyShape = true;
	 // if (LineBuffer.compare(0, 15, "HeliosTrapezoid")  == 0) isTrapezoid  = true;
	 // if (LineBuffer.compare(0, 13, "HeliosAnnular")    == 0) isAnnular    = true;
         ReadingStatus = true;
      }
      // Else don't toggle to Reading Block Status
      else ReadingStatus = false;

      // Reading Block
      while (ReadingStatus) {	 
	if (isDummyShape) { 	// dummyshape shape
            ConfigFile >> DataBuffer ;
            // Comment Line 
            if (DataBuffer.compare(0, 1, "%") == 0) {
               ConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
            }
            // Finding another telescope (safety), toggle out
            else if (DataBuffer.compare(0, 16, "HeliosDummyShape") == 0) {
               cout << "WARNING: Another Module is find before standard sequence of Token, Error may occured in Telecope definition" << endl;
               ReadingStatus = false;
            }

            // Position method
            else if (DataBuffer.compare(0, 6, "X1_Y1=") == 0) {
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
               cout << "X1 Y1 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
            }
            else if (DataBuffer.compare(0, 8, "X128_Y1=") == 0) {
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
               cout << "X128 Y1 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
            }
            else if (DataBuffer.compare(0, 8, "X1_Y128=") == 0) {
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
               cout << "X1 Y128 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
            }
            else if (DataBuffer.compare(0, 10, "X128_Y128=") == 0) {
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
               cout << "X128 Y128 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;
            } // End Position Method

            // Angle method
            else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
               check_Theta = true;
               ConfigFile >> DataBuffer ;
               Theta = atof(DataBuffer.c_str()) ;
               Theta = Theta ;
               cout << "Theta:  " << Theta << endl;
            }
            else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
               check_Phi = true;
               ConfigFile >> DataBuffer ;
               Phi = atof(DataBuffer.c_str()) ;
               Phi = Phi ;
               cout << "Phi:  " << Phi << endl;
            }
            else if (DataBuffer.compare(0, 2, "R=") == 0) {
               check_R = true;
               ConfigFile >> DataBuffer ;
               R = atof(DataBuffer.c_str()) ;
               R = R ;
               cout << "R:  " << R << endl;
            }
            else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
               check_beta = true;
               ConfigFile >> DataBuffer ;
               beta_u = atof(DataBuffer.c_str()) ;
               beta_u = beta_u    ;
               ConfigFile >> DataBuffer ;
               beta_v = atof(DataBuffer.c_str()) ;
               beta_v = beta_v    ;
               ConfigFile >> DataBuffer ;
               beta_w = atof(DataBuffer.c_str()) ;
               beta_w = beta_w    ;
               cout << "Beta:  " << beta_u << " " << beta_v << " " << beta_w << endl  ;
            }

            /////////////////////////////////////////////////
            // If All necessary information there, toggle out
            if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta)  ) {
               ReadingStatus = false; 

               // Add The previously define telescope
               // With position method
               if ( check_A && check_B && check_C && check_D ) {
                  AddModuleDummyShape(A   ,
                                      B   ,
                                      C   ,
                                      D   ) ;
               }

               // with angle method
               else if ( check_Theta && check_Phi && check_R && check_beta ) {
                  AddModuleDummyShape(Theta,
                                      Phi,
                                      R,
                                      beta_u,
                                      beta_v,
                                      beta_w);
               }

               // reset boolean flag for point positioning
               check_A = false;
               check_B = false;
               check_C = false;
               check_D = false;

               // reset boolean flag for angle positioning
               check_Theta = false;
               check_Phi   = false;
               check_R     = false;
               check_beta  = false;

               // reset boolean flag for shape determination
               isSquare     = false;
               isDummyShape = false;
               isTrapezoid  = false;
               isAnnular    = false;
            } // end test for adding a module
         } // end test for DummyShape shape


      } // end while for reading block
   } // end while for reading file

   cout << endl << "/////////////////////////////" << endl<<endl;
}


// Read stream at Path and pick-up calibration parameter using Token
// If argument is "Simulation" no change calibration is loaded
void Helios::ReadCalibrationFile(string Path)	
{
   Path.c_str();   

}		

	

// Activated associated Branches and link it to the private member DetectorData address
// In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void Helios::InitializeRootInputRaw() 		
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("HELIOS", true);
   inputChain->SetBranchStatus("fHelios*", true);
   inputChain->SetBranchAddress("HELIOS", &m_EventData);
}



// Create associated branches and associated private member DetectorPhysics address
void Helios::InitializeRootOutput() 	
{
   TTree* outputTree = RootOutput::getInstance()->GetTree();
   outputTree->Branch("HELIOS", "THeliosPhysics", &m_EventPhysics);
}



// This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
void Helios::BuildPhysicalEvent()		
{
   m_EventPhysics -> BuildPhysicalEvent(m_EventData);
}



// Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
// This method aimed to be used for analysis performed during experiment, when speed is requiered.
// NB: This method can eventually be the same as BuildPhysicalEvent.
void Helios::BuildSimplePhysicalEvent()	
{
   m_EventPhysics -> BuildSimplePhysicalEvent(m_EventData);
}






void Helios::AddModuleDummyShape(TVector3 C_X1_Y1,
                                         TVector3 C_X128_Y1,
                                         TVector3 C_X1_Y128,
                                         TVector3 C_X128_Y128)
{
   m_NumberOfModule++;

   cout << m_NumberOfModule << endl;

   // remove warning using C_X128_Y128
   C_X128_Y128.Unit();

   // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
   TVector3 U = C_X128_Y1 - C_X1_Y1;
   U = U.Unit();

   // Vector V on Module Face (parallele to X Strip)
   TVector3 V = C_X1_Y128 - C_X1_Y1;
   V = V.Unit();

   // Position Vector of Strip Center
   TVector3 StripCenter = TVector3(0,0,0);
   // Position Vector of X=1 Y=1 Strip 
   TVector3 Strip_1_1;

   // Geometry Parameter
   //Commented by Adrien Matta (produce a warning)
  //  double Face = 50;		// mm
  //double NumberOfStrip = 128;
  //double StripPitch = 0.98; //Face/NumberOfStrip; // mm
   //double NumberOfLStrip = 22; // 1mm
   //double StripLPitch = 1.; //Face/NumberOfStrip; // mm

   //double FaceL = 20.;
   //double NumberOfLStrip = 10;  // 2mm
   double FaceL = 22.;
   double NumberOfLStrip = 11;  // 2mm
   //double FaceL = 24.;
   //double NumberOfLStrip = 12;  // 2mm

   double StripLPitch = FaceL/NumberOfLStrip; // mm
   //double NumberOfTStrip = 250;
   //double StripTPitch = .5; //Face/NumberOfStrip; // mm
   double FaceT = 125.;
   double NumberOfTStrip = 128;
   double StripTPitch = FaceT/NumberOfTStrip; //125./128.; //Face/NumberOfTStrip; // mm
   //double NumberOfTStrip = 62;
   //double StripTPitch = 2.016; //Face/NumberOfStrip; // mm
   //double NumberOfTStrip =25;
   //double StripTPitch = 5.; //Face/NumberOfStrip; // mm


   // Buffer object to fill Position Array
   vector<double> lineX; 
   vector<double> lineY; 
   vector<double> lineZ;

   vector< vector< double > >	OneModuleStripPositionX;
   vector< vector< double > >	OneModuleStripPositionY;
   vector< vector< double > >	OneModuleStripPositionZ;

   // Moving StripCenter to 1.1 corner:
   //Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2.);
   Strip_1_1 = C_X1_Y1 + (U*(StripLPitch/2.)) + (V*(StripTPitch/2.));  // or: C_X1_Y1 + (U * (StripTPitch/2.) + (V* (StripLPitch/2.) !!!  To be checked !!! 

   cout << "Strip_1_1 x=" << Strip_1_1[0] << endl;
   cout << "Strip_1_1 y=" << Strip_1_1[1] << endl;
   cout << "Strip_1_1 z=" << Strip_1_1[2] << endl;

   /*
   for (int i = 0; i < NumberOfStrip; i++) {
      lineX.clear();
      lineY.clear();
      lineZ.clear();

      for (int j = 0; j < NumberOfStrip; j++) {
         StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  );
//         StripCenter += -TargetPosition;

         lineX.push_back( StripCenter.X() );
         lineY.push_back( StripCenter.Y() );
         lineZ.push_back( StripCenter.Z() );	
      }

      OneModuleStripPositionX.push_back(lineX);
      OneModuleStripPositionY.push_back(lineY);
      OneModuleStripPositionZ.push_back(lineZ);
   }
   */
   for (int i = 0; i < NumberOfLStrip; i++) {
      lineX.clear();
      lineY.clear();
      lineZ.clear();

      for (int j = 0; j < NumberOfTStrip; j++) {
	StripCenter  = Strip_1_1 + StripLPitch*( i*U ) + StripTPitch*( j*V  ); // or: Strip_1_1 + StripTPitch*( i*U ) + StripLPitch*( j*V  ) !!! To be checked as above!!!
//         StripCenter += -TargetPosition;

/*
  	if(i<3 && j<5){
	cout << "i:" << i << " j:" << j << endl;
	cout << "StripCenter x= "<<  StripCenter[0] << endl;
	cout << "StripCenter y= "<<  StripCenter[1] << endl;
	cout << "StripCenter z= "<<  StripCenter[2] << endl;
	}	
*/
         lineX.push_back( StripCenter.X() );
         lineY.push_back( StripCenter.Y() );
         lineZ.push_back( StripCenter.Z() );	
      }

      OneModuleStripPositionX.push_back(lineX);
      OneModuleStripPositionY.push_back(lineY);
      OneModuleStripPositionZ.push_back(lineZ);
   }

   m_StripPositionX.push_back( OneModuleStripPositionX );
   m_StripPositionY.push_back( OneModuleStripPositionY );
   m_StripPositionZ.push_back( OneModuleStripPositionZ );
}



void Helios::AddModuleDummyShape(double theta,
                                         double phi,
                                         double distance,
                                         double beta_u,
                                         double beta_v,
                                         double beta_w)
{
   m_NumberOfModule++;

   // convert from degree to radian:
   double Pi = 3.141592654;
   theta = theta * Pi/180. ; 
   phi   = phi   * Pi/180. ;

   // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
   TVector3 U ;	
   // Vector V on Module Face (parallele to X Strip)
   TVector3 V ;
   // Vector W normal to Module Face (pointing CsI)
   TVector3 W ;
   // Vector position of Module Face center
   TVector3 C ;

   C = TVector3(distance * sin(theta) * cos(phi),
                distance * sin(theta) * sin(phi),
                distance * cos(theta));

  TVector3 YperpW = TVector3( cos(theta) * cos(phi),
                              cos(theta) * sin(phi),
                             -sin(theta));

   W = C.Unit();
   U = W.Cross(YperpW);
   V = W.Cross(U);

   U = U.Unit();
   V = V.Unit();

   U.Rotate( beta_u * Pi/180. , U ) ;
   V.Rotate( beta_u * Pi/180. , U ) ;

   U.Rotate( beta_v * Pi/180. , V ) ;
   V.Rotate( beta_v * Pi/180. , V ) ;

   U.Rotate( beta_w * Pi/180. , W ) ;
   V.Rotate( beta_w * Pi/180. , W ) ;

   double Face = 1; // mm
   double NumberOfStrip = 1;
   //double NumberOfStrip = 500;
   double StripPitch = 9.8;  // Face/NumberOfStrip; // mm

   vector<double> lineX;
   vector<double> lineY;
   vector<double> lineZ;

   vector< vector< double > >	OneModuleStripPositionX;
   vector< vector< double > >	OneModuleStripPositionY;
   vector< vector< double > >	OneModuleStripPositionZ;
		
   double X, Y, Z;

   // Moving C to the 1.1 corner:
   C.SetX( C.X() - ( Face/2 - StripPitch/2 ) * ( V.X() + U.X() ) )  ; 
   C.SetY( C.Y() - ( Face/2 - StripPitch/2 ) * ( V.Y() + U.Y() ) )  ; 
   C.SetZ( C.Z() - ( Face/2 - StripPitch/2 ) * ( V.Z() + U.Z() ) )  ; 

   for (int i = 0; i < NumberOfStrip; i++) {
      lineX.clear();
      lineY.clear();
      lineZ.clear();

      for (int j = 0; j < NumberOfStrip; j++) {
         X = C.X() + StripPitch * ( U.X()*i + V.X()*j );
         Y = C.Y() + StripPitch * ( U.Y()*i + V.Y()*j );
         Z = C.Z() + StripPitch * ( U.Z()*i + V.Z()*j );

         lineX.push_back(X);
         lineY.push_back(Y);
         lineZ.push_back(Z);
      }

      OneModuleStripPositionX.push_back(lineX);
      OneModuleStripPositionY.push_back(lineY);
      OneModuleStripPositionZ.push_back(lineZ);
   }

   m_StripPositionX.push_back( OneModuleStripPositionX );
   m_StripPositionY.push_back( OneModuleStripPositionY );
   m_StripPositionZ.push_back( OneModuleStripPositionZ );


}



double Helios::GetEnergyDeposit()		
{ 
   //if (m_EventPhysics->TotalEnergy.size() > 0)
   if (m_EventPhysics->GetEventMultiplicity() > 0)
      return m_EventPhysics->fTotalEnergy[0]; 
   else 
      return -1000;
}

double Helios::GetTimeDetected()		
{ 
   if (m_EventPhysics->fTotalTime.size() > 0)
      return m_EventPhysics->fTotalTime[0]; 
   else 
      return -1000;
}



TVector3 Helios::GetPositionOfInteraction()
{
/* !! not possible with helios as we don't measure Y -> use interaction coordinate Z instead !!
 * 
 *    TVector3 Position = TVector3(-1000,-1000,-1000);


   if (m_EventPhysics->fModuleNumber.size() == 1) {

		cout << "Module=" << m_EventPhysics->fModuleNumber[0] << endl;
        cout << "1ststage X" << m_EventPhysics->fFirstStage_X[0] << endl;  
        cout << "1ststage Y" << m_EventPhysics->fFirstStage_Y[0] << endl;  


     Position = TVector3(GetStripPositionX((m_EventPhysics->fModuleNumber[0])-1000, m_EventPhysics->fFirstStage_X[0], m_EventPhysics->fFirstStage_Y[0]),
			  GetStripPositionY((m_EventPhysics->fModuleNumber[0])-1000, m_EventPhysics->fFirstStage_X[0], m_EventPhysics->fFirstStage_Y[0]),
			  GetStripPositionZ((m_EventPhysics->fModuleNumber[0])-1000, m_EventPhysics->fFirstStage_X[0], m_EventPhysics->fFirstStage_Y[0])); 
   }

   return(Position);
*/
}



void Helios::Print()
{
   cout << "Number of Modules: " << m_NumberOfModule << endl;

   for (int f = 0; f < m_NumberOfModule; f++) {
      cout << "Module " << f+1 << endl;

      for (int i = 0; i < 128; i++) {
         for (int j = 0; j < 128; j++) {
            cout << i+1 << "  "<< j+1 << "  " 
                 << m_StripPositionX[f][i][j]   << "  " 
                 << m_StripPositionY[f][i][j]   << "  "
                 << m_StripPositionZ[f][i][j]   << "  " 
                 << endl ;
         }
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* Helios::Construct(){
  return (NPL::VDetector*) new Helios();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_helios{
  public:
    proxy_helios(){
      NPL::DetectorFactory::getInstance()->AddToken("Helios","Helios");
      NPL::DetectorFactory::getInstance()->AddDetector("Helios",Helios::Construct);
    }
};

proxy_helios p;
}
