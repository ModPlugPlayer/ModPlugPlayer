#include "ModulePlayer.hpp"
#include <QDebug>

void ModulePlayer::mppParametersChanged(MppParameters &mppParameters) {
    this->mppParameters.update(mppParameters);
}

void logModInfo(openmpt::module *mod) {
    int NumOfOrders = mod->get_num_orders();
    qDebug()<<"Duration: "<<mod->get_duration_seconds()<<endl
           <<"Number of orders: "<< NumOfOrders<<endl;
    for(int i=0; i<NumOfOrders; i++) {
        qDebug()<<"Number of rows in order "<<i<<" is "<<mod->get_pattern_num_rows(mod->get_order_pattern(i));
    }
}

void logCurrentModState(openmpt::module *mod){
    qDebug()<<"Speed: "<<mod->get_current_speed()<<" Tempo: "<< mod->get_current_tempo()
         <<" Order: "<<mod->get_current_order()<<" Pattern: "<< mod->get_current_pattern()
        <<" Row: "<<mod->get_current_row() <<" Duration: "<<mod->get_duration_seconds()
       <<" Position time: "<<mod->get_position_seconds();
}

TimeInfo ModulePlayer::getTimeInfo(){
    TimeInfo timeInfo;
    timeInfo.globalRowIndex = 0;
    if(mod != nullptr) {
        int order = mod->get_current_order();
        int row = mod->get_current_row();
        Row &r = rowsByOrders[order][row];
        if(r.time == -1) {
            r.time = mod->get_position_seconds();
        }
        timeInfo.seconds = r.time;
        timeInfo.globalRowIndex = r.rowGlobalIndex;
    }
    return timeInfo;
}

void ModulePlayer::timeInfoRequested(){
    if(mod != nullptr)
//        this->sendTimeInfo();
    return;
}

int ModulePlayer::read(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags){
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    if(mppParameters.isAnyParameterChanged()) {
        if(mppParameters.isRepeatCountChanged()) {
            mod->set_repeat_count(mppParameters.getRepeatCount());
        }
        if(mppParameters.isInterpolationFilterChanged())
            mod->set_render_param(mod ->RENDER_INTERPOLATIONFILTER_LENGTH, mppParameters.getInterpolationFilter());
        mppParameters.clearChangedFlags();
    }

    std::size_t count = mod->read( sampleRate, framesPerBuffer, left.data(), right.data() );
    for (unsigned int i = 0; i < framesPerBuffer; ++i)
    {
        if ( count == 0 ) {
            break;
        }
        try {
            out[0][i] = left.data()[i]*volume;
            out[1][i] = right.data()[i]*volume;

            //const float * const buffers[2] = { left.data(), right.data() };
            //stream.write( buffers, static_cast<unsigned long>( count ) );
        } catch ( const portaudio::PaException & pa_exception ) {
            if ( pa_exception.paError() != paOutputUnderflowed ) {
                throw;
            }
        }
    }

    if(count==0)
        return PaStreamCallbackResult::paComplete;
    return PaStreamCallbackResult::paContinue;
}


ModulePlayer::ModulePlayer()
{
}

int ModulePlayer::open(std::string fileName, std::size_t bufferSize, int framesPerBuffer, SAMPLERATE sampleRate){
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;
    this->framesPerBuffer = framesPerBuffer;
    try {
        left.clear();
        right.clear();
        left.reserve(bufferSize);
        right.reserve(bufferSize);
        std::ifstream file(fileName, std::ios::binary );
        mod = new openmpt::module( file );

        this->rows.clear();
        int numOfOrders = mod->get_num_orders();
        int totalRowAmount = 0;

        for(int i=0; i<numOfOrders; i++) {
            int rowAmountOfCurrentOrder = mod->get_pattern_num_rows(mod->get_order_pattern(i));
            totalRowAmount += rowAmountOfCurrentOrder;
            for(int j=0; j<rowAmountOfCurrentOrder; j++){
                Row r;
                r.rowIndex = j;
                r.orderIndex = i;
                r.rowGlobalIndex = rows.size();
                r.time = -1;
                rows.push_back(r);
            }
        }

        rowsByOrders.clear();
        rowsByOrders.reserve(numOfOrders);
        for(int i=0; i<numOfOrders; i++){
            std::vector<Row> v;
            rowsByOrders.push_back(v);
        }

        for(Row &r : rows){
            rowsByOrders[r.orderIndex].push_back(r);
        }

        emit(timeTicksAmountChanged(rows.size()));
        //portaudio::AutoSystem portaudio_initializer;
        portaudio::DirectionSpecificStreamParameters outputstream_parameters(portaudio::System::instance().defaultOutputDevice(), 2, portaudio::FLOAT32, false, portaudio::System::instance().defaultOutputDevice().defaultLowOutputLatency(), 0 );
        portaudio::StreamParameters stream_parameters( portaudio::DirectionSpecificStreamParameters::null(), outputstream_parameters, sampleRate, framesPerBuffer, paNoFlag );
        stream.open(stream_parameters, *this, &ModulePlayer::read);
    } catch ( const std::bad_alloc & ) {
        std::cerr << "Error: " << std::string( "out of memory" ) << std::endl;
        return 1;
    } catch ( const std::exception & e ) {
        std::cerr << "Error: " << std::string( e.what() ? e.what() : "unknown error" ) << std::endl;
        return 1;
    }
    return 0;
}

int ModulePlayer::play() {
    try {
        logModInfo(mod);
        stream.start();

        /* when this block runs, it freezes the player window.
        while(stream.isActive())
            portaudio::System::instance().sleep(100);

        stream.stop();
        qDebug()<<"Stopped";

        stream.close();
        */
    } catch ( const std::bad_alloc & ) {
        std::cerr << "Error: " << std::string( "out of memory" ) << std::endl;
        return 1;
    } catch ( const std::exception & e ) {
        std::cerr << "Error: " << std::string( e.what() ? e.what() : "unknown error" ) << std::endl;
        return 1;
    }
    return 0;
}

int ModulePlayer::stop() {
    if(stream.isActive())
        stream.stop();
    qDebug()<<"Stream has been stopped";
    if(stream.isOpen()) {
        stream.close();
        qDebug()<<"Stream has been closed";
    }
    return 0;
}

int ModulePlayer::pause()
{
    qDebug()<<"Stream has been paused";
    stream.stop();
}

int ModulePlayer::resume()
{
    stream.start();
}

void ModulePlayer::scrubTime(int rowGlobalId){
    Row r = rows[rowGlobalId];
    mod->set_position_order_row(r.orderIndex, r.rowIndex);

    //double seconds = mod->get_duration_seconds()/(double)rows.size()*(double)rowGlobalId;
    //mod->set_position_seconds(seconds);
}

void ModulePlayer::setVolume(double volume){
    this->volume = volume;
}

void ModulePlayer::run(){
    this->play();
}
