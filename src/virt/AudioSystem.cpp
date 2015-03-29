#include "AudioSystem.h"

ofxOscSender AudioSystem::oscOut;
vector<ofxOscMessage> AudioSystem::messages;

void AudioSystem::setup(string hostname, unsigned int port) {
	oscOut.setup(hostname, port);
}

void AudioSystem::addMessage(unsigned int voiceNumber, string destination, string parameter, double value) {
	string address = "/" + destination + "/" + "voice" + ofToString(voiceNumber) + "/" + parameter;
	ofxOscMessage message;
	message.setAddress(address);
	message.addFloatArg(value);
	messages.push_back(message);
}

void AudioSystem::update() {
	for(unsigned int i = 0; i < messages.size(); i++) {
		oscOut.sendMessage(messages[i]);
	}
	messages.clear();
}