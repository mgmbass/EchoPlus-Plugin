#pragma once

#ifndef __EchoplexDelayModulator__
#define __EchoplexDelayModulator__

#include "fxobjects.h"
#include "TrippleLFO.h"
#include "UniversalComb.h"
#include "SystemNoiseGen.h"

/**
\struct EchoplexDelayModulatorParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the EchoplexDelayModulator object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
struct EchoplexDelayModulatorParameters
{
	EchoplexDelayModulatorParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	EchoplexDelayModulatorParameters& operator=(const EchoplexDelayModulatorParameters& params)	// need this override for collections to work
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
		lfoDepth_Pct = params.lfoDepth_Pct;
		noiseDepth_Pct = params.noiseDepth_Pct;
		noiseFilterFc_Hz = params.noiseFilterFc_Hz;
		noiseFilterAmplitude = params.noiseFilterAmplitude;
		combFilterDelayTime_mSec = params.combFilterDelayTime_mSec;
		delayTime = params.delayTime;
		noiseSaturation = params.noiseSaturation;
		sixtyHzNoiseAmp = params.sixtyHzNoiseAmp;
		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	double lfoFrequency_Hz[3] = { 2.5, 5.0, 26.0 };
	double lfoStartPhase[3] = { 0.0, 0.0, 0.0 };
	double lfoAmplitude[3] = { 0.5, 0.5, 0.5 };
	double lfoDepth_Pct = 50.0; // voice the plugin for default val
	double noiseDepth_Pct = 50.0; // voice the plugin for default val
	double noiseFilterFc_Hz = 100.0; // voice the plugin for default val
	double noiseFilterAmplitude = 0.5; // voice the plugin for default val
	double combFilterDelayTime_mSec = 10.0;
	double delayTime = 0.0;// --- 90.0 to 680mSec for me
	double noiseSaturation = 1.0;
	double sixtyHzNoiseAmp = 0.5;
	double noiseOut = 1.0;
};


/**
\class EchoplexDelayModulator
\ingroup FX-Objects
\brief
The EchoplexDelayModulator object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use EchoplexDelayModulatorParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class EchoplexDelayModulator : public IAudioSignalGenerator
{
public:
	EchoplexDelayModulator(void) {}	/* C-TOR */
	~EchoplexDelayModulator(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = (_sampleRate);
		capstanPinchModulator.reset(sampleRate); //sets random lfo start phase
		TrippleLFOParameters Tparams = capstanPinchModulator.getParameters();
		for (int i = 0; i < 3; i++) {
			Tparams.lfoFrequency_Hz[i] = parameters.lfoFrequency_Hz[i];
			capstanPinchModulator.setParameters(Tparams);
		}
		scallopingFilter.reset(sampleRate);
		UCombFilterParameters combparams = scallopingFilter.getParameters();
		combparams.combFilterType = CombFilterType::inverseCombFilter;
		scallopingFilter.setParameters(combparams);

		lfDriftModulator.reset(sampleRate);

		// --- do any other per-audio-run inits here

		return true;
	}

	/** render a new audio output structure */
	virtual const SignalGenData renderAudioOutput()
	{
		SignalGenData generatorOutput;
		double lfoDepth = parameters.lfoDepth_Pct / 100;
		double noiseDepth = parameters.noiseDepth_Pct / 100;
		double normalDelay = normalizeValue(parameters.delayTime, 90, 680);
		double polyMap = calculateDriftDepth(normalDelay);
		SignalGenData lfDrift = lfDriftModulator.renderAudioOutput();
		SignalGenData lfoMod = capstanPinchModulator.renderAudioOutput();
		double noiseModer = doBipolarModulation(lfDrift.normalOutput * noiseDepth * polyMap, -5.0, 5.0);
		double lfoModer = doBipolarModulation(lfoMod.normalOutput * lfoDepth, -5.0, 5.0);
		generatorOutput.normalOutput = lfDrift.normalOutput * noiseModer + lfoMod.normalOutput * lfoModer;
		generatorOutput.normalOutput = scallopingFilter.processAudioSample(generatorOutput.normalOutput) + parameters.delayTime;
		return generatorOutput;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return EchoplexDelayModulatorParameters custom data structure
	*/
	EchoplexDelayModulatorParameters getParameters()
	{
		return parameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param EchoplexDelayModulatorParameters custom data structure
	*/
	void setParameters(const EchoplexDelayModulatorParameters& _params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU
		TrippleLFOParameters TparamsAF = capstanPinchModulator.getParameters();
		for (int i = 0; i < 3; i++) {
			TparamsAF.lfoAmplitude[i] = parameters.lfoAmplitude[i];
			capstanPinchModulator.setParameters(TparamsAF);
		}
		SystemNoiseGenParameters noiseparams = lfDriftModulator.getParameters();
		noiseparams.tapeNoiseFc_Hz = parameters.noiseFilterFc_Hz;
		noiseparams.tapeNoiseAmplitude = parameters.noiseFilterAmplitude;
		noiseparams.sixtyHzNoiseAmplitude = parameters.sixtyHzNoiseAmp;
		noiseparams.waveshaperSaturation = parameters.noiseSaturation;
		lfDriftModulator.setParameters(noiseparams);

		double mappedValue = parameters.delayTime;
		// --- call the mapping function
		mappedValue = mapDoubleValue(mappedValue, 90.0, 680.0, 0.5, 5.0);
		UCombFilterParameters ucombparamsAF = scallopingFilter.getParameters();
		ucombparamsAF.delayTime_mSec = mappedValue;
		scallopingFilter.setParameters(ucombparamsAF);

		parameters = _params;

	}

protected:
	double calculateDriftDepth(double normalizedDelayTime)
	{
		double y = 1 - 4.646429 * normalizedDelayTime + 7.767857 * normalizedDelayTime;
			return y;
	}

	inline double normalizeValue(double value, double min, double max)
	{
		return (value - min) / (max - min);
	}

	double mapDoubleValue(double mapped, double minIn, double maxIn, double minOut, double maxOut) {
		return (mapped) / (maxIn / (maxOut - minIn)) + minOut;
	}

private:
	EchoplexDelayModulatorParameters parameters; ///< object parameters
	// --- generates the three LFOs
	TrippleLFO capstanPinchModulator;
	// --- filtered noise generator
	SystemNoiseGen lfDriftModulator;
	// --- for "scalloping"
	UCombFilter scallopingFilter;
	// --- local variables used by this object
	double sampleRate = 0.0; ///< sample rate


};

#endif