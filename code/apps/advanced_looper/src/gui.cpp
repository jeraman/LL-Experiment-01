//
//  gui.cpp
//  advanced_looper
//
//  Created by jeronimo on 2016-09-15.
//
//

#include "gui.h"


Gui::Gui()
{
    debug=false;
}

Gui::Gui(bool debug)
{
    this->set_debug(debug);
}

Gui::~Gui()
{
    
}

//@TODO - update the visuals
void Gui::draw(vector<float>& leftMic, vector<float>& rightMic, Loop* first) {
    
    drawBackground(first->is_recording());
    drawFirstLoop(first);
    drawHead(first);
    drawMic(leftMic, rightMic);
    
    if (debug) {
        ofSetColor(200);
        string fps("FPS: ");
        ofDrawBitmapString(fps + ofToString(ofGetFrameRate()), 20, 20);
    }
}

//draws the background
void Gui::drawBackground(bool is_recording) {
    ofBackground(255, 150);
    
    if (is_recording) { //if it's recording
        ofBackground(255, 100, 100, 150);
    }
}


//draws the first loop
void Gui::drawFirstLoop(Loop* first)
{
    //checks if there is first. continues to execute if there is
    if (first->is_empty()) {
        return;
    }
    
    //start drawing first waveform
    ofSetColor(30);
    ofSetLineWidth(4);
    
    float posy = ofGetHeight()/2.0;
    
    int loopsize = first->sample.size();
    
    //iterates over the screenpixels
    for (unsigned int i = 0; i < ofGetWidth(); i++){
        //gets the correspond position of the index of the loop and the screen width
        //float posx = ofMap(i, 0, loopsize, 0, ofGetWidth());
        
        //mapping the screen width to the position in the array
        int convWidthToSamples = (int)((i/(float)ofGetWidth())*loopsize);
        
        //drawing the corresponding rectangle
        //float sizey = ofMap(first->sample[convWidthToSamples], -1, 1, -posy, posy);
        float sizey = 4*ofMap(abs(first->sample[convWidthToSamples]), -1, 1, -posy, posy);
        
        ofDrawRectangle(i,posy,1,sizey);
        ofDrawRectangle(i,posy,1,-sizey);
        
    }
    
    if (debug) {
        float beg  = first->sample[0];
        float end  = first->sample[loopsize-1];
    }
}


void Gui::drawHead(Loop* first) {
    
    //getting the first loop if available
    //Loop* first = lm.get_loop(' ');
    
    //checks if there is first. continues to execute if there is
    if (first == nullptr)
        return;
    
    //stores the loop size
    int loopsize = first->sample.size();
    
    //start drawing first waveform
    ofSetColor(100);
    ofSetLineWidth(20);
    
    //gets the correspond position of the index of the loop and the screen width
    float posx = ofMap(first->outpos, 0, loopsize, 0, ofGetWidth());
    //computing the y size of each rectangle
    
    ofDrawLine(posx,0,posx,ofGetHeight());
    
}



// draw the left channel:
void Gui::drawMic(vector<float> & leftMic, vector<float> & rightMic) {
    
    ofPushStyle();
    ofPushMatrix();
    
    ofSetColor(200, 0, 0);
    ofSetLineWidth(3);
    
    ofBeginShape();
    
    for (unsigned int i = 0; i < leftMic.size(); i++){
        float average=(leftMic[i]+rightMic[i])/2;
        float xpos= ((i/(float)leftMic.size())*ofGetWidth());
        float ypos=((ofGetHeight()/2) - average*1000.0f);
        
        ofVertex(xpos, ypos);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
}


void Gui::set_debug(bool debug)
{
    this->debug=debug;
}