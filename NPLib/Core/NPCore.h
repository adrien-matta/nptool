#ifndef NPCORE_H
#define NPCORE_H
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : February 2017                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Function to issue Warning, Information and Error                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include<string>
namespace NPL{
  std::string itoa(const int&); 
  void SendWarning(std::string Class, std::string Warning);
  void SendInformation(std::string Class, std::string Information);
  void SendErrorAndExit(std::string Class,std::string Error);
  // For use to simulate electronic
  unsigned int EnergyToADC(const double& E, const double& Emin, const double& Emax, const int& Pedestal, const int& ADCMax);
}
#endif
