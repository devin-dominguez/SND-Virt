#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	

	displaySize = ofVec2f(800, 600);
	
	heightMap.setup(160, 120);
	virt.setup(heightMap.getPixelsRef());
}

//--------------------------------------------------------------
void ofApp::update(){
	heightMap.update();
	virt.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	ofPushMatrix();
	ofPushStyle();
	
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotate(0);
	double offsetX = -displaySize.x / 2;
	double offSetY = -displaySize.y / 2;

	heightMap.draw(offsetX, offSetY, displaySize.x, displaySize.y);
	virt.draw(offsetX, offSetY, displaySize.x, displaySize.y);

	//masking
	ofFill();
	ofSetColor(0);
	ofRect(-10000 + offsetX, 0 + offSetY, 20000, -10000); //top
	ofRect(-10000 + offsetX, displaySize.y + offSetY, 20000, 10000); //bottom
	ofRect(0 + offsetX, -10000, -10000 + offSetY, 20000); //left
	ofRect(displaySize.x + offsetX, -10000 + offSetY, 10000, 20000); //right

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}