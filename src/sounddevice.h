/***************************************************************************
                          sounddevice.cpp
                             -------------------
    begin                : Sun Aug 12, 2007, past my bedtime
    copyright            : (C) 2007 Albert Santoni
    email                : gamegod \a\t users.sf.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include <QString>
#include <QList>

#include "soundmanager.h"

//Forward declarations
class SoundDevice;
class SoundManager;
class AudioOutput;
class AudioInput;

enum SoundDeviceError {
    SOUNDDEVICE_ERROR_OK = OK,
    SOUNDDEVICE_ERROR_DUPLICATE_OUTPUT_CHANNEL,
    SOUNDDEVICE_ERROR_EXCESSIVE_OUTPUT_CHANNEL,
    SOUNDDEVICE_ERROR_EXCESSIVE_INPUT_CHANNEL,
};

class SoundDevice {
  public:
    SoundDevice(ConfigObject<ConfigValue> *config, SoundManager* sm);
    virtual ~SoundDevice();

    inline QString getInternalName() const {
        return m_strInternalName;
    }
    QString getDisplayName() const;
    QString getHostAPI() const;
    void setHostAPI(QString api);
    void setSampleRate(double sampleRate);
    void setFramesPerBuffer(unsigned int framesPerBuffer);
    virtual int open() = 0;
    virtual int close() = 0;
    virtual QString getError() const = 0;
    virtual unsigned int getDefaultSampleRate() const = 0;
    int getNumOutputChannels() const;
    int getNumInputChannels() const;
    SoundDeviceError addOutput(const AudioOutputBuffer& out);
    SoundDeviceError addInput(const AudioInputBuffer& in);
    const QList<AudioInputBuffer>& inputs() const {
        return m_audioInputs;
    }
    const QList<AudioOutputBuffer>& outputs() const {
        return m_audioOutputs;
    }

    void clearOutputs();
    void clearInputs();
    bool operator==(const SoundDevice &other) const;
    bool operator==(const QString &other) const;

    void onOutputBuffersReady(const unsigned long iFramesPerBuffer);

  protected:
    void composeOutputBuffer(CSAMPLE* outputBuffer,
                             const unsigned long iFramesPerBuffer,
                             const unsigned int iFrameSize);

    ConfigObject<ConfigValue> *m_pConfig;
    // Pointer to the SoundManager object which we'll request audio from.
    SoundManager* m_pSoundManager;
    // The name of the soundcard, used internally (may include the device ID)
    QString m_strInternalName;
    // The name of the soundcard, as displayed to the user
    QString m_strDisplayName;
    // The number of output channels that the soundcard has
    int m_iNumOutputChannels;
    // The number of input channels that the soundcard has
    int m_iNumInputChannels;
    // The current samplerate for the sound device.
    double m_dSampleRate;
    // The name of the audio API used by this device.
    QString m_hostAPI;
    unsigned int m_framesPerBuffer;
    QList<AudioOutputBuffer> m_audioOutputs;
    QList<AudioInputBuffer> m_audioInputs;
    CSAMPLE* m_pRenderBuffer;
};

#endif
