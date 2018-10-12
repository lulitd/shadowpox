#pragma once

#include "ofMain.h"
#include  "cell.h"

class cellCluster
{
public:
	cellCluster();
	~cellCluster();

	void setup();
	void update();
	void draw();
	void addCell(ofVec2f pos);
	void addCell(ofVec2f p1, ofVec2f p2);
	void addCell(ofVec2f bound1, ofVec2f bounds2, ofVec2f spawnPoint);
	void addCell();
	void addCellCluster(int amount);
	void addCellCluster(int amount, ofVec2f pos);
	void addCellCluster(int amount, ofVec2f p1, ofVec2f p2);
	void clear();

	static bool kinectTracking;

	float growth;
	float minRadius;
	float maxRadius;
	float radius;
	bool isToroPox;
	float rotateSpeed;
	float phaseSpeed;
	float speed;
	int distanceX;
	int distanceY;
	float detectionRange;
	int initCells = 0;
	int maxCells=100;
	float minWaxWane;
	float maxWaxWane;
	bool uniformPhase;
	bool uniformRotation;
	ofVec2f bounds1;
	ofVec2f bounds2;
	ofVec2f targetPos;
	ofVec2f handL;
	ofVec2f handR;
	ofVec2f prevhandL;
	ofVec2f prevhandR;
	vector<ofVec2f> boundsPoints; 
	int handStateL;
	int handStateR;

	int handStateCounterL;
	int handStateCounterR;
	int handStateTestingL;
	int handStateTestingR;

	
	float targetRadius;
	bool tracking = false;

	std::vector <cell> cluster;


	


};

