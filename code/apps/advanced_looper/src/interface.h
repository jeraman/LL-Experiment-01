#ifndef INTERFACE_H
#define INTERFACE_H

#include "ofMain.h"
#include "ofxMultiTouchPad.h"
#include <vector>


class Interface
{
    ofxMultiTouchPad pad;
    std::vector<ofPoint> touches;
    
    public:
        Interface();
        virtual ~Interface();
    
        void draw();
};

#endif // INTERFACE_H
