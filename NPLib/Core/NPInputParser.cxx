#include"NPInputParser.h"
using namespace NPL;
#include"NPSystemOfUnits.h"
#include"NPOptionManager.h"

#include<sstream>
#include<iostream>
#include<fstream>
#include<algorithm>

////////////////////////////////////////////////////////////////////////////////
std::string NPL::StripSpaces(std::string line){
  // Remove preceding spaces
  while(line.front()==' ')
    line = line.substr(1,line.length());

  // Remove trailing spaces
  if(line.length()>0)
    while(line.back()==' ')
      line = line.substr(0,line.length()-1);
  return line;

}
////////////////////////////////////////////////////////////////////////////////
unsigned int NPL::GetLevel(std::string line){
  unsigned int level = 0 ;
  if(line.length()==0)
    return 1000;

  for(unsigned int i = 0 ; i < line.length() ; i++){
    if(line.compare(i,1," ")==0)
      level++;
    else 
      break;
  }

  return level; 
}

////////////////////////////////////////////////////////////////////////////////
std::string NPL::ToLower(std::string line){
  std::transform(line.begin(), line.end(), line.begin(), ::tolower);
  return line;
}

////////////////////////////////////////////////////////////////////////////////
InputBlock::InputBlock(std::string line){
  m_Level     = GetLevel(line);
  m_MainToken = StripSpaces(ExtractToken(line," "));     
  m_MainValue = StripSpaces(ExtractValue(line," "));
}

////////////////////////////////////////////////////////////////////////////////
std::string InputBlock::ExtractToken(std::string line,std::string separator){
  // Find the separator
  if(separator=="")
    separator = NPL::token_separator;
  std::size_t pos = line.find(separator.c_str());
  return line.substr (0,pos);
}

////////////////////////////////////////////////////////////////////////////////
std::string InputBlock::ExtractValue(std::string line,std::string separator){
  // Find the separator
  if(separator=="")
    separator = NPL::token_separator;
  std::size_t pos = line.find(separator.c_str());
  if(pos!=std::string::npos)
    return line.substr (pos+1);
  else
    return "";
}

////////////////////////////////////////////////////////////////////////////////
void InputBlock::AddLine(std::string line){
  m_Token.push_back(ToLower(StripSpaces(ExtractToken(line))));
  m_Value.push_back(StripSpaces(ExtractValue(line)));
}
////////////////////////////////////////////////////////////////////////////////
void InputBlock::Dump(){
  std::cout << "//////////// Block ////////////" << std::endl;
  std::cout << " * Main Token: " << m_MainToken << std::endl ;
  std::cout << " * Main Value: "  << m_MainValue << std::endl;
  std::cout << " * Number of Line: " << GetSize() << std::endl;
  for(unsigned int i = 0 ; i < GetSize() ; i++)
    std::cout << "  - " << i+1 << " " << m_Token[i] << ": " << m_Value[i] << std::endl; 
}
////////////////////////////////////////////////////////////////////////////////
bool InputBlock::HasTokenList(std::vector<std::string> Token){
  if(m_Token.size()==0)
    return false;

  bool res = true;
  for(unsigned int i = 0 ; i < Token.size() ; i++)
    res = res && HasToken(Token[i]);

  return res;
}

