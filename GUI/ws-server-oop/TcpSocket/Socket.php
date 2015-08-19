<?php
/// \file Socket.php
/// \brief Definition and implementation of the CERN\Alice\DAQ\O2\Socket class.
///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>

namespace CERN\Alice\DAQ\O2;

/// Represents TCP-level logic of client connected to the server 
///
/// Provides basic functions to deal with TCP socket: read, write and close.
/// It is created for each connected client.
class Socket {
	/// stream resource that represent TCP socket of connected client
	private $socket;

	/// ID of socket
	protected $id;

	/// The orgin of the connection
	protected $hostname;

	/// Socket reference getter, needed by ServerSocket class to pass it to socket_select
	/// \return 	reference to socket
	public function &getSocket() {
		return $this->socket;
	}

	/// Socket class constructor, stores socket and initialize id and hostname variables
	/// \param 	reference to socket
	public function __construct(&$socket) {
		$this->socket  = $socket;
		$this->id = intval($socket);
		$this->hostname = stream_socket_get_name($socket, true);
		stream_socket_enable_crypto($this->socket, true, STREAM_CRYPTO_METHOD_SSLv3_SERVER);
	}
	/// Writes the socket
	/// \param 	string that is written to socket
	/// \return number of written bytes OR false when failed to write the socket, or number of written bytes is 0
	protected function write($response) {
		if (($bytes = stream_socket_sendto($this->socket, $response)) < 0) {
			return false;
		}
		Log::write(sprintf("Sending %d bytes to #%d", $bytes, $this->id));
		return $bytes;
	}
	/// Reads the socket
	/// \param  number of bytes to read, default = 1500
	/// \return string that was read from the socket OR false when failed 	
	protected function read($toRead = 1500) {
		if (($return = stream_socket_recvfrom($this->socket, $toRead)) === false) {
			Log::write(sprintf("%s, Error while reading %d bytes of data", $this->hostname, $toRead), "error", 20);
			return false;
		}
        return $return;
    }
    /// Closes to socket
    protected function close() {
		fclose($this->socket);
		Log::write(sprintf("Socket #%d has been closed.", $this->id));
    }
}
?>