// Copyright (2019) Cobalt Speech and Language, Inc.

#include "luna_voice.h"

LunaVoice::LunaVoice(const cobaltspeech::luna::Voice &voice) :
    mId(voice.id()),
    mName(voice.name())
{}

LunaVoice::~LunaVoice()
{}

const std::string& LunaVoice::id() const
{
    return mId;
}

const std::string& LunaVoice::name() const
{
    return mName;
}