////////////////////////////////////////////////////////////////////////////////
bool InputBlock::HasToken(std::string Token){
  for(unsigned int i = 0 ; i < m_Token.size() ; i++){
    if(m_Token[i] == ToLower(Token)){
      return true;
    }
  }
  return false;
}
////////////////////////////////////////////////////////////////////////////////
std::string InputBlock::GetValue(std::string Token){
  for(unsigned int i = 0 ; i < m_Token.size() ; i++){
    if(m_Token[i]==ToLower(Token))
      return m_Value[i];
  }
  return "";

}
////////////////////////////////////////////////////////////////////////////////
double InputBlock::GetDouble(std::string Token,std::string default_unit){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();
  std::stringstream iss(GetValue(Token));
  double val;
  std::string unit;
  iss >> val >> unit ;
  if(unit==""){
    if(default_unit!="void")
      std::cout <<"WARNING: Using default units for token " << Token << std::endl;
    val = ApplyUnit(val,default_unit); 
  }
  else{
    val = ApplyUnit(val,unit);
  }
  
  if(verbose)
    cout << " " << Token << " (" <<default_unit << "): " << val/ApplyUnit(1,default_unit) << endl; 

  return val;        
}
////////////////////////////////////////////////////////////////////////////////
int InputBlock::GetInt(std::string Token){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();
  std::stringstream iss(GetValue(Token));
  int val;
  iss >> val ;

  if(verbose)
    cout << " " << Token << ": " << val << endl; 


  return val;        
}
////////////////////////////////////////////////////////////////////////////////
std::string InputBlock::GetString(std::string Token){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();
  if(verbose)
    cout << " " << Token << ": " << GetValue(Token) << endl; 

  return GetValue(Token);
}
////////////////////////////////////////////////////////////////////////////////
TVector3 InputBlock::GetTVector3(std::string Token,std::string  default_unit){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();
  std::stringstream iss(GetValue(Token));

  double x,y,z;
  std::string unit;
  iss >> x >> y >> z >> unit ;
  if(unit==""){
    if(default_unit!="void")
      std::cout <<"WARNING: Using default units for token " << Token << std::endl;
    x = ApplyUnit(x,default_unit); 
    y = ApplyUnit(y,default_unit); 
    z = ApplyUnit(z,default_unit); 
  }
  else{
    x = ApplyUnit(x,unit);
    y = ApplyUnit(y,unit);
    z = ApplyUnit(z,unit);
  }

  if(verbose)
    cout << " " << Token << " (" <<default_unit << "): (" 
    << x/ApplyUnit(1,default_unit) << " ; " 
    << y/ApplyUnit(1,default_unit) << " ; " 
    << z/ApplyUnit(1,default_unit) << ")" << endl; 


  return TVector3(x,y,z);        
}
////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> InputBlock::GetVectorString(std::string Token){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();

  std::stringstream iss(GetValue(Token));

  std::vector<std::string> val;
  std::string buffer;
    while(iss>>buffer)
      val.push_back(buffer);

  
  if(verbose){
    cout << " " << Token << ": ";
    for(unsigned int i = 0 ; i < val.size() ; i++)
      cout << val[i] << " " ;
    cout << endl; 
  }
  return val;        
}

////////////////////////////////////////////////////////////////////////////////
std::vector<double> InputBlock::GetVectorDouble(std::string Token,std::string  default_unit){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();

  std::stringstream iss(GetValue(Token));

  std::vector<double> val;
  double buffer;
    while(iss>>buffer)
      val.push_back(buffer);

  // Try to read the unit  
  iss.clear();
  std::string unit;
  iss>>unit;
 
  if(unit==""){
    if(default_unit!="void")
      std::cout <<"WARNING: Using default units for token " << Token << std::endl;
    for(unsigned int i = 0 ; i < val.size() ; i++)
      val[i]=ApplyUnit(val[i],default_unit);
  }
  else{
    for(unsigned int i = 0 ; i < val.size() ; i++)
      val[i]=ApplyUnit(val[i],unit);
  }

  if(verbose){
    cout << " " << Token << " (" << default_unit << "): ";
    for(unsigned int i = 0 ; i < val.size() ; i++)
      cout << val[i]/ApplyUnit(1,default_unit) << " " ;
    cout << endl; 
  }

return val;        
}

////////////////////////////////////////////////////////////////////////////////
std::vector<int> InputBlock::GetVectorInt(std::string Token){
  int verbose = NPOptionManager::getInstance()->GetVerboseLevel();

  std::stringstream iss(GetValue(Token));
  std::vector<int> val;
  int buffer;
    while(iss>>buffer)
      val.push_back(buffer);

 if(verbose){
    cout << " " << Token << ": ";
    for(unsigned int i = 0 ; i < val.size() ; i++)
      cout << val[i] << " " ;
    cout << endl; 
  }


  return val;        
}

