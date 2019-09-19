// Copyright (2019) Cobalt Speech and Language, Inc.

#include "luna_client.h"

#include "luna_exception.h"

#include <chrono>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

LunaClient::LunaClient(const std::string &url, bool secureConnection)
    : mLunaVersion(""), mTimeout(30000)
{
    // Quick runtime check to verify that the user has linked against
    // a version of protobuf that is compatible with the version used
    // to generate the c++ files.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Setup credentials
    std::shared_ptr<grpc::ChannelCredentials> creds;
    if (secureConnection)
        creds = grpc::SslCredentials(grpc::SslCredentialsOptions());
    else
        creds = grpc::InsecureChannelCredentials();

    // Create the channel and stub
    std::unique_ptr<cobaltspeech::luna::Luna::Stub> tmpStub =
        cobaltspeech::luna::Luna::NewStub(grpc::CreateChannel(url, creds));
    mStub.swap(tmpStub);
}

LunaClient::~LunaClient() {}

const std::string &LunaClient::lunaVersion()
{
    // Check if we have it cached
    if (mLunaVersion.empty())
    {
        // Send the grpc request to get the version
        grpc::ClientContext ctx;
        cobaltspeech::luna::VersionRequest request;
        cobaltspeech::luna::VersionResponse response;

        this->setContextDeadline(ctx);
        grpc::Status status = mStub->Version(&ctx, request, &response);
        if (!status.ok())
            throw LunaException(status);

        mLunaVersion = response.version();
    }

    return mLunaVersion;
}

std::vector<LunaVoice> LunaClient::listVoices()
{
    // Check if we have them cached
    if (mVoices.empty())
    {
        // Send the grpc request to get the voices
        grpc::ClientContext ctx;
        cobaltspeech::luna::ListVoicesRequest request;
        cobaltspeech::luna::ListVoicesResponse response;

        this->setContextDeadline(ctx);
        grpc::Status status = mStub->ListVoices(&ctx, request, &response);
        if (!status.ok())
            throw LunaException(status);

        // Cache the voices
        for (int i = 0; i < response.voices_size(); i++)
        {
            LunaVoice voice(response.voices(i));
            mVoices.push_back(voice);
        }
    }

    return mVoices;
}

ByteVector
LunaClient::synthesize(const cobaltspeech::luna::SynthesizerConfig &config,
                       const std::string &text)
{
    // Setup the request
    grpc::ClientContext ctx;
    this->setContextDeadline(ctx);

    cobaltspeech::luna::SynthesizeResponse response;
    cobaltspeech::luna::SynthesizeRequest request;
    request.mutable_config()->CopyFrom(config);
    request.set_text(text);

    grpc::Status status = mStub->Synthesize(&ctx, request, &response);
    if (!status.ok())
        throw LunaException(status);

    ByteVector audio(response.audio().begin(), response.audio().end());

    return audio;
}

LunaSynthesizerStream LunaClient::synthesizeStream(
    const cobaltspeech::luna::SynthesizerConfig &config,
    const std::string &text)
{
    // We need the context to exist for as long as the stream,
    // so we are creating it as a managed pointer.
    std::shared_ptr<grpc::ClientContext> ctx(new grpc::ClientContext);
    this->setContextDeadline(*ctx);

    // Create the grpc reader
    cobaltspeech::luna::SynthesizeRequest request;
    request.mutable_config()->CopyFrom(config);
    request.set_text(text);

    std::shared_ptr<grpc::ClientReader<cobaltspeech::luna::SynthesizeResponse>>
        reader(mStub->SynthesizeStream(ctx.get(), request));

    return LunaSynthesizerStream(reader, ctx);
}

void LunaClient::setRequestTimeout(unsigned int milliseconds)
{
    mTimeout = milliseconds;
}

LunaClient::LunaClient(const LunaClient &)
{
    // Do nothing. This copy constructor is intentionally private
    // and does nothing because we don't want to copy client objects.
}

LunaClient &LunaClient::operator=(const LunaClient &)
{
    // Do nothing. The assignment operator is intentionally private
    // and does nothing because we don't want to copy client objects.
    return *this;
}

void LunaClient::setContextDeadline(grpc::ClientContext &ctx)
{
    std::chrono::system_clock::time_point deadline =
        std::chrono::system_clock::now() + std::chrono::milliseconds(mTimeout);
    ctx.set_deadline(deadline);
}
