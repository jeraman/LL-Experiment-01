#include "ofApp.h"


bool isMouseClicked = false;

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
	
    //ofSetCircleResolution(80);
	//ofBackground(54, 54, 54);
    
    //setting the debug var
    debug = true;
	
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


void ofApp::drawFirstLoop()
{
    //getting the first loop if available
    Loop* first = lm.get_loop(' ');
    
    //checks if there is first. continues to execute if there is
    if (first == nullptr) {
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

void ofApp::drawHead() {
    
    //getting the first loop if available
    Loop* first = lm.get_loop(' ');
    
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


void ofApp::drawBackground() {
    ofBackground(255, 150);
    
    if (lm.is_recording()) { //if it's recording
        ofBackground(255, 100, 100, 150);
    }
    
    inter.draw();
    
}

// draw the left channel:
void ofApp::drawMic() {

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
    
    /*
    if (isMouseClicked) {
        float newHead = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
        lm.set_head(' ', newHead);
    }
     */
        
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    drawBackground();
    
    if (debug) {
        ofSetColor(200);
        string fps("FPS: ");
        ofDrawBitmapString(fps + ofToString(ofGetFrameRate()), 20, 20);
    }
    
    drawFirstLoop();
    drawHead();
    drawMic();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    lm.audio_input(input, bufferSize, nChannels);
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        leftMic[i]	= input[i*2]*0.5;
        rightMic[i]	= input[i*2+1]*0.5;
    }

}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    
    
    lm.audio_output(output, bufferSize, nChannels);
}


bool paused = false;
//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

    //disabling debug condition
    if (key=='d' || key=='D')
        debug=!debug;
    
    if (key==' ') {
        Loop* l = lm.get_loop(' ');
        if (l==nullptr)
            lm.record(' ');
        else
            lm.overdub(' ');
    }
    
    if (key=='-')
        lm.kill_all();
    
    if (key=='p' || key=='P') {
        paused = !paused;
        if (paused)
            lm.stop(' ');
        else
            lm.play(' ');
    }
    
    cout << key <<endl;
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

