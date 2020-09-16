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
	
	
	// Register all available streaming events.
	// Alternatively, one can register events individually.
	//
	// In default mode all events are sent during the update() event loop.
	client.registerStreamingEvents(this);
	
	// Create a bounding box for San Francisco.
	//    ofxGeo::CoordinateBounds sanFranciscoBounds(ofxGeo::Coordinate(37.8, -122.75),
	//                                                ofxGeo::Coordinate(36.8, -121.75));
	//
	//
	//    // Create a bounding box for New York.
	//    ofxGeo::CoordinateBounds newYorkBounds(ofxGeo::Coordinate(41, -74),
	//                                           ofxGeo::Coordinate(40, -73));
	
	// Create a filter query.
//	ofxTwitter::FilterQuery query;
//
	// Set the bouning boxes of interest.
	
	// Note that bounding boxes do not act as filters for other filter
	// parameters. For example the locations below match any Tweets containing
	// track terms (even non-geo Tweets) OR coming from the San Francisco /
	// New York area.
	//
	// query.setLocations({ sanFranciscoBounds, newYorkBounds });
	
	// Track two emoji.
	// Read more about query terms here:
	// https://dev.twitter.com/streaming/overview/request-parameters
//	query.setTracks({":(", ":)"});
//
//	// Start filter query.
//	client.filter(query);
	
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
	//    count++;
//	    ofLogNotice("ofxTwitterStream::onStatus") << "Text: " << status.text();
	//    ofLogNotice("ofxTwitterStream::onStatus") << "\tCoordinates: " << (status.coordinates() ? ofToString(status.coordinates()) : "NONE");
	//    ofLogNotice("ofxTwitterStream::onStatus") << "\tPlace: " << (status.place() ? ofToString(status.place()->fullName()) : "NONE");
	
	

	
	
	string text = killEmoji(status.text());
	string name;
	if(status.user()){
		name = killEmoji(status.user()->name());
	}
	string created_at = ofx::Twitter::Utils::format(status.createdAt(), ofx::Twitter::Utils::TWITTER_DATE_FORMAT);
	
//	statuses[i]["user"]["name"].get_to(name);
//	statuses[i]["created_at"].get_to(created_at);
//
	
	//Emojis in the names was making FTGL to crash too
//	name = killEmoji(name);
	
	
	
	auto theEvent = make_shared<MessageEvent>(text,
										 name,
										 created_at,
										 SRC_TWITTER );
	
	//			if (id > m_lastId) {
	//				m_lastId = id;
	//			}
	
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
//							std::cout << "Adding message " << text << "\n";
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
	//    ofLogNotice("ofxTwitterStream::onLimitNotice") << "Status witheld.";
}


void ofxTwitterStream::onUserWitheldNotice(const ofxTwitter::UserWithheldNotice& notice)
{
	//    ofLogNotice("ofxTwitterStream::onUserWitheldNotice") << "User witheld.";
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





void ofxTwitterStream::onMessage(const ofJson& json)
{
	// This is the raw message json and is ignored here.
	
//	if(json.empty()){
//		m_tree->realtimeStarving(true);
//		return;
//	}
//
//	unsigned long long id;
//	std::shared_ptr<MessageEvent> theEvent;
//
//	if(!json.contains("statuses"))
//	{
////		m_tree->realtimeStarving(true);
//		return;
//	}
//	auto statuses = json["statuses"];
//
//	if(statuses.is_array()){
//
////		if(statuses.size() == 0){
////			m_tree->realtimeStarving(true);
////			return;
////		}
//
//		for(int i = 0; i < statuses.size(); ++i){
//
////			id = statuses[i]["id"].get<uint64_t>();
//
//
//
//			string text = killEmoji(statuses[i]["text"].get<std::string>());
//
//
//			string name;
//			string created_at;
//
//			statuses[i]["user"]["name"].get_to(name);
//			statuses[i]["created_at"].get_to(created_at);
//
//
//			//Emojis in the names was making FTGL to crash too
//			name = killEmoji(name);
//
//
//			theEvent = make_shared<MessageEvent>(text,
//												 name,
//												 created_at,
//												 SRC_TWITTER );
//
//			//			if (id > m_lastId) {
//			//				m_lastId = id;
//			//			}
//
//			if (theEvent->isValid()) {
//				for(auto t: _triggers)
//				{
//					if(t && t->isRunning() && t->m_tree && ofIsStringInString(text, t->m_tree->getKeyword()))
//					{
//						t->addMessageToBuffer(theEvent);
//					}
//				}
////				m_tree->realtimeStarving(false);
//
////				ofLog() << "Current Buffser Size : " << m_msgBuffer.size();
//
////				m_msgBuffer.push_front(theEvent);
////				if (m_current == m_msgBuffer.end())
////				{
////					m_current = m_msgBuffer.begin();
////				}
//			}
//		}
//	}
	
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
//	std::cout << "ofxTwitterStream::updateQuery " ;
	std::string keyword;
	for(auto t: _triggers)
	{
		if(t && t->m_tree)
		{
			keyword = t->m_tree->getKeyword();
			tracks.push_back(keyword);
			
//			std::cout << keyword << ", ";
			
		}
	}
//	std::cout << "\n";
	query.setTracks(tracks);
	if(client.isRunning())
	{
		client.filter(query);
	}
	
	
	
	
	
}
