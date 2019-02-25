#include "ofApp.h"





//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    
    bSendSerialMessage = false;
//    ofBackground(241,150,33);   
        ofBackground(71,6,60); 
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    visca_object.connect();
    bShowGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    visca_object.update();
    /*
     if (bSendSerialMessage){
     
     // (1) write the letter "a" to serial:
     serial.write''a');
     
     // (2) read
     // now we try to read 3 chars
     // since we might not get them all the time 3 - but sometimes 0, 6, or something else,
     // we will try to read three chars, as much as we can
     // otherwise, we may have a "lag" if we don't read fast enough
     // or just read three every time. now, we will be sure to 
     // read as much as we can in groups of three...
     
     nTimesRead = 0;
     ncharsRead = 0;
     int nRead  = 0;  // a temp variable to keep count per read
     
     unsigned char charsReturned[3];
     
     memset(charsReadString, 0, 4);
     memset(charsReturned, 0, 3);
     
     while( (nRead = serial.readchars( charsReturned, 3)) > 0){
     nTimesRead++;    
     ncharsRead = nRead;
     };
     
     memcpy(charsReadString, charsReturned, 3);
     
     bSendSerialMessage = false;
     readTime = ofGetElapsedTimef();
     }
     */    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofDrawBitmapStringHighlight("ofxVisca example", 10,10);
//    ofDrawBitmapStringHighlight("fps "+ofToString(ofGetFrameRate()), 10,30);
    //    if (ncharsRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
    //        ofSetColor(0);
    //    } else {
    //        ofSetColor(220);
    //    }
    /*
     string msg;
     msg += "click to test serial:\n";
     msg += "nchars read " + ofToString(ncharsRead) + "\n";
     msg += "nTimes read " + ofToString(nTimesRead) + "\n";
     msg += "read: " + ofToString(charsReadString) + "\n";
     msg += "(at time " + ofToString(readTime, 3) + ")";
     font.drawString(msg, 50, 100);
     */
    
    visca_object.draw(10,100);
    if(bShowGui){
//        visca_object.gui_visca.draw();
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
    
    visca_object.keyReleased(key);
    if(key == 'g'){
        bShowGui = !bShowGui;
    }
    
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


