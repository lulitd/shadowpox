#include "ofApp.h"

#define DEPTH_WIDTH			512
#define DEPTH_HEIGHT		424
#define	MIN_DEPTH_CLIP		50.0f
#define MAX_DEPTH_CLIP		10000.0f

#define BACKGROUND_COLOR 0
#define FIGURE_COLOR 255 
#define FIGURE_COLOR_TRANSITION 200 


//--------------------------------------------------------------
void ofApp::setup() {
	text.load("font/euro.ttf", 55, true, true, false, false, 72);
	textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
	textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_TOP;

	// KINECT STUFF
	kinect.open();
	kinect.initDepthSource();
	kinect.initBodySource();
	numOfBodiesTracked = 0;

	// LOAD RULR CALIBRATION FILE
	auto result = ofSystemLoadDialog("Select ofxRay::Camera file for this projector");
	if (!result.bSuccess) {
		ofExit();
	}
	ifstream filein(ofToDataPath(result.filePath), ios::binary | ios::in);
	filein >> this->projector;
	filein.close();

	// todo move all image loading to new thread!
	// loading in all the svgs 
	// read the directory for the images we know that they are named in seq
	ofDirectory dir;
	dir.allowExt("svg");
	int nFiles = dir.listDir("svg");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << (ofToString(nFiles)) << " horns svgs";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			cell::addFile(filePath, cell::HORN);
		}
	}
	else ofLogWarning("shadowpox","Could not find folder: svg");


	dir.allowExt("png");
	nFiles = dir.listDir("toropox");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << (ofToString(nFiles)) << " toropox frames";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			cell::addFile(filePath, cell::TOROPOX);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder: toropox");

	nFiles = dir.listDir("anim/sick");
	ofLogNotice("shadowpox") << "loading sick " << nFiles << " frames";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);

			miniFig::loadImage(filePath, miniFig::SICK, false);
		}
	}
	else ofLogWarning("shadowpox","Could not find folder:anim/sick");

	nFiles = dir.listDir("anim/f_sick");
	ofLogNotice("shadowpox") << "loading  flipped sick" << nFiles << " frames";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);

			miniFig::loadImage(filePath, miniFig::SICK, true);
		}
	}
	else ofLogWarning("shadowpox","Could not find folder:anim/f_sick");

	nFiles = dir.listDir("anim/healthy");
	ofLogNotice("shadowpox") << "loading healthy" << nFiles << " frames";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::HAPPY, false);
		}
	}
	else ofLogWarning("shadowpox","Could not find folder:anim/healthy");

	nFiles = dir.listDir("anim/f_healthy");
	ofLogNotice("shadowpox") << "loading flipped healthy" << nFiles << " frames";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::HAPPY, true);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:anim/f_healthy");

	nFiles = dir.listDir("anim/figure");
	ofLogNotice("shadowpox") << "loading figure" << nFiles << " stills";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::START, false);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:anim/figure");

	nFiles = dir.listDir("anim/dead");
	ofLogNotice("shadowpox") <<"dead "<< nFiles << " image";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::DEAD, false);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:anim/dead");

	nFiles = dir.listDir("anim/vaccine_walking");
	ofLogNotice("shadowpox") << "vaccine walking " << nFiles << " images";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::VACCINE_WALK, false);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:anim/vaccine_walking");

	nFiles = dir.listDir("anim/f_vaccine_walking");
	ofLogNotice("shadowpox") << "vaccine walking flipped " << nFiles << " images";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::VACCINE_WALK, true);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:anim/f_vaccine_walking");

	nFiles = dir.listDir("anim/vaccine_waving");
	ofLogNotice("shadowpox") << "vaccine_waving " << nFiles << " image";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			string filePath = dir.getPath(i);
			miniFig::loadImage(filePath, miniFig::VACCINE_WAVE, false);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:anim/vaccine_waving");

	worldMap.load("choices/world-map_flipped.png");

	nFiles = dir.listDir("choices/map_bounds/flipped");
	dir.sort();
	ofLogNotice("shadowpox")<<"loading " << nFiles << " REGIONS Bounds" ;
	if (nFiles) {
		for (int j = 0; j < dir.numFiles(); j++) {

			ofxSVG bounds;

			bounds.load(dir.getPath(j));

			for (int i = 0; i < bounds.getNumPath(); i++) {
				ofPath p = bounds.getPathAt(i);
				p.setPolyWindingMode(OF_POLY_WINDING_ODD);

				// convert shape path to polyline
				ofPolyline  contour;
				contour.addVertices(p.getOutline().at(0).getVertices());

				// close the shape by adding a last vertex which is the first vertex of the shape
				contour.addVertex(p.getOutline().at(0).getVertices().at(0));

				boundaries.push_back(contour);
			}
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/map_bounds");

	nFiles = dir.listDir("choices/regions");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " REGIONS Labels";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			ofImage r;
			r.load(dir.getPath(i));
			r.setAnchorPoint(r.getWidth() / 2, r.getHeight() / 2);
			regions.push_back(r);
		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/regions");

	nFiles = dir.listDir("choices/flags");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " flags";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			ofImage r;
			r.load(dir.getPath(i));
			r.setAnchorPercent(0.5, 0.5);
			flags.push_back(r);

		}
	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/flags");

	nFiles = dir.listDir("choices/hands");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " hand icons";
	if (nFiles) {
		for (int i = 0; i < dir.numFiles(); i++) {
			// get file paths to load into cells
			ofImage r;
			r.load(dir.getPath(i));
			r.setAnchorPercent(0.5, 0.5);
			hands.push_back(r);

		}
		handGuideImage = &(hands [0]);
	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/hands");

	nFiles = dir.listDir("choices/vaccine");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " vaccine icons";
	if (nFiles) {

		vaccineButton.image.load(dir.getPath(0));
		vaccineButton.index = 0;
		vaccineButton.bounds.setFromCenter(0, 0, vaccineButton.image.getWidth(), vaccineButton.image.getHeight());
		vaccineButton.size = ofPoint(vaccineButton.image.getWidth(), vaccineButton.image.getHeight());
		virusButton.image.load(dir.getPath(1));
		virusButton.index = 1;
		virusButton.bounds.setFromCenter(0, 0, virusButton.image.getWidth(), virusButton.image.getHeight());
		vaccineButton.size = ofPoint(vaccineButton.image.getWidth(), vaccineButton.image.getHeight());

	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/vaccine");

	flagOrgSize.x = flags[0].getWidth();
	flagOrgSize.y = flags[0].getHeight();


	screenSaverPlayer.load("clip/opening.mp4");
	screenSaverPlayer.isLoaded();
	screenSaverPlayer.setLoopState(ofLoopType::OF_LOOP_NORMAL);
	screenSaverPlayer.play();
	introTextPlayer.load("clip/intro_text.mp4");
	introTextPlayer.setLoopState(ofLoopType::OF_LOOP_NONE);

	patients.reserve(6); // maximum skeleton tracking of kinect

	for (int i = 0; i < 6; i++) {
		cellCluster cluster;
		cluster.isToroPox = gui->toropoxMode;
		cluster.minRadius = gui->minSize;
		cluster.maxRadius = gui->maxSize;
		cluster.growth = gui->growthRate;
		cluster.rotateSpeed = gui->rotationSpeed;
		cluster.phaseSpeed = gui->phaseSpeed;
		cluster.speed = gui->migrationSpeed;
		cluster.maxCells = gui->maxCount;
		cluster.initCells = gui->initCount;
		cluster.maxCells = gui->maxCount;
		cluster.initCells = gui->initCount;
		cluster.minWaxWane = gui->minWaxWane;
		cluster.maxWaxWane = gui->maxWaxWane;
		cluster.uniformPhase = gui->uniformPhase;
		cluster.uniformRotation = gui->uniformRotation;
		cluster.setup();

		cluster.handStateL = HandState_Unknown;
		cluster.handStateR = HandState_Unknown;
		patients.push_back(cluster);
	}

	spawnTimer = 0;
	cellCluster::kinectTracking = true;

	ofSetWindowTitle("Shadowpox - Display Window");
	ofSetVerticalSync(true);
	ofSetCircleResolution(360);
	ofSetBackgroundColor(BACKGROUND_COLOR);

	skeletonFBO.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA);


	for (int rows = 0; rows < 10; rows++) {
		for (int columns = 0; columns < 10; columns++) {
			ofVec2f loc = ofVec2f((rows * 75) + 100, (columns * 100) + 100);
			leftLocations.push_back(loc);
		}
	}


	for (int rows = 0; rows < 10; rows++) {
		for (int columns = 0; columns < 10; columns++) {
			ofVec2f loc = ofVec2f(ofGetScreenWidth() - (rows * 75) - 100, ofGetScreenHeight() - (columns * 100) - 100);
			rightLocations.push_back(loc);
		}
	}


	for (int columns = 0; columns < 4; columns++) {
		for (int rows = 0; rows < 8; rows++) {
			ofVec2f loc = ofVec2f((rows * 140) + 500, (columns * 175) + 250);
			flagLocations.push_back(loc);
		}
	}

	// create our map datastructure to hold all the pairings. 
	for (int i = 0; i < 11; i++) {
		countriesInRegion.insert(pair<int, vector<countryInfo>>(i, vector<countryInfo>()));
	}

	/* Columns
	0.Region Code int
	1.Country Number  int
	2.Country	string
	3.Starting Vaccination Rate float
	4.Starting Severity Score float
	5.Change in Severity for 1 Unit Decrease in Vaccination Rate	float
	6.Maximum Severity Score	float
	7.Current Severity	float
	8.Current Vaccination Rate float
	*/


	if (sourceData.load("csv/vax_serverity.csv")) {
		dataIsLoaded = true;
		for (auto row : sourceData) {
			countryInfo country;
			country.index = row.getInt(1);
			country.flagImage = &flags[country.index];
			country.bounds.setFromCenter(0, 0, flagOrgSize.x, flagOrgSize.y);
			countriesInRegion[row.getInt(0)].push_back(country);
		}
	} else ofLogWarning("shadowpox", "Could not find csv/vax_serverity.csv");

	fileName = "d:/" + ofGetTimestampString("%Y-%m-%d-%H-%M") + "shadowpox.csv";

	logData.createFile(fileName);
	ofLogNotice("shadowpox", "file created:"+ fileName);

	ofxCsvRow header;
	header.addString("Country Choice");
	header.addString("Date");
	header.addString("Vaccination Choice");
	header.addString("Score");
	header.addString("Deaths");

	// time // country // choice // score
	logData.addRow(header);

	
	currentState = sequenceMode::SCREENSAVER;



}

//--------------------------------------------------------------
void ofApp::update() {
	
	float now = ofGetElapsedTimef();

	//HACK to get it to fullscreen on second monitor. window must be positioned on second monitor first, then fullscreen is called. 
	if (ofGetFrameNum() == 20   ) {
		ofSetWindowPosition(1920, 0);
	}
	if (ofGetFrameNum() == 25) {
		ofSetFullscreen(true);
	}

	this->kinect.update();
		
	//if (gui->openingSeq) currentState = sequenceMode::COUNTRYCHOICE;



	switch (currentState) {
	case sequenceMode::SCREENSAVER: {
		ofHideCursor();
		if (!gui->skipScreenSaver){
		if (!screenSaverPlayer.isPlaying()) screenSaverPlayer.play();
		if (kinect.isFrameNew()) {
			auto bodies = kinect.getBodySource()->getBodies();
			  
			const auto & bonesDictionary = ofxKinectForWindows2::Data::Body::getBonesAtlas();

			numOfBodiesTracked = 0;
			for (auto body : bodies) {
				if (body.tracked) {
					numOfBodiesTracked++;
					currentState = sequenceMode::INTRO;
					screenSaverPlayer.stop(); // STOP SCREENSAVER!
					introTextPlayer.play();// PLAY INTRO
					return;
				}
			}
		}
		screenSaverPlayer.update();
		}
		else {
			currentState = (gui->skipIntroText)?sequenceMode::COUNTRYCHOICE : sequenceMode::INTRO; 
		}
		break; }
	case sequenceMode::INTRO: {
		ofHideCursor();
		introTextPlayer.update();
		if (introTextPlayer.getIsMovieDone() || gui->skipIntroText) {
			currentState = sequenceMode::COUNTRYCHOICE;
			introTextPlayer.stop();// stop INTRO
			return;
		}
		break;
	}
	case sequenceMode::COUNTRYCHOICE: {

		textAlignFlag = 0;
		textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
		textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_TOP;


		if (!MANUAL_SELECTION) {
			if (kinect.isFrameNew()) {
				auto bodies = kinect.getBodySource()->getBodies();
				const auto & bonesDictionary = ofxKinectForWindows2::Data::Body::getBonesAtlas();

				projector.beginAsCamera(gui->skeletonFlipX, gui->skeletonFlipY);

				numOfBodiesTracked = 0;

				for (auto body : bodies) {
					if (body.tracked) {
						numOfBodiesTracked++;
						rightHand.pos = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_HandTipRight].getPosition());
						rightHand.prevHandState = rightHand.currentHandState;

						if (body.rightHandState == HandState::HandState_Closed || body.rightHandState == HandState::HandState_Open)
							rightHand.currentHandState = body.rightHandState;

						if (rightHand.prevHandState == rightHand.currentHandState) {
							rightHand.HandStateCounter++;
						}
						else { rightHand.HandStateCounter = 0; }

						break;
					}
				}
				projector.endAsCamera();
				if (selectedCountry == nullptr && selectedRegion == nullptr) {
					chooseRegion = true;
				}
				// show regions
				if (chooseRegion || selectedRegion == nullptr) {

					displayText = "Please choose your region";

					for (int i = 0; i < boundaries.size(); i++) {
						// replace mouseX and mouseY with body.sel;
						if (boundaries[i].inside(rightHand.pos)) {
							regSel = i;
							break;
						}
						else { regSel = -1; }
					}

					if (regSel != -1) {
						displayRegion = &regions[regSel];
						if (rightHand.currentHandState == HandState::HandState_Closed && rightHand.HandStateCounter > 50) {
							chooseRegion = false;
							selectedRegion = &countriesInRegion.at(regSel);
							rightHand.HandStateCounter = 0;
						}
					}
					else {
						rightHand.HandStateCounter = 0;
					}
				}
				else {
					for (unsigned n = 0; n < selectedRegion->size(); n++) {
						if (selectedRegion->at(n).bounds.inside(rightHand.pos)) {
							selectedCountry = &(selectedRegion->at(n));
							break;
						}
					}
					if (rightHand.currentHandState == HandState::HandState_Closed && rightHand.HandStateCounter > handPointerStateCounterThres) {

						float f = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);

						if (f + 1 > 80) f--;
						sourceData[(selectedCountry->index) + 1].setFloat(sourceCol::CURRENTVACCINATIONRATE, f);
						currentState = sequenceMode::VACCINECHOICE;
						displayText = "Shadowpox vaccination rate in \n" + sourceData[(selectedCountry->index) + 1].getString(sourceCol::COUNTRY) + ": \n" + ofToString(f, 2) + "%";
						return;
					}

				}
			}
		}
		else {
			ofShowCursor();
			drawHuman(FIGURE_COLOR_TRANSITION);
			if (selectedCountry == nullptr && selectedRegion == nullptr) {
				chooseRegion = true;
				countrySelect = false;
			}
			// show regions
			if (chooseRegion || selectedRegion == nullptr) {

				displayText = "Please choose your region";

				for (int i = 0; i < boundaries.size(); i++) {
					// replace mouseX and mouseY with body.sel;
					if (boundaries[i].inside(mouseX, mouseY)) {
						regSel = i;
						break;
					}
					else { regSel = -1; }
				}

				if (regSel != -1) {
					displayRegion = &regions[regSel];

					if (ofGetMousePressed()) {
						chooseRegion = false;
						selectedRegion = &countriesInRegion.at(regSel);

						transitionTimer = now;
						countrySelect = true;

						for (unsigned n = 0; n < selectedRegion->size(); n++) {

							selectedRegion->at(n).bounds.setPosition(flagLocations[n]);
						}
					}

				}
			}
			else if (countrySelect) {

				displayText = "Please select your Country";
				for (unsigned n = 0; n < selectedRegion->size(); n++) {

					if (selectedRegion->at(n).bounds.inside(mouseX, mouseY)) {
						selectedCountryInRegion = n;

						if (ofGetMousePressed() && (now - transitionTimer > 2)) {

							selectedCountry = &(selectedRegion->at(n));
							float f = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);

							if (f + 1 > 80) f--;
							sourceData[(selectedCountry->index) + 1].setFloat(sourceCol::CURRENTVACCINATIONRATE, f);
							currentState = sequenceMode::VACCINECHOICE;
							transitionTimer = now;
							choiceSeqVaccine = 0;

							figureSetup(figureAmount, f);
							displayText = "Shadowpox vaccination rate in \n" + sourceData[(selectedCountry->index)+1].getString(sourceCol::COUNTRY) + ": \n" + ofToString(f,0) + "%";

							vaccineButton.bounds.setPosition((100 + ((f / 10)+1) * 100) - vaccineButton.size.x / 2, ofGetHeight() / 2 - vaccineButton.size.y / 2);
							virusButton.bounds.setPosition(ofGetWidth() - vaccineButton.size.x / 2 - ((((100 - f) / 10) + 1) * 100), ofGetHeight() / 2 - vaccineButton.size.y / 2);
							
							textAlignmentX = (vaccineButton.bounds.getCenter().x + virusButton.bounds.getCenter().x) / 2;
							return;

						}
						break;
					}

					else {
						selectedCountryInRegion = -1;
					}


				}



			}

		}
		break;
	}
	case sequenceMode::VACCINECHOICE: {

		ofShowCursor();
		drawHuman(FIGURE_COLOR_TRANSITION);
		// CHOICE 	
		// show choice 
		if (choiceSeqVaccine == 0) {
			float currentVaccine = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);

			for (int k = 0; k < figures.size(); k++) {
				figures[k].changeState(miniFig::START);
			}
			// show text
			if (now - transitionTimer > 3) {
				if (vaccineButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
					isVaccine = true;
					choiceSeqVaccine = 1;
					transitionTimer = now;
					return;
				}
				if (virusButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
					isVaccine = false;
					choiceSeqVaccine = 1;
					transitionTimer = now;
					return;
				}
			}
		}

		else if (choiceSeqVaccine == 1) {
			float currentVaccine = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);
			float currentSeverityRate = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTSEVERITY);
			float changeInSeverity = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CHANGEINSEVERITY);
			(isVaccine) ? currentVaccine++ : currentVaccine--;
			(isVaccine) ? currentSeverityRate -= changeInSeverity : currentSeverityRate += changeInSeverity;
			//displayText = "Shadowpox vaccination rate \n in " + sourceData[selectedCountry->index].getString(sourceCol::COUNTRY) + ": \n" + ofToString(currentVaccine) + "%";

			miniFig::countrySeverityRate = currentSeverityRate;
			sourceData[(selectedCountry->index) + 1].setFloat(sourceCol::CURRENTSEVERITY, currentSeverityRate);
			sourceData[(selectedCountry->index) + 1].setFloat(sourceCol::CURRENTVACCINATIONRATE, currentVaccine);
			choiceSeqVaccine = 2;

			transitionTimer = now;
			return;
		}

		else if (choiceSeqVaccine == 2) {

			float currentVaccine = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);
			displayText = "Shadowpox vaccination rate \n in " + sourceData[(selectedCountry->index)+1].getString(sourceCol::COUNTRY) + ": \n"
				+ "Was: " + ofToString(((isVaccine)? currentVaccine-1:currentVaccine+1),0) + "% \n"
				+ "Now: " + ofToString(currentVaccine,0) + "% \n";
			figures.clear();
			figureSetup(figureAmount, (int)currentVaccine);

			for (int k = 0; k < figures.size(); k++) {
				figures[k].changeState(miniFig::START);
			}

			// show text
			// delay

			if (now - transitionTimer > 5) {				
				sourceData.save("csv/vax_serverity.csv");
				ofLogNotice("shadowpox") << "data saved!" << "csv/vax_serverity.csv";
				choiceSeqVaccine = 3;
				transitionTimer = now;
				vaccineDialog = 0;
				return;
			}
		}

		else if (choiceSeqVaccine == 3) {
			if (isVaccine) {
				switch (vaccineDialog) {
				case 0: {
					displayText = "The shadowpox vaccine is\n a weakened form of the virus.\n It is not infectious. \n \n The vaccine lets you safely practice\n fighting the disease.";
					if (now - transitionTimer > 10) { vaccineDialog = 1;
					transitionTimer = now;
					}
					break;
				}
				case 1: {
					displayText = "Practice fighting shadowpox \n by pushing them off your body.";
						
					if (now - transitionTimer > 5) {
						vaccineDialog = 2;
						transitionTimer = now;
					}
					break;
				}
				case 2: {
					displayText = "If you had caught the wild virus, \n you would be infecting others \n while you fight it. \n";
				
					if (now - transitionTimer > 5) {
						vaccineDialog = 3;
						transitionTimer = now;
					}
					break;
				}

				case 3: { 
					displayText="Your Protection Score is the number \n of people you have NOT infected \n because you are immunized. \n ";

					if (now - transitionTimer > 5) {
						vaccineDialog = 4;
						transitionTimer = now;
					}
					break;
				}
				case 4: {
					displayText = (isVaccine) ? "Protection Score: " : "Infection Score: ";
					currentState = sequenceMode::GAMEPLAY;
					gameTimer = now;
					break;
				}
				}
			}

			else {
				switch (vaccineDialog) {
				case 0: {
					displayText = "You have caught shadowpox. \n You can fight the virus.";

					if (now - transitionTimer > 5) { vaccineDialog = 1; 
					transitionTimer = now;
					}
					break;
				}
				case 1: {
					displayText = "Fight the virus by pushing \n the shadowpox off your body.";
					if (now - transitionTimer > 7) { vaccineDialog = 2;
					transitionTimer = now;
					}
					break;
				}
				case 2: {
					displayText = "As you fight the virus,\n you can infect others. \n";

					if (now - transitionTimer > 5) { vaccineDialog = 3;
					transitionTimer = now;
					}
					break;
				}

				case 3: {
					displayText = "Your Infection Score is the \n number of people to whom \n you have passed the virus.";

					if (now - transitionTimer > 5) {
						vaccineDialog = 4;
						transitionTimer = now;
					}
					break;
				}
				case 4: {
					displayText = (isVaccine) ? "Protection Score: " : "Infection Score: ";
					currentState = sequenceMode::GAMEPLAY;
					gameTimer = now;
					break;
				}
				}
			}

		}
		// DISPLAY
		break;
	}
	case sequenceMode::GAMEPLAY: {
		ofHideCursor();
		if (now - gameTimer < 30 * gui->gamePlayLength)
		{
			// timer 
			for (int k = 0; k < figures.size(); k++) {
				figures[k].update();
				figures[k].changeState(miniFig::HAPPY);
			}
			drawHuman(FIGURE_COLOR);

			for (int i = 0; i < patients.size(); i++) {
				// if tracking update!
				if (patients[i].tracking) {
					patients[i].isToroPox = gui->toropoxMode;//
					patients[i].minRadius = gui->minSize;//
					patients[i].maxRadius = gui->maxSize;//
					patients[i].growth = gui->growthRate;//
					patients[i].rotateSpeed = gui->rotationSpeed;//
					patients[i].phaseSpeed = gui->phaseSpeed;//
					patients[i].speed = gui->migrationSpeed;//
					patients[i].maxCells = gui->maxCount;
					patients[i].minWaxWane = gui->minWaxWane;
					patients[i].maxWaxWane = gui->maxWaxWane;
					patients[i].distanceX = gui->distanceX;
					patients[i].distanceY = gui->distanceY;
					patients[i].detectionRange = gui->detectionRange;
					patients[i].update();

					if ((ofGetElapsedTimeMillis() - spawnTimer) >= (1000.0f / (float)gui->buddingSpeed) && patients[i].cluster.size() < gui->maxCount) {
						patients[i].addCell(p1, p2);

						if (patients[i].cluster.size()) {
							ofLogNotice("shadowpox") << ofToString(patients[i].cluster.size());

							for (int j = 0; j < patients[i].cluster.size(); j++) {

								//if (patients[i].cluster[j].isDying) {
								for (int k = 0; k < figures.size(); k++) {
									miniFig &fig = figures[k];
								   fig.getInfected(patients[i].cluster[j].pos, infectionScore,isVaccine);
								}
								//}
								//else continue;
							}
						}
					}
				
}

				// if not clear the pox of that patient, only if it hasn't been cleared already
				else {
					if (patients[i].cluster.size()) {
						patients[i].clear();
					}
				}
			}

		}
		else { currentState = sequenceMode::END; 
		endDialog = 0;
		transitionTimer = now;
		
		}
		break;
	}
	case sequenceMode::END: {

		switch (endDialog) {
		case 0: {
			// clear remove all the pox
			for (cellCluster pox : patients) {
				if (pox.cluster.size()) {
					pox.cluster.clear();
				}
			}
			// show the score!
			SaveData();
			endDialog = 1;
		}
		case 1:{
			displayText = (isVaccine) ?
				"Your Protection Score is " + ofToString(infectionScore,0) + ".\n" + " \n" + "This is how many people you\n would have infected\n if you were not immunized. \n \n Of these, " + ofToString(miniFig::getDeathScore())+ " would have died." :
				"Your Infection Score is " + ofToString(infectionScore,0) + ".\n" + " \n" + "This is the number of \n people to whom \n you passed the virus. \n \n Of these, " + ofToString(miniFig::getDeathScore()) + " died." ;
			if (now - transitionTimer > 7) { endDialog = 2;
			transitionTimer = now;
			}
			break;
		}
		case 2: {
			if (!scoreCalculated) {
				modInfectionScore = infectionScore % 99;
				int div = infectionScore / 99;
				div = ofClamp(div, 1, 10);
			if (infectionScore < 100)modInfectionScore = infectionScore;
			else if (modInfectionScore < 25)modInfectionScore += (rand() % (10 + div));
			else if (modInfectionScore > 85)modInfectionScore -= (rand() % (20 - div));
				scoreCalculated = true;
			}
			displayText = (isVaccine) ?
				"To meet the people\n in your\n Protection Collection,\n visit : \n \n www.shadowpox.org/p/" + ofToString(modInfectionScore,3,3,'0')+ ".html" :
				"To meet the people\n in your\n Infection Collection,\n visit : \n \n www.shadowpox.org/p/" + ofToString(modInfectionScore,2, 2,'0')+ ".html";

				if (now - transitionTimer > 12) {
				endDialog = 3;
				transitionTimer = now;
			}
			break;
		
		}
		case 3: {
			
			selectedCountry = nullptr;
			selectedRegion = nullptr;
			isVaccine = false;
			infectionScore = 0;
			modInfectionScore = 0;
			scoreCalculated = false;
			// after a certain amount of time
			currentState = sequenceMode::SCREENSAVER;
			miniFig::resetDeathScore(); 

			textAlignFlag = 0;
			textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
			textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_TOP;
			break; 
		}
		}

		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {


	/*if (numOfBodiesTracked > 1) {
		// too many people tracked!

		// center the text
		textAlignFlag = 0;
		textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
		textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;


		glPushMatrix();
		glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2, 0);
		glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f); // mirror the text

		text.draw(warningText, 0, 0, textAlignFlag);

		glPopMatrix();
	}
	else {  */

		switch (currentState) {
		case sequenceMode::SCREENSAVER: {
			screenSaverPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
			break;
		}
		case sequenceMode::INTRO: {
			introTextPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
			break;
		}
		case sequenceMode::COUNTRYCHOICE:
		{

			ofEnableAlphaBlending();
			ofSetColor(255, 255, 255, 255);

			if (chooseRegion) {
				worldMap.draw(0, 0, ofGetWidth(), ofGetHeight());
				ofSetColor(255, 255, 255, 255);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // set blending mode 

				this->skeletonFBO.draw(0, 0, ofGetWidth(), ofGetHeight());

				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);

				glPushMatrix();
				glTranslatef(ofGetWidth() / 2, 50, 0);
				glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);
				text.draw(displayText, 0, 0, textAlignFlag);
				glPopMatrix();

				ofPushStyle();
				ofSetColor(225);
				if (regSel != -1) {
					displayRegion->draw(mouseX, mouseY,
						displayRegion->getWidth(),
						displayRegion->getHeight());

				}
				ofPopStyle();
			}
			else if (selectedRegion != nullptr && countrySelect) {

				if (regSel != -1) {

					for (int i = 0; i < selectedRegion->size(); i++) {
						ofPushStyle();
						if (i == selectedCountryInRegion) {
							ofSetColor(255);
							selectedRegion->at(i).bounds.setSize(flagOrgSize.x*1.1, flagOrgSize.y*1.1);
						}
						else {
							ofSetColor(150);
							selectedRegion->at(i).bounds.setSize(flagOrgSize.x, flagOrgSize.y);
						}
						selectedRegion->at(i).flagImage->draw(selectedRegion->at(i).bounds);
						ofPopStyle();
					}
				}


				ofSetColor(255, 255, 255, 255);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // set blending mode 

				this->skeletonFBO.draw(0, 0, ofGetWidth(), ofGetHeight());


				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);


				glPushMatrix();
				glTranslatef(ofGetWidth() / 2, 50, 0);
				glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);
				text.draw(displayText, 0, 0, textAlignFlag);
				glPopMatrix();


			}


			break;
		}
		case sequenceMode::VACCINECHOICE: {

				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);

				for (int k = 0; k < figures.size(); k++) {
					figures[k].draw();
				}

				ofSetColor(255, 255, 255, 255);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // set blending mode 
				this->skeletonFBO.draw(0, 0, ofGetWidth(), ofGetHeight());

				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);

				if (choiceSeqVaccine == 0) {

					glPushMatrix();
					glTranslatef(textAlignmentX, 50, 0);
					glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);
					text.draw(displayText, 0, 0, textAlignFlag);
					glPopMatrix();


					vaccineButton.image.draw(vaccineButton.bounds);
					virusButton.image.draw(virusButton.bounds);



				}
				else if (choiceSeqVaccine == 2) {


					glPushMatrix();
					glTranslatef(textAlignmentX, 50, 0);
					glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);
					text.draw(displayText, 0, 0, textAlignFlag);
					glPopMatrix();


				}
				else if (choiceSeqVaccine == 3) {


					glPushMatrix();
					glTranslatef(textAlignmentX, 50, 0);
					glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);
					text.draw(displayText, 0, 0, textAlignFlag);
					glPopMatrix();

				}
			
			break;
		}
		case sequenceMode::GAMEPLAY: {
			// reset blending mode
				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);

				for (int k = 0; k < figures.size(); k++) {
					figures[k].draw();
				}

				ofSetColor(255, 255, 255, 255);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // set blending mode 
				this->skeletonFBO.draw(0, 0, ofGetWidth(), ofGetHeight());

				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);
				for (cellCluster cluster : patients) {
					if (cluster.tracking) {
						cluster.draw();
					}
				}

				glPushMatrix();
				glTranslatef(ofGetWidth() / 2, 50, 0);
				glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);

				text.draw(displayText + ofToString(infectionScore), 0, 0, textAlignFlag);

				glPopMatrix();
			

			break;
		}
		case sequenceMode::END: {

			textAlignFlag = 0;
			textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
			textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;


			glPushMatrix();
			glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2, 0);
			glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);

			text.draw(displayText, 0, 0, textAlignFlag);

			glPopMatrix();
			break;
		}
		}
	//}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {

	case 't':
	case 'T':
		gui->toropoxMode = !gui->toropoxMode;
		break;

	case GLFW_KEY_0:
		selected = 0;
		break;
	case GLFW_KEY_1:
		selected = 1;
		break;
	case GLFW_KEY_2:
		selected = 2;
		break;
	case GLFW_KEY_3:
		selected = 3;
		break;
	case GLFW_KEY_4:
		selected = 4;
		break;
	case GLFW_KEY_5:
		selected = 5;
		break;

	case 'b':
	case 'B':
		gui->spawn = !gui->spawn;
		break;
	case 'k':
	case 'K':
		gui->skeleton = !gui->skeleton;
		break;

	case 's':
	case'S':
		logData.save(fileName);
		break;

	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


