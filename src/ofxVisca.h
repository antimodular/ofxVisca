// TODO: support input, and perhaps other devices.

#pragma once

#include <ofSerial.h> 


class ofxVisca {
public:
	ofxVisca();
	~ofxVisca();
	
    
    bool connect(int device = -1);
    void update();
    void draw(int _x, int _y);
    void keyReleased(int key);
    
    std::vector<std::vector<unsigned char>> serialMessages;    
    
    void addCommand(int _camID, std::vector<unsigned char> _command);
    
private:	
    ofSerial serial;
    bool serialActive;
    int myBaud;
    std::string serialID;
    
    unsigned long lastSendTime;
    bool bSerialConnected;
    int serialSendPause;
    

    
    void serialSending();
    void getSerialDevice();
};
