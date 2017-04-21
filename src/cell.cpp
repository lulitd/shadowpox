#include "cell.h"

#define main_col 0


// TODO CHANGE POX COLORS WHEN OFF BODY

vector<ofxSVG> cell::horns;
vector<ofxSVG>cell::toropox;

cell::cell(){
	index = ofMap(ofRandom(20000), 0, 20000, -5000 * 0.01, 5000 * 0.01); // index is to ensure pox are not identical in movement and look
	d1 = ofVec2f(-1000, -1000);
	d2 = ofVec2f(-1000, -1000);

	offset = ofRandomuf();
}


cell::~cell(){
}

void cell::setup() {
	if (horns.size()) {
		int i = (int)(ofRandom(0, horns.size()/2));
		hornLeft = &(horns[i]);
		int j = (int)(ofRandom(horns.size() / 2+1, horns.size() ));
		hornRight = &(horns[j]);
	}
	else  printf("Failed to set horns. Empty list");


	if (toropox.size()) {
		offsetFrame = (int)toropox.size() * offset;
		toropoxFrame = &(toropox[offsetFrame]);
	}
	else  printf("Failed to set toropox frame. Empty list"); 
}

void cell::update() {

	radius += (maxRadius/radius)*growth*0.0001*ABS(index)*ofGetLastFrameTime();
	radius = MIN(radius, maxRadius);

	isFullyGrown = (radius == maxRadius);

	ofPoint p;

	// check for the intersection of the two lines joint1 in the direction of d1 
	//  and joint 2 and the direction of d2 
	// TODO: FIX CHECK THE ANGLES FIRST  FOR 180s, unwanted behaviour at those spec
	if (ofLineSegmentIntersection(bounds1,bounds1+(d1*5000), bounds2,bounds2+(d2*5000), p)) {
		pos.x = p.x; /// the position is their intersection!
		pos.y = p.y;
	}

	
	// movement!
	if (!isTargeted) {
		// if its hasn't been targeted let it do its own thing
		pos.x += sin(ofGetElapsedTimef()+ index)*(distanceX*ofGetLastFrameTime())*speed;
		pos.x += ofRandomf()*ofGetLastFrameTime();
		pos.y += cos(sin(ofGetElapsedTimef()) + index)*(distanceY*ofGetLastFrameTime())*speed;
		pos.y += ofRandomf()*ofGetLastFrameTime();
	}

	// alternative movement
	if (isTargeted) {
	pos.x += force.x ;
	pos.y += force.y ;
	phase = ofLerp(phase, 0, poxHealth / targetTimer);
	isToroPox = true;
	}

	if (isToroPox) {
		phase = ofMap(cos(ofGetElapsedTimef() + index), -1, 1, radius*0.5, 1.25*radius);
		if (uniformPhase) { phase = ofMap(cos(ofGetElapsedTimef()), -1, 1, radius*minWaxWane, maxWaxWane*radius); }
		if (isFullyGrown && !isTargeted) {
			phase = ofMap(cos(ofGetElapsedTimef()), -1, 1, radius*0.75, 1.75*radius);
		}
	}

	if (isTargeted) {
		float d = ofDist(bounds1.x, bounds1.y, bounds2.x, bounds2.y);
		ofVec2f m = (bounds1 + bounds2) / 2; // must do it this way! don't use .middle
		float l = (ofDist(m.x, m.y, pos.x, pos.y));
		isDying = (l > d) && (( (pos.x > max(bounds2.x, bounds1.x) + (ofGetWidth()*0.05) || pos.x < min(bounds2.x, bounds1.x) - (ofGetWidth()*0.05))));
	}
		// is it being targeted?
	if (isDying) {
		targetTimer += ofGetLastFrameTime();
	} // life
	isDefeated = (targetTimer >= poxHealth);
	
	if (!isDefeated && !isDying){
	isDefeated=(pos.y <= min(bounds1.y,bounds2.y)-(ofGetHeight()*0.15))  || (pos.y >= ofGetHeight()*0.8 || radius== 0);
	}


	d1 = pos - bounds1; // direction vector with distance between joint1 and the cell's position
	d2 = pos - bounds2; // direction vector with distance between joint2 and the cell's position 
	d1.normalize();
	d2.normalize();


	if (toropox.size()) {
		frameNum = ((int)(ofGetElapsedTimef() * frameRate) + offsetFrame) % toropox.size();
		toropoxFrame = &toropox[frameNum];
	} 
}


void cell::draw() {

	ofPushStyle();
	// shadowpox
	
			ofSetColor(main_col);
		

			if (!isDying) {
				ofDrawCircle(pos.x, pos.y, radius);

				if ((isToroPox || isTargeted)) {
					/* (toropox.size()) {

						ofPushMatrix();

						ofTranslate(pos.x, pos.y);
						ofRotate((ofGetFrameNum()*-0.01*rotateSpeed) + index, 0, 0, 1);
						toropoxFrame->draw(0,0, radius, radius);
						ofPopMatrix();
					} */
					/*ofPushMatrix();
					ofTranslate(pos.x, pos.y);
					if (uniformRotation) {
						ofRotate((ofGetFrameNum()*-0.01*rotateSpeed), 0, 0, 1);
					}

					else {
						ofRotate((ofGetFrameNum()*-0.01*rotateSpeed) + index, 0, 0, 1);
					}

					ofTranslate(phase, phase);
					ofDrawCircle(0, 0, radius);
					ofPopMatrix(); */
				}
			//	else {
					//ofDrawCircle(pos.x, pos.y, radius);
			//	}
			}

			else {

		ofPushMatrix();

		float TargetY = ((1 - (targetTimer/poxHealth)) * pos.y) + ((targetTimer/poxHealth)*ofGetWindowHeight())+sin(ofGetFrameNum()*0.001);
		float translationX = 25*cos(ofGetFrameNum()*0.001);
		float size = ofMap(radius, minRadius, maxRadius, 0.3, 0.5, true);

		if (TargetY >= ofGetWindowHeight()) { isDefeated = TRUE;
		isDying = false;
		}

		ofPushMatrix();
		ofTranslate(pos.x-5-translationX,TargetY);
		ofScale(size); // general resize
		ofRotate((ofGetFrameNum()*-0.1*rotateSpeed) + index, 0, 0, 1);
		ofScale(1-(targetTimer/poxHealth)); // shrinking resize
		hornLeft->draw();
		ofPopMatrix();
		ofTranslate(pos.x+5+translationX, TargetY);
		ofScale(size);
		ofRotate((ofGetFrameNum()*0.1*rotateSpeed) + index, 0, 0, 1);
		ofScale(1 - (targetTimer / poxHealth));
		hornRight->draw();
		ofPopMatrix();
	}
	ofPopStyle();

}

bool cell::circleIntersect(ofVec2f center, float r) {
	
	float distanceX = pos.x - center.x;
	float distanceY = pos.y - center.y;
	float radiusSum = r + radius;
	return distanceX * distanceX + distanceY * distanceY <= radiusSum * radiusSum;

}





