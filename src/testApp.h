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
    
        vector<ofPolyline3D> getImageContours( ofImage &image );
    
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
        vector<vector<ofPolyline3D> >  mContours;
        vector<Obstacle>     mObstacles;
        Stencil*             mStencil;
    
        bool bInvertSelection;
        bool bTriangulate;
        bool bMarchingCubes;
        bool bWireframe;
        bool bContours;
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
        bool bRotSel;
    
        ofVec3f mContInd;
        ofVec3f mContIndSet;
    
        ofPolyline3D* curSel;
    
        int triResX;
        int triResY;
        int triResZ;
        int scaleX, scaleY, scaleZ;
        int cubeResolution;

        float mTracerSep;
    
        int mRunTracers;
        int mNumTracers;
    
        int sampleImgWidth;
        int sampleImgHeight;
    
        float tX, tY, tZ, tS;
        float prevX, prevY, prevZ, prevS;
        float mObsX, mObsY, mObsZ, mObsStrength, mObsRadius;
        Obstacle* curObs;
        float mObsChoose;
        bool bHasObs;
        bool bShowObs;
    
        float isoValue;
    
        ofShader shader;
    
        ofMatrix4x4 mTransformMatrix;
        ofMesh mTracerMesh;
    
        //master controls
    
    
    ofQuaternion mMasterRotation;
    ofVec2f mLastMouse;
    float mRotDampen;
    bool mEnableSelection;

    
    //duration integration
};