////////////////////////////////////////////////////////////////////////////////
std::vector<NPL::InputBlock*> InputBlock::GetSubBlock(std::string Token){
  std::vector<NPL::InputBlock*> blocks;
  for(unsigned int i = 0 ; i < m_SubBlock.size() ; i++){
    if(Token == m_SubBlock[i]->GetMainToken())
      blocks.push_back(m_SubBlock[i]);
  }
  return blocks;        
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void InputParser::Dump(){
  for(unsigned int i = 0 ; i < m_Block.size() ; i++)
    m_Block[i]->Dump();
}
////////////////////////////////////////////////////////////////////////////////
bool InputParser::IsNotComment(std::string line){
  line = StripSpaces(line);
  if(line.length()==0)
    return false;

  std::size_t pos = line.find("%");
  if(pos==std::string::npos)
    return true;
  else
    return false;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<InputBlock*> InputParser::GetAllBlocksWithToken(std::string Token){
  std::vector<InputBlock*> res;
  for(unsigned int i = 0 ; i < m_Block.size(); i++){
    if(m_Block[i]->GetMainToken() == Token){
      res.push_back(m_Block[i]);
    }
  }

  return res;

}
////////////////////////////////////////////////////////////////////////////////
std::vector<InputBlock*> InputParser::GetAllBlocksWithTokenAndValue(std::string Token,std::string Value){
  std::vector<InputBlock*> res;
  for(unsigned int i = 0 ; i < m_Block.size(); i++){
    if(m_Block[i]->GetMainToken() == Token){
      if(m_Block[i]->GetMainValue() == Value)
        res.push_back(m_Block[i]);
    }
  }

  return res;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> InputParser::GetAllBlocksToken(){
  std::vector<std::string> token;
  for(unsigned int i = 0 ; i < m_Block.size() ; i++)
      token.push_back(m_Block[i]->GetMainToken());

  return token;
}

////////////////////////////////////////////////////////////////////////////////
void InputParser::ReadFile(std::string filename){
  std::ifstream file(filename.c_str());
  if(!file.is_open()){
    std::cout <<"ERROR : file " << filename << " not found " << std::endl;
    exit(1);
  }

  std::string line;
  InputBlock* block = 0;
  while(!file.eof()){
    getline(file,line);
    if(IsNotComment(line) && GetLevel(line)==0) {
      if(block)
        m_Block.push_back(block);

      block = new InputBlock(line);
      if(!file.eof())
        getline(file,line);
    }
    if(block && IsNotComment(line)){
      block->AddLine(line);
    }
  }
  // Add the last block
  if(block)
    m_Block.push_back(block);
}
////////////////////////////////////////////////////////////////////////////////
double NPL::ApplyUnit(double value, std::string unit){
  if(unit=="void") // apply no unit
    return value;

  else if(unit=="millimeter"){
    return value*NPUNITS::millimeter;
  }

  else if(unit=="millimeter2"){
    return value*NPUNITS::millimeter2;
  }

  else if(unit=="millimeter3"){
    return value*NPUNITS::millimeter3;
  }

  else if(unit=="centimeter"){
    return value*NPUNITS::centimeter;
  }

  else if(unit=="centimeter2"){
    return value*NPUNITS::centimeter2;
  }

  else if(unit=="centimeter3"){
    return value*NPUNITS::centimeter3;
  }

  else if(unit=="meter"){
    return value*NPUNITS::meter;
  }

  else if(unit=="meter2"){
    return value*NPUNITS::meter2;
  }

  else if(unit=="meter3"){
    return value*NPUNITS::meter3;
  }

  else if(unit=="kilometer"){
    return value*NPUNITS::kilometer;
  }

  else if(unit=="kilometer2"){
    return value*NPUNITS::kilometer2;
  }

  else if(unit=="kilometer3"){
    return value*NPUNITS::kilometer3;
  }

  else if(unit=="parsec"){
    return value*NPUNITS::parsec;
  }

  else if(unit=="micrometer"){
    return value*NPUNITS::micrometer;
  }

  else if(unit=="nanometer"){
    return value*NPUNITS::nanometer;
  }

  else if(unit=="angstrom"){
    return value*NPUNITS::angstrom;
  }

  else if(unit=="fermi"){
    return value*NPUNITS::fermi;
  }

  else if(unit=="barn"){
    return value*NPUNITS::barn;
  }

  else if(unit=="millibarn"){
    return value*NPUNITS::millibarn;
  }

  else if(unit=="microbarn"){
    return value*NPUNITS::microbarn;
  }

  else if(unit=="nanobarn"){
    return value*NPUNITS::nanobarn;
  }

  else if(unit=="picobarn"){
    return value*NPUNITS::picobarn;
  }

  else if(unit=="mm"){
    return value*NPUNITS::mm;
  }

  else if(unit=="um"){
    return value*NPUNITS::um;
  }

  else if(unit=="nm"){
    return value*NPUNITS::nm;
  }

  else if(unit=="mm2"){
    return value*NPUNITS::mm2;
  }

  else if(unit=="mm3"){
    return value*NPUNITS::mm3;
  }

  else if(unit=="cm"){
    return value*NPUNITS::cm;
  }

  else if(unit=="cm2"){
    return value*NPUNITS::cm2;
  }

  else if(unit=="cm3"){
    return value*NPUNITS::cm3;
  }

  else if(unit=="m"){
    return value*NPUNITS::m;
  }

  else if(unit=="m2"){
    return value*NPUNITS::m2;
  }

  else if(unit=="m3"){
    return value*NPUNITS::m3;
  }

  else if(unit=="km"){
    return value*NPUNITS::km;
  }

  else if(unit=="km2"){
    return value*NPUNITS::km2;
  }

  else if(unit=="km3"){
    return value*NPUNITS::km3;
  }

  else if(unit=="pc"){
    return value*NPUNITS::pc;
  }

  else if(unit=="radian"){
    return value*NPUNITS::radian;
  }

  else if(unit=="milliradian"){
    return value*NPUNITS::milliradian;
  }

  else if(unit=="degree"){
    return value*NPUNITS::degree;
  }

  else if(unit=="steradian"){
    return value*NPUNITS::steradian;
  }

  else if(unit=="rad"){
    return value*NPUNITS::rad;
  }

  else if(unit=="mrad"){
    return value*NPUNITS::mrad;
  }

  else if(unit=="sr"){
    return value*NPUNITS::sr;
  }

  else if(unit=="deg"){
    return value*NPUNITS::deg;
  }

  else if(unit=="nanosecond"){
    return value*NPUNITS::nanosecond;
  }

  else if(unit=="second"){
    return value*NPUNITS::second;
  }

  else if(unit=="millisecond"){
    return value*NPUNITS::millisecond;
  }

  else if(unit=="microsecond"){
    return value*NPUNITS::microsecond;
  }

  else if(unit=="picosecond"){
    return value*NPUNITS::picosecond;
  }

  else if(unit=="hertz"){
    return value*NPUNITS::hertz;
  }

  else if(unit=="kilohertz"){
    return value*NPUNITS::kilohertz;
  }

  else if(unit=="megahertz"){
    return value*NPUNITS::megahertz;
  }

  else if(unit=="ns"){
    return value*NPUNITS::ns;
  }

  else if(unit=="s"){
    return value*NPUNITS::s;
  }

  else if(unit=="ms"){
    return value*NPUNITS::ms;
  }

  else if(unit=="eplus"){
    return value*NPUNITS::eplus;
  }

  else if(unit=="e_SI"){
    return value*NPUNITS::e_SI;
  }

  else if(unit=="coulomb"){
    return value*NPUNITS::coulomb;
  }

  else if(unit=="megaelectron"){
    return value*NPUNITS::megaelectronvolt;
  }

  else if(unit=="electronvolt"){
    return value*NPUNITS::electronvolt;
  }

  else if(unit=="kiloelectron"){
    return value*NPUNITS::kiloelectronvolt;
  }

  else if(unit=="gigaelectron"){
    return value*NPUNITS::gigaelectronvolt;
  }

  else if(unit=="teraelectron"){
    return value*NPUNITS::teraelectronvolt;
  }

  else if(unit=="petaelectron"){
    return value*NPUNITS::petaelectronvolt;
  }

  else if(unit=="joule"){
    return value*NPUNITS::joule;
  }

  else if(unit=="MeV"){
    return value*NPUNITS::MeV;
  }

  else if(unit=="eV"){
    return value*NPUNITS::eV;
  }

  else if(unit=="keV"){
    return value*NPUNITS::keV;
  }

  else if(unit=="GeV"){
    return value*NPUNITS::GeV;
  }

  else if(unit=="TeV"){
    return value*NPUNITS::TeV;
  }

  else if(unit=="PeV"){
    return value*NPUNITS::PeV;
  }

  else if(unit=="kilogram"){
    return value*NPUNITS::kilogram;
  }

  else if(unit=="gram"){
    return value*NPUNITS::gram;
  }

  else if(unit=="milligram"){
    return value*NPUNITS::milligram;
  }

  else if(unit=="kg"){
    return value*NPUNITS::kg;
  }

  else if(unit=="g"){
    return value*NPUNITS::g;
  }

  else if(unit=="mg"){
    return value*NPUNITS::mg;
  }

  else if(unit=="watt"){
    return value*NPUNITS::watt;
  }

  else if(unit=="newton"){
    return value*NPUNITS::newton;
  }

  else if(unit=="hep_pascal"){
    return value*NPUNITS::hep_pascal;
  }

  else if(unit=="bar"){
    return value*NPUNITS::bar;
  }

  else if(unit=="atmosphere"){
    return value*NPUNITS::atmosphere;
  }

  else if(unit=="ampere"){
    return value*NPUNITS::ampere;
  }

  else if(unit=="milliampere"){
    return value*NPUNITS::milliampere;
  }

  else if(unit=="microampere"){
    return value*NPUNITS::microampere;
  }

  else if(unit=="nanoampere"){
    return value*NPUNITS::nanoampere;
  }

  else if(unit=="megavolt"){
    return value*NPUNITS::megavolt;
  }

  else if(unit=="kilovolt"){
    return value*NPUNITS::kilovolt;
  }

  else if(unit=="volt"){
    return value*NPUNITS::volt;
  }

  else if(unit=="ohm"){
    return value*NPUNITS::ohm;
  }

  else if(unit=="farad"){
    return value*NPUNITS::farad;
  }

  else if(unit=="millifarad"){
    return value*NPUNITS::millifarad;
  }

  else if(unit=="microfarad"){
    return value*NPUNITS::microfarad;
  }

  else if(unit=="nanofarad"){
    return value*NPUNITS::nanofarad;
  }

  else if(unit=="picofarad"){
    return value*NPUNITS::picofarad;
  }

  else if(unit=="weber"){
    return value*NPUNITS::weber;
  }

  else if(unit=="tesla" || unit=="T"){
    return value*NPUNITS::tesla;
  }

  else if(unit=="gauss"){
    return value*NPUNITS::gauss;
  }

  else if(unit=="kilogauss"){
    return value*NPUNITS::kilogauss;
  }

  else if(unit=="henry"){
    return value*NPUNITS::henry;
  }

  else if(unit=="kelvin"){
    return value*NPUNITS::kelvin;
  }

  else if(unit=="mole"){
    return value*NPUNITS::mole;
  }

  else if(unit=="becquerel"){
    return value*NPUNITS::becquerel;
  }

  else if(unit=="curie"){
    return value*NPUNITS::curie;
  }

  else if(unit=="gray"){
    return value*NPUNITS::gray;
  }

  else if(unit=="kilogray"){
    return value*NPUNITS::kilogray;
  }

  else if(unit=="milligray"){
    return value*NPUNITS::milligray;
  }

  else if(unit=="microgray"){
    return value*NPUNITS::microgray;
  }

  else if(unit=="candela"){
    return value*NPUNITS::candela;
  }

  else if(unit=="lumen"){
    return value*NPUNITS::lumen;
  }

  else if(unit=="lux"){
    return value*NPUNITS::lux;
  }

  else if(unit=="perCent"){
    return value*NPUNITS::perCent;
  }

  else if(unit=="perThousand"){
    return value*NPUNITS::perThousand;
  }

  else if(unit=="perMillion"){
    return value*NPUNITS::perMillion;
  }

  else{
    std::cout << "WARNING: Unknown unit " << unit << std::endl;
    return value;
  }
}
