#include "SoundObject.h"

void SoundObject::setup(string type, unsigned int voiceNumber, double interval) {
	this->type = type;
	this->voiceNumber = voiceNumber;
	this->interval = interval;

	timeLeft = ofRandom(0.0, interval);
}

void SoundObject::update(double dt) {
	if(timeLeft > 0 ) {
		timeLeft -= dt;
	}
	else {
		for(unsigned int i = 0; i < messageQueue.size(); i++) {
			messageNow(messageQueue[i].parameter, messageQueue[i].value);
		}
		messageQueue.clear();
		timeLeft = interval;
	}
}

void SoundObject::messageNow(string parameter, double value) {
	AudioSystem::addMessage(voiceNumber, type, parameter, value);
}

void SoundObject::messageLater(string parameter, double value) {
	//check if parameter is already in queue and update if it is
	for(unsigned int i = 0; i < messageQueue.size(); i++) {
		if(parameter == messageQueue[i].parameter) {
			messageQueue[i].value = value;
			return;
		}
	}

	//otherwise push new message onto queue
	Message message;
	message.parameter = parameter;
	message.value = value;
	messageQueue.push_back(message);

}