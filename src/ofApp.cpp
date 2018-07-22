#include "ofApp.h"

#define DEPTH_WIDTH			512
#define DEPTH_HEIGHT		424
#define	MIN_DEPTH_CLIP		50.0f
#define MAX_DEPTH_CLIP		10000.0f

#define BACKGROUND_COLOR 0
#define FIGURE_COLOR 255
#define FIGURE_COLOR_TRANSITION 200


//TODO: ADD SKIP BUTTON
//TODO: KINECT SELECTION


//--------------------------------------------------------------
void ofApp::setup() {
	text.load("font/euro.ttf", 55, true, true, false, false, 72);
	text.setLetterSpacing(1.1f);

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

	dir.allowExt("png");
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
			r.setAnchorPercent(0.5,0.5);
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

	nFiles = dir.listDir("choices/cursor");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " hand icons";
	if (nFiles) {

			leftHand.cursor.load(dir.getPath(0));
			leftHand.cursor.setAnchorPercent(0.5, 0.5);

			rightHand.cursor.load(dir.getPath(0));
			rightHand.cursor.setAnchorPercent(0.5, 0.5);
	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/cursor");

	nFiles = dir.listDir("choices/vaccine");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " vaccine icons";
	if (nFiles) {

		vaccineButton.image.load(dir.getPath(0));
		vaccineButton.bounds.setFromCenter(0, 0, vaccineButton.image.getWidth(), vaccineButton.image.getHeight());
		vaccineButton.size = ofPoint(vaccineButton.image.getWidth(), vaccineButton.image.getHeight());
		virusButton.image.load(dir.getPath(1));
		virusButton.bounds.setFromCenter(0, 0, virusButton.image.getWidth(), virusButton.image.getHeight());
		virusButton.size = ofPoint(virusButton.image.getWidth(), virusButton.image.getHeight());

	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/vaccine");

	nFiles = dir.listDir("choices/buttons");
	dir.sort();
	ofLogNotice("shadowpox") << "loading " << nFiles << " button graphics";
	if (nFiles) {

		backButton.image.load(dir.getPath(0));
		backButton.bounds.setFromCenter(0, 0, backButton.image.getWidth(), backButton.image.getHeight());
		backButton.size = ofPoint(backButton.image.getWidth(), backButton.image.getHeight());

		backButtonAlt.image.load(dir.getPath(1));
		backButtonAlt.bounds.setFromCenter(0, 0, backButtonAlt.image.getWidth(), backButtonAlt.image.getHeight());
		backButtonAlt.size = ofPoint(backButtonAlt.image.getWidth(), backButtonAlt.image.getHeight());

		skipButton.image.load(dir.getPath(2));
		skipButton.bounds.setFromCenter(0, 0, skipButton.image.getWidth(), skipButton.image.getHeight());
		skipButton.size = ofPoint(skipButton.image.getWidth(), skipButton.image.getHeight());


		playAgainButton.image.load(dir.getPath(3));
		playAgainButton.bounds.setFromCenter(0, 0, playAgainButton.image.getWidth(), playAgainButton.image.getHeight());
		playAgainButton.size = ofPoint(playAgainButton.image.getWidth(), playAgainButton.image.getHeight());
	}
	else ofLogWarning("shadowpox", "Could not find folder:choices/buttons");

	flagOrgSize.x = flags[0].getWidth();
	flagOrgSize.y = flags[0].getHeight();


	screenSaverPlayer.load("clip/opening.mp4");
	screenSaverPlayer.isLoaded();
	screenSaverPlayer.setLoopState(ofLoopType::OF_LOOP_NORMAL);
	screenSaverPlayer.play();
	introTextPlayer.load("clip/intro_text.mp4");
	introTextPlayer.setLoopState(ofLoopType::OF_LOOP_NONE);

	demoPushPlayer.load("clip/demo_pox.mp4");
	demoPushPlayer.setLoopState(ofLoopType::OF_LOOP_NORMAL);


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


	int textOffsetHeight = 150;
	int heightWithPadding = flagOrgSize.y + 10;
	int widthWithPadding = flagOrgSize.x + 15;
	int offsetX = (1920 - (9 * widthWithPadding)) / 2;

	for (int columns = 0; columns < 4; columns++) {
		for (int rows = 0; rows < 8; rows++) {
			ofVec2f loc = ofVec2f((rows * widthWithPadding) + offsetX, (columns * heightWithPadding) + textOffsetHeight);
			flagLocations.push_back(loc);
		}
	}

	// create our map datastructure to hold all the pairings.
	for (int i = 0; i < 11; i++) {
		// region index and array of country infor
		countriesInRegion.insert(pair<int, vector<countryInfo>>(i, vector<countryInfo>()));
	
	}


	regionCenter[0] = ofPoint(937,711); // central africa
	regionCenter[1] = ofPoint(846,782);// south eastern africa
	regionCenter[2] = ofPoint(976,576);// nothern africa
	regionCenter[3] = ofPoint(622,520);// southern asia
	regionCenter[4] = ofPoint(347,747);// asia pacific
	regionCenter[5] = ofPoint(784,527);// middle east
	regionCenter[6] = ofPoint(459,457);// eastern asia
	regionCenter[7] = ofPoint(1344,772);// south america
	regionCenter[8] = ofPoint(802,364);// eastern europe
	regionCenter[9] = ofPoint(1486,426);// north america
	regionCenter[10] = ofPoint(988,396);// western europe


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

	fileName = "C:/Users/YORK/Google Drive/Shadowpox-game-data/" + ofGetTimestampString("%Y-%m-%d-%H-%M") + "shadowpox.csv";

	logData.createFile(fileName);
	ofLogNotice("shadowpox", "file created:"+ fileName);

	ofxCsvRow header;
	header.addString("Date");
	header.addString("Country Choice");
	header.addString("Vaccination Choice");
	header.addString("Score");
	header.addString("Deaths");

	// time // country // choice // score
	logData.addRow(header);


	currentState = sequenceMode::SCREENSAVER;
	displaySkipButton = true;

}

//--------------------------------------------------------------
void ofApp::update() {

	float now = ofGetElapsedTimef();

	//HACK to get it to fullscreen on second monitor. window must be positioned on second monitor first, then fullscreen is called.

	if (ofGetFrameNum() == 20) {
		ofSetWindowPosition(1920, 0);
	}
	if (ofGetFrameNum() == 25) {
		ofSetFullscreen(true);
	} 

	if (ofGetFrameNum() == 30) {
		backButton.bounds.setPosition(ofGetWidth() - backButton.size.x, ofGetHeight() - backButton.size.y);
		backButtonAlt.bounds.setPosition(ofGetWidth() - (backButtonAlt.size.x * 2), ofGetHeight() - backButtonAlt.size.y - 15);

		skipButton.bounds.setPosition(0, ofGetHeight() - skipButton.size.y);
		playAgainButton.bounds.setPosition(0, ofGetHeight() - playAgainButton.size.y);
	}
	this->kinect.update();

	//if (gui->openingSeq) currentState = sequenceMode::COUNTRYCHOICE;

	if (!gui->manualSelection && currentState != sequenceMode::GAMEPLAY) {

		if (kinect.isFrameNew()) {
			auto bodies = kinect.getBodySource()->getBodies();
			const auto & bonesDictionary = ofxKinectForWindows2::Data::Body::getBonesAtlas();

			projector.beginAsCamera(gui->skeletonFlipX, gui->skeletonFlipY);

			numOfBodiesTracked = 0;

			for (auto body : bodies) {
				if (body.tracked) {
					numOfBodiesTracked++;
					rightHand.pos = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_HandTipRight].getPosition());
					leftHand.pos = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_HandTipLeft].getPosition());
					break;
				}
			}
			projector.endAsCamera();

			if (numOfBodiesTracked == 0) {
				rightHand.pos = ofPoint(-5000, 0);
				leftHand.pos = ofPoint(-5000, 0);
			}
		}
	}

	if (!(numOfBodiesTracked > 1 && (currentState == sequenceMode::COUNTRYCHOICE || currentState == sequenceMode::VACCINECHOICE))) {

		switch (currentState) {
		case sequenceMode::SCREENSAVER: {
			ofHideCursor();
			if (!gui->skipScreenSaver) {
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
				currentState = (gui->skipIntroText) ? sequenceMode::COUNTRYCHOICE : sequenceMode::INTRO;
				if (screenSaverPlayer.isPlaying())screenSaverPlayer.stop();
			}
			break; }
		case sequenceMode::INTRO: {
			ofHideCursor();
			introTextPlayer.update();

			drawHuman(FIGURE_COLOR_TRANSITION, true);
			if (displaySkipButton) {
				if (gui->manualSelection) {
					if (skipButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
						currentState = sequenceMode::COUNTRYCHOICE;
						introTextPlayer.stop();// stop INTRO
						displaySkipButton = false;
						transitionTimer = now;
						return;
					}
				}
				else {

					if (skipButton.bounds.inside(rightHand.pos)) {
						skipButton.event = true;
						if (skipButton.timeStamp == 0)
							skipButton.timeStamp = now;
					}
					else {
						skipButton.event = false;
						skipButton.timeStamp = 0;
					}

					if (skipButton.event && now - skipButton.timeStamp > 2) {
						currentState = sequenceMode::COUNTRYCHOICE;
						introTextPlayer.stop();// stop INTRO
						displaySkipButton = false;
						transitionTimer = now;
						return;
					}

				}
			}
			if (introTextPlayer.getIsMovieDone() || gui->skipIntroText) {
				currentState = sequenceMode::COUNTRYCHOICE;
				introTextPlayer.stop();// stop INTRO
				return;
			}
			break;
		}
		case sequenceMode::COUNTRYCHOICE: {
			displaySkipButton = false;

			textAlignFlag = 0;
			textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
			textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_TOP;

			drawHuman(FIGURE_COLOR_TRANSITION, true);
			if (!gui->manualSelection) {
				if (selectedCountry == nullptr && selectedRegion == nullptr) {
					chooseRegion = true;
					countrySelect = false;
				}


				//pick a region
				if (chooseRegion || selectedRegion == nullptr) {


					displayBackButton = false;
					displayText = "Please choose your region";

					pastRegSel = regSel;

					for (int i = 0; i < boundaries.size(); i++) {
						if (boundaries[i].inside(rightHand.pos)) {


							regSel = i;

							if (rightHand.HandStateCounter == 0)
								rightHand.HandStateCounter = now;
							break;
						}
						else {
							regSel = -1;
						}
					}

					if (regSel == -1 || pastRegSel != regSel) {
						rightHand.HandStateCounter = 0;
					}

					if (regSel != -1) {
						displayRegion = &regions[regSel];

						// check if its been confirmed
						if (now - rightHand.HandStateCounter > handPointerStateCounterThres && rightHand.HandStateCounter != 0) {
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

					// DISPLAY BACK BUTTON


					displayBackButton = true;

					if (backButtonAlt.bounds.inside(rightHand.pos)) {

						if (backButtonAlt.timeStamp == 0) {
							backButtonAlt.timeStamp = now;
						}
						backButtonAlt.event = true;
					}
					else {
						backButtonAlt.timeStamp = 0;
						backButtonAlt.event = false;
					}

					if (backButtonAlt.event && now - backButtonAlt.timeStamp > handPointerStateCounterThres) {

						selectedRegion = nullptr;
						chooseRegion = true;
						countrySelect = false;
						backButtonAlt.event = false;
						goto regionSelectionManual;

					}

					pastRegSel = selectedCountryInRegion;
					for (unsigned n = 0; n < selectedRegion->size(); n++) {

						if (selectedRegion->at(n).bounds.inside(rightHand.pos)) {

							selectedCountryInRegion = n;


							if (rightHand.HandStateCounter == 0)
								rightHand.HandStateCounter = now;
							break;
						}

						else {
							selectedCountryInRegion = -1;
						}
					}

					if (selectedCountryInRegion == -1 || pastRegSel != selectedCountryInRegion) {
						rightHand.HandStateCounter = 0;
					}

					if (selectedCountryInRegion != -1) {
						displayRegion = &regions[regSel];

						// check if its been confirmed
						if (now - rightHand.HandStateCounter > handPointerStateCounterThres && rightHand.HandStateCounter != 0) {

							// check if it has been selected

							selectedCountry = &(selectedRegion->at(selectedCountryInRegion));
							float f = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);

							if (f + 1 > 80) f--;
							sourceData[(selectedCountry->index) + 1].setFloat(sourceCol::CURRENTVACCINATIONRATE, f);

							currentState = sequenceMode::VACCINECHOICE;
							transitionTimer = now;
							choiceSeqVaccine = 0;

							figureSetup(figureAmount, f);

							displayText = "Shadowpox vaccination rate in \n" + sourceData[(selectedCountry->index) + 1].getString(sourceCol::COUNTRY) + ": \n" + ofToString(f, 0) + "%";

							vaccineButton.bounds.setPosition((100 + ((f / 10) + 1) * 100) - vaccineButton.size.x / 2, ofGetHeight() / 2 - vaccineButton.size.y / 2);
							virusButton.bounds.setPosition(ofGetWidth() - vaccineButton.size.x / 2 - ((((100 - f) / 10) + 1) * 100), ofGetHeight() / 2 - vaccineButton.size.y / 2);

							textAlignmentX = (vaccineButton.bounds.getCenter().x + virusButton.bounds.getCenter().x) / 2;
							return;
						}

					}


				}
			}
			else {
				ofShowCursor();
				drawHuman(FIGURE_COLOR_TRANSITION, true);

				if (selectedCountry == nullptr && selectedRegion == nullptr) {
					chooseRegion = true;
					countrySelect = false;
				}
				// show regions
				if (chooseRegion || selectedRegion == nullptr) {

				regionSelectionManual:
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

						if (ofGetMousePressed() && now - transitionTimer > 2) {
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

					// DISPLAY BACK BUTTON

					displayBackButton = true;

					backButtonAlt.event = ofGetMousePressed();
					if (now - transitionTimer > 1) {
						if (backButtonAlt.bounds.inside(mouseX, mouseY) && backButtonAlt.event) {
							selectedRegion = nullptr;
							chooseRegion = true;
							countrySelect = false;
							backButtonAlt.event = false;
							goto regionSelectionManual;
						}
					}

				countrySelctionManual:


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
								displayText = "Shadowpox vaccination rate in \n" + sourceData[(selectedCountry->index) + 1].getString(sourceCol::COUNTRY) + ": \n" + ofToString(f, 0) + "%";

								vaccineButton.bounds.setPosition((100 + ((f / 10) + 1) * 100) - vaccineButton.size.x / 2, ofGetHeight() / 2 - vaccineButton.size.y / 2);
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

			if (gui->manualSelection)ofShowCursor();
			drawHuman(FIGURE_COLOR_TRANSITION, true);
			// CHOICE
			// show choice
			if (choiceSeqVaccine == 0) {
				float currentVaccine = sourceData[(selectedCountry->index) + 1].getFloat(sourceCol::CURRENTVACCINATIONRATE);

				for (int k = 0; k < figures.size(); k++) {
					figures[k].changeState(miniFig::START);
				}
				// DISPLAY BACK BUTTON

				displayBackButton = true;


				// mouse check back 

				if (gui->manualSelection) {
					backButton.event = ofGetMousePressed();

					if (backButton.bounds.inside(mouseX, mouseY) && backButton.event) {
						choiceSeqVaccine = 0;
						countrySelect = true;
						chooseRegion = false;
						currentState = sequenceMode::COUNTRYCHOICE;
						backButton.event = false;
						transitionTimer = now;
						goto countrySelctionManual;
					}


					// show text
						// mouse check vaccine choice 
					if (now - transitionTimer > 1) {
						if (vaccineButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
							isVaccine = true;
							choiceSeqVaccine = 1;
							transitionTimer = now;
							displayBackButton = false;
							return;
						}
						if (virusButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
							isVaccine = false;
							choiceSeqVaccine = 1;
							transitionTimer = now;
							displayBackButton = false;
							return;
						}
					}

				}

				else {

					if (backButton.bounds.inside(rightHand.pos)) {

						if (backButton.timeStamp == 0) {
							backButton.timeStamp = now;
						}
						backButton.event = true;
					}
					else {
						backButton.timeStamp = 0;
						backButton.event = false;
					}

					if (backButton.event && now - backButton.timeStamp > handPointerStateCounterThres) {
						choiceSeqVaccine = 0;
						countrySelect = true;
						chooseRegion = false;
						currentState = sequenceMode::COUNTRYCHOICE;
						backButton.event = false;
						transitionTimer = now;
						goto countrySelctionManual;
					}


					if (now - transitionTimer > 3) {
						if (vaccineButton.bounds.inside(rightHand.pos)) {

							if (vaccineButton.timeStamp == 0) {
								vaccineButton.timeStamp = now;
							}
							vaccineButton.event = true;
						}
						else {
							vaccineButton.timeStamp = 0;
							vaccineButton.event = false;
						}

						if (vaccineButton.event && now - vaccineButton.timeStamp > handPointerStateCounterThres) {
							isVaccine = true;
							choiceSeqVaccine = 1;
							transitionTimer = now;
							displayBackButton = false;
							return;
						}

						if (virusButton.bounds.inside(rightHand.pos)) {

							if (virusButton.timeStamp == 0) {
								virusButton.timeStamp = now;
							}
							virusButton.event = true;
						}
						else {
							virusButton.timeStamp = 0;
							virusButton.event = false;
						}

						if (virusButton.event && now - virusButton.timeStamp > handPointerStateCounterThres) {
							isVaccine = false;
							choiceSeqVaccine = 1;
							transitionTimer = now;
							displayBackButton = false;
							return;
						}
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
				displayText = "Shadowpox vaccination rate \n in " + sourceData[(selectedCountry->index) + 1].getString(sourceCol::COUNTRY) + ": \n"
					+ "Was: " + ofToString(((isVaccine) ? currentVaccine - 1 : currentVaccine + 1), 0) + "% \n"
					+ "Now: " + ofToString(currentVaccine, 0) + "% \n";
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
					displaySkipButton = true;
					return;
				}
			}

			else if (choiceSeqVaccine == 3) {

				if (displaySkipButton) {

					if (gui->manualSelection) {
						if (skipButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
							displayText = (isVaccine) ? "Protection Score: " : "Infection Score: ";
							currentState = sequenceMode::GAMEPLAY;
							gameTimer = now;
							displaySkipButton = false;
							displayDemoVid = false;
							return;
						}
					}
					else {

						if (skipButton.bounds.inside(rightHand.pos)) {
							skipButton.event = true;
							if (skipButton.timeStamp == 0)
								skipButton.timeStamp = now;
						}
						else {
							skipButton.event = false;
							skipButton.timeStamp = 0;
						}

						if (skipButton.event && now - skipButton.timeStamp > 1) {
							displayText = (isVaccine) ? "Protection Score: " : "Infection Score: ";
							currentState = sequenceMode::GAMEPLAY;
							gameTimer = now;
							displaySkipButton = false;
								displayDemoVid = false;
							return;
						}

					}
				}
				if (isVaccine) {
					switch (vaccineDialog) {
					case 0: {
						displayText = "The shadowpox vaccine is\n a weakened form of the virus.\n It is not infectious. \n \n The vaccine lets you safely practice\n fighting the disease.";
						if (now - transitionTimer > 8) {
							vaccineDialog = 1;
							transitionTimer = now;
							displayDemoVid = true;
							demoPushPlayer.play();
						}
						break;
					}
					case 1: {
						displayText = "Practice fighting shadowpox \n by moving your hands to \n push the pox off your body.";


						if (!demoPushPlayer.isPlaying()) demoPushPlayer.play();

						demoPushPlayer.update();
						if (now - transitionTimer > 15) {
							vaccineDialog = 2;
							transitionTimer = now;
							demoPushPlayer.stop();
							displayDemoVid = false;
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
						displayText = "Your Protection Score is the number \n of people you have NOT infected \n because you are immunized. \n ";

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

						if (now - transitionTimer > 4) {
							vaccineDialog = 1;
							transitionTimer = now;
							displayDemoVid = true;
							demoPushPlayer.play();
						}
						break;
					}
					case 1: {
						displayText = "Fight the virus \n by moving your hands to \n push the pox off your body.";
						if (!demoPushPlayer.isPlaying()) demoPushPlayer.play();
						demoPushPlayer.update();
						if (now - transitionTimer > 15) {
							vaccineDialog = 2;
							transitionTimer = now;
							demoPushPlayer.stop();
							displayDemoVid = false;
						}
						break;
					}
					case 2: {
						displayText = "As you fight the virus,\n you can infect others. \n";

						if (now - transitionTimer > 4) {
							vaccineDialog = 3;
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

			displaySkipButton = false;
			ofHideCursor();
			if (now - gameTimer < 15 * gui->gamePlayLength)
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
						patients[i].isToroPox = isVaccine;
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

						if ((ofGetElapsedTimef() - spawnTimer) >= (1000.0f / (float)gui->buddingSpeed)*0.001f && patients[i].cluster.size() < gui->maxCount) {

							patients[i].addCellCluster(1, p1, p2);
							spawnTimer = ofGetElapsedTimef();
							//if (patients[i].cluster.size()) {
								//ofLogNotice("shadowpox") << ofToString(patients[i].cluster.size());

							for (int j = 0; j < patients[i].cluster.size(); j++) {

								if (patients[i].cluster[j].isDying) {
									for (int k = 0; k < figures.size(); k++) {
										miniFig &fig = figures[k];
										fig.getInfected(patients[i].cluster[j].pos, infectionScore, isVaccine);
									}
								}

								else { continue; }
							}
						}
						//}
					}

					// if not clear the pox of that patient, only if it hasn't been cleared already
					else {
						if (patients[i].cluster.size()) {
							patients[i].cluster.clear();
						}
					}
				}
			}
			else {
				currentState = sequenceMode::END;
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
			case 1: {
				displayText = (isVaccine) ?
					"Your Protection Score is " + ofToString(infectionScore, 0) + ".\n" + " \n" + "This is how many people you\n would have infected\n if you were not immunized. \n \n Of these, " + ofToString(miniFig::getDeathScore()) + " would have died." :
					"Your Infection Score is " + ofToString(infectionScore, 0) + ".\n" + " \n" + "This is the number of \n people to whom \n you passed the virus. \n \n Of these, " + ofToString(miniFig::getDeathScore()) + " died.";
				if (now - transitionTimer > 7) {
					endDialog = 2;
					transitionTimer = now;
				}
				break;
			}
			case 2: {
				/*if (!scoreCalculated) {
					modInfectionScore = infectionScore % 99;
					int div = infectionScore / 99;
					div = ofClamp(div, 1, 10);
				if (infectionScore < 100)modInfectionScore = infectionScore;
				else if (modInfectionScore < 25)modInfectionScore += (rand() % (10 + div));
				else if (modInfectionScore > 85)modInfectionScore -= (rand() % (20 - div));
					scoreCalculated = true;
				}*/

				poxencode(isVaccine, infectionScore, miniFig::getDeathScore(), code);

				displayText = "Your web code is:\n \n";

				displayText += code;

				displayText += "\n \n Write your code in the blank on the card, and visit \n \n";
				displayText += "shadowpox.org /";
				displayText += code;
				displayText += "\n \n to meet the people in your \n";
				displayText += (isVaccine) ? "Protection " : "Infection ";
				displayText += "Collection.";

				// play again button

				if (gui->manualSelection) {
					if (playAgainButton.bounds.inside(mouseX, mouseY) && ofGetMousePressed()) {
						currentState = sequenceMode::COUNTRYCHOICE;
						selectedCountry = nullptr;
						selectedRegion = nullptr;
						isVaccine = false;
						infectionScore = 0;
						modInfectionScore = 0;
						scoreCalculated = false;
						displaySkipButton = true;
						miniFig::resetDeathScore();

						textAlignFlag = 0;
						textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
						textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_TOP;
						displaySkipButton = false;
						transitionTimer = now;
						return;
					}
				}
				else {

					if (playAgainButton.bounds.inside(rightHand.pos)) {
						playAgainButton.event = true;
						if (playAgainButton.timeStamp == 0)
							playAgainButton.timeStamp = now;
					}
					else {
						playAgainButton.event = false;
						playAgainButton.timeStamp = 0;
					}

					if (playAgainButton.event && now - playAgainButton.timeStamp > 2) {
						currentState = sequenceMode::COUNTRYCHOICE;
						selectedCountry = nullptr;
						selectedRegion = nullptr;
						isVaccine = false;
						infectionScore = 0;
						modInfectionScore = 0;
						scoreCalculated = false;
						displaySkipButton = true;
						miniFig::resetDeathScore();

						textAlignFlag = 0;
						textAlignFlag |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
						textAlignFlag |= ofxTextAlign::VERTICAL_ALIGN_TOP;
						displaySkipButton = false;
						transitionTimer = now;
						return;
					}

				}


				if (now - transitionTimer > 60) {
					endDialog = 3;
					transitionTimer = now;
				}

				if (numOfBodiesTracked == 0) {
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
				displaySkipButton = true;
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
}

//--------------------------------------------------------------
void ofApp::draw() {
	
		switch (currentState) {
		case sequenceMode::SCREENSAVER: {
			screenSaverPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
			break;
		}
		case sequenceMode::INTRO: {
			introTextPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());

			rightHand.cursor.draw(rightHand.pos);

				if (displaySkipButton) {
					if (skipButton.event)skipButton.bounds.setSize(skipButton.size.x*1.05, skipButton.size.y*1.05);
					else {
						skipButton.bounds.setSize(skipButton.size.x, skipButton.size.y);
					}
					skipButton.image.draw(skipButton.bounds);
				}
		
			break;
		}
		case sequenceMode::COUNTRYCHOICE:
		{
			if (numOfBodiesTracked > 1) {

				glPushMatrix();
				glTranslatef(ofGetScreenWidth() / 2, ofGetScreenHeight() / 2, 0);
				glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);

				text.draw(warningText, 0, 0, textAlignFlag);
				glPopMatrix();
			}
			else {
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

					if (!gui->manualSelection) {
						displayRegion->draw(
							regionCenter[regSel],
							displayRegion->getWidth(),
							displayRegion->getHeight());


						rightHand.cursor.draw(rightHand.pos);

					}
					else {
						displayRegion->draw(
							regionCenter[regSel],
							displayRegion->getWidth(),
							displayRegion->getHeight());
					}


				}

				ofPopStyle();
			}
			else if (selectedRegion != nullptr && countrySelect) {

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

				if (displayBackButton) {

					if (backButtonAlt.event)backButtonAlt.bounds.setSize(backButtonAlt.size.x*1.05, backButtonAlt.size.y*1.05);
					else {
						backButtonAlt.bounds.setSize(backButtonAlt.size.x, backButtonAlt.size.y);
					}
					backButtonAlt.image.draw(backButtonAlt.bounds);


				}
				if (regSel != -1) {

					for (int i = 0; i < selectedRegion->size(); i++) {
						ofPushStyle();
						if (i == selectedCountryInRegion) {
							ofSetColor(255);
							selectedRegion->at(i).bounds.setSize(flagOrgSize.x*1.05, flagOrgSize.y*1.05);
						}
						else {
							ofSetColor(125);
							selectedRegion->at(i).bounds.setSize(flagOrgSize.x, flagOrgSize.y);
						}
						selectedRegion->at(i).flagImage->draw(selectedRegion->at(i).bounds);
						ofPopStyle();
					}
				}


				if (!gui->manualSelection)
					rightHand.cursor.draw(rightHand.pos);
			}
		}
			break;
		}
		case sequenceMode::VACCINECHOICE: {
			if (numOfBodiesTracked > 1) {
				glPushMatrix();
				glTranslatef(ofGetScreenWidth() / 2, ofGetScreenHeight() / 2, 0);
				glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);

				text.draw(warningText, 0, 0, textAlignFlag);
				glPopMatrix();
			}
			else {

				if (displayDemoVid) {
					demoPushPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
				}
				else {
					ofEnableAlphaBlending();
					ofSetColor(255, 255, 255, 255);

					for (int k = 0; k < figures.size(); k++) {
						figures[k].draw();
					}

					ofSetColor(255, 255, 255, 255);
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // set blending mode
					this->skeletonFBO.draw(0, 0, ofGetWidth(), ofGetHeight());
				}
				ofEnableAlphaBlending();
				ofSetColor(255, 255, 255, 255);

				

				if (displayBackButton) {
					if (backButton.event)backButton.bounds.setSize(backButton.size.x*1.05, backButton.size.y*1.05);
					else {
						backButton.bounds.setSize(backButton.size.x, backButton.size.y);
					}
					backButton.image.draw(backButton.bounds);
				}
				if (displaySkipButton) {
					if (skipButton.event)skipButton.bounds.setSize(skipButton.size.x*1.05, skipButton.size.y*1.05);
					else {
						skipButton.bounds.setSize(skipButton.size.x, skipButton.size.y);
					}
					skipButton.image.draw(skipButton.bounds);
				}


				if (choiceSeqVaccine == 0) {

					glPushMatrix();
					glTranslatef(textAlignmentX, 50, 0);
					glScalef((gui->flipText) ? -1.0f : 1.0f, 1.0f, 1.0f);
					text.draw(displayText, 0, 0, textAlignFlag);
					glPopMatrix();

					if (vaccineButton.event)vaccineButton.bounds.setSize(vaccineButton.size.x*1.05, vaccineButton.size.y*1.05);
					else {
						vaccineButton.bounds.setSize(vaccineButton.size.x, vaccineButton.size.y);
					}
					vaccineButton.image.draw(vaccineButton.bounds);
					if (virusButton.event)virusButton.bounds.setSize(virusButton.size.x*1.05, virusButton.size.y*1.05);
					else {
						virusButton.bounds.setSize(virusButton.size.x, virusButton.size.y);
					}
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


				if (!gui->manualSelection && (displayBackButton || displaySkipButton)) {
					rightHand.cursor.draw(rightHand.pos);
				}
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

				if (playAgainButton.event)playAgainButton.bounds.setSize(playAgainButton.size.x*1.05, playAgainButton.size.y*1.05);
				else {
					playAgainButton.bounds.setSize(playAgainButton.size.x, playAgainButton.size.y);
				}
				playAgainButton.image.draw(playAgainButton.bounds);
			

			break;
		}
		}
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

		ofLogNotice("shadowpox") << "saving" << fileName;
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
	///logData.save(fileName);

	ofLogNotice("shadowpox") << "saving" << fileName;
	screenSaverPlayer.stop();
	screenSaverPlayer.close();
	introTextPlayer.stop();
	introTextPlayer.close();
}

void ofApp::SaveData() {
	ofxCsvRow data;

	// d/m/y h:m:s
	//data.addString(ofGetTimestampString("%Y-%m-%d-%H-%M-%S-%i"));
	data.addString(ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
	data.addString(sourceData[(selectedCountry->index) + 1].getString(sourceCol::COUNTRY));
	data.addString((isVaccine) ? "Yes" : "No");
	data.addInt(infectionScore);
	data.addInt(miniFig::getDeathScore());
	logData.addRow(data);
	logData.save(fileName);
	ofLogNotice("Shadowpox") << "data added to csv";

}


void ofApp::pointsOnCirleFromLine(ofPoint p1, ofPoint p2, float r, ofPoint& p3, ofPoint& p4, ofPoint& p5, ofPoint& p6) {

	// calculate the direction between joint1 and joint2. 
	ofVec2f dir = p2 - p1;
	
	// set the direction vector length to 1. 
	dir.normalize();
	// find a perpendicular line
	dir.perpendicular();
	// set the length to the radius. 
	dir.scale(r);

	// calculate the 4 points which make up the rectangle of the limb. 
	p3 = p1 - dir;
	p4 = p1 + dir;
	p5 = p2 + dir;
	p6 = p2 - dir;
}

void ofApp::drawHuman(int fig_color, bool showCursor) {

	if (kinect.isFrameNew()) {
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
					p2 = projector.getScreenCoordinateOfWorldPosition(body.joints[JointType_SpineBase].getPosition());

					patients[body.bodyId].bounds1 = p1;
					patients[body.bodyId].bounds2 = p2;
					if (gui->skeleton) {
						ofDrawLine(body.joints[bone.first].getPosition(), body.joints[bone.second].getPosition());
					}
				
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

				// body.joints[JointType_KneeLeft].getTrackingState() == TrackingState_Inferred || TrackingState_NotTracked

			
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

}
