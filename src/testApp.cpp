#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);

	drawGrid = true;
    bWireframe = false;
    scale = 1;
    
	mc.setSmoothing( false );
    for(int i=0; i<13; i++){
        ofImage img;
        img.loadImage("Images/"+ofToString(i)+".png");
        imgs.push_back(img);
        
    }
    
    float x= imgs[0].getWidth()/32;
    float y = imgs[0].getHeight()/32;
    mc.setup();
	mc.setResolution(x,y,32);
	mc.scale.set( 500, 250, 500 );

    bTriangulate = false;

    setupGUI();
//    ofLog(OF_LOG_VERBOSE);
}

void testApp::setupGUI(){

    gui = new ofxUISuperCanvas("MOTION MONSTERS");
    gui->addSpacer();
    gui->addToggle("TRIANGULATE", &bTriangulate);
    gui->addToggle("DRAW WIREFRAME", &bWireframe);
    gui->addSlider("MC THRESHOLD", 0.1, 1, &mc.threshold);
    gui->addToggle("SAVE OBJ",false);
    gui->addToggle("LOAD IMGS",false);
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
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
        }
        
    }
    if(name == "LOAD IMGS"){
        ofxUIToggle* t  = (ofxUIToggle*) e.widget;
        if (t->getValue()) {
            ofFileDialogResult result = ofSystemLoadDialog("Load Images From Folder", true, "Images/");
//            cout<<result.filePath<<endl;

            if(result.bSuccess && result.fileName.length())
            {
                loadImagesFromPath(result.filePath);
            }
            else{
//                loadGUIS();
            }
        }
        
    }
}
//--------------------------------------------------------------
void testApp::loadImagesFromPath(string filePath){
    cout<<filePath<<endl;
    ofDirectory dir(filePath);
    if(dir.exists()){
        int size = dir.size();

        cout<<size<<endl;
        
        for(int i=0; i<size; i++){
          //  cout<<files[i].getFileName()<<endl;
//            for(int i=0; i<13; i++){
//                ofImage img;
//                img.loadImage("Images/"+ofToString(i)+".png");
//                imgs.push_back(img);
//                
        }
    }
    else{
        cout<<"directory doesnt exist"<<endl;
    }

}
//--------------------------------------------------------------
void testApp::update(){
    
    if (bTriangulate) {
        updateTraingulation();
    }
    else{
        int imageIndex=0;
        for(int i=0; i<mc.resX; i++){
            for(int j=0; j<mc.resY; j++){
                for(int k=0; k<mc.resZ; k++){
                    imageIndex = imageIndex%3;
                    
                    float value;
                    ofColor c = imgs[0].getPixelsRef().getColor(i*32, j*32);
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
        }
        mc.update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    camera.begin();
    if(bTriangulate){
        ofPushStyle();
        bWireframe?  ofNoFill(): ofFill();
        triangulation.draw();
        ofPopStyle();
    }
    else{
        bWireframe?	mc.drawWireframe() : mc.draw();
        //draw the voxel grid
        if(drawGrid)mc.drawGrid();
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
        
    }
    if(key =='s'){
        saveToObj();
    }
    
}
void testApp::updateTraingulation(){
    int imageIndex=0;
    triangulation.reset();
    for(int i=0; i<mc.resX; i++){
        for(int j=0; j<mc.resY; j++){
                imageIndex = imageIndex%13;
                
                float value;
                ofColor c = imgs[imageIndex].getPixelsRef().getColor(i*32, j*32);
                if(c.getHue()>0.){
                    
                    triangulation.addPoint(ofPoint(i, j, imageIndex*5));
                }
                imageIndex++;
        }
    }
    triangulation.triangulate();
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
