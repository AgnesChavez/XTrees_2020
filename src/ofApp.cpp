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
	
	ofTrueTypeFontSettings fontSettings = { ofToDataPath("fonts/verdana.ttf"), 10 };
	
	ofxGuiSetFont(fontSettings);
	
	globalSettings::instance()->setupGuis();
	
	
	
	
	
	
	m_screenshot = false;
	globalSettings::instance()->g_activeThreads = 0;
	globalSettings::instance()->g_app = this;
	globalSettings::instance()->g_fps = 30;
	m_simulationState = TIME_GROWING;
	m_simulationTimer = 0;
	m_numberOfIterations = 0;
	m_clearFBO = false;
	
	//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableSmoothing();
	ofSetFrameRate(globalSettings::instance()->g_fps);
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
	globalSettings::instance()->g_initializeBranchImages();
	globalSettings::instance()->g_initializeLeafImages();
	globalSettings::instance()->g_initializeBackgroundImages();
	TwitterBaloon::initFbo();
	
	m_backgroundFadeTime = 1;
	
	// Twilio is enabled by default, disable it for now
	globalSettings::instance()->g_useTwilio = false;
	
	//disable twitter
	//	globalSettings::instance()->g_useTwitter = false;
	
	
	//	msg = ofToDataPath("fonts/ArialUnicode.ttf");
	//	stringstream ss;
	//	ss << msg << " exists: " << boolalpha << ofFile::doesFileExist(msg);
	//
	
	
	// Archive is enabled if the file achive.txt is present
	globalSettings::instance()->g_useArchive =  ofFile::doesFileExist(ofToDataPath("archive.txt"));
	
	
	//	cout << "globalSettings::instance()->g_useArchive "<< boolalpha << globalSettings::instance()->g_useArchive<< endl;
	
	//	msg = ss.str();
	
	m_leavesLayer = make_shared<LeavesLayer>();
	globalSettings::instance()->g_leavesLayer = m_leavesLayer.get();
	m_twitterLayer = make_shared<TwitterLayer>();
	m_treesLayer = make_shared<TreesManager>(m_twitterLayer);
	//  m_flowersLayer = new FlowersLayer(m_treesLayer);
	m_linesLayer = make_shared<LinesLayer>(m_treesLayer);
	
	m_goBtn = make_unique<XTreeButton>("Go", 80, 50, 30);
	m_resetBtn= make_unique<XTreeButton>("Reset", 80, 150, 30);
	
	
	
	globalSettings::instance()->g_useInteractiveAudio = true;
	
	if(globalSettings::instance()->g_useInteractiveAudio){
		InteractiveAudio::instance()->init(this);
		InteractiveAudio::instance()->start();
	}
	
	
	
	
	// init Ghost Fbo
	
	
	if(use_background){
		int size = globalSettings::instance()->g_backgroundImages.size();
		if(size == 0){
			use_background = false;
		}
		else{
			m_back1 = kplToss(globalSettings::instance()->g_backgroundImages.size());
			m_back2 = kplToss(globalSettings::instance()->g_backgroundImages.size());
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
	 if (globalSettings::instance()->g_useInteractiveAudio) {
	 InteractiveAudio::instance()->sendFloat(kBranchPolyphony,
	 ofClamp(ceil((float)globalSettings::instance()->g_growthMax / (float)globalSettings::instance()->g_growthMin), 1, 4));
	 InteractiveAudio::instance()->sendFloat(kActivateBirds, 0);
	 InteractiveAudio::instance()->sendFloat(kActivateVinyl, 0);
	 InteractiveAudio::instance()->sendFloat(kActivateRand1, 0);
	 }
	 */
	
	InteractiveAudio::instance()->sendBang(kStart);
	
	//	m_fadeRectangle.init(ofGetWidth(), ofGetHeight() / 7);
	//	// to set the color of the fading rectangle
	//	globalSettings::instance()->g_updateBackground();
	//
	//	globalSettings::instance()->g_scene = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//	globalSettings::instance()->g_sceneBounded = ofRectangle(20, 0, ofGetWidth() - 20, ofGetHeight());
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
	globalSettings::instance()->g_updateBackground();
	
	globalSettings::instance()->g_scene = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	globalSettings::instance()->g_sceneBounded = ofRectangle(20, 0, ofGetWidth() - 20, ofGetHeight());
	
	m_splashPosition.x = std::max(ofGetWidth() / 2 - m_splash.getWidth() / 2, m_splash.getWidth() / 2);
	m_splashPosition.y = ofGetHeight() / 2 - m_splash.getHeight() / 2;
	
	
	globalSettings::instance()->s_windowSize = MSA::Vec2f(ofGetWidth(),ofGetHeight());
	globalSettings::instance()->s_invWindowSize = MSA::Vec2f( 1.0f / globalSettings::instance()->s_windowSize.x, 1.0f / globalSettings::instance()->s_windowSize.y );
	
	//	setRenderSize(ofGetWidth(), ofGetHeight());
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
	globalSettings::instance()->g_globalCounterSec = 1;
	
	if(!bDrawGui)
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
	if(globalSettings::instance()->g_showFlowers){
		//m_flowersLayer->start();
		//m_linesLayer->start();
	}
	//if (globalSettings::instance()->g_leavesActive)
	//m_leavesLayer->start();
	//        m_twitterLayer->prestart();
	m_state = STATE_SIMULATION;
	
	if(!globalSettings::instance()->g_useInteractiveAudio){
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
	if(!globalSettings::instance()->g_useInteractiveAudio){
		soundtrack.stop();
	}
	m_splashActive = false;
}

void ofApp::keyReleased(int key)
{
	if(key == 'g'){
		bDrawGui = !bDrawGui;
		if(bDrawGui) ofShowCursor();
	}
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
	
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
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
				m_maxThreads = globalSettings::instance()->g_activeThreads;
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
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
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
		if(m_backgroundFadeTime > globalSettings::instance()->g_backgroundTransitionTime * globalSettings::instance()->g_fps){
			m_backgroundFadeTime = 1;
			m_back1 = m_back2;
			m_back2 = kplToss(globalSettings::instance()->g_backgroundImages.size());
		}
	}
	
	// reduce sampling rate for other operations: every 3 seconds
	if(m_state == STATE_SIMULATION && globalSettings::instance()->g_globalCounter % (int)globalSettings::instance()->g_fps == 0){
		globalSettings::instance()->g_globalCounterSec++;
		globalSettings::instance()->g_globalCounter = 0;
		updateSimulationState();
	}
	
	++globalSettings::instance()->g_globalCounter;
}

/*!
 
 Simulation state is updated every second!
 
 */

void ofApp::updateSimulationState(){
	
	if(m_treesLayer->averageMaturationLevel() > globalSettings::instance()->g_minColonization){
		RealtimeFetcher::instance()->pause();
		InteractiveAudio::instance()->sendBang(kMsgOff);
		m_treesLayer->stopGrowing();
		ofLog() << "REACHED MIN COLO";
	}
	
	if(m_simulationState == TIME_GROWING){
		
		if(m_numberOfIterations < globalSettings::instance()->g_firstIterations){
			if(m_treesLayer->averageMaturationLevel() > globalSettings::instance()->g_minColonization){
				ofLog() << "TIME_GROWING -> TIME_LEAVESFALLING";
				m_simulationTimer = 0;
				m_simulationState = TIME_LEAVESFALLING;
			}
		} else{
			// it reached the last iteration
			
			float maxMinColonization = MIN(globalSettings::instance()->g_minColonizationForLeaves, globalSettings::instance()->g_minColonization);
			
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
		if(m_leavesLayer->numberOfLeaves() >= globalSettings::instance()->g_leavesRows * globalSettings::instance()->g_leavesColumns){
			m_leavesLayer->stop();
			m_simulationTimer = 0;
			m_simulationState = TIME_LINES_TRANS;
			ofLog() << "Moving to TIME_LINES_TRANS state..";
		}
	}
	else if(m_simulationState == TIME_LINES_TRANS){
		
		if(m_simulationTimer >= globalSettings::instance()->g_waitLinesTime){ // HERE WE USE A TIMER
			ofLog() << "Moving to TIME_LINES state..";
			//			globalSettings::instance()->g_linesMin = (float)globalSettings::instance()->g_linesMinNorm;
			//			globalSettings::instance()->g_linesMax = (float)globalSettings::instance()->g_linesMaxNorm;
			InteractiveAudio::instance()->sendBang(kLinesOn);
			m_linesLayer->start();
			m_simulationTimer = 0;
			m_simulationState = TIME_LINES;
		}
	}
	else if(m_simulationState == TIME_LINES){
		if(m_simulationTimer >= globalSettings::instance()->g_startDetachingLeavesTime){ // HERE WE USE A TIMER
			m_leavesLayer->setWindForce(0);
			m_leavesLayer->detachLeaves();
			InteractiveAudio::instance()->sendBang(kLeavesOff);
			m_linesLayer->setMinMaxToAcc();
			//			globalSettings::instance()->g_linesMin = (float)globalSettings::instance()->g_linesMinAcc;
			//			globalSettings::instance()->g_linesMax = (float)globalSettings::instance()->g_linesMaxAcc;
			m_simulationState = TIME_LEAVESFALLING;
			ofLog() << "Moving to TIME_LEAVESFALLING";
			// NB: do not reset the counter
		}
	}
	else if(m_simulationState == TIME_LEAVESFALLING){
		
		if(m_leavesLayer->numberOfLeaves() == 0 && m_twitterLayer->numberOfBaloons() == 0){ // 12 seconds
			m_linesLayer->stop();
			if(m_numberOfIterations >= globalSettings::instance()->g_firstIterations){
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
		if(m_simulationTimer >= globalSettings::instance()->g_waitRegenerateTime){
			m_simulationTimer = 0;
			if(m_numberOfIterations >= globalSettings::instance()->g_firstIterations){
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
		if(m_simulationTimer == globalSettings::instance()->g_waitSeedTime - 3){
			InteractiveAudio::instance()->sendBang(kBreath);
		}
		if(m_simulationTimer >= globalSettings::instance()->g_waitSeedTime){
			m_treesLayer->restartGrowing();
			RealtimeFetcher::instance()->restart();
			InteractiveAudio::instance()->sendBang(kMsgOn);
			m_simulationState = TIME_GROWING;
		}
	}
	++m_simulationTimer;
	
}
//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
	if(!m_internetOk || bDebug){
		return;
	}
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
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
//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){
	if(!m_internetOk || bDebug){
		return;
	}
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
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

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){
	if(!m_internetOk || bDebug){
		return;
	}
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
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
	
	if(m_state == STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Resetting: " + ofToString(100.f - ((float)globalSettings::instance()->g_activeThreads / m_maxThreads) * 100.F) + " %", ofGetWidth() / 2 - 200, ofGetHeight() / 2);
		return;
	}
	if(!m_internetOk){
		ofDrawBitmapString("This program requires an active internet connection,", ofGetWidth() / 2 - 200, ofGetHeight() / 2);
		ofDrawBitmapString("please check your configuration!", ofGetWidth() / 2 - 200, ofGetHeight() / 2 + 30);
		return;
	}
	
	
	
	//	pushTransforms();
	
	ofEnableAlphaBlending();
	m_treesLayer->draw();
	m_linesLayer->draw();
	m_leavesLayer->draw();
	m_twitterLayer->draw();
	//m_fadeRectangle.draw();
	
	if(m_splashActive){
		ofSetColor(globalSettings::instance()->g_backgroundC, m_splashOpacity * globalSettings::instance()->g_splashOpacity);
		ofRect(0, 0, globalSettings::instance()->g_scene.width, globalSettings::instance()->g_scene.height);
		ofSetColor(255);
		ofSetColor(255, 255.0F * m_splashOpacity);
		m_splash.draw(m_splashPosition.x, m_splashPosition.y);
	}
	
	//	popTransforms();
	
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
	
	
	m_goBtn->draw();
	m_resetBtn->draw();
	
	if(bDrawGui)
		globalSettings::instance()->drawGui();
	
	
}

void ofApp::goClicked(bool & do_){
	if(!m_internetOk){
		return;
	}
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
		return;
	}
	start();
}

void ofApp::resetClicked(bool & do_){
	
	if(!m_internetOk){
		return;
	}
	
	if(STATE_SETUP && globalSettings::instance()->g_activeThreads > 0){
		return;
	}
	
	m_clearFBO = true;
	
	RealtimeFetcher::instance()->reset();
	m_treesLayer->reset();
	m_leavesLayer->reset();
	
}


#pragma mark - Control Panel

void ofApp::setupControlPanel(){
	
	
	
}

void ofApp::setupControlPanelVariables(){
	
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
		*thisOut *= globalSettings::instance()->g_soundVolume;
	}
}
#endif



void ofApp::exit()
{
	//	cout << "ofApp::exit\n";
	globalSettings::instance()->g_deallocateFonts();
}
