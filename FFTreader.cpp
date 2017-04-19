#include "aquila/global.h"
#include "aquila/source/SignalSource.h"
#include "aquila/transform/FftFactory.h"
#include "aquila/source/WaveFile.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>
#include <cstdlib>
#include "FFTreader.hpp"
using namespace std;

#define DEBUG_FLAG     (1) 

void FFTreader::parse(){
    std::size_t start = 0;


    while(start < END){
        vector<int> peak= freqOfindex(start);

        if (peak.size() ==1){
         break;
        }
        start += SIZE;
    }
    std::vector<int> data;
    bool endchirp = false;
    int pre_freq = 0, right = start; 
    while(1){
        vector<int> peak= freqOfindex(right);

#if DEBUG_FLAG
        cout << "@ time " << (double)right/(sampleFreq) << "s"<< endl;

        cout << "Tracked freq (100 Hz): ";
        for(auto&x : peak) cout<< x <<" ";
        cout<<endl;
#endif
   
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
                int d = tmp & _8bitMask;    
#if DEBUG_FLAG
                cout <<  d <<" ";
#endif
                tmp >>= 8;
            }
        }else{
            tmp = x;
            while(tmp>0){
                 char c = (char)(tmp & _8bitMask);
#if DEBUG_FLAG
                cout <<  c;
#endif
                tmp >>= 8;
            }
        }
        read_bytes += 2;


    }
    cout << endl;

}

vector<int> FFTreader::findMax(Aquila::SpectrumType spectrum){
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

vector<int> FFTreader::freqOfindex(std::size_t start){

    vector<Aquila::SampleType> chunk;
    for (std::size_t i =start; i< start+SIZE; ++i)
    {   

        chunk.push_back(wav.sample(i));

    }
    Aquila::SignalSource data(chunk, sampleFreq);

   

 
    
    auto fft = Aquila::FftFactory::getFft(SIZE);
    // cout << "\n\nSignal spectrum of time index: "<<start<< endl;
    Aquila::SpectrumType spectrum = fft->fft(data.toArray());
 
    //plt.plotSpectrum(spectrum);
    return findMax(spectrum);
    

}