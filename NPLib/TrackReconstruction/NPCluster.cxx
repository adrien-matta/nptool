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
 * Creation Date   : Septemeber 2018                                         *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with finding all the track event by event                *
 *****************************************************************************/

#include "NPCluster.h"
using namespace NPL;

using namespace std;

ClassImp(Cluster)

/////////////////////////////////////////////////
Cluster::Cluster()
{
    fNumberOfPadsX = 128;
    fNumberOfPadsY = 128;
    fNumberOfSample = 512;
    fNumberOfTracksMax = 20;
    fClusterDistance = 5;
    fClusterThreshold = 15;

    fVisu= 0;

    LocBigVoxels = new int[BigVoxelSize];
}

/////////////////////////////////////////////////
Cluster::Cluster(int NumberOfPadsX, int NumberOfPadsY, bool Visu)
{
    fNumberOfPadsX = NumberOfPadsX;
    fNumberOfPadsY = NumberOfPadsY;
    fNumberOfSample = 512;
    fNumberOfTracksMax = 20;
    fClusterDistance = 5;
    fClusterThreshold = 15;

    fVisu = 0;

    LocBigVoxels = new int[BigVoxelSize];

    if(fVisu==1){
        m_ServerSocket = new TServerSocket(9092,true,100);
        //pl		= new TGraph2D();
        if(!m_ServerSocket->IsValid()){
            cout << endl;
            cout << "/// ServerSocket invalid! ///" << endl;
            exit(1);
        }
        cout << endl;
        cout << "/// ServerSocket valid ///" << endl;
        m_ServerSocket->SetCompressionSettings(1);

        // Add server socket to monitor so we are notified when a client needs to be
        // accepted
        m_Monitor  = new TMonitor();
        m_Monitor->Add(m_ServerSocket,TMonitor::kRead|TMonitor::kWrite);
        // Create a list to contain all client connections
        m_Sockets = new TList;

        // Create a list to contain all client connections
        m_Histo = new TList;

    }
}


//////////////////////////////////////////////////
Cluster::~Cluster()
{
}

//////////////////////////////////////////////////
void Cluster::ReadParameterValue(string filename)
{
    bool ReadingStatus = false;

    ifstream ClusterParamFile;
    ClusterParamFile.open(filename.c_str());

    if(!ClusterParamFile.is_open()){
        cout << "/// No Paramter File found for Cluster parameters ///" << endl;
        cout << "/// Using the default parameter:" << endl;
        cout << "/// Number of tracks max= " << fNumberOfTracksMax << endl;
    }
    else{
        string LineBuffer, whatToDo, DataBuffer;
        while(!ClusterParamFile.eof()){
            getline(ClusterParamFile,LineBuffer);
            string name = "ConfigCluster";
            if(LineBuffer.compare(0, name.length(), name) == 0){
                cout << endl;
                cout << "**** ConfigCluster found" << endl;
                ReadingStatus = true;
            }
            while(ReadingStatus){
                whatToDo="";
                ClusterParamFile >> whatToDo;
                // Search for comment symbol (%)
                if (whatToDo.compare(0, 1, "%") == 0) {
                    ClusterParamFile.ignore(numeric_limits<streamsize>::max(), '\n' );
                }

                else if (whatToDo.compare(0,18,"NumberOfTracksMax=") == 0) {
                    ClusterParamFile >> DataBuffer;
                    fNumberOfTracksMax = atoi(DataBuffer.c_str());
                    cout << "/// Number of tracks max= " << " " << fNumberOfTracksMax << " ///" << endl;
                }
                else if (whatToDo.compare(0,16,"ClusterDistance=") == 0) {
                    ClusterParamFile >> DataBuffer;
                    fClusterDistance = atoi(DataBuffer.c_str());
                    cout << "/// Cluster Distance max= " << " " << fClusterDistance << " ///" << endl;
                }
                else if (whatToDo.compare(0,17,"ClusterThreshold=") == 0) {
                    ClusterParamFile >> DataBuffer;
                    fClusterThreshold = atoi(DataBuffer.c_str());
                    cout << "/// Cluster Threshold= " << " " << fClusterThreshold << " ///" << endl;
                }
                else{
                    ReadingStatus = false;
                }
            }
        }
    }
}

