#pragma once

#ifndef __TrippleLFO__
#define __TrippleLFO__

#include "fxobjects.h"
#include "LFOEx.h"

/**
\struct TrippleLFOParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the TrippleLFO object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
struct TrippleLFOParameters
{
	TrippleLFOParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	TrippleLFOParameters& operator=(const TrippleLFOParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		for (int i = 0; i < 3; i++) {
			lfoFrequency_Hz[i] = params.lfoFrequency_Hz[i];
			lfoStartPhase[i] = params.lfoStartPhase[i];
			lfoAmplitude[i] = params.lfoAmplitude[i];
		}
		outputAmplitude = params.outputAmplitude;

		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	double lfoFrequency_Hz[3] = { 2.5, 5.0, 26.0 };
	double lfoStartPhase[3] = { 0.0, 0.0, 0.0 };
	double lfoAmplitude[3] = { 0.0, 0.0, 0.0 };
	double outputAmplitude = 1.0; // not dB///< init
};


/**
\class TrippleLFO
\ingroup FX-Objects
\brief
The TrippleLFO object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use TrippleLFOParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class TrippleLFO : public IAudioSignalGenerator
{
public:
	TrippleLFO(void) {}	/* C-TOR */
	~TrippleLFO(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = (_sampleRate);

		for (int i = 0; i < 3; i++) {
			LFO_ExParameters params = lfoEx[i].getParameters();
			params.waveform = generatorWaveform::kSin;
			double phaseOffset = doWhiteNoise();
			phaseOffset = bipolarToUnipolar(phaseOffset); // [0, 1]
			params.startPhase = phaseOffset;
			lfoEx[i].setParameters(params);
			lfoEx[i].reset(sampleRate);
		}
		AudioFilterParameters filterParams = highPassFilter.getParameters();
		filterParams.algorithm = filterAlgorithm::kHPF2;
		filterParams.fc = 0.01;
		highPassFilter.setParameters(filterParams);
		highPassFilter.reset(sampleRate);

		// --- do any other per-audio-run inits here

		return true;
	}

	/** render a new audio output structure */
	virtual const SignalGenData renderAudioOutput()
	{
		SignalGenData generatorOutput;
		double y = 0.0;
		SignalGenData a = lfoEx[0].renderAudioOutput();
		SignalGenData b = lfoEx[1].renderAudioOutput();
		SignalGenData c = lfoEx[2].renderAudioOutput();
		y = (a.normalOutput * 0.33) + (b.normalOutput * 0.33) + (c.normalOutput * 0.33);
		generatorOutput.normalOutput = highPassFilter.processAudioSample(y);
		generatorOutput.normalOutput *= Tparameters.outputAmplitude;
		return generatorOutput;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return TrippleLFOParameters custom data structure
	*/
	TrippleLFOParameters getParameters()
	{
		return Tparameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param TrippleLFOParameters custom data structure
	*/
	void setParameters(const TrippleLFOParameters& _params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU
		for (int i = 0; i < 3; i++) {
			LFO_ExParameters paramsAF = lfoEx[i].getParameters();
			paramsAF.amplitude = Tparameters.lfoAmplitude[i];
			paramsAF.frequency_Hz = Tparameters.lfoFrequency_Hz[i];
			lfoEx[i].setParameters(paramsAF);
		}

		Tparameters = _params;

		// --- cook parameters here
	}

private:
	TrippleLFOParameters Tparameters; ///< object parameters
	LFO_Ex lfoEx[3];
	AudioFilter highPassFilter;

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate

};
#endif
