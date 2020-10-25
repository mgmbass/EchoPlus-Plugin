// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"

/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**


	// --- Declaration of Plugin Parameter Objects 
	PluginParameter* piParam = nullptr;

	// --- continuous control: Delay Time
	piParam = new PluginParameter(controlID::delayTime_ms, "Delay Time", "ms", controlVariableType::kDouble, 90.000000, 680.000000, 90.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&delayTime_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Noise Filter
	piParam = new PluginParameter(controlID::noiseFilter_Hz, "Noise Filter", "Hz", controlVariableType::kDouble, 50.000000, 500.000000, 50.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&noiseFilter_Hz, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Low Freq Noise
	piParam = new PluginParameter(controlID::lowFreqAmp, "Low Freq Noise", "Units", controlVariableType::kFloat, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lowFreqAmp, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- continuous control: NoiseAmp
	piParam = new PluginParameter(controlID::noiseAmp, "NoiseAmp", "Units", controlVariableType::kDouble, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&noiseAmp, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Noise Mod Depth
	piParam = new PluginParameter(controlID::noisemodDepth, "Noise Mod Depth", "Percent", controlVariableType::kDouble, 0.000000, 100.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&noisemodDepth, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO Mod Depth
	piParam = new PluginParameter(controlID::lfoModDepth, "LFO Mod Depth", "", controlVariableType::kDouble, 0.000000, 20.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfoModDepth, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Wet
	piParam = new PluginParameter(controlID::wetMix, "Wet", "dB", controlVariableType::kFloat, -80.000000, 0.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&wetMix, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- continuous control: Dry
	piParam = new PluginParameter(controlID::dryMix, "Dry", "dB", controlVariableType::kDouble, -80.000000, 0.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&dryMix, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Feeb Back
	piParam = new PluginParameter(controlID::feedBack_pct, "Feeb Back", "percent", controlVariableType::kDouble, 0.000000, 100.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&feedBack_pct, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Noise Level
	piParam = new PluginParameter(controlID::noiseLevel_dB, "Noise Level", "dB", controlVariableType::kDouble, -80.000000, 12.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&noiseLevel_dB, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: record Level
	piParam = new PluginParameter(controlID::recordLevel_dB, "record Level", "dB", controlVariableType::kDouble, -80.000000, 20.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&recordLevel_dB, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Playback Level
	piParam = new PluginParameter(controlID::playbackLevel_dB, "Playback Level", "dB", controlVariableType::kDouble, -80.000000, 20.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&playbackLevel_dB, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Noise Out Filter
	piParam = new PluginParameter(controlID::noiseOutFIlter, "Noise Out Filter", "Units", controlVariableType::kDouble, 50.000000, 1000.000000, 50.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&noiseOutFIlter, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- Aux Attributes
	AuxParameterAttribute auxAttribute;

	// --- RAFX GUI attributes
	// --- controlID::delayTime_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::delayTime_ms, auxAttribute);

	// --- controlID::noiseFilter_Hz
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::noiseFilter_Hz, auxAttribute);

	// --- controlID::lowFreqAmp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lowFreqAmp, auxAttribute);

	// --- controlID::noiseAmp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::noiseAmp, auxAttribute);

	// --- controlID::noisemodDepth
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::noisemodDepth, auxAttribute);

	// --- controlID::lfoModDepth
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfoModDepth, auxAttribute);

	// --- controlID::wetMix
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::wetMix, auxAttribute);

	// --- controlID::dryMix
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::dryMix, auxAttribute);

	// --- controlID::feedBack_pct
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::feedBack_pct, auxAttribute);

	// --- controlID::noiseLevel_dB
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::noiseLevel_dB, auxAttribute);

	// --- controlID::recordLevel_dB
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::recordLevel_dB, auxAttribute);

	// --- controlID::playbackLevel_dB
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::playbackLevel_dB, auxAttribute);

	// --- controlID::noiseOutFIlter
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::noiseOutFIlter, auxAttribute);


	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;
	delayMod.reset(resetInfo.sampleRate);
	tapeDelay.reset(resetInfo.sampleRate);
	EchoplexTapeDelayParameters tapeParams = tapeDelay.getParameters();
	tapeParams.algorithm = delayAlgorithm::kNormal;
	tapeDelay.setParameters(tapeParams);
	EchoplexDelayModulatorParameters paramsAF = delayMod.getParameters();
	paramsAF.noiseDepth_Pct = 1.0;
	paramsAF.sixtyHzNoiseAmp = 0.1;
	paramsAF.noiseFilterFc_Hz = 50.0;
	paramsAF.noiseFilterAmplitude = 0.5;
	delayMod.setParameters(paramsAF);

    // --- other reset inits
    return PluginBase::reset(resetInfo);
}
void PluginCore::updateParameters() {
	noiseLevel_cooked = pow(10.0, noiseLevel_dB / 20.0);
	playbackLevel_cooked = pow(10.0, playbackLevel_dB / 20);
	recordLevel_cooked = pow(10.0, recordLevel_dB / 20);
	EchoplexDelayModulatorParameters params = delayMod.getParameters();
	params.delayTime = delayTime_ms;
	params.lfoDepth_Pct = lfoModDepth;
	params.noiseDepth_Pct = 1.0;
	params.sixtyHzNoiseAmp = 0.1;
	params.noiseFilterFc_Hz = 50.0;
	params.noiseFilterAmplitude = 0.5;
	delayMod.setParameters(params);
	SignalGenData y = delayMod.renderAudioOutput();
	EchoplexTapeDelayParameters tapeParamsAF = tapeDelay.getParameters();
	tapeParamsAF.leftDelay_mSec = y.normalOutput;
	tapeParamsAF.rightDelay_mSec = y.normalOutput;
	tapeParamsAF.feedback_Pct = feedBack_pct;
	tapeParamsAF.wetLevel_dB = wetMix;
	tapeParamsAF.dryLevel_dB = dryMix;
	tapeParamsAF.noiseLevel = noiseLevel_cooked;
	tapeParamsAF.recordLevel_dB = recordLevel_cooked;
	tapeParamsAF.playbackLevel_dB = playbackLevel_cooked;
	tapeParamsAF.noiseFreq = noiseOutFIlter;
	tapeDelay.setParameters(tapeParamsAF);

}
/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

    return true;
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
    // --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();
	updateParameters();

    // --- decode the channelIOConfiguration and process accordingly
    //
	// --- Synth Plugin:
	// --- Synth Plugin --- remove for FX plugins
	if (getPluginType() == kSynthPlugin)
	{
		// --- output silence: change this with your signal render code
		processFrameInfo.audioOutputFrame[0] = 0.0;
		if (processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
			processFrameInfo.audioOutputFrame[1] = 0.0;

		return true;	/// processed
	}

    // --- FX Plugin:
    if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Mono-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Stereo-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
		SignalGenData y;
		y = delayMod.renderAudioOutput();
		double xnR = processFrameInfo.audioInputFrame[0];
		double xnL = processFrameInfo.audioInputFrame[1];
		float inputs[2] = { xnR, xnL };
		float outputs[2] = { 0.0, 0.0 };
		tapeDelay.processAudioFrame(inputs, outputs, 2, 2);
		processFrameInfo.audioOutputFrame[0] = outputs[0]; // processFrameInfo.audioInputFrame[0];
		processFrameInfo.audioOutputFrame[1] = outputs[1];// processFrameInfo.audioInputFrame[1];

        return true; /// processed
    }

    return false; /// NOT processed
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}


/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// --- Plugin Presets 
	int index = 0;
	PresetInfo* preset = nullptr;

	// --- Preset: Factory Preset
	preset = new PresetInfo(index++, "Factory Preset");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::delayTime_ms, 90.000000);
	setPresetParameter(preset->presetParameters, controlID::noiseFilter_Hz, 50.000000);
	setPresetParameter(preset->presetParameters, controlID::lowFreqAmp, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::noiseAmp, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::noisemodDepth, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::lfoModDepth, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::wetMix, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::dryMix, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::feedBack_pct, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::noiseLevel_dB, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::recordLevel_dB, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::playbackLevel_dB, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::noiseOutFIlter, 50.000000);
	addPreset(preset);

	// --- Preset: Noice Bit of Wobble
	preset = new PresetInfo(index++, "Noice Bit of Wobble");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::delayTime_ms, 387.950012);
	setPresetParameter(preset->presetParameters, controlID::noiseFilter_Hz, 340.250000);
	setPresetParameter(preset->presetParameters, controlID::lowFreqAmp, 0.100000);
	setPresetParameter(preset->presetParameters, controlID::noiseAmp, 0.495000);
	setPresetParameter(preset->presetParameters, controlID::noisemodDepth, 9.999999);
	setPresetParameter(preset->presetParameters, controlID::lfoModDepth, 9.599998);
	setPresetParameter(preset->presetParameters, controlID::wetMix, -12.000000);
	setPresetParameter(preset->presetParameters, controlID::dryMix, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::feedBack_pct, 45.999996);
	setPresetParameter(preset->presetParameters, controlID::noiseLevel_dB, -46.460007);
	setPresetParameter(preset->presetParameters, controlID::recordLevel_dB, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::playbackLevel_dB, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::noiseOutFIlter, 406.250061);
	addPreset(preset);


	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;
	apiSpecificInfo.auBundleName = kAUBundleName;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.auBundleName = kAUBundleName;

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
