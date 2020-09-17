/*
 * Copyright (c) 2012-2013 Agnes Chavez and Alessandro Saccoia
 * Written by Alessandro Saccoia, <alessandro.saccoia@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Sotware.
 */


#include "ofMain.h"
#include "ofApp.h"

#include "ofxAppUtils.h"

int main(){
	int dimensionX = 1920;
	int dimensionY = 1080;
	
	ofEnableDataPath();
	ofSetDataPathRoot("../Resources");
	
    ofSetupOpenGL(dimensionX,dimensionY, OF_WINDOW);
		
	ofRunApp(std::make_shared<ofxRunnerApp>(new ofApp()));
		  
	
	return 0;
}
//#include "KepleroUtils.h"
//#include "inputBox.h"
//
//#include "CoreFoundation/CoreFoundation.h"
//
//
//int main(){
//
//	#ifdef __APPLE__
//		CFBundleRef mainBundle = CFBundleGetMainBundle();
//		CFURLRef mainURL = CFBundleCopyExecutableURL(mainBundle);
//		char path[PATH_MAX];
//		if(!CFURLGetFileSystemRepresentation(mainURL, TRUE, (UInt8 *)path, PATH_MAX)){
//			std::cout << "Doesn't work like this..." << std::endl;
//			// error!
//		}
//		CFRelease(mainURL);
//
//		string strExecFile(path);
//		int found = strExecFile.find_last_of("/");
//		string strPath = strExecFile.substr(0, found + 1);
//		strPath.append("../../../data/screenDimensions.txt");
//	#endif
//
//	int resX = 1024;
//	int resY = 768;
//
//
//	std::ifstream myfile(strPath.c_str());
//	std::string line;
//	if(myfile.is_open()){
//		getline(myfile, line);
//		resX = ofToInt(line);
//		getline(myfile, line);
//		resY = ofToInt(line);
//		myfile.close();
//	}
//
//	// Always set the Extended Desktop to TRUE
//	bool bUseExtendedDesktop = true;
//	string cmd = "defaults write com.apple.glut GLUTUseExtendedDesktopKey ";
//	cmd += bUseExtendedDesktop ? "TRUE" : "FALSE";
//	std::system(cmd.c_str());
//
//
//	ofAppGlutWindow window;
//	ofSetupOpenGL(&window,  resX, resY, OF_FULLSCREEN);
//	ofRunAppWithAppUtils(new ofApp());
//}