//////////////////////////////////////////////////
void Cluster::FindTracks()
{
    for(unsigned int i=0; i<fOriginalCloudSize; i++){
      if(vX[i]>=0 && vX[i]<fNumberOfPadsX && vY[i]>=0 && vY[i]<fNumberOfPadsY){
        FillBigVoxels(vX[i],vY[i],(int)vZ[i],(int)vQ[i]);
      }
    }

    RegroupZone();

    FitZone();

    RegroupTrack();

    FitTrack();
}

//////////////////////////////////////////////////
// The goal is to remake space into bigger voxels so as to determine regions with tracks (TrackZone),
// and still keep the information on the signal amplitude (BigVoxels) and the exact position in the small voxel frame (IDBigVoxels)
// This should get rid of small noises and predefine first regions where it is interesting to try and make a fit of tracks.
// A TrackZone is defined only when 2 neighbour big voxels are found.
//////////////////////////////////////////////////
void Cluster::FillBigVoxels(int x, int y, int t, int q)
{
    BigVoxels[int(x/4 + y/4*fNumberOfPadsX/4 + t/8*fNumberOfPadsX/4*fNumberOfPadsY/4)].push_back(q);
    IDBigVoxels[int(x/4 + y/4*fNumberOfPadsX/4 + t/8*fNumberOfPadsX/4*fNumberOfPadsY/4)].push_back(x + y*fNumberOfPadsX + t*fNumberOfPadsX*fNumberOfPadsY);

    for(int tB=TMath::Max(t/8-1.,0.);tB<=TMath::Min(t/8+1.,510/8.);tB++){
        for(int yB=TMath::Max(y/4-1.,0.);yB<=TMath::Min(y/4+1.,(fNumberOfPadsY-1)/4.);yB++){
            for(int xB=TMath::Max(x/4-1.,0.);xB<=TMath::Min(x/4+1.,(fNumberOfPadsX-1)/4.);xB++){
                if(BigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4].size()>0 && (xB!=x/4||yB!=y/4||tB!=t/8) && LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]==-1 && LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]==-1){
                    //(yB+y!=fNumberOfPadsY/4-1) corresponds to the differentiation between each side of the pad plane
                    // Case where neither the current box nor the neighbour studied are associated to any track zone yet
                    //cout << 1 << endl;
                    it++;

                    NPL::Track BigTrack;
                    BigTrack.SetPointX(x/4);
                    BigTrack.SetPointY(y/4);
                    BigTrack.SetPointZ(t/8);

                    BigTrack.SetPointX(xB);
                    BigTrack.SetPointY(yB);
                    BigTrack.SetPointZ(tB);

                    LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]=it;
                    LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]=it;
                    TrackZone.push_back(BigTrack);
                    BigTrack.Clear();


                }
                else if(BigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4].size()>0 && (xB!=x/4||yB!=y/4||tB!=t/8) && LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]==-1 && LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]>-1){
                    // Case where the neighbour is already associated with a track zone but not the current box
                    //cout << 2 << endl;
                    LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]=LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4];

                    TrackZone.at(LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]).SetPointX(x/4);
                    TrackZone.at(LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]).SetPointY(y/4);
                    TrackZone.at(LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]).SetPointZ(t/8);


                }
                else if(BigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4].size()>0 && (xB!=x/4||yB!=y/4||tB!=t/8) && LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]>-1 && LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]==-1){
                    // Case where the current box is already associated with a track zone but not the neighbour
                    //cout << 3 << endl;
                    LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]=LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4];

                    TrackZone.at(LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]).SetPointX(xB);
                    TrackZone.at(LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]).SetPointY(yB);
                    TrackZone.at(LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]).SetPointZ(tB);


                }
                else if(LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]!=LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4] && (xB!=x/4||yB!=y/4||tB!=t/8) && LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4]>-1 && LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]>-1){
                    // Case where both boxes are associated to a track zone, we keep the smallest possible id.
                    //cout << 4 << endl;
                    unsigned int maxid=TMath::Max(LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4],LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]);

                    unsigned int minid=TMath::Min(LocBigVoxels[x/4+y/4*fNumberOfPadsX/4+t/8*fNumberOfPadsX/4*fNumberOfPadsY/4],LocBigVoxels[xB+yB*fNumberOfPadsX/4+tB*fNumberOfPadsX/4*fNumberOfPadsY/4]);

                    unsigned int mysize=TrackZone.at(maxid).GetXPoints().size();

                    int element = TrackZone.at(maxid).GetXPoints().at(0) + fNumberOfPadsX/4*TrackZone.at(maxid).GetYPoints().at(0) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(maxid).GetZPoints().at(0);
                    int Loc=LocBigVoxels[element];

                    for(unsigned int i=0;i<mysize;i++){
                        int element_max = TrackZone.at(maxid).GetXPoints().at(mysize-i-1) + fNumberOfPadsX/4*TrackZone.at(maxid).GetYPoints().at(mysize-i-1) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(maxid).GetZPoints().at(mysize-i-1);

                        int element_min = TrackZone.at(minid).GetXPoints().at(0) + fNumberOfPadsX/4*TrackZone.at(minid).GetYPoints().at(0) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(minid).GetZPoints().at(0);

                        LocBigVoxels[element_max]=LocBigVoxels[element_min];

                        TrackZone.at(minid).SetPointX(TrackZone.at(maxid).GetXPoints().at(mysize-i-1));
                        TrackZone.at(minid).SetPointY(TrackZone.at(maxid).GetYPoints().at(mysize-i-1));
                        TrackZone.at(minid).SetPointZ(TrackZone.at(maxid).GetZPoints().at(mysize-i-1));

                        TrackZone.at(maxid).GetXPoints().pop_back();
                        TrackZone.at(maxid).GetYPoints().pop_back();
                        TrackZone.at(maxid).GetZPoints().pop_back();

                    }

                    for(unsigned int i=Loc+1;i<TrackZone.size();i++){
                        for(unsigned int j=0;j<TrackZone.at(i).GetXPoints().size();j++){

                            element = TrackZone.at(i).GetXPoints().at(j) + fNumberOfPadsX/4*TrackZone.at(i).GetYPoints().at(j) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(i).GetZPoints().at(j);
                            LocBigVoxels[element]--;
                        }
                        TrackZone.at(i-1) = TrackZone.at(i);
                    }
                    it--;
                    TrackZone.at(TrackZone.size()-1).Clear();
                    TrackZone.pop_back();
                }
            }
        }
    }
}

