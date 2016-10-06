#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
    
    //initi the debug
    set_debug(false);
    
    //freezing the framerate
    ofSetFrameRate(60);
    
    //init sound
    setup_sound();
    
    //seting up state machine
    sm.setup();
    
    //debug: end of setup function
    if (debug)
        cout << "setuped!" <<endl;
}

//--------------------------------------------------------------
void ofApp::setup_sound(){
    
    //debug which devices are available in this machine
    if (debug)
        soundStream.printDeviceList();
    
    
    //sets up the input and output interfaces
    bool result = soundStream.setup(this, N_CHANNELS, N_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, 4);
    
    if (debug) {
        cout<< "sound setup succeded? " << result << " setup: " << endl;
        cout<< "      N_CHANNELS:  " << N_CHANNELS << endl;
        cout<< "      SAMPLE_RATE: " << SAMPLE_RATE << endl;
        cout<< "      BUFFER_SIZE: " << BUFFER_SIZE << endl;
    }

}

//--------------------------------------------------------------
void ofApp::update() {
    sm.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    sm.draw();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    sm.audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    sm.audioOut(output, bufferSize, nChannels);
}


bool paused = false;
//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

    //disabling debug condition
    if (key=='d' || key=='D')
        this->set_debug(!debug);
    
    if (key==' ') {
        if (sm.is_loop_empty())
            sm.record();
        else
            sm.overdub();
    }
    
    if (key=='-')
        sm.clear_loops();
    
    if (key=='p' || key=='P') {
        paused = !paused;
        if (paused)
            sm.stop();
        else
            sm.resume();
    }
    
    if (debug)
        cout << "key pressed: " << key <<endl;
}


void ofApp::set_debug(bool debug)
{
    this->debug=debug;
    sm.set_debug(debug);
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

