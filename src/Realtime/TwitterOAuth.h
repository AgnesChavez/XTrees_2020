//
//  TwitterOAuth.h
//  Xtrees
//
//  Created by Kamen Dimitrov on 8/4/14.
//
//

#ifndef Xtrees_TwitterOAuth_h
#define Xtrees_TwitterOAuth_h

#include "ofxOAuth.h"

#define OAUTH_SECRET "***REMOVED***"
#define OAUTH_KEY "***REMOVED***"

class TwitterOAuth : public ofxOAuth {
public:
	TwitterOAuth() {
		
	}
	
	~TwitterOAuth() {
		
	}
	
	void setup() {
		//ofxOAuth::setup("https://api.twitter.com", OAUTH_KEY, OAUTH_SECRET);
		setEnableVerifierCallbackServer(false);
		setAccessToken("***REMOVED***");
		setAccessTokenSecret("***REMOVED***");
		setConsumerKey(OAUTH_KEY);
		setConsumerSecret(OAUTH_SECRET);
		setApiURL("https://api.twitter.com");
		
	}
	
	std::string getQuery(string keyword, int rpp, unsigned long long int last_id) {
		return get("/1.1/search/tweets.json?lang=en&q="+keyword+"&rpp="+ofToString(rpp)+"&since_id="+ofToString(last_id));
	}
};

#endif
