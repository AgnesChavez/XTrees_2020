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


globalSettings::globalSettings(){
	listeners.push(g_fertility.newListener([&](int&){g_computeMaxBranches();}));
	listeners.push(g_maxLevel.newListener([&](int&){g_computeMaxBranches();}));
	listeners.push(g_useInteractiveAudio.newListener([&](bool&){g_activateSoundtrack();}));
//	listeners.push(g_soundtrack.newListener([&](&){g_activateSoundtrack();}));
	
//	g_backgroundC;

	


	splash_screen_parameters.add(g_splashC);
	splash_screen_parameters.add(g_splashDuration);
	splash_screen_parameters.add(g_splashFrequency);
	splash_screen_parameters.add(g_splashOpacity);

	loop_settings_parameters.add(g_firstIterations);
	loop_settings_parameters.add(g_waitLinesTime);
	loop_settings_parameters.add(g_waitRegenerateTime);
	loop_settings_parameters.add(g_waitSeedTime);
	loop_settings_parameters.add(g_showDuration);

	layers_control_parameters.add(g_useTwilio);
	layers_control_parameters.add(g_showFlowers);
	layers_control_parameters.add(g_showThumbs);
	layers_control_parameters.add(g_leavesActive);
	layers_control_parameters.add(g_msgOpacity);
	layers_control_parameters.add(g_treeGhostOpacity);
	layers_control_parameters.add(g_treesLayerOpacity);
	layers_control_parameters.add(g_twitterLayerOpacity);
	layers_control_parameters.add(g_leavesOpacity);

	
	branches_parameters.add(g_branchWidth);
	branches_parameters.add(g_branchMinWidth);
	branches_parameters.add(g_widthDecrease);
	branches_parameters.add(g_branchLength);
	branches_parameters.add(g_lengthDecrease);
	branches_parameters.add(g_minAngle);
	branches_parameters.add(g_maxAngle);

	
	branch_joints_parameters.add(g_jointC);
	branch_joints_parameters.add(g_jointLength);
	branch_joints_parameters.add(g_jointsAreCircles);
	branch_joints_parameters.add(g_jointThickness);


	leaves_parameters.add(g_leavesEndColor);
	leaves_parameters.add(g_leavesFertility);
	leaves_parameters.add(g_leavesLife);
	leaves_parameters.add(g_leavesMinFreq);
	leaves_parameters.add(g_leavesMaxFreq);
	leaves_parameters.add(g_leavesMinWidth);
	leaves_parameters.add(g_leavesMaxWidth);
	leaves_parameters.add(g_leavesColumns);
	leaves_parameters.add(g_leavesRows);
	
	flowers_parameters.add(g_flowersMax);
	flowers_parameters.add(g_flowersMin);
	flowers_parameters.add(g_flowersOpacity);

	
	
	
	general_tree_structure_parameters.add(g_fertility);
	general_tree_structure_parameters.add(g_minColonization);
	general_tree_structure_parameters.add(g_minColonizationForLeaves);
	general_tree_structure_parameters.add(g_maxLevel);
	general_tree_structure_parameters.add(g_treeC);
	
	
	general_tree_structure_parameters.add(branches_parameters);
	general_tree_structure_parameters.add(branch_joints_parameters);
	general_tree_structure_parameters.add(leaves_parameters);
	general_tree_structure_parameters.add(flowers_parameters);
	general_tree_structure_parameters.add(g_treeGradStart);
	general_tree_structure_parameters.add(g_treeGradEnd);

	timing_parameters.add(g_growthMin);
	timing_parameters.add(g_growthMax);
	timing_parameters.add(g_minFrequency);
	timing_parameters.add(g_maxFrequency);
	timing_parameters.add(g_linesMin);
	timing_parameters.add(g_linesMax);
	timing_parameters.add(g_treesFadeTime);
	timing_parameters.add(g_startDetachingLeavesTime);
	timing_parameters.add(g_backgroundTransitionTime);
	timing_parameters.add(g_timedExhibit);
	timing_parameters.add(loop_settings_parameters);

	messagesColors.add(g_tweetMsgC);
	messagesColors.add(g_tweetUsrC);
	messagesColors.add(g_twilioMsgC);
	messagesColors.add(g_twilioUsrC);
	messagesColors.add(g_databaseMsgC);
	messagesColors.add(g_databaseUsrC);
	
	
	messages_parameters.add(g_floatingSpeed);
	messages_parameters.add(g_pastTweets);
	messages_parameters.add(g_tweetMaxLife);
	messages_parameters.add(g_tweetMinLife);
	messages_parameters.add(messagesColors);
	
	
	lines_parameters.add(g_tLineC);
	lines_parameters.add(g_tLineWidth);
	lines_parameters.add(g_crazyCircleWidth);
	lines_parameters.add(g_crazyLineWidth);
	lines_parameters.add(g_linesWidth);
	lines_parameters.add(g_linesColor);
	
	

	audio_settings_parameters.add(g_useInteractiveAudio);
	audio_settings_parameters.add(g_soundVolume);
	
	
	
//	setupGuis();
}

