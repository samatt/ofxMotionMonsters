#pragma once

#include "ofMain.h"
#include "ofxMarchingCubes.h"
#include "ofxDelaunay.h"
#include "ofxObjLoader.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void updateTraingulation();
    void updateMarchingCubes();
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    void saveToObj();
    void loadImagesFromPath(string filePath);
    void updateResolution();
    
    ofEasyCam camera;
    ofxMarchingCubes mc;
    
    vector<ofImage> imgs;
    float scale;
    ofxDelaunay triangulation;
    ofxUISuperCanvas *gui;
    
    bool bInvertSelection;
    bool bTriangulate;
    bool bWireframe;
    bool drawGrid;
    
    
    int triResX;
    int triResY;
    int triResZ;
    int scaleX, scaleY, scaleZ;
    int cubeResolution;
    
    int sampleImgWidth;
    int sampleImgHeight;
    
    ofShader normalShader;
    
    //duration integration
};
