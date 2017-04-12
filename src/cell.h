#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

class cell
{
public:
	cell();
	~cell();

	void setup();
	void update();
	void draw();
	bool circleIntersect(ofVec2f center, float r);

	static void addFile(string filename) {
		ofxSVG svg;
		svg.load(filename);
		horns.push_back(svg);
	}

	float growth;
	float minRadius;
	float maxRadius;
	float radius;
	bool isToroPox;
	float rotateSpeed;
	float phaseSpeed;
	float speed;
	float distanceX=20;
	float distanceY= 20;
	ofVec2f pos;

	ofVec2f bounds1;
	ofVec2f bounds2;

	ofVec2f d1;
	ofVec2f d2;

	bool isFullyGrown;
	bool uniformRotation;
	bool uniformPhase;
	float index;
	float minWaxWane;
	float maxWaxWane;

	bool isTargeted= false;
	bool isDefeated = false;
	bool isDying = false;
	float targetTimer;
	float poxHealth=3;// it takes 5 secs to kill a pox
	ofVec2f force;


	//shared_ptr<GuiApp> gui;

protected:
	static vector<ofxSVG> horns;
	ofxSVG* hornLeft;
	ofxSVG* hornRight;


private:
	float phase;
	 
};

