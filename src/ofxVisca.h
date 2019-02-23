// TODO: support input, and perhaps other devices.

#pragma once

#include "ofMain.h"

#include <ofSerial.h> 
#include "ofxGui.h"

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

struct visca_commands {
    //https://www.epiphan.com/userguides/LUMiO12x/Content/UserGuides/PTZ/3-operation/VISCAcommands.htm
    
    //-----menu
    vector<unsigned char> menuON = {0x80, 0x01, 0x06, 0x06, 0x02, 0xFF};
    vector<unsigned char> menuOFF = {0x80, 0x01, 0x06, 0x06, 0x03, 0xFF}; //Menu off
    vector<unsigned char> menuBACK = {0x80, 0x01, 0x06, 0x06, 0x00, 0xFF};
    vector<unsigned char> menuUP = {0x80, 0x01, 0x06, 0x06, 0x11, 0xFF};
    vector<unsigned char> menuDOWN = {0x80, 0x01, 0x06, 0x06, 0x12, 0xFF};
    vector<unsigned char> menuLEFT = {0x80, 0x01, 0x06, 0x06, 0x14, 0xFF};
    vector<unsigned char> menuRIGHT = {0x80, 0x01, 0x06, 0x06, 0x18, 0xFF};
    
    //-----filter
    vector<unsigned char> dnManual = {0x80, 0x01, 0x04, 0x51, 0x03, 0xFF};  // day night manual
    vector<unsigned char> night = {0x80, 0x01, 0x04, 0x01, 0x02, 0xFF};
    vector<unsigned char> day = {0x80, 0x01, 0x04, 0x01, 0x03, 0xFF};
    
    //-----white balance
    vector<unsigned char> wbAuto = {0x80, 0x01, 0x04, 0x35, 0x00, 0xFF};  //
    vector<unsigned char> wbIndoor = {0x80, 0x01, 0x04, 0x35, 0x01, 0xFF};
    vector<unsigned char> wbOutdoor = {0x80, 0x01, 0x04, 0x35, 0x02, 0xFF};
    vector<unsigned char> wbOnePush = {0x80, 0x01, 0x04, 0x35, 0x03, 0xFF};  //
    vector<unsigned char> wbATW = {0x80, 0x01, 0x04, 0x35, 0x04, 0xFF}; // Auto Tracing White Balance
    vector<unsigned char> wbOnePushTrig = {0x80, 0x01, 0x04, 0x10, 0x05, 0xFF}; //One Push Trigger
    vector<unsigned char> wbManual = {0x80, 0x01, 0x04, 0x35, 0x05, 0xFF};
    
    vector<unsigned char> wbRGain_Reset = {0x80, 0x01, 0x04, 0x03, 0x00, 0xFF};
    vector<unsigned char> wbRGain_Up = {0x80, 0x01, 0x04, 0x03, 0x02, 0xFF};
    vector<unsigned char> wbRGain_Down = {0x80, 0x01, 0x04, 0x03, 0x03, 0xFF};
    vector<unsigned char> wbRGain_Direct = {0x80, 0x01, 0x04, 0x43, 0x00, 0x00, 0x00, 0x00, 0xFF}; //byte 6 and 7 (0~0xFF)
    
    vector<unsigned char> wbBGain_Reset = {0x80, 0x01, 0x04, 0x04, 0x00, 0xFF};
    vector<unsigned char> wbBGain_Up = {0x80, 0x01, 0x04, 0x04, 0x02, 0xFF};
    vector<unsigned char> wbBGain_Down = {0x80, 0x01, 0x04, 0x04, 0x03, 0xFF};
    vector<unsigned char> wbBGain_Direct =  {0x80, 0x01, 0x04, 0x44, 0x00, 0x00, 0x00, 0x00, 0xFF}; //byte 6 and 7 (0~0xFF)
    
