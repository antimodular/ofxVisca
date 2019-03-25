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
    serialSendPause = 50; //300;
    lastSendTime = ofGetElapsedTimeMillis();
    
//    vector<unsigned char> _command
    buttonGroup.resize(56); 
    int mX = 300;
    int mY = 300;
    int mW = 100;
    int mH = 50;
    int m = 0;
//    buttonGroup[m++].setup("MenuOn",1,XML,{commands.menuON},10, 60 , mW, mH);
    buttonGroup[m++].setup("MenuOn",1,XML,{commands.menuON});
    buttonGroup[m++].setup("MenuOff",1,XML,{commands.menuOFF});
    buttonGroup[m++].setup("MenuBack",1,XML,{commands.menuBACK});
    
    buttonGroup[m++].setup("MenuUp",1,XML,{commands.menuUP});
    buttonGroup[m++].setup("MenuDown",1,XML,{commands.menuDOWN});
   
    buttonGroup[m++].setup("MenuLeft",1,XML,{commands.menuLEFT});
    buttonGroup[m++].setup("MenuRight",1,XML,{commands.menuRIGHT});

    buttonGroup[m++].setup("camReset",1,XML,{commands.camReset});
    buttonGroup[m++].setup("lensInit",1,XML,{commands.lensInit});
    buttonGroup[m++].setup("saveCustom",1,XML,{commands.saveCustom});
    buttonGroup[m++].setup("recallCustom",1,XML,{commands.recallCustom});
    
    buttonGroup[m++].setup("flipOn",1,XML,{commands.flipOn});
    buttonGroup[m++].setup("flipOff",1,XML,{commands.flipOff});
    buttonGroup[m++].setup("mirrorOn",1,XML,{commands.mirrorOn});
    buttonGroup[m++].setup("mirrorOff",1,XML,{commands.mirrorOff});
    
//    buttonGroup[m++].setup("redDown",1,XML,{commands.wbManual,commands.wbRGain_Down,commands.saveCustom});
//    buttonGroup[m++].setup("redUp",1,XML,{commands.wbManual,commands.wbRGain_Down,commands.saveCustom}); 
//    buttonGroup[m++].setup("blueDown",1,XML,{commands.wbManual,commands.wbBGain_Down,commands.saveCustom});
//    buttonGroup[m++].setup("blueUp",1,XML,{commands.wbManual,commands.wbBGain_Down,commands.saveCustom});

    buttonGroup[m++].setup("redDown",1,XML,{commands.wbRGain_Down});
    buttonGroup[m++].setup("redUp",1,XML,{commands.wbRGain_Down}); 
    buttonGroup[m++].setup("blueDown",1,XML,{commands.wbBGain_Down});
    buttonGroup[m++].setup("blueUp",1,XML,{commands.wbBGain_Down});

    
    int temp_x = 0; //temp_x+=100
    buttonGroup[m++].setup("wbAuto",1,XML,{commands.wbAuto});
    buttonGroup[m++].setup("wbIndoor",1,XML,{commands.wbIndoor});
    buttonGroup[m++].setup("wbOutdoor",1,XML,{commands.wbOutdoor});
    buttonGroup[m++].setup("wbOnePush",1,XML,{commands.wbOnePush});
    buttonGroup[m++].setup("wbATW",1,XML,{commands.wbATW});
    buttonGroup[m++].setup("wbPushTrig",1,XML,{commands.wbOnePushTrig});
    buttonGroup[m++].setup("wbManual",1,XML,{commands.wbManual});
   
