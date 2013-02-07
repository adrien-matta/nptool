#ifndef TTigEventFragment_H
#define TTigEventFragment_H
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:    contact address:                                      *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include"TObject.h"
#include"TH1F.h"
#include <vector>
using namespace std;

class TTigEventFragment : public TObject{

  public:
    TTigEventFragment();
    // Create the class and reserve the vector size for MaxChannel
    // Limit the number of dynamic allocation, so better perf but more memory used
    TTigEventFragment(int MaxChannel);
    ~TTigEventFragment();
    
  public:
     int tig_num_chan;
	   int tig_event_id;

	  vector<int> tig_midas_id;
  
    //0 for tig10 and 1 for tig 64
    vector<int> tig_type;

	  vector<int> channel_number;
    vector<int> channel_raw;
	  vector<int> cfd_value;
	  vector<int> led_value;
	  vector<int> charge_raw;
	  vector<float> charge_cal;

	  vector<int> timestamp_low;
	  vector<int> timestamp_high;
	  vector<int> timestamp_live;
	  vector<int> timestamp_tr;  // triggers requested
	  vector<int> timestamp_ta;  // triggers accepted

    vector<TH1F> waveform;

  public:
      void   Clear() ;
      void   Clear(const Option_t*) {};
      void   Dump() const {};

  ClassDef(TTigEventFragment,1)  // TTigEventFragment structure
};
#endif
