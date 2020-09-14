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
		
		m_lastId = 0;
	

  setKeyword(keyword_);
  s_instances++;
		
		
		// Easily register for all search events.
		client.registerSearchEvents(this);

		// First we load credentials from a file. These can also be loaded manually
		// using the ofxTwitter::Credentials constructor.
		//
		// Developers must get their credentials after creating an app at
		// https://apps.twitter.com
		client.setCredentialsFromFile("credentials.json");

		// Set the polling interval for.6 seconds.
		// Results will appear in the callbacks onStatus(..), onError(...), etc.
		// The thread is synchronized with the update() loop by default and is
		// thread-safe.
		client.setPollingInterval(600);
		
		//
		
}

TwitterTrigger::~TwitterTrigger() {
  //if (m_running) {
    //ofThread::stopThread();
  //}
	BaseTrigger::stop();
	client.stopAndJoin();
	
}

void TwitterTrigger::stop() {
  BaseTrigger::stop();
	client.stop();
  m_lastId = 0;
}


void TwitterTrigger::start()
{
	BaseTrigger::start();
	client.start();
}



// TODO lots of string copying and operations (it's on the main thread)
// MOVE THIS PROCESSING TO THE THREADEDFUNCTION
//https://dev.twitter.com/docs/error-codes-responses

void TwitterTrigger::fetch() {


	if(m_keyword == "nature rights") {
		m_tree->realtimeStarving(true);
		return;
	}
	ofxTwitter::SearchQuery query(m_keyword);
	query.setLanguage("en");
	query.setCount((m_lastId == 0)?globalSettings::g_pastTweets:100);
	query.setSinceId(m_lastId);
	client.search(query);
	
}

string TwitterTrigger::killEmoji(string s){

	string returnString = "";
	int l = s.length();
	int i;

	// for each byte
	for(i = 0; i < l; ++i){
		char mychar = s[i];
		string binary = ofToBinary(mychar);
		binary = binary.substr(0, 4);
		if(binary == "1111"){
			i += 3;       //jump to next char
			//cout << "Found Emoji, killing... " << endl;
		}
		else{
			returnString += mychar;
		}
	}
	return returnString;

}

unsigned long long TwitterTrigger::extractId(std::string fieldValue_) {
  std::vector<std::string> tokens = tokenize(fieldValue_,":");
  stringstream m_idReader(tokens[2]);
  unsigned long long id;
  m_idReader >> id;
  return id;
}

//<published>2012-01-18T18:14:55Z</published>
std::string TwitterTrigger::extractTime(std::string fieldValue_) {
  size_t posT, posZ;
  posT = fieldValue_.find("T");
  posZ = fieldValue_.find("Z");
  return fieldValue_.substr(posT + 1, posZ - posT - 1);
}



void TwitterTrigger::onStatus(const ofxTwitter::Status& status)
{

    ofLogNotice("TwitterTrigger::onStatus") << "Text: " << status.text();
    ofLogNotice("TwitterTrigger::onStatus") << "\tCoordinates: " << (status.coordinates() ? ofToString(status.coordinates()) : "NONE");
    ofLogNotice("TwitterTrigger::onStatus") << "\tPlace: " << (status.place() ? ofToString(status.place()->fullName()) : "NONE");
}


void TwitterTrigger::onError(const ofxTwitter::Error& error)
{
    ofLogError("TwitterTrigger::onError") << "Error: " << error.code() << " " << error.message();
}


void TwitterTrigger::onException(const std::exception& notice)
{
    ofLogError("TwitterTrigger::onException") << "Exception: " << notice.what();
}


void TwitterTrigger::onMessage(const ofJson& json)
{
    // This is the raw message json and is ignored here.
	
	if(json.empty()){
		m_tree->realtimeStarving(true);
		return;
	}

	//ofLog(OF_LOG_WARNING, "Tree %s is fetching realtime messages", m_keyword.c_str());
	
	unsigned long long id;
	std::shared_ptr<MessageEvent> theEvent;
//	bool loadedOk = true;
	
	if(!json.contains("statuses"))
	{
		m_tree->realtimeStarving(true);
		return;
	}
	auto statuses = json["statuses"];
	
	if(statuses.is_array()){
		
		if(statuses.size() == 0){
			m_tree->realtimeStarving(true);
			return;
		}
		
		for(int i = 0; i < statuses.size(); ++i){
			
//			loadedOk = true;
			
			id = statuses[i]["id"].get<uint64_t>();//.asUInt64();
			
			
			
			string noEmoji = killEmoji(statuses[i]["text"].get<std::string>());//.asString());
	
			
			string name;
			string created_at;
			
			statuses[i]["user"]["name"].get_to(name);
			statuses[i]["created_at"].get_to(created_at);
			
			
			
			
//			if(statuses[i].contains("user") && statuses[i]["user"].contains("name") && statuses[i].contains("created_at"))
//			{
				theEvent = make_shared<MessageEvent>(noEmoji,
													 statuses[i]["user"]["name"].get<std::string>(),
													 statuses[i]["created_at"].get<std::string>(),
													 SRC_TWITTER );
				
//			}
			
			
//			theEvent->message = noEmoji;
//			theEvent->user = statuses[i]["user"]["name"];//.asString();
//			theEvent->time = statuses[i]["created_at"];//.asString();
//			theEvent->consumed = false;
//			theEvent->src = SRC_TWITTER;
			
//			loadedOk = theEvent->isValid();
			
			
//			loadedOk &= theEvent->message.length() > 0;
//			loadedOk &= theEvent->user.length() > 0;
//			loadedOk &= theEvent->time.length() > 0;
			
			if (id > m_lastId) {
				m_lastId = id;
			}
			
			if (theEvent->isValid() && m_running) {
				
				m_tree->realtimeStarving(false);
				
				
				// INSERT AT THE FRONT OF THE QUEUE
//				while (!lock()) {
//					// trylock polling
//					ofSleepMillis(10);
//				}
				ofLog() << "Current Buffser Size : " << m_msgBuffer.size();
				m_msgBuffer.push_front(theEvent);
				if (m_current == m_msgBuffer.end()) {
					m_current = m_msgBuffer.begin();
				}
//				unlock();
//				ofSleepMillis(50);
				
			}
		}
	}
}
