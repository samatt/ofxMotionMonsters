//
//  ofPolyline3D.cpp
//  ofxMotionMonsters
//
//  Created by Mike Allison on 11/18/13.
//
//

#include "ofPolyline3D.h"

ofPolyline3D::ofPolyline3D() : mRotation(ofMatrix4x4::newIdentityMatrix()), mWorldCenter(ofVec3f::zero()) {
}
ofPolyline3D::ofPolyline3D(ofPolyline &old) : mRotation(ofMatrix4x4::newIdentityMatrix()), mWorldCenter(ofVec3f::zero()) {
    getVertices() = old.getVertices();
}

ofVec3f ofPolyline3D::getWorldLocation(int &i){
    ofVec3f newVert = getVertices()[i]*mRotation;
    newVert+=mWorldCenter;
    return newVert;
}

ofVec3f ofPolyline3D::getLocalLocation(int &i){
    return getVertices()[i];
}

ofPolyline3D ofPolyline3D::convertToPolyline3D(ofPolyline &p, ofPolyline3D &newPoly){
    
    newPoly.getVertices() = p.getVertices();
    
    return newPoly;
}

void ofPolyline3D::draw(){
    ofPushMatrix();
    ofTranslate(mWorldCenter);
    ofVec3f axis;
    float angle;
    mRotation.getRotate().getRotate(angle, axis);
    ofRotate(angle, axis.x, axis.y, axis.z);
    ofPolyline::draw();
    ofPopMatrix();
}
