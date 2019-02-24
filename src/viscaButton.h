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
    
    //----------------------------------------------------
    viscaButton(){
        bWasSetup = false;
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
    
    void setup(string _label,int _camID, vector<vector<unsigned char>> _commands, float _x, float _y, float _w, float _h){
        x = _x;
        y = _y;
        width = _w;
        height = _h;
        camID = _camID;
        
        bHasFocus = false;
        bInside = false;
        bUseCommand = false;
        
        bEditMode = false;
        bSetPositon = false;
        
        box.set(x,y, width, height); 
        
        labelString = _label; 
        
        commands = _commands;
        
        if(!bWasSetup){
            ofAddListener(ofEvents().draw, this, &viscaButton::draw);
            ofAddListener(ofEvents().mouseMoved, this, &viscaButton::mouseMoved);
            ofAddListener(ofEvents().mousePressed, this, &viscaButton::mousePressed);
            ofAddListener(ofEvents().mouseReleased, this, &viscaButton::mouseReleased);
                        ofAddListener(ofEvents().mouseDragged, this, &viscaButton::mouseDragged);
            bWasSetup = true;
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
                ofSetColor(64,64,266, 150); //sliderAlpha); 
            }else{
                ofSetColor(64,64,266, 50); 
            }
        }else{
            ofSetColor(64,64,64, 50);
        }
        ofDrawRectangle(0,0, width,height); 
        
        ofNoFill();
        ofSetLineWidth(1.0);
        ofSetColor(64,64,64, 150);
        ofDrawRectangle(0,0, width,height); 
        
        // draw focus value 
        if (bHasFocus){
            ofSetColor(0); 
        } else {
            ofSetColor(128); 
        }
        
        //            ofDrawBitmapString( ofToString(getValue(),numberDisplayPrecision), width+5,height/2 + 4);
        
        ofSetColor(64,64,64, 200);
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
protected:
    
    float    x;
    float    y; 
    float    width; 
    float    height;
    ofRectangle box; 
    
    bool    bHasFocus; 
    bool bInside;
    string    labelString; 
    
//    vector<vector<unsigned char>> commands;
//    int camID;
    
private:
    
    bool    bWasSetup;
    
};
#endif /* viscaButton_h */
