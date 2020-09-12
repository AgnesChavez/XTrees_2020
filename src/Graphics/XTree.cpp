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

#include "RealtimeFetcher.h"


#define VAL_RADIUS 200
#define VAL_REPETITIONS 5.0F


ofFbo XTree::s_fbo;


XTree::XTree(int x, int y, std::string key_, std::tr1::shared_ptr <TwitterLayer> twitterLayer_, float direction_) :
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
	m_twilioTrigger(NULL),
	m_databaseTrigger(NULL),
	m_keywordActivity(.0f),
	m_regenerating(false),
	m_isStarting(false),
	m_isFadingImagesQuickly(false),
	m_treeIsStarvingOfRealtimeMessages(false),
	m_isStopped(false){

	m_textbox.init(g_guiRenderer);
	m_textbox.setText(key_);
	m_textbox.enabled = false;
	ofAddListener(m_textbox.evtEnter, this, &XTree::setKeyword);
	
	m_twitterTrigger = new TwitterTrigger(this, key_);
	RealtimeFetcher::instance()->triggers.push_back(m_twitterTrigger);
	
		
	if(g_useTwilio){
		m_twilioTrigger = new TwilioTrigger(this, key_);
		RealtimeFetcher::instance()->triggers.push_back(m_twilioTrigger);
	}
	if(g_useArchive){
		m_databaseTrigger = new DatabaseTrigger(this, key_);
		RealtimeFetcher::instance()->triggers.push_back(m_databaseTrigger);
	}
	m_seed = new XTreeSeed(this);
	m_flowerTimer.setAlarm(100);
		m_isReadyToChangeColor = false;
}

XTree::~XTree(){

	delete m_trunk;
	delete m_seed;

	if(m_twitterTrigger != NULL){
		RealtimeFetcher::instance()->removeTrigger(m_twitterTrigger);
		delete m_twitterTrigger;
	}

	if(g_useTwilio && m_twilioTrigger != NULL){
		RealtimeFetcher::instance()->removeTrigger(m_twilioTrigger);
		delete m_twilioTrigger;
	}
	if(g_useArchive && m_databaseTrigger != NULL){
		RealtimeFetcher::instance()->removeTrigger(m_databaseTrigger);
		delete m_databaseTrigger;
	}
}

void XTree::start(){
	m_isReadyToChangeColor = true;
	m_numberOfBranches = 0;
	m_numberOfBranchesStuck = 0;
	m_direction = m_originalDirection;
	m_x = m_originalX;
	m_y = m_originalY;
	deselect();
	m_seed->hideHandle();
	ofAddListener(m_twitterTrigger->newMessageEvent, this, &XTree::evolve);
	if(g_useTwilio && m_twilioTrigger != NULL){
		ofAddListener(m_twilioTrigger->newMessageEvent, this, &XTree::evolve);
	}
	if(g_useArchive && m_databaseTrigger != NULL){
		ofAddListener(m_databaseTrigger->newMessageEvent, this, &XTree::evolve);
	}
	m_timer.setAlarm(g_waitSeedTime * 1000 + ofRandom(g_waitSeedTime * 1000));
	m_isStarting = true;
	m_isFadingImagesQuickly = false;

}

void XTree::stopGrowing(){
}

void XTree::restartGrowing(){
	m_numberOfBranches = 0;
	m_numberOfBranchesStuck = 0;
}

void XTree::reset(){
	m_direction = m_originalDirection;
	m_x = m_originalX;
	m_y = m_originalY;
	m_seed->resetPosition();
	ofRemoveListener(m_twitterTrigger->newMessageEvent, this, &XTree::evolve);

	if(g_useTwilio && m_twilioTrigger != NULL){
		ofRemoveListener(m_twilioTrigger->newMessageEvent, this, &XTree::evolve);
	}
	if(g_useArchive && m_databaseTrigger != NULL){
		ofRemoveListener(m_databaseTrigger->newMessageEvent, this, &XTree::evolve);
	}
	m_treeIsStarvingOfRealtimeMessages = false;
	stopGrowing();
	delete m_trunk;
	m_trunk = NULL;
	m_seed->startBlinking();
}



