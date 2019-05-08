#ifndef __LightPipeDATA__
#define __LightPipeDATA__
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold LightPipe Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <vector>
#include <iostream>

// ROOT
#include "TObject.h"

class TLightPipeData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
public:
	// Energy (# of photons)
	// Left side
	std::vector<UShort_t>  fLightPipe_Left_LayerNbr;
	std::vector<UShort_t>  fLightPipe_Left_RowNbr;
	std::vector<Double_t>  fLightPipe_Left_Energy;
	// Right side
	std::vector<UShort_t>  fLightPipe_Right_LayerNbr;
	std::vector<UShort_t>  fLightPipe_Right_RowNbr;
	std::vector<Double_t>  fLightPipe_Right_Energy;
	// Top
	std::vector<UShort_t>  fLightPipe_Top_LayerNbr;
	std::vector<UShort_t>  fLightPipe_Top_RowNbr;
	std::vector<Double_t>  fLightPipe_Top_Energy;
	// Bottom
	std::vector<UShort_t>  fLightPipe_Bottom_LayerNbr;
	std::vector<UShort_t>  fLightPipe_Bottom_RowNbr;
	std::vector<Double_t>  fLightPipe_Bottom_Energy;

public:
	enum  { Left_t, Right_t, Top_t, Bottom_t };
	
  //////////////////////////////////////////////////////////////
  // Constructor and destructor
public: 
	TLightPipeData();
	~TLightPipeData();
    

  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
public:
	void Clear()
		{
			fLightPipe_Left_LayerNbr.clear();
			fLightPipe_Left_RowNbr.clear();
			fLightPipe_Left_Energy.clear();

			fLightPipe_Right_LayerNbr.clear();
			fLightPipe_Right_RowNbr.clear();
			fLightPipe_Right_Energy.clear();

			fLightPipe_Top_LayerNbr.clear();
			fLightPipe_Top_RowNbr.clear();
			fLightPipe_Top_Energy.clear();

			fLightPipe_Bottom_LayerNbr.clear();
			fLightPipe_Bottom_RowNbr.clear();
			fLightPipe_Bottom_Energy.clear();
		}
	void Clear(const Option_t*) {};
	void Dump() const
		{
#if 0
			// This method is very useful for debuging and worth the dev.
			std::cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TLightPipeData::Dump()] XXXXXXXXXXXXXXXXX\n";
			// Energy
			size_t mysize = fLightPipe_E_DetectorNbr.size();
			std::cout << "LightPipe_E_Mult: " << mysize << "\n";
			for (size_t i = 0 ; i < mysize ; i++){
				std::cout << "DetNbr: " << fLightPipe_E_DetectorNbr[i]
									<< "RowNbr (x): " << fLightPipe_E_RowNbr[i]
									<< "ColumnNbr (y): " << fLightPipe_E_ColumnNbr[i]
									<< "DetNbr (z): " << fLightPipe_E_DetectorNbr[i]
									<< " Energy: " << fLightPipe_Energy[i];
			}
			std::cout << "\n";
#endif
		}


  //////////////////////////////////////////////////////////////
  // Getters and Setters
  // Prefer inline declaration to avoid unnecessary called of 
  // frequently used methods
  // add //! to avoid ROOT creating dictionnary for the methods
public:
	//////////////////////    SETTERS    ////////////////////////
	// Energy
	void SetEnergy(Int_t side, UShort_t layer, UShort_t row, Double_t energy) {
		switch(side) {
		case Left_t:
			fLightPipe_Left_LayerNbr.push_back(layer);
			fLightPipe_Left_RowNbr.push_back(row);
			fLightPipe_Left_Energy.push_back(energy);
			break;
		case Right_t:
			fLightPipe_Right_LayerNbr.push_back(layer);
			fLightPipe_Right_RowNbr.push_back(row);
			fLightPipe_Right_Energy.push_back(energy);
			break;
		case Top_t:
			fLightPipe_Top_LayerNbr.push_back(layer);
			fLightPipe_Top_RowNbr.push_back(row);
			fLightPipe_Top_Energy.push_back(energy);
			break;
		case Bottom_t:
			fLightPipe_Bottom_LayerNbr.push_back(layer);
			fLightPipe_Bottom_RowNbr.push_back(row);
			fLightPipe_Bottom_Energy.push_back(energy);
			break;
		default:
			std::cerr << "WARNING: Invalid side: " << side << " specified to TLightPipeData::SetEnergy(); nothing will be set!\n";
			break;
		}
	};//!

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TLightPipeData,1);  // LightPipeData structure
};

#endif
