#pragma once

#include "ofMain.h"
#include "ofPolyline3D.h"
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>


class Tracer {
public:
    Tracer( ofVec3f &_loc,  ofVec3f _target, int &_id );
    void update();
    void applyBehaviors(  vector<Tracer*> &tracers );
    void updateTarget( const ofVec3f &newTarget );
    void draw();
    bool hasArrived();
    void addForce( const ofVec3f &force);
    void findNext( vector<ofVec3f>& targets);
    ofVec3f getLocation();
    ofVec3f getVel();
    void setSeperation(float &sep);

    int mId;
    int mCurTargetID;
    ofPolyline3D mPath;
    float mSeperation;

private:
    
    ofVec3f seek( const ofVec3f &target );
    ofVec3f separate(  vector<Tracer*> &tracers );
    ofVec3f mLocation;
    ofVec3f mStartLoc;
    ofVec3f mVelocity;
    ofVec3f mAcceleration;
    float mR;
    float mMaxforce = 5;    // Maximum steering force
    float mMaxspeed = .5;    // Maximum speed
    ofVec3f curCenter;
    ofVec3f mStartLocation;
    ofVec3f mCurrentTarget;
    
};

enum ObstacleType {
    ATTRACTOR, REPELLER, NOISEDRAG
};

class Obstacle {
public:
    
    Obstacle();
    Obstacle( ofVec3f &_location, float &_strength, float &_radius,  ObstacleType _type );
    void setStrength( float &_s );
    void setLocation( ofVec3f &_l );
    void setType( ObstacleType _t );
    void obstruct( vector<Tracer*> &tracers );
    void setRadius(float &_r);
    void draw();
    void drawSelection();
    float   mRadius;
    ofVec3f mLocation;
    float   mStrength;
    ObstacleType mType;
    bool    bEnabled;
    
    
private:
    
    ofVec3f repel(Tracer* &t);
    ofVec3f attract(Tracer* &t);
    ofVec3f drag(Tracer* &t);
    ofFloatColor mColor;
    
};