#include "loop.h"


Loop::Loop()
{
    vector<float> newsample;
    Loop(newsample, 0, BUFFER_SIZE, N_CHANNELS);
}

Loop::Loop(vector<float> b, int ip, int bufsize, int nchan)
{
    //inicializando as variáveis
    sample = b;
    inipos = ip;
    bufferSize = bufsize;
    nChannels = nchan;
    //muted = false;
    
    setup();
}

Loop::~Loop()
{
    sample.clear();
}

void Loop::setup() {
    vector<float> new_sample;
    
    sample=new_sample;
    inipos = 0;
    bufferSize = BUFFER_SIZE;
    nChannels = N_CHANNELS;
    
    recording  = false;
    playing    = true;
    overdubbing = false;
    
    //começando do início
    outpos = 0;
    
    //inicializando as variáveis não usadas
    volume = 1.f;
    leftpan = 1.f;
    rightpan = 1.f;
    fade = 0.7f;
    
    debug = false;
}

void Loop::stop()
{
    playing = false;
    outpos = 0;
}

void Loop::resume()
{
    playing = true;
}

void Loop::overdub()
{
    overdubbing = !overdubbing;
}


int  Loop::get_size()
{
    return sample.size();
}


void Loop::play(float* &output)
{
    if ((!is_empty()) & playing)
    {
        
        for(int i=0; i<bufferSize; i++) {
            int   index = i*nChannels;
            
            output[index  ] += sample[outpos + index] * volume * leftpan;
            output[index+1] += sample[outpos + index] * volume * rightpan;
        }
    
        outpos = ((outpos + bufferSize*2)%(sample.size()));

    }
}

//////////////////////////////////
// grava um determinado loop com o id pedido
// - id - o id do botao dono do loop
//////////////////////////////////
void Loop::record()
{
    if (!recording) //se não tá gravando, comece a gravar
    {
        recording = !recording;
        start_time = ofGetElapsedTimef();
        input_buf.clear();
        temp_inipos = 0;
    }
    
    else       //se tá gravando, finalize
    {
        recording = !recording;
        int time = ofGetElapsedTimef() - start_time;
        
        //interpolates
        //interpolating_beg_and_end();
            
        sample=input_buf;
        
        if (debug)
            cout << " loop created! size: " << sample.size() << " time: "<< time << endl;
    }
    
}


//--------------------------------------------------------------
void Loop::audio_input(float * &input)
{
    if (recording)
    {
        for(int i=0; i<bufferSize; i++) {
            input_buf.push_back(input[ i*nChannels]);
            input_buf.push_back(input[(i*nChannels)+1]);
        }
    }
    
    //this will be executed if only if the current loop is set to overdub
    //for (int i = 0; i < loops.size();i++)
    this->overdub_sample_vector(input);
}



//////////////////////////////////
// funcao que é chamada quando existe saida a ser liberada
// - output - caixa de som
//////////////////////////////////
void Loop::audio_output(float * &output)
{
    if (playing)
        this->play(output);    //toca o loop principal
}


void Loop::overdub_sample_vector(float* &input)
{
    //ignores this function if this loop is not setted to overdub
    if (!overdubbing)
        return;
    
    //overdubs
    for(int i=0; i<bufferSize; i++) {
        int   index = i*nChannels;
        
        sample[outpos + index]      += input[index  ] * volume * leftpan;
        sample[outpos + index + 1 ] += input[index +1 ] * volume * rightpan;
        
    }
}


void Loop::set_head(float position)
{
    int newHead = position*sample.size();
    
    //if newHead is an odd number (right channel), makes it even (left channel)
    if (newHead%2 != 0)
        newHead -=1;
    
    //cout << newHead <<endl;
    outpos=newHead;
}


void Loop::update_buffer(vector<float> b, int ip)
{
    //inicializando as variáveis
    sample = b;
    inipos = ip;
}

//////////////////////////////////
// returns if it's recording or not
//////////////////////////////////
bool Loop::is_recording()
{
    return (this->recording | this->overdubbing);
}

//////////////////////////////////
// returns if this loop is empty/clear nor not
//////////////////////////////////
bool Loop::is_empty()
{
    return (this->sample.size()==0);
}


//////////////////////////////////
// clears the loop
//////////////////////////////////
void Loop::clear()
{
    this->sample.clear();
    this->input_buf.clear();
    recording  = false;
    playing    = true;
    overdubbing = false;
    outpos = 0;
}

void Loop::set_debug(bool debug)
{
    this->debug=debug;
}