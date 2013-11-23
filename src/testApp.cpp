#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);
    
    imgs.resize(0);
	drawGrid = false;
    bWireframe = false;
    mGenContours = false;
    mShowStencils = true;
    mShowTracers = false;
    mUseTracers = false;
    mSaveObj = false;
    mLoadImages = false;
    mUseMC = false;
    bTriangulate = false;
    bContours = false;
    mAllowSliceComp = false;
    mShowContours = true;
    mAddContour = false;
    mRunTracers = false;
    mShowSelection = false;
    mEnableSelection = false;
    mShowTracerMesh = false;
    bRotSel = false;
    isoValue = 1;
    
    bInvertSelection = false;
    scale = 1;
	mc.setSmoothing( false );
    
    
    mNumTracers = 100;
    
   // for(int i=0; i<13; i++){
        ofImage img;
        img.loadImage("Images/face/"+ofToString(0)+".png");
        imgs.push_back(img);
        
    //}
    mc.setup();
    cubeResolution = 32;
	mc.scale.set( 500, 250, 500 );
    
    
    bTriangulate = false;
    triResX=2;
    triResY=2;
    triResZ=100;
    
    mObsX = 0;
    mObsY = 0;
    mObsZ = 0;
    mObsStrength = 0;
    mObsRadius = 0;
    mObsChoose = 0;
    bHasObs = false;
    bShowObs = true;
    
    tX = 0;
    tY = 0;
    tZ = 0;
    tS = 0;
    
    prevX = 0;
    prevY = 0;
    prevZ = 0;
    
    mRotDampen = .2;
    
    shader.load("shaders/facingRatio");
    
    setupGUI();
    setupGUIMC();
    setupGUITracers();
    
    guiMC->toggleVisible();
    guiT->toggleVisible();
    
    //mCameraPos = camera.getScale();
    
    mContInd = ofVec3f::zero();
    mContIndSet = ofVec3f::zero();
    
    
    mStencil = new Stencil();
    ofEnableAlphaBlending();
    
    camera.enableMouseInput();
    camera.disableMouseInput();
    //    ofLog(OF_LOG_VERBOSE);
    
}
void testApp::updateResolution(){
    if(imgs.size()> 0){
        float x= imgs[0].getWidth()/cubeResolution;
        float y = imgs[0].getHeight()/cubeResolution;
        mc.setResolution(x,y,cubeResolution);
    }
    else{
        cout<<"NO IMAGES LOADED!"<<endl;
    }
    
    
}
void testApp::setupGUIMC(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    guiMC = new ofxUISuperCanvas("MARCHING CUBES", length+xInit+2, 0, length+xInit, ofGetHeight()/2);
    guiMC->addSpacer();
    guiMC->addToggle("INVERT SELECTION", &bInvertSelection);
    guiMC->addToggle("DRAW MC", &bMarchingCubes);
    guiMC->addToggle("DRAW TRIANGULATION", &bTriangulate);
    guiMC->addToggle("DRAW WIREFRAME", &bWireframe);
    guiMC->addSpacer();
    guiMC->addLabel("TRIANGULATION PARAMS");
    guiMC->addSpacer();
    guiMC->addIntSlider("RES X", 1, ofGetWidth(), &triResX);
    guiMC->addIntSlider("RES Y", 1, ofGetHeight(), &triResY);
    guiMC->addIntSlider("RES Z", 10, 500, &triResZ);
    guiMC->addSpacer();
    guiMC->addLabel("MC PARAMS");
    guiMC->addIntSlider("CUBE RESOLUTION", 20, 500, &cubeResolution);
    guiMC->addToggle("UPDATE MC", false);
    guiMC->addSlider("ISO VALUE",0.1,10.0,&isoValue);
    guiMC->addSpacer();
    guiMC->addSlider("MC THRESHOLD", 0.1, 1, &mc.threshold);
    guiMC->autoSizeToFitWidgets();
    
    
    ofAddListener(guiMC->newGUIEvent, this, &testApp::guiEvent);
    
}

