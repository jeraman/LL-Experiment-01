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
        Loop(vector<float>, int, int = BUFFER_SIZE, int = N_CHANNELS, int = -1);
        virtual ~Loop();

        void play (float* &output, bool = false);     //
    
        void overdub_sample_vector (float* &input);   //adds more audio info inside this loop
        void overdub();                               //sets the overdubbing boolean to true
        void stop ();                                 //stops this sample
        void resume ();                               //resumes the sample if stopped
        int  get_size();                              //gets the size of this loop
        void set_head(float);                           //sets the head position of this loop (between 0 and 1)
    
    
        void update_buffer(vector<float>, int); //replaces the content of this loop

        int outpos;           //a agulha do vinil (para este loop)
        int bufferSize;       //tamanho do buffer quando gravado
        int nChannels;        //número de canais do loop
        int inipos;           //posição inicial com relação ao main


        int id; //id do botao correspondente
        bool muted;           //to mute the loop
        bool overdubbing;      //indicates if this loop is currently overdubbing or not

        //variaveis não utilizadas
        float volume;
        float leftpan;
        float rightpan;
        float fade;


    //private:
        vector<float> sample; //guarda os dados sonoros

};

#endif // LOOP_H
