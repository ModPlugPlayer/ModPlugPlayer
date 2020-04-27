#include "ModulePlayer.hpp"
#include <QDebug>
#define REAL 0
#define IMAG 1
#include <cmath>

float *ModulePlayer::calculateHanningMultipliers(int N, short itype){
    int half, i, idx, n;
    float *w;

    w = new float[N];
    std::fill(w, w+N, float(0));

    if(itype==1)    //periodic function
        n = N-1;
    else
        n = N;

    if(n%2==0)
    {
        half = n/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (float(1) - cos(float(2)*M_PI*(i+1) / (n+1)));

        idx = half-1;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }
    else
    {
        half = (n+1)/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (float(1) - cos(float(2)*M_PI*(i+1) / (n+1)));

        idx = half-2;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }

    if(itype==1)    //periodic function
    {
        for(i=N-1; i>=1; i--)
            w[i] = w[i-1];
        w[0] = 0.0;
    }
    return w;
}


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

void ModulePlayer::openStream() {
        portaudio::DirectionSpecificStreamParameters outputstream_parameters(portaudio::System::instance().defaultOutputDevice(), 2, portaudio::FLOAT32, false, portaudio::System::instance().defaultOutputDevice().defaultLowOutputLatency(), 0 );
        portaudio::StreamParameters stream_parameters( portaudio::DirectionSpecificStreamParameters::null(), outputstream_parameters, sampleRate, framesPerBuffer, paNoFlag );

        stream.open(stream_parameters, *this, &ModulePlayer::read);
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
            fftInput[i] = left.data()[i] * hanningMultipliers[i];

            //const float * const buffers[2] = { left.data(), right.data() };
            //stream.write( buffers, static_cast<unsigned long>( count ) );
        } catch ( const portaudio::PaException & pa_exception ) {
            if ( pa_exception.paError() != paOutputUnderflowed ) {
                throw;
            }
        }
    }

    fftw_execute(fftPlan); /* repeat as needed */

    double *magnitude = new double[12];
    for(int i=0; i<10; i++){
        magnitude[i] = std::sqrt(fftOutput[i][REAL]*fftOutput[i][REAL] + fftOutput[i][IMAG]*fftOutput[i][IMAG]);
        double magnitude_dB = 20*log10(magnitude[i]);

        //qDebug()<<"Max Magnitude: "<<maxMagnitude<<" FFT Output["<<i<<"] Real: "<<QString::number(fftOutput[i][REAL], 'g', 6) << "Imaginary: "<<fftOutput[i][IMAG]<<" Magnitude: "<<magnitude<<" DB: "<<magnitude_dB;
    }

    emit spectrumAnalyzerData(10, magnitude);

    //qDebug()<<(int)(magnitude[0]*100)<<"\t"<<(int)(magnitude[1]*100)<<"\t"<<100*magnitude[2]<<"\t"<<magnitude[3]<<"\t"<<magnitude[4]<<"\t"<<magnitude[5]<<"\t"<<magnitude[6]<<"\t"<<magnitude[7]<<"\t"<<magnitude[8]<<"\t"<<magnitude[9];
    //qDebug()<<"Count: "<<count;

    if(count==0) {
        stop();
        return PaStreamCallbackResult::paComplete;
    }

    return PaStreamCallbackResult::paContinue;
}


ModulePlayer::ModulePlayer()
{

}

int ModulePlayer::open(std::string fileName, std::size_t bufferSize, int framesPerBuffer, SAMPLERATE sampleRate){
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;
    this->framesPerBuffer = framesPerBuffer;
    this->hanningMultipliers = calculateHanningMultipliers(this->framesPerBuffer);

    fftInput = fftw_alloc_real(bufferSize);
    fftOutput = fftw_alloc_complex(10);

    fftPlan = fftw_plan_dft_r2c_1d(19, fftInput, fftOutput, FFTW_ESTIMATE);

    if (!fftPlan)
       qDebug("plan not created");

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
        openStream();
    } catch ( const std::bad_alloc & ) {
        std::cerr << "Error: " << std::string( "out of memory" ) << std::endl;
        return 1;
    } catch ( const std::exception & e ) {
        std::cerr << "Error: " << std::string( e.what() ? e.what() : "unknown error" ) << std::endl;
        return 1;
    }
    return 0;
}

int ModulePlayer::close()
{
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput); fftw_free(fftOutput);
}

int ModulePlayer::play() {
    try {
        if(!stream.isOpen())
            openStream();
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
    if(stream.isActive()){
        stream.stop();
        qDebug()<<"Stream has been stopped";
        scrubTime(0);
    }
    /*
    if(stream.isOpen()) {
        stream.close();
        qDebug()<<"Stream has been closed";
    }*/
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
