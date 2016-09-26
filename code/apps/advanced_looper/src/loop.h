#ifndef LOOP_H
#define LOOP_H

#include "ofMain.h"
#include <vector>


#define BUFFER_SIZE 256
#define SAMPLE_RATE	44100
#define N_CHANNELS 2

class Loop
{
    public:
         Loop();
         Loop(vector<float>, int, int = BUFFER_SIZE, int = N_CHANNELS);
        ~Loop();
    
        void setup();
    
        void audio_input (float*&); //should be called when there's any audio input
        void audio_output(float*&); //should be called when there's any audio output
    
        void play (float* &output);                   //plays the loop
        void overdub_sample_vector (float* &input);   //adds more audio info inside this loop
        void overdub();                               //sets the overdubbing boolean to true
        void record ();                               //records and stops recording a loop
        void stop ();                                 //stops this sample
        void resume ();                               //resumes the sample if stopped
        void clear ();                                //cleans the loop
        void set_volume (float);                      //sets the volume
        void set_debug (bool);                        //debug control
        void set_head_normalized (float);             //sets the loop head position (between 0 and 1)
        void set_head_absolute (int);               ///using the size of the sample
        void update_output_buffer (bool);             //updates the output buffer
        void update_output_buffer (int, int, bool);   //updates the output buffer
    
        vector<float> interpolating_beg_and_end (vector<float> &newoutput);            //interpolates the begning and the and of an audio sample in order to avoid glitch
    
    
        bool is_recording ();                          //returns if it's recording or not
        bool is_empty ();                              // if this loop is clear/empty or not
        int  get_size ();                              //gets the size of this loop
    
    
    
        int outpos;          //a agulha do vinil (para este loop)
        int bufferSize;      //tamanho do buffer quando gravado
        int nChannels;       //número de canais do loop
        int inipos;          //posição inicial com relação ao main

        int  start_time;     //how many seconds have passed
        int  temp_inipos;    //stores the temp inipos!  
        bool muted;          //to mute the loop
        bool overdubbing;    //indicates if this loop is currently overdubbing or not
        bool recording;
        bool playing;
        bool debug;

        //variaveis não utilizadas
        float volume;
        float leftpan;
        float rightpan;
        float fade;
    
        vector<float> input_buf;        //stores the loop being currently recorded
        vector<float> output_buf;       //stores the loop that is being played

    //private:
        vector<float> sample; //stores the raw loop

};

#endif // LOOP_H
