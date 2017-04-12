#pragma once
#include "country.h"
#define FLAG_WIDTH 115
#define FLAG_HEIGHT 150

vector<ofImage> Country::countriesFlags;

Country::Country() {}
Country::~Country() {}

void Country::setup(int id2, ofVec2f loc) {

	id = id2;
	// setup display images
	if (countriesFlags.size()) {
		displayImage = &(countriesFlags[id%countriesFlags.size()]); // select random image from start of array
	}
	boundingBox.setFromCenter(loc, FLAG_WIDTH, FLAG_HEIGHT);
}
bool Country::isOver(ofVec2f point) {
	return boundingBox.inside(point);
}
bool Country::isSelected(ofVec2f point, int handstate) {
	return isOver(point);
}
void Country::draw() {
	displayImage->draw(boundingBox);
}