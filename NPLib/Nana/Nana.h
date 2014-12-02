
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    : 02/07/2014                                               *
 *---------------------------------------------------------------------------*
 * Decription: This class is mainly an interface to the                      *
 *             TNanaPhysics class                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef Nana_H
#define Nana_H

// NPL
#include "../include/VDetector.h"
#include "TNanaData.h"
#include "TNanaPhysics.h"

// Root
#include "TVector3.h"

class Nana : public NPA::VDetector
{
public:
   Nana();
   virtual ~Nana();

public:
   /////////////////////////////////////
   // Innherited from VDetector Class //
   /////////////////////////////////////
   // Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
   void ReadConfiguration(string);

   // Read stream at CalibFile and pick-up calibration parameter using Token
   // If argument is "Simulation" no change calibration is loaded
   void ReadCalibrationFile(string);

   // Activated associated Branches and link it to the private member DetectorData address
   // In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
   void InitializeRootInput();

   // Create associated branches and associated private member DetectorPhysics address
   void InitializeRootOutput();

   // This method is called at each event read from the Input Tree. 
   // The aim is to build treat Raw dat in order to extract physical parameter. 
   void BuildPhysicalEvent();
	
   // Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
   // This method aimed to be used for analysis performed during experiment, when speed is requiered.
   // NB: This method can eventually be the same as BuildPhysicalEvent.
   void BuildSimplePhysicalEvent();

   // Those two method all to clear the Event Physics or Data
   void ClearEventPhysics()	{m_EventPhysics->Clear();}
   void ClearEventData()	{m_EventData->Clear();}


public:
   ////////////////////////////////
   // Specific to Nana //
   ////////////////////////////////
   // Case of a Square module
   // Add a Module using Corner Coordinate information
   void AddModuleSquare(TVector3 C_X1_Y1,
                        TVector3 C_X128_Y1,
                        TVector3 C_X1_Y128,
                        TVector3 C_X128_Y128);

   // Add a Module using R Theta Phi of Si center information
   void AddModuleSquare(double theta,
                        double phi,
                        double distance, 
                        double beta_u,
                        double beta_v,
                        double beta_w);

   // Case of a DummyShape module
   // Add a Module using Corner Coordinate information
   void AddModuleDummyShape(TVector3 C_X1_Y1,
                            TVector3 C_X128_Y1,
                            TVector3 C_X1_Y128,
                            TVector3 C_X128_Y128);

   // Add a Module using R Theta Phi of Si center information
   void AddModuleDummyShape(double theta,
                            double phi,
                            double distance, 
                            double beta_u,
                            double beta_v,
                            double beta_w);

   // Getters to retrieve the (X,Y,Z) coordinates of a pixel (crystal) defined by (X,Y) for Detector
   
   double GetDetPositionX(int N ,int X ,int Y)	{ return m_DetPositionX[N-1][X][Y]; }  // remember index=0 for Nana Detector (cf NanaModule.cxx)
   double GetDetPositionY(int N ,int X ,int Y)	{ return m_DetPositionY[N-1][X][Y]; }
   double GetDetPositionZ(int N ,int X ,int Y)	{ return m_DetPositionZ[N-1][X][Y]; }

   double GetNumberOfDetector()	 			{ return m_NumberOfDetector; }

   /*
   // Getters to retrieve the (X,Y,Z) coordinates of a pixel (crystal) defined by (X,Y) for clusters
   
   double GetCrystPositionX(int N ,int X ,int Y)	{ return m_CrystPositionX[N-101][X][Y]; }  // remember index=100 for Nana Cluster (cf NanaModule.cxx)
   double GetCrystPositionY(int N ,int X ,int Y)	{ return m_CrystPositionY[N-101][X][Y]; }
   double GetCrystPositionZ(int N ,int X ,int Y)	{ return m_CrystPositionZ[N-101][X][Y]; }
   */


   //double GetStripPositionX(int N ,int X ,int Y)	{ return m_StripPositionX[N-1][X-1][Y-1]; }
   //double GetStripPositionY(int N ,int X ,int Y)	{ return m_StripPositionY[N-1][X-1][Y-1]; }
   //double GetStripPositionZ(int N ,int X ,int Y)	{ return m_StripPositionZ[N-1][X-1][Y-1]; }

   //double GetNumberOfModule()	 			{ return m_NumberOfModule; }


   // Get Root input and output objects
   TNanaData* 	GetEventData()		{return m_EventData;}
   TNanaPhysics*	GetEventPhysics()	{return m_EventPhysics;}

   // To be called after a build Physical Event 
   double	GetEnergyDeposit();
   double	GetEnergyInDeposit();
   double	GetEnergyOutDeposit();
   TVector3	GetPositionOfInteraction();

   void		Print();


private:
   ////////////////////////////////////////
   // Root Input and Output tree classes //
   ////////////////////////////////////////
   TNanaData*		m_EventData;
   TNanaPhysics*	m_EventPhysics;


private:
   // Spatial Position of Crystal Calculated on basis of detector(cluster) position
   /*
   int m_NumberOfModule;  // cluster
   vector< vector < vector < double > > >	m_CrystPositionX;
   vector< vector < vector < double > > >	m_CrystPositionY;
   vector< vector < vector < double > > >	m_CrystPositionZ;
   */
   int m_NumberOfDetector; // single detector
   vector< vector < vector < double > > >	m_DetPositionX;
   vector< vector < vector < double > > >	m_DetPositionY;
   vector< vector < vector < double > > >	m_DetPositionZ;
  
};

#endif
