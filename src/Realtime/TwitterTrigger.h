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

#include "tinyxml.h"
#include "Settings.h"

class TwitterTrigger :
public BaseTrigger {
public:
	TwitterTrigger(XTree* tree, std::string keyword_, int capacity = 100);
	virtual ~TwitterTrigger();

	virtual void fetch()override {}
	virtual float timeBetweenQueries()override
	{
		return 0;
	}
	virtual void setKeyword(std::string key_) override{
		m_keyword = key_;
		m_msgBuffer.clear();
	}

	void updateTreeStarvation();
	
private:

	static int s_instances;


};


#endif
