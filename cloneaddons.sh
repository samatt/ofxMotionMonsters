#!/bin/bash

# Dependencies for MotionMonsters

cd ../../addons

#passing no arguments means read only
if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
	echo "Cloning read-only"
else
    PREFIX="git clone git@github.com:"
	echo "Cloning writable"
fi

#private, must be read
git clone git@github.com:rezaali/ofxExtras
git clone https://github.com/rezaali/ofxUI


git clone https://github.com/satoruhiga/ofxCocoaGLView
git clone https://github.com/satoruhiga/ofxPCL
git clone https://github.com/roymacdonald/ofxCameraSaveLoad
git clone https://github.com/Larsberg/ofxMarchingCubes
git clone https://github.com/obviousjim/ofxDelaunay
${PREFIX}Flightphase/ofxRange
${PREFIX}Flightphase/ofxTween
${PREFIX}YCAMInterlab/ofxTimeline

${PREFIX}obviousjim/ofxObjLoader		
