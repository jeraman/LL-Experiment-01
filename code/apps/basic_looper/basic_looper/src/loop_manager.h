#ifndef LOOP_MANAGER_H
#define LOOP_MANAGER_H

#include "loop.h"
#include <vector>


/**************************
 **************************
 **************************
 *** COMANDOS POSSÍVEIS  **
 ***  - record (ID);     **
 ***  - play   (ID);     **
 ***  - stop   (ID);     **
 ***  - erase  (ID);     **
 **************************
 **************************
 **************************/


class Loop_Manager
{
    public:
         Loop_Manager();
        ~Loop_Manager(){};

        void audio_input (float*&, int, int); //should be called when there's any audio input
        void audio_output(float*&, int, int); //should be called when there's any audio output
    
        Loop* get_loop (int);   //returns a pointer to given loop
        bool  is_recording();   //returns if it's recording or not
        void  record   (int);   //records and stops recording a loop
        void  play     (int);   //play a given loop
        void  stop     (int);   //stops playing a given loop
        void  erase    (int);   //deletes a given loop
        void  overdub  (int);   //overdubs a given loop
        void  kill_all ();      //kills all the loops
        void  set_head(int, float); //set where the head of a loop will be
    
        void  interpolating_beg_and_end(); //in terpolates the begning and the and of an audio sample in order to avoid glitch
        void  pop ();                      //pops the last recorded loop

   	private:
		vector<float> input_buf;
		vector<Loop>  loops; //armazena os loops
    
        int  temp_inipos;    //stores the temp inipos!
        int  start_time;     //conta o tempo passado em segundos
		int  temp_id;        //id do loop que está sendo gravado
		bool recording;      //marca se esta gravando
		bool playing;        //marca se esta tocando

		int   search   (int);   //searches a given loop in database

};

#endif // LOOP_MANAGER_H
