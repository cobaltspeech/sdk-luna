// Copyright (2019) Cobalt Speech and Language, Inc.

#ifndef LUNA_SYNTHESIZER_STREAM_H
#define LUNA_SYNTHESIZER_STREAM_H

#include "luna.grpc.pb.h"

#include <memory>

using ByteVector = std::vector<char>;

//! The LunaSynthesizerStream class represents a stream of audio
//! generated by the Luna server in response to a synthesis request.
//! Instead of waiting for synthesis to complete and returning all the
//! audio at once, this class receives the audio samples as they are
//! ready.
class LunaSynthesizerStream
{
public:
    using LunaReader = std::shared_ptr<
        grpc::ClientReaderInterface<cobaltspeech::luna::SynthesizeResponse>>;

    //! Create a new synthesizer stream. Most users of this class should
    //! not need to call this constructor. Instead, they should use
    //! LunaClient::synthesizeStream().
    LunaSynthesizerStream(const LunaReader &reader,
                          const std::shared_ptr<grpc::ClientContext> &ctx);

    ~LunaSynthesizerStream();

    //! Receive audio samples from the server as they become available.
    //! Returns false when synthesis is complete and there are no more
    //! samples to receive.
    bool receiveAudio(ByteVector &audio);

    //! Close the synthesis stream. This should be done after all the
    //! samples have been received (i.e., recieveAudio() returned false).
    void close();

private:
    LunaReader mReader;
    std::shared_ptr<grpc::ClientContext> mCtx;
};

#endif // LUNA_SYNTHESIZER_STREAM_H
