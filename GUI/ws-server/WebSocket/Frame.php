<?php
namespace CERN\Alice\DAQ\O2;

require_once __DIR__.'/../Exceptions/WebSocketFrameException.php';

use CERN\Alice\DAQ\O2\Exceptions\WebSocketFrameException;
/**
 * Decodes and stores incomming frames or encodes outgoing frames
 * 
 * @see http://tools.ietf.org/html/rfc6455#section-5.2
 */

class Frame {
	
	/**
	 * Frame opcodes as in RFC 6455 (page 29)
	 * @see http://tools.ietf.org/html/rfc6455#page-29
	 */
	const OPCODE_CNT	= 0x0;
	const OPCODE_TXT	= 0x1;
	const OPCODE_BIN	= 0x2;
	const OPCODE_CLS	= 0x8;
	const OPCODE_PING	= 0x9;
	const OPCODE_PONG	= 0xA;
	
	/**
	 * The FIN bit, states if it's finnishing frame (1) or not (0)
	 */
	private $fin;
	/**
	 * 3 bits of the RSV, reserved for WebSocket extensions
	 */
	private $rsv;
	/**
	 * 4 bits of the Opcode, decides about type of the frame
	 */
	private $opcode;
	/**
	 * The MASK bit, says if the payload is masked
	 */
	private $isMasked;
	/**
	 * Full lenght of the frame saved on 7, 16 or 64 bits
	 */
	private $length;
	/**
	 * 4 bits The Masking-key, null if the MASK is set to 0
	 */
	private $mask;
	/**
	 * payload (masked or unmasked accroding to isMasked)
	 */
	private $payload;

