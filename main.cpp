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

const std::size_t SIZE = 1024;


vector<int> findMax(Aquila::SpectrumType spectrum, Aquila::FrequencyType sampleFreq)
    {
        std::size_t halfLength = spectrum.size() / 2;
        std::vector<double> absSpectrum(halfLength);
        double max = 0;
        int peak_freq  = 0;
        std::vector<int> ret;
        
        //search the band of the freq >= 15000
        int start = 0;
        int highpass = 190;
        for (std::size_t i = start; i < halfLength; ++i)
        {
            absSpectrum[i] = std::abs(spectrum[i]);
            int round_freq = (int)((i-1)*(sampleFreq/halfLength)/2 + 50) /100;

            // if(round_freq > highpass) cout << round_freq<< " amp " << absSpectrum[i-1] << endl;
            if(round_freq > highpass && absSpectrum[i-2] < absSpectrum[i-1] && absSpectrum[i-1] > absSpectrum[i] 
                && absSpectrum[i-2]> 10000 && absSpectrum[i-1]> 10000 && absSpectrum[i]> 10000 ){
                 
                 ret.push_back(round_freq);
                 // cout << round_freq<< " amp " <<absSpectrum[i-1] << endl;
            
            }
            
            if(absSpectrum[i] > max){ 
                max = absSpectrum[i];
                peak_freq = round_freq;
            }
        }
        //cout << "peak freq for input with sample size: "<< halfLength*2 << " which needs to be pow of 2" <<endl;
        //cout <<peak_freq << " Hz max amp:" << max << endl;
        //plot(absSpectrum);
        // if(peak_freq < 190) ret.clear();

        return ret;
    }

vector<int> freqOfindex(std::size_t start, Aquila::WaveFile wav, int size){
    Aquila::FrequencyType sampleFreq = wav.getSampleFrequency();
    vector<Aquila::SampleType> chunk;
    for (std::size_t i =start; i< start+size; ++i)
    {   

        chunk.push_back(wav.sample(i));

    }
    Aquila::SignalSource data(chunk, sampleFreq);

   

    Aquila::TextPlot plt("input wave");

 
    
    auto fft = Aquila::FftFactory::getFft(size);
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
    std::cout << "Filename: "           << wav.getFilename();
    std::cout << "\nLength: "           << wav.getAudioLength()     << " ms";
    std::cout << "\nSample frequency: " << wav.getSampleFrequency() << " Hz";
    std::cout << "\nChannels: "         << wav.getChannelsNum();
    std::cout << "\nByte rate: "        << wav.getBytesPerSec()/1024 << " kB/s";
    std::cout << "\nBits per sample: "  << wav.getBitsPerSample() << "b\n";

    const std::size_t END = wav.getSamplesCount();
    const std::size_t sampleFreq = wav.getSampleFrequency(); 

    std::size_t start = 0;


    while(start < END){
        vector<int> peak= freqOfindex(start,wav, SIZE/2);
        // cout <<"time " << start << endl;
        if (peak.size() ==1){
            cout << "Tracked freq (100 Hz): ";
            for(auto&x : peak) cout<< x <<" ";
            cout<<endl;
            
         break;
        }
        start += SIZE/2;
    }
    //220500 = 5*44100
    cout << "total:"<< END <<endl;
    cout << "Starting at time: "<< start << endl;
    int right = start, two_block = right+2*(sampleFreq/10);
    // vector<int> peak= freqOfindex(start+SIZE/2,wav, SIZE/2);
    // cout << "Tracked freq (100 Hz): ";
    // for(auto&x : peak) cout<< x <<" ";
    // cout<<endl;
    // int left = start - SIZE, right = start;
    // while(left < right){
    //     int mid = (left+right)/2;
    //     vector<int> peak= freqOfindex(mid,wav,SIZE/2);

    //     if(peak.size() == 0) left = mid+1;
    //     else if(peak.size() > 1) right = mid;
    
    // }
    // cout << "located peak: "<< right << endl;
    // right = 67914;
    while(right <= two_block){
        vector<int> peak= freqOfindex(right,wav, SIZE);
        cout << " time " << right << endl;

        cout << "Tracked freq (100 Hz): ";
        for(auto&x : peak) cout<< x <<" ";
        cout<<endl;
   
        if (peak.size() == 1 ) break;

        right += sampleFreq/10;
    }
   
    return 0;
}
