<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/../WebSocket/Handshake.php';
require_once __DIR__.'/../Exceptions/ClientException.php';
require_once __DIR__.'/../WebSocket/Frame.php';

use CERN\Alice\DAQ\O2\Exceptions\ClientException;


class ConnectedClient extends Socket {

	const HANDSHAKE_2_BYTES = 'GE';

	public function __construct(&$socket) {
		parent::__construct($socket);
	}
	public function readSocket() {
    	try {
	    	if (($rawheader = $this->read(2)) == false) {
				throw new ClientException(sprintf("%s, Cannot read first 2 bytes", $this->hostname));
			}
			if (strcmp($rawheader, self::HANDSHAKE_2_BYTES) === 0) { //is_string
				$this->handshake();
				return null;
			} else {
				$frame = $this->readFrame($rawheader);
				$frame->controlFrame();
				return $this->sendResponse($frame->getOpcode(), $frame->getPayload());

			}
    	} catch (\Exception $e) {
    		$this->close();
			return false;
		}	
    }
    /**
	 * Sends a response to client's request:
	 * - text message - according to Client Observer
	 * - close frame - sends a close confirmation
	 * - ping frame - sends a pong frame
	 * @param $message - message to sent
	 * @param $opcode - opcode of message
	 */
	private function sendResponse($opcode, $payload) {
		if ($opcode == Frame::OPCODE_TXT) {
			return $payload;
		} else if ($opcode == Frame::OPCODE_CLS) {
			$this->sendFrame($payload, Frame::OPCODE_CLS);
			$this->close();
			return $this->id;
		} else if ($opcode == Frame::OPCODE_PING) {
			$this->sendFrame($payload, Frame::OPCODE_PONG);
			return null;
		}	
	}
	public function sendFrame($message, $opcode = Frame::OPCODE_TXT) {
		$fToSend = new Frame($opcode, $message);

		if ($this->write($fToSend->encodeFrame()) === false) {
			throw new ClientException(sprintf("Could not write frame [%d]: %s", $opcode, substr($message, 0, 30)));
		}
		unset($fToSend);	
	}
    private function handshake() {
    	if (($read = $this->read()) == false) {
			throw new ClientException(sprintf("%s, Cannot read handshake", $this->hostname));
		}
		$read = self::HANDSHAKE_2_BYTES.$read;
    	$h = new Handshake($read);
		$response = $h->parseHandshake();
		unset($h);
		$this->sendHttp($response);
		if (strpos($response[0], '101') == false) {
			throw new ClientException(sprintf("%s Invaild handshake", $this->hostname));	
		}
    }

    protected function sendHttp($response) {
		foreach ($response as $r) {
			if ($this->write($r . "\r\n") === false) {
				throw new ClientException(sprintf("%s, Cannot send HTTP response", $this->hostname));
			}
		}
		return true;
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
	private function readFrame($rawheader) {
		//try {	
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
?>