// TODO: support input, and perhaps other devices.

#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include <ofSerial.h> 
#include "ofxGui.h"
#include "viscaButton.h"
#include "viscaCommands.h"

#define NUM_MSG_STRINGS 50

class visca_item{
    
public:
    vector<unsigned char> item_command;
    string item_name;
    int bytePosA, bytePosB;
    int sliderAmount;
    int sliderType;
    
    ofParameter<bool> bButton;
    ofParameter<int> bByteSlider0;
    ofParameter<int> bIntSlider0;
    ofParameter<int> bIntSlider1;
    
    bool useCommand;
    
    
    void setup(string _name, ofParameterGroup & _group, vector<unsigned char> _command,int _bytePosA = -1, int _bytePosB = -1, int _sliders = 0, int _sliderType = 0){
        item_name = _name;
        item_command = _command;
        
        bytePosA = _bytePosA;
        bytePosB = _bytePosB;
        
        sliderAmount = _sliders;
        sliderType = _sliderType;
        
        ofLog()<<"setup item "<<item_name;
        
        addToGui(_group, sliderAmount, sliderType);
    }
//    vector<unsigned char> checkGui(){
//        if(bButton){
//            bButton = false;
//            return item_command;
//        }
//    }
    
    void buttonChange(bool & _state){
        ofLogNotice() << "buttonChange "<< item_name<<" state " << _state;
        bButton = false;
        useCommand = true;
    }
    void sliderChange(int & _state){
        ofLogNotice() << "sliderChange "<< item_name<<" state " << _state;
        useCommand = true;
    }
   
    void addToGui2(ofParameterGroup & _group, int _silderAmt = 0, int _type = 0){
        ofLog()<<"addToGui item "<<item_name<<" sliderAmt "<<_silderAmt<<" _type "<<_type;

        _group.add(bButton.set(item_name,false));
        bButton.addListener(this, &visca_item::buttonChange);


    }
    
    void addToGui(ofParameterGroup & _group, int _silderAmt = 0, int _type = 0){
        
        ofLog()<<"addToGui item "<<item_name<<" sliderAmt "<<_silderAmt<<" _type "<<_type;
        
        if(_silderAmt == 0){
            _group.add(bButton.set(item_name,false));
            bButton.addListener(this, &visca_item::buttonChange);
        }else if(_silderAmt == 1){
            if(_type == 1){
                _group.add(bByteSlider0.set(item_name,0,0,255));
                bByteSlider0.addListener(this, &visca_item::sliderChange);
            }else if (_type == 2){
                _group.add(bByteSlider0.set(item_name,0,0,65535));
                bByteSlider0.addListener(this, &visca_item::sliderChange);
            }
        } else if(_silderAmt == 2){
            
        }
    }
    
private:

    
};



class ofxVisca {
public:
	ofxVisca();
	~ofxVisca();
	
    
    bool connect(int device = -1);
    void update();
    void draw(int _x, int _y);
    void keyReleased(int key);
    
    
    
    void addCommand(int _camID, vector<unsigned char> _command);
    void addCommand(int _camID, vector<unsigned char> _command, int _bytePosA, int _valueA, int _bytePosB = -1, int _valueB = -1);
    
    
    visca_commands commands;
    
    vector<viscaButton> buttonGroup;
    
    bool bShowGui;
    bool bEditMode;
private:	
    
    ofxXmlSettings XML;
    
    
    
    ofSerial serial;
    bool serialActive;
    int myBaud;
    string serialID;
    
    unsigned long lastSendTime;
    bool bSerialConnected;
    int serialSendPause;
    
      vector<vector<unsigned char>> serialMessages;  

    
    void serialSending();
    void getSerialDevice();
    void checkGui();
    
    vector<unsigned char> intToBytes(int paramInt);
    
    int currentMsgString;
    string msgStrings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    
    int emptyValue = -1;
    int brightnessValue;
        int gammaValue;
    
};
