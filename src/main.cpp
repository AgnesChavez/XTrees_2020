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
#include <mach-o/dyld.h>
#include <limits.h>
#include "ofxAppUtils.h"
#include "ofConstants.h"

int main(){
	
	// Set the data path to the resources folder inside the app bundle
	ofEnableDataPath();
	ofSetDataPathRoot("../Resources");
	
	
	
	int screenWidth = 1920;
	int screenHeight = 1080;
	
	
	bool startOnFullscreen = false;

	auto filepath =   ofFilePath::join(ofFilePath::getCurrentExeDir(), "../Resources/screenDimensions.json");
	
	auto json = ofLoadJson(filepath);
	

	if(json.count("screenWidth")){  screenWidth = json["screenWidth"].get<int>();}
	if(json.count("screenHeight")){  screenHeight = json["screenHeight"].get<int>();}
	if(json.count("startOnFullscreen")){  startOnFullscreen = json["startOnFullscreen"].get<bool>();}
	
	
    ofSetupOpenGL(screenWidth, screenHeight, (startOnFullscreen?OF_FULLSCREEN:OF_WINDOW));
		
	ofRunApp(std::make_shared<ofxRunnerApp>(new ofApp()));
		  
	
	return 0;
}