void ofApp::figureSetup(int amount, int percent) {

	if (amount < 0) return;

	figures.clear(); // clear the list first!
	for (int i = 0; i < amount; i++) {
		miniFig fig;
		fig.setup((i < percent) ? leftLocations[i] : rightLocations[i - percent], i);
		figures.push_back(fig);
	}
}

void ofApp::exit() {
	SaveData();
	logData.save(fileName);
	screenSaverPlayer.stop();
	screenSaverPlayer.close();
	introTextPlayer.stop();
	introTextPlayer.close();
}

void ofApp::SaveData() {
	ofxCsvRow data;
	data.addString(ofGetTimestampString("%Y-%m-%d-%H-%M-%S-%i"));
	data.addString(sourceData[(selectedCountry->index) + 1].getString(sourceCol::COUNTRY));
	data.addString((isVaccine) ? "Yes" : "No");
	data.addInt(infectionScore);
	data.addInt(miniFig::getDeathScore());
	logData.addRow(data);
}

void ofApp::pointsOnCirleFromLine(ofPoint p1, ofPoint p2, float r, ofPoint& p3, ofPoint& p4, ofPoint& p5, ofPoint& p6) {

	ofVec2f v = p2 - p1;
	v.normalize().perpendicular().scale(r);

	p3 = p1 - v;
	p4 = p1 + v;
	p5 = p2 + v;
	p6 = p2 - v;
}

