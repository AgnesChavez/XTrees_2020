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


#include "Settings.h"
#include "ofApp.h"
#include "LeavesLayer.h"
#include "ofMain.h"
#include "GraphicUtilities.hpp"
//#include "MSACore.h"
//#include "MSAFluidSolver.h"

#include "guiCustomColorPreview.h"
#include "guiCustomGradientPreview.h"
#include "guiTypeConfigFileList.h"



using namespace MSA;

#include <fstream>
#include <iostream>



#ifndef XTREES_IOS
	ofApp * g_app;
#endif


//unsigned int globalSettings::g_globalCounter = 0; // number of frames, updated in update();
//unsigned int globalSettings::g_globalCounterSec = 0; // number of seconds
//unsigned int globalSettings::g_splashFrequency = 0;
//unsigned int globalSettings::g_splashDuration = 0;
//float globalSettings::g_splashOpacity = 0;

//ofApp* globalSettings::g_app = nullptr;

//guiCustomColorPreview * globalSettings::g_treeGradStartPreview = nullptr;
//guiCustomColorPreview * globalSettings::g_treeGradEndPreview = nullptr;
//ofColor globalSettings::g_treeGradStart = ofColor();
//ofColor globalSettings::g_treeGradEnd = ofColor();
//guiCustomGradientPreview * globalSettings::g_treeGradPreview = nullptr;
// guiTypeConfigFileList* m_configurationLister = nullptr;
//float globalSettings::g_treeGhostOpacity = 0.0f;


//guiCustomColorPreview * globalSettings::g_treeColorPreview = nullptr;
//ofColor globalSettings::g_backgroundC = ofColor();
//ofColor globalSettings::g_treeC = ofColor();
//bool globalSettings::g_jointsAreCircles = true;
//ofColor globalSettings::g_jointC = ofColor();
//float globalSettings::g_branchWidth = 0.0f;
//float globalSettings::g_branchMinWidth = 0.0f;
//float globalSettings::g_branchLength = 0.0f;
//float globalSettings::g_lengthDecrease = 0.0f;
//float globalSettings::g_widthDecrease = 0.0f;
//int globalSettings::g_pastTweets = 0;
//int globalSettings::g_jointThickness = 0;
//int globalSettings::g_jointLength = 0;
//ofColor globalSettings::g_tweetMsgC = ofColor();
//ofColor globalSettings::g_tweetUsrC = ofColor();
//ofColor globalSettings::g_twilioMsgC = ofColor();
//ofColor globalSettings::g_twilioUsrC = ofColor();
//ofColor globalSettings::g_databaseMsgC = ofColor();
//ofColor globalSettings::g_databaseUsrC = ofColor();
//ofColor globalSettings::g_tLineC = ofColor();
//int globalSettings::g_tLineWidth = 0;
//int globalSettings::g_growthMin = 0;
//int globalSettings::g_growthMax = 0;
//bool globalSettings::g_timedExhibit = false;
//float globalSettings::g_showDuration = 10;
//int globalSettings::g_minFrequency = 0;
//int globalSettings::g_maxFrequency = 100;
//int globalSettings::g_minAngle = 0;
//int globalSettings::g_maxAngle = 360;
//bool globalSettings::g_showThumbs = false;
//int globalSettings::g_fertility = 5;
//int globalSettings::g_maxLevel = 5;
//int globalSettings::g_maxBranches = 5;
//float globalSettings::g_floatingSpeed = 1;
//ofColor globalSettings::g_splashC = ofColor();
//int globalSettings::g_flowersMin = 0;
//int globalSettings::g_flowersMax = 10;
//int globalSettings::g_crazyLineWidth = 1;
//int globalSettings::g_crazyCircleWidth = 10;
//bool globalSettings::g_useBackground = false;
//int globalSettings::g_tweetMinLife = 1000;
//int globalSettings::g_tweetMaxLife = 10000;

//std::map <string, int> globalSettings::g_textAnchorX =  std::map <string, int>();
//std::map <string, int> globalSettings::g_textAnchorY =  std::map <string, int>();



//std::vector <ofImage > globalSettings::g_originalImages = std::vector <ofImage >();
//std::vector <ofImage > globalSettings::g_leafImages = std::vector <ofImage >();
//std::vector <ofImage > globalSettings::g_backgroundImages = std::vector <ofImage >();

//ofRectangle globalSettings::g_scene = ofRectangle();
//ofRectangle globalSettings::g_sceneBounded = ofRectangle();

//float globalSettings::g_soundVolume = 1.0f;

//bool globalSettings::g_useInteractiveAudio = true;

//bool globalSettings::g_useBackgroundImage = false;
//bool globalSettings::g_changeBranchImages = true;

