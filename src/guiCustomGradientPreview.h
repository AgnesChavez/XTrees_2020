//
//  guiCustomGradientPreview.h
//  Xtrees
//
//  Created by Kamen Dimitrov on 8/18/14.
//
//
#pragma once

#include "guiCustomImpl.h"

class guiCustomGradientPreview : public guiCustomImpl {
	public:
		guiCustomGradientPreview() : guiCustomImpl(){
			colorA.set(255, 255, 255, 255);
			colorB.set(255, 255, 255, 255);
		}

		void draw(float x, float y, float w, float h){
			ofEnableAlphaBlending();
			ofFill();

			float inc = 0.05;
			float rw = w * inc;

			int idx = 0;
			for(float t = 0; t < 1.0f; t += inc){
				ofSetColor(lerp(colorA, colorB, t));
				ofDrawRectangle(x + rw * idx, y, rw, h);
				idx++;
			}
			ofDisableAlphaBlending();

		}

		ofColor lerp(ofColor c1, ofColor c2, float t){
			float cH1, cS1, cB1;
			c1.getHsb(cH1, cS1, cB1);

			float cH2, cS2, cB2;
			c2.getHsb(cH2, cS2, cB2);

			float newH, newS, newB, newA;
			newH = ofMap(t, 0, 1, cH1, cH2);
			newS = ofMap(t, 0, 1, cS1, cS2);
			newB = ofMap(t, 0, 1, cB1, cB2);
			newA = ofMap(t, 0, 01, c1.a, c2.a);

			ofColor r;
			r.setHsb(newH, newS, newB, newA);

			return r;
		}


		ofColor colorA;
		ofColor colorB;
};

