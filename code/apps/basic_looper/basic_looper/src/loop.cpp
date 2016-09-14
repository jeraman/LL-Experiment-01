#include "loop.h"

Loop::Loop(vector<float> b, int ip, int bufsize, int nchan, int i)
{
    //inicializando as variáveis
    sample = b;
    inipos = ip;
    bufferSize = bufsize;
    nChannels = nchan;
    muted = false;

    //inicializando o id
    id = i;

    //começando do início
    outpos = 0;

    //inicializando as variáveis não usadas
    volume = 1.f;
    leftpan = 1.f;
    rightpan = 1.f;
    fade = 0.7f;
    
    overdubbing = false;
}

Loop::~Loop()
{
    sample.clear();
}

void Loop::stop()
{
    muted = true;
    outpos = 0;
}

void Loop::resume()
{
    muted = false;
    //outpos = 0;
}

void Loop::overdub()
{
    overdubbing = !overdubbing;
}


int  Loop::get_size()
{
    return sample.size();
}


void Loop::play(float* &output, bool e_principal)
{
    if (!muted)
    {
        //debugging variable
        //vector<float> copy;
        
        for(int i=0; i<bufferSize; i++) {
            int   index = i*nChannels;
            
            output[index  ] += sample[outpos + index] * volume * leftpan;
            output[index+1] += sample[outpos + index] * volume * rightpan;
            
            //copy.push_back(output[index]);
            //copy.push_back(output[index+1]);

        }
    
        outpos = ((outpos + bufferSize*2)%(sample.size()));

    }
    /*
    else if (e_principal) //se e o principal e ele esta mudo
    {
        /* old code, see the comments below
        if (outpos + bufferSize <= buffer.size())
            outpos += (int)bufferSize;
        else
            outpos = 0;
         //fechar comments
        
        outpos = (((int)bufferSize*2)%(sample.size()));
    }
    */
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
