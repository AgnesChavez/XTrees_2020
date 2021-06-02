#pragma once

#include "ofMain.h"
//#include "ofxControlPanel.h"
#include "TreesManager.h"
#include "InteractiveAudio.h"
//#include "ofxApp.h"
#include "XTreeButton.h"
#include "AlphaRect.h"

#include "TwitterLayer.h"
#include "LeavesLayer.h"
#include "LinesLayer.h"
#include "TwitterStream.h"

class ofApp :
//public ofxApp{
public ofBaseApp{
public:
	ofApp();
	~ofApp();
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void mousePressed(ofMouseEventArgs& args);
	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void keyPressed(ofKeyEventArgs& args);
	void keyReleased(int key);
	void windowResized(int w, int h);
	
#if USE_PDLIB
	void audioReceived(float * input, int bufferSize, int nChannels);
	void audioRequested(float * output, int bufferSize, int nChannels);
#endif
	
	vector<ofNode> nodes;
//	AlphaRect m_fadeRectangle;
	
	ofSoundPlayer soundtrack;
	//  FlowersLayer* m_flowersLayer;
	
	ofImage background;
	bool use_background;
	int backgroundOpacity;
	
	
	void setFromWindowSize();
	
	
private:
	void start();
	void stop();
	
	void goClicked(bool& do_);
	void resetClicked(bool& do_);
	//  void controlChanged(guiCallbackData & data);
	
	
	
	void updateSimulationState();
	
	enum GuiState {
		STATE_SETUP,
		STATE_GUI,
		STATE_SIMULATION
	};
	
	enum TimelineStates {
		TIME_GROWING, // starts growing trees
		TIME_LEAVES, // stop trees, start leaves
		TIME_LINES_TRANS, // wait some more time after leaves have fallen
		TIME_LINES, // starts the lines
		TIME_LEAVESFALLING_TRANS, // stops for N seconds with all the leaves in place
		TIME_LEAVESFALLING, // leaves fall,
		TIME_LINES_FAST, // when no leaves are anymore present, accelerate lines
		TIME_STOP_LINES, // stop
		TIME_REGENERATE, // regenerate
		TIME_GROWING_TRANS, // wait some time before restarting
	};
	
	
	GuiState m_state;
	
	
	
	std::shared_ptr<TreesManager> m_treesLayer = nullptr;
	std::shared_ptr<TwitterLayer> m_twitterLayer = nullptr;
	std::shared_ptr<LinesLayer> m_linesLayer = nullptr;
	std::shared_ptr<LeavesLayer> m_leavesLayer = nullptr;
	
	std::unique_ptr<XTreeButton> m_goBtn = nullptr;
	std::unique_ptr<XTreeButton> m_resetBtn = nullptr;
	
	ofColor m_layerObfuscationColor;
//	simpleFileLister m_backgroundsLister;
//	simpleFileLister m_soundtracksLister;
	bool m_internetOk;
	
	int m_backgroundFadeTime;
	
	int m_back1, m_back2;
	
	float m_maxThreads;
	
	bool m_screenshot;
	
	TimelineStates m_simulationState;
	
	unsigned int m_simulationTimer;
	
	unsigned int m_numberOfIterations;
	
	
	bool m_clearFBO;
	
	
	
	string toString(GuiState s)  {
		switch(s){
			case STATE_SETUP : return "STATE_SETUP";
			case STATE_GUI : return "STATE_GUI";
			case STATE_SIMULATION : return "STATE_SIMULATION";
		}
		return "";
	}
	
	string toString(TimelineStates s) {
		switch(s)
		{
			case TIME_GROWING: return "TIME_GROWING";
			case TIME_LEAVES: return "TIME_LEAVES";
			case TIME_LINES_TRANS: return "TIME_LINES_TRANS";
			case TIME_LINES: return "TIME_LINES";
			case TIME_LEAVESFALLING_TRANS: return "TIME_LEAVESFALLING_TRANS";
			case TIME_LEAVESFALLING: return "TIME_LEAVESFALLING";
			case TIME_LINES_FAST: return "TIME_LINES_FAST";
			case TIME_STOP_LINES: return "TIME_STOP_LINES";
			case TIME_REGENERATE: return "TIME_REGENERATE";
			case TIME_GROWING_TRANS: return "TIME_GROWING_TRANS";
		}
		return "";
	}
	
	
	bool bDrawStatesInfo = false;
	
	
	bool bDebug = false;
	bool bDrawGui = false;
	
};
