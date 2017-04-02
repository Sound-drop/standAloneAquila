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

int main(int argc, char *argv[])
{
	if (argc < 2)
    {
        std::cout << "Usage: main <FILENAME>" << std::endl;
        return 1;
    }

    Aquila::WaveFile wav(argv[1]);
    std::cout << "Loaded file: " << wav.getFilename()
              << " (" << wav.getBitsPerSample() << "b)" << std::endl;
    Aquila::SampleType maxValue = 0, minValue = 0, average = 0;

    const Aquila::FrequencyType sampleFreq = wav.getSampleFrequency();
    const std::size_t SIZE = 4000;
    const std::size_t END = wav.getSamplesCount();
    //220500
    //cout << END;
    const Aquila::FrequencyType f_lp = 500;

    vector<Aquila::SampleType> chunk;
    for (std::size_t i =END-SIZE; i< END; ++i)
    {	
    	chunk.push_back(wav.sample(i));

    }
    Aquila::SignalSource data(chunk, sampleFreq);
    // std::cout << "Maximum sample value: " << maxValue << std::endl;

	// const Aquila::FrequencyType sampleFreq = 2000;
    //  const Aquila::FrequencyType f1 = 180, f2 = 300;

   

    Aquila::TextPlot plt("input wave");

    //trap??
    //plt.plot(data);


    
    /*
    // input signal parameters
    
  

    Aquila::SquareGenerator generator1(SIZE);
    generator1.setFrequency(f1).setAmplitude(255).generate(64);
    ///Aquila::SquareGenerator generator2(SIZE);
    //generator2.setFrequency(f2).setAmplitude(255).generate(64);
   
    auto sum = generator1;
    // + generator2;

    Aquila::TextPlot plt("Signal waveform before filtration");
    plt.plot(sum);*/

    // calculate the FFT

    //for(auto&x :generator.toArray()) cout<<x;
    
    auto fft = Aquila::FftFactory::getFft(SIZE);
    cout<<"work";
    Aquila::SpectrumType spectrum = fft->fft(data.toArray());
    plt.setTitle("Signal spectrum before filtration");
    //cout << spectrum.size();
    int kk = 0;
    for(auto&x : spectrum){
     cout<<x<<endl;
     kk++;
     cout << kk;
 	}

    // plt.plotSpectrum(spectrum);

    // generate a low-pass filter spectrum
    Aquila::SpectrumType filterSpectrum(SIZE);
    for (std::size_t i = 0; i < SIZE; ++i)
    {
        if (i < (SIZE * f_lp / sampleFreq))
        {
            // passband
            filterSpectrum[i] = 1.0;
        }
        else
        {
            // stopband
            filterSpectrum[i] = 0.0;
        }
    }
    plt.setTitle("Filter spectrum");
    // plt.plotSpectrum(filterSpectrum);

    // the following call does the multiplication of two spectra
    // (which is complementary to convolution in time domain)
    std::transform(
        std::begin(spectrum),
        std::end(spectrum),
        std::begin(filterSpectrum),
        std::begin(spectrum),
        [] (Aquila::ComplexType x, Aquila::ComplexType y) { return x * y; }
    );
    plt.setTitle("Signal spectrum after filtration");
    // plt.plotSpectrum(spectrum);

    // Inverse FFT moves us back to time domain
    double x1[SIZE];
    fft->ifft(spectrum, x1);
    plt.setTitle("Signal waveform after filtration");
    //plt.plot(x1, SIZE);

    return 0;
}
