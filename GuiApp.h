#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GuiApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofParameterGroup genParam;
	ofParameterGroup toropoxParam;
	ofParameter<float> minSize;
	ofParameter<float> maxSize;
	ofParameter<float> migrationSpeed;
	ofParameter<float> buddingSpeed;
	ofParameter<float> growthRate;
	ofParameter<bool> toropoxMode;
	ofParameter<bool> debug;
	ofParameter<bool> fullscreen;
	ofParameter <float> phaseSpeed;
	ofParameter<float> rotationSpeed;

	ofParameter <float> minWaxWane;
	ofParameter<float>	maxWaxWane;
	ofParameter <int> maxCount;
	ofParameter <int> initCount;
	ofxButton resetButton;
	ofxPanel gui;

};

