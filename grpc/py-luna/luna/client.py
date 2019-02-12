# Copyright (2019) Cobalt Speech and Language Inc.

import grpc
import sys
import os

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__))))
import luna_pb2, luna_pb2_grpc

class LunaClient(object):
    """LunaClient is a client for interacting with Cobalt's Luna GRPC API."""

    def __init__(self, service_address, channel=None):
        """Constructor.

        Args:
        service_address: host:port of where luna server is running
        channel: grpc.Channel, a channel instance through which to make calls.
                 If set to None, a new channel will be created
                 that has transport security enabled.
        """
        if channel is None:
            self.channel = grpc.secure_channel(service_address,
                                               grpc.ssl_channel_credentials())
        else:
            self.channel = channel

        self.client = luna_pb2_grpc.LunaStub(self.channel)

    @classmethod
    def with_insecure(cls, service_address):
        """Constructor.  Creates a LunaClient object with transport layer security
disabled.  This client will only work if the luna server provided to you also
has TLS disabled.

        Args:
        service_address: host:port of where luna server is running
        """

        return cls(service_address, grpc.insecure_channel(service_address))

    @classmethod
    def with_cert_file(cls, service_address, certfile):
        """Constructor.  Creates a new LunaClient object with transport layer security
enabled, and configured to use the provided certificate file.  This certificate
will be validated by the server, thus providing mutually-authenticated TLS.

        Args:
        service_address: host:port of where luna server is running
        certfile: path to the ssl certificate file
        """

        with open(certfile) as f:
            creds = grpc.ssl_channel_credentials(f.read())
            channel = grpc.secure_channel(service_address, creds)
            return cls(service_address, channel)

    def __del__(self):
        """Destructor."""
        self.channel.close()

    def Version(self):
        """Query the server for its version"""
        return self.client.Version(luna_pb2.VersionRequest())

    def ListVoices(self):
        """Query the server for configured list of voices"""
        return self.client.ListVoices(luna_pb2.ListVoicesRequest())

    def Synthesize(self, req):
        """Request the server to perform text-to-speech using the given request.
        Args:
        req: luna_pb2.SynthesizeRequest object containing appropriate parameters
        """
        return self.client.Synthesize(req)
