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
    
    //initializes the first state
    state = inter.get_state();
    
    //initializes the last state
    last_state = state;
    
    //debug: end of setup function
    if (debug)
        cout << "setuped!" <<endl;
}

//--------------------------------------------------------------
void ofApp::setup_sound(){
    
    //initializing the loop
    loop.setup();
    
    //init the buffer fmor mic monitoring
    leftMic.assign(loop.bufferSize, 0.0);
    rightMic.assign(loop.bufferSize, 0.0);
    
    //debug which devices are available in this machine
    if (debug) {
        soundStream.printDeviceList();
    }
    
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
        /*
        case THREE_FINGERS: //three fingers in the screen
            update_THREE_FINGERS(is_first_time_state_is_accessed);
            break;
        case FOUR_FINGERS: //four fingers in the screen
            update_FOUR_FINGERS(is_first_time_state_is_accessed);
            break;
         */
    }
    
    //updates last state
    last_state = state;
}

//--------------------------------------------------------------
void ofApp::update_NONE(bool is_first_time_state_is_accessed)
{
    if (is_first_time_state_is_accessed) {
        //updates to the full looping area
        loop.set_full_looping_area();
       
        //removes any window the gui might have
        gui.remove_window();
    }
    
    //if (debug)
    //    cout << "update_NONE!" << endl;
}


//--------------------------------------------------------------
void ofApp::update_ONE_FINGER(bool is_first_time_state_is_accessed)
{
    Touch f1 = fingers[0];
    
    //computing the position in the sound
    float newx = f1.x*loop.get_size();
    
    //computing value for the volume
    float newy = (1-f1.y)*2;
    
    //new attempt
    //loop.set_head_absolute(newx);
    
    //updates the looping area
    loop.set_looping_area(newx, newx + 15*loop.bufferSize);
    
    //sets the new volume
    loop.set_volume(newy);
    
    //updates the drawing scale for the waveform
    gui.set_scale(newy);
    
    //removes any possible window
    gui.remove_window();
    
    //if (debug) {
    //    cout << "update_ONE_FINGER!"<< endl;
    //    cout << "           x: " << f1.x << " y: " << f1.y << endl;
    //}
    
    
}

//--------------------------------------------------------------
void ofApp::update_TWO_FINGERS(bool is_first_time_state_is_accessed)
{
    Touch f1 = fingers[0];
    Touch f2 = fingers[1];
    
    //computing the position in the sound
    int newf1x = f1.x*loop.get_size();
    int newf2x = f2.x*loop.get_size();
    
    float newy = (1-((f1.y+f2.y)/2))*2;

    //updates the looping area
    loop.set_looping_area(newf1x, newf2x);
        
    //sets the window
    gui.set_window(f1.x*ofGetWidth(), f2.x*ofGetWidth());
    
    //sets the volue
    loop.set_volume(newy);
    
    //sets the scale
    gui.set_scale(newy);
    
    //organizes each one should go first and updates the head of the gui
    
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
    
    //gui.remove_window();
    
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
        cout << "key pressed: " << key <<endl;
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

