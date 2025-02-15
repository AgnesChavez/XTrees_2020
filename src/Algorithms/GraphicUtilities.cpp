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

#include "GraphicUtilities.hpp"




// TODO : a better way that preserves brightness
// http://stackoverflow.com/questions/7521058/colorize-grayscale-image

void kplColorize(ofImage * from, ofImage * to,  ofColor & color_){

	ofColor now;
	ofColor then;
	float lightness;
	for(int x = 0; x < from->getWidth(); ++x){
		for(int y = 0; y < from->getHeight(); ++y){
			now = from->getColor(x, y);
			lightness = now.getLightness() / 255.F;
			then.a = now.a;
			then.r = color_.r * lightness;
			then.g = color_.g * lightness;
			then.b = color_.b * lightness;
			to->setColor(x, y, then);
		}
	}
	to->update();
}

void kplSetMaxAlpha(ofImage * fromTo){
	ofColor now;
	for(int x = 0; x < fromTo->getWidth(); ++x){
		for(int y = 0; y < fromTo->getHeight(); ++y){
			now = fromTo->getColor(x, y);
			if(now.a != 0){
				now.a = std::max(now.a + 126, 255);
			}
			fromTo->setColor(x, y, now);
		}
	}
	fromTo->update();
}

float kplNormalizeAngle(float angle_){
	angle_ = (float)fmod((double)angle_, 2. * PI);
	if(angle_ < 0){
		return 2. * M_PI + angle_;
	}
	return angle_;
}

ofColor lerpHSB(ofColor c1, ofColor c2, float t){
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

ofColor applyGamma(ofColor c, float gamma) {
	float r = ofMap(c.r, 0, 255,0,1.0);
	float g = ofMap(c.g, 0, 255,0,1.0);
	float b = ofMap(c.b, 0, 255,0,1.0);
	
	r = powf(r, 1.0/gamma);
	g = powf(g, 1.0/gamma);
	b = powf(b, 1.0/gamma);
	
	ofColor result;
	result.a = c.a;
	result.r = r * 255.0;
	result.g = g * 255.0;
	result.b = b * 255.0;
	
	return result;
}