void testApp::setupGUITracers(){
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    guiT = new ofxUISuperCanvas("TRACERS", length*2+xInit*2+4, 0, length+xInit, ofGetHeight()/2);
    guiT->addSpacer();
    guiT->addToggle("SHOW TRACERS", &mShowTracers);
    guiT->addIntSlider("NUM TRACERS", 0, 1000, &mNumTracers);
    guiT->addSlider("SEPERATION", 0, 100, &mTracerSep);
    guiT->addToggle("RUN TRACERS", &mRunTracers);
    guiT->addSpacer();
    guiT->addButton("MAKE MESH", false);
    guiT->addSpacer();
    guiT->addButton("RESET", false);
    guiT->addSpacer();
    guiT->addLabel("OBSTACLES");
    guiT->addSpacer();
    guiT->addButton("ADD OBS", false);
    guiT->addSlider("CHOOSE OBS", 0, 1, &mObsChoose);
    guiT->addSlider("OBS-X", -ofGetWidth()/2, ofGetWidth()/2, &mObsX);
    guiT->addSlider("OBS-Y", -ofGetHeight()/2, ofGetHeight()/2, &mObsY);
    guiT->addSlider("OBS-Z", -1000, 1000, &mObsZ);
    guiT->addSlider("OBS-STRENGTH", 0, 1, &mObsStrength);
    guiT->addSlider("OBS-RADIUS", 0, 1000, &mObsRadius);
    vector<string> items;
    items.push_back("ATTRACTOR");
    items.push_back("REPELLER");
    items.push_back("NOISEDRAG");
    guiT->addDropDownList("CHOOSE OBS TYPE", items, 200);
    guiT->addToggle("ENABLE OBS", false);
    guiT->addToggle("HIDE OBS", &bShowObs);
    guiT->autoSizeToFitWidgets();

    
    
    
    ofAddListener(guiT->newGUIEvent, this, &testApp::guiEvent);
    
}

void testApp::setupGUI(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    gui = new ofxUISuperCanvas("MOTION MONSTERS", 0,0,length+xInit, ofGetHeight());
    gui->addSpacer();
    gui->addToggle("LOAD IMGS",&mLoadImages);
    gui->addToggle("GENERATE CONTOURS", &mGenContours);
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("CREATE STENCIL", OFX_UI_FONT_MEDIUM));
    gui->addToggle("SHOW CONTOURS", &mShowContours);
    gui->addToggle("SHOW STENCIL", &mShowStencils);
    gui->addToggle("SHOW SELECTION", &mShowSelection);
    gui->addToggle("CREATE BASES", false);
    gui->addWidgetDown(new ofxUI2DPad("CHOOSE", ofPoint(0,length-xInit), ofPoint(0,120), &mContInd, length-xInit,120));
    gui->addSlider("X", -ofGetWidth()/2, ofGetWidth()/2, &tX);
    gui->addSlider("Y", -ofGetHeight()/2, ofGetHeight()/2, &tY);
    gui->addSlider("Z", -1000, 1000, &tZ);
    gui->addSlider("SCALE", 1 , 10, &tS);
    gui->addToggle("ADD SELECITON", &mAddContour);
    gui->addSpacer();
    gui->addButton("RESET STENCIL", false);
    gui->addSpacer();
    gui->addToggle("USE TRACERS", &mUseTracers);
    gui->addToggle("USE MC", &mUseMC);
    gui->addSpacer();
    gui->addLabel("HOLD q TO ENABLE MOUSE");
    gui->addLabel("CAMERA CONTROLS");
    gui->addSpacer();
    gui->addToggle("NORMAL SHADER", false);
    gui->addToggle("FR SHADER", false);
    gui->addSpacer();
    gui->addToggle("SAVE OBJ",&mSaveObj);

    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    
}


void testApp::createBase(){
    
    vector<ofPolyline3D> tmp;
    ofPolyline3D base;
    for(int i=0;i<100;i++){
        float angle = ofMap(i,0,100,0,TWO_PI);
        base.addVertex(ofVec3f(cos(angle), sin(angle), 0));
    }
    tmp.push_back(base);
    
    ofPolyline3D base2;
    
    for(int i=0;i<100;i++){
        float angle = ofMap(i,0,100,0,TWO_PI);
        base2.addVertex(ofVec3f(100*cos(angle), 100*sin(angle), 0));
    }
    base2.close();
    tmp.push_back(base2);
    
    mContours.push_back(tmp);
    
}

