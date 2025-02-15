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
 *
 *  Implements an asynchronous trigger, twitter, sms, or database.
 *  One of the decisions to be made is the strategy to follow when there's not enough to grow.
 *  There's a repository of old messages, or when it begins we store a padding of the latest x messages
 *  from the server or from the database. This can be policy-driven
 *
 *
 */

#ifndef MESSAGETRIGGER_HPP_
#define MESSAGETRIGGER_HPP_

#include <string>
#include <list>
#include <memory>

#include "ofMain.h"
#include "ofxTimer.h"

// TODO a shared pointer technique with reference counting,
// this is a lot of copying and the stuff could stay around.

enum MessageSource {
  SRC_TWITTER,
  SRC_TWILIO,
  SRC_DATABASE
};

struct MessageEvent {
	MessageEvent(){}
	MessageEvent(const std::string& _message,
				 const std::string& _user,
				 const std::string& _time,
				 MessageSource _src
				 )
	: message(_message)
	, user(_user)
	, time(_time)
	, src(_src)
	{}
	
	bool isValid()
	{
		return (message.length() > 0) &&
		(user.length() > 0) &&
		(time.length() > 0);
	}
	
  std::shared_ptr<ofPixels> image;
  std::string user;
  std::string message;
  std::string date; //unused
  std::string time;
  bool consumed = false;
  MessageSource src;
};

class XTree;


class BaseTrigger
{
public:
	BaseTrigger(XTree* tree, std::string keyword_, size_t  capacity = 100);
	virtual ~BaseTrigger(){}
	
	
	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void restart();
	
	
	virtual void cleanup();
	virtual void clearMessages();
	
	virtual void update();
	virtual void fetch() = 0;
	virtual float timeBetweenQueries() = 0;
	virtual void setKeyword(std::string key_);
	std::string getKeyword() const {
	  return m_keyword;
	}
	
	bool providesImages = false;
	  
	static vector<std::string> tokenize(const std::string & str, const std::string & delim);
	
	
	ofEvent<std::shared_ptr<MessageEvent> > newMessageEvent;
	
	XTree* m_tree = nullptr;
	
	void addMessageToBuffer(std::shared_ptr<MessageEvent> m);

	bool isRunning()
	{
		return m_running;
	}
	
protected:
	///this gets called from the main thread
	void notifyMessages();
	
	std::string m_keyword;
	std::atomic<bool> m_running = {true};
	std::atomic<bool> m_paused = {false};
	std::atomic<bool> m_needsReset = {false};
	size_t m_capacity;
	
	std::list<std::shared_ptr<MessageEvent> > m_msgBuffer;
	std::list<std::shared_ptr<MessageEvent> >::iterator m_current;
	
	ofxTimer m_timer;
	

};


class MessageTrigger :
public ofThread,
public BaseTrigger
{
public:
  MessageTrigger(XTree* tree, std::string keyword_, size_t  capacity = 100);
  
  virtual ~MessageTrigger();
  
	virtual void start() override;
	virtual void stop() override;
	
	int getNumQueued() {
		  return m_msgBuffer.size();
	  }
  
  
  void threadedFunction() override;
  
};

#endif // MESSAGETRIGGER_HPP_
