//
//  guiTypeConfigFileList.cpp
//  Xtrees
//
//  Created by Kamen Dimitrov on 8/19/14.
//
//

#include "guiTypeConfigFileList.h"

void guiTypeConfigFileList::setValue(float _value, int whichParam) {
	selection = _value;
}


void guiTypeConfigFileList::saveSettings(ofxXmlSettings& settings) {
	//  settings.pushTag(name, 0);
	if (selection != -1) {
		settings.setValue(name, lister->entries[selection].filename);
	} else {
		settings.setValue(name, "");
	}
	//  settings.popTag();
}

void guiTypeConfigFileList::loadSettings(ofxXmlSettings& settings) {
	std::string filename = settings.getValue(name, "", 0);
	if (filename != "") {
		int nPosition = -1;
		for (int i = 0; i < lister->entries.size(); ++i) {
			if (lister->entries[i].filename == filename) {
				nPosition = i;
			}
		}
		
		if (nPosition != -1) { // found it
			selection = nPosition;
		} else {
			// TODO: log error somehow!
		}
	}
	notify();
}