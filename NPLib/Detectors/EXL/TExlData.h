#ifndef __EXLDATA__
#define __EXLDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $GANIL2ROOT/Licence                           *
 * For the list of contributors see $GANIL2ROOT/README/CREDITS               *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                    							     *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds EXL Raw data                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#include <vector>

#include "TObject.h"
using namespace std ;


class TExlData : public TObject {
    private:
        // ADC
        vector<double>  fExlE_Energy;
        vector<short>   fExlE_Number ;
        vector<short>   fExlE_Crystal;
        
        vector<double>  fExlT_Time;
        vector<short>   fExlT_Number ;
        vector<short>   fExlT_Crystal;


    public:
        TExlData();
        virtual ~TExlData();

        void   Clear();
        void   Clear(const Option_t*) {};
        void   Dump() const;

        //////////////////////////////////////////////////////////////
        // Getters and Setters
        // Prefer inline declaration to avoid unnecessary called of 
        // frequently used methods
        // add //! to avoid ROOT creating dictionnary for the methods
    public:
        /////////////////////           GETTERS           ////////////////////////
        inline double   GetEnergy(const int& i)        const { return fExlE_Energy[i] ;}
        double          GetEnergyCrystalNumber(const int crystal); 
        double          GetTimeCrystalNumber(const int crystal); 
        inline double   GetTime(const int& i)          const { return fExlT_Time[i] ;}
        inline int      GetE_ExlNumber(const int& i)     const { return fExlE_Number[i] ;}
        inline int      GetE_CrystalNumber(const int& i) const { return fExlE_Crystal[i] ;}
        inline int      GetT_ExlNumber(const int& i)     const { return fExlT_Number[i] ;}
        inline int      GetT_CrystalNumber(const int& i) const { return fExlT_Crystal[i] ;}

        //Mult
        inline unsigned int GetEMult() const {return fExlE_Energy.size();}
        inline unsigned int GetTMult() const {return fExlT_Time.size();}


        /////////////////////           SETTERS           ////////////////////////
        inline void SetEnergy(const int& N,const int& C,const double& E){
            fExlE_Number.push_back(N) ; 	   
            fExlE_Crystal.push_back(C); 	   
            fExlE_Energy.push_back(E);
        }
        inline void SetTime(const int& N,const int& C,const double& T){
            fExlT_Number.push_back(N) ; 	   
            fExlT_Crystal.push_back(C); 	   
            fExlT_Time.push_back(T);
        }
        ClassDef(TExlData,1)  // ExlData structure
};

#endif
