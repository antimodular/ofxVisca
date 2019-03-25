//
//  viscaButton.h
//  example_ofxVisca
//
//  Created by Stephan Schulz on 2019-02-23.
//  inspired by noise1dOctaveExample gui

#ifndef viscaButton_h
#define viscaButton_h

class viscaButton{
    
public:
    
    bool bUseCommand;
    vector<vector<unsigned char>> commands;
    int camID;
    bool bEditMode;
    bool bSetPositon;
    
    int* value;
    int stepValue;
    
    int defaultValue;
    int minValue;
    int maxValue;
    int bytePosA;
    int bytePosB;
    //----------------------------------------------------
    viscaButton(){
        bWasSetup = false;
        value = NULL;
    }
    
    //----------------------------------------------------
    ~viscaButton(){
        clear();
    }
    
    void clear(){
        if(bWasSetup){
            ofRemoveListener(ofEvents().draw, this, &viscaButton::draw);
            ofRemoveListener(ofEvents().mouseMoved, this, &viscaButton::mouseMoved);
            ofRemoveListener(ofEvents().mousePressed, this, &viscaButton::mousePressed);
            ofRemoveListener(ofEvents().mouseReleased, this, &viscaButton::mouseReleased);
            ofRemoveListener(ofEvents().mouseDragged, this, &viscaButton::mouseDragged);
        }
        bWasSetup = false;
    }
    void enable(){
        if(!bWasSetup){
            ofAddListener(ofEvents().draw, this, &viscaButton::draw);
            ofAddListener(ofEvents().mouseMoved, this, &viscaButton::mouseMoved);
            ofAddListener(ofEvents().mousePressed, this, &viscaButton::mousePressed);
            ofAddListener(ofEvents().mouseReleased, this, &viscaButton::mouseReleased);
            ofAddListener(ofEvents().mouseDragged, this, &viscaButton::mouseDragged);
            bWasSetup = true;
        }
    }
    void setup(string _label,int _camID,  ofxXmlSettings & XML, vector<vector<unsigned char>> _commands, int _stepValue = 0, vector<int> _specs = vector<int>(),  int* _value = 0){
        
        labelString = _label; 
        
        if(!XML.tagExists("BUTTONS:"+labelString+":x")){
            ofLog()<<_label<<" XML BUTTONS does not exists";
        }
        
        x = XML.getValue("BUTTONS:"+labelString+":x", ofRandom(ofGetWidth()-200));
        y = XML.getValue("BUTTONS:"+labelString+":y", ofRandom(ofGetWidth()-200));
        width = XML.getValue("BUTTONS:"+labelString+":w", 100);
        height = XML.getValue("BUTTONS:"+labelString+":h", 50);
        
        setup(_label, _camID, _commands, x,y,width,height, _stepValue, _specs, _value);
    }

    void setup(string _label,int _camID, vector<vector<unsigned char>> _commands, float _x, float _y, float _w, float _h,int _stepValue = 0 , vector<int> _specs = vector<int>(),  int* _value = 0){
        
        labelString = _label; 
        
        x = _x;
        y = _y;
        width = _w;
        height = _h;
        camID = _camID;
        
         stepValue = _stepValue;
        
        if(_specs.size() > 0){
            defaultValue = _specs[0];
            minValue = _specs[1];
            maxValue = _specs[2];
             bytePosA = _specs[3];
             bytePosB = _specs[4];
        }
        value = _value;
        //https://www.tutorialspoint.com/cplusplus/cpp_pointers.htm
        if(labelString == "gammaDown" || labelString == "gammaUp"){
            ofLog()<<" value "<<*value<<" _value "<<*_value;
            cout<<" value "<<*value<<" _value "<<*_value<<endl;
           
        }
        bHasFocus = false;
        bInside = false;
        bUseCommand = false;
        
        bEditMode = false;
        bSetPositon = false;
        
        box.set(x,y, width, height); 

        commands = _commands;
        
        clickColor = ofColor(148,17,57,255); //ofColor(64,64,266, 150)
        hoverColor = ofColor(148,17,57,200);
        normalColor =  ofColor(204,63,17,255); // ofColor(148,17,57,150);
        textColor = ofColor(64,64,64,200);
        enable();
    }
    
    void showButton(bool _show){
        if(_show == true){
            enable();
        }else{
            clear();
        }
    }
    
