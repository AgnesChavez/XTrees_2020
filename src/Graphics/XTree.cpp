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


#include "XTree.h"
#include "Settings.h"
#include "XTreeSeed.h"
#include "MathUtilities.h"
#include "Settings.h"
#include "LeavesLayer.h"

#include "RealtimeFetcher.h"


#define VAL_RADIUS 200
#define VAL_REPETITIONS 5.0F


ofFbo XTree::s_fbo;


XTree::XTree(int x, int y, std::string key_, std::shared_ptr <TwitterLayer> twitterLayer_, float direction_) :
m_trunk(NULL),
m_originalX(x),
m_originalY(y),
m_x(x),
m_y(y),
m_keyword(key_),
m_isSelected(false),
m_isValidating(false),
m_direction(direction_),
m_originalDirection(direction_),
m_twitterLayer(twitterLayer_),
m_currentLevel(0),
m_numberOfBranches(0),
m_numberOfBranchesStuck(0),
currentBaloonsAlive(0),
m_scheduledForRegrowth(false),
m_keywordActivity(.0f),
m_regenerating(false),
m_isStarting(false),
m_isFadingImagesQuickly(false),
m_treeIsStarvingOfRealtimeMessages(false),
m_isStopped(false) {
	
	m_textbox.init(&globalSettings::instance()->g_guiRenderer());
	m_textbox.setText(key_);
	m_textbox.enabled = false;
	ofAddListener(m_textbox.evtEnter, this, &XTree::setKeyword);
	
	
	if(globalSettings::instance()->g_useTwitter){
		m_twitterTrigger = std::make_unique<TwitterTrigger>(this, key_);
		RealtimeFetcher::instance()->addTrigger(m_twitterTrigger.get());
	}
	
	if (globalSettings::instance()->g_useTwilio) {
		m_twilioTrigger = std::make_unique<TwilioTrigger>(this, key_);
		RealtimeFetcher::instance()->addTrigger(m_twilioTrigger.get());
	}
	if (globalSettings::instance()->g_useArchive) {
		m_databaseTrigger = std::make_unique<DatabaseTrigger>(this, key_);
		RealtimeFetcher::instance()->addTrigger(m_databaseTrigger.get());
	}
	m_seed = new XTreeSeed(this);
	m_flowerTimer.setAlarm(100);
	m_isReadyToChangeColor = false;
	
	
	realtimeStarving(!globalSettings::instance()->g_useTwilio && !globalSettings::instance()->g_useTwitter);
	
	
}

XTree::~XTree() {
	
	delete m_trunk;
	delete m_seed;
	
	RealtimeFetcher::instance()->removeTrigger(m_twitterTrigger.get());
	RealtimeFetcher::instance()->removeTrigger(m_twilioTrigger.get());
	RealtimeFetcher::instance()->removeTrigger(m_databaseTrigger.get());
	
}

void XTree::start() {  
	m_isReadyToChangeColor = true;
	m_numberOfBranches = 0;
	m_numberOfBranchesStuck = 0;
	m_direction = m_originalDirection;
	m_x = m_originalX;
	m_y = m_originalY;
	deselect();
	m_seed->hideHandle();
	if(globalSettings::instance()->g_useTwitter && m_twitterTrigger){
		ofAddListener(m_twitterTrigger->newMessageEvent,this,&XTree::evolve);
	}
	if (globalSettings::instance()->g_useTwilio && m_twilioTrigger) {
		ofAddListener(m_twilioTrigger->newMessageEvent,this,&XTree::evolve);
	}
	if (globalSettings::instance()->g_useArchive && m_databaseTrigger) {
		ofAddListener(m_databaseTrigger->newMessageEvent,this,&XTree::evolve);
	}
	m_timer.setAlarm(globalSettings::instance()->g_waitSeedTime * 1000 + ofRandom(globalSettings::instance()->g_waitSeedTime * 1000));
	m_isStarting = true;
	m_isFadingImagesQuickly = false;
	
}

void XTree::stopGrowing() {
}

void XTree::restartGrowing() {
	m_numberOfBranches = 0;
	m_numberOfBranchesStuck = 0;
}

