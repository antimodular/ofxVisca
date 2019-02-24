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
    all_viscaItems[i++].setup("wbOnePushTrig",parameters_wb,commands.wbOnePushTrig);
    all_viscaItems[i++].setup("wbManual",parameters_wb,commands.wbManual);

    all_viscaItems[i++].setup("wbRGain_Reset",parameters_wb,commands.wbRGain_Reset);
    all_viscaItems[i++].setup("wbRGain_Up",parameters_wb,commands.wbRGain_Up);
    all_viscaItems[i++].setup("wbRGain_Down",parameters_wb,commands.wbRGain_Down);
    all_viscaItems[i++].setup("wbRGain_Direct",parameters_wb,commands.wbRGain_Direct,6,7,1,2); // byte pos A, pos B, silder amt, silder type byte or int

//    all_viscaItems[i++].setup("wbBGain_Reset",parameters_wb,commands.wbBGain_Reset);
//    all_viscaItems[i++].setup("wbBGain_Up",parameters_wb,commands.wbBGain_Up);
//    all_viscaItems[i++].setup("wbBGain_Down",parameters_wb,commands.wbBGain_Down);
//    all_viscaItems[i++].setup("wbBGain_Direct",parameters_wb,commands.wbBGain_Direct,1,2);

    
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
    
//    vector<unsigned char> _command
    buttonGroup.resize(25); 
    int mX = 300;
    int mY = 300;
    int mW = 100;
    int mH = 50;
    int m = 0;
    buttonGroup[m++].setup("MenuOn",1,{commands.menuON},10, 60 , mW, mH);
    buttonGroup[m++].setup("MenuOff",1,{commands.menuOFF},10, 120, mW, mH);
    buttonGroup[m++].setup("MenuBack",1,{commands.menuBACK},10, 180, mW, mH);
    
    buttonGroup[m++].setup("MenuUp",1,{commands.menuUP},200, 60, mW, mH);
    buttonGroup[m++].setup("MenuDown",1,{commands.menuDOWN},200, 180, mW, mH);
   
    buttonGroup[m++].setup("MenuLeft",1,{commands.menuLEFT},150, 120, mW, mH);
    buttonGroup[m++].setup("MenuRight",1,{commands.menuRIGHT}, 260, 120, mW, mH);

    buttonGroup[m++].setup("camReset",1,{commands.camReset},10, 840, mW, mH);
    buttonGroup[m++].setup("lensInit",1,{commands.lensInit},120, 840, mW, mH);
    buttonGroup[m++].setup("saveCustom",1,{commands.saveCustom},510, 270, mW, mH);
    
    buttonGroup[m++].setup("flipOn",1,{commands.flipOn},400, 60, mW, mH);
    buttonGroup[m++].setup("flipOff",1,{commands.flipOff},400, 120, mW, mH);
    buttonGroup[m++].setup("mirrorOn",1,{commands.mirrorOn},510, 60, mW, mH);
    buttonGroup[m++].setup("mirrorOff",1,{commands.mirrorOff},510, 120, mW, mH);
    
    buttonGroup[m++].setup("redDown",1,{commands.wbManual,commands.wbRGain_Down,commands.saveCustom},120, 320, mW, mH); 
    buttonGroup[m++].setup("redUp",1,{commands.wbManual,commands.wbRGain_Down,commands.saveCustom},120, 370, mW, mH); 
    buttonGroup[m++].setup("blueDown",1,{commands.wbManual,commands.wbBGain_Down,commands.saveCustom},120, 420, mW, mH); 
    buttonGroup[m++].setup("blueUp",1,{commands.wbManual,commands.wbBGain_Down,commands.saveCustom},120, 470, mW, mH); 
    
    int temp_x = 0; //temp_x+=100
    buttonGroup[m++].setup("wbAuto",1,{commands.wbAuto},10, 270, mW, mH);
    buttonGroup[m++].setup("wbIndoor",1,{commands.wbIndoor},10, 320, mW, mH);
    buttonGroup[m++].setup("wbOutdoor",1,{commands.wbOutdoor},10, 370, mW, mH);
    buttonGroup[m++].setup("wbOnePush",1,{commands.wbOnePush},10, 420, mW, mH);
    buttonGroup[m++].setup("wbATW",1,{commands.wbATW},10, 470, mW, mH);
    buttonGroup[m++].setup("wbPushTrig",1,{commands.wbOnePushTrig},10, 520, mW, mH);
    buttonGroup[m++].setup("wbManual",1,{commands.wbManual},120, 270, mW, mH);
    
}