    void draw(ofEventArgs& event){
        
        ofEnableAlphaBlending();
        ofPushMatrix();
        ofTranslate(x,y,0);
        
        // draw box with outline
        ofFill();
        if(bInside){
            if (bHasFocus){
                ofSetColor(clickColor); //sliderAlpha); 
            }else{
                ofSetColor(hoverColor); 
            }
        }else{
            ofSetColor(normalColor);
        }
        ofDrawRectangle(0,0, width,height); 
        
        ofNoFill();
        ofSetLineWidth(1.0);
        ofSetColor(clickColor);
        ofDrawRectangle(0,0, width,height); 
        
        // draw focus value 
//        if (bHasFocus){
//            ofSetColor(0); 
//        } else {
//            ofSetColor(128); 
//        }
        
        //            ofDrawBitmapString( ofToString(getValue(),numberDisplayPrecision), width+5,height/2 + 4);
        
        ofSetColor(textColor);
        float labelStringWidth = labelString.size();
        //        ofDrawBitmapString( labelString, 0-labelStringWidth*8-5, height/2 + 4); 
        ofDrawBitmapString( labelString, width/2 - ((labelStringWidth*8-5) / 2), height/2 + 4); 
        
        ofPopMatrix();
        ofSetLineWidth(1.0);
        ofDisableAlphaBlending();
    }
    
    void setLabelString (string str){
        labelString = str;
    }
    string getLabelString (){
        return labelString;
    }
    void mousePressed(ofMouseEventArgs& event){
        bHasFocus = false;
        if (box.inside(event.x, event.y)){
            if(bEditMode){
                bSetPositon = true;
            } else {
                bHasFocus = true;
            //            updatePercentFromMouse (event.x, event.y); 
            ofLog()<<"mousePressed "<<labelString;
            }
        }
    }
    void mouseReleased(ofMouseEventArgs& event){
        
        if (bHasFocus){
            if (box.inside(event.x, event.y)){
                if(bEditMode == false){
                    //                updatePercentFromMouse (event.x, event.y); 
                    ofLog()<<"mouseReleased "<<labelString;
                    bUseCommand = true;
                    
                    if(stepValue != 0){
                        
                        //some buttons might execute multiple commands; like set to manual and then UP value
                          int command_index = commands.size() - 1;
                        
//                        *value += stepValue;
//                        *value = ofClamp(*value,0,maxValue);
                        commands[command_index][bytePosA] = (unsigned char) *value; // in case *value was modified by other up or down button, we need to carry it over first
                        
//                        ofLog()<<labelString<<" *value "<<*value;
                        //we construct the new value. 
//                        ofLog()<<"bytePosA "<<bytePosA<<" bytePosB "<<bytePosB;
                        
                      //*value is a pointer so that other UP or DOWN button can also have access to this value
                        *value = int(commands[command_index][bytePosA]) + stepValue;
                        *value = ofClamp(*value,0,maxValue);
                        
//                        ofLog()<<"byte# "<<ofToString(bytePosA)<<" as int "<<int(commands[command_index][bytePosA])<<" newValue "<<*value;
                        
                        //since default is 0x00 it's like setting it to the new value
                        commands[command_index][bytePosA] = (unsigned char) *value;
//                        ofLog()<<"new byteA as hex "<<ofToHex(commands[command_index][bytePosA]);
                    }
                    printCommands();
                }
            }
        }
        bHasFocus = false;
        
        if(bEditMode && bSetPositon){
            ofLog()<<labelString<<" : "<<x<<" , "<<y;
        }
        bSetPositon = false;
         
    }
    void mouseMoved(ofMouseEventArgs& event){
        bHasFocus = false;
        if (box.inside(event.x, event.y)){
            bInside = true;
        }else{
            bInside = false;
        }
    }
    
    void mouseDragged(ofMouseEventArgs& event){
        if(bEditMode && bSetPositon){
            int multiple = 10;
            
            //snap to grid
            x = (int(event.x) / multiple) * multiple;
            y = (int(event.y) / multiple) * multiple;
                       
            box.set(x,y, width, height); 
        }
    }
    
    void printCommands(){
        ofLog()<<labelString;
        for(int i=0; i<commands.size();i++){
            cout<<i<<endl;
            for(int n=0; n<commands[i].size();n++){
                cout<<ofToHex(commands[i][n])<<" , ";
            }
            cout<<endl;
        }
    }
    
    vector<vector<unsigned char>> getCommands(){
        return commands;
    }
    
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    int getWidth(){
        return width;
    }
    int getHeight(){
        return height;
    }
    
    void saveButton(ofxXmlSettings & XML){
        XML.setValue("BUTTONS:"+labelString+":x", x);
        XML.setValue("BUTTONS:"+labelString+":y", y);
        XML.setValue("BUTTONS:"+labelString+":width", width);
        XML.setValue("BUTTONS:"+labelString+":height", height);
//        if(!xml.tagExists(name))
//            xml.addValue(name,value);
//        else
//            xml.setValue(name,value);
        
    }
protected:
    
    float    x;
    float    y; 
    float    width; 
    float    height;
    ofRectangle box; 
    
    bool    bHasFocus; 
    bool bInside;
    string    labelString; 
    
    ofColor clickColor;
    ofColor hoverColor;
    ofColor normalColor;
     ofColor textColor;
//    vector<vector<unsigned char>> commands;
//    int camID;
    
private:
    
    bool    bWasSetup;
    
//     ofxXmlSettings XML;
};
#endif /* viscaButton_h */