void XTree::draw(){
	
	ofSetColor(255,255,255,255);
	
	if(m_trunk != NULL){
		ofPushMatrix();
		m_trunk->draw();
		ofPopMatrix();
	}

	m_seed->draw();
	
	if(m_isValidating){
		ofSetColor(255, 0, 0);
		ofSetLineWidth(1);
		ofNoFill();
		ofEllipse(m_x, m_y, m_validationRadius, m_validationRadius);
	}

	ofPushMatrix();
	glTranslatef(m_x, m_y + 40, 0);
	m_textbox.draw();
	ofPopMatrix();

}

void XTree::regenerate(){

	m_regenerating = true;

}

void XTree::update(){
	if(1){
		if(m_regenerating){
			if(!m_scheduledForRegrowth){
				m_flowerTimer.stop();
				stopGrowing();
				//m_timer.setAlarm(10000);
				m_scheduledForRegrowth = true;
			}
			else{
				if(currentBaloonsAlive == 0){
					
					ofEnableAlphaBlending();
					
					// draws the tree
					XTree::beginGhostFbo(false);
					if(m_trunk != NULL){
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
						newHorOffset = ofRandomf() * ofRandom(50, 120);
						newPosition.x = m_originalX + newHorOffset;
					} while(!g_sceneBounded.inside(newPosition));

					m_seed->move(newHorOffset, 0);
					m_x = m_originalX + newHorOffset;
					m_scheduledForRegrowth = false;
					m_seed->startBlinking();
					m_seed->hideHandle();
					return;
				}
			}
		}
		else if(g_leavesLayer->m_running){
			if(m_flowerTimer.alarm()){
				Leaf * theleaf = addLeaf();
				if(theleaf != NULL){
					g_leavesLayer->addLeaf(theleaf);
				}
				m_flowerTimer.setAlarm(ofRandom(g_leavesMinFreq, g_leavesMaxFreq));
			}
		}

	}

	m_seed->update();
	if(m_trunk != NULL){
		m_trunk->update();
	}
	
	if(m_isValidating){
		if(m_timer.alarm()){
			m_isValidating = false;
			m_validationRadius = VAL_RADIUS;
			m_timer.stop();
		}
		else{
			m_validationRadius = VAL_RADIUS * (1.F - fmod((1.F / VAL_REPETITIONS), m_timer.getDiffN()) * VAL_REPETITIONS);
		}
		
	}


}

void XTree::mouseMoved(int mouseX, int mouseY){
	m_seed->mouseMoved(mouseX, mouseY);
}

bool XTree::mousePressed(int mouseX, int mouseY){
	return m_seed->mousePressed(mouseX, mouseY);
}

void XTree::mouseDragged(int mouseX, int mouseY){
	m_seed->mouseDragged(mouseX, mouseY);
}

void XTree::keyIn(int key_){
	if(m_isSelected){
		#ifdef XTREES_APP
			m_textbox.keyPressed(key_);
		#endif
	}
}

bool XTree::isSelected(int mouseX, int mouseY){
	return m_isSelected;
}

void XTree::select(){
	m_textbox.enabled = true;
	m_isSelected = true;
}


void XTree::deselect(){
	m_textbox.enabled = false;
	m_isSelected = false;
}

// could use this for reloading from saved seeds
void XTree::setKeyword(std::string & keyword){
	m_keyword = keyword;
	m_textbox.setText(keyword);
	m_twitterTrigger->setKeyword(keyword);
	if(g_useTwilio && m_twilioTrigger != NULL){
		m_twilioTrigger->setKeyword(keyword);
	}
	if(g_useArchive && m_databaseTrigger != NULL){
		m_databaseTrigger->setKeyword(keyword);
	}
	m_isSelected = false;
	m_timer.set();
	m_timer.setAlarm(500);
	m_isValidating = true;
}

