#ifndef EventGeneratorGammaDecay_H
#define EventGeneratorGammaDecay_H
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : May 2012                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated a gamma decay of nuclei genera-*
 * ted by previous event generator. Multiple cases are supported:            *
 *  - Only one gamma is emmited, in this case a Cross section can be given   *
 *  - A cascade decay, in this case the CS is ignore                         *
 *  - If more than one cascade are given, Branching Ratio could be given     *
 *                                                                           *
 *  Using the "Source Token" the class can be used to simulate gamma ray sou-*
 * rce and Doppler shifted gamma rays.                                       *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include <string>
#include <iostream>
using namespace std;

// NPSimulation
#include "VEventGenerator.hh"
#include "Target.hh"
#include "ParticleStack.hh"

class EventGeneratorGammaDecay : public VEventGenerator
{
    public: // Constructor and destructor
        EventGeneratorGammaDecay();
        ~EventGeneratorGammaDecay();
    
    public: // Inherit from VEventGenerator class
        void ReadConfiguration(string,int);
        void GenerateEvent(G4Event*);
        void SetTarget(Target* Target) ;
    
    private: // Target Parameter
        Target* m_Target;


    private: // The decaying nuclei
        string m_NucleiName;
    private: // the gamma rays property 
        vector<double>  m_BranchingRatio;
        vector<string>  m_CrossSectionPath;
        vector<double*> m_CrossSectionArray;
        vector<double>  m_CrossSectionThetaMin;
        vector<double>  m_CrossSectionThetaMax;
        vector<double>  m_CrossSectionSize;
        vector<double>  m_CascadeTotalEnergy;
        vector< vector<double> > m_Energies;
        
    private: // Pointer to the Particle stack for faster acces
        ParticleStack* m_ParticleStack;
    public: // Managing the different cascade
        // Add a new cascade
        void AddCascade(vector<double> Energies, double BranchingRatio=-1, string CrossSectionPath="_void_");
        // Read all the added cscade en instentiate every thing that is needed
        void PrepareCascade();
};
#endif