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

            //if(round_freq > highpass) cout << round_freq<< " amp " << absSpectrum[i-1] << endl;
            if(round_freq > highpass && absSpectrum[i-2] < absSpectrum[i-1] && absSpectrum[i-1] > absSpectrum[i] 
                && absSpectrum[i-1] > 800000 ){
                 
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
    // std::cout << "Filename: "           << wav.getFilename();
    // std::cout << "\nLength: "           << wav.getAudioLength()     << " ms";
    // std::cout << "\nSample frequency: " << wav.getSampleFrequency() << " Hz";
    // std::cout << "\nChannels: "         << wav.getChannelsNum();
    // std::cout << "\nByte rate: "        << wav.getBytesPerSec()/1024 << " kB/s";
    // std::cout << "\nBits per sample: "  << wav.getBitsPerSample() << "b\n";

    const std::size_t END = wav.getSamplesCount();
    const std::size_t sampleFreq = wav.getSampleFrequency(); 

    std::size_t start = 0;


    while(start < END){
        vector<int> peak= freqOfindex(start,wav, SIZE);
        // cout <<"time " << start << endl;
        if (peak.size() ==1){
        //     // cout << "Tracked freq (100 Hz): ";
        //     for(auto&x : peak) cout<< x <<" ";
        //     cout<<endl;
            
         break;
        }
        start += SIZE;
    }
    //220500 = 5*44100
    // cout << "total:"<< END <<endl;
    // cout << "Starting at time: "<< start << endl;
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
    std::vector<int> data;
    bool endchirp = false;
    int pre_freq = 0, right = start; 
    while(1){
        vector<int> peak= freqOfindex(right,wav, SIZE);
        cout << "@ time " << (double)right/(sampleFreq) << "s"<< endl;

        cout << "Tracked freq (100 Hz): ";
        for(auto&x : peak) cout<< x <<" ";
        cout<<endl;
   
        if (peak.size() != 1 ){
             if(pre_freq == 211) endchirp = true;
             int cur = 0;
             for(auto&x : peak){

                int shift = x - 195;
                if(shift >=0) cur |= 1 << shift;
             }
             data.push_back(cur);

        //separator or start/end
        }else{
            if(pre_freq==0) pre_freq = peak.back();
            else if(pre_freq == 211 && !endchirp) endchirp = true;
            else if(peak.back()==211 && endchirp) break;

        }
        right += sampleFreq/10;
    }


    const unsigned short _8bitMask  = 0x00FF;
    int read_bytes = 0;
    for(auto x : data){
        int tmp = x;
       
        if(read_bytes==4 || read_bytes==10 ) cout << endl;
        if(read_bytes < 4){
            while(tmp>0){
            cout <<  (tmp & _8bitMask) <<" ";
            tmp >>= 8;
            }
        }else{
            tmp = x;
            while(tmp>0){
            cout <<  (char)(tmp & _8bitMask);
            tmp >>= 8;
            }
        }
        read_bytes += 2;


    }
    cout << endl;
   
    return 0;
}
