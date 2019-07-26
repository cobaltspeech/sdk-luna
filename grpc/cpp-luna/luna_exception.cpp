// Copyright (2019) Cobalt Speech and Language, Inc.

#include "luna_exception.h"

LunaException::LunaException(const std::string &msg) :
    mMsg(msg)
{}

LunaException::LunaException(const grpc::Status &status) :
    mMsg(status.error_message())
{}

LunaException::~LunaException()
{}

const char* LunaException::what() const noexcept
{
    return mMsg.c_str();
}
