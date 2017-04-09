#include "aquila/global.h"
#include "aquila/source/generator/SquareGenerator.h"
#include "aquila/source/SignalSource.h"
#include "aquila/transform/FftFactory.h"
#include "aquila/tools/TextPlot.h"
#include "aquila/source/WaveFile.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>
#include <cstdlib>
using namespace std;

const std::size_t SIZE = 512;

int findMax(Aquila::SpectrumType spectrum, Aquila::FrequencyType sampleFreq)
    {
        std::size_t halfLength = spectrum.size() / 2;
        std::vector<double> absSpectrum(halfLength);
        double max = 0;
        int peak_freq  = 0;
        //search the band of the freq >= 15000
        int start = 0;
        for (std::size_t i = start; i < halfLength; ++i)
        {
            absSpectrum[i] = std::abs(spectrum[i]);
            // cout << i*(sampleFreq/halfLength)<< " amp " <<absSpectrum[i] << endl;
            if(absSpectrum[i] > max){ 
                max = absSpectrum[i];
                peak_freq = i*(sampleFreq/halfLength)/2;
            }
        }
        //cout << "peak freq for input with sample size: "<< halfLength*2 << " which needs to be pow of 2" <<endl;
        //cout <<peak_freq << " Hz max amp:" << max << endl;
        //plot(absSpectrum);
        return peak_freq;
    }

int freqOfindex(std::size_t start, Aquila::WaveFile wav){
    Aquila::FrequencyType sampleFreq = wav.getSampleFrequency();
    vector<Aquila::SampleType> chunk;
    for (std::size_t i =start; i< start+SIZE; ++i)
    {   

        chunk.push_back(wav.sample(i));

    }
    Aquila::SignalSource data(chunk, sampleFreq);

   

    Aquila::TextPlot plt("input wave");

 
    
    auto fft = Aquila::FftFactory::getFft(SIZE);
    // cout << "\n\nSignal spectrum of time index: "<<start<< endl;
    Aquila::SpectrumType spectrum = fft->fft(data.toArray());
    plt.setTitle("Signal spectrum of "+ std::to_string(start));

    //plt.plotSpectrum(spectrum);
    return findMax(spectrum, sampleFreq);
    

}

int main(int argc, char *argv[])
{
	if (argc < 2)
    {
        std::cout << "Usage: main <FILENAME>" << std::endl;
        return 1;
    }

    Aquila::WaveFile wav(argv[1]);
   

    const std::size_t END = wav.getSamplesCount();
    //const Aquila::FrequencyType f_fp = 15000;
    // Aquila::FrequencyType sampleFreq = wav.getSampleFrequency();
    // int full = SIZE;
    // for(; full*2 < END; full*=2);
    
    // vector<Aquila::SampleType> chunk;
    // for (std::size_t i =0; i<full; ++i)
    // {   

    //     chunk.push_back(wav.sample(i));
    //     //62269 max entry
    //     //if(i>61000 && i< 64000)cout<<wav.sample(i)<< " "<<i << endl;
    // }
    // Aquila::SignalSource dataBeforeFilter(chunk, sampleFreq);
    
    // cout<<"test"<< full<< endl;
    // auto fft = Aquila::FftFactory::getFft(full);
    // Aquila::SpectrumType spectrum = fft->fft(dataBeforeFilter.toArray());

    // Aquila::TextPlot plt("Signal spectrum before filtration");
    // //plt.plotSpectrum(spectrum);

    // Aquila::SpectrumType filterSpectrum(full);
    // //high pass filter
    // for (std::size_t i = 0; i <  full; ++i){
    //     if (i < (full*f_fp / sampleFreq))
    //     {
    //         // stopband
    //         filterSpectrum[i] = 0.0;
    //     }
    //     else
    //     {
    //         // passband
    //         filterSpectrum[i] = 1.0;
    //     }
    // }
    // std::transform(
    //     std::begin(spectrum),
    //     std::end(spectrum),
    //     std::begin(filterSpectrum),
    //     std::begin(spectrum),
    //     [] (Aquila::ComplexType x, Aquila::ComplexType y) { return x * y; }
    // );

    // // Inverse FFT moves us back to time domain
    // double filtered[full];
    // fft->ifft(spectrum, filtered);
    
    // //plt.plot(filtered, END);

    // for(int i=0; i< full; i++ ){
    //   if(std::abs(filtered[i]) > 1000 && i <64000)cout<<filtered[i]<< " "<<i<<endl;
    // }

    std::size_t start = 0;

    while(start < END){
        int peak = freqOfindex(start,wav);
        //cout << peak << " time " << start << endl;
        if (peak >= 15000) break;
        start += SIZE;
    }
    //220500 = 5*44100
    // cout << END <<endl;
    //cout << start;

    for(int x = start; x < END; x+=END/50){ 
        int peak = freqOfindex(x,wav);
        if(peak < 15000 ) break;
        cout << peak << " time " << x << endl;
        
    }

   
    return 0;
}
