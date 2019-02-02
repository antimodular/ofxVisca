#include "ofApp.h"





//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    bSendSerialMessage = false;
    ofBackground(255);    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    visca_object.connect();
    
    vidGrabber.setDeviceID(1);
    vidGrabber.initGrabber(1920, 1080);
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
//    if(key == 'd'){
//        visca_object.serialMessages.push_back(menuON2);
//        ofLog()<<"menuON2";
//    }
//    if(key == 'c'){
//        visca_object.serialMessages.push_back(menuOFF2);
//        ofLog()<<"menuOFF2";
//    }
//    if(key == '1'){
//        visca_object.serialMessages.push_back(dnManual);
//        ofLog()<<"dnManual";
//    }
//    if(key == '2'){
//        visca_object.serialMessages.push_back(night);
//        ofLog()<<"night";
//    }
//    if(key == '3'){
//        visca_object.serialMessages.push_back(day);
//        ofLog()<<"day";
//    }
//    if(key == '0'){
//        visca_object.serialMessages.push_back(powerON);
//        ofLog()<<"powerON";
//    }
//    if(key == '9'){
//        visca_object.serialMessages.push_back(powerOFF);
//        ofLog()<<"powerOFF";
//    }
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


