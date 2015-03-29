#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class AudioSystem {
public:
	static void setup(string hostname, unsigned int port);
	static void update();
	static void addMessage(unsigned int voiceNumber, string destination, string parameter, double value);
private:
	static ofxOscSender oscOut;
	static vector<ofxOscMessage> messages;
};