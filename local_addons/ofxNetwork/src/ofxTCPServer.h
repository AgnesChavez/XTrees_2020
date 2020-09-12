#ifndef _OFX_TCP_SERVER_
#define _OFX_TCP_SERVER_

#include "ofConstants.h"
#include "ofThread.h"
#include "ofxTCPManager.h"
#include <map>

#define TCP_MAX_CLIENTS  32

//forward decleration
class ofxTCPClient;

class ofxTCPServer : public ofThread{

	public:

		ofxTCPServer();
		~ofxTCPServer();
		void setVerbose(bool _verbose);
		bool setup(int _port, bool blocking = false);
		void setMessageDelimiter(std::string delim);
	
		bool close();
		bool disconnectClient(int clientID);

		int getNumClients(); //total number of clients - not sutible for iterating through clients with
		int getLastID(); //this returns the last current id number if you want to loop through with a for loop 
		
		int getPort();
		bool isConnected();

		int getClientPort(int clientID);
		std::string getClientIP(int clientID);

		bool isClientSetup(int clientID);
		bool isClientConnected(int clientID);

		//send data as a std::string - a short message
		//is added to the end of the std::string which is
		//used to indicate the end of the message to
		//the receiver see: STR_END_MSG (ofTCPClient.h)
		bool send(int clientID, std::string message);
		bool sendToAll(std::string message);

		//send and receive raw bytes lets you send and receive
		//byte (char) arrays without modifiying or appending the data.
		//Strings terminate on null bytes so this is the better option
		//if you are trying to send something other than just ascii std::strings

		bool sendRawBytes(int clientID, const char * rawBytes, const int numBytes);
		bool sendRawBytesToAll(const char * rawBytes, const int numBytes);

		//the received message length in bytes
		int getNumReceivedBytes(int clientID);

		//get the message as a std::string
		//this will only work with messages coming via
		//send() and sendToAll()
		//or from messages terminating with the STR_END_MSG
		//which by default is  [/TCP]
		//eg: if you want to send "Hello World" from other
		//software and want to receive it as a std::string
		//sender should send "Hello World[/TCP]"
		std::string receive(int clientID);

		//pass in buffer to be filled - make sure the buffer
		//is at least as big as numBytes
		int receiveRawBytes(int clientID, char * receiveBytes,  int numBytes);


		//don't call this
		//--------------------------
		void threadedFunction();


		ofxTCPManager			TCPServer;
	std::map<int,ofxTCPClient>	TCPConnections;

	protected:
		bool			connected, verbose;
		std::string			str;
		int				idCount, port;
		bool			bClientBlocking;
		std::string			messageDelimiter;

};


#endif



