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

#include "ofApp.h"


#include "Settings.h"
#include "GraphicUtilities.hpp"
#include "XTree.h"
#include "XTreeBranch.h"
#include "TwitterTrigger.h"

#include "guiCustomColorPreview.h"
#include "guiCustomGradientPreview.h"
#include "guiTypeConfigFileList.h"

#include "RealtimeFetcher.h"

#define MIN_X 150
#define MIN_Y 210

//--------------------------------------------------------------

float vSavedLinesSpeedMin;
float vSavedLinesSpeedMax;

ofApp::ofApp() :
	m_state(STATE_SETUP){
}


void ofApp::setup(){
	m_screenshot = false;
	globalSettings::g_activeThreads = 0;
	globalSettings::g_app = this;
	globalSettings::g_fps = 30;
	m_simulationState = TIME_GROWING;
	m_simulationTimer = 0;
	m_numberOfIterations = 0;
	m_clearFBO = false;

//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableSmoothing();
	ofSetFrameRate(globalSettings::g_fps);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetCircleResolution(100);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ofBackground(0);
	ofSetVerticalSync(false);


	m_internetOk = true;
	m_splashActive = false;

	if(!CheckInternetConnection()){
		m_internetOk = false;
		return;
	}

	XTree::initGhostFbo();
	
	// needs to be called after the settings have been loaded!
	globalSettings::g_initializeBranchImages();
//	globalSettings::g_initializeFonts();
	globalSettings::g_initializeLeafImages();
	globalSettings::g_initializeBackgroundImages();
	TwitterBaloon::initFbo();

	m_backgroundFadeTime = 1;
	
	// Twilio is enabled by default, disable it for now
	globalSettings::g_useTwilio = false;
	
	//disable twitter
//	globalSettings::g_useTwitter = false;
	
	
//	msg = ofToDataPath("fonts/ArialUnicode.ttf");
//	stringstream ss;
//	ss << msg << " exists: " << boolalpha << ofFile::doesFileExist(msg);
//
	
	
	// Archive is enabled if the file achive.txt is present
	globalSettings::g_useArchive =  ofFile::doesFileExist(ofToDataPath("archive.txt"));
	
	
//	cout << "globalSettings::g_useArchive "<< boolalpha << globalSettings::g_useArchive<< endl;

//	msg = ss.str();
	
	m_leavesLayer = make_shared<LeavesLayer>();
	  globalSettings::g_leavesLayer = m_leavesLayer.get();
	  m_twitterLayer = make_shared<TwitterLayer>();
	  m_treesLayer = make_shared<TreesManager>(m_twitterLayer);
	//  m_flowersLayer = new FlowersLayer(m_treesLayer);
	  m_linesLayer = make_shared<LinesLayer>(m_treesLayer);

	m_goBtn = make_unique<XTreeButton>("Go", 80, 50, 30);
	m_resetBtn= make_unique<XTreeButton>("Reset", 80, 150, 30);
	


	globalSettings::g_useInteractiveAudio = true;

	if(globalSettings::g_useInteractiveAudio){
		InteractiveAudio::instance()->init(this);
		InteractiveAudio::instance()->start();
	}


	setupControlPanel();
	controlPanel.loadSettings(ofToDataPath("configurations/default.xml"));
	setupControlPanelVariables();

	
	
	
	
	
	// init Ghost Fbo
	

	if(use_background){
		int size = globalSettings::g_backgroundImages.size();
		if(size == 0){
			use_background = false;
		}
		else{
			m_back1 = kplToss(globalSettings::g_backgroundImages.size());
			m_back2 = kplToss(globalSettings::g_backgroundImages.size());
		}

	}

	ofAddListener(ofEvents().mouseMoved, this, &ofApp::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &ofApp::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofApp::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &ofApp::keyPressed);

	ofAddListener(m_goBtn->clickedEvent, this,  &ofApp::goClicked);
	ofAddListener(m_resetBtn->clickedEvent, this,  &ofApp::resetClicked);

	/* signal the soundsystem how many branches will grow at the same time
	// to avoid clipping and decide the mix parameters
	if (globalSettings::g_useInteractiveAudio) {
	  InteractiveAudio::instance()->sendFloat(kBranchPolyphony,
	    ofClamp(ceil((float)globalSettings::g_growthMax / (float)globalSettings::g_growthMin), 1, 4));
	  InteractiveAudio::instance()->sendFloat(kActivateBirds, 0);
	  InteractiveAudio::instance()->sendFloat(kActivateVinyl, 0);
	  InteractiveAudio::instance()->sendFloat(kActivateRand1, 0);
	}
	*/

	InteractiveAudio::instance()->sendBang(kStart);

//	m_fadeRectangle.init(ofGetWidth(), ofGetHeight() / 7);
//	// to set the color of the fading rectangle
//	globalSettings::g_updateBackground();
//
//	globalSettings::g_scene = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
//	globalSettings::g_sceneBounded = ofRectangle(20, 0, ofGetWidth() - 20, ofGetHeight());
	m_layerObfuscationColor = ofColor(0);

	m_splash.loadImage(ofToDataPath("splash/splash.png"));

//	m_splashPosition.x = std::max(ofGetWidth() / 2 - m_splash.getWidth() / 2, m_splash.getWidth() / 2);
//	m_splashPosition.y = ofGetHeight() / 2 - m_splash.getHeight() / 2;

	setFromWindowSize();
	

	m_goBtn->showImmediately();
	m_resetBtn->showImmediately();

	// save seeds
	m_treesLayer->loadSeeds();
}
//--------------------------------------------------------------
void ofApp::setFromWindowSize()
{
	XTree::initGhostFbo();
	
	m_fadeRectangle.init(ofGetWidth(), ofGetHeight() / 7);
	// to set the color of the fading rectangle
	globalSettings::g_updateBackground();

	globalSettings::g_scene = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	globalSettings::g_sceneBounded = ofRectangle(20, 0, ofGetWidth() - 20, ofGetHeight());
	
	m_splashPosition.x = std::max(ofGetWidth() / 2 - m_splash.getWidth() / 2, m_splash.getWidth() / 2);
	m_splashPosition.y = ofGetHeight() / 2 - m_splash.getHeight() / 2;

	
	globalSettings::s_windowSize = Vec2f(ofGetWidth(),ofGetHeight());
	globalSettings::s_invWindowSize = Vec2f( 1.0f / globalSettings::s_windowSize.x, 1.0f / globalSettings::s_windowSize.y );
	
	setRenderSize(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
ofApp::~ofApp(){
	
	
	
	
	if(!m_internetOk){
		return;
	}
	// save seeds
	m_treesLayer->saveSeeds();

	if(!m_internetOk){
		return;
	}
	
	if(m_state == STATE_SIMULATION){
		RealtimeFetcher::instance()->stop();
	}
	
	

}

void ofApp::start(){
	if(!m_internetOk){
		return;
	}

	if(!m_treesLayer->start()){
		return;
	}
	globalSettings::g_globalCounterSec = 1;

	ofHideCursor();

	m_simulationState = TIME_GROWING;
	m_splashActive = false;
	m_numberOfIterations = 0;

	RealtimeFetcher::instance()->start();

	InteractiveAudio::instance()->sendBang(kBreath);

	InteractiveAudio::instance()->sendBang(kSimulOn);

	//ofHideCursor();
	m_goBtn->hide();
	m_resetBtn->hide();
	if(globalSettings::g_showFlowers){
		//m_flowersLayer->start();
		//m_linesLayer->start();
	}
	//if (globalSettings::g_leavesActive)
	//m_leavesLayer->start();
	//        m_twitterLayer->prestart();
	m_state = STATE_SIMULATION;

	if(!globalSettings::g_useInteractiveAudio){
		//soundtrack.play();
	}
}

void ofApp::stop(){
	
	m_clearFBO = true;
	
	if(!m_internetOk){
		return;
	}

	ofShowCursor();

	RealtimeFetcher::instance()->stop();
	InteractiveAudio::instance()->sendBang(kSimulOff);

	//m_treesLayer->stopGrowing();
	m_treesLayer->stop();

	m_linesLayer->stop();
	m_twitterLayer->stop();
	m_leavesLayer->reset();

	m_goBtn->show();
	m_resetBtn->show();
	
	m_state = STATE_SETUP;
	if(!globalSettings::g_useInteractiveAudio){
		soundtrack.stop();
	}
	m_splashActive = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs & args){
	

	
	
	if(args.key == '/'){
		ofToggleFullscreen();
	}
	else
	if(args.key == 'i'){
		bDrawStatesInfo ^= true;
	}
	else
	if(args.key == 'c'){
		m_clearFBO = true;
	}
	
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}
	if(!m_internetOk){
		return;
	}
	if(args.key == ','){
		if(bDebug){
			if(m_state == STATE_SETUP){
				m_goBtn->showImmediately();
				m_resetBtn->showImmediately();
			}
			else{
				ofHideCursor();
			}
		}
		else{
			if(m_state == STATE_SETUP){
				m_goBtn->hideImmediately();
				m_resetBtn->hideImmediately();
			}
			else{
				ofShowCursor();
			}
		}
		bDebug = !bDebug;
		return;
	}
	switch(m_state){
	 case STATE_SETUP:
		 if(args.key == '.'){
			 start();
		 }
		 else{
			 m_treesLayer->keyPressed(args);
		 }

		 break;

	 case STATE_SIMULATION:
		 if(args.key == ' '){
			 m_maxThreads = globalSettings::g_activeThreads;
			 stop();
		 }
		 else if(args.key == 's'){
			 m_screenshot = m_screenshot ? false : true;
		 }

	 default:
		 break;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(m_screenshot){
		return;
	}
	if(!m_internetOk){
		return;
	}
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}

	if(m_state == STATE_SIMULATION){
		RealtimeFetcher::instance()->update();
	}
	m_twitterLayer->update();
	m_treesLayer->update();
	m_linesLayer->update();
	m_leavesLayer->update();
	if(use_background){
		++m_backgroundFadeTime;
		if(m_backgroundFadeTime > globalSettings::g_backgroundTransitionTime * globalSettings::g_fps){
			m_backgroundFadeTime = 1;
			m_back1 = m_back2;
			m_back2 = kplToss(globalSettings::g_backgroundImages.size());
		}
	}

	// reduce sampling rate for other operations: every 3 seconds
	if(m_state == STATE_SIMULATION && globalSettings::g_globalCounter % (int)globalSettings::g_fps == 0){
		globalSettings::g_globalCounterSec++;
		globalSettings::g_globalCounter = 0;
		updateSimulationState();
	}

	++globalSettings::g_globalCounter;
}

/*!

 Simulation state is updated every second!

*/

void ofApp::updateSimulationState(){
	
	if(m_treesLayer->averageMaturationLevel() > globalSettings::g_minColonization){
		RealtimeFetcher::instance()->pause();
		InteractiveAudio::instance()->sendBang(kMsgOff);
		m_treesLayer->stopGrowing();
		ofLog() << "REACHED MIN COLO";
	}
	
	if(m_simulationState == TIME_GROWING){
		
		if(m_numberOfIterations < globalSettings::g_firstIterations){
			if(m_treesLayer->averageMaturationLevel() > globalSettings::g_minColonization){
				ofLog() << "TIME_GROWING -> TIME_LEAVESFALLING";
				m_simulationTimer = 0;
				m_simulationState = TIME_LEAVESFALLING;
			}
		} else{
			// it reached the last iteration
			
			float maxMinColonization = MIN(globalSettings::g_minColonizationForLeaves, globalSettings::g_minColonization);
			
			if(m_treesLayer->averageMaturationLevel() > maxMinColonization){
				m_simulationState = TIME_LEAVES;
				ofLog() << " moving to TIME_LEAVES state";
				InteractiveAudio::instance()->sendBang(kBreath);
				InteractiveAudio::instance()->sendBang(kLeavesOn);
				m_leavesLayer->start();
			}
		
		}
	}
	else if(m_simulationState == TIME_LEAVES){
		if(m_leavesLayer->numberOfLeaves() >= globalSettings::g_leavesRows * globalSettings::g_leavesColumns){
			m_leavesLayer->stop();
			m_simulationTimer = 0;
			m_simulationState = TIME_LINES_TRANS;
			ofLog() << "Moving to TIME_LINES_TRANS state..";
		}
	}
	else if(m_simulationState == TIME_LINES_TRANS){
	
		if(m_simulationTimer >= globalSettings::g_waitLinesTime){ // HERE WE USE A TIMER
			ofLog() << "Moving to TIME_LINES state..";
			globalSettings::g_linesMin = (float)globalSettings::g_linesMinNorm;
			globalSettings::g_linesMax = (float)globalSettings::g_linesMaxNorm;
			InteractiveAudio::instance()->sendBang(kLinesOn);
			m_linesLayer->start();
			m_simulationTimer = 0;
			m_simulationState = TIME_LINES;
		}
	}
	else if(m_simulationState == TIME_LINES){
		if(m_simulationTimer >= globalSettings::g_startDetachingLeavesTime){ // HERE WE USE A TIMER
			m_leavesLayer->setWindForce(0);
			m_leavesLayer->detachLeaves();
			InteractiveAudio::instance()->sendBang(kLeavesOff);
			globalSettings::g_linesMin = (float)globalSettings::g_linesMinAcc;
			globalSettings::g_linesMax = (float)globalSettings::g_linesMaxAcc;
			m_simulationState = TIME_LEAVESFALLING;
			ofLog() << "Moving to TIME_LEAVESFALLING";
			// NB: do not reset the counter
		}
	}
	else if(m_simulationState == TIME_LEAVESFALLING){
		
		if(m_leavesLayer->numberOfLeaves() == 0 && m_twitterLayer->numberOfBaloons() == 0){ // 12 seconds
			m_linesLayer->stop();
			if(m_numberOfIterations >= globalSettings::g_firstIterations){
				InteractiveAudio::instance()->sendBang(kLinesOff);
			}
			else{
				InteractiveAudio::instance()->sendBang(kReadyToRegenerate);
			}
			m_simulationTimer = 0;
			m_simulationState = TIME_REGENERATE;
		}
	}
	else if(m_simulationState == TIME_REGENERATE){
		ofLog() << "TIME_REGENERATE!";
		if(m_simulationTimer >= globalSettings::g_waitRegenerateTime){
			m_simulationTimer = 0;
			if(m_numberOfIterations >= globalSettings::g_firstIterations){
				m_treesLayer->fadeGhostFbo();
				m_numberOfIterations = 0;
			}
			else{
				m_treesLayer->showGhostFbo();
				++m_numberOfIterations;
			}
			
			m_treesLayer->m_ShuffleColors = true;
			m_treesLayer->regenerate();
			InteractiveAudio::instance()->sendBang(kRegen);

			ofLog() << "TIME_GROWING_TRANS";
			m_simulationState = TIME_GROWING_TRANS;
		}
	}
	else if(m_simulationState == TIME_GROWING_TRANS){
		if(m_simulationTimer == globalSettings::g_waitSeedTime - 3){
			InteractiveAudio::instance()->sendBang(kBreath);
		}
		if(m_simulationTimer >= globalSettings::g_waitSeedTime){
			m_treesLayer->restartGrowing();
			RealtimeFetcher::instance()->restart();
			InteractiveAudio::instance()->sendBang(kMsgOn);
			m_simulationState = TIME_GROWING;
		}
	}
	++m_simulationTimer;

}

void ofApp::mousePressed(ofMouseEventArgs & args){
	if(!m_internetOk || bDebug){
		return;
	}
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}
	Leaf * nleaf;
	switch(m_state){
	 case STATE_SETUP:
		 if(!(args.x < MIN_X && args.y < MIN_Y)){
			 m_treesLayer->mousePressed(args);
		 }
		 break;

	 default:
		 break;
	}
}

