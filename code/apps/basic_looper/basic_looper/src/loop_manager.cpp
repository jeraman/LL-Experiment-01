#include "loop_manager.h"


Loop_Manager::Loop_Manager()
{
    recording  = false;
    playing    = true;
    start_time = ofGetElapsedTimef();
}


//////////////////////////////////
// funcao que é chamada quando existe entrada do microfone
// - input - microfone
// - bufferSize - tamanho do buffer do microfone
// - nChannels - numero de canaix do microfone
//////////////////////////////////


//vector<float> inp_copy;

//--------------------------------------------------------------
void Loop_Manager::audio_input(float * &input, int bufferSize, int nChannels)
{
    if (recording)
    {
        for(int i=0; i<bufferSize; i++) {
            input_buf.push_back(input[ i*nChannels]);
            input_buf.push_back(input[(i*nChannels)+1]);
        }
    }
    
    //this will be executed if only if the current loop is set to overdub
    for (int i = 0; i < loops.size();i++)
        loops[i].overdub_sample_vector(input);
}


//////////////////////////////////
// funcao que é chamada quando existe saida a ser liberada
// - output - caixa de som
// - bufferSize - tamanho do buffer da caixa de som
// - nChannels - numero de canaix da caixa de som
//////////////////////////////////
void Loop_Manager::audio_output(float * &output, int bufferSize, int nChannels)
{
    //debugging variable
    //vector<float> copy;
    
    if (playing & (loops.size() > 0))
    {
        loops[0].play(output, true);    //toca o loop principal
        
        ////////////////////////////////
        //there is a bug in the following commands...
        for (int i = 1; i < loops.size(); i++) //para cada loop...

            if (loops[i].outpos != 0) //toque enquanto não terminar
                loops[i].play(output);
            else
            {
                if (loops[0].outpos == loops[i].inipos) //ou enquanto o principal não começar!
                    loops[i].play(output);
            }
    }

}


//////////////////////////////////
// returns a pointer to the searched loop
// - id - loop's id to be searched
//////////////////////////////////
Loop* Loop_Manager::get_loop(int id)
{
    int ret = search(id);
    if (ret == -1)
        return NULL;
    else
        return &loops[ret];
}

//////////////////////////////////
// returns if it's recording or not
//////////////////////////////////
bool Loop_Manager::is_recording()
{
    return this->recording;
}


//////////////////////////////////
// procura um dado loop no database
// - id - o id do botao dono do loop
//////////////////////////////////
int Loop_Manager::search(int id)
{
    int ret = -1;
    for (int i = 0; i < loops.size(); i++)
        if (loops[i].id == id)
        {
            ret = i;
            break;
        }
    return ret;
}

/////////////////////////////////////////
//performing interpolation in the input buffer to minimize glitch
/////////////////////////////////////////
void Loop_Manager::interpolating_beg_and_end() {
    
    //getting the first chunck
    vector<float> first_chunck(input_buf.begin(), input_buf.begin() + 2*BUFFER_SIZE);
    //getting the last chunk
    vector<float> last_chunck(input_buf.end()-2*BUFFER_SIZE, input_buf.end());
    
    //overlapping the beginning
    //iterating over the BUFFER_SIZE last samples
    for (int a = 0; a < 2*BUFFER_SIZE; a=a+2) {
        //computing the current weight
        float weight     = a/(float)(BUFFER_SIZE*2);
        float inv_weight = 1-weight;
        
        //interpolating between the current sample and the first one (the one we want to morph into)
        float interpolated_value_l = (weight*first_chunck[a])   + (inv_weight*last_chunck[a]);
        float interpolated_value_r = (weight*first_chunck[a+1]) + (inv_weight*last_chunck[a+1]);
        
        //updating values
        input_buf[a  ] = interpolated_value_l;
        input_buf[a+1] = interpolated_value_r;
    }
    
    //cout << input_buf.size()<<endl;
    
    //delete the last chunk
    input_buf.erase(input_buf.end()-(BUFFER_SIZE*2) , input_buf.end());
    
    //cout << input_buf.size()<<endl;
    
    //end of interpolation
    ///////////////////////////////////////////
}

