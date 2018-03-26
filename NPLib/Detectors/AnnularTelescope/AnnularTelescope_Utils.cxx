// STL
#include <vector>
#include <string>
using namespace std;

// NPL
#include "NPOptionManager.h"
#include "NPInputParser.h"

#include "AnnularTelescope_Utils.h"

std::vector<AnnularTelescope_Utils::Geometry>
AnnularTelescope_Utils::ReadConfiguration(NPL::InputParser& parser){
	vector<Geometry> out;
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularTelescope");
  if(NPOptionManager::getInstance()->GetVerboseLevel()){
    cout << "//// " << blocks.size() << " detectors found " << endl; 
	}
  vector<string> wedge = {"R_MIN", "R_MAX", "CSI_WEDGES", "SI_THETA_STRIPS", "ST_PHI_STRIPS", "Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(wedge)){
      if(NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  AnnularTelescope " << i+1 <<  endl;
			}
			Geometry g;
			g.R_min   = blocks[i]->GetDouble("R_MIN",   "mm");
			g.R_max   = blocks[i]->GetDouble("R_MAX",   "mm");

			int n_wedges = blocks[i]->GetInt("CSI_WEDGES");
			g.CsI_Wedge_Angle_Pitch = 360. / n_wedges;
			for(int i=0; i< n_wedges; ++i){
				g.CsI_Wedge_Phi_Angle.push_back(
					i*g.CsI_Wedge_Angle_Pitch + g.CsI_Wedge_Angle_Pitch/2. );
			}

			int n_theta = blocks[i]->GetInt("SI_THETA_STRIPS");
			g.Si_Theta_Radius_Pitch = (g.R_max - g.R_min) / n_theta;
			for(int i=0; i< n_theta; ++i){
				g.Si_Strip_Theta_Radius.push_back(
					i*g.Si_Theta_Radius_Pitch + g.Si_Theta_Radius_Pitch/2. );
			}

			int n_phi = blocks[i]->GetInt("SI_PHI_STRIPS");
			g.Si_Phi_Angle_Pitch = 360. / n_phi;
			for(int i=0; i< n_phi; ++i){
				g.Si_Strip_Phi_Angle.push_back(
					i*g.Si_Phi_Angle_Pitch + g.Si_Phi_Angle_Pitch/2. );
			}
				
      g.Z = blocks[i]->GetDouble("Z", "mm");
			out.push_back(g);
		}
    else{
      cout << "ERROR (AnnularTelescope): "
					 << "check your input file formatting " << endl;
      exit(1);
    }
  }
	return out;
}
