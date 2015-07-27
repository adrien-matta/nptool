// STL
#include <vector>
using namespace std;

// Root
#include "TH1.h"
#include "TGraphErrors.h"
#include "TGraph.h"
// NPL
#include "NPEnergyLoss.h"
#include "NPCalibrationSource.h"

namespace NPL{
  class SiliconCalibrator{

    public:
      SiliconCalibrator();
      ~SiliconCalibrator();

    private:
      NPL::EnergyLoss* m_EL_Al;
      NPL::CalibrationSource* m_CalibrationSource;
      TF1* m_FitFunction;

    public:
      // Use the Zero Extrapolation method to perform fit and return the dead layer thickness
      double ZeroExtrapolation(TH1* histo, NPL::CalibrationSource* CS, NPL::EnergyLoss* EL, vector<double>& coeff, unsigned int pedestal, unsigned int max_iteration = 10000 , double rmin=-1,double rmax=-1);

      double SimpleCalibration(TH1* histo, NPL::CalibrationSource* CS, NPL::EnergyLoss* EL, vector<double>& coeff, double rmin=-1,double rmax=-1);

      // Return distance to pedestal. Use energies in Energies to perform fit and fill coeff with the results
      double FitPoints(TGraphErrors* Graph, double* Energies , double* ErrEnergies, vector<double>& coeff , double pedestal = 0 );
      // 
      TGraphErrors* FitSpectrum(TH1* histo);
  };
}
