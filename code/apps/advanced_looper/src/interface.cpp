#include "interface.h"

Interface::Interface()
{
    pad = ofxMultiTouchPad();
}

Interface::~Interface()
{
    
}

void Interface::draw() {
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
    
    
    // Print some Debug Info
    string info = "Number of touches: "+ofToString(pad.getTouchCount(),0);
    ofDrawBitmapString(info, ofPoint(0, 20));
}