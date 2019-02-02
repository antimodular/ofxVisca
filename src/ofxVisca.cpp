//
//  ofxVisca.cpp
//  serialExample
//
//  Created by Stephan Schulz on 2019-02-02.
//

#include "ofxVisca.h"
#include "ofMain.h"


//https://www.epiphan.com/userguides/LUMiO12x/Content/UserGuides/PTZ/3-operation/VISCAcommands.htm

//0x80 needs to be changed to reflect actual camera ID. the id is set in the camera.
//usually it is 1 which means 0x81

vector<unsigned char> menuON = {0x80, 0x01, 0x06, 0x06, 0x02, 0xFF};
vector<unsigned char> menuOFF = {0x80, 0x01, 0x06, 0x06, 0x03, 0xFF}; //Menu off

vector<unsigned char> dnManual = {0x80, 0x01, 0x04, 0x51, 0x03, 0xFF};  // day night manual
vector<unsigned char> night = {0x80, 0x01, 0x04, 0x01, 0x02, 0xFF};
vector<unsigned char> day = {0x80, 0x01, 0x04, 0x01, 0x03, 0xFF};

//does not work on CV345-CS, CV380-CS
vector<unsigned char> powerON = {0x80, 0x01, 0x04, 0x00, 0x02, 0xFF}; 
vector<unsigned char> powerOFF = {0x80, 0x01, 0x04, 0x00, 0x03, 0xFF};

ofxVisca::ofxVisca(){
    
}
ofxVisca::~ofxVisca(){
    serial.close();
    bSerialConnected = false;
}

bool ofxVisca::connect(int _device){
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)

    if(_device == -1){
        getSerialDevice();
    }else{
        serialID = _device;
    }
    myBaud = 9600; //57600; //115200;
    serialActive = serial.setup(serialID,myBaud);
    serialSendPause = 200;
    lastSendTime = ofGetElapsedTimeMillis();
    
}

void ofxVisca::draw(int _x, int _y){
    ofPushMatrix();
    ofTranslate(_x, _y);
    int temp_y = 0;
    ofDrawBitmapString("key m | show menu", 0, temp_y+=15);
     ofDrawBitmapString("key n | hide menu", 0, temp_y+=15);
     ofDrawBitmapString("key m | show menu", 0, temp_y+=15);
     ofDrawBitmapString("key 2 | IR pass filter", 0, temp_y+=15);
     ofDrawBitmapString("key 3 | IR cut filter", 0, temp_y+=15);
    ofPopMatrix();
}
void ofxVisca::update(){
    if(ofGetElapsedTimeMillis() - lastSendTime > serialSendPause && serialMessages.size() > 0){
        lastSendTime = ofGetElapsedTimeMillis();
        serialSending();
    }  
    
    /*
    int myByte = serial.readByte();

    if ( myByte == OF_SERIAL_NO_DATA ){
//        printf("no data was read");
    } else if ( myByte == OF_SERIAL_ERROR ){
        printf("an error occurred");
    } else {
        printf("myByte is %d", myByte);
    }
     */
}

void ofxVisca::addCommand(int _camID, vector<unsigned char> _command){
    
    ofLog()<<"first byte as hex "<<ofToHex(_command[0]);
    
    int temp_id = int(_command[0]) + _camID;
    ofLog()<<"first byte as int "<<int(_command[0])<<" +1 "<<temp_id;
    
    _command[0] = (unsigned char) temp_id;
    ofLog()<<"new byte as hex "<<ofToHex(_command[0]);
    
    serialMessages.push_back(_command);
}
void ofxVisca::keyReleased(int key){ 
    
    if(key == 'm'){
        addCommand(1,menuON);
        ofLog()<<"menuON";
    }
    if(key == 'n'){
         addCommand(1,menuOFF);
        ofLog()<<"menuOFF";
    }
    if(key == '1'){
         addCommand(1,dnManual);
        ofLog()<<"dnManual";
    }
    if(key == '2'){
        addCommand(1,night);
        ofLog()<<"night";
    }
    if(key == '3'){
        addCommand(1,day);
        ofLog()<<"day";
    }
    if(key == '0'){
        addCommand(1,powerON);
        ofLog()<<"powerON";
    }
    if(key == '9'){
        addCommand(1,powerOFF);
        ofLog()<<"powerOFF";
    }
}

