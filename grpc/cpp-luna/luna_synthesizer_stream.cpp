// Copyright (2019) Cobalt Speech and Language, Inc.

#include "luna_synthesizer_stream.h"

#include "luna_exception.h"

LunaSynthesizerStream::LunaSynthesizerStream(const LunaReader &reader,
                                             const std::shared_ptr<grpc::ClientContext> &ctx) :
    mReader(reader),
    mCtx(ctx)
{}

LunaSynthesizerStream::~LunaSynthesizerStream()
{}

bool LunaSynthesizerStream::receiveSamples(std::vector<float> &samples)
{
    cobaltspeech::luna::SynthesizeResponse response;
    bool streamOpen = mReader->Read(&response);
    if(!streamOpen)
    {
        samples.clear();
        return false;
    }

    samples.resize(response.samples_size());
    for(int i = 0; i < response.samples_size(); i++)
        samples[i] = response.samples(i);

    return true;
}

void LunaSynthesizerStream::close()
{
    grpc::Status status = mReader->Finish();
    if(!status.ok())
        throw LunaException(status);
}

