//
//  guiTypeConfigFileList.h
//  Xtrees
//
//  Created by Kamen Dimitrov on 8/19/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiTypeFileLister.h"


class guiTypeConfigFileList : public guiTypeFileLister {
	void setValue(float _value, int whichParam);
	void saveSettings(ofxXmlSettings& settings);
	void loadSettings(ofxXmlSettings& settings);

	
	bool hasSelectionChanged() { return selection_has_changed; }
	void clearSelectionChangedFlag() { selection_has_changed = false; }
	
private:
	
	bool selection_has_changed;
};