//////////////////////////////////////////////////
// Check neighbouring TrackZone to regroup the closest zone together.
//////////////////////////////////////////////////
void Cluster::RegroupZone()
{
    int x1,y1,z1,x2,y2,z2;
    if(TrackZone.size()>0){
        for(int i=0;i<TrackZone.size()-1;i++){
            for(int j=i+1;j<TrackZone.size();j++){
                it=0;
                for(int k1=0;k1<TrackZone.at(i).GetXPoints().size();k1++){
                    for(int k2=0;k2<TrackZone.at(j).GetXPoints().size();k2++){
                        // Extract the coordinates of each big voxel of 2 TrackZone to check their closeness.
                        x1 = TrackZone.at(i).GetPointX(k1);
                        y1 = TrackZone.at(i).GetPointY(k1);
                        z1 = TrackZone.at(i).GetPointZ(k1);

                        x2 = TrackZone.at(j).GetPointX(k2);
                        y2 = TrackZone.at(j).GetPointY(k2);
                        z2 = TrackZone.at(j).GetPointZ(k2);

                        //if(fabs(x1-x2)+fabs(y1-y2)+fabs(z1-z2)==1 && y1+y2!=fNumberOfPadsY/4-1) it++;
                        if(fabs(x1-x2)+fabs(y1-y2)+fabs(z1-z2)==1) it++;
                    }
                    if(it>0){
                        //Threshold to decide when two TrackZone are actually only one or not.
                        for(int k2=0;k2<TrackZone.at(j).GetXPoints().size();k2++){
                            //Add the neighbour TrackZone j to the considered TrackZone i.
                            TrackZone.at(i).SetPointX(TrackZone.at(j).GetPointX(k2));
                            TrackZone.at(i).SetPointY(TrackZone.at(j).GetPointY(k2));
                            TrackZone.at(i).SetPointZ(TrackZone.at(j).GetPointZ(k2));
                        }
                        for(int k2=0;k2<TrackZone.at(j).GetXPoints().size();k2++){
                            // Get rid of the remnant of this TrackZone j integrated to the afore mentioned TrackZone i.
                            TrackZone.at(j).GetXPoints().pop_back();
                            TrackZone.at(j).GetYPoints().pop_back();
                            TrackZone.at(j).GetZPoints().pop_back();
                        }
                        RemoveTrack(TrackZone,j);
                    }
                }
            }
        }
    }
    for(int i=0;i<TrackZone.size();i++){
        if(TrackZone.at(i).GetXPoints().size()<2){
            RemoveTrack(TrackZone,i);
            i--;
        }
    }
}