///--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    cout<<name<<endl;
    
    if(name == "TRIANGULATE"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bTriangulate = t->getValue();
        if(bTriangulate){
            updateTraingulation();
        }
        
    }
    if(name == "DRAW MC"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bMarchingCubes = t->getValue();
        updateResolution();
        updateMarchingCubes();
    }
    
    if(name == "RESET STENCIL"){
        mStencil->reset();
    }
    if(name == "UPDATE MC"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        t->setValue(false);
        if(bMarchingCubes){
            updateResolution();
            updateMarchingCubes();
        }
        else{
            cout<<"Not in marching cubes mode"<<endl;
        }
        
    }
    if(name == "DRAW WIREFRAME"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bWireframe = t->getValue();
    }
    if(name == "SAVE OBJ"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        if (t->getValue()) {
            saveToObj();
            t->setValue(false);
        }
    }
    
    if(name=="ATTRACTOR")curObs->setType(ATTRACTOR);
    if(name=="REPELLER")curObs->setType(REPELLER);
    if(name=="NOISEDRAG")curObs->setType(NOISEDRAG);
    
    if(name=="ENABLE OBS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        curObs->bEnabled = t->getValue();
    }
    
    if(name=="MAKE MESH"){
        mModel.triangluation(mTracerMesh);
        mShowTracerMesh = true;
    }
    
    if(name == "CREATE BASES"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        createBase();
        t->setValue(false);
    }
    
    if(name == "GENERATE CONTOURS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bContours = t->getValue();
        if (t->getValue()) {
            for(int i=0;i<imgs.size();i++){
                mContours.push_back(getImageContours(imgs[i]));
            }
            cout<<mContours.size()<<endl;
        }
    }
    if(name == "NORMAL SHADER"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        t->setValue(false);
        shader.load("shaders/normalShader");
    }
    if(name == "FR SHADER"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        t->setValue(false);
        shader.load("shaders/facingRatio");
    }
    
    if(name == "USE TRACERS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        guiT->toggleVisible();
        mShowTracers = true;
        mShowStencils = true;
    }
    
    if(name == "USE MC"){
        guiMC->toggleVisible();
    }
    
    if(name == "SHOW TRACERS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        mShowTracers = t->getValue();
    }
    
    if(name == "SHOW STENCIL"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        mShowStencils = t->getValue();
    }
    
    if(name == "SHOW CONTOURS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        mShowContours = t->getValue();
    }
    
    if(name == "RUN TRACERS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        mRunTracers = t->getValue();
        if(t->getValue())
            mModel.set(mStencil, mNumTracers);
    }
    
    if(name=="RESET"){
        ofxUIButton* b  = (ofxUIButton*) e.widget;
        if(b->getValue()){
            mModel.reset();
            mTracerMesh.clear();
            mShowTracerMesh = false;
        }
    }
    
    
    if(name == "ZOOM")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        mCameraPos.z = slider->getScaledValue();
    }
    
    if(name == "ADD OBS"){
        ofxUIButton* b  = (ofxUIButton*) e.widget;
        if(b->getValue()){
            mObstacles.push_back(Obstacle());
            cout<<mObstacles.size()<<endl;
            bHasObs = true;
        }
    }
    
    if(name == "X")
    {
        curSel->mWorldCenter.x = tX; //+= slider->getIncrement();
    }
    if(name == "Y")
    {
        curSel->mWorldCenter.y = tY; //+= slider->getIncrement();
    }
    
    if(name == "Z")
    {
        curSel->mWorldCenter.z = tZ; //+= slider->getIncrement();
    }
    
    if(name == "OBS-X")
    {
        curObs->mLocation.x = mObsX; //+= slider->getIncrement();
    }
    if(name == "OBS-Y")
    {
        curObs->mLocation.y = mObsY; //+= slider->getIncrement();
    }
    
    if(name == "OBS-Z")
    {
        curObs->mLocation.z = mObsZ; //+= slider->getIncrement();
    }
    
    if(name == "OBS-STRENGTH")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        // mObsStrength = slider->getValue();
        curObs->setStrength(mObsStrength); //+= slider->getIncrement();
    }
    
    if(name == "OBS-RADIUS")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        // mObsRadius = slider->getValue();
        curObs->setRadius(mObsRadius);
        
    }
    
    if(name == "SEPERATION")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        // mTracerSep = slider->getValue();
        mModel.setSeperation(mTracerSep);
        
    }
    
    if(name == "SCALE")
    {
        //		ofxUISlider *slider = (ofxUISlider *) e.widget;
        //        for(int i=0;i<curSel->getVertices().size();i++){
        //            curSel->getVertices()[i] *= slider->getScaledValue();
        //        }
    }
    
    if(name == "UPDATE MARCHING CUBES"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bMarchingCubes = t->getValue();
        if(t->getValue()){
            updateResolution();
            updateMarchingCubes();
            t->setValue(false);
        }
        
    }
    
    if(name == "INVERT SELECTION"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bInvertSelection = t->getValue();
    }
    
    if(name == "LOAD IMGS"){
        cout<<"Im in load images"<<endl;
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        if (t->getValue()) {
            ofFileDialogResult result = ofSystemLoadDialog("Load Images From Folder", true, "Images/");
            
            if(result.bSuccess && result.fileName.length())
            {
                loadImagesFromPath(result.filePath);
            }
            
            mLoadImages = false;
            t->setValue(false);
            
        }
        
    }
}

