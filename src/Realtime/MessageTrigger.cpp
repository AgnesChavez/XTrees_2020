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


#include "MessageTrigger.h"
#include "KepleroUtils.h"
#include "Settings.h"


#define MAX_QUEUE_SIZE 100

// this is needed because on 64 bit we need to register the thread

BaseTrigger::BaseTrigger(XTree* tree, std::string keyword_, size_t  capacity)
: m_tree(tree)
, m_capacity(capacity)
, m_keyword (keyword_)
{
	m_current = m_msgBuffer.begin();
	
}


void BaseTrigger::start() {
	m_running = true;
	m_paused = false;
	m_timer.setAlarm(10);
}


void BaseTrigger::stop() {
	m_running = false;
}


void BaseTrigger::pause() {
	m_paused = true;
}

void BaseTrigger::clearMessages() {
	m_msgBuffer.clear();
	m_current = m_msgBuffer.begin();
}

void BaseTrigger::restart() {
	m_paused = false;
}

void BaseTrigger::cleanup() {
	
	if (m_msgBuffer.size() > m_capacity) {
		int i = m_msgBuffer.size() - m_capacity;
		auto ite = m_msgBuffer.end();
		--ite;
		if (m_current == ite) {
			for (int j = i; j > 0 && m_current != m_msgBuffer.begin(); --j) {
				--m_current;
			}
		}
		while (m_current != ite && i-- > 0) {
			--ite;
			m_msgBuffer.pop_back();
		}
	}
}


vector<string> BaseTrigger::tokenize(const string & str, const string & delim)
{
	vector<string> tokens;
	
	size_t p0 = 0, p1 = string::npos;
	while(p0 != string::npos)
	{
		p1 = str.find_first_of(delim, p0);
		if(p1 != p0)
		{
			string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}
	return tokens;
}

void BaseTrigger::setKeyword (std::string key_) {
	m_keyword = kplToUrlEncoding(key_);
	m_msgBuffer.clear();
}


void BaseTrigger::notifyMessages()
{
	
	if (!m_paused && !m_msgBuffer.empty()) {
		// TODO priority management now
		// now the ownership is to the receiving thread :)
		// actually bullshit, but we are always on the main thread.
		// this just works on this assumption
		std::shared_ptr<MessageEvent> argument = *m_current;
		ofNotifyEvent(newMessageEvent, argument, this);
		m_current = m_msgBuffer.erase(m_current);
		if (!(m_current == m_msgBuffer.begin()))
			--m_current;
	}
}

// called from the local thread, blocks on the mutex of this trigger
void BaseTrigger::update() {
	// keyword has changed... wait to reset without blocking the thread
	// and do not produce anything until has been reset
	bool emitNewMessage = m_timer.alarm();
	
	/* once the alarm has been triggered we basically poll the mutex waiting the
	 other thread to sleep when the http request is over */
	
	if (emitNewMessage) {
		
		//      if (mutex.try_lock()) {
		notifyMessages();
		//        mutex.unlock();
		//      }
		
		m_timer.set();
		m_timer.setAlarm(ofRandom(g_minFrequency, g_maxFrequency));
	}
}






MessageTrigger::MessageTrigger(XTree* tree, std::string keyword_, size_t capacity_) :
BaseTrigger(tree, keyword_, capacity_)
{
	m_running = true;
	m_paused = false;
	m_needsReset = false;
}

MessageTrigger::~MessageTrigger() {
	stop();
	ofThread::waitForThread(true);
}

void MessageTrigger::start() {
	BaseTrigger::start();
	ofThread::startThread();//false, false);   // non blocking (uses trylock)
}


void MessageTrigger::stop() {
	if (m_running) {
		m_running = false;
		ofThread::stopThread();
	}
}



// TODO lots of string copying and operations (it's on the main thread)
// MOVE THIS PROCESSING TO THE THREADEDFUNCTION
//https://dev.twitter.com/docs/error-codes-responses


void MessageTrigger::threadedFunction() {
	
	
	++g_activeThreads;
	
	//  std::cout << this << " threadedFunction has entered" << std::endl;
	while(isThreadRunning()){
		if(!m_paused){
			
			if (!m_msgBuffer.size()) {
				fetch();   // to be implemented by subclasses
			}
			
			cleanup();
			//      float sleepingTime = timeBetweenQueries() / 20;
			for (int i = 0; i < 20 && m_running; ++i) {
				ofSleepMillis(30);
			}
		}
		else{
			ofSleepMillis(200);
		}
	}
	m_msgBuffer.clear();
	m_current = m_msgBuffer.begin();
	
	
	--g_activeThreads;
	
	//  std::cout << this << " threadedFunction has exited" << std::endl;
}