//////////////////////////////////////////////////
// We fit each zone and identify zones more related to the beam
// The beam would make the reconstructed angle too small.
// By removing points in the beam region towards column number smaller than the point of crossing roughly between the Zone fit and the middle pad,
// we expect, if points of the beam were actually taken, that the angle will increase, in which case a new shift position will be calculated. When it stops changing, the TrackZone is supposed to be found.
//////////////////////////////////////////////////
void Cluster::FitZone()
{
    double p1xy, p1xz, p1yz, p0xy, p0xz, p0yz;
    for(unsigned int i = 0; i<TrackZone.size(); i++){
        NPL::Track myTrack;
        for(unsigned int j=0;j<TrackZone.at(i).GetXPoints().size();j++){
            int element = TrackZone.at(i).GetPointX(j) + fNumberOfPadsX/4*TrackZone.at(i).GetPointY(j) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(i).GetPointZ(j);

            unsigned int mysize = IDBigVoxels[element].size();

            for(unsigned int k=0;k<mysize;k++){
                int x, y, z, q;
                q = BigVoxels[element].at(k);
                ExtractCoordinate(IDBigVoxels[element].at(k), x, y, z);

                myTrack.SetPointX(x);
                myTrack.SetPointY(y);
                myTrack.SetPointZ(z);
                myTrack.SetPointQ(q);
            }
        }
        if(myTrack.GetXPoints().size()>0){
            Fit3D(myTrack);

            TrackZone[i].SetXm(myTrack.GetXm());
            TrackZone[i].SetYm(myTrack.GetYm());
            TrackZone[i].SetZm(myTrack.GetZm());
            TrackZone[i].SetXh(myTrack.GetXh());
            TrackZone[i].SetYh(myTrack.GetYh());
            TrackZone[i].SetZh(myTrack.GetZh());
            TrackZone[i].SetSlopesAndOffsets();
        }
        myTrack.Clear();
    }
}

//////////////////////////////////////////////////
// Regroup the points into tracks based on the distance between the points and the 3d fit.
//////////////////////////////////////////////////
void Cluster::RegroupTrack()
{
    ReorderTracks(TrackZone);

    unsigned int TrackZoneSize = TrackZone.size();
    int ite=0;

    for(unsigned int id=0; id<TrackZoneSize; id++){
        if(TrackZone[id].GetXPoints().size()>0){
            NPL::Track newTrack;

            int TrackZoneNumberOfPoints=0;
            int xT, xTZ;
            int yT, yTZ;
            int zT, zTZ;
            int element1 =  TrackZone.at(id).GetPointX(0) + fNumberOfPadsX/4*TrackZone.at(id).GetPointY(0) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(id).GetPointZ(0);
            ExtractCoordinate(IDBigVoxels[element1].at(0),xT,yT,zT);
            for(unsigned int i=0; i<TrackZoneSize; i++){
                for(unsigned int j=0; j<TrackZone[i].GetXPoints().size(); j++){
                    int element2 =  TrackZone.at(i).GetPointX(j) + fNumberOfPadsX/4*TrackZone.at(i).GetPointY(j) + fNumberOfPadsX/4*fNumberOfPadsY/4*TrackZone.at(i).GetPointZ(j);
                    ExtractCoordinate(IDBigVoxels[element2].at(0),xTZ,yTZ,zTZ);


                    //We verify that we are on the same side.
                    if((yTZ-fNumberOfPadsY/2.)*(yT-fNumberOfPadsY/2.)>=0){
                        if(i==id) TrackZoneNumberOfPoints+= IDBigVoxels[element2].size();
                        for(unsigned int k=0; k<IDBigVoxels[element2].size(); k++){
                            if(IDBigVoxels[element2].at(k)>0){
                                ExtractCoordinate(IDBigVoxels[element2].at(k),xTZ,yTZ,zTZ);
                                if(yTZ-fNumberOfPadsY/2.-1) yTZ=fNumberOfPadsY/2.;
                                if( (i==id && BigVoxels[element2].at(k)>4050) || (((i!=id && (xTZ>=100 || fabs(yTZ-fNumberOfPadsY/2)>=6)) || i==id) && (yTZ-fNumberOfPadsY/2.)*(yT-fNumberOfPadsY/2.+0.1)>=0 && IDBigVoxels[element2].at(k)>0 && Distance3D(TrackZone.at(id),IDBigVoxels[element2].at(k))<fClusterDistance) ){

                                    ite++;

                                    int x_track, y_track, z_track;
                                    int q_track = BigVoxels[element2].at(k);

                                    ExtractCoordinate(IDBigVoxels[element2].at(k),x_track,y_track,z_track);
                                    newTrack.SetPointX(x_track);
                                    newTrack.SetPointY(y_track);
                                    newTrack.SetPointZ(z_track);
                                    newTrack.SetPointQ(q_track);
                                }
                            }
                            else if(i==id && IDBigVoxels[element2].at(k)==-1) ite++;
                        }
                    }
                }
            }
            if((float)ite>=TrackZoneNumberOfPoints*0.9){
                vTrack.push_back(newTrack);
            }

            newTrack.Clear();
        }
    }
}

