#ifndef EventGeneratorTransfert_H
#define EventGeneratorTransfert_H
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : January 2011                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated two body TransfertReaction.    *
 *  A Relativistic computation is performed to determine angle and energy of *
 *   the different particle, knowing the ThetaCM angle given by a cross      *
 *   section shoot. Eleastic scattering can also be simulated.               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                      *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>

// NPSimulation
#include "VEventGenerator.hh"
#include "Target.hh"

// NPLib header
#include "NPReaction.h"

using namespace std;
using namespace NPL ;


class EventGeneratorTransfert : public VEventGenerator
{
   public:     // Constructors and Destructors
      // Default constructor used to allocate memory
      EventGeneratorTransfert();

      // This is the constructor to be used
      EventGeneratorTransfert(string name1                  ,           // Beam nuclei
            string name2                  ,        // Target nuclei
            string name3                  ,        // Product of reaction
            string name4                  ,        // Product of reaction
            double BeamEnergy             ,        // Beam Energy
            double ExcitationEnergyLight  ,        // Excitation of Light Nuclei
            double ExcitationEnergyHeavy  ,        // Excitation of Heavy Nuclei
            double BeamEnergySpread       ,
            double SigmaX                 ,
            double SigmaY                 ,
            double SigmaThetaX            ,
            double SigmaPhiY              ,
            bool   ShootLight             ,
            bool   ShootHeavy             ,
            string Path,
            double CSThetaMin,
            double CSThetaMax);                          // Path of the differentiel Cross Section

      // Default Destructor
      virtual ~EventGeneratorTransfert();


   public: // Inherit from VEventGenerator class
      void ReadConfiguration(string);
      void GenerateEvent(G4Event*);
      void SetTarget(Target* Target) ;


   private: // Particle Shoot Option
      bool m_ShootLight;
      bool m_ShootHeavy;


   private: // Target Parameter
      Target* m_Target;


   private: // Reaction and CrossSection Shoot
      Reaction* m_Reaction;
      G4double  m_HalfOpenAngleMin;  // Min Half open angle of the source
      G4double  m_HalfOpenAngleMax;  // Max Half open angle of the source


   private: // Beam Parameters
      double m_BeamEnergy;
      double m_BeamEnergySpread;
      double m_SigmaX;
      double m_SigmaY;
      double m_SigmaThetaX;
      double m_SigmaPhiY;

      // Other methods
      void Print() const;
      void InitializeRootOutput();

      void SetEverything(string name1                     ,         // Beam nuclei
                         string name2                     ,        // Target nuclei
                         string name3                     ,        // Product of reaction
                         string name4                     ,        // Product of reaction
                         double BeamEnergy                ,        // Beam Energy
                         double ExcitationEnergyLight     ,        // Excitation of Light Nuclei
                         double ExcitationEnergyHeavy     ,        // Excitation of Heavy Nuclei
                         double BeamEnergySpread          ,
                         double SigmaX                    ,
                         double SigmaY                    ,
                         double SigmaThetaX               ,
                         double SigmaPhiY                 ,
                         bool   ShootLight                ,
                         bool   ShootHeavy                ,
                         string Path,
                         double CSThetaMin,
                         double CSThetaMax);                             // Path of the differentiel Cross Section
};

#endif
