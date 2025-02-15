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

#include "TreesManager.h"
#ifndef XTREES_SCREENSAVER
#include "InteractiveAudio.h"
#endif
#include "Settings.h"
#include "GraphicUtilities.hpp"
#include "ofxXmlSettings.h"

#define SEED_RADIUS 21
#define HALF_RADIUS_BACK 13
#define SEED_DISTANCE_SQUARED 121
#define TIMETOFADE 5.0f

TreesManager::TreesManager(std::shared_ptr<TwitterLayer> twitterLayer_) :
  m_simulationRunning(false),
  m_twitterLayer(twitterLayer_) {
	m_bRemoveTree = false;
//  CopyPixels((PixelType *)image.getPixels(), (const PixelType *)ms_tempPixels.getPixels(), xmin, xmax + 1, ymin, ymax, TEXTURE_WIDTH, TEXTURE_HEIGHT);
//  image.update();
		
	m_GhostLifespan = globalSettings::instance()->g_treesFadeTime * globalSettings::instance()->g_fps * 60;
	m_GhostTimeToLive = m_GhostLifespan;
	m_GhostCurrentAlpha = 255.0f;
	m_GhostCurrentFadingSpeed=-1;
		m_ShuffleColors = false;

}

TreesManager::~TreesManager() {
  reset();
}



// TODO: this becomes private and gets called just once the used has decided which kind of tree he wants
bool TreesManager::addTree(int x_, int y_, std::string keyword_, float direction_) {
  XTree* tree = new XTree(x_, y_, keyword_, m_twitterLayer, direction_);

  m_trees.push_back(tree);
  return true;
}

void TreesManager::setDelTree(int id){
	m_bRemoveTree = true;
	m_treeIdToRemove = id;
}

void TreesManager::delTree(int id){
	std::vector <XTree *>::iterator it = m_trees.begin();
	int idx = 0;

	while(it != m_trees.end()){
		if(idx == id){
			((XTree *)*it)->reset();
			delete *it;
			it = m_trees.erase(it);
			break;
		}
		else{
			++it;
			idx++;
		}
	}



}
ofColor TreesManager::getRandomTreeColor()
{
	float t = abs(ofRandomf());
	return lerpHSB(globalSettings::instance()->g_treeGradStart, globalSettings::instance()->g_treeGradEnd, t);
}

void TreesManager::shuffleTreesColor() {
	ofColor newColor = getRandomTreeColor();
	currentTreesColor = newColor;
	
	/*
	std::vector <XTree *>::iterator it = m_trees.begin();
	while(it != m_trees.end()) {
		((XTree*)*it)->setTreeColor(newColor);
		++it;
	}
	*/
}

void TreesManager::update() {    
  for (int i = 0; i < m_trees.size(); ++i) {
    m_trees[i]->update();
  }
	
	// update colors
	if(m_trees.size()){
		for(int i = 0; i < m_trees.size(); ++i){
			if(m_trees[i]->m_isReadyToChangeColor) {
				m_trees[i]->setTreeColor(currentTreesColor);
			}
		}
	}

	
	if(m_bRemoveTree){
		m_bRemoveTree = false;
		delTree(m_treeIdToRemove);
	}
}

void TreesManager::draw() {  
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	if(m_GhostCurrentFadingSpeed < 0) {
		ofSetColor(255, 255, 255,255);
	} else {
		m_GhostTimeToLive -= m_GhostCurrentFadingSpeed;
		m_GhostCurrentAlpha = (m_GhostTimeToLive / m_GhostLifespan) * 255.0f;
		
		if(m_GhostTimeToLive < 0) {
			XTree::clearFbo();
			m_GhostCurrentFadingSpeed=-1;
			m_GhostTimeToLive = 0;
		}
		ofSetColor(255, 255, 255,m_GhostCurrentAlpha);
	}
	XTree::drawGhostFbo();
	
  for (int i = 0; i < m_trees.size(); ++i) {
    m_trees[i]->draw();
  }
}

void TreesManager::stopGrowing() {

	for(int i = 0; i < m_trees.size(); ++i){
    m_trees[i]->stopGrowing();
}
}


void TreesManager::restartGrowing() {
	for(int i = 0; i < m_trees.size(); ++i){
    m_trees[i]->restartGrowing();
}
}

void TreesManager::stop() {
  for (int i = 0; i < m_trees.size(); ++i) {
    m_trees[i]->reset();
  }
}



bool TreesManager::start() {

	shuffleTreesColor();
	
  if (m_trees.size()) {
    for (int i = 0; i < m_trees.size(); ++i) {
      m_trees[i]->start();
    }
    return true;
  }
  return false;
}

void TreesManager::reset() {
	
  std::vector<XTree*>::iterator it = m_trees.begin();
	while(it != m_trees.end()){
    delete *it++;
	}
  m_trees.clear();
}

Leaf* TreesManager::addLeaf() {
  return m_trees[kplToss(m_trees.size())]->addLeaf();
}

