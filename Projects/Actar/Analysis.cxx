/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Morfouace Pierre  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : April 2018                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Actar analysis project                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include<iostream>

using namespace std;
#include"Analysis.h"
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
#include"RootOutput.h"
#include"RootInput.h"


////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
    Actar= (TActarPhysics*) m_DetectorManager->GetDetector("Actar");
    ReactionConditions = new TReactionConditions();

    Actar->ReadAnalysisConfig();
    if(Actar->GetRansacStatus()){
        Actar->SetRansacParameter("./configs/RansacConfig.dat");
    }

    DriftVelocity = Actar->GetDriftVelocity();
    PadSizeX = Actar->GetPadSizeX();
    PadSizeY = Actar->GetPadSizeY();
    NumberOfPadsX = Actar->GetNumberOfPadsX();
    NumberOfPadsY = Actar->GetNumberOfPadsY();

    EnergyLoss_3He = NPL::EnergyLoss("./EnergyLossTable/He3_D2_6.24151e+08_295.G4table","G4Table",100);
    EnergyLoss_17C = NPL::EnergyLoss("./EnergyLossTable/C17_D2_6.24151e+08_295.G4table","G4Table",100);
    TheReaction = new NPL::Reaction("17C(d,3He)16B@510");

    InitOutputBranch();
    InitInputBranch();
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
    ReInitValue();
    
    if(ReactionConditions->GetParticleMultiplicity()>0){
      InitXVertex = ReactionConditions->GetVertexPositionZ();
      InitE3      = ReactionConditions->GetKineticEnergy(0);
      InitTheta3  = ReactionConditions->GetTheta(0);
    }

    int TrackMult = Actar->GetTrackMult();

    TVector3 vX = TVector3(1,0,0);
    TVector3 aTrack, vB;

    if(TrackMult>1){
        vTrack = Actar->GetTracks();
        double scalarproduct=0;
        int BeamTrack=0;
        for(unsigned int i=0; i<TrackMult; i++){
            TVector3 vtest = TVector3(vTrack[i].GetDirectionVector().X(),vTrack[i].GetDirectionVector().Y(),vTrack[i].GetDirectionVector().Z());
            TVector3 vunit = vtest.Unit();
            double scalar = abs(vunit.Dot(vX));
            vScalar.push_back(scalar);
            //cout << scalar << endl;
            //cout << scalarproduct << endl;
            if(scalar>scalarproduct){
                BeamTrack=i;
                scalarproduct=scalar;
            }
        }

        double XBeam = vTrack[BeamTrack].GetDirectionVector().X();
        double YBeam = vTrack[BeamTrack].GetDirectionVector().Y();
        double ZBeam = vTrack[BeamTrack].GetDirectionVector().Z();
        TVector3 vBeam = TVector3(XBeam,YBeam,ZBeam);

        double XBeamPoint = vTrack[BeamTrack].GetXh();
        double YBeamPoint = vTrack[BeamTrack].GetYh();
        double ZBeamPoint = vTrack[BeamTrack].GetZh();
        TVector3 vBeamPos = TVector3(XBeamPoint,YBeamPoint,ZBeamPoint);

        vB = TVector3(XBeam*PadSizeX, YBeam*PadSizeY,ZBeam*DriftVelocity);
        BeamAngle = (vX.Angle(vB))*180/TMath::Pi();

        for(unsigned int i=0; i<TrackMult; i++){
            if(i!=BeamTrack){
                double Xdir = vTrack[i].GetDirectionVector().X();
                double Ydir = vTrack[i].GetDirectionVector().Y();
                double Zdir = vTrack[i].GetDirectionVector().Z();

                XVertex.push_back(vTrack[i].GetVertexPostion(vBeam,vBeamPos).X()*PadSizeX);
                YVertex.push_back(vTrack[i].GetVertexPostion(vBeam,vBeamPos).Y()*PadSizeY);
                ZVertex.push_back(vTrack[i].GetVertexPostion(vBeam,vBeamPos).Z()*DriftVelocity);

                aTrack = TVector3(Xdir*PadSizeX, Ydir*PadSizeY, Zdir*DriftVelocity);


                double angle = vX.Angle(aTrack)*180/TMath::Pi();
                //double angle = vB.Angle(aTrack)*180/TMath::Pi();
                if(angle>90) angle = 180-angle;

                double x1 = vTrack[i].GetXm()*PadSizeX;
                double x2 = vTrack[i].GetXh()*PadSizeX;
                double y1 = vTrack[i].GetYm()*PadSizeY-0.5*NumberOfPadsY*PadSizeY;
                double y2 = vTrack[i].GetYh()*PadSizeY-0.5*NumberOfPadsY*PadSizeY;
                //double z1 = -(vTrack[i].GetZm()-256)*DriftVelocity;
                //double z2 = -(vTrack[i].GetZh()-256)*DriftVelocity;
                double z1 = vTrack[i].GetZm()*DriftVelocity;
                double z2 = vTrack[i].GetZh()*DriftVelocity;

                GetMayaSiHitPosition(x1,x2,y1,y2,z1,z2);

                if(XVertex[i]>0 && XVertex[i]<256){
                    double LengthInGas = fSiDistanceX - XVertex[i];
                    for(unsigned int k=0; k<Actar->Si_E.size(); k++){
                      ESi.push_back(Actar->Si_E[k]);
                      SiNumber.push_back(Actar->Si_Number[k]);

                      DE.push_back(vTrack[i].GetPartialCharge(108,128)/(20./cos(angle*TMath::Pi()/180)));
                      double E3 = EnergyLoss_3He.EvaluateInitialEnergy(Actar->Si_E[k]*MeV,LengthInGas*mm,angle*TMath::Pi()/180);
                      double BeamEnergy = EnergyLoss_17C.Slow(510*MeV,(XVertex[i]+60)*mm, BeamAngle*TMath::Pi()/180);
                      TheReaction->SetBeamEnergy(BeamEnergy);
                      ELab.push_back(E3);
                      ThetaLab.push_back(angle);
                      TheReaction->SetNuclei3(E3,angle*TMath::Pi()/180);
                      Ex.push_back(TheReaction->GetExcitation4());
                      ThetaCM.push_back(TheReaction->GetThetaCM()*180./TMath::Pi());
                    }
                }
            }
        }
    }

    //for(unsigned int i=0; i<Actar->PadX.size(); i++){
        //cout << "X= " << Actar->PadX[i] << endl;
    //}
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::GetMayaSiHitPosition(double xm, double xh, double ym, double yh, double zm, double zh)
{
    double X1, X2, Y1, Y2, Z1, Z2;

    if(xm>xh){
      X1 = xh;
      Y1 = yh;
      Z1 = zh;

      X2 = xm;
      Y2 = ym;
      Z2 = zm;
    }
    else if(xh>xm){
      X1 = xm;
      Y1 = ym;
      Z1 = zm;

      X2 = xh;
      Y2 = yh;
      Z2 = zh;
    }

    double l, L, t;
    double zf, yf;

    if(fSiDistanceX>X2){
      L = fSiDistanceX-X2;
      l = X2 - X1;
      t = (l+L)/l;

      zf = Z1 + (Z2-Z1)*t;
      yf = Y1 + (Y2-Y1)*t;
    }
    else if(fSiDistanceX<X2){
      L = X2 - fSiDistanceX;
      l = fSiDistanceX - X1;
      t = (l+L)/l;

      zf = Z1 + (Z2-Z1)/t;
      yf = Y1 + (Y2-Y1)/t;
    }

    SiPosY.push_back(yf);
    SiPosZ.push_back(zf);
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch() {
  RootInput::getInstance()->GetChain()->SetBranchStatus("ReactionConditions",true);
  RootInput::getInstance()->GetChain()->SetBranchStatus("fRC_*",true);
  RootInput::getInstance()->GetChain()->SetBranchAddress("ReactionConditions",&ReactionConditions);
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
    RootOutput::getInstance()->GetTree()->Branch("DE",&DE);
    RootOutput::getInstance()->GetTree()->Branch("SiNumber",&SiNumber);
    RootOutput::getInstance()->GetTree()->Branch("ESi",&ESi);
    RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab);
    RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab);
    RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex);
    RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM);
    RootOutput::getInstance()->GetTree()->Branch("vScalar",&vScalar);
    RootOutput::getInstance()->GetTree()->Branch("XVertex",&XVertex);
    RootOutput::getInstance()->GetTree()->Branch("YVertex",&YVertex);
    RootOutput::getInstance()->GetTree()->Branch("ZVertex",&ZVertex);
    RootOutput::getInstance()->GetTree()->Branch("BeamAngle",&BeamAngle,"BeamAngle/D");
    RootOutput::getInstance()->GetTree()->Branch("SiPosY",&SiPosY);
    RootOutput::getInstance()->GetTree()->Branch("SiPosZ",&SiPosZ);
    RootOutput::getInstance()->GetTree()->Branch("InitXVertex",&InitXVertex,"InitXVertex/D");
    RootOutput::getInstance()->GetTree()->Branch("InitE3",&InitE3,"InitE3/D");
    RootOutput::getInstance()->GetTree()->Branch("InitTheta3",&InitTheta3,"InitTheta3/D");

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
    DE.clear();
    SiNumber.clear();
    ESi.clear();
    ELab.clear();
    ThetaLab.clear();
    Ex.clear();
    ThetaCM.clear();
    vScalar.clear();
    XVertex.clear();
    YVertex.clear();
    ZVertex.clear();
    SiPosY.clear();
    SiPosZ.clear();
    BeamAngle=-1000;
    InitXVertex=-1000;
    InitE3=-1000;
    InitTheta3=-1000;

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* Analysis::Construct(){
    return (NPL::VAnalysis*) new Analysis();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy{
    public:
        proxy(){
            NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
        }
    };

    proxy p;
}
