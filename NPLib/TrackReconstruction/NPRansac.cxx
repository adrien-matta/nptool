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

#include "NPRansac.h"
using namespace NPL;

using namespace std;

ClassImp(Ransac)

/////////////////////////////////////////////////
Ransac::Ransac()
{
    fRANSACMaxIteration = 1000;
    fRANSACThreshold = 30;//100;
    fRANSACPointThreshold = 0.04;//0.07;
    fRANSACChargeThreshold = 0.04;//0.07;
    fRANSACDistance = 11;//7;
    fMAXBarycenterDistance = 10;
    fAngleMax = 2;//degree
    fMergeWithBarycenter = true;
    fMergeWithScalarProduct = false;
    
    fNumberOfPadsX = 128;
    fNumberOfPadsY = 128;
}

/////////////////////////////////////////////////
Ransac::Ransac(int NumberOfPadsX, int NumberOfPadsY, bool Visu)
{
    fRANSACMaxIteration = 1000;
    fRANSACThreshold = 60;//100;
    fRANSACPointThreshold = 0.04;//0.07;
    fRANSACChargeThreshold = 0.04;//0.07;
    fRANSACDistance = 11;//7;
    
    fNumberOfPadsX = NumberOfPadsX;
    fNumberOfPadsY = NumberOfPadsY;
    fVisu = Visu;
    //fNumberOfTracksMax = 10;
    
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
Ransac::~Ransac()
{
    for(unsigned int i=0; i<vline.size(); i++){
        delete vline[i];
    }
}

//////////////////////////////////////////////////
void Ransac::ReadParameterValue(string filename)
{
    bool ReadingStatus = false;
    
    ifstream RansacParamFile;
    RansacParamFile.open(filename.c_str());
    
    if(!RansacParamFile.is_open()){
        cout << "No Paramter File found for Ransac parameters" << endl;
        cout << "Using the default parameter:" << endl;
        cout << "RANSAC MaxIteration= " << fRANSACMaxIteration << endl;
        cout << "RANSAC Threshold=" << fRANSACThreshold << endl;
        cout << "RANSAC ChargeThreshold= " << fRANSACChargeThreshold << endl;
        cout << "RANSAC Distance= " << fRANSACDistance << endl;
        cout << "RANSAC PointThreshold= " << fRANSACPointThreshold << endl;
        cout << "MAX Barycenter Distance= " << fMAXBarycenterDistance << endl;
        cout << "Angle Max to merge tracks= " << fAngleMax << endl;
        cout << "Merging Tracks using the barycenter= " << fMergeWithBarycenter << endl;
        cout << "Merging Tracks using the scalar product= " << fMergeWithScalarProduct << endl;
    }
    else{
        string LineBuffer, whatToDo, DataBuffer;
        while(!RansacParamFile.eof()){
            getline(RansacParamFile,LineBuffer);
            string name = "ConfigRansac";
            if(LineBuffer.compare(0, name.length(), name) == 0){
                cout << endl;
                cout << "**** ConfigRansac found" << endl;
                ReadingStatus = true;
            }
            while(ReadingStatus){
                whatToDo="";
                RansacParamFile >> whatToDo;
                // Search for comment symbol (%)
                if (whatToDo.compare(0, 1, "%") == 0) {
                    RansacParamFile.ignore(numeric_limits<streamsize>::max(), '\n' );
                }
                else if (whatToDo.compare(0,19,"RANSACMaxIteration=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fRANSACMaxIteration = atoi(DataBuffer.c_str());
                    cout << "/// RANSAC MaxIteration= " << " " << fRANSACMaxIteration << " ///" << endl;
                }
                else if (whatToDo.compare(0,15,"RANSACDistance=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fRANSACDistance = atoi(DataBuffer.c_str());
                    cout << "/// RANSAC Distance= " << " " << fRANSACDistance << " ///" << endl;
                }
                else if (whatToDo.compare(0,22,"RANSACChargeThreshold=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fRANSACChargeThreshold = atof(DataBuffer.c_str());
                    cout << "/// RANSAC ChargeThreshold= " << " " << fRANSACChargeThreshold << " ///" << endl;
                }
                else if (whatToDo.compare(0,21,"RANSACPointThreshold=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fRANSACPointThreshold = atof(DataBuffer.c_str());
                    cout << "/// RANSAC PointThreshold= " << " " << fRANSACPointThreshold << " ///" << endl;
                }
                else if (whatToDo.compare(0,16,"RANSACThreshold=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fRANSACThreshold = atoi(DataBuffer.c_str());
                    cout << "/// RANSAC Threshold= " << " " << fRANSACThreshold << " ///" << endl;
                }
                else if (whatToDo.compare(0,22,"MAXBarycenterDistance=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fMAXBarycenterDistance = atoi(DataBuffer.c_str());
                    cout << "/// Max Barycenter Distance= " << " " << fMAXBarycenterDistance << " ///" << endl;
                }
                else if (whatToDo.compare(0,16,"AngleMaxToMerge=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fAngleMax = atoi(DataBuffer.c_str());
                    cout << "/// Angle Max to merge tracks= " << " " << fAngleMax << " ///" << endl;
                }
                else if (whatToDo.compare(0,20,"MergeWithBarycenter=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fMergeWithBarycenter = atoi(DataBuffer.c_str());
                    cout << "/// Merging Tracks using the barycenter= " << " " << fMergeWithBarycenter << " ///" << endl;
                }
                else if (whatToDo.compare(0,23,"MergeWithScalarProduct=") == 0) {
                    RansacParamFile >> DataBuffer;
                    fMergeWithScalarProduct = atoi(DataBuffer.c_str());
                    cout << "/// Merging Tracks using the scalar product= " << " " << fMergeWithScalarProduct << " ///" << endl;
                }
                else{
                    ReadingStatus = false;
                }
            }
        }
    }
}

//////////////////////////////////////////////////
void Ransac::Init(vector<int> v1, vector<int> v2, vector<double> v3, vector<double> v4)
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
vector<NPL::Track> Ransac::SimpleRansac()
{
    if(fVisu==1){
        m_Histo->Clear();
    }
    
    TRandom* Rand=new TRandom();
    double RemainingCharge = fTotalCharge;
    /*cout << "/// Original total charge= " << fTotalCharge << endl;
     cout << "/// Charge Threshold= " << fTotalCharge*fRANSACChargeThreshold << endl;
     cout << "/// vX.size()= " << vX.size() << endl;
     cout << "/// Point Threshold= " << fOriginalCloudSize*fRANSACPointThreshold << endl;*/
    
    int aa=0;
    //while(vX.size() > fOriginalCloudSize*fRANSACPointThreshold){
    //while(RemainingCharge > fTotalCharge*fRANSACChargeThreshold){
    while(RemainingCharge > fTotalCharge*fRANSACChargeThreshold && vX.size() > fOriginalCloudSize*fRANSACPointThreshold){
        aa++;
        
        double minErr=1E20;
        trackX.clear();
        trackY.clear();
        trackZ.clear();
        trackQ.clear();
        
        TVector3 V1, V2;
        std::vector<int> inliners;
        inliners.clear();
        
        fRANSACMaxIteration = 2.0*vX.size();
        for(int i=0;i<fRANSACMaxIteration;i++){
            int vXsize = vX.size();
            int p1=(int)(Rand->Uniform(0,vXsize));
            int p2;
            do p2=(int)(Rand->Uniform(0,vXsize));
            while(p2==p1);
            
            TVector3 Vu;
            Vu.SetXYZ(vX[p2]-vX[p1],vY[p2]-vY[p1],vZ[p2]-vZ[p1]);
            
            std::vector<int> alsoin;
            alsoin.clear();
            for(int p=0;p<vXsize;p++){
                if(p!=p1 && p!=p2 && vQ[p]){
                    TVector3 Va;
                    Va.SetXYZ(vX[p2]-vX[p],vY[p2]-vY[p],vZ[p2]-vZ[p]);
                    TVector3 VaCVu=Va.Cross(Vu);
                    
                    if(VaCVu.Mag()/Vu.Mag()<fRANSACDistance){
                        alsoin.push_back(p);
                    }
                }
            }
            //cout << "****** " << vX.size() << " / " << alsoin.size() << endl;
            
            if(alsoin.size()>fRANSACThreshold){
                TVector3 v1, v2;
                double chi2=Fit3D(vX,vY,vZ,vQ,alsoin,v1,v2);
                if(chi2<minErr){
                    minErr=chi2;
                    V1=v1;
                    V2=v2;
                    inliners=alsoin;
                }
            }
        }
        
        if(inliners.size()==0) break;
        //cout << "iteration " << aa << ":  with also inliners: " << inliners.size() << endl;
        
        TVector3 Vdir = TVector3(V2.x()-V1.x(),V2.y()-V1.y(),V2.z()-V1.z());
        Vdir = Vdir.Unit();
        
        NPL::Track myTrack = NPL::Track();
        
        myTrack.Xm=V1.x();
        myTrack.Ym=V1.y();
        myTrack.Zm=V1.z();
        
        myTrack.Xh=V2.x();
        myTrack.Yh=V2.y();
        myTrack.Zh=V2.z();
        
        
        
        int TrackNumber = vTrack.size();
        TString TrackName = Form("Track%d",TrackNumber);
        
        int inliner_size = inliners.size();
        for(int p=0; p<inliner_size; p++){
            trackX.push_back(vX[inliners[p]]);
            trackY.push_back(vY[inliners[p]]);
            trackZ.push_back(vZ[inliners[p]]);
            trackQ.push_back(vQ[inliners[p]]);
        }
        
        myTrack.SetXPoints(trackX);
        myTrack.SetYPoints(trackY);
        myTrack.SetZPoints(trackZ);
        myTrack.SetQPoints(trackQ);
        
        vTrack.push_back(myTrack);
        
        for(int i=inliner_size-1 ; i>=0 ; i--){
            vQ.erase(vQ.begin()+inliners[i]);
            vX.erase(vX.begin()+inliners[i]);
            vY.erase(vY.begin()+inliners[i]);
            vZ.erase(vZ.begin()+inliners[i]);
        }
        
        RemainingCharge = 0;
        for(unsigned int i =0; i<vQ.size(); i++){
            RemainingCharge += vQ[i];
        }
        //cout << "/// RemainingCharge= " << RemainingCharge << endl;
        //cout << "////****** " << vX.size() << " / " <<  fOriginalCloudSize*fRANSACPointThreshold << endl;
        //cout << "////****** " << RemainingCharge << " / " <<  fTotalCharge*fRANSACChargeThreshold << endl;
    }
    //cout << " //// Number of Tracks found " << vTrack.size() << " ////" << endl;

    if(fMergeWithBarycenter) DoMerge("barycenter");
    if(fMergeWithScalarProduct) DoMerge("scalar");
    
    if(fVisu==1){
        int track_size = vTrack.size();
        for(int t=0; t<track_size; t++){
            h3D = new TGraph2D();
            for(int i=0; i<vTrack[t].GetXPoints().size(); i++){
                h3D->SetPoint(i, vTrack[t].GetXPoints()[i], vTrack[t].GetYPoints()[i], vTrack[t].GetZPoints()[i]);
            }
            m_Histo->Add(h3D);
            
            pl = new TGraph2D();
            
            TVector3 Vdir = TVector3(vTrack[t].Xh-vTrack[t].Xm, vTrack[t].Yh-vTrack[t].Ym, vTrack[t].Zh-vTrack[t].Zm);
            Vdir = Vdir.Unit();
            
            //pl->SetName(TrackName);
            pl->SetPoint(0,vTrack[t].Xm, vTrack[t].Ym, vTrack[t].Zm);
            pl->SetPoint(1,vTrack[t].Xh, vTrack[t].Yh, vTrack[t].Zh);
            pl->SetPoint(2,vTrack[t].Xm + 10*Vdir.x(), vTrack[t].Ym + 10*Vdir.y(), (vTrack[t].Zm + 10*Vdir.z()));
            pl->SetPoint(3,vTrack[t].Xm + 100*Vdir.x(), vTrack[t].Ym + 100*Vdir.y(), (vTrack[t].Zm + 100*Vdir.z()));
            pl->SetPoint(4,vTrack[t].Xm - 100*Vdir.x(), vTrack[t].Ym - 100*Vdir.y(), (vTrack[t].Zm - 100*Vdir.z()));
            pl->SetLineColor(2);
            pl->SetMarkerColor(2);
            pl->SetMarkerStyle(8);
            pl->SetMarkerSize(1);
            pl->SetLineWidth(2);
            m_Histo->Add(pl);
        }
    }
    
    
    
    if(fVisu && vTrack.size()>1 && vTrack.size()<4){
        TSocket* s = NULL;
        char request[64];
        if(m_ServerSocket && m_Monitor){
            
            m_Monitor->ResetInterrupt();
            TList readClients;
            if((m_Monitor->Select(&readClients, NULL, 10)) > 0){
                s = (TSocket*) readClients.Last();
                readClients.RemoveLast();
            }
        }
        
        if(s){
            if (s->IsA() == TServerSocket::Class()) {
                // accept new connection from spy
                TSocket* socket = ((TServerSocket*)s)->Accept();
                m_Monitor->Add(socket,TMonitor::kRead|TMonitor::kWrite);
                m_Sockets->Add(socket);
                cout << "Client Connected" << endl;
                //NPL::SendInformation("NPL::SpectraServer","Accepted new client connection");
            }
            else{
                // we only get string based requests from the spy
                char request[64];
                if (s->Recv(request, sizeof(request)) <= 0) {
                    m_Monitor->Remove(s);
                    m_Sockets->Remove(s);
                    delete s;
                    //return;
                }
                // send requested object back
                static TMessage answer(kMESS_OBJECT);
                answer.Reset();
                
                if (!strcmp(request, "RequestHisto")){
                    answer.WriteObject(m_Histo);
                    s->Send(answer);
                }
                
                else // answer nothing
                    s->Send(answer);
            }
        }
    }
    
    return vTrack;
}

//////////////////////////////////////////////////
bool Ransac::CompareTracksWithBarycenter(NPL::Track track1, NPL::Track track2)
{
    bool ToBeMerged = false;
    
    TVector3 bar1 = track1.GetPointBarycenter();
    TVector3 bar2 = track2.GetPointBarycenter();
    double Y1 = bar1.Y();
    double Z1 = bar1.Z();
    double Y2 = bar2.Y();
    double Z2 = bar2.Z();
    
    double Distance = sqrt(pow(Y2-Y1,2) + pow(Z2-Z1,2));
    
    if(Distance<fMAXBarycenterDistance) ToBeMerged=true;
    
    return ToBeMerged;
}

//////////////////////////////////////////////////
bool Ransac::CompareTracksWithScalarProduct(NPL::Track track1, NPL::Track track2)
{
    bool ToBeMerged = false;
    
    TVector3 v1 = TVector3(track1.Xh-track1.Xm, track1.Zh-track1.Zm, track1.Zh-track1.Zm);
    TVector3 v2 = TVector3(track2.Xh-track2.Xm, track2.Zh-track2.Zm, track2.Zh-track2.Zm);
    
    TVector3 v1_unit = v1.Unit();
    TVector3 v2_unit = v2.Unit();
    
    double ScalarProduct = v1_unit.Dot(v2_unit);
    
    if(abs(ScalarProduct)>cos(fAngleMax*TMath::Pi()/180)) ToBeMerged=true;
    
    return ToBeMerged;
}

//////////////////////////////////////////////////
NPL::Track Ransac::MergeTracks(vector<NPL::Track> tracks)
{
    NPL::Track newTrack = NPL::Track();
    vector<int> newTrackX, newTrackY, newTrackZ, newTrackQ;
    
    double X1=0; double Y1=0; double Z1=0;
    double X2=0; double Y2=0; double Z2=0;
    
    for(int j=0; j<tracks.size(); j++){
        X1 += tracks[j].Xm;
        Y1 += tracks[j].Ym;
        Z1 += tracks[j].Zm;
        
        X2 += tracks[j].Xh;
        Y2 += tracks[j].Yh;
        Z2 += tracks[j].Zh;
        
        for(int k=0; k<tracks[j].GetXPoints().size(); k++){
            newTrackX.push_back(tracks[j].GetXPoints()[k]);
            newTrackY.push_back(tracks[j].GetYPoints()[k]);
            newTrackZ.push_back(tracks[j].GetZPoints()[k]);
        }
        
    }
    X1 = X1/tracks.size();
    Y1 = Y1/tracks.size();
    Z1 = Z1/tracks.size();
    X2 = X2/tracks.size();
    Y2 = Y2/tracks.size();
    Z2 = Z2/tracks.size();
    
    newTrack.Xm=X1;
    newTrack.Ym=Y1;
    newTrack.Zm=Z1;
    
    newTrack.Xh=X2;
    newTrack.Yh=Y2;
    newTrack.Zh=Z2;
    
    newTrack.SetXPoints(newTrackX);
    newTrack.SetYPoints(newTrackY);
    newTrack.SetZPoints(newTrackZ);
    
    return newTrack;
}

//////////////////////////////////////////////////
void Ransac::DoMerge(string method)
{
    vector<NPL::Track> vTrack_new;
    vector<NPL::Track> vTrackToMerge;
    vector<NPL::Track> vTrack_temp;
    vTrack_temp = vTrack;
    NPL::Track newTrack = NPL::Track();
    
    int size = vTrack.size();
    vector<int> id_track;
    for(int i=0; i<size-1; i++){
        int NumberOfTrackToBeMergedWith=0;
        vTrackToMerge.clear();
        
        for(int j=i+1; j<size; j++){
            if(find(id_track.begin(),id_track.end(), j) == id_track.end()){
                if(method=="scalar") {
                    if( CompareTracksWithScalarProduct(vTrack[i],vTrack[j]) ){
                        NumberOfTrackToBeMergedWith++;
                        id_track.push_back(j);
                        vTrackToMerge.push_back(vTrack[j]);
                    }
                }
                else if(method=="barycenter") {
                    if( CompareTracksWithBarycenter(vTrack[i],vTrack[j]) ){
                        NumberOfTrackToBeMergedWith++;
                        id_track.push_back(j);
                        vTrackToMerge.push_back(vTrack[j]);
                    }
                }
                else cout << "Method for merging not known: Problem with token!" << endl;
            }
        }
        if(NumberOfTrackToBeMergedWith>0){
            if(find(id_track.begin(),id_track.end(), i) == id_track.end()) id_track.push_back(i);
            vTrackToMerge.push_back(vTrack[i]);
            
            newTrack = MergeTracks(vTrackToMerge);
            
            vTrack_new.push_back(newTrack);
        }
    }
    
    std::sort(id_track.begin(), id_track.end());
    for(int p=id_track.size()-1; p>=0; p--){
        vTrack_temp.erase(vTrack_temp.begin()+id_track[p]);
    }
    
    for(int p=0; p<vTrack_temp.size(); p++){
        vTrack_new.push_back(vTrack_temp[p]);
    }
    
    vTrack.clear();
    vTrack = vTrack_new;
}


//////////////////////////////////////////////////
/*void Ransac::CheckTracks()
{
    vector<NPL::Track> vTrack_temp;
    vTrack_temp = vTrack;
    
    int size = vTrack.size();
    vector<double> Ybar;
    vector<double> Zbar;
    
    Ybar.clear();
    Zbar.clear();
    for(int i=0; i<size; i++){
        TVector3 bar = vTrack[i].GetPointBarycenter();
        Ybar.push_back(bar.Y());
        Zbar.push_back(bar.Z());
    }
    
    vector<double> Distance;
    vector<int> id_i, id_j;
    for(int i=0; i<size-1; i++){
        for(int j=i+1; j<size; j++){
            id_i.push_back(i);
            id_j.push_back(j);
            Distance.push_back(sqrt(pow(Ybar[i]-Ybar[j],2) + pow(Zbar[i]-Zbar[j],2)));
        }
    }
    
    vector<int> removed_id;
    for(int c=0; c<Distance.size(); c++){
        if(Distance[c]<fMAXBarycenterDistance){
            if(find(removed_id.begin(),removed_id.end(), id_i[c]) == removed_id.end()) removed_id.push_back(id_i[c]);
            if(find(removed_id.begin(),removed_id.end(), id_j[c]) == removed_id.end()) removed_id.push_back(id_j[c]);
        }
    }
    
    std::sort (removed_id.begin(), removed_id.end());
    for(int j=removed_id.size()-1; j>=0; j--){
        vTrack.erase(vTrack.begin()+removed_id[j]);
    }
    
    
    double X1=0; double Y1=0; double Z1=0;
    double X2=0; double Y2=0; double Z2=0;
    
    NPL::Track newTrack = NPL::Track();
    vector<int> newTrackX, newTrackY, newTrackZ, newTrackQ;
    
    if(removed_id.size()>0){
        for(int j=0; j<removed_id.size(); j++){
            X1 += vTrack_temp[removed_id[j]].Xm;
            Y1 += vTrack_temp[removed_id[j]].Ym;
            Z1 += vTrack_temp[removed_id[j]].Zm;
            
            X2 += vTrack_temp[removed_id[j]].Xh;
            Y2 += vTrack_temp[removed_id[j]].Yh;
            Z2 += vTrack_temp[removed_id[j]].Zh;
            
            for(int k=0; k<vTrack_temp[removed_id[j]].GetXPoints().size(); k++){
                newTrackX.push_back(vTrack_temp[removed_id[j]].GetXPoints()[k]);
                newTrackY.push_back(vTrack_temp[removed_id[j]].GetYPoints()[k]);
                newTrackZ.push_back(vTrack_temp[removed_id[j]].GetZPoints()[k]);
            }
            
        }
        X1 = X1/removed_id.size();
        Y1 = Y1/removed_id.size();
        Z1 = Z1/removed_id.size();
        X2 = X2/removed_id.size();
        Y2 = Y2/removed_id.size();
        Z2 = Z2/removed_id.size();
        
        newTrack.Xm=X1;
        newTrack.Ym=Y1;
        newTrack.Zm=Z1;
        
        newTrack.Xh=X2;
        newTrack.Yh=Y2;
        newTrack.Zh=Z2;
        
        newTrack.SetXPoints(newTrackX);
        newTrack.SetYPoints(newTrackY);
        newTrack.SetZPoints(newTrackZ);
        
        vTrack.push_back(newTrack);
    }
}*/

//////////////////////////////////////////////////
vector<double> Ransac::GetChargeOfTracks()
{
    return vTrackCharge;
}

//////////////////////////////////////////////////
vector<double> Ransac::GetTrackLength(double PadSizeX, double PadSizeY, double DriftVelocity)
{
    vector<double> length;
    double Ymin, Ymax;
    double Zmin, Zmax;
    double Xmin = 240;
    double Xmax = 256;
    
    for(unsigned int i=0; i<vTrack.size(); i++){
        double xh = vTrack[i].GetXh()*PadSizeX;
        double xm = vTrack[i].GetXm()*PadSizeX;
        double yh = vTrack[i].GetYh()*PadSizeY;
        double ym = vTrack[i].GetYm()*PadSizeY;
        double zh = vTrack[i].GetZh()*DriftVelocity;
        double zm = vTrack[i].GetZm()*DriftVelocity;
        
        Ymin = yh + (ym - yh)*(Xmin-xh)/(xm-xh);
        Ymax = yh + (ym - yh)*(Xmax-xh)/(xm-xh);
        
        Zmin = zh + (zm - zh)*(Xmin-xh)/(xm-xh);
        Zmax = zh + (zm - zh)*(Xmax-xh)/(xm-xh);
        
        length.push_back(sqrt( pow(Xmax-Xmin,2) + pow(Ymax-Ymin,2) + pow(Zmax-Zmin,2) ));
    }
    
    return length;
}


//////////////////////////////////////////////////
void Ransac::Reset()
{
    vline.clear();
    vX.clear();
    vY.clear();
    vZ.clear();
    vQ.clear();
    vTrack.clear();
    vTrackCharge.clear();
}

//////////////////////////////////////////////////
double Ransac::Fit3D(vector<int> X, vector<int> Y, vector<double> Z, vector<double> Charge, vector<int> inliners, TVector3& V1, TVector3& V2)
{
    // adapted from: http://fr.scribd.com/doc/31477970/Regressions-et-trajectoires-3D
    
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
    double total_charge=0;
    Sxx=Syy=Szz=Sxy=Sxz=Syz=0.;
    
    for (auto i : inliners)
    {
        if(X[i]>119)total_charge+=Charge[i];
        Q+=Charge[i]/10.;
        Xm+=X[i]*Charge[i]/10.;
        Ym+=Y[i]*Charge[i]/10.;
        Zm+=Z[i]*Charge[i]/10.;
        Sxx+=X[i]*X[i]*Charge[i]/10.;
        Syy+=Y[i]*Y[i]*Charge[i]/10.;
        Szz+=Z[i]*Z[i]*Charge[i]/10.;
        Sxy+=X[i]*Y[i]*Charge[i]/10.;
        Sxz+=X[i]*Z[i]*Charge[i]/10.;
        Syz+=Y[i]*Z[i]*Charge[i]/10.;
    }
    vTrackCharge.push_back(total_charge);
    
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
    
    V1.SetXYZ(Xm,Ym,Zm);
    V2.SetXYZ(Xh,Yh,Zh);
    
    return(fabs(dm2/Q));
}
