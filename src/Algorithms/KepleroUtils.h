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

 
#ifndef KEPLEROUTILS_H_
#define KEPLEROUTILS_H_

#include "ofMain.h"
#include <string>

int getClientWidth();

int getClientHeight();

void utf8toWStr(std::wstring& dest, const std::string& src);

std::wstring toWstring(std::string s);

std::string kplToUrlEncoding(std::string str_);

ofColor mixColor(const ofColor& rhs, const ofColor lhs, float percentage);

ofColor kplToColor(const ofColor& rhs, float percentage);

int kplToss(int number);

//int kplScreenSelection();

//int kplFullscreen();

int kplGetNumberOfDisplays();

bool CheckInternetConnection();

#endif // KEPLEROGRAPHICS_H_
