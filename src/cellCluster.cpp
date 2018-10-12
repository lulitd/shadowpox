#include "cellCluster.h"



bool cellCluster::kinectTracking = false;

cellCluster::cellCluster() {
}

cellCluster::~cellCluster() {
}

void cellCluster::setup() {
	handL = ofVec2f(-1000, -1000);
	handR = ofVec2f(-1000, -1000);
	cluster.reserve(maxCells);
	boundsPoints.reserve(4);

	for (size_t i = 0; i < 4; i++)
	{
		ofVec2f p = ofVec2f(-9999,9999);
		boundsPoints.push_back(p);

	}
	addCellCluster(initCells);
}


void cellCluster::update() {

	int i = cluster.size();
	while (i--) {
		if (!cluster[i].isDefeated) {
			cluster[i].isToroPox = isToroPox;//
			cluster[i].minRadius = minRadius;//
			cluster[i].maxRadius = maxRadius;//
			cluster[i].growth = growth;//
			cluster[i].rotateSpeed = rotateSpeed;//
			cluster[i].phaseSpeed = phaseSpeed;//
			cluster[i].speed = speed;
			cluster[i].minWaxWane = minWaxWane;
			cluster[i].maxWaxWane = maxWaxWane;
			cluster[i].distanceX = distanceX;
			cluster[i].distanceY = distanceY;
			cluster[i].uniformPhase = uniformPhase;
			cluster[i].uniformRotation = uniformRotation;
			cluster[i].p1 = bounds1;
			cluster[i].p2 = bounds2;
			cluster[i].bounds = &boundsPoints;
			cluster[i].isTargeted = (kinectTracking) ? ((cluster[i].circleIntersect(handL, detectionRange) && handStateL == 3) || (cluster[i].circleIntersect(handR, detectionRange) && handStateR == 3)) :
				(cluster[i].circleIntersect(ofVec2f(ofGetMouseX(), ofGetMouseY()), detectionRange));

			if (cluster[i].isTargeted) {
				cluster[i].force = (kinectTracking) ? ((cluster[i].circleIntersect(handL, detectionRange)) ? handL - prevhandL : handR - prevhandR) :
					ofVec2f(ofGetMouseX() - ofGetPreviousMouseX(), ofGetMouseY() - ofGetPreviousMouseY());
			}

			cluster[i].update();
		}
		else {
			// pox has been defeated, erase it
			cluster.erase(cluster.begin() + i);
		}
	}
}


void cellCluster::addCell(ofVec2f pos) {
	cell c;
	c.setup();
	c.isToroPox = isToroPox;
	c.minRadius = minRadius;
	c.radius = minRadius + c.index *0.001* ofRandomf();
	c.maxRadius = maxRadius;
	c.growth = growth;
	c.rotateSpeed = rotateSpeed;
	c.phaseSpeed = phaseSpeed;
	c.speed = speed;
	c.minWaxWane = minWaxWane;
	c.maxWaxWane = maxWaxWane;
	c.uniformPhase = uniformPhase;
	c.uniformRotation = uniformRotation;
	c.p1 = ofVec2f(ofRandomWidth(), ofRandomHeight());
	c.p2 = ofVec2f(ofRandomWidth(), ofRandomHeight());
	c.pos = pos;

	c.d1 = c.pos - c.p1;
	c.d2 = c.pos - c.p2;

	c.d1.normalize();
	c.d2.normalize();
	cluster.push_back(c);
}

void cellCluster::addCell() {
	cell c;
	c.setup();
	c.isToroPox = isToroPox;
	c.minRadius = minRadius;
	c.radius = minRadius + c.index *0.001* ofRandomf();
	c.maxRadius = maxRadius;
	c.growth = growth;
	c.rotateSpeed = rotateSpeed;
	c.phaseSpeed = phaseSpeed;
	c.speed = speed;
	c.minWaxWane = minWaxWane;
	c.maxWaxWane = maxWaxWane;
	c.uniformPhase = uniformPhase;
	c.p1 = ofVec2f(ofRandomWidth(), ofRandomHeight());
	c.p2 = ofVec2f(ofRandomWidth(), ofRandomHeight());
	c.pos = ofVec2f(ofRandomWidth(), ofRandomHeight());

	float r = ofRandom((bounds1.distance(bounds2) / 2)); // random distance from the midpoint of the two points. [0-radius]
	float angle = ofRandomf()*PI * 2; // random angle
	ofVec2f mid = (bounds1 + bounds2) / 2; // find midpoint
	float x = (cos(angle)*r) + mid.x;
	float y = (sin(angle)*r) + mid.y;
	c.d1 = c.pos - c.p1;
	c.d2 = c.pos - c.p2;

	c.d1.normalize();
	c.d2.normalize();
	cluster.push_back(c);
}

void cellCluster::addCell(ofVec2f p1, ofVec2f p2) {
	cell c;
	c.setup();
	c.isToroPox = isToroPox;
	c.minRadius = minRadius;
	c.radius = minRadius + c.index *0.001* ofRandomf();
	c.maxRadius = maxRadius;
	c.growth = growth;
	c.rotateSpeed = rotateSpeed;
	c.phaseSpeed = phaseSpeed;
	c.speed = speed;
	c.minWaxWane = minWaxWane;
	c.maxWaxWane = maxWaxWane;
	c.uniformPhase = uniformPhase;
	c.p1 = p1;
	c.p2 = p2;

	float r = ofRandom(p1.distance(p2) / 2); // random distance from the midpoint of the two points. [0-r]
	float angle = ofRandomf()*PI * 2; // random angle
	ofVec2f mid = (p1 + p2) / 2; // find midpoint
	float x = (cos(angle)*r) + mid.x;
	float y = (sin(angle)*r) + mid.y;
	c.pos = ofVec2f(x, y);
	c.d1 = c.pos - c.p1;
	c.d2 = c.pos - c.p2;

	c.d1.normalize();
	c.d2.normalize();
	cluster.push_back(c);
}

void cellCluster::addCell(ofVec2f bound1, ofVec2f bounds2, ofVec2f spawnPoint) {
	cell c;
	c.setup();
	c.isToroPox = isToroPox;
	c.minRadius = minRadius;
	c.radius = minRadius + c.index *0.001* ofRandomf();
	c.maxRadius = maxRadius;
	c.growth = growth;
	c.rotateSpeed = rotateSpeed;
	c.phaseSpeed = phaseSpeed;
	c.speed = speed;
	c.minWaxWane = minWaxWane;
	c.maxWaxWane = maxWaxWane;
	c.uniformPhase = uniformPhase;
	c.p1 = bounds1;
	c.p2 = bounds2;

	c.pos = spawnPoint;
	c.d1 = c.pos - c.p1;
	c.d2 = c.pos - c.p2;

	c.d1.normalize();
	c.d2.normalize();
	cluster.push_back(c);
}

void cellCluster::addCellCluster(int amount, ofVec2f p1, ofVec2f p2) {
	while (cluster.size() < maxCells && amount) {
		addCell(p1, p2);
		amount--;
	}
}

void cellCluster::addCellCluster(int amount, ofVec2f pos) {
	while (cluster.size() < maxCells && amount) {
		addCell(pos);
		amount--;
	}
}

void cellCluster::addCellCluster(int amount) {
	while (cluster.size() < maxCells && amount) {
		addCell();
		amount--;
	}
}

void cellCluster::clear() {
	cluster.clear();
}

void cellCluster::draw() {
	for (int i = 0; i < cluster.size(); i++) {
		cluster[i].draw();
	}
}