void ofxVisca::serialSending(){
    
    ofLog()<<"serialSending() ";
    
    //    if(serialMessages.size() > 0){
    if(serialMessages.size() > 0){        
        //        unsigned char* sendCmd = (unsigned char*)  serialMessages[0].c_str();
        
        unsigned char* sendCmd = &serialMessages[0][0];
        cout<<"serialMessages[0]: ";
        for(int i=0; i<serialMessages[0].size();i++){
            cout<<ofToHex(serialMessages[0][i])<<" , ";
        }
        cout<<endl;
        //        cout<<"*sendCmd "<<ofToHex(serialMessages[0])<<" : "<<serialMessages[0]<<endl;
        
        //        unsigned char* sendCmd = menuOFF;
        
        //        int tempLength = serialMessages[0].length();
        //  int tempLength = sendCmd.length();      
        
        //    const char packet[] = {"\xBE\xEF\x03\x06\x00\xf3\x93\x01\x00\x05\x24\x00\x00"};
        //    client.sendRawBytes(packet, 13); 
        
        /*
         // convert the LED image to raw data  
         unsigned char * ledData = new unsigned char[((int)ledImage[i].getWidth() * (int)ledImage[i].getHeight() * 3) + 3];  
         image2data(ledImage[i], ledData, ledLayout[i]);  
         if (i == 0) {  
         ledData[0] = '*';  // first Teensy is the frame sync master  
         int usec = (int)((1000000.0 / framerate) * 0.75);  
         ledData[1] = (unsigned char)(usec);   // request the frame sync pulse  
         ledData[2] = (unsigned char)(usec >> 8); // at 75% of the frame time  
         } else {  
         ledData[0] = '%';  // others sync to the master board  
         ledData[1] = 0;  
         ledData[2] = 0;  
         }  
         
         // send the raw data to the LEDs  :-)  
         ledSerial[i].writeBytes(ledData, 3);  
         ledSerial[i].drain();  
         */
        
        if(serialActive){
            
            //bSendSomething = true;
            //serialTxRxTimer = ofGetElapsedTimeMillis();
            
            serial.writeBytes(sendCmd, sizeof(sendCmd));
            //            serial.writeBytes(sendCmd, tempLength);
            
            //            for(int i=0; i<6; i++){
            //                 serial.writeByte(menuON[i]);
            //            }
            cout<<"*sendCmd "<<ofToHex(sendCmd)<<", size "<<sizeof(sendCmd)<<endl;
            
//            ofLog()<<"*sendCmd "<<sendCmd;
        }
        
        //        serialMessages.erase(serialMessages.begin());
        serialMessages.erase(serialMessages.begin());
        
    }
    
}

void ofxVisca::getSerialDevice(){
    
    vector<ofSerialDeviceInfo> serialDevices = serial.getDeviceList();
    
    string deviceLine;
    for(int i=0; i<serialDevices.size();i++){
        
        deviceLine = serialDevices[i].getDeviceName().c_str();
        
        //cout<<serialDevices[i].getDeviceName().c_str()<<endl;
        
        if(deviceLine.substr(0,12) == "tty.usbmodem"){
            serialID = "/dev/" +deviceLine;
            cout<<"serial = "<<serialID<<endl;
        }else if(deviceLine.substr(0,13) == "tty.usbserial"){
            serialID = "/dev/" +deviceLine;
            cout<<"serial = "<<serialID<<endl;
        }
    }
    
}

