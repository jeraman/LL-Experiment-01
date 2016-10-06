#include "input_interface.h"


Input_Interface::Input_Interface()
{
    //pad = ofxMultiTouchPad();
    set_debug(false);
    state = NONE;
    number_of_fingers = 0;
}

Input_Interface::Input_Interface(bool debug)
{
    Input_Interface();
    set_debug(debug);
}

Input_Interface::~Input_Interface()
{
    
}

void Input_Interface::set_debug(bool debug)
{
    this->debug=debug;
}

int Input_Interface::get_number_of_fingers()
{
    return number_of_fingers;
}

void Input_Interface::set_number_of_fingers (int n)
{
    number_of_fingers = n;
}

State Input_Interface::get_state ()
{
    int options = get_number_of_fingers();
    
    switch(options)
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

//@TODO - update the visuals
void Input_Interface::draw ()
{
    ofSetColor(255, 128, 0);
    std::vector<Touch> mTouches = get_fingers();
    float scale = 100;
    for (std::vector<Touch>::iterator touch=mTouches.begin(); touch!=mTouches.end(); ++touch)
    {
        float size = (touch->size)*scale;
        ofPushMatrix();
        ofTranslate(touch->x*ofGetWidth(), touch->y*ofGetHeight());
        ofRotate(touch->angle);
        ofEllipse(0, 0, size, size);
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
    //debugging
    if (debug) {
        string info = "CURRENT STATE: "+ofToString(get_number_of_fingers(),0);
        ofSetColor(200);
        ofDrawBitmapString(info, ofPoint(20, 50));
    }
}



void Input_Interface::set_fingers (vector<Touch>& fingers)
{
    this->touches = &fingers;
}

vector<Touch> Input_Interface::get_fingers ()
{
    return *(this->touches);
}