void ofApp::mouseMoved(ofMouseEventArgs & args){
	if(!m_internetOk || bDebug){
		return;
	}
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}
	switch(m_state){
	 case STATE_SETUP:
		 if(!(args.x < MIN_X && args.y < MIN_Y)){
			 m_treesLayer->mouseMoved(args);
		 }
		 break;

	 default:
		 break;
	}
}


void ofApp::mouseDragged(ofMouseEventArgs & args){
	if(!m_internetOk || bDebug){
		return;
	}
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}
	switch(m_state){
	 case STATE_SETUP:
		 if(!(args.x < MIN_X && args.y < MIN_Y)){
			 m_treesLayer->mouseDragged(args);
		 }
		 break;

	 default:
		 break;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(m_clearFBO) {
		XTree::clearFbo();
		m_clearFBO = false;
	}
	
	if(m_state == STATE_SETUP && globalSettings::g_activeThreads > 0){
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Resetting: " + ofToString(100.f - ((float)globalSettings::g_activeThreads / m_maxThreads) * 100.F) + " %", ofGetWidth() / 2 - 200, ofGetHeight() / 2);
		return;
	}
	if(!m_internetOk){
		ofDrawBitmapString("This program requires an active internet connection,", ofGetWidth() / 2 - 200, ofGetHeight() / 2);
		ofDrawBitmapString("please check your configuration!", ofGetWidth() / 2 - 200, ofGetHeight() / 2 + 30);
		return;
	}
	

	
	pushTransforms();

	ofEnableAlphaBlending();
	m_treesLayer->draw();
	m_linesLayer->draw();
	m_leavesLayer->draw();
	m_twitterLayer->draw();
	//m_fadeRectangle.draw();

	if(m_splashActive){
		ofSetColor(globalSettings::g_backgroundC, m_splashOpacity * globalSettings::g_splashOpacity);
		ofRect(0, 0, globalSettings::g_scene.width, globalSettings::g_scene.height);
		ofSetColor(255);
		ofSetColor(255, 255.0F * m_splashOpacity);
		m_splash.draw(m_splashPosition.x, m_splashPosition.y);
	}

	popTransforms();

	ofSetLineWidth(1);

	if(bDrawStatesInfo){
		stringstream ss;
		
//		ss << "Draw Trees Layer   : " << boolalpha << _bDrawTreesLayer << "\n";
//		ss << "Draw Lines Layer   : " << boolalpha << _bDrawLinesLayer << "\n";
//		ss << "Draw Leaves Layer  : " << boolalpha << _bDrawLeavesLayer << "\n";
//		ss << "Draw Twitter Layer : " << boolalpha << _bDrawTwitterLayer << "\n";
//
		
		
		
	ofDrawBitmapStringHighlight("state : " + toString(m_state)+ "\n" +
								"simulation : " + toString(m_simulationState) +
//								"\n" + ss.str() +
								"\n\n press key 'i' to toggle this message", 200, 50);
	}
	
	
}

