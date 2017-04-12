#pragma once
#include "ofMain.h"
#include "ofxSvg.h"

class miniFig {
public:
	miniFig();
	~miniFig();
	void setup(ofVec2f loc, int index);
	void update();
	void getInfected(ofVec2f point,int&);
	void draw();

	static float countrySeverityRate;

	ofVec2f Velocity; 

	enum STATE {
		START,
		HAPPY,
		SICK,
		DEAD
	};
	void changeState(STATE s);


	

	static void loadImage(string filename, STATE imgType,bool flipped) {
		ofImage img;
		img.load(filename);
		switch (imgType)
		{
		case START:
			initFigures.push_back(img);
			break;
		case HAPPY:
			if (!flipped) happyAnim.push_back(img);
			else happyAnimF.push_back(img);
			break;
		case SICK:
			if (!flipped) sickAnim.push_back(img);
			else sickAnimF.push_back(img);
			break;
		case DEAD:
			deathImg = img;
			break;
		}
	}
	
protected:
	ofImage* displayImage; 
	static vector<ofImage> initFigures;
	static vector<ofImage> happyAnim; 
	static vector<ofImage> sickAnim; 
	static vector<ofImage> happyAnimF;
	static vector<ofImage> sickAnimF;
	static ofImage deathImg; 
	STATE currentState = START;

private:
	float offset; 
	void move();
	void animate();
	void updateHealth();
	float lifeTime = 0;
	float lifespan = 5;
	int frameNum; 
	ofRectangle boundingBox;
	int check = 0; 
	int sequenceFPS = 25;
	int offsetFrame = 0;
};