// Copyright (2019) Cobalt Speech and Language Inc.

// Package luna provides for interacting with an instance of luna server using
// GRPC for performing text-to-speech.
package luna

import (
	"context"
	"fmt"

	"github.com/cobaltspeech/sdk-luna/grpc/go-luna/lunapb"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials"
)

// Client is an object for interacting with the Luna GRPC API.
//
// All methods except Close may be called concurrently.
type Client struct {
	conn   *grpc.ClientConn  // connection to the service
	client lunapb.LunaClient // the underlying API client
}

// NewClient creates a new Luna Client to use the provided addr as the luna server.
func NewClient(addr string) (*Client, error) {
	conn, err := grpc.Dial(addr)
	if err != nil {
		return nil, fmt.Errorf("unable to create a client: %s", err)
	}

	return &Client{
		conn:   conn,
		client: lunapb.NewLunaClient(conn),
	}, nil
}

// NewClientWithInsecure creates a new Luna Client to use the provided addr as the luna server.
//
// Transport layer security is disabled in the client.  This client will only
// work if the luna server provided to you also has TLS disabled.
func NewClientWithInsecure(addr string) (*Client, error) {
	conn, err := grpc.Dial(addr, grpc.WithInsecure())
	if err != nil {
		return nil, fmt.Errorf("unable to create a client: %s", err)
	}

	return &Client{
		conn:   conn,
		client: lunapb.NewLunaClient(conn),
	}, nil
}

// NewClientWithCertFile creates a new Luna Client to use the provided addr as the luna server.
//
// Transport layer security is enabled and configured to use the provided
// certificate file.  This certificate will be validated by the server, thus
// providing mutually-authenticated TLS.
func NewClientWithCertFile(addr string, certfile string) (*Client, error) {
	tc, err := credentials.NewClientTLSFromFile(certfile, "")
	if err != nil {
		return nil, fmt.Errorf("unable to create a client: %s", err)
	}

	conn, err := grpc.Dial(addr, grpc.WithTransportCredentials(tc))
	if err != nil {
		return nil, fmt.Errorf("unable to create a client: %s", err)
	}

	return &Client{
		conn:   conn,
		client: lunapb.NewLunaClient(conn),
	}, nil
}

// Close closes the connection to the API service.  The user should only invoke
// this when the client is no longer needed.
func (c *Client) Close() error {
	return c.conn.Close()
}

// Version queries the server for its version
func (c *Client) Version(ctx context.Context) (*lunapb.VersionResponse, error) {
	return c.client.Version(ctx, &lunapb.VersionRequest{})
}

// ListVoices retrieves a list of configured voices on the server.
func (c *Client) ListVoices(ctx context.Context) (*lunapb.ListVoicesResponse, error) {
	return c.ListVoices(ctx)
}

// Synthesize requests text-to-speech from the server for the given text
func (c *Client) Synthesize(ctx context.Context, r *lunapb.SynthesizeRequest) (*lunapb.SynthesizeResponse, error) {
	return c.client.Synthesize(ctx, r)
}
