// Copyright (2019) Cobalt Speech and Language, Inc.

#include "luna_synthesizer_stream.h"

#include "luna_exception.h"

LunaSynthesizerStream::LunaSynthesizerStream(
    const LunaReader &reader, const std::shared_ptr<grpc::ClientContext> &ctx)
    : mReader(reader), mCtx(ctx)
{
}

LunaSynthesizerStream::~LunaSynthesizerStream() {}

bool LunaSynthesizerStream::receiveAudio(ByteVector &audio)
{
    cobaltspeech::luna::SynthesizeResponse response;
    bool streamOpen = mReader->Read(&response);
    if (!streamOpen)
    {
        audio.clear();
        return false;
    }

    audio.assign(response.audio().begin(), response.audio().end());

    return true;
}

void LunaSynthesizerStream::close()
{
    grpc::Status status = mReader->Finish();
    if (!status.ok())
        throw LunaException(status);
}
