#pragma once

#include "ofMain.h"
#include "loop.h"
#include "input_interface.h"
#include "gui.h"


class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
    
        void update_NONE(bool);
        void update_ONE_FINGER(bool);
        void update_TWO_FINGERS(bool);
        void update_THREE_FINGERS(bool);
        void update_FOUR_FINGERS(bool);
		
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
    
    private:
        Loop loop;
        Input_Interface inter;
        Gui gui;
    
        State state, last_state;
        vector<Touch> fingers;
    
        vector <float> leftMic;
        vector <float> rightMic;
        ofSoundStream soundStream;
    
        float 	pan;
        int		sampleRate;
        float 	volume;
        bool    debug;
    
};
