//
//  TracerModel.cpp
//  SeekModel
//
//  Created by Mike Allison on 11/15/13.
//
//

#include "TracerModel.h"

TracerModel::TracerModel(): mIsSet(false)
{
}

void TracerModel::set( Stencil* stencil, const int &numTracers ){
    
    mContours.resize(0);
    mTargets.resize(0);

    
        for (int i=0;i<numTracers;i++) {
            int arrPos = int(ofMap(i, 0, numTracers, 0, stencil->getSlices()[0].size()));
            ofVec3f target = ofVec3f(stencil->getSlices()[0].getVertices()[arrPos].x, stencil->getSlices()[0].getVertices()[arrPos].y, stencil->getSlices()[0].getVertices()[arrPos].z );
            //target.z += j*100;
            mTargets.push_back(target);
            mBase = stencil->getSlices()[stencil->getSlices().size()-1].getResampledByCount(numTracers);
        }
    
    for (int i=0;i<mTargets.size();i++) {
        mTracers.push_back(new Tracer( mTargets[i],mBase.getVertices()[i], i));
    }
    
}

void TracerModel::update(){
 
    for(int i=0;i<mTracers.size();i++){
       if (!mTracers[i]->hasArrived()){
        mTracers[i]->applyBehaviors(mTracers);
        mTracers[i]->update();
        }
    }
    
    //if(allArrived())cout<<"ALL TRACERS ARRIVED"<<endl;
    
}

bool TracerModel::allArrived(){
    bool check = true;
    for(int i = 0;i<mTracers.size();i++){
        check = mTracers[i]->hasArrived();
    }
    return check;
}

void TracerModel::draw(){
    for(int i=0;i<mTracers.size();i++) mTracers[i]->draw();
}

void TracerModel::triangluation(ofMesh &mesh){
    
    vector<ofVec3f> masterPoints;
    ofxDelaunay* tri = new ofxDelaunay();
    
    for(int i=0;i<mTracers.size();i++){
        for(int j =0; j<mTracers[i]->mPath.getVertices().size();j++){
            masterPoints.push_back(ofVec3f(mTracers[i]->mPath.getVertices()[j]));
        }
    }
    
    tri->addPoints(masterPoints);
    tri->triangulate();
    mesh = tri->triangleMesh;
    delete tri;
}

void TracerModel::reset(){
    mTracers.clear();
}

Stencil::Stencil(){
    slices.resize(0);
}

ofPolyline Stencil::getSlice( int &ID ){
    return slices[ID];
}

bool Stencil::hasSlices(){
    if(slices.size()>0) return true; else return false;
}

void Stencil::setSlice(ofPolyline poly){
    slices.push_back(poly);
}

void Stencil::removeSlice(int &ID){
    vector<ofPolyline>::iterator it = slices.begin()+ID;
    it = slices.erase(it);
}

void Stencil::draw(){
    for(int i=0;i<slices.size();i++){
        slices[i].draw();
    }
}

vector<ofPolyline> Stencil::getSlices(){
    return slices;
}


