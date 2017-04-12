#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	/*ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
	*/


	ofGLFWWindowSettings settings;

	// Check screens size and location
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	cout << "Number of screens found: " << count << endl;
	if (count>1) {
		int xM; int yM;
		glfwGetMonitorPos(monitors[1], &xM, &yM); // We take the second monitor
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[1]);

		settings.width = desktopMode->width;
		settings.height = desktopMode->height;
		settings.setPosition(ofVec2f(xM, yM));
		
	}
	else {
		settings.width = 1280;
		settings.height = 768;
		settings.resizable = true;
		settings.decorated = false;
		settings.setPosition(ofVec2f(0, 0));
		settings.monitor = 1;
		
	}
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	
	settings.width = 400;
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