//--------------------------------------------------------------
void testApp::loadImagesFromPath(string filePath){
    cout<<filePath<<endl;
    imgs.clear();
    
    ofDirectory dir(filePath);
    dir.listDir();
    if(dir.exists()){
        int size = dir.size();
        vector<ofFile>files= dir.getFiles();
        cout<<size<<endl;
        
        for(int i=0; i<files.size(); i++){
            cout<<files[i].getFileName()<<endl;
            ofImage img;
            img.loadImage(filePath + "/"+files[i].getFileName());
            imgs.push_back(img);
        }
    }
    else{
        cout<<"directory doesnt exist"<<endl;
    }
    
    
    updateResolution();
    
}
//--------------------------------------------------------------
void testApp::update(){
    
    
    if(mRunTracers){
        cout<<"run"<<endl;
        mModel.update();
        if(bHasObs){
            for(int i=0;i<mObstacles.size();i++){
                if(mObstacles[i].bEnabled)
                    mObstacles[i].obstruct(mModel.mTracers);
            }
        }
    }
    
    if(mShowSelection){
        int x = (int)ofMap(mContInd.x,0,247,0, mContours.size()-1);
        int y = (int)ofMap(mContInd.y, 0, 120 ,0, mContours[x].size()-1);
        mContIndSet.x = x;
        mContIndSet.y = y;
        curSel = &mContours[x][y];
    }
    
    if(bHasObs){
        int index = (int)ofMap(mObsChoose,0,1,0,mObstacles.size()-1);
        curObs = &mObstacles[index];
    }
    
    if(mAddContour){
        ofPolyline p = mContours[mContIndSet.x][mContIndSet.y].getSmoothed(1);
        ofPolyline3D n1 = mContours[mContIndSet.x][mContIndSet.y];
        ofPolyline3D n = ofPolyline3D::convertToPolyline3D(p, n1);
        mStencil->setSlice(n);
        mAddContour = false;
    }
    
    
    if(mUseMC){
        mc.update();
    }
    
    
}
//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(64), ofColor(0));
    ofSetColor(255, 255, 255);
    camera.begin();
    
    if(mUseMC){
        if(bMarchingCubes){
            shader.begin();
            bWireframe?	mc.drawWireframe() : mc.draw();
            //draw the voxel grid           if(drawGrid)mc.drawGrid();
            shader.end();
        }
        
        if(bTriangulate){
            ofPushStyle();
            shader.begin();
            bWireframe?  ofNoFill(): ofFill();
            triangulation.draw();
            shader.end();
            ofPopStyle();
        }
    }
    
    
    if(mShowSelection){
        ofSetColor(255, 0, 0, 200);
        ofPolyline p = curSel->getSmoothed(1);
        ofPolyline3D n1 = *curSel;
        ofPolyline3D n = ofPolyline3D::convertToPolyline3D(p, n1);
        n.draw();
        ofPushMatrix();
        ofVec3f axis;
        float angle;
        n.mRotation.getRotate().getRotate(angle, axis);
        ofTranslate(n.mWorldCenter);
        ofRotate(angle, axis.x, axis.y, axis.z);
        ofSetColor(255,255,0,30);
        ofCircle(0,0,0, n.getBoundingBox().getWidth()/2<20 ? 20 : n.getBoundingBox().getWidth()/2);
        ofSetColor(0,0,255,30);
        ofPushMatrix();
        ofRotateY(90);
        ofCircle(0,0,0, n.getBoundingBox().getWidth()/2<20 ? 20 : n.getBoundingBox().getWidth()/2 );
        ofPopMatrix();
        ofPopMatrix();
    }
    
    if(mShowStencils){
        ofSetColor(0,0,255, 200);
        if(mStencil->hasSlices())
            mStencil->draw();
    }
    
    if(mShowContours){
        ofSetColor(127, 127);
        for(int i=0;i<mContours.size();i++){
            for(int j=0;j<mContours[i].size();j++){
                mContours[i][j].draw();
                ofCircle( mContours[i][j].getCentroid2D().x,  mContours[i][j].getCentroid2D().y, 2);
            }
        }
    }
    
    if(bHasObs && bShowObs){
        curObs->drawSelection();
        for(int i=0;i<mObstacles.size();i++){
            mObstacles[i].draw();
        }
    }
    
    if(mShowTracers){
        mModel.draw();
    }
    if(mShowTracerMesh){
        ofSetColor(200,127);
        mTracerMesh.drawWireframe();
    }
    
    
    
    
    ofSetColor(100,9,255);
    ofCircle(0, 0, 2);
    
    camera.end();
    
    //RotationArcBall
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()-100, ofGetHeight()-100, 0);
    ofVec3f axis;
    float angle;
    mMasterRotation.getRotate(angle, axis);
	ofRotate(angle, axis.x, axis.y, axis.z);
    ofNoFill();
    ofSetColor(255, 175);
	ofSphere(0, 0, 0, 30);
	ofPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key=='q'){
        camera.enableMouseInput();
        
    }
    
    if(key == ' '){
        bWireframe =  !bWireframe;
    }
    if(key ==  'a'){
        drawGrid = !drawGrid;
    }
    if(key ==OF_KEY_UP){
        mc.threshold += .03;
        cout<<mc.threshold<<endl;
    }
    if(key =='f'){
        ofToggleFullscreen();
    }
    if(key =='h'){
        gui->toggleVisible();
        guiMC->toggleVisible();
        guiT->toggleVisible();
    }
    if(key =='r'){
        gui->toggleMinified();
        guiMC->toggleMinified();
        guiT->toggleMinified();
    }
    if(key ==OF_KEY_DOWN){
        mc.threshold -= .03;
    }
    if(key == OF_KEY_LEFT){
        scale -= 10;
    }
    if(key == OF_KEY_RIGHT){
        scale += 10;
    }
    if (key == '1') {
        bTriangulate = !bTriangulate;
        if(bTriangulate){
            updateTraingulation();
        }
        else{
            updateResolution();
            updateMarchingCubes();
        }
    }
    if(key =='s'){
        saveToObj();
    }
    
    if(key =='t')bRotSel = true;
    
}


