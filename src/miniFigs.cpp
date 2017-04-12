#include "miniFigs.h"

#define FIG_WIDTH 42
#define FIG_HEIGHT 72

#define FIG_DEAD_WIDTH 72
#define FIG_DEAD_HEIGHT 24

vector<ofImage> miniFig:: initFigures;
vector<ofImage> miniFig::happyAnim;
vector<ofImage> miniFig:: sickAnim;
vector<ofImage> miniFig::happyAnimF;
vector<ofImage> miniFig::sickAnimF;
ofImage  miniFig:: deathImg;

float miniFig::countrySeverityRate = 80.0;
miniFig::miniFig() {
	offset = ofRandomuf(); // that each figure does not walk the same
	Velocity = ofVec2f(ofRandom(-1, 1), ofRandom(-1,1));
}

miniFig::~miniFig() {}

void miniFig::setup(ofVec2f loc,int index) {
	offset = ofRandomuf(); // that each figure does not walk the same
	Velocity = ofVec2f(ofRandom(-1,1), ofRandom(-1,1));


	// setup display images
	if (initFigures.size()) {
		displayImage = &(initFigures[index%initFigures.size()]); // select random image from start of array
	}
	
	if (happyAnim.size()) {
		offsetFrame = (int) happyAnim.size() * offset;

	}
	boundingBox.setFromCenter(loc, FIG_WIDTH, FIG_HEIGHT);

}
void miniFig::update() {
/// move figure

	if (currentState!=DEAD)
	move();
	// update the health
	updateHealth();
	// change animation frame
	animate();
}


void miniFig::updateHealth() {
	if (currentState==SICK) {
		// how long as the figure been sick
		if (ofGetElapsedTimef() - lifeTime > lifespan) {
			currentState = (ofRandomuf() > .5f) ? HAPPY : DEAD;
		}
	}
}

void miniFig :: animate() {

	switch (currentState) { 
	case HAPPY:
		frameNum =( (int)(ofGetElapsedTimef() * sequenceFPS) + offsetFrame)% happyAnim.size();
		boundingBox.setWidth(FIG_WIDTH);
		boundingBox.setHeight(FIG_HEIGHT);
		if (Velocity.x<0)
			displayImage = &(happyAnimF[frameNum]); // healthy flipped
		else displayImage = &(happyAnim[frameNum]); // healthy
		break;
	case SICK:
		frameNum = ((int)(ofGetElapsedTimef() * sequenceFPS)+ offsetFrame) % sickAnim.size();

		boundingBox.setWidth(82);
		boundingBox.setHeight(82);
		if (Velocity.x<0)
			displayImage = &(sickAnimF[frameNum]); // healthy flipped
		else displayImage = &(sickAnim[frameNum]); // sick
		break;
	case DEAD: 
		boundingBox.setWidth(FIG_DEAD_WIDTH);
		boundingBox.setHeight(FIG_DEAD_HEIGHT);
		displayImage = &deathImg;
		break;
	}
	// dead
}

void miniFig::move() {

	if (boundingBox.getLeft() < 0|| boundingBox.getRight() > ofGetWidth()) Velocity.x *= -1; // flip to keep within the screen
	if (boundingBox.getBottom()> ofGetHeight() || boundingBox.getTop() < 0) Velocity.y *= -1;
	boundingBox.position += Velocity;

}

void miniFig::getInfected(ofVec2f point, int &score) {

	if (boundingBox.inside(point) && currentState!=SICK && ofRandomuf()<countrySeverityRate) {

		currentState = SICK;
		boundingBox.setWidth(82);
		boundingBox.setHeight(82);

		lifeTime = ofGetElapsedTimef();

		Velocity *= 0.25;
		score++;
		if (sickAnim.size()) {
			offsetFrame = (int)sickAnim.size() * offset;
		}
	}
}



void miniFig::draw() {
		displayImage->draw(boundingBox);
}


void miniFig::changeState(STATE s) {
	if (s == SICK && currentState != SICK) {
		Velocity *= 0.25;
	}

	if (currentState != SICK) {
		currentState = s;
	}

}