//std::string globalSettings::g_backgroundImage =  std::string();
//std::string globalSettings::g_soundtrack = std::string();

//float globalSettings::g_minColonization = 1.f;
//float globalSettings::g_minColonizationForLeaves = 1.f;
//bool globalSettings::g_treesCanFade = true;
//float globalSettings::g_fps = 0.f;
//float globalSettings::g_currentFps = 0.f;

//int globalSettings::g_numberOfseeds = 1;


//float globalSettings::g_treesLayerOpacity = 1.f;
//float globalSettings::g_twitterLayerOpacity = 1.f;
//float globalSettings::g_flowersOpacity = 1.f;

//ofImage globalSettings::g_flowerImage = ofImage();
//bool globalSettings::g_showFlowers = false;

//FluidSolver * globalSettings::s_solver = nullptr;
//Vec2f globalSettings::s_windowSize = Vec2f();
//Vec2f globalSettings::s_invWindowSize= Vec2f();

//LeavesLayer * globalSettings::g_leavesLayer = nullptr;

//float globalSettings::g_leavesFertility = 1.f;
//ofColor globalSettings::g_leavesEndColor = ofColor();
//float globalSettings::g_leavesOpacity = 1.f;
//float globalSettings::g_leavesLife = 1.f;
//float globalSettings::g_leavesMinWidth = 1.f;
//float globalSettings::g_leavesMaxWidth = 1.f;
//ofColor globalSettings::g_flowersColor = ofColor();
//ofColor globalSettings::g_flowersLineColor = ofColor();

//bool globalSettings::g_useTwilio = false;

//bool globalSettings::g_useTwitter = true;

//int globalSettings::g_leavesRows = 10;
//int globalSettings::g_leavesColumns = 10;

//bool globalSettings::g_leavesActive = false;

//float globalSettings::g_leavesMinFreq = 0;
//float globalSettings::g_leavesMaxFreq = 100;

//int globalSettings::g_treesFadeTime = 1000;

//int globalSettings::g_backgroundTransitionTime = 1000;
//bool globalSettings::g_useArchive = false;

//std::atomic<int> globalSettings::g_activeThreads(0);
////ofMutex globalSettings::g_activeThreadsMutex;


//float globalSettings::g_waitSeedTime = 1.f; // wait in seed mode
//float globalSettings::g_waitLinesTime = 1.f; // tree full, wait to start lines
////float globalSettings::g_waitLeavesTime; // how long wait for leaves
//float globalSettings::g_waitRegenerateTime = 1.f; // after leaves fell how long does it take to regenerate
//float globalSettings::g_startDetachingLeavesTime = 1.f;

//int globalSettings::g_msgOpacity = 255;

//float globalSettings::g_linesMin = 0.f;
//float globalSettings::g_linesMax = 1.f;
//float globalSettings::g_linesMinNorm = 0.f;
//float globalSettings::g_linesMaxNorm = 1.f;
//float globalSettings::g_linesMinAcc = 0.f;
//float globalSettings::g_linesMaxAcc = 1.f;


//int globalSettings::g_firstIterations = 0;




void globalSettings::g_deallocateFonts()
{
	if(_guiFont){ delete _guiFont; _guiFont = nullptr;}
	if(_twitterFont){ delete _twitterFont; _twitterFont = nullptr;}
	if(_twilioFont){ delete _twilioFont; _twilioFont = nullptr;}
	if(_databaseFont){ delete _databaseFont; _databaseFont = nullptr;}
}
	
//FTTextureFont* globalSettings::_guiFont = nullptr;
//FTTextureFont* globalSettings::_twitterFont = nullptr;
//FTTextureFont* globalSettings::_twilioFont = nullptr;
//FTTextureFont* globalSettings::_databaseFont = nullptr;



FTTextureFont* makeFTFont(const std::string & filepath, int fontsize)
{
	FTTextureFont* font = new FTTextureFont(ofToDataPath(filepath).c_str());
	font->Outset(0.0f, fontsize);
	font->CharMap(ft_encoding_unicode);
	if(font->Error()){
		ofLog(OF_LOG_ERROR, "error loading font");
		exit(1);
		return nullptr;
	}
	if(!font->FaceSize(fontsize)){
		ofLog(OF_LOG_ERROR, "Failed to set size");
		exit(1);
		return nullptr;
	}


	return font;
}



FTTextureFont & globalSettings::g_twitterFont()
{
	if(_twitterFont == nullptr)
	{
		_twitterFont = makeFTFont("fonts/ArialUnicode.ttf", 12);
	}
	return *_twitterFont;
	
}




