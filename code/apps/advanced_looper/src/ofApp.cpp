#include "ofApp.h"


bool isMouseClicked = false;

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
    
    //initializing the loop
    loop.setup();
    
    //initi the debug
    this->set_debug(false);
    
    //left.assign(BUFFER_SIZE, 0.0);
	//right.assign(BUFFER_SIZE, 0.0);
    
    leftMic.assign(BUFFER_SIZE, 0.0);
    rightMic.assign(BUFFER_SIZE, 0.0);
	
    //debug which devices are available in this machine
    if (debug)
        soundStream.printDeviceList();
        
    //sets up the input and output interfaces
    soundStream.setup(this, N_CHANNELS, N_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, 4);
    
    //freezing the framerate
    ofSetFrameRate(60);
    
    //debug: end of setup function
    if (debug)
        cout << "setuped!" <<endl;
    
    //initializes the first state
    state = inter.get_state();
    //initializes the last state
    last_state = state;
}


//--------------------------------------------------------------
void ofApp::update(){
    
    //if there are no loops, don't even bother processing the rest
    if (loop.is_empty())
        return;
    
    //gets the input_interface current state
    state = inter.get_state();
    fingers = inter.get_fingers();
    
    //verifies if this is the first time the current state is accessed
    bool is_first_time_state_is_accessed = (last_state != state);
    
    //case-by-case analysis
    switch(state) {
        case NONE: //no fingers in the screen
            update_NONE(is_first_time_state_is_accessed);
            break;
        case ONE_FINGER: //one finger in the screen
            update_ONE_FINGER(is_first_time_state_is_accessed);
            break;
        case TWO_FINGERS: //two fingers in the screen
            update_TWO_FINGERS(is_first_time_state_is_accessed);
            break;
        case THREE_FINGERS: //three fingers in the screen
            update_THREE_FINGERS(is_first_time_state_is_accessed);
            break;
        case FOUR_FINGERS: //four fingers in the screen
            update_FOUR_FINGERS(is_first_time_state_is_accessed);
            break;
    }
    
    //updates last state
    last_state = state;
}

//--------------------------------------------------------------
void ofApp::update_NONE(bool is_first_time_state_is_accessed)
{
    if (is_first_time_state_is_accessed) {
        //updates output with interpolation
        loop.update_output_buffer(true);
       
        //@TODO FIX PROBLEM WHERE LOOPS RESTARTS AFTER RELEASING ONE FINGER
        //gets where the xfinger was
        //int old_head = gui.get_head_offset();
        
        //makes it a multitple of bufferSize
        //int rest = old_head%BUFFER_SIZE;
        //old_head += rest;
        
        //updates the loop head to where the finger was
        //loop.set_head(old_head);
        
        //sets the gui headoffset to zero
        gui.set_head_offset(0);
    }
    
    if (debug)
        cout << "update_NONE!" << endl;
}


//--------------------------------------------------------------
void ofApp::update_ONE_FINGER(bool is_first_time_state_is_accessed)
{
    Touch f1 = fingers[0];
    
    //computing the position in the sound
    float newx = f1.x*loop.get_size();
    
    //computing value for the volume
    float newy = 1-f1.y;
    
    //computs the range from newx
    float range = 10*BUFFER_SIZE;
    
    //updates newx in case it's too close to the beginning
    if (newx <= range)
        newx = range+1;
    
    //updates newx in case it's too close to the end
    if (newx >= loop.get_size()-range)
        newx = loop.get_size()-range-1;
    
    loop.update_output_buffer(newx-range, newx+range, true);
    loop.set_volume(newy);
    gui.set_scale(newy*2);
    
    gui.set_head_offset(newx);
    
    if (debug) {
        cout << "update_ONE_FINGER!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
    }
}

//--------------------------------------------------------------
void ofApp::update_TWO_FINGERS(bool is_first_time_state_is_accessed)
{
    Touch f1 = fingers[0];
    Touch f2 = fingers[1];
    
    if (debug) {
        cout << "update_TWO_FINGERS!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
        cout << "           x: " << f2.x << " y: " << f2.y << endl;
    }
}

//--------------------------------------------------------------
void ofApp::update_THREE_FINGERS(bool is_first_time_state_is_accessed)
{
    Touch f1 = fingers[0];
    Touch f2 = fingers[1];
    Touch f3 = fingers[2];
    
    if (debug) {
        cout << "update_THREE_FINGERS!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
        cout << "           x: " << f2.x << " y: " << f2.y << endl;
        cout << "           x: " << f3.x << " y: " << f3.y << endl;
    }
}

//--------------------------------------------------------------
void ofApp::update_FOUR_FINGERS(bool is_first_time_state_is_accessed)
{
    Touch f1 = fingers[0];
    Touch f2 = fingers[1];
    Touch f3 = fingers[2];
    Touch f4 = fingers[3];
    
    if (debug) {
        cout << "update_FOUR_FINGERS!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
        cout << "           x: " << f2.x << " y: " << f2.y << endl;
        cout << "           x: " << f3.x << " y: " << f3.y << endl;
        cout << "           x: " << f4.x << " y: " << f4.y << endl;
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    gui.draw(leftMic, rightMic, &loop);
    inter.draw();
    
    if (debug) {
        string info = "CURRENT STATE: " + ofToString(inter.get_state(),0);
        ofSetColor(200);
        ofDrawBitmapString(info, ofPoint(20, 70));
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    loop.audio_input(input);
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        leftMic[i]	= input[i*2]*0.5;
        rightMic[i]	= input[i*2+1]*0.5;
    }

}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    loop.audio_output(output);
}


bool paused = false;
//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

    //disabling debug condition
    if (key=='d' || key=='D')
        this->set_debug(!debug);
    
    if (key==' ') {
        if (loop.is_empty())
            loop.record();
        else
            loop.overdub();
    }
    
    if (key=='-')
        loop.clear();
    
    if (key=='p' || key=='P') {
        paused = !paused;
        if (paused)
            loop.stop();
        else
            loop.resume();
    }
    
    if (debug)
        cout << key <<endl;
}


void ofApp::set_debug(bool debug)
{
    this->debug=debug;
    gui.set_debug(debug);
    inter.set_debug(debug);
    loop.set_debug(debug);
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    isMouseClicked = true;
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
    isMouseClicked = false;
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

