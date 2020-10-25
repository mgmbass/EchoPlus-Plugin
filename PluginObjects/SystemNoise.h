#pragma once

#ifndef __IASP_Skeleton__
#define __IASP_Skeleton__

#include "fxobjects.h"

/**
\struct IASP_SkeletonParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the IASP_Skeleton object.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
struct IASP_SkeletonParameters
{
	IASP_SkeletonParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	IASP_SkeletonParameters& operator=(const IASP_SkeletonParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		myVariable = params.myVariable;


		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	data_type myVariable = 0.0;	///< init
};


/**
\class IASP_Skeleton
\ingroup FX-Objects
\brief
The IASP_Skeleton object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use IASP_SkeletonParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class IASP_Skeleton : public IAudioSignalProcessor
{
public:
	IASP_Skeleton(void) {}	/* C-TOR */
	~IASP_Skeleton(void) {}	/* D-TOR */

public:
	/** reset members to initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = (_sampleRate);

		// --- do any other per-audio-run inits here

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

		// --- done
		return yn;
	}

	/** query to see if this object can process frames */
	virtual bool canProcessAudioFrame() { return TrueOrFalse; } // <-- change this!

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
	\return IASP_SkeletonParameters custom data structure
	*/
	IASP_SkeletonParameters getParameters()
	{
		return parameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param IASP_SkeletonParameters custom data structure
	*/
	void setParameters(const IASP_SkeletonParameters& _params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU
		parameters = _params;

		// --- cook parameters here
	}

private:
	IASP_SkeletonParameters parameters; ///< object parameters

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate

};

#endif