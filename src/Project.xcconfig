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
            ofVec3f target = ofVec3f(stencil->getSlices()[0].getWorldLocation(arrPos).x, stencil->getSlices()[0].getWorldLocation(arrPos).y, stencil->getSlices()[0].getWorldLocation(arrPos).z );
            //target.z += j*100;
            mTargets.push_back(target);
            ofPolyline b = stencil->getSlices()[stencil->getSlices().size()-1].getResampledByCount(numTracers);
            int ind = stencil->getSlices().size()-1;
            ofPolyline3D n = stencil->getSlice(ind);
            mBase = ofPolyline3D::convertToPolyline3D(b, n);
        }
    
    for (int i=0;i<mTargets.size();i++) {
        mTracers.push_back(new Tracer( mTargets[i],mBase.getWorldLocation(i), i));
    }
    
}

void TracerModel::setSeperation(float &sep){
    for(int i=0;i<mTracers.size();i++){
        mTracers[i]->setSeperation(sep);
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
  //  ofxDelaunay* tri = new ofxDelaunay();
    int smallest = 10000000000;
    for(int i=0;i<mTracers.size();i++){
        if(mTracers[i]->mPath.getVertices().size()<smallest)smallest = mTracers[i]->mPath.getVertices().size();
    }
    
    for(int i=0;i<mTracers.size();i++){
        if(mTracers.size() != smallest){
        ofPolyline3D newPath = mTracers[i]->mPath;
        ofPolyline resample = mTracers[i]->mPath.getResampledByCount(smallest);
        mTracers[i]->mPath = ofPolyline3D::convertToPolyline3D(resample, newPath);
        }
    }
    
    
    for(int i=0;i<mTracers.size();i++){
        for(int j =0; j<smallest;j++){
            masterPoints.push_back(ofVec3f(mTracers[i]->mPath.getWorldLocation(j)));
        }
    }
    
 //   tri->addPoints(masterPoints);
   // tri->triangulate();
   // mesh = tri->triangleMesh;
   // delete tri;
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    int height = mTracers.size();
    int width = smallest;
    
    for (int y = 0; y < height; y++){
		for (int x = 0; x<width; x++){
			mesh.addVertex(masterPoints[x+y*width]);	// mesh index = x + y*width
			//mesh.addTexCoord(ofVec2f(x,y)); // lock each vertex to the right texture coordinate
		}
	}
	
	for (int y = 0; y<height-1; y++){
		for (int x=0; x<width-1; x++){
			mesh.addIndex(x+y*width);				// 0
			mesh.addIndex((x+1)+y*width);			// 1
			mesh.addIndex(x+(y+1)*width);			// 10
			
			mesh.addIndex((x+1)+y*width);			// 1
			mesh.addIndex((x+1)+(y+1)*width);		// 11
			mesh.addIndex(x+(y+1)*width);			// 10
		}
	}
}

void TracerModel::reset(){
    mTracers.clear();
}

Stencil::Stencil(){
    slices.resize(0);
}

ofPolyline3D Stencil::getSlice( int &ID ){
    return slices[ID];
}

bool Stencil::hasSlices(){
    if(slices.size()>0) return true; else return false;
}

void Stencil::setSlice(ofPolyline3D poly){
    slices.push_back(poly);
}

void Stencil::removeSlice(int &ID){
    vector<ofPolyline3D>::iterator it = slices.begin()+ID;
    it = slices.erase(it);
}

void Stencil::draw(){
    for(int i=0;i<slices.size();i++){
        slices[i].draw();
    }
}

vector<ofPolyline3D> Stencil::getSlices(){
    return slices;
}

void Stencil::reset(){
    slices.clear();
}

