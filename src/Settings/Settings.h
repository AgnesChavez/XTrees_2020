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

#include "Singleton.hpp"
#include "MSACore.h"
//#include "MSAFluidSolver.h"
//
//using namespace MSA;

#include "ofxGui.h"

#define BALOON_MSG_SIZE 300
#define BALOON_MWG_LINESPACING .9F



class ofApp;
class LeavesLayer;


class globalSettings:
public Singleton<globalSettings>
{
public:
	
	globalSettings();
	
	ofParameterGroup background_parameters =  {"Background"};
	ofParameterGroup layers_control_parameters =  {"Layers control"};
	ofParameterGroup audio_settings_parameters =  {"Audio settings"};
	ofParameterGroup loop_settings_parameters =  {"Loop settings"};
	ofParameterGroup general_tree_structure_parameters =  {"Tree structure"};
	ofParameterGroup leaves_parameters =  {"Leaves"};
	ofParameterGroup timing_parameters =  {"Timing"};
	
	
	ofParameterGroup splash_screen_parameters =  {"Splash screen"};
	ofParameterGroup branches_parameters =  {"Branches"};
	ofParameterGroup branch_joints_parameters =  {"Branch joints"};
	ofParameterGroup flowers_parameters =  {"Flowers"};

	ofParameterGroup messages_parameters = {"Messages"};
	ofParameterGroup lines_parameters = {"Lines"};
	
	ofParameter<ofColor> g_treeGradStart = {"Tree Gradient Start", {0,0,0,0}, {0,0,0,0}, {255,255,255,255}};
	ofParameter<ofColor> g_treeGradEnd = {"Tree Gradient End", {0,0,0,0}, {0,0,0,0}, {255,255,255,255}};

		
	ofParameter<ofColor> g_backgroundC = { "Background Color",  0, 0, 255}; //backgroundR
	ofParameter<int> g_backgroundTransitionTime = { "Background trans. time",  10, 1, 120}; //addSlider        backgroundTransitionTime

	ofParameter<int> g_showDuration = { "Show duration (min)",  10, 1, 60}; //showDuration

	ofParameter<bool> g_leavesActive = { "Activate leaves",  false};//addToggle        showLeaves
	ofParameter<bool> g_useInteractiveAudio = { "Use interactive audio",  true};//addToggle
	ofParameter<bool> g_useTwilio = { "Use twilio",  true};//addToggle
	ofParameter<bool> g_showFlowers = { "Activate flowers",  false};//addToggle        showFlowers
	ofParameter<bool> g_showThumbs = { "Show twitter thumbnails",  true};//addToggle        showThumbs


	ofParameter<int> g_growthMax = { "Branch max (ms)",  3000, 1000, 10000}; //growthMax
	ofParameter<int> g_growthMin = { "Branch min (ms)",  1500, 1000, 10000}; //growthMin
	ofParameter<float> g_branchLength = { "Branch length (px)",  100, 50, 500}; //branchLength
	ofParameter<float> g_branchMinWidth = { "Branch min width (px)",  1, 1, 200}; //branchMinWidth
	ofParameter<float> g_branchWidth = { "Branch width (px)",  1, 1, 200}; //branchWidth

	ofParameter<int> g_crazyCircleWidth = { "Circle Width",  15, 2, 50}; //crazyCircleWidth
	ofParameter<int> g_crazyLineWidth = { "Line Width",  2, 1, 10}; //crazyLineWidth

	ofParameter<int> g_fertility = { "Fertility",  0, 0, 5}; //fertility
	ofParameter<int> g_firstIterations = { "First iterations",  3, 0, 10}; //firstIterations
	ofParameter<float> g_floatingSpeed = { "Floating speed",  0.06, 0, 0.3}; //floatingSpeed

	ofParameter<int> g_flowersMax = { "Flowers max (ms)",  300, 10, 10000}; //addSlider        flowersMax
	ofParameter<int> g_flowersMin = { "Flowers min (ms)",  50, 10, 10000}; //addSlider        flowersMin
	ofParameter<float> g_flowersOpacity = { "Flowers opacity",  255, 0, 255}; //addSlider        flowersOpacity

	ofParameter<ofColor> g_jointC = { "Joint Color",  255, 0, 255}; //jointR
	ofParameter<int> g_jointLength = { "Joint length",  20, 0, 80}; //jointLength
	ofParameter<bool> g_jointsAreCircles = { "Joints are circles",  true};//addToggle        jointsAreCircles
	ofParameter<int> g_jointThickness = { "Joint thickness",  1, 1, 10}; //jointThickness


	ofParameter<ofColor> g_leavesEndColor = { "Leaf Color",  0, 0, 255}; //tLeavesR
	ofParameter<float> g_leavesFertility = { "Leaf fertility",  1, 1, 10}; //leafFertility
	ofParameter<int> g_leavesLife = { "Leaves life",  100, 1, 200}; //leavesLife
	ofParameter<int> g_leavesMinFreq = { "Leaves min (ms)",  5000, 100, 100000}; //addSlider        leavesMinFreq
	ofParameter<int> g_leavesMaxFreq = { "Leaves max (ms)",  15000, 100, 100000}; //addSlider        leavesMaxFreq
	ofParameter<float> g_leavesMinWidth = { "Leaf min width",  100, 5, 300}; //leafMinWidth
	ofParameter<float> g_leavesMaxWidth = { "Leaf max width",  140, 5, 300}; //leafMaxWidth
	ofParameter<float> g_leavesOpacity = { "Leaf Opacity",  255, 0, 255}; //tLeavesA
	ofParameter<int> g_leavesColumns = { "Leaves columns",  6, 1, 20}; //leavesColumns
	ofParameter<int> g_leavesRows = { "Leaves rows",  8, 1, 20}; //leavesRows
	ofParameter<int> g_startDetachingLeavesTime = { "Detach leaves after time",  5, 0, 60}; //addSlider        startDetachingLeavesTime

	ofParameter<float> g_lengthDecrease = { "Length decrease (%)",  0, 0, 1}; //lengthDecrease
	ofParameter<int> g_linesMax = { "Lines max (ms)",  300, 10, 10000}; //addSlider        LinesMax
	ofParameter<int> g_linesMin = { "Lines min (ms)",  50, 10, 10000}; //addSlider        LinesMin
	ofParameter<int> g_maxAngle = { "Max angle (deg)",  90, 1, 90}; //addSlider        maxAngle
	ofParameter<int> g_maxFrequency = { "Twitter max (ms)",  7000, 100, 20000}; //maxFrequency
	ofParameter<int> g_maxLevel = { "Max levels",  10, 3, 20}; //maxLevel
	ofParameter<int> g_minAngle = { "Min angle (deg.)",  1, 1, 90}; //addSlider        minAngle
	ofParameter<float> g_minColonization = { "Min colonization to regenerate",  30, 1, 100}; //minColonization
	ofParameter<float> g_minColonizationForLeaves = { "Min colonization to create leaves",  20, 1, 100}; //minColonizationForLeaves
	ofParameter<int> g_minFrequency = { "Twitter min (ms)",  3000, 100, 20000}; //minFrequency
	ofParameter<int> g_msgOpacity = { "Message opacity",  150, 0, 255}; //addSlider        msgOpacity
	ofParameter<int> g_pastTweets = { "Nr. past tweets",  5, 0, 100}; //pastTweets

	ofParameter<float> g_soundVolume = { "Global Volume",  0.3, 0, 1}; //addSlider        soundVolume

	ofParameter<ofColor> g_splashC = { "Splash Screen Color",  255, 0, 255}; //splashR
	ofParameter<int> g_splashDuration = { "Splash duration",  15, 5, 60}; //addSlider        splashDuration
	ofParameter<int> g_splashFrequency = { "Splash frequency",  60, 30, 600}; //addSlider        splashFrequency
	ofParameter<float> g_splashOpacity = { "Splash opacity",  255, 0, 255}; //addSlider        splashOpacity

	ofParameter<bool> g_timedExhibit = { "Timed show",  true};//addToggle        timedExhibit

	ofParameter<ofColor> g_tLineC = { "Line Color",  0, 0, 255}; //tLineR
	ofParameter<int> g_tLineWidth = { "Line width",  1, 1, 10}; //tLineWidth

	ofParameter<ofColor> g_treeC = { "Branch Color",  255, 0, 255}; //treeR
	ofParameter<int> g_treeGhostOpacity = { "Ghost Alpha",  255, 0, 255}; //ghostAlpha
	ofParameter<int> g_treesFadeTime = { "Ghost img time",  30, 1, 60}; //addSlider        treesFadeTime
	ofParameter<float> g_treesLayerOpacity = { "Trees opacity",  255, 0, 255}; //addSlider        treesOpacity

	ofParameter<float> g_tweetMaxLife = { "Max life (ms)",  10000, 1000, 20000}; //addSlider        tweetMax
	ofParameter<float> g_tweetMinLife = { "Min life (ms)",  5000, 1000, 20000}; //addSlider        tweetMin
	ofParameter<float> g_twitterLayerOpacity = {  "Twitter opacity",  255, 0, 255}; //addSlider


	//	ofParameter<int> g_waitLinesTime = { "Wait for lines time",  5, 0, 60}; //addSlider
	ofParameter<int> g_waitLinesTime = { "Wait in seed mode",  5, 0, 60}; //addSlider     waitSeedTime
	ofParameter<int> g_waitRegenerateTime = { "Wait after regenerate time",  5, 0, 60}; //addSlider     waitRegenerateTime
	ofParameter<int> g_waitSeedTime = { "Seed time",  5, 1, 60}; //addSlider     waitSeedTime
	ofParameter<float> g_widthDecrease = { "Width decrease (%)",  0, 0, 1}; //widthDecrease

	
	ofParameter<float> g_linesWidth= {"Line Width", 2, 1, 10};
	ofParameter<ofColor> g_linesColor = {"Lines Color", {255,255,255,255}, {0,0,0,0}, {255,255,255,255}};
	
	
	ofParameterGroup messagesColors = {"Messages colors"};
	
	ofParameter<ofColor> g_tweetMsgC = { "Tweet Message",  255, 0, 255}; //tweetMsgR
	ofParameter<ofColor> g_tweetUsrC = { "Tweet User",  255, 0, 255}; //tweetMsgR
	ofParameter<ofColor> g_twilioMsgC = { "Twilio Message",  255, 0, 255}; //tweetMsgR
	ofParameter<ofColor> g_twilioUsrC = { "Twilio User",  255, 0, 255}; //tweetMsgR
	ofParameter<ofColor> g_databaseMsgC = { "Archive Message",  255, 0, 255}; //tweetMsgR
	ofParameter<ofColor> g_databaseUsrC = { "Archive User",  255, 0, 255}; //tweetMsgR
	
	///////////

	int g_maxBranches = 5; // how many branches per tree given the currentLevel

	bool g_changeBranchImages = true;
		
	
	unsigned int g_globalCounter = 0;
	unsigned int g_globalCounterSec = 0;
	
	ofApp* g_app = nullptr;
	
	bool g_useBackgroundImage = false;
	
	std::string g_backgroundImage;
	std::string g_soundtrack;
	
	
	bool g_treesCanFade = true;
	float g_fps = 0.f;
	float g_currentFps = 0.f;
	
	int g_numberOfseeds = 1;
	
	
	bool g_useTwitter = true;
	
	
	bool g_useArchive = false;
	
	
	ofImage g_flowerImage;

	MSA::Vec2f s_windowSize;
	MSA::Vec2f s_invWindowSize;
	
	LeavesLayer* g_leavesLayer = nullptr;
	
	
	std::map<string, int> g_textAnchorX;
	std::map<string, int> g_textAnchorY;
	
	
	ofRectangle g_scene;
	ofRectangle g_sceneBounded;
	
	std::atomic<int> g_activeThreads;


	
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

	
	void drawGui();
	
	void save();
	void load();
	
	void saveTo(const string& filename);
	void loadFrom(const string& filename);
	
	void setupGuis();
	
private:
	
	FTTextureFont* _guiFont = nullptr;
	FTTextureFont* _twitterFont = nullptr;
	FTTextureFont* _twilioFont = nullptr;
	FTTextureFont* _databaseFont = nullptr;
	
	ofEventListeners listeners;

	
	
	

	ofxPanel gui;
	ofxPanel general_tree_structure_gui;
	ofxPanel timing_gui;
	ofxPanel messages_gui;
	ofxPanel lines_gui;
	ofxPanel audio_settings_gui;

	void addSaveLoadListeners(ofxPanel& gui);
	
	string currentFilename = "settings.json";
	
	
	ofParameter<void> saveParam = {"Save"};
	ofParameter<void> saveAsParam = {"Save as ..."};
	ofParameter<void> loadParam = {"Load"};
	ofParameter<void> reloadParam = {"Reload"};
	
	
};
#endif //