void ofApp::goClicked(bool & do_){
	if(!m_internetOk){
		return;
	}
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}
	start();
}

void ofApp::resetClicked(bool & do_){
	
	if(!m_internetOk){
		return;
	}
	
	if(STATE_SETUP && globalSettings::g_activeThreads > 0){
		return;
	}
	
	m_clearFBO = true;
	
	RealtimeFetcher::instance()->reset();
	m_treesLayer->reset();
	m_leavesLayer->reset();
	
}


#pragma mark - Control Panel

void ofApp::setupControlPanel(){


	ofxControlPanel::setBackgroundColor(simpleColor(0, 0, 0, 255));
	ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 255));


	// Color Preview

	globalSettings::g_treeColorPreview = new guiCustomColorPreview();

	globalSettings::g_treeGradStartPreview = new guiCustomColorPreview();
	globalSettings::g_treeGradEndPreview = new guiCustomColorPreview();
	globalSettings::g_treeGradPreview = new guiCustomGradientPreview();

	controlPanel.defaultConfigurationDirectory = ofToDataPath("configurations");

	int panelNr = 0;

	controlPanel.addPanel("General settings", 4, false);
	controlPanel.setWhichPanel(panelNr++);

	/*

	controlPanel.setWhichColumn(0);

	controlPanel.addLabel("Configuration file");
	m_configurationLister.listDir("configurations/");


	ofFile m_initFile(INIT_FILENAME);
	m_currentConfigurationFile = m_initFile.readToBuffer().getText(); // why the hell there's a newline after??
	m_currentConfigurationFile = m_currentConfigurationFile.substr(0, m_currentConfigurationFile.size() - 1);
	m_initFile.close();

	int configIndex = 0;
	for (; configIndex < m_configurationLister.entries.size(); ++configIndex) {
	  if (m_configurationLister.getName(configIndex) == m_currentConfigurationFile)
		break;
	}

	//assert(m_configurationLister.entries.size() && configIndex < m_configurationLister.entries.size());

	guiBaseObject* fileLister = controlPanel.addFileLister("Configuration", &m_configurationLister, 200, 200);
	//m_configurationLister.setSelectedFile(configIndex);
	//fileLister->setValue(configIndex, 0);
	*/

	controlPanel.setWhichColumn(0);
	controlPanel.addLabel("Background");
	controlPanel.addToggle("Use background image", "useBackgroundImage", true);
	controlPanel.addSlider("Background trans. time", "backgroundTransitionTime", 10, 1, 120, false);
	controlPanel.addLabel("Layers control");
	controlPanel.addToggle("Activate flowers", "showFlowers", false);
	controlPanel.addToggle("Activate leaves", "showLeaves", false);
	controlPanel.addSlider("Trees opacity", "treesOpacity", 255, 0, 255, false);
	controlPanel.addSlider("Twitter opacity", "twitterOpacity", 255, 0, 255, false);
	controlPanel.addSlider("Splash opacity", "splashOpacity", 255, 0, 255, false);
	controlPanel.addSlider("Flowers opacity", "flowersOpacity", 255, 0, 255, false);
	controlPanel.addSlider("Ghost img time", "treesFadeTime", 30, 1, 60, false);
	controlPanel.addSlider("Splash frequency", "splashFrequency", 60, 30, 600, false);
	controlPanel.addSlider("Splash duration", "splashDuration", 15, 5, 60, false);



	controlPanel.setWhichColumn(1);
	//controlPanel.addLabel("Audio settings");
	//m_soundtracksLister.listDir("soundtracks/");
	//controlPanel.addFileLister("Soundtrack", &m_soundtracksLister, 200, 200);
	//controlPanel.addToggle("Use interactive audio", "useInteractiveAudio", true);

	controlPanel.addLabel("Loop settings");
	//controlPanel.addSlider("Seed time", "waitSeedTime", 5, 1, 60, false);
	controlPanel.addSlider("First iterations", "firstIterations", 3, 0, 10, true);

	controlPanel.addSlider("Wait in seed mode", "waitSeedTime", 5, 0, 60, false);
	controlPanel.addSlider("Wait for lines time", "waitLinesTime", 5, 0, 60, false);
	controlPanel.addSlider("Detach leaves after time", "startDetachingLeavesTime", 5, 0, 60, false);
	controlPanel.addSlider("Wait after regenerate time", "waitRegenerateTime", 5, 0, 60, false);

	controlPanel.setWhichColumn(2);
	controlPanel.addLabel("Twitter settings");
	/*
	controlPanel.addToggle("Timed show", "timedExhibit", true);
	controlPanel.addSlider("Show duration (min)", "showDuration", 10, 1, 60, true);
	controlPanel.addSlider("Nr. past tweets", "pastTweets", 5, 0, 100, true);
	*/
	controlPanel.addToggle("Show twitter thumbnails", "showThumbs", 1);
	controlPanel.addToggle("Joints are circles", "jointsAreCircles", true);

	controlPanel.addLabel("General tree structure");
	controlPanel.addSlider("Fertility", "fertility", 0, 0, 5, true);
	controlPanel.addSlider("Max levels", "maxLevel", 10, 3, 20, true);
	controlPanel.addSlider("Min colonization to regenerate", "minColonization", 30, 1, 100, true);
	controlPanel.addSlider("Min colonization to create leaves", "minColonizationForLeaves", 20, 1, 100, true);

	controlPanel.addLabel("Leaves");
	controlPanel.addSlider("Leaves rows", "leavesRows", 8, 1, 20, true);
	controlPanel.addSlider("Leaves columns", "leavesColumns", 6, 1, 20, true);
	controlPanel.addSlider("Leaves life", "leavesLife", 100, 1, 200, true);



	//controlPanel.addLabel("Twilio settings");
	//controlPanel.addToggle("Use twilio", "useTwilio", true);

	controlPanel.setWhichColumn(3);
	controlPanel.addLabel("Timing");
	controlPanel.addSlider("Branch min (ms)", "growthMin", 1500, 1000, 10000, true);
	controlPanel.addSlider("Branch max (ms)", "growthMax", 3000, 1000, 10000, true);
	controlPanel.addSlider("Twitter min (ms)", "minFrequency", 3000, 100, 20000, true);
	controlPanel.addSlider("Twitter max (ms)", "maxFrequency", 7000, 100, 20000, true);
	controlPanel.addSlider("Lines min (ms)", "LinesMin", 50, 10, 10000, false);
	controlPanel.addSlider("Lines max (ms)", "LinesMax", 300, 10, 10000, false);
	//controlPanel.addSlider("Flowers min (ms)", "flowersMin", 50, 10, 10000, false);
	//controlPanel.addSlider("Flowers max (ms)", "flowersMax", 300, 10, 10000, false);
