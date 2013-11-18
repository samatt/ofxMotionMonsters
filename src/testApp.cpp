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
    mAllowSliceComp = false;
    mShowContours = true;
    mAddContour = false;
    mRunTracers = false;
    mShowSelection = false;
    mEnableSelection = false;
    mShowTracerMesh = false;
    bInvertSelection = false;
    scale = 1;
	mc.setSmoothing( false );
    
    mNumTracers = 100;
    
//    for(int i=0; i<13; i++){
//        ofImage img;
//        img.loadImage("Images/hands/"+ofToString(i)+".png");
//        imgs.push_back(img);
//        
//    }
    mc.setup();
    cubeResolution = 32;
	mc.scale.set( 500, 250, 500 );
    // updateResolution();
   // updateMarchingCubes();
    bTriangulate = false;
    triResX=2;
    triResY=2;
    triResZ=100;
    
    tX = 0;
    tY = 0;
    tZ = 0;
    tS = 0;
    
    mRotDampen = .2;
    
    normalShader.load("shaders/normalShader");

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
    
    //camera.disableMouseInput();
//    ofLog(OF_LOG_VERBOSE);
}

void testApp::updateResolution(){
    float x= imgs[0].getWidth()/cubeResolution;
    float y = imgs[0].getHeight()/cubeResolution;
	mc.setResolution(x,y,cubeResolution);
    
}

void testApp::setupGUIMC(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    guiMC = new ofxUISuperCanvas("MARCHING CUBES", length+xInit+2, 0, length+xInit, ofGetHeight()/2);
    guiMC->addSpacer();
    guiMC->addToggle("SAVE OBJ",&mSaveObj);
    guiMC->addToggle("INVERT SELECTION", &bInvertSelection);
    guiMC->addToggle("DRAW TRIANGULATION", &bTriangulate);
    guiMC->addToggle("DRAW WIREFRAME", &bWireframe);
    guiMC->addSpacer();
    guiMC->addLabel("TRIANGULATION PARAMS");
    guiMC->addSpacer();
    guiMC->addIntSlider("RES X", 1, 20, &triResX);
    guiMC->addIntSlider("RES Y", 1, 20, &triResY);
    guiMC->addIntSlider("RES Z", 10, 500, &triResZ);
    guiMC->addSpacer();
    guiMC->addLabel("MC PARAMS");
    guiMC->addIntSlider("CUBE RESOLUTION", 20, 500, &cubeResolution);
    guiMC->addToggle("UPDATE MC", false);
    guiMC->addSpacer();
    guiMC->addSlider("MC THRESHOLD", 0.1, 1, &mc.threshold);
    //    gui->addIntSlider("RES X", 1, 20, &triResX);
    //    gui->addIntSlider("RES Y", 1, 20, &triResY);
    //    gui->addIntSlider("RES Z", 10, 500, &triResZ);
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
    guiT->addToggle("RUN TRACERS", &mRunTracers);
    guiT->addButton("MAKE MESH", false);
    guiT->addButton("RESET", false);


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
    gui->addSlider("Z", 0, 1000000, &tZ);
    gui->addSlider("SCALE", 1 , 10, &tS);
    gui->addToggle("ADD SELECITON", &mAddContour);
    gui->addSpacer();
    gui->addToggle("USE TRACERS", &mUseTracers);
    gui->addToggle("USE MC", &mUseMC);

    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);


}

void testApp::createBase(){
    
    vector<ofPolyline> tmp;
    ofRectangle rect = ofRectangle(-100, -100, 200, 200);
    ofPolyline base;
    base.addVertex(rect.getTopLeft());
    base.addVertex(rect.getTopRight());
    base.addVertex(rect.getBottomRight());
    base.addVertex(rect.getBottomLeft());
    base.close();
    tmp.push_back(base);
    
    ofPolyline base2;
    
    for(int i=0;i<100;i++){
        float angle = ofMap(i,0,100,0,TWO_PI);
        base2.addVertex(ofVec3f(100*cos(angle), 100*sin(angle), -500));
    }
    base2.close();
    tmp.push_back(base2);
    
    mContours.push_back(tmp);
    
}