//////////////////////////////////
// grava um determinado loop com o id pedido
// - id - o id do botao dono do loop
//////////////////////////////////
void Loop_Manager::record(int desired_id)
{
    
    if (!recording) //se não tá gravando, comece a gravar
    {
        recording = !recording;
        temp_id = desired_id;

        //playing = false;
        start_time = ofGetElapsedTimef();
        input_buf.clear();

        //pegando o inipos de um loop
        if (loops.size() > 0) //se for um secundario
        {
            Loop* main = &loops[0]; //separa o loop principal
            temp_inipos = main->outpos;
        }
        else   //se for o primeiro a ser inserido...
            temp_inipos = 0;
    }
    else       //se tá gravando, finalize
    {
        if (desired_id == temp_id) //se ele soltou o botao que esta endo gravado
        {
            recording = !recording;
            int time = ofGetElapsedTimef() - start_time;
            //reverse(input_buf.begin(), input_buf.end());

            Loop* old = get_loop(temp_id); //verificando se o loop existe

            //interpolates
            interpolating_beg_and_end();
            
            if (old == NULL) // caso tal loop ainda não exista...
            {
                Loop temp (input_buf, temp_inipos, BUFFER_SIZE, N_CHANNELS, temp_id);
                loops.push_back(temp);
                cout << " loop created in " << loops.size() << "! size: " << input_buf.size() << " time: "<< time << " id: "<< temp_id << endl;
            }
            else //caso tal loop já exista
            {
                old->update_buffer(input_buf, temp_inipos);
                old->resume();
                
                cout << " loop updated in " << loops.size() << "! size: " << input_buf.size() << " time: "<< time << " id: "<< temp_id << endl;
            }
        }
        else
            cout<< "outro botao já está sendo gravado!" << endl;
    }

}

//////////////////////////////////
// da play num determinado loop
// - id - loop's id to be played
//////////////////////////////////
void Loop_Manager::play(int loop)
{
    Loop* result = get_loop(loop);

    if (result == NULL) //senão encontrou
    {
        cout << loop << " não encontrado para tocar!" <<endl;
        return;
    }
    else
    {
        cout<< result->id << " encontrado para tocar!"<< endl;
        result->resume();
    }
}

//////////////////////////////////
// overdubs a certain loop
// - id - loop's id to be overlopped
//////////////////////////////////
void Loop_Manager::overdub(int loop)
{
    Loop* result = get_loop(loop);
    
    if (result == NULL) //senão encontrou
    {
        cout<< loop <<" não encontrado para overdub!"<<endl;
        return;
    }
    else
    {
        cout<< result->id << " encontrado para overdub!"<< endl;
        
        //updating the recording variable
        if (!recording)
            recording=true;
        else
            recording=false;
        
        //calling the overdub function
        result->overdub();
    }
}


//////////////////////////////////
// da stop num determinado loop
// - id - loop's id to be stoped
//////////////////////////////////
void Loop_Manager::stop(int loop)
{
    Loop* result = get_loop(loop);

    if (result == NULL) //senão encontrou
    {
        cout<< loop <<" não encontrado para parar!"<<endl;
        return;
    }
    else
    {
        cout<< result->id << " encontrado para parar!"<< endl;
        result->stop();
    }
}

//////////////////////////////////
// delete a certain loop
// - id - o id do botao dono do loop
//////////////////////////////////
void Loop_Manager::erase(int loop)
{
    int result = search(loop);

    if (result == -1) //senão encontrou
    {
        cout<< loop <<" não encontrado para deletar!"<<endl;
        return;
    }
    else
    {
        cout<< loop << " encontrado para deletar!"<< endl;

        //condicao de deletar o primeiro
        if (result == 0) //se for o primeiro apenas, o mute
            loops[result].stop();
        else //se for outro, apague realmente
            loops.erase(loops.begin()+result);

        //limpando tudo caso não haja mais nenhum loop
        if (loops.size() == 1 && loops[0].muted)
            loops.clear();

        //update o initposition de todos os outros loops!
    }

    cout<< "qde loops: "<< loops.size() << endl;
}


//////////////////////////////////
// sets the head pos of a certain loop
// - id - loops id
// - position (between 0 and 1) for the head
//////////////////////////////////
void Loop_Manager::set_head(int loop, float pos)
{
    Loop* result = get_loop(loop);
    
    if (result == NULL) //senão encontrou
    {

        cout<< loop <<" não encontrado para setar o head!"<<endl;
        return;
    }
    else
    {
        result->set_head(pos);
        //cout<< loop << " encontrado para setar o head!"<< endl;
        //update o initposition de todos os outros loops!
    }
}


//////////////////////////////////
// pops the last recorded loop
//////////////////////////////////
void Loop_Manager::pop()
{
    if (loops.size() > 0)
        loops.pop_back();

    //limpando tudo caso não haja mais nenhum loop
    if (loops.size() == 1 && loops[0].muted == true)
        kill_all();

    cout<< "qde loops: "<< loops.size() << endl;
}


//////////////////////////////////
// clear all the loops
//////////////////////////////////
void Loop_Manager::kill_all()
{
    loops.clear();
    recording=false;
}



