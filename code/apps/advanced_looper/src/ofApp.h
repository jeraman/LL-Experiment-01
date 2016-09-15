#pragma once

#include "ofMain.h"
#include "loop_manager.h"
#include "input_interface.h"
#include "gui.h"

//#include "ofxOsc.h"

// listen on port 12345
//#define PORT            12345
//#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
				
		void audioIn(float * input, int bufferSize, int nChannels);
        void audioOut(float * input, int bufferSize, int nChannels);

        void set_debug(bool);
    
    //audio in variables
		vector <float> left;
		vector <float> right;
		vector <float> volHistory;
		
		int 	bufferCounter;
		int 	drawCounter;
		
		float smoothedVol;
		float scaledVol;
		
		ofSoundStream soundStream;
    
    //audio out variables
        float 	pan;
        int		sampleRate;
        bool 	bNoise;
        float 	volume;
        
        vector <float> leftMic;
        vector <float> rightMic;
    
    //debug variables
        bool    debug;
    
    //old looper variables
    private:
        Loop_Manager lm;
        Input_Interface inter;
        Gui gui;
    
        //int				mouseX, mouseY;
        //string			mouseButtonState;
};
