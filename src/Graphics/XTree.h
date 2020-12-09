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


#pragma once

#include <string>
#include <memory>

#include "textInput.h"
#include "XTreeBranch.h"

#include "TwitterLayer.h"
#include "TwilioTrigger.h"
#include "TwitterTrigger.h"
#include "DatabaseTrigger.h"
#include "ofxTimer.h"
#include "GraphicUtilities.hpp"
#include "Leaf.h"

class XTreeSeed;
//class FlowersLayer;
class TreesManager;

class XTree {
	friend class TreesManager;
	friend class XTreeSeed;
	//  friend class FlowersLayer;
	friend class LinesLayer;
public:
	XTree(int x, int y, std::string key_ , std::shared_ptr<TwitterLayer> twitterLayer_, float direction_ = -M_PI/2.F);
	
	~XTree();
	
	void start();
	void reset();
	void stopGrowing();
	void restartGrowing();
	
	void draw();
	void update();
	void select();
	void deselect();
	
	void mouseMoved(int mouseX, int mouseY);
	bool mousePressed(int mouseX, int mouseY);
	void mouseDragged(int mouseX, int mouseY);
	
	void keyIn(int key_);
	bool isSelected(int mouseX, int mouseY);
	void loseFocus();
	void regenerate();
	void fadeImagesQuickly();
	
	// could use this for reloading from saved seeds
	void setKeyword(std::string& m_keyword);
	std::string getKeyword(){return  m_keyword;}
	
	
	int getCurrentLevel() {
		return m_currentLevel;
	}
	
	int currentBaloonsAlive;
	
	bool m_isReadyToChangeColor;
	ofColor getTreeColor() {
		
		return m_treeColor;
	}
	
	

	static void initGhostFbo() {
		s_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
		clearFbo();
	}
	
	static void clearFbo() {
		ofEnableAlphaBlending();
		beginGhostFbo(true);
		endGhostFbo();
	}
	
	static void beginGhostFbo(bool bClear=true) {
		s_fbo.begin();
		
		if(bClear) {
			auto c = globalSettings::instance()->g_backgroundC.get();
			c.a = 0;
			ofClear(c);
		}
	}
	
	static void endGhostFbo() {
		s_fbo.end();
	}
	
	static void drawGhostFbo() {
		s_fbo.draw(0,0);
	}
	
	void setDirection(float val_) {
		m_direction = val_;
	}
	
	Leaf* addLeaf() {
		if (m_trunk != NULL )
			return m_trunk->addLeaf();
		else
			return NULL;
	}
	
	void realtimeStarving(bool isStarving) {
		
		m_treeIsStarvingOfRealtimeMessages = isStarving;
	}
	
	bool isRealtmeStarving() {
		return m_treeIsStarvingOfRealtimeMessages;
	}
	
private:
	void evolve(std::shared_ptr<MessageEvent>& args);
	
	inline float getColonizationLevel() {
		// consider also stuck the branches... FIXME kludge
		return (((float)m_numberOfBranches + ((float)m_numberOfBranchesStuck / 2.F)) / (float)globalSettings::instance()->g_maxBranches) * 100.F;
	}
	
	
	int m_originalX;
	int m_originalY;
	int m_x;
	int m_y;
	std::string m_keyword;
	float m_direction;
	float m_originalDirection;
	
	ofColor m_treeColor;
	void setTreeColor(ofColor newColor);
	
	XTreeSeed* m_seed;
	XTreeBranch* m_trunk;
	std::unique_ptr<TwitterTrigger> m_twitterTrigger = nullptr;
	std::unique_ptr<TwilioTrigger> m_twilioTrigger = nullptr;
	std::unique_ptr<DatabaseTrigger> m_databaseTrigger = nullptr;
	std::shared_ptr<TwitterLayer> m_twitterLayer;
	bool m_isSelected;
	bool m_regenerating;
	int m_numberOfBranches;
	int m_numberOfBranchesStuck;
	int m_maxNumberOfBranches;
	textInput m_textbox;
	
	
	// this is used for various tasks
	ofxTimer m_timer;
	bool m_isStarting;
	bool m_isValidating;
	int m_validationRadius;
	int m_currentLevel;
	
	bool m_scheduledForRegrowth;
	bool m_isFadingImagesQuickly;
	
	bool m_drawMe;
	
	// Ghost Fbo
	static ofFbo s_fbo;
	static ofColor treesColor;
	
	ofxTimer m_flowerTimer;
	
	float m_keywordActivity;
	
	bool m_treeIsStarvingOfRealtimeMessages;
	bool m_isStopped;
	
};



