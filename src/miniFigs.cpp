#include "miniFigs.h"

#define FIG_WIDTH 42
#define FIG_HEIGHT 72

#define FIG_INFECTED_WIDTH 82
#define FIG_INFECTED_HEIGHT 82

vector<ofImage> miniFig:: initFigures;
vector<ofImage> miniFig::happyAnim;
vector<ofImage> miniFig:: sickAnim;
vector<ofImage> miniFig::happyAnimF;
vector<ofImage> miniFig::sickAnimF;
vector<ofImage> miniFig::vaccine_walking;
vector<ofImage> miniFig::vaccine_walkingF;
vector<ofImage> miniFig::vaccine_waving;
ofImage  miniFig:: deathImg;
int miniFig::DeathScore = 0; 

float miniFig::countrySeverityRate = 80.0;
miniFig::miniFig() {
	offset = ofRandomuf(); // that each figure does not walk the same
	Velocity = ofVec2f(ofRandom(-1.25, 1.25), ofRandom(-1.25,1.25));
	hasEveryBeenInfected = false; 
	lifeTime = 0;
}

miniFig::~miniFig() {}

void miniFig::setup(ofVec2f loc,int index) {
	offset = ofRandomuf(); // that each figure does not walk the same
	Velocity = ofVec2f(ofRandom(-1,1), ofRandom(-1,1));


	// setup display images
	if (initFigures.size()) {
		displayImage = &(initFigures[index%initFigures.size()]); // select image from start of array
	}
	
	if (happyAnim.size()) {
		offsetFrame = (int) happyAnim.size() * offset;

	}
	boundingBox.setFromCenter(loc, FIG_WIDTH, FIG_HEIGHT);

	lifeTime = 0;
	checkHealth = false; 

}
void miniFig::update() {
/// move figure

	if (!(currentState==DEAD || currentState ==VACCINE_WAVE))
	move();
	// update the health
	updateHealth();
	// change animation frame
	animate();
}


void miniFig::updateHealth() {
	if (currentState==SICK && checkHealth==false) {
		// how long as the figure been sick
		if (ofGetElapsedTimef() - lifeTime > lifespan) {
			currentState = (ofRandomuf() <= .2f) ? DEAD: SICK;
			checkHealth = true;

			if (currentState == DEAD) {//how many people have died 
				DeathScore++;
			}
		}
	}

	else if (currentState == VACCINE_WALK && checkHealth == false) {
		if (ofGetElapsedTimef() - lifeTime > lifespan) {


			DeathScore += (ofRandomuf() <= .2f) ?  1 : 0;
			currentState = VACCINE_WALK;
			checkHealth = true;
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

		boundingBox.setWidth(FIG_INFECTED_WIDTH);
		boundingBox.setHeight(FIG_INFECTED_HEIGHT);
		if (Velocity.x<0)
			displayImage = &(sickAnimF[frameNum]); // healthy flipped
		else displayImage = &(sickAnim[frameNum]); // sick
		break;
	case DEAD: {
		boundingBox.setWidth(FIG_INFECTED_WIDTH);
		boundingBox.setHeight(FIG_INFECTED_HEIGHT);
		displayImage = &deathImg;
		break; }
	case VACCINE_WAVE: {
		frameNum = ((int)(ofGetElapsedTimef() * sequenceFPS) ) % vaccine_waving.size();
		if (frameNum > vaccine_waving.size()-5) {
			currentState = VACCINE_WALK;
			offsetFrame = (int)vaccine_walking.size() * offset;
			return;
		}
		boundingBox.setWidth(FIG_INFECTED_WIDTH);
		boundingBox.setHeight(FIG_INFECTED_HEIGHT);
		displayImage = &(vaccine_waving[frameNum]); }
		break;
	case VACCINE_WALK: {
		frameNum = ((int)(ofGetElapsedTimef() * sequenceFPS) + offsetFrame) % vaccine_walking.size();

		boundingBox.setWidth(FIG_INFECTED_WIDTH);
		boundingBox.setHeight(FIG_INFECTED_HEIGHT);
		if (Velocity.x < 0)
			displayImage = &(vaccine_walkingF[frameNum]); // healthy flipped
		else displayImage = &(vaccine_walking[frameNum]); // healthy
		break; 
	}
	}
	// dead
}

void miniFig::move() {

	int movementOffsetY= (currentState==HAPPY)?FIG_HEIGHT:FIG_INFECTED_HEIGHT / 2;

	int movementOffsetX= (currentState== HAPPY)?FIG_WIDTH:FIG_INFECTED_WIDTH / 2;
	
	if (boundingBox.getLeft()+ movementOffsetX< 0|| boundingBox.getRight()- movementOffsetX> ofGetWidth()) Velocity.x *= -1; // flip to keep within the screen
	if (boundingBox.getBottom()- movementOffsetY> ofGetHeight() || boundingBox.getTop()+ movementOffsetY < 0) Velocity.y *= -1;
	boundingBox.position += Velocity;
}

void miniFig::getInfected(ofVec2f& point, int &score, bool isVaccine) {
	if (!isVaccine) {

		
		if (boundingBox.inside(point)
			&& currentState != SICK
			&& ofRandomuf() < countrySeverityRate
			&& hasEveryBeenInfected == false) {

			changeState(SICK);
			boundingBox.setWidth(FIG_INFECTED_WIDTH);
			boundingBox.setHeight(FIG_INFECTED_HEIGHT);

			lifeTime = ofGetElapsedTimef();

			Velocity *= 0.25;

			hasEveryBeenInfected = true;
			score++;
			if (sickAnim.size()) {
				offsetFrame = (int)sickAnim.size() * offset;
			}
		}
	}
	else {
		if (boundingBox.inside(point)
			&& (currentState != VACCINE_WAVE || currentState != VACCINE_WALK)
			&& ofRandomuf() < countrySeverityRate
			&& hasEveryBeenInfected == false) {

			changeState(VACCINE_WAVE);
			boundingBox.setWidth(FIG_INFECTED_WIDTH);
			boundingBox.setHeight(FIG_INFECTED_HEIGHT);

			lifeTime = ofGetElapsedTimef();

			Velocity *= 0.25;

			hasEveryBeenInfected = true;
			score++;
			if (vaccine_walking.size()) {
				offsetFrame = (int)vaccine_walking.size() * offset;
			}
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
	
	if (currentState == DEAD) {
		return;
	}

	if (currentState == SICK) {
		return;
	}

	if (currentState == VACCINE_WALK) {
		return;
	}

	if (currentState == VACCINE_WAVE) {
		return;
	}

		currentState = s;
	

}