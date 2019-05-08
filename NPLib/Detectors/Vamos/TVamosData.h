#ifndef __VamosDATA__
#define __VamosDATA__
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : octobre 2018                                             *
 * Last update    : 09/01/2019                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Vamos Raw data                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// STL
#include <vector>
using namespace std;

// ROOT
#include "TObject.h"

class TVamosData : public TObject {
    //////////////////////////////////////////////////////////////
    // data members are hold into vectors in order
    // to allow multiplicity treatment
    private:
        // Energy
        vector<UShort_t> fVamos_E_DetectorNbr;
        vector<Double_t> fVamos_Energy;

        // Time
        vector<UShort_t> fVamos_T_DetectorNbr;
        vector<Double_t> fVamos_Time;

        // DriftTime in DC
        vector<UShort_t> fVamos_Drift_DetectorNbr;
        vector<Double_t> fVamos_DriftTime;
        vector<Double_t> fVamos_X;

        //////////////////////////////////////////////////////////////
        // Constructor and destructor
    public:
        TVamosData();
        ~TVamosData();

        //////////////////////////////////////////////////////////////
        // Inherited from TObject and overriden to avoid warnings
    public:
        void Clear();
        void Clear(const Option_t*){};
        void Dump() const;

        //////////////////////////////////////////////////////////////
        // Getters and Setters
        // Prefer inline declaration to avoid unnecessary called of
        // frequently used methods
        // add //! to avoid ROOT creating dictionnary for the methods
    public:
        //////////////////////    SETTERS    ////////////////////////
        // Energy
        inline void SetEnergy(const UShort_t& DetNbr, const Double_t& Energy) {
            fVamos_E_DetectorNbr.push_back(DetNbr);
            fVamos_Energy.push_back(Energy);
        }; //!

        // Time
        inline void SetTime(const UShort_t& DetNbr, const Double_t& Time) {
            fVamos_T_DetectorNbr.push_back(DetNbr);
            fVamos_Time.push_back(Time);
        }; //!

        // Position DriftTime and X in DC
        inline void SetDrift(const UShort_t& DetNbr, const Double_t& DriftTime,
                const Double_t& X) {
            fVamos_Drift_DetectorNbr.push_back(DetNbr);
            fVamos_DriftTime.push_back(DriftTime);
            fVamos_X.push_back(X);
        }; //!

        //////////////////////    GETTERS    ////////////////////////
        // Energy
        inline UShort_t GetMultEnergy() const { return fVamos_E_DetectorNbr.size(); }
        inline UShort_t GetE_DetectorNbr(const unsigned int& i) const {
            return fVamos_E_DetectorNbr[i];
        } //!
        inline Double_t Get_Energy(const unsigned int& i) const {
            return fVamos_Energy[i];
        } //!

        // Time
        inline UShort_t GetMultTime() const { return fVamos_T_DetectorNbr.size(); }
        inline UShort_t GetT_DetectorNbr(const unsigned int& i) const {
            return fVamos_T_DetectorNbr[i];
        } //!
        inline Double_t Get_Time(const unsigned int& i) const {
            return fVamos_Time[i];
        } //!

        // Position
        inline UShort_t GetMultDrift() const { return fVamos_Drift_DetectorNbr.size(); }
        inline UShort_t GetDrift_DetectorNbr(const unsigned int& i) const {
            return fVamos_Drift_DetectorNbr[i];
        } //!
        inline Double_t Get_DriftTime(const unsigned int& i) const {
            return fVamos_DriftTime[i];
        } //!

        inline Double_t Get_X(const unsigned int& i) const { return fVamos_X[i]; } //!

        //////////////////////////////////////////////////////////////
        // Required for ROOT dictionnary
        ClassDef(TVamosData, 1) // VamosData structure
};

#endif
