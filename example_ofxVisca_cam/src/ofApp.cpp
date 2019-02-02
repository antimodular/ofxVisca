#include "ofApp.h"





//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    
    vidGrabber.setDeviceID(1);
    vidGrabber.initGrabber(1920, 1080);
    
    bSendSerialMessage = false;
    ofBackground(255);    
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    visca_object.connect();

}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        
    }
    visca_object.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
       
    vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    ofDrawBitmapStringHighlight("ofxVisca example", 10,10);
    ofDrawBitmapStringHighlight("fps "+ofToString(ofGetFrameRate(),1), 10,30);

    visca_object.draw(10,100);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
    
    visca_object.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bSendSerialMessage = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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


