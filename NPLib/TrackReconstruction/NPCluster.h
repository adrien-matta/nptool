#ifndef __CLUSTER__
#define __CLUSTER__
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
 * Creation Date   : September 2018                                          *
 * Last update     :                                                         *
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
#include <TRandom3.h>
#include <TServerSocket.h>
#include <TSocket.h>
#include <TMonitor.h>
#include <TMessage.h>
#include <TList.h>
#include <TVector.h>


using namespace std;

#define BigVoxelSize 128/4*128/4*512/8

namespace NPL{
    class Cluster{
        
    public:
        Cluster();
        Cluster(int NumberOfPadsX, int NumberOfPadsY, bool Visu);
        ~Cluster();
        
    public:
        void ReadParameterValue(string filename);
        void Reset();
        void Clear();
        void Init(vector<int> v1, vector<int> v2, vector<double> v3, vector<double> v4);
        void FindTracks();
        void FillBigVoxels(int x, int y, int t, int q);
        void RegroupZone();
        void RemoveTrack(vector<NPL::Track> &Ephem, int id);
        void ExtractCoordinate(int ID, int& x, int &y, int& z);
        void FitZone();
        void ReorderTracks(vector<NPL::Track> &someTracks);
        void RegroupTrack();
        double Distance3D(NPL::Track aTrack, int ID);
        void FitTrack();
        double Fit3D(NPL::Track &aTrack);
        vector<NPL::Track> GetTracks() {return vTrack;}
        
        
    private:
        vector<int> vX, vY;
        vector<double> vZ, vQ;
        
    private:
        bool fVisu;
        int fNumberOfPadsX;
        int fNumberOfPadsY;
        int fNumberOfSample;
        int fNumberOfTracksMax;
        double fTotalCharge;
        double fClusterDistance;
        double fClusterThreshold;
        unsigned int fOriginalCloudSize;
        
    public:
        int it;
        vector<int> BigVoxels[BigVoxelSize];
        vector<int> IDBigVoxels[BigVoxelSize];
        int* LocBigVoxels;
        //NPL::Track BigTrack;
        vector<NPL::Track> TrackZone;
        vector<NPL::Track> vTrack;
        
    private:
        TServerSocket* m_ServerSocket;
        TSocket* m_Socket;
        TMonitor* m_Monitor;
        TList* m_Histo;
        TList* m_Sockets;
        
        ClassDef(Cluster, 0)
    };
}
#endif
