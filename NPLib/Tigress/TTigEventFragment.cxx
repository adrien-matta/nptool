#include"TTigEventFragment.h"

ClassImp(TTigEventFragment)

extern int midas_events_read;

TTigEventFragment::TTigEventFragment(){
  Clear();
}


TTigEventFragment::TTigEventFragment(int MaxChannel){
	  tig_midas_id.reserve(MaxChannel);
	  tig_type.reserve(MaxChannel);
	  channel_number.reserve(MaxChannel);
    channel_raw.reserve(MaxChannel);
	  cfd_value.reserve(MaxChannel);
	  led_value.reserve(MaxChannel);
	  charge_raw.reserve(MaxChannel);
	  charge_cal.reserve(MaxChannel);
	  timestamp_low.reserve(MaxChannel);
	  timestamp_high.reserve(MaxChannel);
	  timestamp_live.reserve(MaxChannel);
	  timestamp_tr.reserve(MaxChannel); 
	  timestamp_ta.reserve(MaxChannel);  
	  waveform.reserve(MaxChannel);
}

TTigEventFragment::~TTigEventFragment(){
  Clear();
}

void   TTigEventFragment::Clear(){
    tig_num_chan=0;
	  tig_event_id=0;
	  tig_midas_id.clear();
	  tig_type.clear();
	  channel_number.clear();
    channel_raw.clear();
	  cfd_value.clear();
	  led_value.clear();
	  charge_raw.clear();
	  charge_cal.clear();
	  timestamp_low.clear();
	  timestamp_high.clear();
	  timestamp_live.clear();
	  timestamp_tr.clear(); 
	  timestamp_ta.clear();  
	  waveform.clear();
}