//////////////////////////////////////////////////
void Cluster::FitTrack()
{
    vector<int> to_erase;
    for(unsigned int i=0; i<vTrack.size(); i++){
        if(vTrack[i].GetXPoints().size()>fClusterThreshold){
            Fit3D(vTrack[i]);
        }
        else to_erase.push_back(i);
    }

    for(int i=to_erase.size()-1; i>=0; i--) vTrack.erase(vTrack.begin()+to_erase[i]);
}

//////////////////////////////////////////////////
double Cluster::Distance3D(NPL::Track aTrack, int ID)
{
    int x, y, z;
    double xf, yf, zf;
    double distance;
    ExtractCoordinate(ID,x,y,z);

    TVector3 V1;
    TVector3 V2;

    V1.SetXYZ(aTrack.GetXm()-aTrack.GetXh(), aTrack.GetYm()-aTrack.GetYh(), aTrack.GetZm()-aTrack.GetZh());
    V2.SetXYZ(aTrack.GetXm()-x, aTrack.GetYm()-y, aTrack.GetZm()-z);

    TVector3 V2CV1 = V2.Cross(V1);

    distance = V2CV1.Mag()/V1.Mag();

    return distance;
}

//////////////////////////////////////////////////
void Cluster::ReorderTracks(vector<NPL::Track> &someTracks)
{
    NPL::Track MoveTrack;
    if(someTracks.size()>1){
        for(unsigned int i=0; i<someTracks.size(); i++){
            for(int j=i+1; j<someTracks.size(); j++){
                if(someTracks.at(j).GetXPoints().size()>someTracks.at(i).GetXPoints().size()){
                    MoveTrack = someTracks[j];
                    someTracks[j] = someTracks[i];
                    someTracks[i] = MoveTrack;
                }
            }
        }
    }
}

//////////////////////////////////////////////////
void Cluster::ExtractCoordinate(int ID, int& x, int &y, int& z)
{
    x = ID%fNumberOfPadsX;
    y = (ID%(fNumberOfPadsX*fNumberOfPadsY)-x)/fNumberOfPadsX;
    z = ID/fNumberOfPadsX/fNumberOfPadsY;
}

//////////////////////////////////////////////////
void Cluster::RemoveTrack(vector<NPL::Track> &Ephem, int id)
{
    if(Ephem.size()>id)
        for(int i=id;i<Ephem.size()-1;i++){
            Ephem.at(i) = Ephem.at(i+1);
        }
    Ephem.pop_back();
}


//////////////////////////////////////////////////
void Cluster::Init(vector<int> v1, vector<int> v2, vector<double> v3, vector<double> v4)
{
    Reset();

    vX = v1;
    vY = v2;
    vZ = v3;
    vQ = v4;

    fOriginalCloudSize = vX.size();
    double TotalCharge=0;
    for(unsigned int i=0; i< vQ.size(); i++){
        TotalCharge += vQ[i];
    }
    fTotalCharge = TotalCharge;

}

//////////////////////////////////////////////////
void Cluster::Reset()
{
    fTotalCharge = 0;
    vX.clear();
    vY.clear();
    vZ.clear();
    vQ.clear();
    TrackZone.clear();

    for(int i=0; i<BigVoxelSize; i++){
        BigVoxels[i].clear();
        IDBigVoxels[i].clear();
        LocBigVoxels[i] = -1;
    }
    it = -1;
}

//////////////////////////////////////////////////
void Cluster::Clear()
{
    vTrack.clear();
}