//	controlPanel.addSlider("Leaves min (ms)", "leavesMinFreq", 5000, 100, 100000, false);
//	controlPanel.addSlider("Leaves max (ms)", "leavesMaxFreq", 15000, 100, 100000, false);

	controlPanel.addPanel("Messages", 4, false);
	controlPanel.setWhichPanel(panelNr++);
	controlPanel.setWhichColumn(0);
	controlPanel.addSlider("Message opacity", "msgOpacity", 150, 0, 255, false);
	controlPanel.addSlider("Line R", "tLineR", 0, 0, 255, true);
	controlPanel.addSlider("Line G", "tLineG", 0, 0, 255, true);
	controlPanel.addSlider("Line B", "tLineB", 0, 0, 255, true);
	controlPanel.addSlider("Line width", "tLineWidth", 1, 1, 10, true);
	controlPanel.addSlider("Floating speed", "floatingSpeed", 20, 1, 100, true);
	controlPanel.addSlider("Min life (ms)", "tweetMin", 5000, 1000, 20000, false);
	controlPanel.addSlider("Max life (ms)", "tweetMax", 10000, 1000, 20000, false);


	controlPanel.setWhichColumn(1);
	controlPanel.addLabel("Twitter messages");
	controlPanel.addSlider("Message size (px)", "tweetMsgSize", 10, 6, 20, true);
	controlPanel.addSlider("Message R", "tweetMsgR", 255, 0, 255, true);
	controlPanel.addSlider("Message G", "tweetMsgG", 255, 0, 255, true);
	controlPanel.addSlider("Message B", "tweetMsgB", 255, 0, 255, true);
	controlPanel.addSlider("User R", "tweetUsR", 255, 0, 255, true);
	controlPanel.addSlider("User G", "tweetUsG", 255, 0, 255, true);
	controlPanel.addSlider("User B", "tweetUsB", 255, 0, 255, true);

	controlPanel.setWhichColumn(2);
	controlPanel.addLabel("Twilio messages");
	controlPanel.addSlider("Message size (px)", "twilioMsgSize", 10, 6, 20, true);
	controlPanel.addSlider("Message R", "twilioMsgR", 255, 0, 255, true);
	controlPanel.addSlider("Message G", "twilioMsgG", 255, 0, 255, true);
	controlPanel.addSlider("Message B", "twilioMsgB", 255, 0, 255, true);
	controlPanel.addSlider("User R", "twilioUsR", 255, 0, 255, true);
	controlPanel.addSlider("User G", "twilioUsG", 255, 0, 255, true);
	controlPanel.addSlider("User B", "twilioUsB", 255, 0, 255, true);

	controlPanel.setWhichColumn(3);
	controlPanel.addLabel("Database messages");
	controlPanel.addSlider("Message size (px)", "databaseMsgSize", 10, 6, 20, true);
	controlPanel.addSlider("Message R", "databaseMsgR", 255, 0, 255, true);
	controlPanel.addSlider("Message G", "databaseMsgG", 255, 0, 255, true);
	controlPanel.addSlider("Message B", "databaseMsgB", 255, 0, 255, true);
	controlPanel.addSlider("User R", "databaseUsR", 255, 0, 255, true);
	controlPanel.addSlider("User G", "databaseUsG", 255, 0, 255, true);
	controlPanel.addSlider("User B", "databaseUsB", 255, 0, 255, true);


	controlPanel.addPanel("Other", 4, false);
	controlPanel.setWhichPanel(panelNr++);
	controlPanel.setWhichColumn(0);
	controlPanel.addLabel("Background color");
	controlPanel.addSlider("Background R", "backgroundR", 0, 0, 255, true);
	controlPanel.addSlider("Background G", "backgroundG", 0, 0, 255, true);
	controlPanel.addSlider("Background B", "backgroundB", 0, 0, 255, true);
	controlPanel.addLabel("Splash screen");
	controlPanel.addSlider("Splash Screen R", "splashR", 255, 0, 255, true);
	controlPanel.addSlider("Splash Screen G", "splashG", 255, 0, 255, true);
	controlPanel.addSlider("Splash Screen B", "splashB", 255, 0, 255, true);

	controlPanel.setWhichColumn(1);
	controlPanel.addLabel("Branches");
	controlPanel.addCustomRect("Branch Color", globalSettings::g_treeColorPreview, 60, 60);
	controlPanel.addSlider("Branch R", "treeR", 255, 0, 255, true);
	controlPanel.addSlider("Branch G", "treeG", 255, 0, 255, true);
	controlPanel.addSlider("Branch B", "treeB", 255, 0, 255, true);
	controlPanel.addSlider("Branch A", "treeA", 255, 0, 255, true);
	controlPanel.addSlider("Branch width (px)", "branchWidth", 1, 1, 200, true);
	controlPanel.addSlider("Branch min width (px)", "branchMinWidth", 1, 1, 200, true);
	controlPanel.addSlider("Width decrease (%)", "widthDecrease", 1, 1, 100, true);
	controlPanel.addSlider("Branch length (px)", "branchLength", 100, 50, 500, true);
	controlPanel.addSlider("Length decrease (%)", "lengthDecrease", 1, 1, 100, true);
	controlPanel.addSlider("Min angle (deg.)", "minAngle", 1, 1, 90, false);
	controlPanel.addSlider("Max angle (deg)", "maxAngle", 90, 1, 90, false);

	controlPanel.setWhichColumn(2);
	controlPanel.addLabel("Branch joints");
	controlPanel.addSlider("Joint R", "jointR", 255, 0, 255, true);
	controlPanel.addSlider("Joint G", "jointG", 0, 0, 255, true);
	controlPanel.addSlider("Joint B", "jointB", 0, 0, 255, true);
	controlPanel.addSlider("Joint A", "jointA", 0, 0, 255, true);
	controlPanel.addSlider("Joint thickness", "jointThickness", 1, 1, 10, true);
	controlPanel.addSlider("Joint length", "jointLength", 20, 0, 80, true);
	controlPanel.addLabel("Flowers");
	controlPanel.addSlider("Line Width", "crazyLineWidth", 2, 1, 10, true);
	controlPanel.addSlider("Circle Width", "crazyCircleWidth", 15, 2, 50, true);
	controlPanel.addSlider("Tree Lines R", "tFlowersLinesR", 255, 0, 255, true);
	controlPanel.addSlider("Tree Lines G", "tFlowersLinesG", 255, 0, 255, true);
	controlPanel.addSlider("Tree Lines B", "tFlowersLinesB", 255, 0, 255, true);

