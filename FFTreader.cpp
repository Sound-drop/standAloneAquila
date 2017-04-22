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
#define abs_amp 10000 
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
    std::vector<std::vector<int>> data;
    int pre_freq = 0, right = start, pkts = 0, step = sampleFreq/10; 

        vector<int> peak= freqOfindex(right);

#if DEBUG_FLAG
        cout << "@ time " << (double)right/(sampleFreq) << "s"<< endl;

        cout << "Tracked freq (100 Hz): ";
        for(auto&x : peak) cout<< x <<" ";
        cout<<endl;
#endif
        
        if(peak.size()==1 && pre_freq==0 && peak.back() ==211){
            //first 211 chirp
            pre_freq = 211;
            right += step;
        }
        peak= freqOfindex(right);

        if (peak.size()==1 && pre_freq==211 && peak.back() ==211){
            //second 211 chirp
            right += step;
        }

        peak = freqOfindex(right);

        //read pkt lengt
        int cur = 0;
        for(auto&x : peak){
            int shift = x - 195;
            if(shift >=0) cur |= 1 << shift;
        }
        pkts = cur;
        right += step;
        while(pkts-- >0){
            peak = freqOfindex(right);
            int datalen = 0;
            for(auto&x : peak){
                int shift = x - 195;
                if(shift >=0) datalen |= 1 << shift;
            }
            right += step;
            vector<int> pktdata;
            cout <<"Ready to read data len :"<< datalen<< endl;
            while(datalen >0){
                peak = freqOfindex(right);
#if DEBUG_FLAG
        cout << "@ time " << (double)right/(sampleFreq) << "s"<< endl;

        cout << "Tracked freq (100 Hz): ";
        for(auto&x : peak) cout<< x <<" ";
        cout<<endl;
#endif
       
                int content = 0;
                for(auto&x : peak){
                    int shift = x - 195;
                    if(shift >=0) content |= 1 << shift;
                }

                //if odd, right shift padding
                if(datalen==1) content >>= 8;
                pktdata.push_back(content);
                right += step;
                //every data has two bytes
                datalen -= 2;
            }
            data.push_back(pktdata);
        }
        
        const unsigned short _8bitMask  = 0x00FF;
        int pos = 0;
        for(auto& x: data){
            pos++;
            for(auto y :x) {
                // cout<< y <<" ";
                int tmp = y;
                while(tmp > 0){
                    int d = tmp & _8bitMask;    
#if DEBUG_FLAG
                   if(pos==1) cout <<  d <<" ";
                   else cout << (char) d <<" ";
#endif
                    tmp >>= 8;
                }


            }
            cout<<endl;
        }


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
                && absSpectrum[i-1] > abs_amp ){
                 
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