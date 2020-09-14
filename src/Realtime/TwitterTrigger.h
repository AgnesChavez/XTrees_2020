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


/*
 *  TwitterTrigger.h
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 *  Give the option of overriding or not the minimum time leaving the list to go to infinity... risky
 *  In this case we suppose that someone is monitoring the queue and if it gets too long can cut it!
 *
 *  If the minimum time is much less than the filling rate we'll end up out of memory or anyway it's a disaster
 *
 *  With the technique skipIfFull some messages will be removed to try to show the messages always within the specified time limits
 *  With the technique neverSkip we risk out of memory but every message will be displayed
 *  With the technique overrideMinimum we go much faster when there are many messages, otherwise we stay at a normal rate
 */


#ifndef TwitterTrigger_H_
#define TwitterTrigger_H_

#include "MessageTrigger.h"
#include <string>
#include <sstream>
#include <list>

//#include "KplHttpRequest.h"
#include "tinyxml.h"
#include "Settings.h"

#include "ofxTwitter.h"

class TwitterTrigger :
public BaseTrigger {
public:
	TwitterTrigger(XTree* tree, std::string keyword_, int capacity = 100);
	virtual ~TwitterTrigger();
	
	virtual void start() override;
	virtual void stop() override;
	
	virtual void fetch()override;
	virtual float timeBetweenQueries()override
	{
		if(g_timedExhibit) {
			return ofClamp(1, g_showDuration, 60) * 60.F * 1000.F * s_instances / 150.F;
		} else {
			return 60.F * 60.F * 1000.F * s_instances / 150.F;
		}
	}
	virtual void setKeyword(std::string key_) override{
		m_keyword = key_;
		m_msgBuffer.clear();
	}
	
	
	void onStatus(const ofxTwitter::Status& status);
	void onError(const ofxTwitter::Error& error);
	void onException(const std::exception& exception);
	void onMessage(const ofJson& json);
	
	ofxTwitter::SearchClient client;
	
	
private: 
	
	void retrieveMessages();
	
	// <id>tag:search.twitter.com,2005:159700307838894080</id>
	unsigned long long extractId(std::string fieldValue_);
	std::string extractTime(std::string fieldValue_);
	
	//  KplHttpRequest m_http;
	string m_requestUrl;
	
	static int s_instances;
	
	unsigned long long int m_lastId;
	// ofxFTGL crashing with emoji
	string killEmoji(string s);
	
};


#endif