//    buttonGroup[m++].setup("brightDown",1,XML,{commands.brightnessDown},-1,commands.brightnessSpecs,&brightnessValue);
//    buttonGroup[m++].setup("brightUp",1,XML,{commands.brightnessUp},1,commands.brightnessSpecs,&brightnessValue);
       buttonGroup[m++].setup("brightOn",1,XML,{commands.brightnessOn});
      buttonGroup[m++].setup("brightOff",1,XML,{commands.brightnessOff});
    buttonGroup[m++].setup("brightDown",1,XML,{commands.brightnessDown});
    buttonGroup[m++].setup("brightUp",1,XML,{commands.brightnessUp});
    
    buttonGroup[m++].setup("sharpDown",1,XML,{commands.sharpDown});
    buttonGroup[m++].setup("sharpUp",1,XML,{commands.sharpUp});
    buttonGroup[m++].setup("satDown",1,XML,{commands.satDown},-1,commands.satSpecs,&satValue);
    buttonGroup[m++].setup("satUp",1,XML,{commands.satUp},1,commands.satSpecs,&satValue);
    
    buttonGroup[m++].setup("contrastDown",1,XML,{commands.contrastDown}, -1,commands.contrastSpecs,&contrastValue);
    buttonGroup[m++].setup("contrastUp",1,XML,{commands.contrastUp},  1,commands.contrastSpecs,&contrastValue);
    buttonGroup[m++].setup("gammaDown",1,XML,{commands.gammaDown}, -1,commands.gammaSpecs,&gammaValue);
    buttonGroup[m++].setup("gammaUp",1,XML,{commands.gammaUp}, 1,commands.gammaSpecs,&gammaValue);
    
    //-----video format
    buttonGroup[m++].setup("HDp24",1,XML,{commands.format1080p24});
    buttonGroup[m++].setup("HDp25",1,XML,{commands.format1080p25});
    buttonGroup[m++].setup("HDp30",1,XML,{commands.format1080p30});
    buttonGroup[m++].setup("HDp50",1,XML,{commands.format1080p50});
    buttonGroup[m++].setup("HDp60",1,XML,{commands.format1080p60});
    buttonGroup[m++].setup("UHDp25",1,XML,{commands.formatUHDp25});
     buttonGroup[m++].setup("UHDp30",1,XML,{commands.formatUHDp30});
    buttonGroup[m++].setup("fourKp30",1,XML,{commands.format4kp30});
    buttonGroup[m++].setup("fourKp60",1,XML,{commands.format4kp60});
     buttonGroup[m++].setup("half720p50",1,XML,{commands.format720p50});
      buttonGroup[m++].setup("half720p60",1,XML,{commands.format720p60});
    
      buttonGroup[m++].setup("colorYUV",1,XML,{commands.colorYUV});
      buttonGroup[m++].setup("colorRGB",1,XML,{commands.colorRGB});
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
                if(button.getLabelString() == "gammaDown" || button.getLabelString() == "gammaUp"){
                    ofLog()<<"gammaValue "<<gammaValue;
                     ofLog()<<"button.value "<<*button.value;
                }
            }
        }
    }
    
    // check for data
    /*
    if ( serial.available() > 0 )
    {
        ofLog()<<"serial.available() > 0";
        // we want to read 8 bytes
        int bytesRequired = 8;
        unsigned char bytes[bytesRequired];
        int bytesRemaining = bytesRequired;
        // loop until we've read everything
        while ( bytesRemaining > 0 )
        {
            
            // try to read - note offset into the bytes[] array, this is so
            // that we don't overwrite the bytes we already have
            int bytesArrayOffset = bytesRequired - bytesRemaining;
            int result = serial.readBytes( &bytes[bytesArrayOffset],
                                          bytesRemaining );
            
            // check for error code
            if ( result == OF_SERIAL_ERROR )
            {
                // something bad happened
                ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
                // bail out
                break;
            }
            else if ( result == OF_SERIAL_NO_DATA )
            {
                // nothing was read, try again
            }
            else
            {
                // we read some data!
                bytesRemaining -= result;
            }
        }
    }
    */
//    if(serial.available() > 8) {
//        serial.readBytes(buffer, 8);
//    }
//    
//    int myByte = serial.readByte();
//    
//    if ( myByte == OF_SERIAL_NO_DATA ){
//        //        printf("no data was read");
//    } else if ( myByte == OF_SERIAL_ERROR ){
//        printf("an error occurred");
//    } else {
//        printf("myByte is %d", myByte);
//    }
     
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
    
    if(bEditMode) ofDrawBitmapStringHighlight("GUI edit enabled", 10, 35);
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

            if(i != 0 )msgString += ",";
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
        timers[currentMsgString] = ofGetElapsedTimef() + 20.0f; //5.0f;
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

