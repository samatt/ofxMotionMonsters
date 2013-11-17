#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);
    
	drawGrid = true;
    bWireframe = false;
    bInvertSelection = false;
    scale = 1;
    
	mc.setSmoothing( false );
    for(int i=0; i<13; i++){
        ofImage img;
        img.loadImage("Images/hands/"+ofToString(i)+".png");
        imgs.push_back(img);
        
    }
    mc.setup();
    cubeResolution = 32;
    updateResolution();

	mc.scale.set( 500, 250, 500 );
    updateMarchingCubes();
    bTriangulate = false;
    triResX=2;
    triResY=2;
    triResZ=100;

    setupGUI();
    normalShader.load("shaders/normalShader");

    //    ofLog(OF_LOG_VERBOSE);
}
void testApp::updateResolution(){
    float x= imgs[0].getWidth()/cubeResolution;
    float y = imgs[0].getHeight()/cubeResolution;
	mc.setResolution(x,y,cubeResolution);

}
void testApp::setupGUI(){
    
    gui = new ofxUISuperCanvas("MOTION MONSTERS");
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
    gui->addToggle("SAVE OBJ",false);
    gui->addToggle("LOAD IMGS",false);
    gui->addSpacer();
    gui->addToggle("INVERT SELECTION", &bInvertSelection);
    gui->addToggle("DRAW TRIANGULATION", &bTriangulate);
    gui->addToggle("DRAW WIREFRAME", &bWireframe);
    gui->addSpacer();
    gui->addLabel("TRIANGULATION PARAMS");
    gui->addSpacer();
    gui->addIntSlider("RES X", 1, 20, &triResX);
    gui->addIntSlider("RES Y", 1, 20, &triResY);
    gui->addIntSlider("RES Z", 10, 500, &triResZ);
    gui->addSpacer();
    gui->addLabel("MC PARAMS");
    gui->addIntSlider("CUBE RESOLUTION", 20, 500, &cubeResolution);
    gui->addToggle("UPDATE MC", false);
    gui->addSpacer();
    gui->addSlider("MC THRESHOLD", 0.1, 1, &mc.threshold);
//    gui->addIntSlider("RES X", 1, 20, &triResX);
//    gui->addIntSlider("RES Y", 1, 20, &triResY);
//    gui->addIntSlider("RES Z", 10, 500, &triResZ);
    gui->autoSizeToFitWidgets();

    
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
}
///--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    
    if(name == "TRIANGULATE"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        bTriangulate = t->getValue();
        if(bTriangulate){
            updateTraingulation();    
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
        }
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
            t->setValue(false);
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
        bInvertSelection = t->getValue();
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

        for(int i=0; i<size; i++){
            cout<<files[i].getFileName()<<endl;
            for(int i=0; i<files.size(); i++){
                ofImage img;
                img.loadImage(filePath + "/"+files[i].getFileName());
                imgs.push_back(img);
                
            }
        }
    }
    else{
        cout<<"directory doesnt exist"<<endl;
    }
    
        updateResolution();
    
}
//--------------------------------------------------------------
void testApp::update(){
    
    if (bTriangulate) {
        //        updateTraingulation();
    }
    else{
        mc.update();

    }
}
//--------------------------------------------------------------
void testApp::draw(){
    camera.begin();
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
    camera.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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

void testApp::saveToObj(){
    if(bTriangulate){
        cout<<"saving triangulated obj!"<<endl;
        ofxObjLoader::save("TriangulatedObject_" + ofGetTimestampString()+".obj", triangulation.triangleMesh);
    }
    else{
        
        cout<<"saving marching cube obj!"<<endl;
        mc.exportObj("MarchinCubesObject_"+ofGetTimestampString());
    }
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
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
