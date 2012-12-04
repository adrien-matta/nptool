#ifndef TTIGRESSPHYSICS_H
#define TTIGRESSPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                 *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:      *
 *                 *
 * Creation Date  :                 *
 * Last update    :                             *
 *---------------------------------------------------------------------------*
 * Decription:     *
 *  This class should deal with TIGRESS gamma data...                        *
 *                 *
 *---------------------------------------------------------------------------*
 * Comment:        *
 *                 *
 *                 *
 *                 *
 *****************************************************************************/
 
 // STL
#include <vector>
#include <stdio.h>

// NPL
#include "TTigressData.h"
#include "../include/CalibrationManager.h"
#include "../include/VDetector.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"


using namespace std ;

class TTigressPhysics : public TObject, public NPA::VDetector
{
   public:
      TTigressPhysics();
      ~TTigressPhysics() {};

   public: 
      void Clear();   
      void Clear(const Option_t*) {};

   public: 
      //vector < TVector2 > Match_Front_Back() ;
      //int  CheckEvent();

  private:   //   Root Input and Output tree classes
            
    TTigressData*         m_EventData;//!
    TTigressData*         m_PreTreatedData;//!
    TTigressPhysics*      m_EventPhysics;//!


  //private:   //   Map of activated channel
  //  map< int, vector<bool> > m_FrontChannelStatus;//!
  //  map< int, vector<bool> > m_BackChannelStatus;//! 
  //  map< int, vector<bool> > m_PADChannelStatus;//!

  //private:   //   Spatial Position of Strip Calculated on bases of detector position
   
   // int m_NumberOfDetector;//!
   // vector< vector < vector < double > > >   m_StripPositionX;//!
   // vector< vector < vector < double > > >   m_StripPositionY;//!
   // vector< vector < vector < double > > >   m_StripPositionZ;//!
         
   ClassDef(TSharcPhysics,1)  // SharcPhysics structure
};








//=============================================//
// ok, so, i don't really know what I am doing.  but the plan here is to make three classes for tigress.  an "array" class which will contain 16(12) positions.  Each position will be occupied by a "clover" class (also be created here), which will contain four position.  The clover class will contain an add-back method, four "crystal" classes, and I think a "bgo" class.   The Crystal class will contain 8 postions (1 for each segment) and holders for all segment and core time(led,cfd,timestamp) and charge.  I Should try to write this one first.


class TigCrystal  {

  public:
    TTigCrystal();
    ~TTigCrystal() {};

    void Clear();
    int WhoAmI(); {return CloverNumber;}

    void SetData();
    void GetPosition(int*,int*);

  protected: 
    
    int ColverNumber;
    void SetPositionData();

  protected: 
   
    //geometery 
    int Segment_1_theata;  
    int Segment_1_phi;   

    int Segment_2_theata;  
    int Segment_2_phi;   

    int Segment_3_theata;  
    int Segment_3_phi;   

    int Segment_4_theata;  
    int Segment_4_phi;   

    int Segment_5_theata;  
    int Segment_5_phi;   

    int Segment_6_theata;  
    int Segment_6_phi;   

    int Segment_7_theata;  
    int Segment_7_phi;   

    int Segment_8_theata;  
    int Segment_8_phi;   

    int Core_theta;
    int Core_phi;


  protected:

    unsigned short charge;
    unsigned short energy;

    unsigned int cfd;
    unsigned int led;
    
    unsigned int timestamp_low;

    vector<unsigned short> segment
  
    vector<unsigned short> s_charge;  
    vector<unsigned short> s_energy;

    vector<unsigned int> s_cfd;
    vector<unsigned int> s_led;

    vector<unsigned int> s_timestamp_low;

};





#endif






















   
  

