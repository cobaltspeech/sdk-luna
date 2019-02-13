# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

import luna_pb2 as luna__pb2


class LunaStub(object):
  """Service that implements the Cobalt Luna Text-to-Speech API
  """

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.Version = channel.unary_unary(
        '/cobaltspeech.luna.Luna/Version',
        request_serializer=luna__pb2.VersionRequest.SerializeToString,
        response_deserializer=luna__pb2.VersionResponse.FromString,
        )
    self.ListVoices = channel.unary_unary(
        '/cobaltspeech.luna.Luna/ListVoices',
        request_serializer=luna__pb2.ListVoicesRequest.SerializeToString,
        response_deserializer=luna__pb2.ListVoicesResponse.FromString,
        )
    self.Synthesize = channel.unary_unary(
        '/cobaltspeech.luna.Luna/Synthesize',
        request_serializer=luna__pb2.SynthesizeRequest.SerializeToString,
        response_deserializer=luna__pb2.SynthesizeResponse.FromString,
        )


class LunaServicer(object):
  """Service that implements the Cobalt Luna Text-to-Speech API
  """

  def Version(self, request, context):
    """Queries the Version of the Server
    """
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def ListVoices(self, request, context):
    """Retrieves a list of available text-to-speech voices
    """
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def Synthesize(self, request, context):
    """Performs synchronous text-to-speech generation.
    """
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_LunaServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'Version': grpc.unary_unary_rpc_method_handler(
          servicer.Version,
          request_deserializer=luna__pb2.VersionRequest.FromString,
          response_serializer=luna__pb2.VersionResponse.SerializeToString,
      ),
      'ListVoices': grpc.unary_unary_rpc_method_handler(
          servicer.ListVoices,
          request_deserializer=luna__pb2.ListVoicesRequest.FromString,
          response_serializer=luna__pb2.ListVoicesResponse.SerializeToString,
      ),
      'Synthesize': grpc.unary_unary_rpc_method_handler(
          servicer.Synthesize,
          request_deserializer=luna__pb2.SynthesizeRequest.FromString,
          response_serializer=luna__pb2.SynthesizeResponse.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'cobaltspeech.luna.Luna', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))