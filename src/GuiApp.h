#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxInputField.h"

class GuiApp: public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyReleased(int key);

	ofParameterGroup genParam;
	ofParameterGroup toropoxParam;
	ofParameterGroup CVParam;

	ofParameter <float> minSize;
	ofParameter <float> maxSize;
	ofParameter <float> migrationSpeed;
	ofParameter <float> buddingSpeed;
	ofParameter <float> growthRate;
	ofParameter <float> phaseSpeed;
	ofParameter <float> rotationSpeed;
	ofParameter <float> detectionRange;
	ofParameter <int>   distanceX;
	ofParameter <int>   distanceY;
	ofParameter <int>   maxCount;
	ofParameter <int>   initCount;
	ofParameter <bool>  toropoxMode;
	ofParameter <float> minWaxWane;
	ofParameter <float>	maxWaxWane;
	ofParameter <bool>  uniformPhase;
	ofParameter <bool>  uniformRotation;

	ofParameter <bool>  skeletonFlipX;
	ofParameter <bool>  skeletonFlipY;
	ofParameter <bool>  skipIntroText;
	ofParameter <bool>  skipScreenSaver;
	ofParameter <bool>	flipText;
	ofParameter <int>	gamePlayLength; 
	ofParameter <bool>  sweeping;
	ofParameter <bool>	spawn;
	ofParameter <bool>	skeleton;
	ofParameter <int>   handSensitivity;
	ofParameter <bool>  manualSelection; 
	ofxTextField dataLocation; 
	

	ofxPanel gui;



};

