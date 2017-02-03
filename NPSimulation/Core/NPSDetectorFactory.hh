#ifndef _DETECTORFACTORY_
#define _DETECTORFACTORY_
#include"NPSVDetector.hh"
#include<map>
#include<string>

// Geant4
#include"G4ThreeVector.hh"
// Root
#include"TVector3.h"

typedef NPS::VDetector* (*ClassDetectorFactoryFn)();

namespace NPS{
  // Convert fron root to g4 standard
  G4ThreeVector ConvertVector(TVector3 vec);

class DetectorFactory{
  private:
      DetectorFactory();
      ~DetectorFactory();

  public:
      static DetectorFactory* getInstance();
      NPS::VDetector* Construct(std::string Token);
      void ReadClassList(std::string FileList);
      void AddToken(std::string Token, std::string LibName);
      void AddTokenAlternative(std::string mainToken, std::string Alternative);
      void AddDetector(std::string Token, ClassDetectorFactoryFn fn);
      void CreateClassList(string FileName);
  private:
      static DetectorFactory* m_Instance;
      // Map that Match the Token with Constructor, once the lib is loaded
      std::map<std::string,ClassDetectorFactoryFn> m_Construct;
      // Map that Match the Token with the Lib to be loaded
      std::map<std::string,std::string> m_TokenLib;
      // Map of alternative token
      std::map<std::string,std::string> m_TokenAlternative;
};
}
#endif
