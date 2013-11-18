#pragma once

#include "ofMain.h"
#include "ofxMarchingCubes.h"
#include "ofxDelaunay.h"
#include "ofxObjLoader.h"
#include "ofxUI.h"
#include "TracerModel.h"
#include "ofxCv.h"

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
        void setupGUI();
        void setupGUIMC();
        void setupGUITracers();
        void guiEvent(ofxUIEventArgs &e);
        void saveToObj();
        void loadImagesFromPath(string filePath);
        void updateMarchingCubes();
        void updateResolution();
        void createBase();
    
        vector<ofPolyline> getImageContours( ofImage &image );
    
        ofEasyCam camera;
        ofVec3f mCameraPos;
        ofxMarchingCubes mc;

        vector<ofImage> imgs;
        float scale;
        ofxDelaunay triangulation;
        ofxUISuperCanvas *gui;
        ofxUISuperCanvas *guiMC;
        ofxUISuperCanvas *guiT;

        TracerModel          mModel;
        ofxCv::ContourFinder mContourFinder;
        bool                 mShowStencils, mShowTracers;
        vector<vector<ofPolyline> >  mContours;
        Stencil*             mStencil;
    
        bool bInvertSelection;
        bool bTriangulate;
        bool bWireframe;
        bool drawGrid;
        bool mGenContours;
        bool mUseTracers;
        bool mSaveObj;
        bool mLoadImages;
        bool mUseMC;
        bool mAllowSliceComp;
        bool mShowContours;
        bool mAddContour;
        bool mShowSelection;
        bool mShowTracerMesh;
    
        ofVec3f mContInd;
        ofVec3f mContIndSet;
    
        ofPolyline* curSel;
    
        int triResX;
        int triResY;
        int triResZ;
        int scaleX, scaleY, scaleZ;
        int cubeResolution;
    
        int mRunTracers;
        int mNumTracers;
    
        int sampleImgWidth;
        int sampleImgHeight;
    
        float tX, tY, tZ, tS;
    
        ofShader normalShader;
    
        ofMatrix4x4 mTransformMatrix;
        ofMesh mTracerMesh;
    
        //master controls
    
    
    ofQuaternion mMasterRotation;
    ofVec2f mLastMouse;
    float mRotDampen;
    bool mEnableSelection;

    
    //duration integration
};

