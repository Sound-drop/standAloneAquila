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

void plotSpectrum(Aquila::SpectrumType spectrum, Aquila::FrequencyType sampleFreq)
    {
        std::size_t halfLength = spectrum.size() / 2;
        std::vector<double> absSpectrum(halfLength);
        double max = 0;
        int peak_freq  = 0;
        for (std::size_t i = 0; i < halfLength; ++i)
        {
            absSpectrum[i] = std::abs(spectrum[i]);
            //cout << i*(sampleFreq/halfLength)<< " amp " <<absSpectrum[i] << endl;
            if(absSpectrum[i] > max){ 
                max = absSpectrum[i];
                peak_freq = i*(sampleFreq/halfLength)/2;
            }
        }
        cout << "\n\npeak freq for input with sample size: "<< halfLength*2 << " which needs to be pow of 2" <<endl;
        cout <<peak_freq << " Hz max amp:" << max << endl;
        //plot(absSpectrum);
    }

int main(int argc, char *argv[])
{
	if (argc < 2)
    {
        std::cout << "Usage: main <FILENAME>" << std::endl;
        return 1;
    }

    Aquila::WaveFile wav(argv[1]);


    const Aquila::FrequencyType sampleFreq = wav.getSampleFrequency();
    const std::size_t SIZE = 64;
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

 
    
    auto fft = Aquila::FftFactory::getFft(SIZE);
    Aquila::SpectrumType spectrum = fft->fft(data.toArray());
    plt.setTitle("Signal spectrum before filtration");


    plotSpectrum(spectrum, sampleFreq);
    plt.plotSpectrum(spectrum);

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
