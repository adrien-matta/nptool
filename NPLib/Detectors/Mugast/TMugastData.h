#ifndef MUGASTDATA_H
#define MUGASTDATA_H
/*****************************************************************************
 * Copyright (C) 2009-2018    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2018                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MUGAST Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include <map>
#include <iostream>
#include "TObject.h"


// Type of detector to index the correct strip map
enum MG_DetectorType{
  MG_SQUARE,
  MG_TRAPEZE,
  MG_ANNULAR,
  MG_NOCHANGE
  };

using namespace std ;
class TMugastData : public TObject {
   private:
      // First Layer
      // X strips
      // Energy
      vector<unsigned short>   fMG_DSSDXE_DetectorNbr;
      vector<unsigned short>   fMG_DSSDXE_StripNbr;
      vector<double>           fMG_DSSDXE_Energy;
      // Time
      vector<unsigned short>   fMG_DSSDXT_DetectorNbr;
      vector<unsigned short>   fMG_DSSDXT_StripNbr;
      vector<double>           fMG_DSSDXT_Time;
      // Y strips
      // Energy
      vector<unsigned short>   fMG_DSSDYE_DetectorNbr;
      vector<unsigned short>   fMG_DSSDYE_StripNbr;
      vector<double>           fMG_DSSDYE_Energy;
      // Time
      vector<unsigned short>   fMG_DSSDYT_DetectorNbr;
      vector<unsigned short>   fMG_DSSDYT_StripNbr;
      vector<double>           fMG_DSSDYT_Time;

      // Second Layer
      // Front
      // Energy 
      vector<unsigned short>   fMG_SecondLayerE_DetectorNbr;
      vector<unsigned short>   fMG_SecondLayerE_StripNbr;
      vector<double>           fMG_SecondLayerE_Energy;
      // Time
      vector<unsigned short>   fMG_SecondLayerT_DetectorNbr;
      vector<unsigned short>   fMG_SecondLayerT_StripNbr;
      vector<double>           fMG_SecondLayerT_Time;

   private:
      std::map<unsigned int, unsigned int> fMG_MapTrapezeX;//!
      std::map<unsigned int, unsigned int> fMG_MapSquareX;//!
      std::map<unsigned int, unsigned int> fMG_MapAnnularX;//!
      std::map<unsigned int, unsigned int> fMG_MapTrapezeY;//!
      std::map<unsigned int, unsigned int> fMG_MapSquareY;//!
      std::map<unsigned int, unsigned int> fMG_MapAnnularY;//!
  

   public:
      TMugastData();
      virtual ~TMugastData();

      void   Clear();
      void   Clear(const Option_t*) {};
      void   Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      // FirstLayer
      // (X,E)
      public:
      inline void   SetDSSDXE(MG_DetectorType type,const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
        static unsigned int newStrip;
      newStrip = StripNbr;
      if(type==MG_SQUARE)
       newStrip=fMG_MapSquareX[StripNbr]; 
      else if(type==MG_TRAPEZE)
       newStrip=fMG_MapTrapezeX[StripNbr]; 
      else if(type==MG_ANNULAR)
       newStrip=fMG_MapAnnularX[StripNbr];  
      
      SetDSSDXE(DetNbr,newStrip,Energy); 

      }
      private:
      inline void   SetDSSDXE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
        fMG_DSSDXE_DetectorNbr.push_back(DetNbr);
        fMG_DSSDXE_StripNbr.push_back(StripNbr);
        fMG_DSSDXE_Energy.push_back(Energy);
      }
      
      // (X,T)
      public:
      inline void   SetDSSDXT(MG_DetectorType type,const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){
        static unsigned int newStrip;

      newStrip = StripNbr;
      if(type==MG_SQUARE)
       newStrip=fMG_MapSquareX[StripNbr]; 
      else if(type==MG_TRAPEZE)
       newStrip=fMG_MapTrapezeX[StripNbr]; 
      else if(type==MG_ANNULAR)
       newStrip=fMG_MapAnnularX[StripNbr];  

      SetDSSDXT(DetNbr,newStrip,Time); 
      }
 
     private:
     inline void   SetDSSDXT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){   
      fMG_DSSDXT_DetectorNbr.push_back(DetNbr);  
      fMG_DSSDXT_StripNbr.push_back(StripNbr);       
      fMG_DSSDXT_Time.push_back(Time);  
     } 
       // (Y,E)
     public:
       inline void   SetDSSDYE(MG_DetectorType type,const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
        static unsigned int newStrip;

      newStrip = StripNbr;
      if(type==MG_SQUARE)
       newStrip=fMG_MapSquareY[StripNbr]; 
      else if(type==MG_TRAPEZE)
       newStrip=fMG_MapTrapezeY[StripNbr]; 
      else if(type==MG_ANNULAR)
       newStrip=fMG_MapAnnularY[StripNbr];  

      SetDSSDYE(DetNbr,newStrip,Energy); 
      }
 
      private:
      inline void   SetDSSDYE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
        fMG_DSSDYE_DetectorNbr.push_back(DetNbr);
        fMG_DSSDYE_StripNbr.push_back(StripNbr);
        fMG_DSSDYE_Energy.push_back(Energy);
      }
      
      // (Y,T)
      public:
      inline void   SetDSSDYT(MG_DetectorType type,const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){
        static unsigned int newStrip;

      newStrip = StripNbr;
      if(type==MG_SQUARE)
       newStrip=fMG_MapSquareY[StripNbr]; 
      else if(type==MG_TRAPEZE)
       newStrip=fMG_MapTrapezeY[StripNbr]; 
      else if(type==MG_ANNULAR)
       newStrip=fMG_MapAnnularY[StripNbr];  

      SetDSSDYT(DetNbr,newStrip,Time); 
      }
 

      private:
     inline void   SetDSSDYT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){   
      fMG_DSSDYT_DetectorNbr.push_back(DetNbr);  
      fMG_DSSDYT_StripNbr.push_back(StripNbr);       
      fMG_DSSDYT_Time.push_back(Time);  
     } 
     public: 
      // Second Layer
      // E
      inline void SetSecondLayerE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
        fMG_SecondLayerE_DetectorNbr.push_back(DetNbr);
        fMG_SecondLayerE_StripNbr.push_back(StripNbr);
        fMG_SecondLayerE_Energy.push_back(Energy);
      }
      
      // T
     inline void SetSecondLayerT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){   
      fMG_SecondLayerT_DetectorNbr.push_back(DetNbr);  
      fMG_SecondLayerT_StripNbr.push_back(StripNbr);       
      fMG_SecondLayerT_Time.push_back(Time);  
     } 
 
public:
      /////////////////////           GETTERS           ////////////////////////
      // DSSD
      // (X,E)
      inline unsigned short   GetDSSDXEMult()                      const {return fMG_DSSDXE_DetectorNbr.size();}
      inline unsigned short   GetDSSDXEDetectorNbr(const int& i)   const {return fMG_DSSDXE_DetectorNbr[i];}
      inline unsigned short   GetDSSDXEStripNbr(const int& i)       const {return fMG_DSSDXE_StripNbr[i];}
      inline double           GetDSSDXEEnergy(const int& i)        const {return fMG_DSSDXE_Energy[i];}
      // (X,T)
      inline unsigned short   GetDSSDXTMult()                      const {return fMG_DSSDXT_DetectorNbr.size();}
      inline unsigned short   GetDSSDXTDetectorNbr(const int& i)   const {return fMG_DSSDXT_DetectorNbr[i];}
      inline unsigned short   GetDSSDXTStripNbr(const int& i)       const {return fMG_DSSDXT_StripNbr[i];}
      inline double           GetDSSDXTTime(const int& i)          const {return fMG_DSSDXT_Time[i];}
      // (Y,E)
      inline unsigned short   GetDSSDYEMult()                      const {return fMG_DSSDYE_DetectorNbr.size();}
      inline unsigned short   GetDSSDYEDetectorNbr(const int& i)   const {return fMG_DSSDYE_DetectorNbr[i];}
      inline unsigned short   GetDSSDYEStripNbr(const int& i)       const {return fMG_DSSDYE_StripNbr[i];}
      inline double           GetDSSDYEEnergy(const int& i)        const {return fMG_DSSDYE_Energy[i];}
      // (Y,T)
      inline unsigned short   GetDSSDYTMult()                      const {return fMG_DSSDYT_DetectorNbr.size();}
      inline unsigned short   GetDSSDYTDetectorNbr(const int& i)   const {return fMG_DSSDYT_DetectorNbr[i];}
      inline unsigned short   GetDSSDYTStripNbr(const int& i)       const {return fMG_DSSDYT_StripNbr[i];}
      inline double           GetDSSDYTTime(const int& i)          const {return fMG_DSSDYT_Time[i];}

      // Second Layer 
      //(E)
      inline unsigned short   GetSecondLayerEMult()                     const {return fMG_SecondLayerE_DetectorNbr.size();}
      inline unsigned short   GetSecondLayerEDetectorNbr(const int& i)  const {return fMG_SecondLayerE_DetectorNbr[i];}
      inline unsigned short   GetSecondLayerEStripNbr(const int& i)      const {return fMG_SecondLayerE_StripNbr[i];}
      inline double           GetSecondLayerEEnergy(const int& i)       const {return fMG_SecondLayerE_Energy[i];}
      //(T)
      inline unsigned short   GetSecondLayerTMult()                     const {return fMG_SecondLayerT_DetectorNbr.size();}
      inline unsigned short   GetSecondLayerTDetectorNbr(const int& i)  const {return fMG_SecondLayerT_DetectorNbr[i];}
      inline unsigned short   GetSecondLayerTStripNbr(const int& i)      const {return fMG_SecondLayerT_StripNbr[i];}
      inline double           GetSecondLayerTTime(const int& i)         const {return fMG_SecondLayerT_Time[i];}


      ClassDef(TMugastData,1)  // MugastData structure
};

#endif
