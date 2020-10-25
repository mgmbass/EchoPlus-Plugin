#pragma once

#ifndef __SystemNoiseGen__
#define __SystemNoiseGen__

#include "fxobjects.h"
#include "LFOEx.h"
#include "noisegen.h"
/**
\struct SystemNoiseGenParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the SystemNoiseGen object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
struct SystemNoiseGenParameters
{
	SystemNoiseGenParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	SystemNoiseGenParameters& operator=(const SystemNoiseGenParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		waveshaperSaturation = params.waveshaperSaturation;
		sixtyHzNoiseAmplitude = params.sixtyHzNoiseAmplitude;
		tapeNoiseFc_Hz = params.tapeNoiseFc_Hz;
		tapeNoiseAmplitude = params.tapeNoiseAmplitude;


		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	double waveshaperSaturation = 1.0;
	double sixtyHzNoiseAmplitude = 1.0;
	double tapeNoiseFc_Hz = 10000.0;
	double tapeNoiseAmplitude = 1.0;	///< init
};


/**
\class SystemNoiseGen
\ingroup FX-Objects
\brief
The SystemNoiseGen object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use SystemNoiseGenParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class SystemNoiseGen : public IAudioSignalGenerator
{
public:
	SystemNoiseGen(void) {}	/* C-TOR */
	~SystemNoiseGen(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = (_sampleRate);
		LFO_ExParameters lparams = lfoEx.getParameters();
		lparams.waveform = generatorWaveform::kSin;
		lparams.frequency_Hz = 60.0;
		lfoEx.setParameters(lparams);
		lfoEx.reset(sampleRate);

		noiseGen.reset(sampleRate);

		// --- do any other per-audio-run inits here

		return true;
	}

	/** render a new audio output structure */
	virtual const SignalGenData renderAudioOutput()
	{
		SignalGenData generatorOutput;
		NoiseGenData a;
		SignalGenData b;
		a = noiseGen.renderAudioOutput();
		b = lfoEx.renderAudioOutput();
		double lfoShaped = 0.0;
		lfoShaped = b.normalOutput;
		lfoShaped = atanWaveShaper(lfoShaped, Sysparameters.waveshaperSaturation);
		generatorOutput.normalOutput = lfoShaped + a.filteredgaussianNoiseOut;
		return generatorOutput;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return SystemNoiseGenParameters custom data structure
	*/
	SystemNoiseGenParameters getParameters()
	{
		return Sysparameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param SystemNoiseGenParameters custom data structure
	*/
	void setParameters(const SystemNoiseGenParameters& _params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU

		LFO_ExParameters lfoParamsAF = lfoEx.getParameters();
		lfoParamsAF.amplitude = Sysparameters.sixtyHzNoiseAmplitude;
		lfoEx.setParameters(lfoParamsAF);

		NoiseGeneratorParameters noiseParams = noiseGen.getParameters();
		noiseParams.lpf_fc_Hz = Sysparameters.tapeNoiseFc_Hz;
		noiseParams.outputAmplitude = Sysparameters.tapeNoiseAmplitude;
		noiseGen.setParameters(noiseParams);
		
		Sysparameters = _params;

		// --- cook parameters here
	}

private:
	SystemNoiseGenParameters Sysparameters; ///< object parameters
	LFO_Ex lfoEx;
	NoiseGenerator noiseGen;

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate

};

#endif