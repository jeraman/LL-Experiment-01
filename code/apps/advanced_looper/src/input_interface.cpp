#include "input_interface.h"

Input_Interface::Input_Interface()
{
    pad = ofxMultiTouchPad();
    set_debug(false);
}

Input_Interface::Input_Interface(bool debug)
{
    Input_Interface();
    set_debug(debug);
}

Input_Interface::~Input_Interface()
{
    
}

//@TODO - update the visuals
void Input_Interface::draw() {
    ofSetColor(255, 128, 0);
    std::vector<MTouch> mTouches = pad.getTouches();
    float scale = 100;
    for (std::vector<MTouch>::iterator touch=mTouches.begin(); touch!=mTouches.end(); ++touch)
    {
        float size = (touch->size)*scale;
        ofPushMatrix();
        ofTranslate(touch->x*ofGetWidth(), touch->y*ofGetHeight());
        ofRotate(touch->angle);
        ofEllipse(0, 0, size, size*.5);
        ofPopMatrix();
    }
    
    
    /*
     Method 2: Iterate over all touches as a vector of ofPoints – very simple datatype though...
     */
    ofPushMatrix();
    ofSetColor(255, 255, 255);
    pad.getTouchesAsOfPoints(&touches);
    ofPoint size = ofPoint(ofGetWidth(),ofGetHeight(),0);
    ofSetRectMode(OF_RECTMODE_CENTER);
    for (vector<ofPoint>::iterator touch = touches.begin(); touch!=touches.end(); ++touch) {
        ofRect((*touch)*size, 10, 10);
    }
    ofPopMatrix();
    
    //debugging
    if (debug) {
        string info = "NUMBER OF TOUCHES: "+ofToString(pad.getTouchCount(),0);
        ofSetColor(200);
        ofDrawBitmapString(info, ofPoint(20, 50));
    }
}

void Input_Interface::set_debug(bool debug)
{
    this->debug=debug;
}