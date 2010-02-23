#ifndef __PlasticPhysics__
#define __PlasticPhysics__
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold thePlastic Detector  Physics                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 
//	STL
#include <vector>
using namespace std ;

//	ROOT
#include "TObject.h"

//	NPL
#include "TPlasticData.h"
#include "../include/VDetector.h"
#include "../include/CalibrationManager.h"

class TPlasticPhysics : public TObject, public NPA::VDetector
{
	public:	//	Constructor and Destructor
		TPlasticPhysics();
		~TPlasticPhysics();

	public:	//	Calibrated Data

		vector<UShort_t>	DetectorNumber	;
		vector<Double_t>	Energy					;
		vector<Double_t>	Time						;

	public:	//	inherrited from VDetector
		//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
		void ReadConfiguration(string) 				;
		

		//	Add Parameter to the CalibrationManger
		void AddParameterToCalibrationManager()	;		
			
		
		//	Activated associated Branches and link it to the private member DetectorData address
		//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
		void InitializeRootInput() 					;


		//	Create associated branches and associated private member DetectorPhysics address
		void InitializeRootOutput() 		 		;
		
		
		//	This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
		void BuildPhysicalEvent()					;
		
		
		//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
		//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
		//	NB: This method can eventually be the same as BuildPhysicalEvent.
		void BuildSimplePhysicalEvent()				;

		//	Those two method all to clear the Event Physics or Data
		void ClearEventPhysics()		{Clear();}		
		void ClearEventData()				{EventData->Clear();}		

	private:	// Data not writted in the tree
				int									NumberOfDetector	;//!
				TPlasticData* 	  	EventData					;//!
				TPlasticPhysics* 	  EventPhysics			;//!

		void	Clear();
    void  Clear(const Option_t*) {};
	
		ClassDef(TPlasticPhysics,1)  // PlasticPhysics structure
};

#endif