void TreesManager::regenerate() {
	shuffleTreesColor();
	
  for (int i = 0; i < m_trees.size(); ++i) {
    m_trees[i]->regenerate();
  }

}

#ifndef XTREES_SCREENSAVER

//--------------------------------------------------------------
void TreesManager::mouseMoved(ofMouseEventArgs& args){
		for(int i = 0; i < m_trees.size(); ++i){
    m_trees[i]->mouseMoved(args.x, args.y);
}
	}

//--------------------------------------------------------------
void TreesManager::mouseDragged(ofMouseEventArgs& args){
		for(int i = 0; i < m_trees.size(); ++i){
    m_trees[i]->mouseDragged(args.x, args.y);
		}

}

//--------------------------------------------------------------
void TreesManager::mousePressed(ofMouseEventArgs& args){
  if (!globalSettings::instance()->g_sceneBounded.inside(args.x, args.y)) {
    return;
  }
  int sel = -1;
  for (int i = 0; i < m_trees.size(); ++i)
    if (m_trees[i]->mousePressed(args.x, args.y)) {
      sel = i;
      break;
    }
  
  if (sel != -1) {
		
		if(args.button == 2) {
			delTree(sel);
			sel = -1;
		}
		
    for (int i = 0; i < m_trees.size(); ++i)
      if (i != sel)
        m_trees[i]->deselect();
  } else {
    for (int i = 0; i < m_trees.size(); ++i)
        m_trees[i]->deselect();
    addTree(args.x, args.y);
    std::vector<float> msg;
    msg.push_back(4000);    // time
    msg.push_back(0);
    InteractiveAudio::instance()->sendFloatList(kPlaceSeed, msg);
  }
}

//--------------------------------------------------------------
// TODO: a nice fade out
void TreesManager::keyPressed(ofKeyEventArgs& args){
		for(int i = 0; i < m_trees.size(); ++i){
    m_trees[i]->keyIn(args.key);
}
	}

void TreesManager::saveSeeds() {
  TiXmlDocument xmlDocument;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlElement * treesNode = new TiXmlElement( "trees" );
  xmlDocument.LinkEndChild(decl );
  for (int i = 0; i < m_trees.size(); ++i) {
    TiXmlElement * tree;
    tree = new TiXmlElement( "tree" );
    tree->SetAttribute("name", m_trees[i]->m_keyword);
    tree->SetAttribute("x", m_trees[i]->m_x);
    tree->SetAttribute("y", m_trees[i]->m_y);
    tree->SetDoubleAttribute("dir", (double)m_trees[i]->m_originalDirection);
    treesNode->LinkEndChild(tree);
  }
  
  xmlDocument.LinkEndChild(treesNode);
//  std::string filename("../../../data/init.xml");
	std::string filename(ofToDataPath("init.xml"));
  xmlDocument.SaveFile(filename);

}

void TreesManager::loadSeeds() {
	ofxXmlSettings xml;
  std::string filename(ofToDataPath("init.xml"));
		if(!xml.loadFile(filename)){
      return;
		}
  xml.pushTag("trees");
  int entries = xml.getNumTags("tree");
  if (entries > 0) {    
    for (int i = 0; i < entries; ++i) {    
      std::string name = xml.getAttribute("tree", "name", "", i);
      int x = xml.getAttribute("tree", "x", ofGetWidth()/2, i);
      int y = xml.getAttribute("tree" , "y", ofGetHeight()/2, i);
      float direction = xml.getAttribute("tree", "dir", 270.f, i);
      addTree(x, y, name, direction);
    }
  }

}

bool TreesManager::areTreesInFullMaturation() {
  for (int i = 0; i < m_trees.size(); ++i) {
    if (m_trees[i]->getColonizationLevel() < globalSettings::instance()->g_minColonization) {
      return false;
    } 
  }
  return true;
}

float TreesManager::averageMaturationLevel() {
  float runningSum = .0f;
		for(int i = 0; i < m_trees.size(); ++i){
    runningSum += m_trees[i]->getColonizationLevel();
		}
  if (!m_trees.size() || runningSum == 0.F) {
    return -1;
  }
  return runningSum / (float)m_trees.size();
}

	void TreesManager::showGhostFbo() {
		m_GhostLifespan = globalSettings::instance()->g_treesFadeTime * globalSettings::instance()->g_fps * 60;
		m_GhostTimeToLive = m_GhostLifespan;
		m_GhostCurrentAlpha = 255.0f;
		
		m_GhostCurrentFadingSpeed = -1;
	}

	void TreesManager::fadeGhostFbo(){
		
		m_GhostLifespan = globalSettings::instance()->g_treesFadeTime * globalSettings::instance()->g_fps * 60;
		m_GhostTimeToLive = m_GhostLifespan;
		m_GhostCurrentAlpha = 255.0f;
		
		m_GhostCurrentFadingSpeed = (m_GhostTimeToLive / globalSettings::instance()->g_fps) / TIMETOFADE;
}


#endif
