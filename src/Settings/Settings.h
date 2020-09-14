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


#include "MSACore.h"
#include "MSAFluidSolver.h"

using namespace MSA;


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
	
	
	static unsigned int g_globalCounter;
	static unsigned int g_globalCounterSec;
	static unsigned int g_splashFrequency;
	static unsigned int g_splashDuration;
	static float g_splashOpacity;
	
	
	static ofApp* g_app;
	
	static guiCustomColorPreview *g_treeColorPreview;
	
	//Gradient
	static guiCustomColorPreview *g_treeGradStartPreview;
	static guiCustomColorPreview *g_treeGradEndPreview;
	static ofColor g_treeGradStart;
	static ofColor g_treeGradEnd;
	static guiCustomGradientPreview *g_treeGradPreview;
	static guiTypeConfigFileList *m_configurationLister;
	static float g_treeGhostOpacity;
	
	static ofColor g_backgroundC;
	static ofColor g_treeC;
	static bool g_jointsAreCircles;
	static ofColor g_jointC;
	static float g_branchWidth;
	static float g_branchMinWidth;
	static float g_branchLength;
	static float g_lengthDecrease;
	static float g_widthDecrease;
	static int g_pastTweets;
	static int g_jointThickness;
	static int g_jointLength;
	
	static ofColor g_tweetMsgC;
	static ofColor g_tweetUsrC;
	static ofColor g_twilioMsgC;
	static ofColor g_twilioUsrC;
	static ofColor g_databaseMsgC;
	static ofColor g_databaseUsrC;
	
	static ofColor g_tLineC;
	static int g_tLineWidth;
	static int g_growthMin;
	static int g_growthMax;
	static bool g_timedExhibit;
	static float g_showDuration;
	static int g_minFrequency;
	static int g_maxFrequency;
	static int g_minAngle;
	static int g_maxAngle;
	static bool g_showThumbs;
	static int g_fertility;
	static int g_maxLevel; // how many branches per tree given the currentLevel
	static int g_maxBranches; // how many branches per tree given the currentLevel
	static float g_floatingSpeed;
	static ofColor g_splashC;
	static int g_flowersMin;
	static int g_flowersMax;
	static int g_crazyLineWidth;
	static int g_crazyCircleWidth;
	static bool g_useBackground;
	static int g_tweetMinLife;
	static int g_tweetMaxLife;
	
	static std::map<string, int> g_textAnchorX;
	static std::map<string, int> g_textAnchorY;
	
	
	static FTFont*  g_guiFont;
	static FTSimpleLayout* g_guiRenderer;
	
	static FTSimpleLayout* g_twitterLayout;
	static FTSimpleLayout* g_twilioLayout;
	static FTSimpleLayout* g_databaseLayout;
	
	static FTFont* g_twitterFont;
	static FTFont* g_twilioFont;
	static FTFont* g_databaseFont;
	
	static std::vector<ofImage> g_originalImages;
	static std::vector<ofImage> g_leafImages;
	static std::vector<ofImage> g_backgroundImages;
	
	static bool g_changeBranchImages;
	
	static ofRectangle g_scene;
	static ofRectangle g_sceneBounded;
	
	static float g_soundVolume;
	
	static bool g_useInteractiveAudio;
	
	static bool g_useBackgroundImage;
	static std::string g_backgroundImage;
	static std::string g_soundtrack;
	
	static float g_minColonization;
	static float g_minColonizationForLeaves;
	
	static bool g_treesCanFade;
	
	static float g_fps;
	static float g_currentFps;
	
	static int g_numberOfseeds;
	
	
	static float g_treesLayerOpacity;
	static float g_twitterLayerOpacity;
	static float g_flowersOpacity;
	
	static ofImage g_flowerImage;
	
	static bool g_showFlowers;
	
	static FluidSolver *s_solver;
	static Vec2f s_windowSize;
	static Vec2f s_invWindowSize;
	
	static LeavesLayer* g_leavesLayer;
	
	static float g_leavesFertility;
	static ofColor g_leavesEndColor;
	static float g_leavesOpacity;
	static float g_leavesLife;
	static float g_leavesMinWidth;
	static float g_leavesMaxWidth;
	static float g_leavesMinFreq;
	static float g_leavesMaxFreq;
	
	static ofColor g_flowersColor;
	static ofColor g_flowersLineColor;
	
	static bool g_useTwilio;
	static bool g_useTwitter;
	
	
	static int g_leavesRows;
	static int g_leavesColumns;
	
	static bool g_leavesActive;
	
	static int g_treesFadeTime;
	
	static int g_backgroundTransitionTime;
	
	static bool g_useArchive;
	
	static std::atomic<int> g_activeThreads;
	//static ofMutex g_activeThreadsMutex;
	
	static float g_waitSeedTime; // wait in seed mode
	static float g_waitLinesTime; // tree full, wait to start lines
	static float g_startDetachingLeavesTime;
	static float g_waitRegenerateTime; // after leaves fell how long does it take to regenerate
	
	
	
	
	static int g_msgOpacity;
	
	static float g_linesMin;
	static float g_linesMax;
	
	static float g_linesMinNorm;
	static float g_linesMaxNorm;
	static float g_linesMinAcc;
	static float g_linesMaxAcc;
	
	static int g_firstIterations;
	
	/////////////////////////////////////////////////////////////////////////
	
	static void g_initializeFonts();
	static void g_initializeBranchImages();
	static void g_initializeLeafImages();
	static void g_initializeBackgroundImages();
	
	static void g_setTwitterMsgFontSize(int fontsize);
	static void g_setTwilioMsgFontSize(int fontsize);
	static void g_setDatabaseMsgFontSize(int fontsize);
	
	static void g_colorizeSplashImages();
	static void g_computeMaxBranches();
	static void g_updateBackground();
	static void g_activateSoundtrack();
	
};
#endif //