//	controlPanel.setWhichColumn(3);
//	controlPanel.addLabel("Leaves");
//	controlPanel.addSlider("Leaf R", "tLeavesR", 0, 0, 255, true);
//	controlPanel.addSlider("Leaf G", "tLeavesG", 0, 0, 255, true);
//	controlPanel.addSlider("Leaf B", "tLeavesB", 0, 0, 255, true);
//	controlPanel.addSlider("Leaf Opacity", "tLeavesA", 255, 0, 255, true);
//	controlPanel.addSlider("Leaf fertility", "leafFertility", 1, 1, 10, true);
//	controlPanel.addSlider("Leaf min width", "leafMinWidth", 100, 5, 300, true);
//	controlPanel.addSlider("Leaf max width", "leafMaxWidth", 140, 5, 300, true);

	controlPanel.addPanel("Audio Setup", 4, false);
	controlPanel.setWhichPanel(panelNr++);
	controlPanel.setWhichColumn(1);
	controlPanel.addSlider("Global Volume", "soundVolume", 30, 0, 100, false);
	// controlPanel.addLabel("Parametric Equalizer");
	// controlPanel.addSlider("eq100", "eq100", 30, 0, 100, true);
	// controlPanel.addSlider("eq250", "eq250", 44, 0, 100, true);
	// controlPanel.addSlider("eq700", "eq700", 50, 0, 100, true);
	// controlPanel.addSlider("eq1500", "eq1500", 50, 0, 100, true);
	// controlPanel.addSlider("eq3000", "eq3000", 50, 0, 100, true);
	// controlPanel.addSlider("eq7000", "eq7000", 50, 0, 100, true);
	// controlPanel.addSlider("eq15000", "eq15000", 50, 0, 100, true);
	// controlPanel.addSlider("eqGain", "eqGain", 50, 0, 100, true);

	// Gradient
	controlPanel.addPanel("Color Manager", 4, false);
	controlPanel.setWhichPanel(panelNr++);
	controlPanel.setWhichColumn(0);

	controlPanel.addCustomRect("Gradient Start", globalSettings::g_treeGradStartPreview, 60, 60);
	controlPanel.addSlider("Grad Start Hue", "gradStartR", 0, 0, 360, true);
	controlPanel.addSlider("Grad Start Sat", "gradStartG", 100, 0, 255, true);
	controlPanel.addSlider("Grad Start Val", "gradStartB", 100, 0, 255, true);
	controlPanel.addSlider("Grad Start Alpha", "gradStartA", 255, 0, 255, true);

	controlPanel.addCustomRect("Gradient End", globalSettings::g_treeGradEndPreview, 60, 60);
	controlPanel.addSlider("Grad End Hue", "gradEndR", 255, 0, 360, true);
	controlPanel.addSlider("Grad End Sat", "gradEndG", 255, 0, 255, true);
	controlPanel.addSlider("Grad End Val", "gradEndB", 255, 0, 255, true);
	controlPanel.addSlider("Grad End Alpha", "gradEndA", 255, 0, 255, true);
	controlPanel.addCustomRect("Preview", globalSettings::g_treeGradPreview, 200, 60);
	controlPanel.addSlider("Ghost Alpha", "ghostAlpha", 255, 0, 255, true);

	// warp control
	controlPanel.addPanel("Transformer", 1, 0);
	setRenderSize(ofGetWidth(), ofGetHeight());
	setTransforms(true, true, false, true, true);

	addTransformControls(panelNr, 0);

	controlPanel.setupEvents();
	controlPanel.enableEvents();

	ofAddListener(controlPanel.guiEvent, this, &ofApp::controlChanged);

}

