#ifndef __TRACK__
#define __TRACK__
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

#include <TVector3.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

namespace NPL{
    class Track{
        
    public:
        Track();
        ~Track();
    public:        
        double Xm;
        double Ym;
        double Zm;
        double Xh;
        double Yh;
        double Zh;

        
    private:
        vector<int> vX;
        vector<int> vY;
        vector<int> vZ;
        vector<int> vQ;
        
        double SlopeXY;
        double SlopeXZ;
        double SlopeYZ;
        double OffsetXY;
        double OffsetXZ;
        double OffsetYZ;
        
    public:
        ///////////////
        /// SETTERS ///
        ///////////////
        void SetXm(double x) {Xm=x;}
        void SetYm(double y) {Ym=y;}
        void SetZm(double z) {Zm=z;}
        void SetXh(double x) {Xh=x;}
        void SetYh(double y) {Yh=y;}
        void SetZh(double z) {Zh=z;}
        
        
        void SetXPoints(vector<int> vec) {vX=vec;}
        void SetYPoints(vector<int> vec) {vY=vec;}
        void SetZPoints(vector<int> vec) {vZ=vec;}
        void SetQPoints(vector<int> vec) {vQ=vec;}
        
        void SetPointX(int x) {vX.push_back(x);}
        void SetPointY(int y) {vY.push_back(y);}
        void SetPointZ(int z) {vZ.push_back(z);}
        void SetPointQ(int q) {vQ.push_back(q);}
        
        void SetSlopeXY(double p1) {SlopeXY=p1;}
        void SetSlopeXZ(double p1) {SlopeXZ=p1;}
        void SetSlopeYZ(double p1) {SlopeYZ=p1;}
        void SetOffsetXY(double p0) {OffsetXY=p0;}
        void SetOffsetXZ(double p0) {OffsetXZ=p0;}
        void SetOffsetYZ(double p0) {OffsetYZ=p0;}
        
        void SetSlopesAndOffsets();
        
        ///////////////
        /// GETTERS ///
        ///////////////
        vector<int> GetXPoints() {return vX;}
        vector<int> GetYPoints() {return vY;}
        vector<int> GetZPoints() {return vZ;}
        vector<int> GetQPoints() {return vQ;}
        
        int GetPointX(int i) {return vX[i];}
        int GetPointY(int i) {return vY[i];}
        int GetPointZ(int i) {return vZ[i];}
        int GetPointQ(int i) {return vQ[i];}
        
        double GetXm() {return Xm;}
        double GetXh() {return Xh;}
        double GetYm() {return Ym;}
        double GetYh() {return Yh;}
        double GetZm() {return Zm;}
        double GetZh() {return Zh;}
        
        double GetSlopeXY() {return SlopeXY;}
        double GetSlopeXZ() {return SlopeXZ;}
        double GetSlopeYZ() {return SlopeYZ;}
        double GetOffsetXY() {return OffsetXY;}
        double GetOffsetXZ() {return OffsetXZ;}
        double GetOffsetYZ() {return OffsetYZ;}
        
    public:
        TVector3 GetDirectionVector();
        
        TVector3 GetVertexPostion(TVector3 vBeamDir, TVector3 vBeamPoint);
        
        TVector3 GetChargeBarycenter();
        
        TVector3 GetPointBarycenter();
        
        
        
        double GetTrackLengthX();
        double GetTrackLength(double PadSizeX, double PadSizeY, double DriftVelocity);
        double GetTotalCharge();
        double GetPartialCharge(double PadXmin, double PadXmax);
        
        //void ResetLines();
        void Clear();
        };
}
#endif
