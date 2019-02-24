//
//  viscaCommands.h
//  example_ofxVisca
//
//  Created by Stephan Schulz on 2019-02-23.
//

#ifndef viscaCommands_h
#define viscaCommands_h

class visca_commands {
    
public:
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
    
protected:
    
private:
};
#endif /* viscaCommands_h */
