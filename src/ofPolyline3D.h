#pragma once

#include "ofMain.h"

class ofPolyline3D : public ofPolyline {
public:
    static ofPolyline3D convertToPolyline3D(ofPolyline &p, ofPolyline3D &newPoly);

    ofPolyline3D();
    ofPolyline3D(ofPolyline &old);
    void draw();
    ofVec3f getWorldLocation(int &i);
    ofVec3f getLocalLocation(int &i);
    ofVec3f get3DCenter();
    ofMatrix4x4 mRotation;
    ofVec3f mWorldCenter;

};