void XTree::reset() {
	m_direction = m_originalDirection;
	m_x = m_originalX;
	m_y = m_originalY;
	m_seed->resetPosition();
	if(globalSettings::instance()->g_useTwitter && m_twitterTrigger){
		ofRemoveListener(m_twitterTrigger->newMessageEvent,this,&XTree::evolve);
	}
	if (globalSettings::instance()->g_useTwilio && m_twilioTrigger) {
		ofRemoveListener(m_twilioTrigger->newMessageEvent,this,&XTree::evolve);
	}
	if (globalSettings::instance()->g_useArchive && m_databaseTrigger) {
		ofRemoveListener(m_databaseTrigger->newMessageEvent,this,&XTree::evolve);
	}
	m_treeIsStarvingOfRealtimeMessages = false;
	stopGrowing();
	delete m_trunk;
	m_trunk = NULL;
	m_seed->startBlinking();
}



void XTree::draw() {
	
	ofSetColor(255,255,255,255);
	
	if (m_trunk != NULL) {
		ofPushMatrix();
		m_trunk->draw();
		ofPopMatrix();
	}
	
	m_seed->draw();
	
	if (m_isValidating) {
		ofSetColor(255,0,0);
		ofSetLineWidth(1);
		ofNoFill();
		ofDrawEllipse(m_x, m_y, m_validationRadius, m_validationRadius);
	}
	
	ofPushMatrix();
	glTranslatef(m_x, m_y + 40, 0);
	m_textbox.draw();
	ofPopMatrix();
	
}

void XTree::regenerate() {
	
	m_regenerating = true;
	
}

void XTree::update() {
	if (1) {
		if (m_regenerating) {
			if (!m_scheduledForRegrowth) {
				m_flowerTimer.stop();
				stopGrowing();
				//m_timer.setAlarm(10000);
				m_scheduledForRegrowth = true;
			}
			else{
				if (currentBaloonsAlive == 0) {
					
					ofEnableAlphaBlending();
					
					// draws the tree
					XTree::beginGhostFbo(false);
					if (m_trunk != NULL) {
						ofPushMatrix();
						m_trunk->draw(true);
						ofPopMatrix();
					}
					m_seed->draw();
					XTree::endGhostFbo();
					
					m_scheduledForRegrowth = false;
					delete m_trunk;
					m_trunk = NULL;
					m_regenerating = false;
					m_isReadyToChangeColor = true;
					// modify slightly the direction
					m_direction = m_originalDirection + ofRandomf() / 15.f;
					
					// do not go out of bounds
					ofVec2f newPosition(m_originalX, m_originalY);
					int newHorOffset;
					do {
						newHorOffset = ofRandomf() * ofRandom(50,120);
						newPosition.x = m_originalX + newHorOffset;
					} while (!globalSettings::instance()->g_sceneBounded.inside(newPosition));
					
					m_seed->move(newHorOffset, 0);
					m_x = m_originalX + newHorOffset;
					m_scheduledForRegrowth = false;
					m_seed->startBlinking();
					m_seed->hideHandle();
					return;
				}
			}
		}
		else if(globalSettings::instance()->g_leavesLayer->m_running){
			if (m_flowerTimer.alarm()) {
				Leaf* theleaf = addLeaf();
				if (theleaf != NULL) {
					globalSettings::instance()->g_leavesLayer->addLeaf(theleaf);
				}
				m_flowerTimer.setAlarm(ofRandom(globalSettings::instance()->g_leavesMinFreq, globalSettings::instance()->g_leavesMaxFreq));
			}
		}
		
	}
	
	m_seed->update();
	if (m_trunk) {
		m_trunk->update();
	}
	
	if (m_isValidating) {
		if (m_timer.alarm()) {
			m_isValidating = false;
			m_validationRadius = VAL_RADIUS;
			m_timer.stop();
		}
		else{
			m_validationRadius = VAL_RADIUS * (1.F - fmod((1.F/VAL_REPETITIONS), m_timer.getDiffN()) * VAL_REPETITIONS);
		}
		
	}
	
	
}

void XTree::mouseMoved(int mouseX, int mouseY) {
	m_seed->mouseMoved(mouseX, mouseY);
}

bool XTree::mousePressed(int mouseX, int mouseY) {
	return m_seed->mousePressed(mouseX, mouseY);
}

void XTree::mouseDragged(int mouseX, int mouseY) {
	m_seed->mouseDragged(mouseX, mouseY);
}

void XTree::keyIn(int key_) {
	if (m_isSelected) {
//#ifdef XTREES_APP
		m_textbox.keyPressed(key_);
//#endif
	}
}

bool XTree::isSelected(int mouseX, int mouseY) {
	return m_isSelected;
}

void XTree::select() {
	m_textbox.enabled = true;
	m_isSelected = true;
}


void XTree::deselect() {
	m_textbox.enabled = false;
	m_isSelected = false;
}

