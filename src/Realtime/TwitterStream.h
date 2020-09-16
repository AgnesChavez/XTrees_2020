//
//  TwitterStream.hpp
//  XTrees_2020
//
//  Created by Roy Macdonald on 9/16/20.
//

#pragma once


#include "ofMain.h"
#include "ofxTwitter.h"
#include "TwitterTrigger.h"


class ofxTwitterStream
{
public:
    void setup();
    
	~ofxTwitterStream();
	
	void start();
	void stop();
	
	void update();
	
	
	/// TwitterClient callbacks
    void onConnect();
    void onDisconnect();
    void onStatus(const ofxTwitter::Status& status);
    void onStatusDeletedNotice(const ofxTwitter::StatusDeletedNotice& notice);
    void onLocationDeletedNotice(const ofxTwitter::LocationDeletedNotice& notice);
    void onLimitNotice(const ofxTwitter::LimitNotice& notice);
    void onStatusWithheldNotice(const ofxTwitter::StatusWithheldNotice& notice);
    void onUserWitheldNotice(const ofxTwitter::UserWithheldNotice& notice);
    void onDisconnectNotice(const ofxTwitter::DisconnectNotice& notice);
    void onStallWarning(const ofxTwitter::StallWarning& notice);
    void onException(const std::exception& exception);
    void onMessage(const ofJson& json);


	//Utils
	string killEmoji(string s);
	unsigned long long extractId(std::string fieldValue_);
	std::string extractTime(std::string fieldValue_);
	
	void registerTrigger(TwitterTrigger*);
	void unregisterTrigger(TwitterTrigger*);
	
	void updateQuery();
private:
	
	
	ofxTwitter::FilterQuery query;
	std::vector<TwitterTrigger*> _triggers;
	ofxTwitter::StreamingClient client;

	bool bNeedsReconnect = false;
	
	
};
