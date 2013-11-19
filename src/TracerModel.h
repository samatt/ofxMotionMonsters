#pragma once

#include "ofMain.h"
#include "Tracer.h"
#include "ofxDelaunay.h"

class Stencil{
public:
    
    Stencil();
    void setSlice( ofPolyline3D poly );
    void removeSlice( int &ID );
    vector<ofPolyline3D> getSlices();
    bool hasSlices();
    void draw();
    void reset();
    ofPolyline3D getSlice( int &ID );
    
private:
    vector<ofPolyline3D> slices;
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
    void setSeperation(float &sep);
    vector<Tracer*> mTracers;

private:
    
    vector<ofVec3f> mTargets;
    ofPolyline3D mBase;
    vector<vector<ofVec3f> > mContours;
    bool mIsSet;
    ofPolyline3D base;

};


static ofPolyline3D mergePolylines( ofPolyline3D slice1, ofPolyline3D slice2 );


