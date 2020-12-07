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

/*
 * generated with
 * http://www.kjetil-hartveit.com/blog/1/setter-and-getter-generator-for-php-javascript-c%2B%2B-and-csharp#SG_Arguments
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>
#include "ofMain.h"
#include "ofConstants.h"

#ifndef XTREES_IOS
#include <FTGL/ftgl.h>
#else
#include "FTGL/ftgles.h"
#endif


//#include "MSACore.h"
//#include "MSAFluidSolver.h"
//
//using namespace MSA;


#define BALOON_MSG_SIZE 300
#define BALOON_MWG_LINESPACING .9F


#define notifySetting(eventNr) Setting tont = eventNr; ofNotifyEvent(ofConfigurationChanged, tont, this)
class ofApp;
class LeavesLayer;
class guiCustomColorPreview;
class guiCustomGradientPreview;
class guiTypeConfigFileList;

class globalSettings
{
public:
	
	
	unsigned int g_globalCounter = 0;
	unsigned int g_globalCounterSec = 0;
	unsigned int g_splashFrequency = 0;
	unsigned int g_splashDuration = 0;
	
	float g_splashOpacity = 0;
	
	ofApp* g_app = nullptr;
	
	guiCustomColorPreview *g_treeColorPreview = nullptr;
	//Gradient
	guiCustomColorPreview *g_treeGradStartPreview = nullptr;
	guiCustomColorPreview *g_treeGradEndPreview = nullptr;
	
	ofColor g_treeGradStart;
	ofColor g_treeGradEnd;
	guiCustomGradientPreview *g_treeGradPreview = nullptr;
	guiTypeConfigFileList *m_configurationLister = nullptr;
	float g_treeGhostOpacity = 0;
	
	ofColor g_backgroundC;
	ofColor g_treeC;
	bool g_jointsAreCircles = true;
	ofColor g_jointC;
	float g_branchWidth = 0;
	float g_branchMinWidth = 0;
	float g_branchLength = 0;
	float g_lengthDecrease = 0;
	float g_widthDecrease = 0;
	int g_pastTweets = 0;
	int g_jointThickness = 0;
	int g_jointLength = 0;
	
	ofColor g_tweetMsgC;
	ofColor g_tweetUsrC;
	ofColor g_twilioMsgC;
	ofColor g_twilioUsrC;
	ofColor g_databaseMsgC;
	ofColor g_databaseUsrC;
	
	ofColor g_tLineC;
	int g_tLineWidth = 0;
	int g_growthMin = 0;
	int g_growthMax = 0;
	bool g_timedExhibit = false;
	float g_showDuration = 10;
	int g_minFrequency = 0;
	int g_maxFrequency = 100;
	int g_minAngle = 0;
	int g_maxAngle = 360;
	bool g_showThumbs = false;
	int g_fertility = 5;
	int g_maxLevel = 5; // how many branches per tree given the currentLevel
	int g_maxBranches = 5; // how many branches per tree given the currentLevel
	float g_floatingSpeed = 1;
	ofColor g_splashC;
	int g_flowersMin = 0;
	int g_flowersMax = 10;
	int g_crazyLineWidth = 1;
	int g_crazyCircleWidth = 10;
	bool g_useBackground = false;
	int g_tweetMinLife = 1000;
	int g_tweetMaxLife = 10000;
	
	std::map<string, int> g_textAnchorX;
	std::map<string, int> g_textAnchorY;
	
	
	FTTextureFont&  g_guiFont();
	FTSimpleLayout& g_guiRenderer();
	
	FTSimpleLayout& g_twitterLayout();
	FTSimpleLayout& g_twilioLayout();
	FTSimpleLayout& g_databaseLayout();
	
	FTTextureFont& g_twitterFont();
	FTTextureFont& g_twilioFont();
	FTTextureFont& g_databaseFont();
	
	std::vector<ofImage> g_originalImages;
	std::vector<ofImage> g_leafImages;
	std::vector<ofImage> g_backgroundImages;
	
	bool g_changeBranchImages = true;
	
	ofRectangle g_scene;
	ofRectangle g_sceneBounded;
	
	float g_soundVolume = 1;
	
	bool g_useInteractiveAudio  = true;
	
	bool g_useBackgroundImage = false;
	std::string g_backgroundImage;
	std::string g_soundtrack;
	
	float g_minColonization = 1.f;
	float g_minColonizationForLeaves = 1.f;
	bool g_treesCanFade = true;
	float g_fps = 0.f;
	float g_currentFps = 0.f;
	
	int g_numberOfseeds = 1;
	
	
	float g_treesLayerOpacity = 1.f;
	float g_twitterLayerOpacity = 1.f;
	float g_flowersOpacity = 1.f;
	
	ofImage g_flowerImage;
	
	bool g_showFlowers = false;
	
//	FluidSolver *s_solver = nullptr;
	Vec2f s_windowSize;
	Vec2f s_invWindowSize;
	
	LeavesLayer* g_leavesLayer = nullptr;
	
//	float g_leavesFertility = 1.f;
//	ofColor g_leavesEndColor;
//	float g_leavesOpacity = 1.f;
//	float g_leavesLife = 1.f;
//	float g_leavesMinWidth = 1.f;
//	float g_leavesMaxWidth = 1.f;
//	float g_leavesMinFreq = 0;
//	float g_leavesMaxFreq = 100;
	
	ofColor g_flowersColor;
	ofColor g_flowersLineColor;
	
	bool g_useTwilio = false;
	bool g_useTwitter = true;
	
	
	int g_leavesRows = 10;
	int g_leavesColumns = 10;
	
	bool g_leavesActive = false;
	
	int g_treesFadeTime = 1000;
	
	int g_backgroundTransitionTime = 1000;
	
	bool g_useArchive = false;
	
	std::atomic<int> g_activeThreads;
	
	float g_waitSeedTime = 1.0f; // wait in seed mode
	float g_waitLinesTime = 1.0f; // tree full, wait to start lines
	float g_startDetachingLeavesTime = 1.0f;
	float g_waitRegenerateTime = 1.0f; // after leaves fell how long does it take to regenerate
	
	
	int g_msgOpacity = 255;
	
	float g_linesMin = 0.f;
	float g_linesMax = 1.f;
	float g_linesMinNorm = 0.f;
	float g_linesMaxNorm = 1.f;
	float g_linesMinAcc = 0.f;
	float g_linesMaxAcc = 1.f;
	
	int g_firstIterations = 0;
	
	/////////////////////////////////////////////////////////////////////////
	
	//	 void g_initializeFonts();
	
	void g_initializeBranchImages();
	void g_initializeLeafImages();
	void g_initializeBackgroundImages();
	
	void g_setTwitterMsgFontSize(int fontsize);
	void g_setTwilioMsgFontSize(int fontsize);
	void g_setDatabaseMsgFontSize(int fontsize);
	
	void g_colorizeSplashImages();
	void g_computeMaxBranches();
	void g_updateBackground();
	void g_activateSoundtrack();
	
	void g_deallocateFonts();
	
private:
	FTTextureFont* _guiFont = nullptr;
	FTTextureFont* _twitterFont = nullptr;
	FTTextureFont* _twilioFont = nullptr;
	FTTextureFont* _databaseFont = nullptr;
};
#endif //

