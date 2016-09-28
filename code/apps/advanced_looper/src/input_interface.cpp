#include "input_interface.h"


Input_Interface::Input_Interface()
{
    pad = ofxMultiTouchPad();
    set_debug(false);
    state = NONE;
}

Input_Interface::Input_Interface(bool debug)
{
    Input_Interface();
    set_debug(debug);
}

Input_Interface::~Input_Interface()
{
    
}

State Input_Interface::get_state ()
{
    //vector<MTouch> mTouches = pad.getTouches();
    
    switch(pad.getTouchCount())
    {
        case 0:
            state = NONE;
            break;
        case 1:
            state = ONE_FINGER;
            break;
        case 2:
            state = TWO_FINGERS;
            break;
        case 3:
            state = THREE_FINGERS;
            break;
        case 4:
            state = FOUR_FINGERS;
            break;
    }
    
    return state;
}


vector<Touch> Input_Interface::get_fingers ()
{
    vector<MTouch> mTouches = pad.getTouches();
    vector<Touch> fingers;
    
    float scale = 100;
    for (std::vector<MTouch>::iterator touch=mTouches.begin(); touch!=mTouches.end(); ++touch)
    {
        //getting the essential info
        float x = touch->x;
        float y = touch->y;
        float angle = touch->angle;
        float size = touch->size;
        
        //creating a new touch base don it
        Touch newFinger(x, y, angle, size);
        
        //returning the touches
        fingers.push_back(newFinger);
    }
    
    //sorting the output vector
    std::sort(fingers.begin(), fingers.end());
    
    return fingers;
}

//@TODO - update the visuals
void Input_Interface::draw ()
{
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