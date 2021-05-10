// Copyright (2021) Cobalt Speech and Language Inc.

// Package luna provides for interacting with an instance of luna server using
// GRPC for performing text-to-speech.
package luna

import (
	"context"
	"crypto/tls"
	"crypto/x509"
	"fmt"
	"os"
	"time"

	"github.com/cobaltspeech/sdk-luna/grpc/go-luna/lunapb"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials"
)

const defaultConnectTimeout = 2 * time.Second

// Client is an object for interacting with the Luna GRPC API.
//
// All methods except Close may be called concurrently.
type Client struct {
	// The protobuf-defined client. Most users will not need to call this
	// directly, but it is exposed as a convenience for advanced users who
	// wish to use gRPC functionality beyond what this interface provides.
	PBClient lunapb.LunaClient

	// The list of gRPC call options that are used when the client
	// makes server requests.
	CallOpts []grpc.CallOption

	// Internal data
	conn           *grpc.ClientConn
	insecure       bool
	tlscfg         tls.Config
	connectTimeout time.Duration
}

// NewClient creates a new Luna Client to use the provided addr as the luna server.
func NewClient(addr string, opts ...Option) (*Client, error) {
	c := Client{
		connectTimeout: defaultConnectTimeout,
	}

	for _, opt := range opts {
		err := opt(&c)
		if err != nil {
			return nil, fmt.Errorf("unable to create a client: %v", err)
		}
	}

	// Set up dial options
	dopts := []grpc.DialOption{
		grpc.WithBlock(), // Don't proceed until the connection is up
		grpc.WithReturnConnectionError(),
	}

	if c.insecure {
		dopts = append(dopts, grpc.WithInsecure())
	} else {
		dopts = append(dopts, grpc.WithTransportCredentials(credentials.NewTLS(&c.tlscfg)))
	}

	// Set up the dial/connect timeout
	ctx, cancel := context.WithTimeout(context.Background(), c.connectTimeout)
	defer cancel()

	conn, err := grpc.DialContext(ctx, addr, dopts...)
	if err != nil {
		return nil, fmt.Errorf("unable to create a client: %s", err)
	}
	c.conn = conn
	c.PBClient = lunapb.NewLunaClient(conn)

	return &c, nil
}

// Option configures how we setup the connection with a server.
type Option func(*Client) error

// WithInsecure returns an Option which disables transport security for this
// Client.  Use this when connecting to a non-TLS enabled Luna server, such as
// during debugging.
func WithInsecure() Option {
	return func(c *Client) error {
		c.insecure = true
		return nil
	}
}

// WithServerCert returns an Option which sets up the given PEM certificate as a
// root certificate that can validate the certificate presented by the server we
// are connecting to.  Use this when connecting to an instance of Luna server
// that is using a self-signed certificate.
func WithServerCert(cert []byte) Option {
	return func(c *Client) error {
		caCertPool := x509.NewCertPool()
		if ok := caCertPool.AppendCertsFromPEM(cert); !ok {
			return fmt.Errorf("unable to use given caCert")
		}
		c.tlscfg.RootCAs = caCertPool
		return nil
	}
}

// WithClientCert returns an Option which sets up the given PEM certificate and
// key as the credentials presented by this Client when connecting to a server.
// Use this when setting up mutually authenticated TLS.
func WithClientCert(certPem []byte, keyPem []byte) Option {
	return func(c *Client) error {
		clientCert, err := tls.X509KeyPair(certPem, keyPem)
		if err != nil {
			return err
		}

		c.tlscfg.Certificates = []tls.Certificate{clientCert}
		return nil
	}
}

// WithConnectTimeout returns an Option that configures the timeout for
// establishing grpc connection with the server.  Use this only when you
// are on a slow network and when Cobalt recommends you to do so.
func WithConnectTimeout(t time.Duration) Option {
	return func(c *Client) error {
		c.connectTimeout = t
		return nil
	}
}

// NewClientWithInsecure creates a new Luna Client to use the provided addr as the luna server.
//
// Deprecated: Use NewClient and the WithInsecure() option instead.
func NewClientWithInsecure(addr string) (*Client, error) {
	return NewClient(addr, WithInsecure())
}

// NewClientWithCertFile creates a new Luna Client to use the provided addr as the luna server.
//
// Deprecated: Use NewClient and the WithServerCert() option instead.
func NewClientWithCertFile(addr string, certfile string) (*Client, error) {
	certData, err := os.ReadFile(certfile)
	if err != nil {
		return nil, err
	}

	return NewClient(addr, WithServerCert(certData))
}

// Close closes the connection to the API service.  The user should only invoke
// this when the client is no longer needed.
func (c *Client) Close() error {
	return c.conn.Close()
}

// SetCallOptions replaces any current gRPC call options with the given set
// to use when making server requests.
func (c *Client) SetCallOptions(opts ...grpc.CallOption) {
	newOpts := make([]grpc.CallOption, 0)
	for _, o := range opts {
		newOpts = append(newOpts, o)
	}

	c.CallOpts = newOpts
}

// AppendCallOptions adds the given gRPC call options to the current
// list of options to use when making server requests. It does not
// check to see if the options are unique in the final list.
func (c *Client) AppendCallOptions(opts ...grpc.CallOption) {
	for _, o := range opts {
		c.CallOpts = append(c.CallOpts, o)
	}
}

// Version queries the server for its version
func (c *Client) Version(ctx context.Context) (*lunapb.VersionResponse, error) {
	return c.PBClient.Version(ctx, &lunapb.VersionRequest{}, c.CallOpts...)
}

// ListVoices retrieves a list of configured voices on the server.
func (c *Client) ListVoices(ctx context.Context) (*lunapb.ListVoicesResponse, error) {
	return c.PBClient.ListVoices(ctx, &lunapb.ListVoicesRequest{}, c.CallOpts...)
}

// Synthesize requests text-to-speech from the server for the given text
func (c *Client) Synthesize(ctx context.Context, r *lunapb.SynthesizeRequest) (*lunapb.SynthesizeResponse, error) {
	return c.PBClient.Synthesize(ctx, r, c.CallOpts...)
}

// SynthesizeStream requests text-to-speech from the server for the given text and
// returns a stream that receives audio samples as they are generated. The stream
// will close once synthesis has finished.
func (c *Client) SynthesizeStream(ctx context.Context, in *lunapb.SynthesizeRequest) (lunapb.Luna_SynthesizeStreamClient, error) {
	return c.PBClient.SynthesizeStream(ctx, in, c.CallOpts...)
}
