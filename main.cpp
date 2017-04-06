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

void findMax(Aquila::SpectrumType spectrum, Aquila::FrequencyType sampleFreq)
    {
        std::size_t halfLength = spectrum.size() / 2;
        std::vector<double> absSpectrum(halfLength);
        double max = 0;
        int peak_freq  = 0;
        int start = 15000/((sampleFreq/halfLength)/2);
        for (std::size_t i = start; i < halfLength; ++i)
        {
            absSpectrum[i] = std::abs(spectrum[i]);
            // cout << i*(sampleFreq/halfLength)<< " amp " <<absSpectrum[i] << endl;
            if(absSpectrum[i] > max){ 
                max = absSpectrum[i];
                peak_freq = i*(sampleFreq/halfLength)/2;
            }
        }
        cout << "peak freq for input with sample size: "<< halfLength*2 << " which needs to be pow of 2" <<endl;
        cout <<peak_freq << " Hz max amp:" << max << endl;
        //plot(absSpectrum);
    }

void freqOfindex(std::size_t start, Aquila::WaveFile wav){
    Aquila::FrequencyType sampleFreq = wav.getSampleFrequency();
    vector<Aquila::SampleType> chunk;
    for (std::size_t i =start; i< start+SIZE; ++i)
    {   

        chunk.push_back(wav.sample(i));

    }
    Aquila::SignalSource data(chunk, sampleFreq);

   

    Aquila::TextPlot plt("input wave");

 
    
    auto fft = Aquila::FftFactory::getFft(SIZE);
    cout << "\n\nSignal spectrum of "<<start<< endl;
    Aquila::SpectrumType spectrum = fft->fft(data.toArray());
    plt.setTitle("Signal spectrum of "+ std::to_string(start));


    findMax(spectrum, sampleFreq);
    // plt.plotSpectrum(spectrum);

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
    std::size_t start = 0;

    while(start < END && wav.sample(start) <= 1000 ) start++;
    //220500 = 5*44100
    // cout << END <<endl;
    // cout << start;

    for(int x = start; x < END; x+=END/5) freqOfindex(x, wav);

   
    return 0;
}
