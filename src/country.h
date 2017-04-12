#pragma once
#include "ofMain.h"
#include "CountryCode.h"
class Country {

public :
	Country();
	~Country();

	void setup(int,ofVec2f loc);
	bool isSelected(ofVec2f, int);
	void draw();

	int id;
	static void LoadImage(string filename) {
		ofImage img;
		img.load(filename);
	}

protected:
	ofImage* displayImage;
	static vector <ofImage> countriesFlags;
	CountryCode selectedCountry = CountryCode::NONE; 

private:
	ofRectangle boundingBox;
	int checked = 0; 
	bool isOver(ofVec2f point);

};

