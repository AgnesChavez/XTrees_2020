//
//  TwitterStream.cpp
//  XTrees_2020
//
//  Created by Roy Macdonald on 9/16/20.
//

#include "TwitterStream.h"
#include "XTree.h"
#include "ofx/Twitter/Utils.h"

void ofxTwitterStream::setup()
{
	// First we load credentials from a file. These can also be loaded using
	// the other setCredentials*(...) methods.
	//
	// Developers must get their credentials after creating an app at
	// https://apps.twitter.com.
	client.setCredentialsFromFile("credentials.json");
	
	
	//
	// In default mode all events are sent during the update() event loop.
	client.registerStreamingEvents(this);
	
	
}

ofxTwitterStream::~ofxTwitterStream()
{
	client.stopAndJoin();
}


void ofxTwitterStream::onConnect()
{
	ofLogNotice("ofxTwitterStream::onConnect") << "Connected.";
	bNeedsReconnect = true;
}


void ofxTwitterStream::onDisconnect()
{
	ofLogNotice("ofxTwitterStream::onDisconnect") << "Disconnected.";
	//Try to reconnect
	if(bNeedsReconnect)
		client.filter(query);
	bNeedsReconnect = false;
}


void ofxTwitterStream::onStatus(const ofxTwitter::Status& status)
{
	
	
	string text = killEmoji(status.text());
	string name;
	if(status.user()){
		name = killEmoji(status.user()->name());
	}
	string created_at = ofx::Twitter::Utils::format(status.createdAt(), ofx::Twitter::Utils::TWITTER_DATE_FORMAT);
	
	
	auto theEvent = make_shared<MessageEvent>(text,
										 name,
										 created_at,
										 SRC_TWITTER );
	
	if (theEvent->isValid()) {
		for(auto t: _triggers)
		{
			if(t)
			{
				if(t->isRunning())
				{
					if(t->m_tree)
					{
						if(ofIsStringInString(text, t->m_tree->getKeyword()))
						{

							t->addMessageToBuffer(theEvent);
						}
					}else{ofLogVerbose("ofxTwitterStream::onStatus") << "invalid trigger's tree";}
				}
				else{ofLogVerbose("ofxTwitterStream::onStatus")  << "Trigger not Running";}
			}
			else{ofLogVerbose("ofxTwitterStream::onStatus")  << "Invalid trigger";}
		}
	}
}


void ofxTwitterStream::onStatusDeletedNotice(const ofxTwitter::StatusDeletedNotice& notice)
{
	ofLogNotice("ofxTwitterStream::onStatusDeletedNotice") << "Status deleted.";
}


void ofxTwitterStream::onLocationDeletedNotice(const ofxTwitter::LocationDeletedNotice& notice)
{
	ofLogNotice("ofxTwitterStream::onLocationDeletedNotices") << "Location scrubbed.";
}


void ofxTwitterStream::onLimitNotice(const ofxTwitter::LimitNotice& notice)
{
//	countMissed += notice.track();
	ofLogNotice("ofxTwitterStream::onLimitNotice") << "Limited: " << notice.track();
}


void ofxTwitterStream::onStatusWithheldNotice(const ofxTwitter::StatusWithheldNotice& notice)
{
	    ofLogNotice("ofxTwitterStream::onStatusWithheldNotice") << "Status witheld.";
}


void ofxTwitterStream::onUserWitheldNotice(const ofxTwitter::UserWithheldNotice& notice)
{
	    ofLogNotice("ofxTwitterStream::onUserWitheldNotice") << "User witheld.";
}


void ofxTwitterStream::onDisconnectNotice(const ofxTwitter::DisconnectNotice& notice)
{
	ofLogNotice("ofxTwitterStream::onDisconnectNotice") << "Disconnect notice: " << notice.reason();
}


void ofxTwitterStream::onStallWarning(const ofxTwitter::StallWarning& notice)
{
	ofLogNotice("ofxTwitterStream::onStallWarning") << "Stall warning: " << notice.message();
}


void ofxTwitterStream::onException(const std::exception& notice)
{
	ofLogError("ofxTwitterStream::onException") << "Exception: " << notice.what();
}


void ofxTwitterStream::onMessage(const ofJson& json)
{
	// This is the raw message json and is ignored here.
	
}



string ofxTwitterStream::killEmoji(string s){
	
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

unsigned long long ofxTwitterStream::extractId(std::string fieldValue_) {
	std::vector<std::string> tokens = BaseTrigger::tokenize(fieldValue_,":");
	stringstream m_idReader(tokens[2]);
	unsigned long long id;
	m_idReader >> id;
	return id;
}

//<published>2012-01-18T18:14:55Z</published>
std::string ofxTwitterStream::extractTime(std::string fieldValue_) {
	size_t posT, posZ;
	posT = fieldValue_.find("T");
	posZ = fieldValue_.find("Z");
	return fieldValue_.substr(posT + 1, posZ - posT - 1);
}


void ofxTwitterStream::start()
{
	client.filter(query);
}
void ofxTwitterStream::stop()
{
	bNeedsReconnect = false;
	client.stopAndJoin();
}
void ofxTwitterStream::update()
{
	for(auto t: _triggers)
	{
		if(t) t->updateTreeStarvation();
	}
}

void ofxTwitterStream::registerTrigger(TwitterTrigger* t)
{
	if(std::find(_triggers.begin(), _triggers.end(), t) == _triggers.end())
	{
		_triggers.push_back(t);
		updateQuery();
	}
}


void ofxTwitterStream::unregisterTrigger(TwitterTrigger* t)
{
	
	auto iter = std::find(_triggers.begin(), _triggers.end(), t);
	if(iter != _triggers.end())
	{
		_triggers.erase(iter);
		updateQuery();
	}
	
}


void ofxTwitterStream::updateQuery()
{
	std::vector<std::string> tracks;
	std::string keyword;
	for(auto t: _triggers)
	{
		if(t && t->m_tree)
		{
			keyword = t->m_tree->getKeyword();
			tracks.push_back(keyword);
		}
	}
	query.setTracks(tracks);
	if(client.isRunning())
	{
		client.filter(query);
	}
}