void testApp::updateMarchingCubes(){
    int imageIndex=0;
    for(int i=0; i<mc.resX; i++){
        for(int j=0; j<mc.resY; j++){
            for(int k=0; k<mc.resZ; k++){
                imageIndex = imageIndex%13;
                
                float value;
                ofColor c = imgs[imageIndex].getPixelsRef().getColor(i*cubeResolution, j*cubeResolution);
                if(c.getHue()>0.){
                    value = (c.getHue()/255);
                    value =1;
                    mc.setIsoValue( i, j, k, isoValue * isoValue );
                }
                else{
                    
                }
                imageIndex++;
            }
        }
        mc.update();
    }
}
void testApp::updateTraingulation(){
    int imageIndex=0;
    triangulation.reset();
    for(int i=0; i<imgs[0].getWidth(); i+=triResX){
        for(int j=0; j<imgs[0].getHeight(); j+=triResY){
            imageIndex = imageIndex%13;
            
            float value;
            ofColor c = imgs[imageIndex].getPixelsRef().getColor(i, j);
            if(bInvertSelection){
                if(c.getHue()<0){
                    triangulation.addPoint(ofPoint(i, j, triResZ+500));
                }
            }
            else{
                if(c.getHue()>0.){
                    triangulation.addPoint(ofPoint(i, j, triResZ+500));
                }
            }
            imageIndex++;
        }
    }
    triangulation.triangulate();
}



