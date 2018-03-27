// STL
#include <vector>
#include <string>
using namespace std;

// NPL
#include "NPSystemOfUnits.h"
#include "NPOptionManager.h"
#include "NPInputParser.h"
using namespace NPUNITS;

// ROOT
#include <TMath.h>

#include "AnnularTelescope_Utils.h"


namespace { void do_print(AnnularTelescope_Utils::Geometry& g) {
	std::cout
		<< "\tZ POSITION [mm]: " << g.Z / mm << "\n"
		<< "\tRADIUS [mm]: " << g.R_min / mm << " - " << g.R_max / mm << "\n"
		<< "\tCSI_THICKNESS [mm]: " << g.CsIThickness / mm << "\n"
		<< "\tCSI_WEDGES [deg]:\n";
	for(size_t i=0; i< g.CsI_Wedge_Phi_Angle.size(); ++i){
		cout << 
			"\t\t" << i << " : " << (g.CsI_Wedge_Phi_Angle[i] - g.CsI_Wedge_Angle_Pitch/2.) / deg
				 << " - " << (g.CsI_Wedge_Phi_Angle[i] + g.CsI_Wedge_Angle_Pitch/2.) / deg << "\n";
	}
	std::cout 
		<< "\tSI_THICKNESS [mm]: " << g.SiThickness / mm << "\n"
		<< "\tSI_PHI_STRIPS [deg]:\n";
	for(size_t i=0; i< g.Si_Strip_Phi_Angle.size(); ++i){
		cout << 
			"\t\t" << i << " : " << (g.Si_Strip_Phi_Angle[i] - g.Si_Phi_Angle_Pitch/2.)/deg
				 << " - " << (g.Si_Strip_Phi_Angle[i] + g.Si_Phi_Angle_Pitch/2.)/deg << "\n";
	}
	std::cout << "\tSI_THETA_STRIPS [mm]:\n";
	for(size_t i=0; i< g.Si_Strip_Theta_Radius.size(); ++i){
		cout << 
			"\t\t" << i << " : " << (g.Si_Strip_Theta_Radius[i] - g.Si_Theta_Radius_Pitch/2.)/mm
				 << " - " << (g.Si_Strip_Theta_Radius[i] + g.Si_Theta_Radius_Pitch/2.)/mm << "\n";
	}
	std::cout << "////////////////////////////////////////////////////////" << std::endl;
} }

std::vector<AnnularTelescope_Utils::Geometry>
AnnularTelescope_Utils::ReadConfiguration(NPL::InputParser& parser){
	vector<Geometry> out;
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularTelescope");
  if(NPOptionManager::getInstance()->GetVerboseLevel()){
    cout << "//// " << blocks.size() << " detectors found " << endl; 
	}
  vector<string> wedge = {
		"Z", "R_MIN", "R_MAX",
		"CSI_THICKNESS", "CSI_WEDGES",
		"SI_THICKNESS", "SI_THETA_STRIPS", "SI_PHI_STRIPS"
	};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(wedge)){
      if(NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  AnnularTelescope " << i+1 <<  endl;
			}
			Geometry g;
			g.R_min   = blocks[i]->GetDouble("R_MIN",   "mm");
			g.R_max   = blocks[i]->GetDouble("R_MAX",   "mm");

			int n_wedges = blocks[i]->GetInt("CSI_WEDGES");
			g.CsI_Wedge_Angle_Pitch = (2*TMath::Pi()) / n_wedges;
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
			g.Si_Phi_Angle_Pitch = (2*TMath::Pi()) / n_phi;
			for(int i=0; i< n_phi; ++i){
				g.Si_Strip_Phi_Angle.push_back(
					i*g.Si_Phi_Angle_Pitch + g.Si_Phi_Angle_Pitch/2. );
			}
				
      g.Z = blocks[i]->GetDouble("Z", "mm");
			g.SiThickness = blocks[i]->GetDouble("SI_THICKNESS", "um");
			g.CsIThickness = blocks[i]->GetDouble("CSI_THICKNESS", "mm");

			out.push_back(g);
      // if(NPOptionManager::getInstance()->GetVerboseLevel()){
			// 	do_print(g);
			// }
		}
    else{
      cout << "ERROR (AnnularTelescope): "
					 << "check your input file formatting." << endl
					 << "Here is a dump of the problem block: " << endl;
			blocks[i]->Dump();
      exit(1);
    }
  }
	return out;
}