void ofApp::drawHuman(int fig_color) {

	//if (kinect.isFrameNew()) {
		auto bodies = kinect.getBodySource()->getBodies();
		const auto & bonesDictionary = ofxKinectForWindows2::Data::Body::getBonesAtlas();

		// pox locations FBO
		this->skeletonFBO.begin();
		ofClear(0, 0, 0, 0);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		projector.beginAsCamera(gui->skeletonFlipX, gui->skeletonFlipY);

		numOfBodiesTracked = 0; 
		for (auto body : bodies) {
			patients[body.bodyId].tracking = false;

			if (body.tracked) {
				numOfBodiesTracked++;
				selected = body.bodyId;

				if (!gui->sweeping) {
					if ((body.leftHandState == HandState_Open || body.leftHandState == HandState_Closed)) {

						if (patients[body.bodyId].handStateTestingL == body.leftHandState) {
							++patients[body.bodyId].handStateCounterL;
						}
						else {
							patients[body.bodyId].handStateCounterL = 0;
							patients[body.bodyId].handStateTestingL = body.leftHandState;
						}
					}

					if (body.rightHandState == HandState_Open || body.rightHandState == HandState_Closed) {
						if (patients[body.bodyId].handStateTestingR == body.rightHandState) {
							++patients[body.bodyId].handStateCounterR;
						}
						else {
							patients[body.bodyId].handStateCounterR = 0;
							patients[body.bodyId].handStateTestingR = body.rightHandState;
						}
					}

					if (patients[body.bodyId].handStateCounterR >= gui->handSensitivity) {
						patients[body.bodyId].handStateR = patients[body.bodyId].handStateTestingR;
					}

					if (patients[body.bodyId].handStateCounterL >= gui->handSensitivity) {
						patients[body.bodyId].handStateL = patients[body.bodyId].handStateTestingL;
					}
				}
				else {
						patients[body.bodyId].handStateR = HandState_Closed;
						patients[body.bodyId].handStateL = HandState_Closed;
					
				}
				patients[body.bodyId].tracking = true;

				patients[body.bodyId].prevhandL = patients[body.bodyId].handL;
				patients[body.bodyId].prevhandR = patients[body.bodyId].handR;

				patients[body.bodyId].handL = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_HandLeft].getPosition());
				patients[body.bodyId].handR = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_HandRight].getPosition());

				if (gui->skeleton) {
					ofPushStyle();
					ofSetLineWidth(0);
					(patients[body.bodyId].handStateL == HandState_Closed) ? ofSetColor(255, 0, 0, 125) : ofSetColor(0, 255, 0, 125);
					ofDrawCircle(body.joints[JointType_HandLeft].getPosition(), gui->detectionRange*0.001);

					(patients[body.bodyId].handStateR == HandState_Closed) ? ofSetColor(255, 0, 0, 125) : ofSetColor(0, 255, 0, 125);
					ofDrawCircle(body.joints[JointType_HandRight].getPosition(), gui->detectionRange*0.001);
					ofPopStyle();
				}

				ofPushStyle();
				ofSetColor(155, 255);

				//ofVec3f diff = body.joints[JointType_ShoulderLeft].getPosition() - body.joints[JointType_ShoulderRight].getPosition();
				//float radiusJoint = diff.length()*0.165;

				for (auto bone : bonesDictionary) {
					p1 = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_Neck].getPosition());
					p2 = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_SpineMid].getPosition());


					patients[body.bodyId].bounds1 = p1;
					patients[body.bodyId].bounds2 = p2;
					if (gui->skeleton) {
						ofDrawLine(body.joints[bone.first].getPosition(), body.joints[bone.second].getPosition());
					}
					jointsInScreenPos[bone.first] = projector.getScreenCoordinateOfWorldPosition(body.joints[bone.first].getPosition());
					jointsInScreenPos[bone.second] = projector.getScreenCoordinateOfWorldPosition(body.joints[bone.second].getPosition());

				}


				ofPopStyle();
				ofPushStyle();

				ofVec3f diff = body.joints[JointType_ShoulderLeft].getPosition() - body.joints[JointType_ShoulderRight].getPosition();
				float radiusJoint = diff.length()*0.165;

				ofSetColor(fig_color);
				ofDrawCircle(body.joints[JointType_Head].getPosition(), 110 * 0.001);

				polyBody.clear();
				polyBody.moveTo(body.joints[JointType_ShoulderLeft].getPosition().x, body.joints[JointType_ShoulderLeft].getPosition().y + radiusJoint, body.joints[JointType_ShoulderLeft].getPosition().z);//
				polyBody.lineTo(body.joints[JointType_ShoulderRight].getPosition().x, body.joints[JointType_ShoulderRight].getPosition().y + radiusJoint, body.joints[JointType_ShoulderRight].getPosition().z);//
				polyBody.lineTo(body.joints[JointType_ShoulderRight].getPosition().x, body.joints[JointType_HipRight].getPosition().y, body.joints[JointType_HipRight].getPosition().z);
				polyBody.lineTo(body.joints[JointType_ShoulderLeft].getPosition().x, body.joints[JointType_HipLeft].getPosition().y, body.joints[JointType_HipLeft].getPosition().z);

				polyBody.close();
				polyBody.setFilled(true);
				polyBody.setFillColor(fig_color);
				polyBody.draw();


				ofDrawCircle(body.joints[JointType_ShoulderLeft].getPosition(), radiusJoint);
				ofDrawCircle(body.joints[JointType_ShoulderRight].getPosition(), radiusJoint);


				ofPath limb01;
				ofPoint p1, p2, p3, p4;


				pointsOnCirleFromLine(body.joints[JointType_ShoulderLeft].getPosition(), body.joints[JointType_ElbowLeft].getPosition(), radiusJoint, p1, p2, p3, p4);

				limb01.moveTo(p1);
				limb01.lineTo(p2);
				limb01.lineTo(p3);
				limb01.lineTo(p4);
				limb01.close();

				limb01.setFilled(true);
				limb01.setFillColor(fig_color);
				limb01.draw();
				ofSetColor(fig_color, 255);

				ofPath limb02;
				ofPoint p5, p6, p7, p8;


				pointsOnCirleFromLine(body.joints[JointType_ShoulderRight].getPosition(), body.joints[JointType_ElbowRight].getPosition(), radiusJoint, p5, p6, p7, p8);

				limb02.moveTo(p5);
				limb02.lineTo(p6);
				limb02.lineTo(p7);
				limb02.lineTo(p8);
				limb02.close();

				limb02.setFilled(true);
				limb02.setFillColor(fig_color);
				limb02.draw();
				ofSetColor(fig_color, 255);


				ofDrawCircle(body.joints[JointType_ElbowLeft].getPosition(), radiusJoint);
				ofDrawCircle(body.joints[JointType_ElbowRight].getPosition(), radiusJoint);

				ofPath limb03;
				ofPoint p9, p10, p11, p12;


				pointsOnCirleFromLine(body.joints[JointType_ElbowLeft].getPosition(), body.joints[JointType_HandLeft].getPosition(), radiusJoint, p9, p10, p11, p12);

				limb03.moveTo(p9);
				limb03.lineTo(p10);
				limb03.lineTo(p11);
				limb03.lineTo(p12);
				limb03.close();

				limb03.setFilled(true);
				limb03.setFillColor(fig_color);
				limb03.draw();
				ofSetColor(fig_color, 255);

				ofPath limb04;
				ofPoint p13, p14, p15, p16;


				pointsOnCirleFromLine(body.joints[JointType_ElbowRight].getPosition(), body.joints[JointType_HandRight].getPosition(), radiusJoint, p13, p14, p15, p16);

				limb04.moveTo(p13);
				limb04.lineTo(p14);
				limb04.lineTo(p15);
				limb04.lineTo(p16);
				limb04.close();

				limb04.setFilled(true);
				limb04.setFillColor(fig_color);
				limb04.draw();
				ofSetColor(fig_color, 255);

				ofDrawCircle(body.joints[JointType_HandLeft].getPosition(), radiusJoint);
				ofDrawCircle(body.joints[JointType_HandRight].getPosition(), radiusJoint);


				ofPoint hipLeft = ofPoint(body.joints[JointType_ShoulderLeft].getPosition().x + radiusJoint, body.joints[JointType_HipLeft].getPosition().y, body.joints[JointType_HipLeft].getPosition().z);
				ofPoint hipRight = ofPoint(body.joints[JointType_ShoulderRight].getPosition().x - radiusJoint, body.joints[JointType_HipRight].getPosition().y, body.joints[JointType_HipRight].getPosition().z);

				ofDrawCircle(hipLeft, radiusJoint);
				ofDrawCircle(hipRight, radiusJoint);

				ofPath limb05;
				ofPoint p17, p18, p19, p20;

				pointsOnCirleFromLine(hipLeft, body.joints[JointType_KneeLeft].getPosition(), radiusJoint, p17, p18, p19, p20);

				limb05.moveTo(p17);
				limb05.lineTo(p18);
				limb05.lineTo(p19);
				limb05.lineTo(p20);
				limb05.close();

				limb05.setFilled(true);
				limb05.setFillColor(fig_color);
				limb05.draw();
				ofSetColor(fig_color, 255);

				ofPath limb06;
				ofPoint p21, p22, p23, p24;

				pointsOnCirleFromLine(hipRight, body.joints[JointType_KneeRight].getPosition(), radiusJoint, p21, p22, p23, p24);

				limb06.moveTo(p21);
				limb06.lineTo(p22);
				limb06.lineTo(p23);
				limb06.lineTo(p24);
				limb06.close();

				limb06.setFilled(true);
				limb06.setFillColor(fig_color);
				limb06.draw();
				ofSetColor(fig_color, 255);


				ofDrawCircle(body.joints[JointType_KneeLeft].getPosition(), radiusJoint);
				ofDrawCircle(body.joints[JointType_KneeRight].getPosition(), radiusJoint);

				ofPath limb07;
				ofPoint p25, p26, p27, p28;

				pointsOnCirleFromLine(body.joints[JointType_KneeRight].getPosition(), body.joints[JointType_AnkleRight].getPosition(), radiusJoint, p25, p26, p27, p28);

				limb07.moveTo(p25);
				limb07.lineTo(p26);
				limb07.lineTo(p27);
				limb07.lineTo(p28);
				limb07.close();
				limb07.setFilled(true);
				limb07.setFillColor(fig_color);
				limb07.draw();
				ofSetColor(fig_color, 255);

				ofPath limb08;
				ofPoint p29, p30, p31, p32;

				pointsOnCirleFromLine(body.joints[JointType_KneeLeft].getPosition(), body.joints[JointType_AnkleLeft].getPosition(), radiusJoint, p29, p30, p31, p32);

				limb08.moveTo(p29);
				limb08.lineTo(p30);
				limb08.lineTo(p31);
				limb08.lineTo(p32);
				limb08.close();

				limb08.setFilled(true);
				limb08.setFillColor(fig_color);
				limb08.draw();
				ofSetColor(fig_color, 255);
				ofDrawCircle(body.joints[JointType_AnkleLeft].getPosition(), radiusJoint);
				ofDrawCircle(body.joints[JointType_AnkleRight].getPosition(), radiusJoint);
				ofPopStyle();
			}
		}
		this->projector.endAsCamera();
		this->skeletonFBO.end();

		
	}

//}








