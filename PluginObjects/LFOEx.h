#pragma once

#ifndef __LFO_Ex__
#define __LFO_Ex__

#include "fxobjects.h"

/**
\struct LFO_ExParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the LFO_Ex object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
struct LFO_ExParameters : public OscillatorParameters
{
	LFO_ExParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	LFO_ExParameters& operator=(const LFO_ExParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		startPhase = params.startPhase;
		amplitude = params.amplitude;
		OscillatorParameters::operator=(params);

		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	double startPhase = 0.0;	///< init
	double amplitude = 1.0;
	
};


/**
\class LFO_Ex
\ingroup FX-Objects
\brief
The LFO_Ex object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use LFO_ExParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class LFO_Ex : public LFO
{
public:
	LFO_Ex(void) {}	/* C-TOR */
	~LFO_Ex(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = _sampleRate;
		LFO::reset(_sampleRate);
		phaseInc = lfoParameters.frequency_Hz / sampleRate;
		// --- timebase variables
		modCounter = Exparameters.startPhase;///< start phase
		modCounterQP = Exparameters.startPhase + 0.25; ///< QP version
		if (modCounterQP > 1.0) ///< check and wrap modulo if needed
			modCounterQP -= 1.0;
	
		// --- do any other per-audio-run inits here

		return true;
	}

	/** render a new audio output structure */
	virtual const SignalGenData renderAudioOutput()
	{
		// --- always first!
		checkAndWrapModulo(modCounter, phaseInc);

		// --- QP output always follows location of current modulo; first set equal
		modCounterQP = modCounter;

		// --- then, advance modulo by quadPhaseInc = 0.25 = 90 degrees, AND wrap if needed
		advanceAndCheckWrapModulo(modCounterQP, 0.25);

		SignalGenData output;
		generatorWaveform waveform = lfoParameters.waveform;

		// --- calculate the oscillator value
		if (waveform == generatorWaveform::kSin)
		{
			// --- calculate normal angle
			double angle = modCounter * 2.0 * kPi - kPi;

			// --- norm output with parabolicSine approximation
			output.normalOutput = parabolicSine(-angle) * Exparameters.amplitude;

			// --- calculate QP angle
			angle = modCounterQP * 2.0 * kPi - kPi;

			// --- calc QP output
			output.quadPhaseOutput_pos = parabolicSine(-angle) * Exparameters.amplitude;
		}
		else if (waveform == generatorWaveform::kTriangle)
		{
			// triv saw
			output.normalOutput = unipolarToBipolar(modCounter) ;

			// bipolar triagle
			output.normalOutput = (2.0 * fabs(output.normalOutput) - 1.0) * Exparameters.amplitude;

			// -- quad phase
			output.quadPhaseOutput_pos = unipolarToBipolar(modCounterQP);

			// bipolar triagle
			output.quadPhaseOutput_pos = (2.0 * fabs(output.quadPhaseOutput_pos) - 1.0) * Exparameters.amplitude;
		}
		else if (waveform == generatorWaveform::kSaw)
		{
			output.normalOutput = unipolarToBipolar(modCounter) * Exparameters.amplitude;
			output.quadPhaseOutput_pos = unipolarToBipolar(modCounterQP) * Exparameters.amplitude;
		}

		// --- invert two main outputs to make the opposite versions
		output.quadPhaseOutput_neg = -output.quadPhaseOutput_pos;
		output.invertedOutput = -output.normalOutput;

		// --- setup for next sample period
		advanceModulo(modCounter, phaseInc);

		return output;
	}

	/** get parameters: note use of custom structure for passing param data */
	/**
	\return LFO_ExParameters custom data structure
	*/
	LFO_ExParameters getParameters()
	{
		return Exparameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param LFO_ExParameters custom data structure
	*/
	void setParameters(const LFO_ExParameters& _params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU
		LFO::setParameters(_params);
		Exparameters.frequency_Hz = lfoParameters.frequency_Hz;
		Exparameters.waveform = lfoParameters.waveform;
		Exparameters = _params;

		// --- cook parameters here
	}

private:
	LFO_ExParameters Exparameters; ///< object parameters

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate
	

};

#endif