void ofApp::setupControlPanelVariables(){
	globalSettings::g_treeGradStart.setHsb(controlPanel.getValueI("gradStartR"), controlPanel.getValueI("gradStartG"), controlPanel.getValueI("gradStartB"), controlPanel.getValueI("gradStartA"));
	globalSettings::g_treeGradStart.setHueAngle(controlPanel.getValueI("gradStartR"));

	globalSettings::g_treeGradEnd.setHsb(controlPanel.getValueI("gradEndR"), controlPanel.getValueI("gradEndG"), controlPanel.getValueI("gradEndB"), controlPanel.getValueI("gradEndA"));
	globalSettings::g_treeGradEnd.setHueAngle(controlPanel.getValueI("gradEndR"));

	globalSettings::g_treeGradStartPreview->color = globalSettings::g_treeGradStart;
	globalSettings::g_treeGradEndPreview->color = globalSettings::g_treeGradEnd;

	globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
	globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
}

void ofApp::controlChanged(guiCallbackData & data){
	if(data.isElement("backgroundR")){
		globalSettings::g_backgroundC.r = data.getInt(0);
		globalSettings::g_updateBackground();
	}
	else if(data.isElement("backgroundG")){
		globalSettings::g_backgroundC.g = data.getInt(0);
		globalSettings::g_updateBackground();
	}
	else if(data.isElement("backgroundB")){
		globalSettings::g_backgroundC.b = data.getInt(0);
		globalSettings::g_updateBackground();
	}
	else if(data.isElement("treeR")){
		globalSettings::g_treeC.r = data.getInt(0);
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeColorPreview->color = globalSettings::g_treeC;
	}
	else if(data.isElement("treeG")){
		globalSettings::g_treeC.g = data.getInt(0);
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeColorPreview->color = globalSettings::g_treeC;
	}
	else if(data.isElement("treeB")){
		globalSettings::g_treeC.b = data.getInt(0);
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeColorPreview->color = globalSettings::g_treeC;
	}
	else if(data.isElement("treeA")){
		globalSettings::g_treeC.a = data.getInt(0);
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeColorPreview->color = globalSettings::g_treeC;
	}
	else if(data.isElement("jointsAreCircles")){
		globalSettings::g_jointsAreCircles = data.getInt(0);
	}
	else if(data.isElement("jointR")){
		globalSettings::g_jointC.r = data.getInt(0);
	}
	else if(data.isElement("jointG")){
		globalSettings::g_jointC.g = data.getInt(0);
	}
	else if(data.isElement("jointB")){
		globalSettings::g_jointC.b = data.getInt(0);
	}
	else if(data.isElement("jointA")){
		globalSettings::g_jointC.a = data.getInt(0);
	}
	else if(data.isElement("branchWidth")){
		globalSettings::g_branchWidth = data.getFloat(0);
	}
	else if(data.isElement("branchMinWidth")){
		globalSettings::g_branchMinWidth = data.getFloat(0);
	}
	else if(data.isElement("branchLength")){
		globalSettings::g_branchLength = data.getFloat(0);
	}
	else if(data.isElement("widthDecrease")){
		globalSettings::g_widthDecrease = data.getFloat(0) / 100.F;
	}
	else if(data.isElement("lengthDecrease")){
		globalSettings::g_lengthDecrease = data.getFloat(0) / 100.F;
	}
	else if(data.isElement("pastTweets")){
		globalSettings::g_pastTweets = data.getInt(0);
	}
	else if(data.isElement("jointThickness")){
		globalSettings::g_jointThickness = data.getInt(0);
	}
	else if(data.isElement("jointLength")){
		globalSettings::g_jointLength = data.getInt(0);
	}
	else if(data.isElement("tweetMsgR")){
		globalSettings::g_tweetMsgC.r = data.getInt(0);
	}
	else if(data.isElement("tweetMsgG")){
		globalSettings::g_tweetMsgC.g = data.getInt(0);
	}
	else if(data.isElement("tweetMsgB")){
		globalSettings::g_tweetMsgC.b = data.getInt(0);
	}
	else if(data.isElement("tweetMsgSize")){
		globalSettings::g_setTwitterMsgFontSize(data.getInt(0));
	}
	else if(data.isElement("tweetUsR")){
		globalSettings::g_tweetUsrC.r = data.getInt(0);
	}
	else if(data.isElement("tweetUsG")){
		globalSettings::g_tweetUsrC.g = data.getInt(0);
	}
	else if(data.isElement("tweetUsB")){
		globalSettings::g_tweetUsrC.b = data.getInt(0);
	}
	else if(data.isElement("twilioMsgR")){
		globalSettings::g_twilioMsgC.r = data.getInt(0);
	}
	else if(data.isElement("twilioMsgG")){
		globalSettings::g_twilioMsgC.g = data.getInt(0);
	}
	else if(data.isElement("twilioMsgB")){
		globalSettings::g_twilioMsgC.b = data.getInt(0);
	}
	else if(data.isElement("twilioMsgSize")){
		globalSettings::g_setTwilioMsgFontSize(data.getInt(0));
	}
	else if(data.isElement("twilioUsR")){
		globalSettings::g_twilioUsrC.r = data.getInt(0);
	}
	else if(data.isElement("twilioUsG")){
		globalSettings::g_twilioUsrC.g = data.getInt(0);
	}
	else if(data.isElement("twilioUsB")){
		globalSettings::g_twilioUsrC.b = data.getInt(0);
	}
	else if(data.isElement("databaseMsgR")){
		globalSettings::g_databaseMsgC.r = data.getInt(0);
	}
	else if(data.isElement("databaseMsgG")){
		globalSettings::g_databaseMsgC.g = data.getInt(0);
	}
	else if(data.isElement("databaseMsgB")){
		globalSettings::g_databaseMsgC.b = data.getInt(0);
	}
	else if(data.isElement("databaseMsgSize")){
		globalSettings::g_setDatabaseMsgFontSize(data.getInt(0));
	}
	else if(data.isElement("databaseUsR")){
		globalSettings::g_databaseUsrC.r = data.getInt(0);
	}
	else if(data.isElement("databaseUsG")){
		globalSettings::g_databaseUsrC.g = data.getInt(0);
	}
	else if(data.isElement("databaseUsB")){
		globalSettings::g_databaseUsrC.b = data.getInt(0);
	}
	else if(data.isElement("tLineR")){
		globalSettings::g_tLineC.r = data.getInt(0);
	}
	else if(data.isElement("tLineG")){
		globalSettings::g_tLineC.g = data.getInt(0);
	}
	else if(data.isElement("tLineB")){
		globalSettings::g_tLineC.b = data.getInt(0);
	}
	else if(data.isElement("tLineWidth")){
		globalSettings::g_tLineWidth = data.getInt(0);
	}
	else if(data.isElement("growthMin")){
		globalSettings::g_growthMin = data.getInt(0);
	}
	else if(data.isElement("growthMax")){
		globalSettings::g_growthMax = data.getInt(0);
	}
	else if(data.isElement("timedExhibit")){
		globalSettings::g_timedExhibit = data.getInt(0);
	}
	else if(data.isElement("showDuration")){
		globalSettings::g_showDuration = data.getInt(0);
	}
	else if(data.isElement("minFrequency")){
		globalSettings::g_minFrequency = data.getInt(0);
	}
	else if(data.isElement("maxFrequency")){
		globalSettings::g_maxFrequency = data.getInt(0);
	}
	else if(data.isElement("minAngle")){
		globalSettings::g_minAngle = data.getInt(0);
	}
	else if(data.isElement("maxAngle")){
		globalSettings::g_maxAngle = data.getInt(0);
	}
	else if(data.isElement("showThumbs")){
		globalSettings::g_showThumbs = data.getInt(0);
	}
	else if(data.isElement("fertility")){
		globalSettings::g_fertility = data.getInt(0);
		globalSettings::g_computeMaxBranches();
	}
	else if(data.isElement("maxLevel")){
		globalSettings::g_maxLevel = data.getInt(0);
		globalSettings::g_computeMaxBranches();
	}
	else if(data.isElement("floatingSpeed")){
		globalSettings::g_floatingSpeed = data.getFloat(0) / 300.F;
	}
	else if(data.isElement("splashR")){
		globalSettings::g_splashC.r = data.getInt(0);
	}
	else if(data.isElement("splashG")){
		globalSettings::g_splashC.g = data.getInt(0);
	}
	else if(data.isElement("splashB")){
		globalSettings::g_splashC.b = data.getInt(0);
	}
	else if(data.isElement("soundVolume")){
		globalSettings::g_soundVolume = data.getFloat(0) / 100.F;
	}
	else if(data.isElement("flowersMin")){
		globalSettings::g_flowersMin = data.getInt(0);
	}
	else if(data.isElement("flowersMax")){
		globalSettings::g_flowersMax = data.getInt(0);
	}
	else if(data.isElement("crazyLineWidth")){
		globalSettings::g_crazyLineWidth = data.getInt(0);
	}
	else if(data.isElement("crazyCircleWidth")){
		globalSettings::g_crazyCircleWidth = data.getInt(0);
	}
	else if(data.isElement("useBackgroundImage")){
		use_background = data.getInt(0);
	} /* else if (data.isElement("useInteractiveAudio")) {
	globalSettings::g_useInteractiveAudio = data.getInt(0);
	globalSettings::g_activateSoundtrack();
	}*/
	else if(data.isElement("Soundtrack")){
		globalSettings::g_soundtrack = data.getString(2);
		globalSettings::g_activateSoundtrack();
	}
	else if(data.isElement("minColonization")){
		globalSettings::g_minColonization = data.getFloat(0);
	}
	else if(data.isElement("minColonizationForLeaves")){
		globalSettings::g_minColonizationForLeaves = data.getFloat(0);
	}
	else if(data.isElement("treesOpacity")){
		globalSettings::g_treesLayerOpacity = data.getFloat(0);
	}
	else if(data.isElement("twitterOpacity")){
		globalSettings::g_twitterLayerOpacity = data.getFloat(0);
	}
	else if(data.isElement("splashOpacity")){
		globalSettings::g_splashOpacity = data.getFloat(0);
	}
	else if(data.isElement("flowersOpacity")){
		globalSettings::g_flowersOpacity = data.getFloat(0);
	}
	else if(data.isElement("showFlowers")){
		globalSettings::g_showFlowers = data.getInt(0);
	}
	// else if(data.isElement("tLeavesR")){
	// 	globalSettings::g_leavesEndColor.r = data.getInt(0);
	// }
	// else if(data.isElement("tLeavesG")){
	// 	globalSettings::g_leavesEndColor.g = data.getInt(0);
	// }
	// else if(data.isElement("tLeavesB")){
	// 	globalSettings::g_leavesEndColor.b = data.getInt(0);
	// }
	// else if(data.isElement("tLeavesA")){
	// 	globalSettings::g_leavesOpacity = data.getFloat(0);
	// }
	// else if(data.isElement("leafFertility")){
	// 	globalSettings::g_leavesFertility = data.getFloat(0);
	// }
	// else if(data.isElement("leafMinWidth")){
	// 	globalSettings::g_leavesMinWidth = data.getFloat(0);
	// }
	// else if(data.isElement("leafMaxWidth")){
	// 	globalSettings::g_leavesMaxWidth = data.getFloat(0);
	// }
	else if(data.isElement("tweetMin")){
		globalSettings::g_tweetMinLife = data.getFloat(0);
	}
	else if(data.isElement("tweetMax")){
		globalSettings::g_tweetMaxLife = data.getFloat(0);
	} /* else if (data.isElement("useTwilio")) {
	globalSettings::g_useTwilio = data.getInt(0);
	}*/
	// else if(data.isElement("leavesRows")){
	// 	globalSettings::g_leavesRows = data.getInt(0);
	// }
	// else if(data.isElement("leavesColumns")){
	// 	globalSettings::g_leavesColumns = data.getInt(0);
	// }
	// else if(data.isElement("showLeaves")){
	// 	globalSettings::g_leavesActive = data.getInt(0);
	// }
	// else if(data.isElement("leavesMinFreq")){
	// 	globalSettings::g_leavesMinFreq = data.getInt(0);
	// }
	// else if(data.isElement("leavesMaxFreq")){
	// 	globalSettings::g_leavesMaxFreq = data.getInt(0);
	// }
	else if(data.isElement("treesFadeTime")){
		globalSettings::g_treesFadeTime = data.getInt(0);
	}
	else if(data.isElement("backgroundTransitionTime")){
		globalSettings::g_backgroundTransitionTime = data.getInt(0);
	}

	else if(data.isElement("tFlowersLinesR")){
		globalSettings::g_flowersLineColor.r = data.getInt(0);
	}
	else if(data.isElement("tFlowersLinesG")){
		globalSettings::g_flowersLineColor.g = data.getInt(0);
	}
	else if(data.isElement("tFlowersLinesB")){
		globalSettings::g_flowersLineColor.b = data.getInt(0);
	}
	else if(data.isElement("waitSeedTime")){
		globalSettings::g_waitSeedTime = data.getInt(0);
	}
	else if(data.isElement("waitLinesTime")){
		globalSettings::g_waitLinesTime = data.getInt(0);
	}
	else if(data.isElement("startDetachingLeavesTime")){
		globalSettings::g_startDetachingLeavesTime = data.getInt(0);
	}
	else if(data.isElement("waitRegenerateTime")){
		globalSettings::g_waitRegenerateTime = data.getInt(0);
	}
	// else if(data.isElement("leavesLife")){
	// 	globalSettings::g_leavesLife = data.getInt(0);
	// }
	else if(data.isElement("msgOpacity")){
		globalSettings::g_msgOpacity = data.getInt(0);
	}
	else if(data.isElement("LinesMin")){
		globalSettings::g_linesMin = data.getInt(0);
		globalSettings::g_linesMinNorm = data.getInt(0);
		globalSettings::g_linesMinAcc = globalSettings::g_linesMinNorm * 1.5;
	}
	else if(data.isElement("LinesMax")){
		globalSettings::g_linesMax = data.getInt(0);
		globalSettings::g_linesMaxNorm = data.getInt(0);
		globalSettings::g_linesMaxAcc = globalSettings::g_linesMaxNorm * 1.5;
	}
	else if(data.isElement("firstIterations")){
		globalSettings::g_firstIterations = data.getInt(0);
	}
	else if(data.isElement("splashFrequency")){
		globalSettings::g_splashFrequency = data.getInt(0);
	}
	else if(data.isElement("splashDuration")){
		globalSettings::g_splashDuration = data.getInt(0);
	}
	else if(data.isElement("splashOpacity")){
		globalSettings::g_splashOpacity = data.getFloat(0);
	}

	else if(data.isElement("gradStartR")){
		globalSettings::g_treeGradStart.setHueAngle(data.getInt(0));
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradStartPreview->color = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradStartG")){
		globalSettings::g_treeGradStart.setSaturation(data.getInt(0));
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradStartPreview->color = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradStartB")){
		globalSettings::g_treeGradStart.setBrightness(data.getInt(0));
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradStartPreview->color = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradStartA")){
		globalSettings::g_treeGradStart.a = data.getInt(0);
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradStartPreview->color = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradEndR")){
		globalSettings::g_treeGradEnd.setHueAngle(data.getInt(0));
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradEndPreview->color = globalSettings::g_treeGradEnd;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradEndG")){
		globalSettings::g_treeGradEnd.setSaturation(data.getInt(0));
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradEndPreview->color = globalSettings::g_treeGradEnd;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradEndB")){
		globalSettings::g_treeGradEnd.setBrightness(data.getInt(0));
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradEndPreview->color = globalSettings::g_treeGradEnd;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	}
	else if(data.isElement("gradEndA")){
		globalSettings::g_treeGradEnd.a = data.getInt(0);
		globalSettings::g_changeBranchImages = true;
		globalSettings::g_treeGradEndPreview->color = globalSettings::g_treeGradEnd;
		globalSettings::g_treeGradPreview->colorA = globalSettings::g_treeGradStart;
		globalSettings::g_treeGradPreview->colorB = globalSettings::g_treeGradEnd;
	} else if(data.isElement("ghostAlpha")) {
		globalSettings::g_treeGhostOpacity = data.getInt(0);
	}
}

void ofApp::windowResized(int w, int h){
	
	setFromWindowSize();
	
}

#if USE_PDLIB
//--------------------------------------------------------------
	void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
		InteractiveAudio::instance()->audioReceived(input, bufferSize, nChannels);
	}

//--------------------------------------------------------------
	void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
		InteractiveAudio::instance()->audioRequested(output, bufferSize, nChannels);
		float * thisOut = output;
		int numberOfSamples = bufferSize * nChannels;
		for(int i = 0; i < numberOfSamples; ++i, ++thisOut){
			*thisOut *= globalSettings::g_soundVolume;
		}
	}
#endif



void ofApp::exit()
{
//	cout << "ofApp::exit\n";
	globalSettings::g_deallocateFonts();
}