//////////////////////////////////////////////////
double Cluster::Fit3D(NPL::Track &aTrack)
{
    // adapted from: http://fr.scribd.com/doc/31477970/Regressions-et-trajectoires-3D

    vector<int> X;
    vector<int> Y;
    vector<int> Z;

    X = aTrack.GetXPoints();
    Y = aTrack.GetYPoints();
    Z = aTrack.GetZPoints();


    int R, C;
    double Q;
    double Xm,Ym,Zm;
    double Xh,Yh,Zh;
    double a,b;
    double Sxx,Sxy,Syy,Sxz,Szz,Syz;
    double theta;
    double K11,K22,K12,K10,K01,K00;
    double c0,c1,c2;
    double p,q,r,dm2;
    double rho,phi;

    Q=Xm=Ym=Zm=0.;
    Sxx=Syy=Szz=Sxy=Sxz=Syz=0.;

    for (unsigned int i=0; i<X.size(); i++)
    {
        Q++;
        Xm+=X[i];
        Ym+=Y[i];
        Zm+=Z[i];
        Sxx+=X[i]*X[i];
        Syy+=Y[i]*Y[i];
        Szz+=Z[i]*Z[i];
        Sxy+=X[i]*Y[i];
        Sxz+=X[i]*Z[i];
        Syz+=Y[i]*Z[i];
    }

    Xm/=Q;
    Ym/=Q;
    Zm/=Q;
    Sxx/=Q;
    Syy/=Q;
    Szz/=Q;
    Sxy/=Q;
    Sxz/=Q;
    Syz/=Q;
    Sxx-=(Xm*Xm);
    Syy-=(Ym*Ym);
    Szz-=(Zm*Zm);
    Sxy-=(Xm*Ym);
    Sxz-=(Xm*Zm);
    Syz-=(Ym*Zm);

    theta=0.5*atan((2.*Sxy)/(Sxx-Syy));

    K11=(Syy+Szz)*pow(cos(theta),2)+(Sxx+Szz)*pow(sin(theta),2)-2.*Sxy*cos(theta)*sin(theta);
    K22=(Syy+Szz)*pow(sin(theta),2)+(Sxx+Szz)*pow(cos(theta),2)+2.*Sxy*cos(theta)*sin(theta);
    K12=-Sxy*(pow(cos(theta),2)-pow(sin(theta),2))+(Sxx-Syy)*cos(theta)*sin(theta);
    K10=Sxz*cos(theta)+Syz*sin(theta);
    K01=-Sxz*sin(theta)+Syz*cos(theta);
    K00=Sxx+Syy;

    c2=-K00-K11-K22;
    c1=K00*K11+K00*K22+K11*K22-K01*K01-K10*K10;
    c0=K01*K01*K11+K10*K10*K22-K00*K11*K22;


    p=c1-pow(c2,2)/3.;
    q=2.*pow(c2,3)/27.-c1*c2/3.+c0;
    r=pow(q/2.,2)+pow(p,3)/27.;


    if(r>0) dm2=-c2/3.+pow(-q/2.+sqrt(r),1./3.)+pow(-q/2.-sqrt(r),1./3.);
    if(r<0)
    {
        rho=sqrt(-pow(p,3)/27.);
        phi=acos(-q/(2.*rho));
        dm2=min(-c2/3.+2.*pow(rho,1./3.)*cos(phi/3.),min(-c2/3.+2.*pow(rho,1./3.)*cos((phi+2.*TMath::Pi())/3.),-c2/3.+2.*pow(rho,1./3.)*cos((phi+4.*TMath::Pi())/3.)));
    }

    a=-K10*cos(theta)/(K11-dm2)+K01*sin(theta)/(K22-dm2);
    b=-K10*sin(theta)/(K11-dm2)-K01*cos(theta)/(K22-dm2);

    Xh=((1.+b*b)*Xm-a*b*Ym+a*Zm)/(1.+a*a+b*b);
    Yh=((1.+a*a)*Ym-a*b*Xm+b*Zm)/(1.+a*a+b*b);
    Zh=((a*a+b*b)*Zm+a*Xm+b*Ym)/(1.+a*a+b*b);

    aTrack.SetXm(Xm);
    aTrack.SetYm(Ym);
    aTrack.SetZm(Zm);
    aTrack.SetXh(Xh);
    aTrack.SetYh(Yh);
    aTrack.SetZh(Zh);
    aTrack.SetSlopesAndOffsets();


    return(fabs(dm2/Q));
}
