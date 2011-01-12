/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 07/01/11                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Example of rootlogon.C file which calls the NPToolLogon.C     *
 *             located in NPLib/scripts and which loads all necessary NPTool *
 *             information needed by ROOT.                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
{
   // Load NPToolLogon.C macro dealing with all the NPLib shared libraries
   TString path = gSystem->Getenv("NPLIB");
   gROOT->ProcessLine(Form(".L %s/scripts/NPToolLogon.C+", path.Data()));
   
   // No Init Display
   // NPToolLogon(false)
   NPToolLogon();
   
   // With Init Display
   // NPToolLogon(true);
}