FTTextureFont & globalSettings::g_twilioFont()
{
	if(_twilioFont == nullptr)
	{
		_twilioFont = makeFTFont("fonts/ArialUnicode.ttf", 12);
	}
	return *_twilioFont;
}

FTTextureFont & globalSettings::g_databaseFont()
{
	if(_databaseFont == nullptr)
	{
		_databaseFont = makeFTFont("fonts/ArialUnicode.ttf", 12);
	}
	return *_databaseFont;
}




FTTextureFont & globalSettings::g_guiFont()
{
	if(_guiFont == nullptr)
	{
		_guiFont = makeFTFont("fonts/ArialUnicode.ttf", 16);
	}
	return *_guiFont;
	
}

std::unique_ptr<FTSimpleLayout> makeLayout(FTTextureFont* font, float lineLength, const FTGL::TextAlignment alignment,  float lineSpacing)
{
	std::unique_ptr<FTSimpleLayout> layout = std::make_unique<FTSimpleLayout>();
	layout->SetFont(font);
	layout->SetLineLength(lineLength);
	layout->SetAlignment(alignment);
	layout->SetLineSpacing(lineSpacing);
	
	return std::move(layout);
}

FTSimpleLayout & globalSettings::g_guiRenderer()
{
	static std::unique_ptr<FTSimpleLayout> layout = nullptr;
	if(layout == nullptr)
	{
		layout = makeLayout(&g_guiFont(), 300, FTGL::ALIGN_CENTER, .9F);
	}

	return *layout;

}

FTSimpleLayout & globalSettings::g_twitterLayout()
{
	
	static std::unique_ptr<FTSimpleLayout> layout = nullptr;
	if(layout == nullptr)
	{
		layout = makeLayout(&g_twitterFont(), BALOON_MSG_SIZE, FTGL::ALIGN_JUSTIFY, BALOON_MWG_LINESPACING);
		
	}
	return *layout;
	
}

FTSimpleLayout & globalSettings::g_twilioLayout()
{
	static std::unique_ptr<FTSimpleLayout> layout = nullptr;
	if(layout == nullptr)
	{
		layout = makeLayout(&g_twilioFont(), BALOON_MSG_SIZE, FTGL::ALIGN_JUSTIFY, BALOON_MWG_LINESPACING);
	}
	return *layout;
}

FTSimpleLayout & globalSettings::g_databaseLayout()
{
	static std::unique_ptr<FTSimpleLayout> layout = nullptr;
	if(layout == nullptr)
	{
		layout = makeLayout(&g_databaseFont(), BALOON_MSG_SIZE, FTGL::ALIGN_JUSTIFY, BALOON_MWG_LINESPACING);
	}
	return *layout;
}


void globalSettings::g_setTwitterMsgFontSize(int fontsize){
	g_twitterFont().Outset(0.0f, fontsize);
	g_twitterFont().FaceSize(fontsize);
}
void globalSettings::g_setTwilioMsgFontSize(int fontsize){
	g_twilioFont().Outset(0.0f, fontsize);
	g_twilioFont().FaceSize(fontsize);
}

void globalSettings::g_setDatabaseMsgFontSize(int fontsize){
	g_databaseFont().Outset(0.0f, fontsize);
	g_databaseFont().FaceSize(fontsize);
}

void loadImgDir(std::vector<ofImage>& imgs, const std::string& dirPath)
{
	ofDirectory dir;
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.allowExt("jpeg");
	dir.allowExt("tiff");
	dir.listDir(ofToDataPath(dirPath));
	
	imgs.resize(dir.size());
	for(int i = 0; i < dir.size(); ++i){
		imgs[i].load(dir.getPath(i));
	}
}


void globalSettings::g_initializeBranchImages(){
	g_changeBranchImages = false;
	loadImgDir(g_originalImages, "linesTex");
}


void globalSettings::g_initializeBackgroundImages(){
	loadImgDir(g_backgroundImages, "backgrounds");
	
}


void globalSettings::g_initializeLeafImages(){
	loadImgDir(g_leafImages, "graphics/leaves");
}


void globalSettings::g_computeMaxBranches(){
	g_maxBranches = 0;
	for(int i = 0; i < globalSettings::g_maxLevel; ++i){
		g_maxBranches += 2 + (i + globalSettings::g_fertility) / 3;
	}
}


void globalSettings::g_updateBackground(){
	ofBackground(g_backgroundC);
	g_app->m_fadeRectangle.SetColor(g_backgroundC);
	XTree::clearFbo();
}

void globalSettings::g_activateSoundtrack(){
	//if (!g_useInteractiveAudio) {
	//g_app->soundtrack.unloadSound();
	//g_app->soundtrack.loadSound("soundtracks/" + g_soundtrack, true);
	//}
}

