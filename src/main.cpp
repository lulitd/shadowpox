#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

int main( ){
	
	ofGLFWWindowSettings settings;

	settings.width = 1920-GUIWIDTH;
	settings.height = 1080;
	settings.resizable = true;
	settings.decorated = false;
	settings.setPosition(ofVec2f(0,0));
	settings.monitor = 1;
	
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	
	settings.width = GUIWIDTH;
	settings.height = 900;
	settings.resizable = true;
	settings.decorated = true;
	settings.setPosition(ofVec2f(0, 50));
	settings.monitor = 0;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);

	mainApp->gui = guiApp;

	ofRunApp(guiWindow, guiApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}