void globalSettings::addSaveLoadListeners(ofxPanel& gui)
{
	listeners.push(gui.loadPressedE.newListener(this, &globalSettings::load));
	listeners.push(gui.savePressedE.newListener(this, &globalSettings::save));
}

void globalSettings::setupGuis()
{
	
	gui.setup("Settings");
	gui.add(saveParam);
	gui.add(saveAsParam);
	gui.add(loadParam);
	gui.add(reloadParam);
	
	gui.add(g_backgroundC);
	gui.add(splash_screen_parameters);
	gui.add(layers_control_parameters);
	
	
	
	
	general_tree_structure_gui.setup(general_tree_structure_parameters);
	timing_gui.setup(timing_parameters);
	messages_gui.setup(messages_parameters);
	lines_gui.setup(lines_parameters);
	audio_settings_gui.setup(audio_settings_parameters);
	

	general_tree_structure_gui.setPosition(gui.getShape().getTopRight() + glm::vec2(10,0));
	timing_gui.setPosition(general_tree_structure_gui.getShape().getTopRight() + glm::vec2(10,0));
	messages_gui.setPosition(timing_gui.getShape().getTopRight() + glm::vec2(10,0));
	lines_gui.setPosition(messages_gui.getShape().getTopRight() + glm::vec2(10,0));
	audio_settings_gui.setPosition(lines_gui.getShape().getBottomLeft() + glm::vec2(0,10));
	
	listeners.push(saveParam.newListener([&](){
		save();
	}));
	listeners.push(saveAsParam.newListener([&](){
		auto res = ofSystemSaveDialog("settings_"+ofGetTimestampString() + ".json", "Save settings to json file...");
		if(res.bSuccess)
		{
			saveTo(res.getPath());
		}
		
	}));
	listeners.push(loadParam.newListener([&](){
		auto res = ofSystemLoadDialog("Load settings from json file");
		cout << ofFilePath::getFileExt(res.getName()) << endl;
		if(res.bSuccess && ofFilePath::getFileExt(res.getName()) == "json")
		{
			loadFrom(res.getPath());
		}
	}));
	listeners.push(reloadParam.newListener([&](){
		load();
	}));
	
	
	addSaveLoadListeners(gui);
	addSaveLoadListeners(general_tree_structure_gui);
	addSaveLoadListeners(timing_gui);
	addSaveLoadListeners(messages_gui);
	addSaveLoadListeners(lines_gui);
	addSaveLoadListeners(audio_settings_gui);
	
	currentFilename  = ofToDataPath("settings.json");
	load();
	
}

void globalSettings::drawGui()
{
	
	gui.draw();
	general_tree_structure_gui.draw();
	
	timing_gui.draw();
	messages_gui.draw();
	lines_gui.draw();
	audio_settings_gui.draw();
}



void globalSettings::g_deallocateFonts()
{
	if(_guiFont){ delete _guiFont; _guiFont = nullptr;}
	if(_twitterFont){ delete _twitterFont; _twitterFont = nullptr;}
	if(_twilioFont){ delete _twilioFont; _twilioFont = nullptr;}
	if(_databaseFont){ delete _databaseFont; _databaseFont = nullptr;}
}

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

void savePanelToJson(ofJson& json, ofxPanel& panel, const string& name)
{
		panel.saveTo(json[name]);
}

void globalSettings::save()
{
	ofJson json = ofLoadJson(currentFilename);
	
	savePanelToJson( json, gui, "others");
	savePanelToJson( json, general_tree_structure_gui, "general_tree_structure");
	savePanelToJson( json, timing_gui, "timing");
	savePanelToJson( json, messages_gui, "messages");
	savePanelToJson( json, lines_gui, "lines");
	savePanelToJson( json, audio_settings_gui, "audio_settings");

	ofSavePrettyJson(currentFilename, json);
	
	
}

void loadPanelFromJson(ofJson& json, ofxPanel& panel, const string& name)
{
	if(json.contains(name))
	{
		panel.loadFrom(json[name]);
	}
}

void globalSettings::load()
{
	
	ofFile jsonFile(currentFilename);
	ofJson json = ofLoadJson(currentFilename);
	
	loadPanelFromJson(json, gui, "others");
	loadPanelFromJson(json, general_tree_structure_gui, "general_tree_structure");
	loadPanelFromJson(json, timing_gui, "timing");
	loadPanelFromJson(json, messages_gui, "messages");
	loadPanelFromJson(json, lines_gui, "lines");
	loadPanelFromJson(json, audio_settings_gui, "audio_settings");
	
}

void globalSettings::saveTo(const string& filename)
{
	currentFilename = filename;
	save();
}
void globalSettings::loadFrom(const string& filename)
{
	currentFilename = filename;
	load();
}
