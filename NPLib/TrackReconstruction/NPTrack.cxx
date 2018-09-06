/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Pierre MORFOUACE contact address: morfouace@ganil.fr   *
 *                                                                           *
 * Creation Date   : April 2018                                              *
 * Last update     : Apro; 2018                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with track charactieristic from TPC analysis             *
 *****************************************************************************/


#include "NPTrack.h"
using namespace NPL;

using namespace std;

//ClassImp(NPTrack)

//////////////////////////////////////////////////////
Track::Track()
{
	/*L2DXY=new TLine();
	L2DXZ=new TLine();
	L2DYZ=new TLine();
	L3D=new TLine();*/
}

//////////////////////////////////////////////////////
Track::~Track()
{
	/*delete L2DXY;
	delete L2DXZ;
	delete L2DYZ;
	delete L3D;*/
}

//////////////////////////////////////////////////////
TVector3 Track::GetDirectionVector()
{
	TVector3 vTrack = TVector3(Xh-Xm, Yh-Ym, Zh-Zm);
	return vTrack;
}

//////////////////////////////////////////////////////
TVector3 Track::GetVertexPostion(TVector3 vBeamDir, TVector3 vBeamPoint)
{
	//y_beam = Ay_beam*x_beam + By_beam
	double Ay_beam = vBeamDir.Y()/vBeamDir.X();
	double By_beam = vBeamPoint.Y() - (vBeamDir.Y()/vBeamDir.X())*vBeamPoint.X();
	//z_beam = Az_beam*x_beam + Bz_beam
	double Az_beam = vBeamDir.Z()/vBeamDir.X();
	double Bz_beam = vBeamPoint.Z() - (vBeamDir.Z()/vBeamDir.X())*vBeamPoint.X();

	//y_track = Ay_track*x_track + By_track
	double Ay_track = GetDirectionVector().Y()/GetDirectionVector().X();
	double By_track = Ym - (GetDirectionVector().Y()/GetDirectionVector().X())*Xm;
	//z_track = Az_track*x_track + Bz_track
	double Az_track = GetDirectionVector().Z()/GetDirectionVector().X();
	double Bz_track = Zm - (GetDirectionVector().Z()/GetDirectionVector().X())*Xm;

	double DBy 		= By_beam - By_track;
	double DBz 		= Bz_beam - Bz_track;
	double alpha 	= (-Ay_beam*DBy - Az_beam*DBz)/(Ay_beam*Ay_beam + Az_beam*Az_beam + 1);
	double beta 	= (Ay_beam*Ay_track + Az_beam*Az_track + 1)/(Ay_beam*Ay_beam + Az_beam*Az_beam + 1);

	double A			= beta*(Ay_beam*Ay_beam + Az_beam*Az_beam + 1) - (Ay_beam*Ay_track + Az_beam*Az_track + 1);
	double B			= (Ay_track*Ay_track + Az_track*Az_track + 1) - beta*(Ay_beam*Ay_track + Az_beam*Az_track + 1);
	double C 			= beta*(Ay_beam*DBy + Az_beam*DBz) - Ay_track*DBy - Az_track*DBz;

	double xt			= -(A*alpha+C)/(A*beta+B);
	double xb			= beta*xt+alpha;

	double yb			= Ay_beam*xb + By_beam;
	double zb			= Az_beam*xb + Bz_beam;

	double yt			= Ay_track*xt + By_track;
	double zt			= Az_track*xt + Bz_track;

	double xvertex	= (xb+xt)/2;
	double yvertex	= (yb+yt)/2;
	double zvertex	= (zb+zt)/2;

	TVector3 vVertex = TVector3(xvertex,yvertex,zvertex);

	return vVertex;
}

//////////////////////////////////////////////////////
TVector3 Track::GetChargeBarycenter()
{
    double bX=0;
    double bY=0;
    double bZ=0;
    double Qtot=0;
    
    int size = vX.size();
    
    for(int i=0; i<size; i++){
        Qtot    += (double)vQ[i];
        bX      += (double)vX[i]*vQ[i];
        bY      += (double)vY[i]*vQ[i];
        bZ      += (double)vZ[i]*vQ[i];
    }
    
    bX=bX/Qtot;
    bY=bY/Qtot;
    bZ=bZ/Qtot;
    
    TVector3 vBarycentre = TVector3(bX,bY,bZ);
    
    return vBarycentre;
}

//////////////////////////////////////////////////////
TVector3 Track::GetPointBarycenter()
{
    double bX=0;
    double bY=0;
    double bZ=0;
    
    int size = vX.size();
    
    for(int i=0; i<size; i++){
        bX += (double)vX[i];
        bY += (double)vY[i];
        bZ += (double)vZ[i];
    }
    
    bX=bX/size;
    bY=bY/size;
    bZ=bZ/size;
    
    TVector3 vBarycentre = TVector3(bX,bY,bZ);
    
    return vBarycentre;
}

//////////////////////////////////////////////////////
double Track::GetTrackLengthX()
{
    double length;
    
    double Xmax = *max_element(vX.begin(), vX.end());
    double Xmin = *min_element(vX.begin(), vX.end());
    
    length = sqrt(pow(Xmax-Xmin,2));
    
    return length;

}
//////////////////////////////////////////////////////
double Track::GetTrackLength(double PadSizeX, double PadSizeY, double DriftVelocity)
{
    double length;
    
    double Xmax = *max_element(vX.begin(), vX.end())*PadSizeX;
    double Xmin = *min_element(vX.begin(), vX.end())*PadSizeX;
    
    double Ymax = *max_element(vY.begin(), vY.end())*PadSizeY;
    double Ymin = *min_element(vY.begin(), vY.end())*PadSizeY;

    double Zmax = *max_element(vZ.begin(), vZ.end())*DriftVelocity;
    double Zmin = *min_element(vZ.begin(), vZ.end())*DriftVelocity;
    
    length = sqrt(pow(Xmax-Xmin,2) + pow(Ymax-Ymin,2) + pow(Zmax-Zmin,2));

    return length;
}

//////////////////////////////////////////////////////
double Track::GetPartialCharge(double PadXmin, double PadXmax)
{
    double Q=0;
    unsigned int size = vQ.size();
    for(unsigned int i=0; i<size; i++){
        if(vX[i]>=PadXmin && vX[i]<=PadXmax){
            Q += vQ[i];
        }
    }
    return Q;
}
//////////////////////////////////////////////////////
double Track::GetTotalCharge()
{
    double Qtot=0;
    
    int size = vQ.size();
    for(int i=0; i<size; i++){
        Qtot += vQ[i];
    }
    Qtot = Qtot/size;
    
    return Qtot;
}

//////////////////////////////////////////////////////
void Track::Clear()
{
    vX.clear();
    vY.clear();
    vZ.clear();
    vQ.clear();
}

//////////////////////////////////////////////////////
/*void Track::ResetLines()
{
	L2DXY->SetX1(-1);
	L2DXY->SetY1(-1);
	L2DXY->SetX2(-1);
	L2DXY->SetY2(-1);

	L2DXZ->SetX1(-1);
	L2DXZ->SetY1(-1);
	L2DXZ->SetX2(-1);
	L2DXZ->SetY2(-1);

	L2DYZ->SetX1(-1);
	L2DYZ->SetY1(-1);
	L2DYZ->SetX2(-1);
	L2DYZ->SetY2(-1);

	L3D->SetX1(-1);
	L3D->SetY1(-1);
	L3D->SetX2(-1);
	L3D->SetY2(-1);
}*/
