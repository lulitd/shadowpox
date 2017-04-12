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

	enum imageType {
		HORN,
		TOROPOX
	};

	static void addFile(string filename , imageType type ) {

		switch (type) {
		case HORN: {ofxSVG svg;
			svg.load(filename);
			horns.push_back(svg);
			break;
			}
		case TOROPOX: {
			ofImage img; 
			img.load(filename);
			img.setAnchorPercent(0.5, 0.5); // allow us to draw from center
			toropox.push_back(img);
			break;
		}

		}
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



protected:
	static vector<ofxSVG> horns;
	ofxSVG* hornLeft;
	ofxSVG* hornRight;
	static vector <ofImage> toropox; 
	ofImage* toropoxFrame; 

private:
	float phase;
	int frameRate = 24; 
	float offset; 
	int frameNum = 0; 
	int offsetFrame = 0; 
};