    //-----exposure
    vector<unsigned char> aeAuto = {0x80, 0x01, 0x04, 0x39, 0x00, 0xFF};
    vector<unsigned char> aeShutter = {0x80, 0x01, 0x04, 0x39, 0x0A, 0xFF}; // Shutter Priority Auto Exposure mode
    vector<unsigned char> aeIris = {0x80, 0x01, 0x04, 0x39, 0x0B, 0xFF}; // Iris Priority Auto Exposure mode
    vector<unsigned char> aeGain = {0x80, 0x01, 0x04, 0x39, 0x0C, 0xFF}; // Gain Priority Auto Exposure mode
    vector<unsigned char> aeBright = {0x80, 0x01, 0x04, 0x39, 0x0D, 0xFF}; //Bright Mode (Manual control)
    vector<unsigned char> aeManual = {0x80, 0x01, 0x04, 0x39, 0x03, 0xFF};
    

    
    //----digital zoom
    vector<unsigned char> dzoomON = {0x80, 0x01, 0x04, 0x06, 0x02, 0xFF}; // d zoom on
    vector<unsigned char> dzoomOFF = {0x80, 0x01, 0x04, 0x06, 0x03, 0xFF}; // d zoom off
    
    vector<unsigned char> dzoomCombined = {0x80, 0x01, 0x04, 0x36, 0x00, 0xFF}; // Optical/Digital Zoom Combined
    vector<unsigned char> dzoomSeparate = {0x80, 0x01, 0x04, 0x36, 0x01, 0xFF}; // Optical/Digital Zoom Separate
    vector<unsigned char> dzoomSTOP = {0x80, 0x01, 0x04, 0x06, 0x00, 0xFF}; //
    
    vector<unsigned char> dzoomTELE = {0x80, 0x01, 0x04, 0x06, 0x20, 0xFF}; //0x2p p=0 (Low) to 7 (High)
    vector<unsigned char> dzoomTELE0 = {0x81, 0x01, 0x04, 0x06, 0x20, 0xFF}; 
    vector<unsigned char> dzoomTELE7 = {0x81, 0x01, 0x04, 0x06, 0x27, 0xFF}; 
    vector<unsigned char> dzoomWIDE = {0x80, 0x01, 0x04, 0x06, 0x30, 0xFF}; //0x3p,  Enabled during Separate Mode
    
    vector<unsigned char> dzoomDIRECT = {0x80, 0x01, 0x04, 0x46, 0x00, 0x00, 0x00, 0x00, 0xFF}; //pq: D-Zoom Position,  Enabled during Separate Mode
    
    //------init
    vector<unsigned char> camReset = {0x80, 0x01, 0x04, 0x19, 0x03, 0xFF};
    vector<unsigned char> lensInit = {0x80, 0x01, 0x04, 0x19, 0x01, 0xFF};

    vector<unsigned char> flipOn = {0x80, 0x01, 0x04, 0x66, 0x02, 0xFF};
    vector<unsigned char> flipOff = {0x80, 0x01, 0x04, 0x66, 0x03, 0xFF};
    vector<unsigned char> mirrorOn = {0x80, 0x01, 0x04, 0x61, 0x02, 0xFF};
    vector<unsigned char> mirrorOff = {0x80, 0x01, 0x04, 0x61, 0x03, 0xFF};
    
    //-----does not work on CV345-CS, CV380-CS
    vector<unsigned char> powerON = {0x80, 0x01, 0x04, 0x00, 0x02, 0xFF};  //8x 01 04 00 02 FF
    vector<unsigned char> powerOFF = {0x80, 0x01, 0x04, 0x00, 0x03, 0xFF};
};

class ofxVisca {
public:
	ofxVisca();
	~ofxVisca();
	
    
    bool connect(int device = -1);
    void update();
    void draw(int _x, int _y);
    void keyReleased(int key);
    
    vector<vector<unsigned char>> serialMessages;    
    
    void addCommand(int _camID, vector<unsigned char> _command);
    void addCommand(int _camID, vector<unsigned char> _command, int _bytePosA, int _valueA, int _bytePosB = -1, int _valueB = -1);
    
    ofxPanel gui_visca;
     ofParameterGroup parameters_menu;

    ofParameterGroup parameters_lens;
//    ofParameter<bool> dnManual;
//    ofParameter<bool> day;
//    ofParameter<bool> night;
    
    ofParameterGroup parameters_wb;
    
    ofParameterGroup parameters_dzoom;
    
    visca_commands commands;
    
    vector<visca_item> all_viscaItems;
    
private:	
    ofSerial serial;
    bool serialActive;
    int myBaud;
    string serialID;
    
    unsigned long lastSendTime;
    bool bSerialConnected;
    int serialSendPause;
    

    
    void serialSending();
    void getSerialDevice();
    void checkGui();
    
    vector<unsigned char> intToBytes(int paramInt);
    
   
};