vector<ofPolyline3D> testApp::getImageContours( ofImage &image){
    
    vector<ofPolyline3D> contours;
    contours.resize(0);
    
    mContourFinder.setMinAreaRadius(20);
    mContourFinder.setThreshold(0);
    mContourFinder.setSimplify(true);
    mContourFinder.findContours(image);
    
    vector<vector<cv::Point> > points = mContourFinder.getContours();
    
    if (points.size() > 0) {
        //   for(int i =0;i<mContourFinder.getContours().size();i++){
        for(int i =0;i<points.size();i++){
            
            ofPolyline line;
            float zAvg = 0;
            for(int j = 0; j<points[i].size()-2;j++){
                ofVec3f pos;
                ofColor ptColor = image.getColor(points[i][j].x, points[i][j].y);
                float hue = ptColor.getHue();
                //change the 0 500 for scale
                pos = ofVec3f(points[i][j].x,points[i][j].y,ofMap(hue,0,255,0,500));
                zAvg += pos.z;
                line.addVertex(pos);
            }
            
            ofPolyline3D n1 = ofPolyline3D();
            //n1.mWorldCenter = ofVec3f(line.getCentroid2D().x,line.getCentroid2D().y,0);
            line.close();
            zAvg/=line.getVertices().size();

            
            ofVec3f center = ofVec3f(line.getCentroid2D().x, line.getCentroid2D().y, zAvg);
            
            for(int i=0;i<line.getVertices().size();i++){
                ofVec3f diff = line.getVertices()[i] - center;
                line.getVertices()[i] = diff;
            }
            
            ofPolyline p = line.getSmoothed(2);
            ofPolyline3D n = ofPolyline3D::convertToPolyline3D(p,n1);
            n.close();
            contours.push_back(n);
        }
    }
    return contours;
}

void testApp::saveToObj(){
    if(bTriangulate){
        cout<<"saving triangulated obj!"<<endl;
        ofxObjLoader::save("TriangulatedObject_" + ofGetTimestampString()+".obj", triangulation.triangleMesh);
    }
    else if (mUseTracers){
        cout<<"saving tracer obj!"<<endl;
        ofxObjLoader::save("TracerObject_" + ofGetTimestampString()+".obj", mTracerMesh);
    }
    else{
        
        cout<<"saving marching cube obj!"<<endl;
        mc.exportObj("MarchinCubesObject_"+ofGetTimestampString());
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key=='q'){
        camera.disableMouseInput();
        mEnableSelection = false;
    }
    if(key=='r')mRunTracers = !mRunTracers;
    if(key == 't')bRotSel = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    ofVec2f mouse(x,y);
    
    //rotate camera
  //  if(mouse.x > ofGetWidth()-200 && mouse.y > ofGetHeight()-200){
        
    //    float scaledX = ofMap(x, ofGetWidth()-200,ofGetWidth(), 0, ofGetWidth());
     //   float scaledY = ofMap(y, ofGetHeight()-200,ofGetHeight(), 0, ofGetHeight());
     //   float lastScaledX = ofMap(mLastMouse.x, ofGetWidth()-200,ofGetWidth(), 0, ofGetWidth());
    //    float lastScaledY = ofMap(mLastMouse.y, ofGetHeight()-200,ofGetHeight(), 0, ofGetHeight());
        
        ofQuaternion yRot((mouse.x - mLastMouse.x)*mRotDampen, ofVec3f(0,1,0));
        ofQuaternion xRot((mouse.y - mLastMouse.y)*mRotDampen, ofVec3f(-1,0,0));
        // camera.setOrientation(mMasterRotation);
//        ofVec3f axis;
//        float angle;
//        mMasterRotation.getRotate(angle, axis);
        if(mShowSelection && bRotSel ){
            mMasterRotation = yRot*xRot*curSel->mRotation.getRotate();
            curSel->mRotation.setRotate(mMasterRotation);
  
    }
    mLastMouse = mouse;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

