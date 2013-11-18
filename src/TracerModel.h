#pragma once

#include "ofMain.h"
#include "Tracer.h"
#include "ofxDelaunay.h"

class Stencil{
public:
    
    Stencil();
    void setSlice( ofPolyline poly );
    void removeSlice( int &ID );
    vector<ofPolyline> getSlices();
    bool hasSlices();
    void draw();
    ofPolyline getSlice( int &ID );
    
private:
    vector<ofPolyline> slices;
};

class TracerModel{
public:
    
    TracerModel();
    
    void update();
    void draw();
    bool allArrived();
    void set( Stencil* stencil, const int &numTracers);
    void triangluation(ofMesh &mesh);
    void reset();
    
private:
    
    vector<Tracer*> mTracers;
    vector<ofVec3f> mTargets;
    ofPolyline mBase;
    vector<vector<ofVec3f> > mContours;
    bool mIsSet;
    ofPolyline base;

};


static ofPolyline mergePolylines( ofPolyline slice1, ofPolyline slice2 );


