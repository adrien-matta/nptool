/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 31/08/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the tracker part of the Gaspard detector.                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TGaspardTrackerPhysics.h"
#include <iostream>


ClassImp(TGaspardTrackerPhysics)


TGaspardTrackerPhysics::TGaspardTrackerPhysics() 
{
   EventMultiplicity = 0;
}



TGaspardTrackerPhysics::~TGaspardTrackerPhysics()
{
   Clear();
}



void TGaspardTrackerPhysics::BuildSimplePhysicalEvent(TGaspardTrackerData* Data)
{
   BuildPhysicalEvent(Data);
}



void TGaspardTrackerPhysics::BuildPhysicalEvent(TGaspardTrackerData* Data)
{ 
   // Check
   bool Check_FirstStage = false ;bool Check_SecondStage = false ; bool Check_ThirdStage = false ;

   // Thresholds
   double FirstStage_Front_E_Threshold = 0; double FirstStage_Front_T_Threshold = 0;
   double FirstStage_Back_E_Threshold  = 0; double FirstStage_Back_T_Threshold  = 0;
   double SecondStage_E_Threshold      = 0; double SecondStage_T_Threshold      = 0;
   double ThirdStage_E_Threshold       = 0; double ThirdStage_T_Threshold	= 0;

   // calculate multipicity in the first stage
   int multXE = Data->GetGPDTrkFirstStageFrontEMult();
   int multYE = Data->GetGPDTrkFirstStageBackEMult();
   int multXT = Data->GetGPDTrkFirstStageFrontTMult();
   int multYT = Data->GetGPDTrkFirstStageBackTMult();
   // calculate multiplicity of 2nd and third stages
   int mult2E = Data->GetGPDTrkSecondStageEMult();
   int mult2T = Data->GetGPDTrkSecondStageTMult();
   int mult3E = Data->GetGPDTrkThirdStageEMult();
   int mult3T = Data->GetGPDTrkThirdStageTMult();

   // Deal with multiplicity 1 for the first layer
   if (multXE==1 && multYE==1 && multXT==1 && multYT==1) {
      // calculate detector number
      int det_ref = Data->GetGPDTrkFirstStageFrontEDetectorNbr(0);
      int detecXE = Data->GetGPDTrkFirstStageFrontEDetectorNbr(0) / det_ref;
      int detecXT = Data->GetGPDTrkFirstStageFrontTDetectorNbr(0) / det_ref;
      int detecYE = Data->GetGPDTrkFirstStageBackEDetectorNbr(0) / det_ref;
      int detecYT = Data->GetGPDTrkFirstStageBackTDetectorNbr(0) / det_ref;

      // case of same detector
      if (detecXE*detecXT*detecYE*detecYT == 1) {
         // store module number
         ModuleNumber.push_back(det_ref);
         // calculate strip number
         int stripXE = Data->GetGPDTrkFirstStageFrontEStripNbr(0);
         int stripXT = Data->GetGPDTrkFirstStageFrontTStripNbr(0);
         int stripYE = Data->GetGPDTrkFirstStageBackEStripNbr(0);
         int stripYT = Data->GetGPDTrkFirstStageBackTStripNbr(0);

         // case of same strips on X and Y
         if (stripXE == stripXT  &&  stripYE == stripYT) {        // here we have a good strip event
            // various
            Check_FirstStage = true;
            EventMultiplicity = 1;
            // store strip ID
            FirstStage_X.push_back(stripXE);
            FirstStage_Y.push_back(stripYE);
            // get energy from strips and store it
            double EnergyStripFront = Data->GetGPDTrkFirstStageFrontEEnergy(0);
            double EnergyStripBack  = Data->GetGPDTrkFirstStageBackEEnergy(0);
            double EnergyStrip  = 0.5 * (EnergyStripFront + EnergyStripBack);
//                  double EnergyStrip  = EnergyStripFront;
//                  if (EnergyStripBack > EnergyStrip) EnergyStrip = EnergyStripBack;
            FirstStage_E.push_back(EnergyStrip);
            double EnergyTot = EnergyStrip;
            // get time from strips and store it
            double TimeStripFront = Data->GetGPDTrkFirstStageFrontEEnergy(0);
            double TimeStripBack  = Data->GetGPDTrkFirstStageBackEEnergy(0);
            double TimeStrip  = 0.5 * (EnergyStripFront + EnergyStripBack);
//                  double TimeStrip  = EnergyStripFront;
//                  if (TimeStripBack > TimeStrip) TimeStrip = TimeStripBack;
            FirstStage_T.push_back(TimeStrip);

            // check if we have a 2nd stage event
            if (mult2E==1 && mult2T==1) {
               Check_SecondStage = true;
               double EnergySecond = Data->GetGPDTrkSecondStageEEnergy(0);
               SecondStage_E.push_back(EnergySecond);
               EnergyTot += EnergySecond;
            }
            else if (mult2E>1 || mult2T>1) {
               cout << "Warning: multiplicity in second stage greater than in firststage" << endl;
            }
            // check if we have a third stage event
            if (mult3E==1 && mult3T==1) {
               Check_ThirdStage = true;
               double EnergyThird = Data->GetGPDTrkThirdStageEEnergy(0);
               ThirdStage_E.push_back(EnergyThird);
               EnergyTot += EnergyThird;
            }
            else if (mult3E>1 || mult3T>1) {
               cout << "Warning: multiplicity in third stage greater than in firststage" << endl;
            }

            // Analysis code here.
            TotalEnergy.push_back(EnergyTot);
         }
         else {
            cout << "Not same strips" << endl;
         }
      }
      else {
         cout << "Not same detector" << endl;
      }
   }
   else {
/*      cout << "Multiplicity is not one, it is: " << endl;
      cout << "\tmultXE: " << multXE << endl;
      cout << "\tmultXT: " << multXT << endl;
      cout << "\tmultYE: " << multYE << endl;
      cout << "\tmultYT: " << multYT << endl;*/
   }

/*	
		//	Multiplicity 2
		if( Data->GetMMStripXEMult()==2 && Data->GetMMStripYEMult()==2 && Data->GetMMStripXTMult()==2 && Data->GetMMStripXTMult()==2 )
			{
				// Different telescope case
				if (	Data->GetMMStripXEDetectorNbr(0) != Data->GetMMStripXEDetectorNbr(1)
					&&	Data->GetMMStripYEDetectorNbr(0) != Data->GetMMStripYEDetectorNbr(1) )
					{
					
						double EY, EX, TX, TY = 0;
						// loop on both event
						for (int jj = 0 ; jj < 2 ; jj++)
							{
								Check_FirstStage = false ;Check_SecondStage = false ;Check_ThirdStage = false ;
								
							
								ModuleNumber.push_back( Data->GetMMStripXEDetectorNbr(jj) )	;
								EX = Data->GetMMStripXEEnergy(jj) 				;
								FirstStage_X.push_back( Data->GetMMStripXEStripNbr(jj))	;
								
								// Get Corresponding time
								for(int i = 0 ; i < 2 ; i++)
									{ 
										if(		Data->GetMMStripXTDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj) 
											&&	Data->GetMMStripXTStripNbr(i)    == Data->GetMMStripXEStripNbr(jj)   )
											{ TX = Data->GetMMStripXTTime(jj)	; }
									}
								
								// Get Corresponding Y strip
								for(int i = 0 ; i < 2 ; i++)
									{
										if(	Data->GetMMStripYEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj) )
											{
												FirstStage_Y.push_back( Data->GetMMStripYEStripNbr(i))	;
												EY = Data->GetMMStripXEEnergy(i) 				;
												TY = Data->GetMMStripXTTime(i) 					;
												
										//		if (EX>EY)	FirstStage_E.push_back(EX)	;
										//		else	  	FirstStage_E.push_back(EY)	;
												FirstStage_E.push_back(EX);
												
												
												if (TX>TY)	FirstStage_T.push_back(TY)	;
												else	  	FirstStage_T.push_back(TX)	; 
												Check_FirstStage = true ;
											}
									}
							
								if (Check_FirstStage)
									{ 
										//	Si(Li)
										for (int i = 0 ; i < Data->GetMMSiLiEMult() ; i++)
											{ 
												if (	Data->GetMMSiLiEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
													&&	Data->GetMMSiLiEEnergy(i) > SecondStage_E_Threshold	)
													{
													SecondStage_E.push_back(Data->GetMMSiLiEEnergy(i))	;
													SecondStage_N.push_back(Data->GetMMSiLiEPadNbr(i))	;
													
													if (	Data->GetMMSiLiTDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
														&&	Data->GetMMSiLiTPadNbr(i)      == Data->GetMMSiLiEPadNbr(i) )
															{
															SecondStage_T.push_back(Data->GetMMSiLiTTime(i))	;
															Check_SecondStage = true ;
															}
													}
													
												else
													{
													SecondStage_E.push_back(-1)	;
													SecondStage_T.push_back(-1)	;
													SecondStage_N.push_back(-1)	;
													}
											}
								
										//	CsI
										for (int i = 0 ; i < Data->GetMMCsIEMult() ; i++)
											{
												if (	Data->GetMMCsIEDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
													&&	Data->GetMMCsIEEnergy(i) > ThirdStage_E_Threshold	)
													{
													ThirdStage_E.push_back(Data->GetMMCsIEEnergy(i))		;
													ThirdStage_N.push_back(Data->GetMMCsIECristalNbr(i))	;
													if (	Data->GetMMCsITDetectorNbr(i) == Data->GetMMStripXEDetectorNbr(jj)
														&&	Data->GetMMCsITCristalNbr(i)  == Data->GetMMCsIECristalNbr(i) )
															{
															ThirdStage_T.push_back(Data->GetMMCsITTime(i))		;
															Check_ThirdStage = true ;
															}
													}
													
												else
													{
													ThirdStage_E.push_back(-1)	;
													ThirdStage_T.push_back(-1)	;
													ThirdStage_N.push_back(-1)	;
													}
											}
										
										TotalEnergy.push_back(FirstStage_E.at(jj)) ;
										if (Check_SecondStage) TotalEnergy.at(jj) += SecondStage_E.at(jj)	;
										if (Check_ThirdStage)  TotalEnergy.at(jj) += ThirdStage_E.at(jj)	;
									}	
							}
						return;	
					}
					
				//	Same detector case :
				if(		Data->GetMMStripXEDetectorNbr(0) == Data->GetMMStripXEDetectorNbr(1)
					&&	Data->GetMMStripYEDetectorNbr(0) == Data->GetMMStripYEDetectorNbr(1))
					{
					
					
					return;
					}
			}
			*/
}


void TGaspardTrackerPhysics::Clear()
{
   EventMultiplicity= 0;
   ModuleNumber.clear();
   EventType.clear();
   TotalEnergy.clear();

   // Si X
   FirstStage_E.clear();
   FirstStage_T.clear();
   FirstStage_X.clear();
   FirstStage_Y.clear();

   // Si(Li)
   SecondStage_E.clear();
   SecondStage_T.clear();
   SecondStage_N.clear();

   // CsI	
   ThirdStage_E.clear();
   ThirdStage_T.clear();
   ThirdStage_N.clear();
}
