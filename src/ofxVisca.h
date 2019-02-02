// TODO: support input, and perhaps other devices.

#pragma once

#include <ofSerial.h> 
#include "ofxGui.h"
#include "ofParameter.h"

class visca_item{
public:
    void setup(std::string _name, std::vector<unsigned char> _command){
        item_name = _name;
        item_command = _command;
    }
    std::vector<unsigned char> checkGui(){
        if(bButton){
            bButton = false;
            return item_command;
        }
    }
      ofParameter<bool> bButton;
//private:
    std::vector<unsigned char> item_command;
    std::string item_name;
  
    
   
};

struct visca_commands {
    //-----menu
    std::vector<unsigned char> menuON = {0x80, 0x01, 0x06, 0x06, 0x02, 0xFF};
    std::vector<unsigned char> menuOFF = {0x80, 0x01, 0x06, 0x06, 0x03, 0xFF}; //Menu off
    std::vector<unsigned char> menuBACK = {0x80, 0x01, 0x06, 0x06, 0x00, 0xFF};
    std::vector<unsigned char> menuUP = {0x80, 0x01, 0x06, 0x06, 0x11, 0xFF};
    std::vector<unsigned char> menuDOWN = {0x80, 0x01, 0x06, 0x06, 0x12, 0xFF};
    std::vector<unsigned char> menuLEFT = {0x80, 0x01, 0x06, 0x06, 0x14, 0xFF};
    std::vector<unsigned char> menuRIGHT = {0x80, 0x01, 0x06, 0x06, 0x18, 0xFF};
    
    //-----filter
    std::vector<unsigned char> dnManual = {0x80, 0x01, 0x04, 0x51, 0x03, 0xFF};  // day night manual
    std::vector<unsigned char> night = {0x80, 0x01, 0x04, 0x01, 0x02, 0xFF};
    std::vector<unsigned char> day = {0x80, 0x01, 0x04, 0x01, 0x03, 0xFF};
    
    //----digital zoom
    std::vector<unsigned char> dzoomON = {0x80, 0x01, 0x04, 0x06, 0x02, 0xFF}; // d zoom on
    std::vector<unsigned char> dzoomOFF = {0x80, 0x01, 0x04, 0x06, 0x03, 0xFF}; // d zoom off
    
    std::vector<unsigned char> dzoomCombined = {0x80, 0x01, 0x04, 0x36, 0x00, 0xFF}; // Optical/Digital Zoom Combined
    std::vector<unsigned char> dzoomSeparate = {0x80, 0x01, 0x04, 0x36, 0x01, 0xFF}; // Optical/Digital Zoom Separate
    std::vector<unsigned char> dzoomSTOP = {0x80, 0x01, 0x04, 0x06, 0x00, 0xFF}; //
    
    std::vector<unsigned char> dzoomTELE = {0x80, 0x01, 0x04, 0x06, 0x20, 0xFF}; //0x2p p=0 (Low) to 7 (High)
    std::vector<unsigned char> dzoomTELE0 = {0x81, 0x01, 0x04, 0x06, 0x20, 0xFF}; 
    std::vector<unsigned char> dzoomTELE7 = {0x81, 0x01, 0x04, 0x06, 0x27, 0xFF}; 
    std::vector<unsigned char> dzoomWIDE = {0x80, 0x01, 0x04, 0x06, 0x30, 0xFF}; //0x3p,  Enabled during Separate Mode
    
    std::vector<unsigned char> dzoomDIRECT = {0x80, 0x01, 0x04, 0x46, 0x00, 0x00, 0x00, 0x00, 0xFF}; //pq: D-Zoom Position,  Enabled during Separate Mode
    
    
    //-----does not work on CV345-CS, CV380-CS
    std::vector<unsigned char> powerON = {0x80, 0x01, 0x04, 0x00, 0x02, 0xFF}; 
    std::vector<unsigned char> powerOFF = {0x80, 0x01, 0x04, 0x00, 0x03, 0xFF};
};

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
    void addCommand(int _camID, std::vector<unsigned char> _command, int _bytePosA, int _valueA, int _bytePosB = -1, int _valueB = -1);
    
    ofxPanel gui_visca;
     ofParameterGroup parameters_menu;
//    ofParameter<bool> menuON;
    ofParameter<bool> menuOFF;
    ofParameter<bool> menuBACK;
    ofParameter<bool> menuLEFT;
    ofParameter<bool> menuRIGHT;
    ofParameter<bool> menuUP;
    ofParameter<bool> menuDOWN;
    
    ofParameterGroup parameters_lens;
    ofParameter<bool> dnManual;
    ofParameter<bool> day;
    ofParameter<bool> night;
    
    ofParameterGroup parameters_dzoom;
    
    visca_commands commands;
    
    std::vector<visca_item> allVisca;
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
    void checkGui();
};
