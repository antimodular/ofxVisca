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
    
    parameters_menu.setName("menu");
    parameters_lens.setName("lens");
    parameters_wb.setName("whiteBalance");
    parameters_dzoom.setName("dZoom");
    
    //    all_viscaItems.push_back(visca_item());
    //all_viscaItems.emplace_back(make_shared<visca_item>());
    all_viscaItems.resize(23);
    //  all_viscaItems.emplace_back(visca_item());
    //     all_viscaItems.push_back(visca_item());

    //------cam menu
    int i = 0;
    all_viscaItems[i++].setup("menuON",parameters_menu,commands.menuON);
    all_viscaItems[i++].setup("menuOFF",parameters_menu,commands.menuOFF);
    all_viscaItems[i++].setup("menuBACK",parameters_menu,commands.menuBACK);
    all_viscaItems[i++].setup("menuUP",parameters_menu,commands.menuUP);
    all_viscaItems[i++].setup("menuDOWN",parameters_menu,commands.menuDOWN);
    all_viscaItems[i++].setup("menuLEFT",parameters_menu,commands.menuLEFT);
    all_viscaItems[i++].setup("menuRIGHT",parameters_menu,commands.menuRIGHT);
    
    //----cam lens
    all_viscaItems[i++].setup("dnManual",parameters_lens,commands.dnManual);
    all_viscaItems[i++].setup("day",parameters_lens,commands.day);
    all_viscaItems[i++].setup("night",parameters_lens,commands.night);
    
    //------cam white balance
    all_viscaItems[i++].setup("wbAuto",parameters_wb,commands.wbAuto);
    all_viscaItems[i++].setup("wbIndoor",parameters_wb,commands.wbIndoor);
    all_viscaItems[i++].setup("wbOutdoor",parameters_wb,commands.wbOutdoor);
    all_viscaItems[i++].setup("wbOnePush",parameters_wb,commands.wbOnePush);
    all_viscaItems[i++].setup("wbATW",parameters_wb,commands.wbATW);
    all_viscaItems[i++].setup("wbManual",parameters_wb,commands.wbManual);
    all_viscaItems[i++].setup("wbOnePushTrig",parameters_wb,commands.wbOnePushTrig);
    
    //------cam digital zoom    
    all_viscaItems[i++].setup("dzoomON",parameters_dzoom,commands.dzoomON);
    all_viscaItems[i++].setup("dzoomOFF",parameters_dzoom,commands.dzoomOFF);
    
    
    gui_visca.setup();
    gui_visca.setName("VISCA");
    gui_visca.setPosition(0,0);
    gui_visca.add(parameters_menu);
    gui_visca.add(parameters_lens);
    gui_visca.add(parameters_wb);
    gui_visca.add(parameters_dzoom);
    gui_visca.setDefaultHeaderBackgroundColor(ofColor(255,0,0));
    
    
    gui_visca.loadFromFile("visca_gui.xml");
    
    //-----serial
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
    
    for(int i=0; i<all_viscaItems.size(); i++){
        if( all_viscaItems[i].useCommand == true){
            all_viscaItems[i].useCommand = false;
            addCommand(1, all_viscaItems[i].item_command);
        }
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

    
}

