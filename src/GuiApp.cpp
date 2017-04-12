#include "GuiApp.h"

#define GUIWIDTH 300

void GuiApp::setup(){
	gui.setDefaultWidth(GUIWIDTH);

	CVParam.setName("Game Parameters");
	CVParam.add(sweeping.set("Hand Sweeping", true));
	CVParam.add(handSensitivity.set("Hand Sensitivity", 15, 0, 100));
	CVParam.add(detectionRange.set("Detection Range", 20, 10, 500));
	CVParam.add(gamePlayLength.set("Game Play Length (x30Secs)", 3, 1, 120));
	CVParam.add(skipIntroText.set("Skip Intro Text", false));
	CVParam.add(skipScreenSaver.set("Skip screensaver", false));
	CVParam.add(flipText.set("Mirror Text", true));
	CVParam.add(skeletonFlipX.set("Flip Player(X axis)", true));
	CVParam.add(skeletonFlipY.set("Flip Player(Y axis)", false));
	

	genParam.setName("Parameters");
	genParam.add(initCount.set("Inital pox count", 1, 1, 50));
	genParam.add(maxCount.set("Max pox count",50,25,400));
	genParam.add(minSize.set("Inital size",1,0,20));
	genParam.add(maxSize.set("Max size", 5,0,100));
	genParam.add(growthRate.set("Growth Rate",100, 5, 100));
	genParam.add(migrationSpeed.set("Migration Speed", 0.25, 0, 2));
	genParam.add(distanceX.set("Migration spread X axis", 5, 5, 500));
	genParam.add(distanceY.set("Migration spread Y axis", 5, 5, 500));
	genParam.add(buddingSpeed.set("Budding Rate per Sec", 5, 1, 50));
	genParam.add(toropoxMode.set("Toropox mode", true));

	toropoxParam.setName("Toropox Parameters");
	toropoxParam.add(rotationSpeed.set("RotationSpeed", 5, 1, 10));
	toropoxParam.add(phaseSpeed.set("Phase Speed",5, 1, 20));
	toropoxParam.add(minWaxWane.set("Min Phase",0.25,0.1,0.5));
	toropoxParam.add(maxWaxWane.set("Max Phase", 0.5,0.5,1));
	toropoxParam.add(uniformPhase.set("Uniform Toropox Phase", true));
	toropoxParam.add(uniformRotation.set("Uniform Toropox Rotation",true));

/*	CVParam.setName("CV Parameters");
	CVParam.add(nearClipping.set("Minimum distance", 1000.0f, 0, 8000.0f));
	CVParam.add(farClipping.set("Maximum distance", 3500.0f, 0, 8000.0f));
	CVParam.add(threshold.set("Threshold", 50, 0, 200));
	CVParam.add(blur.set("Blur", 13, 0,50));
	CVParam.add(erosionStr.set("Errosion Strength", 5, 0, 20));
	CVParam.add(dilationStr.set("Dilation Strength", 5, 0, 20));
	CVParam.add(minCVblobSize.set("min blob size",0.01,0,2));
	CVParam.add(maxCVblobSize.set("Max blob size",25,0,30));
	CVParam.add(maxNumBlobs.set("Max blob detected", 8, 0, 10));
*/	
	CVParam.add(genParam);
	CVParam.add(toropoxParam);
	//genParam.add(CVParam);

	gui.setup(CVParam);
	//gui.add(debug.set("[D]ebug", false));

	gui.add(skeleton.set("Skeleton Debug", false));
	//gui.add(fullscreen.set("[F]ullscreen", false));

	//gui.add(mouseMovement.set("MouseControl", false));
	//gui.add(openingSeq.set("Opening", true));
	//gui.add(beginAnimation.set("Opening Animation", false));

	//gui.add(closingSeq.set("Closing animation", false));
	//gui.add(fade.set("fade", false));
	gui.add(spawn.set("Spawn Pox", true));
	//gui.add(autoSelect.set("Auto Select Patients", true));
	//gui.add(availablePatients.setup("Available Patients", "0"));

	//gui.add(xPos.set("X axis", 0.5, 0, 1));
	//gui.add(yPos.set("Y axis", 0.5, 0, 1));
	//gui.add(scale.set("scale", 7,1,10));
	ofBackground(0);

	ofSetVerticalSync(false);

	gui.setPosition((ofGetWidth() - gui.getWidth()) / 2, (ofGetHeight() - gui.getHeight()) / 2);

	gui.loadFromFile("settings.xml");


	ofSetWindowTitle("Shadowpox v1.0 - Settings Window");

}

void GuiApp::update(){
}

void GuiApp::draw(){
	gui.draw();
}

//--------------------------------------------------------------
void GuiApp::keyReleased(int key) {
	switch (key) {
	case 's':
		gui.saveToFile("settings.xml");
		cout << "Presets saved" << endl;
		break;
	case OF_KEY_RETURN:
		gui.saveToFile("settings.xml");
		cout << "Presets saved" << endl;
		break;
	case 'a':
		gui.loadFromFile("settings.xml");
		cout << "Save settings loaded" << endl;
		break;
	
		case 'T':
		case 't':
			toropoxMode = !toropoxMode;
			break;
		case 'b':
		case 'B':
			spawn = !spawn;
			break;
}
}


