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
    void drawAuxHead(Loop*);                              //draws the head of the looper
    void drawMic(vector<float> &, vector<float> &);       //draws the input mic in realtime
    void drawWindow(bool);                               //draws the window
    
    void set_debug(bool);                                 //debug control
    void set_scale(float);
    void set_window(int, int);
    void set_aux_window(int, int);
    void remove_aux_window();
    void remove_window();
    
private:
    bool debug;
    bool there_is_an_aux_window;
    //int head_offset;
    float scale;
    int window1_start, window1_end;
    int window2_start, window2_end;
};

#endif /* gui_h */
