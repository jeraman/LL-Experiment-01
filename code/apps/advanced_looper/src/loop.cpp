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
    bufferSize = bufsize;
    nChannels = nchan;
    debug = false;
    
    setup();
}

Loop::~Loop()
{
    sample.clear();
}

void Loop::setup() {
    vector<float> new_sample;
    
    sample=new_sample;
    bufferSize = BUFFER_SIZE;
    nChannels = N_CHANNELS;
    
    recording  = false;
    playing    = true;
    overdubbing = false;
    
    //começando do início
    outpos = 0;
    
    //setting the full loop
    set_full_looping_area();
    
    //inicializando as variáveis não usadas
    volume = 1.f;
    leftpan = 1.f;
    rightpan = 1.f;
    
    //leaves debug as it is! do not modify it!
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
    //if the system will stop overdubing now
    if (overdubbing)
        
        //updates output without interpolation
        update_output_buffer();
    
    overdubbing = !overdubbing;
}


int  Loop::get_size()
{
    return sample.size();
}

//int old_outpos = 0;


void Loop::play(float* &output)
{
    if ((!is_empty()) & playing)
    {
        //interates of buffersize for loading the output
        for(int i=0; i<bufferSize; i++) {
            
            //computing the index
            int   index = i*nChannels;
            
            //computing the index in the output_buf
            int   output_buf_index = (outpos + index)%output_buf.size();
            
            //debug - ERASE ME!
            if (output_buf.size()==0) {
                cout << "output_buf has size of 0" << endl;
                return;
            }
            
            //debug - ERASE ME!
            if (output_buf_index > output_buf.size()) {
                cout << "hey man! you're trying to access an area bigger then output buffer!!!" << endl;
                cout << "      outpos: " << outpos << ";" << endl;
                cout << "      outpos+index: " << outpos+index << ";" << endl;
                cout << "      output_buf.size(): " << output_buf.size() << ";" << endl;
                cout << "      index: " << index << ";" << endl;
                break;
            }
            
            //updates left channel
            output[index  ] += output_buf[output_buf_index] * volume * leftpan;
            
            //in this case, we are getting the value from the right, and feeding the left channel
            //because we are using only the left channel of the focus right
            output[index+1] += output_buf[output_buf_index] * volume * rightpan;
            
            //ideally, this should be the right code
            //output[index+1] += output_buf[outpos + index+1] * volume * rightpan;
            
            //in case there are more channels (eg. x channels), remember to update output[index+x]. so far, this code will only work with n channels.
        }
        
        //getting the old outpos before updating it
        //old_outpos = outpos;
        
        //updating the current position
        outpos = ((outpos + (bufferSize*nChannels))%(end_index));
        
        //if outpos exploded this time, time to return to beginning!
        //if ((old_outpos + (bufferSize*nChannels) > end_index))
        //checks if the outpos if suitable
        if (outpos < start_index || outpos > end_index)
            //goes directly to the start_index
            outpos = start_index;
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
        input_buf.clear();
    }
    
    else       //se tá gravando, finalize
    {
        recording = !recording;
        
        sample=input_buf;
        
        //updates the output buffer
        update_output_buffer();
        
        //sets the loop from its begnning to its end
        set_full_looping_area();
        
        if (debug)
            cout << " loop created! size: " << sample.size() << endl;
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
        
        sample[(outpos + index)%sample.size()]     += input[index  ]   *  leftpan;
        sample[(outpos + index + 1)%sample.size()] += input[index +1 ] * rightpan;
        
    }
}


void Loop::set_head_normalized(float position)
{
    int newHead = position*sample.size();
    
    set_head_absolute(newHead);
}

void Loop::set_head_absolute(int position)
{
    //if newHead is an odd number (right channel), makes it even (left channel)
    if (position%2 != 0)
        position -=1;
    
    //cout << newHead <<endl;
    outpos=position;
}


//gets the sample and sets it to the output
void Loop::update_output_buffer()
{
    output_buf = sample;
    
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
    this->output_buf.clear();
    recording  = false;
    playing    = true;
    overdubbing = false;
    outpos = 0;
}

void Loop::set_debug(bool debug)
{
    this->debug=debug;
}

void Loop::set_volume(float volume)
{
    this->volume=volume;
}


//////////////////////////////////
// sets the sample to fully loop from its beg to its end
//////////////////////////////////
void Loop::set_full_looping_area()
{
    set_looping_area(0, sample.size());
}

//////////////////////////////////
// sets the sample to loop between start and end
//////////////////////////////////
void Loop::set_looping_area(int start, int end)
{
    //if start is an odd number (right channel), makes it even (left channel)
    if (start%2 != 0)
        start -=1;
    
    //if start is an even number (left channel), makes it even (right channel)
    if (end%2 != 0)
        end -=1;
    
    //if (debug)
    //    cout << "start: " << start << " end: " << end <<endl;
    
    start_index = start;
    end_index   = end;
}