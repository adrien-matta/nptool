#ifndef VDectector_H
#define VDectector_H
/*****************************************************************************
 * Copyright (C) 2009 	this file is part of the NPTool Project              *
 * 				                                                             *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   :   June 2009                                             *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:  Class VDetector (virtual) for NPAnalysis                     *
 * All detector class used in NPAnalysis should derived from this virtual    *
 *  class. Those VDetector daughter will deal with geometry, calibration and *
 *  basic data treatment.                                                    *
 *  	                                                                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   See MUST2 array for exemple of VDetector derived class                  *
 *                                                                           *
 *****************************************************************************/
// STL header
#include <string>

using namespace std;

namespace NPA 
	{
				
		class VDetector
		{
			public:

				//	Default Constructor and destructor
				VDetector()   ;
				virtual ~VDetector()   ;


				//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
				virtual void ReadConfiguration(string) 		{}		;
		
				//	Add Parameter to the CalibrationManger
				virtual void AddParameterToCalibrationManager()	{} ;		

				//	XXX To be DUMPED XXX
				//	Read stream at CalibFile and pick-up calibration parameter using Token
				//	If argument is "Simulation" no change calibration is loaded
				virtual void ReadCalibrationFile(string)	{} ;		
					
				
				//	Activated associated Branches and link it to the private member DetectorData address
				//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
				virtual void InitializeRootInput() 			{}		;


				//	Create associated branches and associated private member DetectorPhysics address
				virtual void InitializeRootOutput() 		{} 	;
				
				
				//	This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
				virtual void BuildPhysicalEvent()			{}		;
				
				
				//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
				//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
				//	NB: This method can eventually be the same as BuildPhysicalEvent.
				virtual void BuildSimplePhysicalEvent()	{}		;
				
				//	Those two method all to clear the Event Physics or Data
				virtual void ClearEventPhysics()		{}		;
				virtual void ClearEventData()			{}		;
				
			private:	//	The list below is here to help you building your own detector
			/*
				//	GRU and Simulated Tree output are made of Data Object wich contain all the RAW parameter output by Detectors during an experiment.
				//	You need to add a pointer to those kind of object in order to received data from the Tree at each getEntry call. Just replace Detector
				//	by your Detector name, like Must2Data for Must2
				
				DetectorData* EventData	;
				
				//	Aime of analysis is to treat those raw data and output some physical data, during this process, Raw data will be calibrated, threshold applied,
				//	and whatever you need to do. Those Physical data are output in a DetectorPhysics object, attached in the output Tree
				
				DetectorPhysics* EventPhysics ;
				
				//	Position of detector: An array which held geometric information of detector, such as Strip position,...
				//	Dimension must be suited for your need
				
				vector< vector <double > >	DetectorPosition	;
				
				//	Calibration are passed to the EventPhysics Object, so you don't need to store them here
				
				nothing	noCalibration	;
				
			*/
		};

	}

#endif
