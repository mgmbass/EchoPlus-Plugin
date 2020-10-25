#pragma once

#ifndef __NoiseGenerator__
#define __NoiseGenerator__

#include "fxobjects.h"
#include <iostream>
#include <random>
#include <vector>

enum class NoiseType { whiteNoise, filteredWhiteNoise };

struct NoiseGenData 
{
	NoiseGenData() {}

	double whiteNoiseOut = 0.0;
	double filteredWhiteNoiseOut = 0.0;
	double pinkNoiseOut = 0.0;				///< 90 degrees out
	double filteredPinkNoiseOut = 0.0;	
	double gaussianNoiseOut;
	double filteredgaussianNoiseOut;///< -90 degrees out
};

/**
\struct NoiseGeneratorParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the NoiseGenerator object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
struct NoiseGeneratorParameters
{
	NoiseGeneratorParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	NoiseGeneratorParameters& operator=(const NoiseGeneratorParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		lpf_fc_Hz = params.lpf_fc_Hz;
		outputAmplitude = params.outputAmplitude;

		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	double lpf_fc_Hz= 1000.0;
	double outputAmplitude = 1.0;
};


/**
\class NoiseGenerator
\ingroup FX-Objects
\brief
The NoiseGenerator object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use NoiseGeneratorParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class NoiseGenerator 
{
public:
	NoiseGenerator(void) {}	/* C-TOR */
	~NoiseGenerator(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = (_sampleRate);

		// --- setup the audio filter
		AudioFilterParameters params = lowPassFilter.getParameters();
		params.algorithm = filterAlgorithm::kButterLPF2;
		params.fc = 100.0;
		lowPassFilter.setParameters(params);

		// --- reset
		lowPassFilter.reset(_sampleRate);

		// --- seed random number generator
		srand(time(NULL));

		return true;
	}
	
	/** render a new audio output structure */
	virtual const NoiseGenData renderAudioOutput()
	{
		NoiseGenData generatorOutput;

		// --- noise
		generatorOutput.whiteNoiseOut = doWhiteNoise();
		generatorOutput.filteredWhiteNoiseOut = lowPassFilter.processAudioSample(generatorOutput.whiteNoiseOut);
		generatorOutput.gaussianNoiseOut = doGaussianWhiteNoise();
		generatorOutput.filteredgaussianNoiseOut = lowPassFilter.processAudioSample(generatorOutput.gaussianNoiseOut);

		// --- TODO: add pink and filtered pink noise

		generatorOutput.whiteNoiseOut *= parameters.outputAmplitude;
		generatorOutput.filteredWhiteNoiseOut *= parameters.outputAmplitude;
		generatorOutput.gaussianNoiseOut *= parameters.outputAmplitude;
		generatorOutput.filteredgaussianNoiseOut *= parameters.outputAmplitude;

		return generatorOutput;
	}

	inline double doGaussianWhiteNoise(double mean = 0.0, double variance = 1.0)
	{
		std::default_random_engine defaultGeneratorEngine;
		std::normal_distribution<double> normalDistribution(mean, variance);
		double output = normalDistribution(defaultGeneratorEngine);

		// --- can scale here to change sigma

		return output;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return NoiseGeneratorParameters custom data structure
	*/
	NoiseGeneratorParameters getParameters()
	{
		return parameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param NoiseGeneratorParameters custom data structure
	*/
	void setParameters(const NoiseGeneratorParameters& _params)
	{
		parameters = _params;

		AudioFilterParameters params = lowPassFilter.getParameters();
		params.fc = _params.lpf_fc_Hz;
		lowPassFilter.setParameters(params);
	}

private:
	NoiseGeneratorParameters parameters; ///< object parameters

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate

	// --- smoothing filter
	AudioFilter lowPassFilter;

};

#endif