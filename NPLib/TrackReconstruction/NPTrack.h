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

#include <TLine.h>
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
        
        TLine* L2DXY;
        TLine* L2DXZ;
        TLine* L2DYZ;
        TLine* L3D;
        
    private:
        vector<int> vX;
        vector<int> vY;
        vector<int> vZ;
        vector<int> vQ;
        
    public:
        ///////////////
        /// SETTERS ///
        ///////////////
        void SetXPoints(vector<int> vec) {vX=vec;}
        void SetYPoints(vector<int> vec) {vY=vec;}
        void SetZPoints(vector<int> vec) {vZ=vec;}
        void SetQPoints(vector<int> vec) {vQ=vec;}
        
        ///////////////
        /// GETTERS ///
        ///////////////
        vector<int> GetXPoints() {return vX;}
        vector<int> GetYPoints() {return vY;}
        vector<int> GetZPoints() {return vZ;}
        vector<int> GetQPoints() {return vQ;}

        double GetXm() {return Xm;}
        double GetXh() {return Xh;}
        double GetYm() {return Ym;}
        double GetYh() {return Yh;}
        double GetZm() {return Zm;}
        double GetZh() {return Zh;}
        
    public:
        TVector3 GetDirectionVector();
        
        TVector3 GetVertexPostion(TVector3 vBeamDir, TVector3 vBeamPoint);
        
        TVector3 GetChargeBarycenter();
        
        TVector3 GetPointBarycenter();
        
        double GetTrackLength(double PadSizeX, double PadSizeY, double DriftVelocity);
        double GetTotalCharge();
        
        //void ResetLines();
        void Clear();
        };
}
#endif