// could use this for reloading from saved seeds
void XTree::setKeyword(std::string& keyword) {
	m_keyword = keyword;
	m_textbox.setText(keyword);
	if(globalSettings::instance()->g_useTwitter && m_twitterTrigger){
		m_twitterTrigger->setKeyword(keyword);
	}
	if (globalSettings::instance()->g_useTwilio && m_twilioTrigger) {
		m_twilioTrigger->setKeyword(keyword);
	}
	if (globalSettings::instance()->g_useArchive && m_databaseTrigger) {
		m_databaseTrigger->setKeyword(keyword);
	}
	m_isSelected = false;
	m_timer.set();
	m_timer.setAlarm(500);
	m_isValidating = true;
}

void XTree::evolve(std::shared_ptr<MessageEvent>& args) {
	if(args->consumed)return;
	if (m_trunk == NULL) {
		ofVec2f start(m_x, m_y);   //20% randomicity
		ofVec2f end = ofPointFromPivot(start, m_direction, globalSettings::instance()->g_branchLength + ofRandomf() * (float)globalSettings::instance()->g_branchLength / 10.F);
		m_trunk= new XTreeBranch(this,
								 NULL,
								 globalSettings::instance()->g_branchWidth,
								 ofVec2f(m_x, m_y),
								 end);
		float fakeBaloonDirection = m_direction;
		ofVec2f endPlusBaloon = end;
		fakeBaloonDirection = kplNormalizeAngle(fakeBaloonDirection);
		// now m_branchRotation it's the angle between the horizontal line passing thrugh the apex  real branch rotation
		if (fakeBaloonDirection >=0.F && fakeBaloonDirection <= PI/2.F) {
			endPlusBaloon.x += 357;
			if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
				fakeBaloonDirection += M_PI/2.F;
			}
		}
		else if(fakeBaloonDirection > PI / 2.F && fakeBaloonDirection <= M_PI){
			endPlusBaloon.x -= 357;
			if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
				fakeBaloonDirection -= M_PI/2.F;
			}
		}
		else if(fakeBaloonDirection > M_PI && fakeBaloonDirection <= 3. * M_PI / 2.){
			endPlusBaloon.x -= 357;
			if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
				fakeBaloonDirection += M_PI/2.F;
			}
		}
		else if(fakeBaloonDirection > 3. * M_PI / 2.){
			endPlusBaloon.x += 357;
			if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
				fakeBaloonDirection -= M_PI/2.F;
			}
		}
		m_twitterLayer->addBaloon(this, args, m_trunk->currentEnd(), fakeBaloonDirection);
		m_seed->stopBlinking();
	}
	else{
		XTreeBranch* firstActive = m_trunk->returnInactiveBranch();
		if (firstActive == NULL) {
			ofLog() << "Branched stuck..";
			m_numberOfBranchesStuck++;
			return;
		}
		else{
			if (firstActive->getLevel() > m_currentLevel) {
				m_currentLevel = firstActive->getLevel();
			}
			float fakeBaloonDirection = firstActive->getRotation();
			fakeBaloonDirection = kplNormalizeAngle(fakeBaloonDirection);
			ofVec2f endPlusBaloon = firstActive->end();
			// now m_branchRotation it's the angle between the horizontal line passing thrugh the apex  real branch rotation
			if (fakeBaloonDirection >=0.F && fakeBaloonDirection <= PI/2.F) {
				endPlusBaloon.x += 357;
				if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
					fakeBaloonDirection += M_PI/2.F;
				}
			}
			else if(fakeBaloonDirection > PI / 2.F && fakeBaloonDirection <= M_PI){
				endPlusBaloon.x -= 357;
				if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
					fakeBaloonDirection -= M_PI/2.F;
				}
			}
			else if(fakeBaloonDirection > M_PI && fakeBaloonDirection <= 3. * M_PI / 2.){
				endPlusBaloon.x -= 357;
				if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
					fakeBaloonDirection += M_PI/2.F;
				}
			}
			else if(fakeBaloonDirection > 3. * M_PI / 2.){
				endPlusBaloon.x += 357;
				if (!globalSettings::instance()->g_scene.inside(endPlusBaloon)) {
					fakeBaloonDirection -= M_PI/2.F;
				}
			}
			m_twitterLayer->addBaloon(this, args, firstActive->currentEnd(), fakeBaloonDirection);
		}
	}
	++m_numberOfBranches;
}



void XTree::fadeImagesQuickly() {
	m_isFadingImagesQuickly = true;
}


#pragma mark - Colors

void XTree::setTreeColor(ofColor newColor) {
	m_treeColor = newColor;
	m_isReadyToChangeColor = false;
}

