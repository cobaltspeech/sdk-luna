// Copyright (2021) Cobalt Speech and Language, Inc.

#include "luna_voice.h"

LunaVoice::LunaVoice(const cobaltspeech::luna::Voice &voice)
    : mId(voice.id()), mName(voice.name()), mSampleRate(voice.sample_rate()),
      mLanguage(voice.language())
{
}

LunaVoice::~LunaVoice() {}

const std::string &LunaVoice::id() const { return mId; }

const std::string &LunaVoice::name() const { return mName; }

uint LunaVoice::sampleRate() const { return mSampleRate; }

const std::string &LunaVoice::language() const { return mLanguage; }
