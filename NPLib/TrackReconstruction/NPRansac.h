#ifndef __RANSAC__
#define __RANSAC__
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
 * Last update     : April 2018                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with finding all the track event by event                *
 *****************************************************************************/
//C++ Header
#include <stdio.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <string>
#include <algorithm>

//NPL
#include "NPTrack.h"
using namespace NPL;

// ROOT Headers
#include <TLine.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TGraph2D.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TRandom.h>
#include <TServerSocket.h>
#include <TSocket.h>
#include <TMonitor.h>
#include <TMessage.h>
#include <TList.h>
#include <TVector.h>

using namespace std;

namespace NPL{
    
    class Ransac
    {
    public:
        Ransac();
        Ransac(int NumberOfPadsX, int NumberOfPadsY, bool Visu);
        ~Ransac();
        
    public:
        void Reset();
        void ReadParameterValue(string filename);
        void Init(vector<int> v1, vector<int> v2, vector<double> v3, vector<double> v4);
        vector<NPL::Track> SimpleRansac();
        bool CompareTracksWithBarycenter(NPL::Track track1, NPL::Track track2);
        bool CompareTracksWithScalarProduct(NPL::Track track1, NPL::Track track2);
        NPL::Track MergeTracks(vector<NPL::Track> tracks);
        void DoMerge(string method);
        //void CheckTracks();
        
        vector<double> GetChargeOfTracks();
        vector<double> GetTrackLength(double PadSizeX, double PadSizeY, double DriftVelocity);
        double Fit3D(vector<int> X, vector<int> Y, vector<double> Z, vector<double> Charge, vector<int> inliners, TVector3& V1, TVector3& V2);
        
    private:
        TCanvas* c1;
        vector<TLine*> vline;
        vector<int> vX, vY;
        vector<int> trackX, trackY, trackZ, trackQ;
        vector<double> vZ, vQ;
        TLine* L;
        TGraph2D* h3D;
        TGraph2D* pl;
        vector<NPL::Track> vTrack;
        vector<double> vTrackCharge;
        
    private:
        float fRANSACThreshold;
        float fRANSACPointThreshold;
        float fRANSACChargeThreshold;
        float fRANSACDistance;
        float fRANSACMaxIteration;
        float fMAXBarycenterDistance;
        float fAngleMax;
        int fNumberOfTracksMax;
        int fOriginalCloudSize;
        double fTotalCharge;
        int fNumberOfPadsX;
        int fNumberOfPadsY;
        int fVisu;
        bool fMergeWithBarycenter;
        bool fMergeWithScalarProduct;
        
    private:
        TServerSocket* m_ServerSocket;
        TSocket* m_Socket;
        TMonitor* m_Monitor;
        TList* m_Histo;
        TList* m_Sockets;
        
        ClassDef(Ransac, 1)
    };
}
#endif
