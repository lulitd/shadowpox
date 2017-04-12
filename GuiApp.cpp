#include "GuiApp.h"

void GuiApp::setup() {

	genParam.setName("Parameters");

	genParam.add(initCount.set("Inital pox count", 5, 1, 20));
	genParam.add(maxCount.set("Max pox count", 50, 25, 500));
	genParam.add(minSize.set("Inital size", 1, 1, 10));
	genParam.add(maxSize.set("Max size", 5, 5, 20));
	genParam.add(migrationSpeed.set("Migration Speed", 5, 1, 10));
	genParam.add(buddingSpeed.set("Budding Speed", 5, 1, 10));
	genParam.add(growthRate.set("Growth Rate", 5, 1, 10));
	genParam.add(toropoxMode.set("Toropox mode", true));

	toropoxParam.setName("Toropox Parameters");
	toropoxParam.add(phaseSpeed.set("Phase Speed", 5, 1, 20));
	toropoxParam.add(rotationSpeed.set("RotationSpeed", 5, 1, 10));
	toropoxParam.add(minWaxWane.set("Min Wax Wane", 0.25, 0.25, 0.5));
	toropoxParam.add(maxWaxWane.set("Max Wax Wane", 0.5, 0.5, 0.75));

	genParam.add(toropoxParam);
	genParam.add(debug.set("[D]ebug", false));
	genParam.add(fullscreen.set("[F]ullscreen", false));
	gui.setup(genParam);
	gui.add(resetButton.setup("Clear"));
	ofBackground(0);
	ofSetVerticalSync(false);

	gui.loadFromFile("settings.xml");

}

void GuiApp::update() {
}

void GuiApp::draw() {
	gui.draw();
}