	/**
	 * Constructs empty frame to be filled with data comming from socket
	 * or creates frame based on Opcode, Payload
	 * @param $opcode
	 * @param $payload
	 */
	public function __construct($opcode = null, $payload = null) {
		if ($opcode != null) {
			$this->opcode = $opcode;
			$this->fin = 1;
			$this->rsv = 0;
			$this->isMasked = 0;
		}
		if ($payload == null) {
			$this->payload = "";
			$this->length = 0;
		} else {
			$this->payload = $payload;
			$this->length = strlen($payload);
			//$this->strBytes($payload); if using characters that size is more than 1 byte each
		} 
	}
	/**
	 * Parses first 2 bytes of the frame, extracts FIN, RSV, Opcode, MSK, Lenght bits
	 *
	 * EDIT: Firefox sends close frame (2 close frames?) that is only 1 byte long (136)10 or (10001000)b, that why
	 * if is needed (looks like bug cos each websocket frame need first 2 bytes)
	 * @var $rawHeader data from the socket
	 */
	public function processHeader($rawheader) {
		if (strlen($rawheader) < 2) {
			$f = unpack('CByte', $rawheader);
			$this->fin 		= (0x80 & $f['Byte']) >> 7;
			$this->rsv 		= (0x70 & $f['Byte']) >> 4;
			$this->opcode 	= (0xF  & $f['Byte']);
		} else {
			$f = unpack('C2Byte', $rawheader);
			$this->fin 		= (0x80 & $f['Byte1']) >> 7;
			$this->rsv 		= (0x70 & $f['Byte1']) >> 4;
			$this->opcode 	= (0xF  & $f['Byte1']);
			$this->isMasked	= (0x80 & $f['Byte2']) >> 7;
			$this->length 	= (0x7F & $f['Byte2']);
		}
		unset($f);
	}
	/**
	 * Encodes the frame, before writting socket
	 */
	public function encodeFrame() {
		$byte = array();
		$byte[1] = $this->fin << 7;
		$byte[1] = $byte[1] | $this->rsv << 4;
		$byte[1] = $byte[1] | $this->opcode;
		$byte[2] = $this->isMasked << 7;
		if ($this->length <= 125) {
			$byte[2] = $byte[2] | $this->length;
			$frame = pack('C2a*',  $byte[1], $byte[2], $this->payload);
		} elseif($this->length <= 65536) {
			$byte[2] = $byte[2] | 0x7E;
			$frame = pack('C2na*', $byte[1], $byte[2], $this->length, $this->payload);
		} else {
			$byte[2] = $byte[2] | 0x7F;
			//max size of the frame ~4Gb
			//$frame = pack('CCNN', $byte[1], 127, 0, $this->length) . $this->payload;
			$frame = pack('C2N2a*', $byte[1], $byte[2], 0, $this->length, $this->payload);

		}
		return $frame;
	}
	/**
	 * Checks if the frame is consistent with RFC 6455: RSV = 0, 
	 * length of the control frame less than 125 bytes.
	 * Possible to implement more constrains.
	 * 
	 */
	public function controlFrame() {
		//try {
			if ($this->getRsv() != 0) {
				echo $this->frameDump();
				throw new WebSocketFrameException("RSV must be 0", 1002);
			}
			switch ($this->getOpcode()) {
				case Frame::OPCODE_CNT: /* last continuation frame, fin must be 1 */
					throw new WebSocketFrameException("Continuation frames are not supported yet");
				break; 
				case Frame::OPCODE_TXT: /* text frame - 0x1 */		
					break;
				case Frame::OPCODE_BIN: /* binary frame - 0x2; NOT SUPPORTED */
					throw new WebSocketFrameException(sprintf("Binary frame is not supported: [%d]", $this->getOpcode()));
					break; 
				case Frame::OPCODE_CLS: /* closing frame - 0x8 */
					break;
				case Frame::OPCODE_PING: /* ping - 0x9*/
					if ($this->getLength() > 125) {
						throw new WebSocketFrameException("Ping frame with payload of more than 125 bytes");
					}
					break;
				case Frame::OPCODE_PONG: /*pong - 0xA */
					/* Commented, to avoid loss of data that might arrive after pong frame - Autobahn Testsuite Case 2.9 */
					//throw new WebSocketFrameException(sprintf("Unsolicited pong frame: [%d]", $this->getOpcode())); 
					break;
				default: /* reserveed: 0x3 - 0x7 and 0xB - 0xF */ 
					throw new WebSocketFrameException("Unknows WS frame");
			}
		/*} catch (\Exception $e) {
			return false;
		} 
		return true;*/
	}
	/**
	 * Mask or unmask the payload in the frame
	 * @param $payload
	 * @param $mask masking key of the rame
	 */
	protected function mask($payload, $mask) {
		for ($i = 0, $k = strlen($payload); $i < $k; ++$i) {
			$payload[$i] = $payload[$i] ^ $mask[$i % 4];
		}
		return $payload;
	}
	/**
	 * Length getter
	 */
	public function getLength() {
		return $this->length;
	}
	/**
	 * Length setter
	 */
	public function setLength($length) {
		$this->length = $length;
	}
	/**
	 * MASK bit getter
	 */
	public function isMasked() {
		if ($this->isMasked) return true;
		else return false;
	}
	/**
	 * Mask setter
	 */
	public function setMask($mask) {
		$this->mask = $mask;
	}
	/**
	 * Opcode getter
	 */
	public function getOpcode() {
		return $this->opcode;
	}
	/**
	 * FIN bit getter
	 */
	public function getFin() {
		return $this->fin;
	}
	/**
	 * Payload setter, also masks the payload if needed
	 * @param $payload
	 */
	public function setPayload($payload) {
		if ($this->isMasked) {
			$payload = $this->mask($payload, $this->mask);
		}
		$this->payload = $payload;
	}
	/**
	 * Payload getter
	 */
	public function getPayload() {
		return $this->payload;
	}
	/**
	 * RSV getter
	 */
	public function getRsv() {
		return $this->rsv;
	}
	/**
	 * Frame dump, just for test reasons
	 */
	public function frameDump() {
		return sprintf('F(%d)R(%d)O(%d) "%s" [%d]', $this->fin, $this->rsv, $this->opcode, substr($this->payload, 0, 15), $this->length);
	}

}