<?php
namespace CERN\Alice\DAQ\O2;

require_once __DIR__.'/../Exceptions/WebSocketFrameException.php';
require_once __DIR__.'/../Exceptions/TCPSocketException.php';

use CERN\Alice\DAQ\WebSocket\Exceptions\WebSocketFrameException;
use CERN\Alice\DAQ\WebSocket\Exceptions\TCPSocketException;

class TCPHandler {
	/**
	 * Processing socket and it's hostname
	 */
	private $socket;
	private $hostname;

	public function write() {
		
	}

	private function read($toRead = 1500) {
		if (($return = stream_socket_recvfrom($this->socket, $toRead)) === false) {
			Log::write(sprintf("%s, Error while reading %d bytes of data", $this->hostname, $toRead), "error", 20);
		}
        return $return;
    }
    public function readSocket(&$pSocket) {
    	$this->socket = $pSocket;
    	$this->hostname = stream_socket_get_name($pSocket, true);
    	try {
	    	if (($rawheader = $this->read(2)) == false) {
				return false;
			}

			if (strcmp($rawheader, "GE") === 0) { //is_string
				return $this->readHandshake();
			} else {
				//$this->readFrame();
			}
    	} catch (\Exception $e) {
    		$this->socket = null;
    		$this->hostname = null;
			// $this->close(); ERROR, CLOSE CONNECTION!
			return false;
		}
    	
    }
    private function readHandshake() {
    	if (($read = $this->read()) == false) {
			throw new HandshakeException(sprintf("%s, Cannot read handshake", $this->hostname));
		}
		return "GE".$read;
    }
    /**
	 * Reads the frame from the client: first two bytes which are processed in Frame class (FIN, RSV, Opcode, MASK, Payload length);
	 * based on these information it reads 0, 2 or 8 bytes (extended payload length); then if the MASK is set to 1, it reads the
	 * masking key, then it reads the payload length bytes, finally extracts and unmask the payload
	 * @see Frame
	 * @see https://tools.ietf.org/html/rfc6455
	 * @throws WebSocketFrameException if the frame doesn't meet the requirements of RFC
	 * @return parsed frame as a Frame object
	 */
	private function readFrame() {
		//try {	
			if (($rawheader = $this->read(2)) == false) {
				return false;
			}
			$frame = new Frame();
			$frame->processHeader($rawheader);
			if ($frame->getLength() <= 125) {

			} elseif ($frame->getLength() == 126) {
				if (($rawlength = $this->read(2)) == false) {
					throw new WebSocketFrameException(sprintf("%s, Could not read the extended length bits (%s)", 
												$this->hostname, __FUNCTION__));
				}
				$length = unpack('n', $rawlength);
				$frame->setLength($length[1]);
			} elseif ($frame->getLength() == 127) {
				if (($rawlength = $this->read(8)) == false) {
					throw new WebSocketFrameException(sprintf("%s, Could not read the extended length bits (%s)", 
												$this->hostname, __FUNCTION__));
				}
				$length = unpack('n4', $rawlength);
				$len = $length[4];
				$len +=  ($length[3] << 16);
				$len +=  ($length[2] << 32);
				$len +=  ($length[1] << 48);
				$frame->setLength($len);
			} else {
				throw new WebSocketFrameException(sprintf("%s, Error while reading the frame (%s)", 
												$this->hostname, __FUNCTION__));
			}
			if ($frame->isMasked()) {
				if (($rawmask = $this->read(4)) == false) {
					throw new WebSocketFrameException(sprintf("%s, Cannot read the mask bits (%s)", 
												$this->hostname, __FUNCTION__));
				}
				$mask = unpack('a4', $rawmask);
				$frame->setMask($mask[1]); 
			}
			
			if ($frame->getLength() == 0) {
				$frame->setPayload("");
			} else {
				if (($rawpayload = $this->read($frame->getLength())) == false) {
					throw new WebSocketFrameException(sprintf("%s, Cannot read the payload (%s)", 
													$this->hostname, __FUNCTION__));
				}
				$payload = unpack('a*', $rawpayload);
				$frame->setPayload($payload[1]);
			}
			return $frame;
		/*}
		catch (\Exception $e) {
			$this->close();
			return false;
		}*/
	}
}