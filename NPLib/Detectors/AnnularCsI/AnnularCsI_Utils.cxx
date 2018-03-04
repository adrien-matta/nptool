// STL
#include <vector>
#include <string>
using namespace std;

// NPL
#include "NPOptionManager.h"
#include "NPInputParser.h"

#include "AnnularCsI_Utils.h"

std::vector<AnnularCsI_Utils::Geometry>
AnnularCsI_Utils::ReadConfiguration(NPL::InputParser& parser){
	vector<Geometry> out;
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularCsI");
  if(NPOptionManager::getInstance()->GetVerboseLevel()){
    cout << "//// " << blocks.size() << " detectors found " << endl; 
	}
  vector<string> wedge = {"R_MIN", "R_MAX", "PHI_MIN","PHI_MAX", "Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(wedge)){
      if(NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  AnnularCsI " << i+1 <<  endl;
			}
			Geometry g;
			g.R_min   = blocks[i]->GetDouble("R_MIN",   "mm");
			g.R_max   = blocks[i]->GetDouble("R_MAX",   "mm");
			g.Phi_min = blocks[i]->GetDouble("PHI_MIN", "deg");
			g.Phi_max = blocks[i]->GetDouble("PHI_MAX", "deg");	
      g.Z       = blocks[i]->GetDouble("Z",       "mm");
			g.R_delta = g.R_max - g.R_min;
			g.Phi_delta = g.Phi_max - g.Phi_min;
			out.push_back(g);
		}
    else{
      cout << "ERROR (AnnularCsI): "
					 << "check your input file formatting " << endl;
      exit(1);
    }
  }
	return out;
}
