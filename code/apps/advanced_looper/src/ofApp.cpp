#include "ofApp.h"


bool isMouseClicked = false;

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
	   
    //setting the debug var
    debug = true;
    
    //initializing the loop
    loop.setup();
    
    //debug which devices are available in this machine
    if (debug)
        soundStream.printDeviceList();
	
	left.assign(BUFFER_SIZE, 0.0);
	right.assign(BUFFER_SIZE, 0.0);
    
    leftMic.assign(BUFFER_SIZE, 0.0);
    rightMic.assign(BUFFER_SIZE, 0.0);
	
    //volHistory.assign(400, 0.0);
	
    //sets up the input and output interfaces
	soundStream.setup(this, N_CHANNELS, N_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, 4);
    
    //freezing the framerate
    ofSetFrameRate(60);
    
    //debug: end of setup function
    if (debug)
        cout << "setuped!" <<endl;

}


//--------------------------------------------------------------
void ofApp::update(){
    /*
    //////////////////////////////////
    ///WHY THAT?????
   	//lets scale the vol up to a 0-1 range
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    ///end of WHY THAT?
     */
    
    /*
    if (isMouseClicked) {
        float newHead = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
        lm.set_head(' ', newHead);
    }
     */
        
}

//--------------------------------------------------------------
void ofApp::draw(){
    //getting the necessary data
    //Loop* first       = lm.get_loop(' ');
    //bool is_recording = lm.is_recording();
    //gui.draw(leftMic, rightMic, first, is_recording);
    
    gui.draw(leftMic, rightMic, &loop);
    inter.draw();
    
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

