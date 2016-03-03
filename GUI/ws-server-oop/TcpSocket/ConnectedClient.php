<?php

declare(strict_types=1);
namespace CERN\Alice\DAQ\O2;

require_once __DIR__.'/../WebSocket/Handshake.php';
require_once __DIR__.'/../Exceptions/TcpException.php';
require_once __DIR__.'/../WebSocket/Frame.php';
require_once __DIR__.'/../Exceptions/WebSocketException.php';

use CERN\Alice\DAQ\O2\Exceptions\TcpException;
use CERN\Alice\DAQ\O2\Exceptions\WebSocketException;

class ConnectedClient extends Socket {
	
	/// End of HTTP header string
	const END_OF_HEADER = "\r\n\r\n";	
	
	/// Flag that states whether client is handshaked or not
	private $handshaked = false;
	
	/// Invokes constructor of Socket (parent) class
	public function __construct(&$socket) 
	{
		parent::__construct($socket);
	}

	/// Reads handshake (if it was not done before) or WebSocket frame
	/// \return read message that is forwarder to ZMQ server
	public function readSocket():
	{
    		try {
			if (!$this->handshaked) {
				$this->handshake();
				return json_encode(array('action' => 'connected', 'hostname' => $this->hostname), true);
			} else {
				$frame = $this->readFrame();
				/// check whether frame is correct or not
				$frame->controlFrame();
				return $this->sendResponse($frame->getOpcode(), $frame->getPayload());
			}
    		} catch (\Exception $e) {
			$this->close();
			return '';
		}	
    	}

	/// Sends a response to WebSocket control frames
	/// - text message - according to Client Observer
	/// - close frame - sends a close confirmation
	/// - ping frame - sends a pong frame
	/// \param $opcode - WebSocket opcode (frame type)
	/// \param $payload - frame payload
	private function sendResponse(int $opcode, string $payload): string
	{
		if ($opcode == Frame::OPCODE_TXT) {
			return $payload;
		} else if ($opcode == Frame::OPCODE_CLS) {
			$this->sendFrame($payload, Frame::OPCODE_CLS);
			$this->close();
			return '';
		} else if ($opcode == Frame::OPCODE_PING) {
			$this->sendFrame($payload, Frame::OPCODE_PONG);
			return null;
		}	
	}
	public function sendFrame($message, $opcode = Frame::OPCODE_TXT) {
		$fToSend = new Frame($opcode, $message);
		$this->write($fToSend->encodeFrame());
		unset($fToSend);	
	}
	protected function readHandshake(): string
	{
		$output = '';
                for (;;) {
                        if (($output .= $this->readNotExactly()) == false) {
				throw new WebSocketException(sprintf("%s, Cannot read handshake (null string or time out)", $this->hostname));
			}
                        if (substr($output, -4) == self::END_OF_HEADER) break;
                }
		return $output;

	}
    private function handshake() {
	$read = $this->readHandshake();
    	$h = new Handshake($read);
	$this->write($h->handshakeResponse());
	$this->handshaked = true;
		//unset($h);
		/*if (strpos($response[0], '101') == false) {
			throw new WebSocketException(sprintf("%s Invaild handshake", $this->hostname));	
		}*/
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
	public function readFrame() {
			$rawheader = $this->read(2);
			$frame = new Frame();
			$frame->processHeader($rawheader);
			if ($frame->getLength() <= 125) {

			} elseif ($frame->getLength() == 126) {
				if (($rawlength = $this->read(2)) == false) {
					throw new WebSocketException(sprintf("%s, Could not read the extended length bits (%s)", 
												$this->hostname, __FUNCTION__));
				}
				$length = unpack('n', $rawlength);
				$frame->setLength($length[1]);
			} elseif ($frame->getLength() == 127) {
				if (($rawlength = $this->read(8)) == false) {
					throw new WebSocketException(sprintf("%s, Could not read the extended length bits (%s)", 
												$this->hostname, __FUNCTION__));
				}
				$length = unpack('n4', $rawlength);
				$len = $length[4];
				$len +=  ($length[3] << 16);
				$len +=  ($length[2] << 32);
				$len +=  ($length[1] << 48);
				var_dump($len);
				$frame->setLength($len);
			} else {
				throw new WebSocketException(sprintf("%s, Error while reading the frame (%s)", 
												$this->hostname, __FUNCTION__));
			}
			if ($frame->isMasked()) {
				if (($rawmask = $this->read(4)) == false) {
					throw new WebSocketException(sprintf("%s, Cannot read the mask bits (%s)", 
												$this->hostname, __FUNCTION__));
				}
				$mask = unpack('a4', $rawmask);
				$frame->setMask($mask[1]); 
			}
			
			if ($frame->getLength() == 0) {
				$frame->setPayload("");
			} else {
				if (($rawpayload = $this->read($frame->getLength())) == false) {
					throw new WebSocketException(sprintf("%s, Cannot read the payload (%s)", 
													$this->hostname, __FUNCTION__));
				}
				$payload = unpack('a*', $rawpayload);
				$frame->setPayload($payload[1]);
			}
			return $frame;
	
	}
}
?>
