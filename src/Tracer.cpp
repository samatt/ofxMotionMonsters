//
//  Tracer.cpp
//  SeekModel
//
//  Created by Mike Allison on 11/15/13.
//
//

#include "Tracer.h"


Tracer::Tracer( ofVec3f &_loc, ofVec3f _target, int &_id )
{
    mStartLocation = _loc;
    mLocation = _loc;
    mCurrentTarget = _target;
    mId = _id;
    mMaxforce = 5.f;
    mMaxspeed = .5f;
    mCurTargetID = 2;
    mR = 12;
    mVelocity = ofVec3f::zero();
    mAcceleration = ofVec3f::zero();
}

void Tracer::update(){
    if (!hasArrived()) {
        mVelocity+=mAcceleration;
        mVelocity.limit(mMaxspeed);
        mLocation+=mVelocity;
        mAcceleration*=0;
        if (ofGetFrameNum()%10==0)mPath.addVertex(mLocation);
    }
}

void Tracer::addForce( const ofVec3f &force){
    mAcceleration+=force;
}

void Tracer::applyBehaviors( vector<Tracer*> &tracers){
    ofVec3f separateForce = separate(tracers);
    ofVec3f seekForce = seek(mCurrentTarget);
    separateForce*=2;
    addForce(separateForce);
    addForce(seekForce);
}

ofVec3f Tracer::seek(const ofVec3f& target) {
    ofVec3f desired = target - mLocation;
    desired.normalize();
    desired *= mMaxspeed;
    ofVec3f steer = desired - mVelocity;
    steer.limit(mMaxforce);
    return steer;
}

ofVec3f Tracer::separate ( vector<Tracer*>& tracers) {
    float desiredseparation = 5;
    ofVec3f sum;
    int count = 0;
    for (int i=0;i<tracers.size();i++){
        float d = mLocation.distance(tracers[i]->mLocation);
        if ((d > 0) && (d < desiredseparation)) {
            ofVec3f diff = mLocation - tracers[i]->mLocation;
            diff.normalize();
            diff/=d;
            sum+=diff;
            count++;
            
        }
    }
    
    if (count > 0) {
        sum/=count;
        sum.normalize();
        sum*=mMaxspeed;
        sum -= mVelocity;
        sum.limit(mMaxforce);
    }
    return sum;
}

bool Tracer::hasArrived() {
    
    ofVec3f loc = ofVec3f(mLocation);
    ofVec3f dest = ofVec3f(mCurrentTarget);
    
    if(loc.distance(dest)<3)  return true; else return false;
}

void Tracer::updateTarget(const ofVec3f &newTarget){
    mCurrentTarget = newTarget;
}

void Tracer::findNext( vector<ofVec3f>& targets) {
    ofVec3f bestMatch;
        float smallest = 10000000;
        for (auto t : targets) {
          float d = mLocation.distance(t);
          if(d<smallest){
            smallest = d;
            bestMatch = t;
          }
        }
    updateTarget(bestMatch);
}

void Tracer::draw() {
   // if(mId == 0)cout<<mCurrentTarget<<endl;
    ofSetColor(200, 175);
    mPath.draw();
    ofSetColor(255, 0, 0);
    ofRect(mCurrentTarget, 1, 1);
    ofSetColor(0, 255, 0);
    ofRect(mLocation, 1, 1);
    
    
}

ofVec3f Tracer::getLocation(){
    return mLocation;
}
ofVec3f Tracer::getVel(){
    return mVelocity;
}



Obstacle::Obstacle( ofVec3f &_location, float &_strength, float &_radius,  ObstacleType _type  )
{
    mLocation = _location;
    mRadius = _radius;
    mStrength = _strength;
    mType = _type;
}

void Obstacle::setLocation(ofVec3f &_l){
    mLocation = _l;
}

void Obstacle::setStrength(float &_s){
   mStrength = _s;
}
void Obstacle::setRadius(float &_r){
    mRadius = _r;
}
void Obstacle::setType(ObstacleType _t){
    mType = _t;
}
ofVec3f Obstacle::repel(Tracer *&t){
    ofVec3f diff = mLocation - t->getLocation();
    float d = diff.length();
    d = ofClamp(d, 5.f, mRadius);
    float force = mStrength/d*d;
    diff.normalize();
    diff.scale(-force);
    return diff;
}
ofVec3f Obstacle::attract(Tracer *&t){
    ofVec3f diff = mLocation - t->getLocation();
    float d = diff.length();
    d = ofClamp(d, 5.f, mRadius);
    float force = mStrength/d*d;
    diff.normalize();
    diff.scale(force);
    return diff;
}
ofVec3f Obstacle::drag(Tracer *&t){
    float speed = t->getVel().length();
    float dragMagnitude = mStrength * speed * speed;
    ofVec3f dragForce = ofVec3f(t->getVel());
    dragForce.scale(-1);
    dragForce.normalize();
    dragForce.scale(dragMagnitude);
    return dragForce;
}

void Obstacle::obstruct(vector<Tracer *> &tracers){
    
    for(int i=0;i<tracers.size();i++){
        
        switch (mType) {
            case REPELLER:
                tracers[i]->addForce(repel(tracers[i]));
                break;
            case RESISTANCE:
                tracers[i]->addForce(drag(tracers[i]));
                break;
            case ATTRACTOR:
                tracers[i]->addForce(attract(tracers[i]));
                break;
            default:
                break;
        }
        
    }
    
}
