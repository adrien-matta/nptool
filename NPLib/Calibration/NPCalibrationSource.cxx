// NPL
#include "NPCalibrationSource.h"

// Root
#include"TDirectory.h"

////////////////////////////////////////////////////////////////////////////////
NPL::CalibrationSource::CalibrationSource(){
m_SourceSignal = 0 ;
}

////////////////////////////////////////////////////////////////////////////////
NPL::CalibrationSource::~CalibrationSource(){
}

////////////////////////////////////////////////////////////////////////////////
void NPL::CalibrationSource::ComputeSourceSignal(){
  TString contribution;
  unsigned int arg_number = 0;

  for(unsigned int i = 0 ; i < m_Energies.size() ; i++){
    for(unsigned int j = 0 ; j < m_Energies[i].size() ; j++){
      contribution +=
      Form("(%f)*[%i]*exp(-(x-[%i]*%f)*(x-[%i]*%f)/(2*[%i]*[%i]))",
            m_BranchingRatio[i][j]/m_BranchingRatio[i][0], arg_number,
            arg_number+1, m_Energies[i][j]/m_Energies[i][0],
            arg_number+1, m_Energies[i][j]/m_Energies[i][0],
            arg_number+2,arg_number+2);
    
      if(j!=m_Energies[i].size()-1) contribution+="+";
    }

    arg_number+=3;
      if(i!=m_Energies.size()-1) contribution+="+";
  }

  m_SourceSignal = new TF1("np_source_signal",contribution,0,1e9);
  m_SourceSignal->SetNpx(20000);
  arg_number = 0;
  for(unsigned int i = 0 ; i < m_Energies.size() ; i++){
    m_SourceSignal->SetParameter(arg_number++,1000);
    m_SourceSignal->SetParameter(arg_number++,5000+i*1000);
    m_SourceSignal->SetParameter(arg_number++,20);
  }

}

////////////////////////////////////////////////////////////////////////////////
TF1* NPL::CalibrationSource::GetSourceSignal(){
  if(!m_SourceSignal)
    ComputeSourceSignal();

  return m_SourceSignal;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::CalibrationSource::AddParticleContribution(vector<double> Energies ,  vector<double> ErrEnergies, vector<double> BranchingRatio){
  m_Energies.push_back(Energies);
  m_ErrEnergies.push_back(ErrEnergies);
  m_BranchingRatio.push_back(BranchingRatio);
}

////////////////////////////////////////////////////////////////////////////////
void NPL::CalibrationSource::Set_ThreeAlphaSource(){
 Set_239Pu();
 Set_241Am();
 Set_244Cm();
}

////////////////////////////////////////////////////////////////////////////////
void NPL::CalibrationSource::Set_239Pu(){
  vector<double> Energies;
  vector<double> ErrEnergies;
  vector<double> BranchingRatio;
  
  Energies.push_back(5.15659);
  Energies.push_back(5.11443);
  Energies.push_back(5.1055);

  ErrEnergies.push_back(1.4e-4);
  ErrEnergies.push_back(8e-5);
  ErrEnergies.push_back(8e-4);
  
  BranchingRatio.push_back(70.77);
  BranchingRatio.push_back(17.11);
  BranchingRatio.push_back(11.94);

  AddParticleContribution(Energies,ErrEnergies,BranchingRatio);

}

////////////////////////////////////////////////////////////////////////////////
void NPL::CalibrationSource::Set_241Am(){
  vector<double> Energies;
  vector<double> ErrEnergies;
  vector<double> BranchingRatio;
  
  Energies.push_back(5.48556);
  Energies.push_back(5.44280);
  Energies.push_back(5.388);

  ErrEnergies.push_back(1.2e-4);
  ErrEnergies.push_back(1.3e-4);
  ErrEnergies.push_back(1e-3); // to be checked
  
  BranchingRatio.push_back(84.8);
  BranchingRatio.push_back(13.1);
  BranchingRatio.push_back(1.66);

  AddParticleContribution(Energies,ErrEnergies,BranchingRatio);
}

////////////////////////////////////////////////////////////////////////////////
void NPL::CalibrationSource::Set_244Cm(){
  vector<double> Energies;
  vector<double> ErrEnergies;
  vector<double> BranchingRatio;
  
  Energies.push_back(5.80477);
  Energies.push_back(5.76264);

  ErrEnergies.push_back(5e-5);
  ErrEnergies.push_back(3e-5);
  
  BranchingRatio.push_back(76.40);
  BranchingRatio.push_back(23.60);

  AddParticleContribution(Energies,ErrEnergies,BranchingRatio);
}


