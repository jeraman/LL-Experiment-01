//
//  gui.hpp
//  advanced_looper
//
//  Created by jeronimo on 2016-09-15.
//
//

#ifndef gui_h
#define gui_h

#include "ofMain.h"
#include "loop.h"

/********************************
 Handles the graphic user interface
 ********************************/

class Gui
{
   
public:
     Gui();   //constructor
     Gui(bool);   //constructor
    ~Gui();   //desctructor
    
    void draw(vector<float> &, vector<float> &, Loop*);   //drawclass for visual feedback
    void drawBackground(bool);                            //draws the background
    void drawFirstLoop(Loop*);                            //draws the first loop
    void drawHead(Loop*);                                 //draws the head of the looper
    void drawMic(vector<float> &, vector<float> &);       //draws the input mic in realtime
    void drawWindow();                                    //draws the window
    
    void set_debug(bool);                                 //debug control
    void set_head_offset(int);
    void set_scale(float);
    void set_window(int, int);
    void remove_window();
    int  get_head_offset();
    
private:
    bool debug;
    int head_offset;
    float scale;
    int window1_start, window1_end;
};

#endif /* gui_h */