void ofxVisca::update(){
    
    checkGui();
    
    if(ofGetElapsedTimeMillis() - lastSendTime > serialSendPause && serialMessages.size() > 0){
        lastSendTime = ofGetElapsedTimeMillis();
        serialSending();
    }  
    
    //      for(int i=0; i<buttonGroup.size(); i++){
    for(auto & button: buttonGroup){
        if(button.bUseCommand == true){
            button.bUseCommand = false;
            for(int i=0; i<button.commands.size(); i++){
                addCommand(button.camID, button.commands[i]);
            }
        }
    }
    
    /*
    for(int i=0; i<all_viscaItems.size(); i++){
        if( all_viscaItems[i].useCommand == true){
            all_viscaItems[i].useCommand = false;
            
            if(all_viscaItems[i].sliderAmount == -1){
                addCommand(1, all_viscaItems[i].item_command);
            }else{
                int val = all_viscaItems[i].bByteSlider0.get();
                vector<unsigned char> allBytes = intToBytes(val);
                ofLog()<<"allBytes[0] "<<int(allBytes[0])<<"allBytes[1] "<<int(allBytes[1])<<"allBytes[2] "<<int(allBytes[2])<<"allBytes[3] "<<int(allBytes[3]);
               int valA = int(allBytes[3]);
                int valB = int(allBytes[2]);
                
                ofLog()<<"new slider value from "<<all_viscaItems[i].item_name<<" val "<<val<<" valA "<<valA<<" valB "<<valB;
                ofLog()<<"all_viscaItems[i].bytePosA "<<all_viscaItems[i].bytePosA<<" all_viscaItems[i].bytePosB "<<all_viscaItems[i].bytePosB;
                
                addCommand(1, all_viscaItems[i].item_command, all_viscaItems[i].bytePosA, valA ,all_viscaItems[i].bytePosB, valB);
            }
        }
    }
     */
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
    
    // draw recent unrecognized messages
    
    if(serialID == ""){
        ofDrawBitmapStringHighlight("serial device not found", 10, 15);
    }else{
        ofDrawBitmapStringHighlight("serialID: "+serialID + " at baud rate "+ofToString(myBaud), 10, 15);
    }
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() - 250, 15);
    ofDrawBitmapStringHighlight("recent serial messages", 0, 0);
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapStringHighlight(msgStrings[i], 0, 40 + 15 * i);
    }
    ofPopMatrix();
       
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msgStrings[i] = "";
        }
    }
}

void ofxVisca::addCommand(int _camID, vector<unsigned char> _command){
    //this is the command and only adds the camera ID
    
    ofLog()<<"first byte as hex "<<ofToHex(_command[0]);
    
    int temp_id = int(_command[0]) + _camID;
    ofLog()<<"first byte as int "<<int(_command[0])<<" +1 "<<temp_id;
    
    _command[0] = (unsigned char) temp_id;
    ofLog()<<"new byte as hex "<<ofToHex(_command[0]);
    
    serialMessages.push_back(_command);
}

void ofxVisca::addCommand(int _camID, vector<unsigned char> _command, int _bytePosA, int _valueA, int _bytePosB, int _valueB){
    
    ofLog()<<"byteA# "<<ofToString(_bytePosA)<<" as hex "<<ofToHex(_command[_bytePosA]);
    ofLog()<<"byteB# "<<ofToString(_bytePosB)<<" as hex "<<ofToHex(_command[_bytePosB]);

    //_command[_bytePosA] is the default value for this command. probably 0x00
    
    //we construct the new value. 
    int temp_id_A = int(_command[_bytePosA]) + _valueA;
    ofLog()<<"byte# "<<ofToString(_bytePosA)<<" as int "<<int(_command[_bytePosA])<<" +1 "<<temp_id_A;

    //since default is 0x00 it's like setting it to the new value
    _command[_bytePosA] = (unsigned char) temp_id_A;
    ofLog()<<"new byteA as hex "<<ofToHex(_command[_bytePosA]);
  
    int temp_id_B = int(_command[_bytePosB]) + _valueB;
    _command[_bytePosB] = (unsigned char) temp_id_B;
    ofLog()<<"new byteB as hex "<<ofToHex(_command[_bytePosB]);

    //    serialMessages.push_back(_command);
    addCommand(_camID,_command);
}

void ofxVisca::keyReleased(int key){ 
    
    if(key =='e'){
         for(auto & button: buttonGroup){
        button.bEditMode = !button.bEditMode;
         }
    }
 
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
        
        string msgString;
        unsigned char* sendCmd = &serialMessages[0][0];
        cout<<"serialMessages[0]: ";
        for(int i=0; i<serialMessages[0].size();i++){
            cout<<ofToHex(serialMessages[0][i])<<" , ";

            if(i != 0 )msgString += " , ";
            msgString += ofToHex(serialMessages[0][i]);
           
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
        
        // add to the list of strings to display
        msgStrings[currentMsgString] = msgString;
        timers[currentMsgString] = ofGetElapsedTimef() + 5.0f;
        currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;
        
        // clear the next line
        msgStrings[currentMsgString] = "";
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

vector<unsigned char> ofxVisca::intToBytes(int paramInt)
{
    vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
        arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
}

void ofxVisca::checkGui(){

    
}

