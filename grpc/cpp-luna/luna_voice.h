// Copyright (2021) Cobalt Speech and Language, Inc.

#ifndef LUNA_VOICE_H
#define LUNA_VOICE_H

#include "luna.pb.h"

#include <string>

//! The LunaVoice class represents a TTS voice model that is being
//! used by the Luna server.
class LunaVoice
{
public:
    //! Create a LunaVocie from the gRPC data structure.
    LunaVoice(const cobaltspeech::luna::Voice &voice);
    ~LunaVoice();

    //! Return the id of the model, as specified in the server's
    //! config file.
    const std::string &id() const;
    const std::string &name() const;
    uint sampleRate() const;
    const std::string &language() const;

private:
    std::string mId;
    std::string mName;
    uint mSampleRate;
    std::string mLanguage;
};

#endif // LUNA_VOICE_H
