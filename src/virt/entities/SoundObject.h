#pragma once

#include "ofMain.h"
#include "AudioSystem.h"

class SoundObject {
public:
	void setup(string type, unsigned int voiceNumber, double interval);
	void update(double dt);
	void messageNow(string parameter, double value);
	void messageLater(string parameter, double value);
private:
	string type;
	unsigned int voiceNumber;
	double interval;
	double timeLeft;

	struct Message {
		string parameter;
		double value;	
	};
	deque<Message> messageQueue;

};