///--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
        
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    
    if(name == "TRIANGULATE"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bTriangulate = t->getValue();
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
        if (t->getValue()) {
            for(int i=0;i<imgs.size();i++){
                mContours.push_back(getImageContours(imgs[i]));
            }
            cout<<mContours.size()<<endl;
        }
    }
    
    
    if(name == "USE TRACERS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        guiT->toggleVisible();
        mShowTracers = true;
        mShowStencils = true;
    }
    
    if(name == "USE MC"){
        //ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        guiMC->toggleVisible();
     //   cout<<"visible"<<endl;
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
        mModel.reset();
        mShowTracerMesh = false;
    }

    
    if(name == "ZOOM")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		mCameraPos.z = slider->getScaledValue();
	}
    
    
    
    if(name == "X")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        
        mTransformMatrix.setTranslation(slider->getValue(), 0, 0);
        
        for(int i=0;i<curSel->getVertices().size();i++){
            curSel->getVertices()[i] = curSel->getVertices()[i] * mTransformMatrix; //+= slider->getIncrement();
        }
	}
    if(name == "Y")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        for(int i=0;i<curSel->getVertices().size();i++){
            float delta = curSel->getVertices()[i].y - slider->getScaledValue();
            curSel->getVertices()[i].y += delta;
        }
    }
    
    if(name == "Z")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        for(int i=0;i<curSel->getVertices().size();i++){
            float delta = curSel->getVertices()[i].z - slider->getScaledValue();
            curSel->getVertices()[i].z += delta;
        }
	}
    
    if(name == "SCALE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        for(int i=0;i<curSel->getVertices().size();i++){
            curSel->getVertices()[i] *= slider->getScaledValue();
        }
	}
    
    if(name == "UPDATE MARCHING CUBES"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        if(!bTriangulate && t->getValue()){
            updateResolution();
            updateMarchingCubes();
            t->setValue(false);
        }
        
    }
    
    if(name == "INVERT SELECTION"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
    }

    
    if(name == "LOAD IMGS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        if (t->getValue()) {
            ofFileDialogResult result = ofSystemLoadDialog("Load Images From Folder", true, "Images/");
            //cout<<result.filePath<<endl;
            
            if(result.bSuccess && result.fileName.length())
            {
                loadImagesFromPath(result.filePath);
            }
            mLoadImages = false;
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
        
        //for(int i=0; i<size; i++){
            for(int i=0; i<files.size(); i++){
                cout<<files[i].getFileName()<<endl;
                ofImage img;
                img.loadImage(filePath + "/"+files[i].getFileName());
                imgs.push_back(img);
                
            }
        //}
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
    }
    
    if(mShowSelection){
        int x = (int)ofMap(mContInd.x,0,247,0, mContours.size()-1);
        int y = (int)ofMap(mContInd.y, 0, 120 ,0, mContours[x].size()-1);
        mContIndSet.x = x;
        mContIndSet.y = y;
        curSel = &mContours[x][y];
    }
    
    if(mAddContour){
        mStencil->setSlice(mContours[mContIndSet.x][mContIndSet.y].getSmoothed(1));
        mAddContour = false;
    }
    
    
    if(mUseMC){
        if (bTriangulate) {
            //        updateTraingulation();
        }
        else{
            mc.update();
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(64), ofColor(0));
    ofSetColor(255, 255, 255);
    camera.begin();
    if(mUseMC){
        if(bTriangulate){
            ofPushStyle();
            normalShader.begin();
            bWireframe?  ofNoFill(): ofFill();
            triangulation.draw();
            normalShader.end();
            ofPopStyle();
        }
        else{
            normalShader.begin();
            bWireframe?	mc.drawWireframe() : mc.draw();
            //draw the voxel grid           if(drawGrid)mc.drawGrid();
            normalShader.end();
        }
    }
    
    
    if(mShowSelection){
        ofSetColor(255, 0, 0, 200);
        ofPolyline cur = curSel->getSmoothed(1);
        cur.draw();
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
            }
        }
    }
    
    if(mShowTracers){
        mModel.draw();
    }
    if(mShowTracerMesh){
        ofSetColor(200,127);
        mTracerMesh.drawWireframe();
    }
        
    
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
    ofRect(-50,-50,100,100);
	ofSphere(0, 0, 0, 30);
	ofPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key=='q'){
        camera.disableMouseInput();
        
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
    
    if(key =='h'){
        gui->toggleVisible();
    }
    if(key =='r'){
        gui->toggleMinified();
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
    
}

void testApp::saveToObj(){
    if(bTriangulate){
        cout<<"saving triangulated obj!"<<endl;
        ofxObjLoader::save("TriangulatedObject_" + ofGetTimestampString(), triangulation.triangleMesh);
    }
    else{
        
        cout<<"saving marching cube obj!"<<endl;
        mc.exportObj("MarchinCubesObject_"+ofGetTimestampString());
    }
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
                    mc.setIsoValue( i, j, k, value * value );
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


vector<ofPolyline> testApp::getImageContours( ofImage &image){
    
    vector<ofPolyline> contours;
    contours.resize(0);
    
    mContourFinder.setMinAreaRadius(20);
    mContourFinder.setThreshold(0);
    mContourFinder.setSimplify(true);
    mContourFinder.findContours(image);
    
    if (mContourFinder.getContours().size() > 0) {
        for(int i =0;i<mContourFinder.getContours().size();i++){
            ofPolyline line;
            for(auto pt: mContourFinder.getContour(i)){
                ofVec3f pos;
                ofColor ptColor = image.getColor(pt.x, pt.y);
                float hue = ptColor.getHue();
                pos = ofVec3f(pt.x,pt.y,ofMap(hue,0,255,0,500));
                
                line.addVertex(pos);
            }
            
            for(int i=0;i<line.getVertices().size();i++)
                line.getVertices()[i] -= ofVec3f(line.getCentroid2D().x, line.getCentroid2D().y, 0);
            
            contours.push_back(line.getSmoothed(2));
        }
    }

    return contours;
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key=='q'){
        camera.enableMouseInput();
        mEnableSelection = false;
    }
    if(key=='r')mRunTracers = !mRunTracers;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
 
    ofVec2f mouse(x,y);
    
    
    
    
    //rotate camera
    if(mouse.x > ofGetWidth()-200 && mouse.y > ofGetHeight()-200){
        
        float scaledX = ofMap(x, ofGetWidth()-200,ofGetWidth(), 0, TWO_PI);
        float scaledY = ofMap(y, ofGetHeight()-200,ofGetHeight(), 0, TWO_PI);
        float lastScaledX = ofMap(mLastMouse.x, ofGetWidth()-200,ofGetWidth(), 0, TWO_PI);
        float lastScaledY = ofMap(mLastMouse.y, ofGetHeight()-200,ofGetHeight(), 0, TWO_PI);

        ofQuaternion yRot((mouse.x - mLastMouse.x)*mRotDampen, ofVec3f(0,1,0));
        ofQuaternion xRot((mouse.y - mLastMouse.y)*mRotDampen, ofVec3f(-1,0,0));
        mMasterRotation *= yRot*xRot;
       // camera.setOrientation(mMasterRotation);
        ofVec3f axis;
        float angle;
        mMasterRotation.getRotate(angle, axis);
        for(int i=0;i<curSel->getVertices().size();i++){
            curSel->getVertices()[i] = curSel->getVertices()[i].rotated(angle, axis);
        }
        
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

