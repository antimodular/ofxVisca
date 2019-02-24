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
    
    if(ofFile::doesFileExist("visca_GuiSettings.xml") == false){
        XML.saveFile("visca_GuiSettings.xml");
    }
    XML.loadFile("visca_GuiSettings.xml");
    
    if(!XML.tagExists("SERIAL:BAUD")){
        ofLog()<<"XML SERIAL:BAUD does not exists";
        XML.addValue("SERIAL:BAUD",9600);
    }
    myBaud = XML.getValue("SERIAL:BAUD",9600);
  
    ofLog()<<"myBaud "<<myBaud;
    
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
//    myBaud = 9600; //57600; //115200;
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
//    buttonGroup[m++].setup("MenuOn",1,{commands.menuON},10, 60 , mW, mH);
    buttonGroup[m++].setup("MenuOn",1,{commands.menuON},XML);
    buttonGroup[m++].setup("MenuOff",1,{commands.menuOFF},XML);
    buttonGroup[m++].setup("MenuBack",1,{commands.menuBACK},XML);
    
    buttonGroup[m++].setup("MenuUp",1,{commands.menuUP},XML);
    buttonGroup[m++].setup("MenuDown",1,{commands.menuDOWN},XML);
   
    buttonGroup[m++].setup("MenuLeft",1,{commands.menuLEFT},XML);
    buttonGroup[m++].setup("MenuRight",1,{commands.menuRIGHT},XML);

    buttonGroup[m++].setup("camReset",1,{commands.camReset},XML);
    buttonGroup[m++].setup("lensInit",1,{commands.lensInit},XML);
    buttonGroup[m++].setup("saveCustom",1,{commands.saveCustom},XML);
    
    buttonGroup[m++].setup("flipOn",1,{commands.flipOn},XML);
    buttonGroup[m++].setup("flipOff",1,{commands.flipOff},XML);
    buttonGroup[m++].setup("mirrorOn",1,{commands.mirrorOn},XML);
    buttonGroup[m++].setup("mirrorOff",1,{commands.mirrorOff},XML);
    
    buttonGroup[m++].setup("redDown",1,{commands.wbManual,commands.wbRGain_Down,commands.saveCustom},XML);
    buttonGroup[m++].setup("redUp",1,{commands.wbManual,commands.wbRGain_Down,commands.saveCustom},XML); 
    buttonGroup[m++].setup("blueDown",1,{commands.wbManual,commands.wbBGain_Down,commands.saveCustom},XML);
    buttonGroup[m++].setup("blueUp",1,{commands.wbManual,commands.wbBGain_Down,commands.saveCustom},XML);
    
    int temp_x = 0; //temp_x+=100
    buttonGroup[m++].setup("wbAuto",1,{commands.wbAuto},XML);
    buttonGroup[m++].setup("wbIndoor",1,{commands.wbIndoor},XML);
    buttonGroup[m++].setup("wbOutdoor",1,{commands.wbOutdoor},XML);
    buttonGroup[m++].setup("wbOnePush",1,{commands.wbOnePush},XML);
    buttonGroup[m++].setup("wbATW",1,{commands.wbATW},XML);
    buttonGroup[m++].setup("wbPushTrig",1,{commands.wbOnePushTrig},XML);
    buttonGroup[m++].setup("wbManual",1,{commands.wbManual},XML);
    
    bEditMode = false;
    
    bShowGui = true;
    for(auto & button: buttonGroup){
        button.showButton(bShowGui);
    }
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

    // draw recent serial messages    
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
    
    if(bEditMode) ofDrawBitmapStringHighlight("GUI edit enabled", ofGetWidth()/2, ofGetHeight()/2);
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
    
    if(key =='g'){
        bShowGui = !bShowGui;
        for(auto & button: buttonGroup){
            button.showButton(bShowGui);
        }
    }
    if(key =='e'){
        
        bEditMode = !bEditMode;
        
        if(bEditMode == false){
            for(auto & button: buttonGroup){
                button.bEditMode = false;
                button.saveButton(XML);
            }
            XML.saveFile("visca_GuiSettings.xml");
        }else{
            for(auto & button: buttonGroup){
                button.bEditMode = true;
            }
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

