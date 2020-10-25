#pragma once

#ifndef __UCombFilter__
#define __UCombFilter__

#include "fxobjects.h"

/**
\struct UCombFilterParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the UCombFilter object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/

enum class CombFilterType { combFilter, inverseCombFilter };

struct UCombFilterParameters
{
	UCombFilterParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	UCombFilterParameters& operator=(const UCombFilterParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		combFilterType = params.combFilterType;
		delayTime_mSec = params.delayTime_mSec;
		feedbackGain = params.feedbackGain;
		dryGain = params.dryGain;
		wetGain = params.wetGain;
		// --- MUST be last
		return *this;
	}

	// --- individual parameters

	CombFilterType combFilterType = CombFilterType::combFilter;
	double delayTime_mSec = 0.0; ///< delay time in mSec
	double feedbackGain = 0.0; ///< feedback for comb filter
	double dryGain = 0.5; ///< gain
	double wetGain = 0.5; ///< gain 
};


/**
\class UCombFilter
\ingroup FX-Objects
\brief
The UCombFilter object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use UCombFilterParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class UCombFilter : public IAudioSignalProcessor
{
public:
	UCombFilter(void) {}	/* C-TOR */
	~UCombFilter(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = (_sampleRate);

		// --- do any other per-audio-run inits here
		combDelayLine.createDelayBuffer(_sampleRate, 500.0);
		combDelayLine.reset(_sampleRate);
		return true;
	}

	/** process MONO input */
	/**
	\param xn input
	\return the processed sample
	*/
	virtual double processAudioSample(double xn)
	{
	
		// --- the output variable
		double yn = 0.0;

		// --- do your DSP magic here to create yn
		double delayedSample = 0.0;
		if (UParameters.combFilterType == CombFilterType::inverseCombFilter) {
			delayedSample = combDelayLine.readDelayAtTime_mSec(UParameters.delayTime_mSec);
			combDelayLine.writeDelay(xn);
			double drySignal = xn * UParameters.dryGain;
			double wetSignal = delayedSample * UParameters.wetGain;
			yn = wetSignal + drySignal;
		}
		if (UParameters.combFilterType == CombFilterType::combFilter) {
			delayedSample = combDelayLine.readDelayAtTime_mSec(UParameters.delayTime_mSec);
			combDelayLine.writeDelay(xn + delayedSample * UParameters.feedbackGain);
			double wetSignal = delayedSample * UParameters.wetGain;
			yn = wetSignal;
		}
		// --- done
  		return yn;
	}

	/** query to see if this object can process frames */
	virtual bool canProcessAudioFrame() { return false; } // <-- change this!

	/** process audio frame: implement this function if you answer "true" to above query */
	virtual bool processAudioFrame(const float* inputFrame,	/* ptr to one frame of data: pInputFrame[0] = left, pInputFrame[1] = right, etc...*/
					     float* outputFrame,
					     uint32_t inputChannels,
					     uint32_t outputChannels)
	{
		// --- do nothing
		return false; // NOT handled
	}


	/** get parameters: note use of custom structure for passing param data */
	/**
	\return UCombFilterParameters custom data structure
	*/
	UCombFilterParameters getParameters()
	{
		return UParameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param UCombFilterParameters custom data structure
	*/
	void setParameters(const UCombFilterParameters& _params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU
		//createDelayBuffer(sampleRate, UParameters.delayTime_mSec);
		
		UParameters = _params;

		// --- cook parameters here
	}

private:
	void createDelayBuffer(double _sampleRate, double _bufferLength_mSec)
	{
		// --- store for math
		//bufferLength_mSec = _bufferLength_mSec;
		sampleRate = _sampleRate;
		// --- create new buffer
		combDelayLine.createDelayBuffer(_sampleRate, _bufferLength_mSec);
	}
	UCombFilterParameters parameters; ///< object parameters

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate
	SimpleDelay combDelayLine;
	UCombFilterParameters UParameters;

};

#endif