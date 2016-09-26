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
    if (overdubbing) //if the system will stop overdubing now
        update_output_buffer(false); //updates output without interpolation
    
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
        if (outpos > output_buf.size())
            cout << "hey man! output is bigger then output buffer!!!" << endl;
        
        for(int i=0; i<bufferSize; i++) {

            //computing the index
            int   index = i*nChannels;
            
            if (output_buf.size()==0) {
                cout << "output_buf has size of 0" << endl;
                return;
            }
            
            output[index  ] += output_buf[(outpos + index)%output_buf.size()] * volume * leftpan;
            //in this case, we are getting the value from the right, and feeding the left channel
            //because we are using only the left channel of the focus right
            output[index+1] += output_buf[(outpos + index)%output_buf.size()] * volume * rightpan;
            //this should be the right code
            //output[index+1] += output_buf[(outpos + index+1)%output_buf.size()] * volume * rightpan;
            
            //in case there are more channels (eg. x channels), remember to update output[index+x]. so far, this code will only work with n channels. 
        }
    
        outpos = ((outpos + bufferSize*nChannels)%(output_buf.size()));
        

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
        
        //updates the output buffer with interpolation
        update_output_buffer(true);
        
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
        
        sample[outpos + index]      += input[index  ]   *  leftpan;
        sample[outpos + index + 1 ] += input[index +1 ] * rightpan;
        
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

//////////////////////////////////
//updates the output buffer, performing as well the required interpolation
// - begin - init position from the sample vector
// - end   - end position from the sample vector
//////////////////////////////////
void Loop::update_output_buffer(int begin, int end, bool needs_interpolate)
{
    vector<float>::const_iterator first = sample.begin() + begin;
    vector<float>::const_iterator last  = sample.begin() + end;
    
    //selecting the subarray that will become the output
    vector<float> newoutput(first, last);
    
    //interpolates if needed and stores the new array in the output
    
    if (needs_interpolate)
        output_buf = interpolating_beg_and_end(newoutput);
    else
        output_buf = newoutput;
        //output_buf = sample;
    
    if (debug)
        cout << "updating the output buffer! new beg: " << begin << " new end: " << end <<  endl;
    
}

//we can do it faster, read the whole storage and add it to the output
void Loop::update_output_buffer(bool needs_interpolate)
{
    update_output_buffer(0, sample.size(), needs_interpolate);
    
}

//////////////////////////////////
//interpolation of the vector newoutput
//////////////////////////////////
vector<float> Loop::interpolating_beg_and_end(vector<float> &newoutput) {
    /*
    /////////////////
    // ONE BUFFER SIZE IN THE BEGINING ANOTHER IN THE END
     
    //getting the first chunck
    vector<float> first_chunck(newoutput.begin(), newoutput.begin() + bufferSize);
    //getting the last chunk
    vector<float> last_chunck(newoutput.end()-bufferSize, newoutput.end());
    
    //overlapping the beginning
    //iterating over the BUFFER_SIZE last samples
    for (int a = 0; a < bufferSize; a=a+2) {
        //computing the current weight
        float weight     = a/(float)(bufferSize);
        float inv_weight = 1-weight;
        
        //interpolating between the current sample and the first one (the one we want to morph into)
        float interpolated_value_l = (weight*first_chunck[a])   + (inv_weight*last_chunck[a]);
        float interpolated_value_r = (weight*first_chunck[a+1]) + (inv_weight*last_chunck[a+1]);
        
        //updating values
        newoutput[a  ] = interpolated_value_l;
        newoutput[a+1] = interpolated_value_r;
    }
    
    //delete the last chunk
    newoutput.erase(newoutput.end()-(bufferSize) , newoutput.end());
    
    return newoutput;
    
    //end of interpolation
    ///////////////////////////////////////////
     */
    
    // TWO BUFFER SIZE IN THE BEGINING ANOTHER IN THE END
    //getting the first chunck
    vector<float> first_chunck(newoutput.begin(), newoutput.begin() + 2*bufferSize);
    //getting the last chunk
    vector<float> last_chunck(newoutput.end()-2*bufferSize, newoutput.end());
    
    //overlapping the beginning
    //iterating over the BUFFER_SIZE last samples
    for (int a = 0; a < 2*bufferSize; a=a+2) {
        //computing the current weight
        float weight     = a/(float)(bufferSize*2);
        float inv_weight = 1-weight;
        
        //interpolating between the current sample and the first one (the one we want to morph into)
        float interpolated_value_l = (weight*first_chunck[a])   + (inv_weight*last_chunck[a]);
        float interpolated_value_r = (weight*first_chunck[a+1]) + (inv_weight*last_chunck[a+1]);
        
        //updating values
        newoutput[a  ] = interpolated_value_l;
        newoutput[a+1] = interpolated_value_r;
    }
    
    //delete the last chunk
    newoutput.erase(newoutput.end()-(bufferSize*2) , newoutput.end());
    
    return newoutput;
    
    //end of interpolation
    ///////////////////////////////////////////
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