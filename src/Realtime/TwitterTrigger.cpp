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
 *  TwitterTrigger.cpp
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 *  Algorithm:
 *  This class needs to be a thread because every call to getUrl is blocking
 *  even if it's callback based.
 *  When started, the threadedFunction does the call to getUrl. Note that the
 *  httpResponse method will be called from the same thread. The locking mechanism
 *  will then protect the list and not block the update callback that is called
 *  from the main thread and serves to consume the messages (timer based)
 *
 *
 */

 // TODO: do we need a watchdog to see if internet is down?
 // does m_http raise an exception i internet is broken?
 // TODO: check for bad values especially in ids

#include "TwitterTrigger.h"
#include <limits>
#include "KepleroUtils.h"
#include "XTree.h"
#include "ofJson.h"


int TwitterTrigger::s_instances;


//----------------------------------------

TwitterTrigger::TwitterTrigger(XTree* tree, std::string keyword_, int capacity) :
	BaseTrigger(tree, keyword_, capacity){

  setKeyword(keyword_);
  s_instances++;
}

TwitterTrigger::~TwitterTrigger() {
	BaseTrigger::stop();
}

void TwitterTrigger::updateTreeStarvation()
{
	if(m_tree) m_tree->realtimeStarving(m_msgBuffer.size() == 0);
	
}
