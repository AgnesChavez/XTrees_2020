//
//  guiCustomColorPreview.h
//  Xtrees
//
//  Created by Kamen Dimitrov on 8/17/14.
//
//

#pragma once

#include "guiCustomImpl.h"

class guiCustomColorPreview : public guiCustomImpl {
	public:
		guiCustomColorPreview() : guiCustomImpl(){
			color.set(255, 255, 255, 255);
		}

		void draw(float x, float y, float w, float h){
			ofEnableAlphaBlending();
			ofFill();
			ofSetColor(color);
			ofRect(x, y, w, h);
			ofDisableAlphaBlending();
		}

		ofColor color;
};