void XTree::evolve(std::tr1::shared_ptr <MessageEvent> & args){
	
	if(m_trunk == NULL){
		ofVec2f start(m_x, m_y); //20% randomicity
		ofVec2f end = ofPointFromPivot(start, m_direction, g_branchLength + ofRandomf() * (float)g_branchLength / 10.F);
		m_trunk = new XTreeBranch(this,
								  NULL,
								  g_branchWidth,
								  ofVec2f(m_x, m_y),
								  end);
		float fakeBaloonDirection = m_direction;
		ofVec2f endPlusBaloon = end;
		fakeBaloonDirection = kplNormalizeAngle(fakeBaloonDirection);
		// now m_branchRotation it's the angle between the horizontal line passing thrugh the apex  real branch rotation
		if(fakeBaloonDirection >= 0.F && fakeBaloonDirection <= PI / 2.F){
			endPlusBaloon.x += 357;
			if(!g_scene.inside(endPlusBaloon)){
				fakeBaloonDirection += M_PI / 2.F;
			}
		}
		else if(fakeBaloonDirection > PI / 2.F && fakeBaloonDirection <= M_PI){
			endPlusBaloon.x -= 357;
			if(!g_scene.inside(endPlusBaloon)){
				fakeBaloonDirection -= M_PI / 2.F;
			}
		}
		else if(fakeBaloonDirection > M_PI && fakeBaloonDirection <= 3. * M_PI / 2.){
			endPlusBaloon.x -= 357;
			if(!g_scene.inside(endPlusBaloon)){
				fakeBaloonDirection += M_PI / 2.F;
			}
		}
		else if(fakeBaloonDirection > 3. * M_PI / 2.){
			endPlusBaloon.x += 357;
			if(!g_scene.inside(endPlusBaloon)){
				fakeBaloonDirection -= M_PI / 2.F;
			}
		}
		m_twitterLayer->addBaloon(this, args, m_trunk->currentEnd(), fakeBaloonDirection);
		m_seed->stopBlinking();
	}
	else{
		XTreeBranch * firstActive = m_trunk->returnInactiveBranch();
		if(firstActive == NULL){
			ofLog() << "Branched stuck..";
			m_numberOfBranchesStuck++;
			return;
		}
		else{
			if(firstActive->getLevel() > m_currentLevel){
				m_currentLevel = firstActive->getLevel();
			}
			float fakeBaloonDirection = firstActive->getRotation();
			fakeBaloonDirection = kplNormalizeAngle(fakeBaloonDirection);
			ofVec2f endPlusBaloon = firstActive->end();
			// now m_branchRotation it's the angle between the horizontal line passing thrugh the apex  real branch rotation
			if(fakeBaloonDirection >= 0.F && fakeBaloonDirection <= PI / 2.F){
				endPlusBaloon.x += 357;
				if(!g_scene.inside(endPlusBaloon)){
					fakeBaloonDirection += M_PI / 2.F;
				}
			}
			else if(fakeBaloonDirection > PI / 2.F && fakeBaloonDirection <= M_PI){
				endPlusBaloon.x -= 357;
				if(!g_scene.inside(endPlusBaloon)){
					fakeBaloonDirection -= M_PI / 2.F;
				}
			}
			else if(fakeBaloonDirection > M_PI && fakeBaloonDirection <= 3. * M_PI / 2.){
				endPlusBaloon.x -= 357;
				if(!g_scene.inside(endPlusBaloon)){
					fakeBaloonDirection += M_PI / 2.F;
				}
			}
			else if(fakeBaloonDirection > 3. * M_PI / 2.){
				endPlusBaloon.x += 357;
				if(!g_scene.inside(endPlusBaloon)){
					fakeBaloonDirection -= M_PI / 2.F;
				}
			}
			m_twitterLayer->addBaloon(this, args, firstActive->currentEnd(), fakeBaloonDirection);
		}
	}
	++m_numberOfBranches;
}



void XTree::fadeImagesQuickly(){
	m_isFadingImagesQuickly = true;
}


#pragma mark - Colors

void XTree::setTreeColor(ofColor newColor) {
	m_treeColor = newColor;
	m_isReadyToChangeColor = false;
}

