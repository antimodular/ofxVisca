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
    
    parameters_menu.setName("menu");
    parameters_menu.add(menuON.set("menuON",false));
    parameters_menu.add(menuOFF.set("menuOFF",false));
    parameters_menu.add(menuBACK.set("menuBACK",false));
    parameters_menu.add(menuUP.set("menuUP",false));
    parameters_menu.add(menuDOWN.set("menuDOWN",false));
    parameters_menu.add(menuLEFT.set("menuLEFT",false));
    parameters_menu.add(menuRIGHT.set("menuRIGHT",false));
    
    parameters_lens.setName("lens");
    parameters_lens.add(dnManual.set("dnManual",false));
    parameters_lens.add(day.set("day",false));
    parameters_lens.add(night.set("night",false));
    
    parameters_dzoom.setName("dZoom");
    
    gui_visca.setup();
    gui_visca.setName("VISCA");
    gui_visca.setPosition(0,0);
    gui_visca.add(parameters_menu);
    gui_visca.add(parameters_lens);
    gui_visca.add(parameters_dzoom);
    gui_visca.setDefaultHeaderBackgroundColor(ofColor(255,0,0));


    gui_visca.loadFromFile("visca_gui.xml");
}

void ofxVisca::draw(int _x, int _y){
//    ofPushMatrix();
//    ofTranslate(_x, _y);
//    int temp_y = 0;
//    ofDrawBitmapString("key m | show menu", 0, temp_y+=15);
//    ofDrawBitmapString("key n | hide menu", 0, temp_y+=15);
//    ofDrawBitmapString("key m | show menu", 0, temp_y+=15);
//    ofDrawBitmapString("key 2 | IR pass filter", 0, temp_y+=15);
//    ofDrawBitmapString("key 3 | IR cut filter", 0, temp_y+=15);
//    ofPopMatrix();
    
    
}

void ofxVisca::update(){
    
    checkGui();
    
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

void ofxVisca::addCommand(int _camID, vector<unsigned char> _command, int _bytePosA, int _valueA, int _bytePosB, int _valueB){
    
    ofLog()<<"byte# "<<ofToString(_bytePosA)<<" as hex "<<ofToHex(_command[_bytePosA]);
    
    int temp_id = int(_command[_bytePosA]) + _valueA;
    ofLog()<<"byte# "<<ofToString(_bytePosA)<<" as int "<<int(_command[_bytePosA])<<" +1 "<<temp_id;
    
    _command[_bytePosA] = (unsigned char) temp_id;
    ofLog()<<"new byte as hex "<<ofToHex(_command[_bytePosA]);
    
    //    serialMessages.push_back(_command);
    addCommand(_camID,_command);
}

void ofxVisca::keyReleased(int key){ 
    
//    if(key == 'm'){
//        addCommand(1,commands.menuON);
//        ofLog()<<"menuON";
//    }
//    if(key == 'n'){
//        addCommand(1,commands.menuOFF);
//        ofLog()<<"menuOFF";
//    }
//    if(key == '1'){
//        addCommand(1,commands.dnManual);
//        ofLog()<<"dnManual";
//    }
//    if(key == '2'){
//        addCommand(1,commands.night);
//        ofLog()<<"night";
//    }
//    if(key == '3'){
//        addCommand(1,commands.day);
//        ofLog()<<"day";
//    }
    
    if(key == '0'){
        addCommand(1,commands.dzoomON);
        ofLog()<<"dzoomON";
    }
    if(key == '9'){
        addCommand(1,commands.dzoomOFF);
        ofLog()<<"dzoomOFF";
    }
    if(key == '8'){
        addCommand(1,commands.dzoomTELE, 4, 7);
        addCommand(1,commands.dzoomSTOP);
        //         serialMessages.push_back(dzoomTELE7);
        ofLog()<<"dzoomTELE 7";
    }
    if(key == '7'){
        addCommand(1,commands.dzoomTELE, 4, 1);
        addCommand(1,commands.dzoomSTOP);
        //         serialMessages.push_back(dzoomTELE0);
        ofLog()<<"dzoomTELE 0";
    }
    
    //------did not work
    /*
     if(key == '0'){
     addCommand(1,powerON);
     ofLog()<<"powerON";
     }
     if(key == '9'){
     addCommand(1,powerOFF);
     ofLog()<<"powerOFF";
     }
     */
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

void ofxVisca::checkGui(){
    if(menuON){
        menuON = false;
        addCommand(1,commands.menuON);
    }
    if(menuOFF){
        menuOFF = false;
        addCommand(1,commands.menuOFF);
    }
    if(menuDOWN){
        menuDOWN = false;
        addCommand(1,commands.menuDOWN);
    }
    if(menuUP){
        menuUP = false;
        addCommand(1,commands.menuUP);
    }

    if(dnManual){
        dnManual = false;
        addCommand(1,commands.dnManual);
    }
    if(day){
        day = false;
        addCommand(1,commands.day);
    }
    if(night){
        night = false;
        addCommand(1,commands.night);
    }
    if(menuUP){
        menuUP = false;
        addCommand(1,commands.menuUP);
    }
}

