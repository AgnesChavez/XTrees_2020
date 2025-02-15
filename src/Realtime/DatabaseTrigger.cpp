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


#include "DatabaseTrigger.h"

#include <limits>
#include "KepleroUtils.h"
#include "XTree.h"


DatabaseTrigger::DatabaseTrigger(XTree* tree, std::string keyword_, int capacity) :
  MessageTrigger(tree, keyword_, capacity),
  m_size(0) {
	  this->setKeyword(keyword_);
}

DatabaseTrigger::~DatabaseTrigger() {
  
}


void DatabaseTrigger::fetch() {

  if (m_size > 0 && m_tree->isRealtmeStarving()) {
    int pos = kplToss(m_size);
    std::shared_ptr<MessageEvent> theEvent(make_shared<MessageEvent>());
    theEvent->user = m_users[pos];
    theEvent->message = m_messages[pos];
    theEvent->consumed = false;
    theEvent->src = SRC_DATABASE;
    while (!lock()) {
      // trylock polling
      ofSleepMillis(10);
    }
	  
	  addMessageToBuffer(theEvent);
    unlock();
    ofSleepMillis(50);      
  } else {
    ofSleepMillis(1000);
  }
}


void DatabaseTrigger::setKeyword(std::string key_) {

  m_keyword = key_;
  m_size = 0;
  m_users.clear();
  m_messages.clear();
	
	
	
	ofBuffer buffer = ofBufferFromFile(ofToDataPath("archive.txt"));
	
	if( buffer.size() == 0){
		ofLog(OF_LOG_ERROR, "Tree %s cannot open offline archive", m_keyword.c_str());
		return;
	}
	
	for(auto& line: buffer.getLines())
	{
		int found = line.find(m_keyword);
		if (found!=string::npos) {
		  std::vector<std::string> tokens = tokenize(line,"#");
		  if (tokens.size() == 2 ) {
			m_users.push_back(tokens[0]);
			m_messages.push_back(tokens[1]);
			++m_size;
		  }
		}
	}
}
