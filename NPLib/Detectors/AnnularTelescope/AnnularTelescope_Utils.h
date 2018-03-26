/// \file Define some utility functions and structs for the annular telescope
/// detector
#ifndef ANNULAR_TELESCOPE_UTILS_HEADER
#define ANNULAR_TELESCOPE_UTILS_HEADER

// STL
#include <vector>
// NPL
namespace NPL { class InputParser; }

namespace AnnularTelescope_Utils {

/// Sescribes the geometry of an annular telescope
struct Geometry{
	double R_min; /// inner radius
	double R_max; /// outer radius
	double CsI_Wedge_Angle_Pitch;  /// ANGULAR pitch of CsI wedges
	double Si_Theta_Radius_Pitch;  /// RADIAL pitch of Si theta strips
	double Si_Phi_Angle_Pitch;     /// ANGULAR pitch of Si Phi strips
	std::vector<double> CsI_Wedge_Phi_Angle;   /// CsI wedge phi central angles
	std::vector<double> Si_Strip_Theta_Radius; /// Si Theta strips RADIUS (central)
	std::vector<double> Si_Strip_Phi_Angle;    /// Si Phi strips ANGLE (central)
	double Z;
};

/// Read detector geometry from a config file
std::vector<Geometry> ReadConfiguration(NPL::InputParser& parser);

}

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
