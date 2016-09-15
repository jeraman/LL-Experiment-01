#ifndef INPUT_INTERFACE_H
#define INPUT_INTERFACE_H

#include "ofMain.h"
#include "ofxMultiTouchPad.h"
#include <vector>


/********************************
 Hides from the system what input interface is currently being used (e.g. ipad, mouse pad, etc).
 ********************************/

class Input_Interface
{
    ofxMultiTouchPad pad;
    std::vector<ofPoint> touches;
    
public:
     Input_Interface();   //constructor
     Input_Interface(bool);   //constructor
    ~Input_Interface();   //desctructor
    
    void draw();                  //drawclass for visual feedback\
   
    void set_debug(bool);
    
private:
    bool debug;
};

#endif // INPUT_INTERFACE_H
