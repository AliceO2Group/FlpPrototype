<?php
namespace CERN\Alice\DAQ\O2;

class Socket {

	private $socket;
	protected $id;
	protected $hostname;

	public function &getSocket() {
		return $this->socket;
	}

	public function __construct(&$socket) {
		$this->socket  = $socket;
		$this->id = intval($socket);
		$this->hostname = stream_socket_get_name($socket, true);
	}

	protected function write($response) {
		if (($bytes = stream_socket_sendto($this->socket, $response)) < 0) {
			return false;
		}
		Log::write(sprintf("Sending %d bytes to #%d", $bytes, $this->id));
		return $bytes;
	}

	protected function read($toRead = 1500) {
		if (($return = stream_socket_recvfrom($this->socket, $toRead)) === false) {
			Log::write(sprintf("%s, Error while reading %d bytes of data", $this->hostname, $toRead), "error", 20);
		}
        return $return;
    }

    protected function close() {
		fclose($this->socket);
		Log::write(sprintf("Socket #%d has been closed.", $this->id));
    }

}
?>