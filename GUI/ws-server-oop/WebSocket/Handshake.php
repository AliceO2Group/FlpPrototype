<?php
namespace CERN\Alice\DAQ\O2;

require_once __DIR__.'/../Exceptions/HandshakeException.php';
require_once __DIR__.'/../Exceptions/OriginException.php';

use CERN\Alice\DAQ\O2\Exceptions\HandshakeException;
use CERN\Alice\DAQ\O2\Exceptions\OriginException;
/**
 * Handles client handshake specified in RFC 6455 (Page 14)
 * @see http://tools.ietf.org/html/rfc6455#page-14
 */
class Handshake {
	
	const HTTP_PROTOCOL 	= "HTTP/1.1";
	/**
	 * HTTP header fileds
	 */
	const HEADER_HOST 		= 'Host';
    const HEADER_KEY 		= 'Sec-WebSocket-Key';
    const HEADER_PROTOCOL 	= 'Sec-WebSocket-Protocol';
    const HEADER_VERSION 	= 'Sec-WebSocket-Version';
    const HEADER_ACCEPT 	= 'Sec-WebSocket-Accept';
    const HEADER_EXTENSIONS = 'Sec-WebSocket-Extensions';
    const HEADER_ORIGIN 	= 'Origin';
    const HEADER_CONNECTION = 'Connection';
    const HEADER_UPGRADE 	= 'Upgrade';
    
    /**
     * HTTP error codes
     */
    const SWITCHING_PROTOCOLS = 101;
    const ERROR_BAD_REQUEST = 400;
    const ERROR_UNAUTHORIZED = 401;
    const ERROR_FORBIDDEN = 403;
    const ERROR_NOT_FOUND = 404;
    const ERROR_RATE_LIMITED = 420;
    const ERROR_UPGRADE_REQUIRED = 426;
    const ERROR_SERVER_ERROR = 500;  

	/**
	 * Parsed header
	 */
	protected $parsedHeader = array();
	
	/**
	 * Constructor of Hanshake class; parses HTTP header into an array
	 * @param $header - HTTP header as a string
	 */
	public function __construct($header) {
		$rows = explode("\n", $header);
		foreach ($rows as $row) {
			if (strpos($row,':')) {
				$temp = explode(":", $row);
				
				/* Explode fucntion destroys Origin URL, this is the quick fix */
				if (trim($temp[0])==self::HEADER_ORIGIN && count($temp) == 3) {
					$this->parsedHeader[trim($temp[0])]=trim($temp[1]).":".trim($temp[2]);
				} else {
					$this->parsedHeader[trim($temp[0])]=trim($temp[1]);
				}
				
			}
		}
	}
	/**
	 * 
	 * Websocket handshake restrictions according to RFC 6455 (Page 14)
	 * @see http://tools.ietf.org/html/rfc6455#page-14
	 * @throws HandshakeException
	 * @throws OriginException
	 */
	public function parseHandshake() {
			
		try {
			/* Upgrade header filed is mandatory */
			if (!isset($this->parsedHeader[self::HEADER_UPGRADE])) {
				throw new HandshakeException(sprintf(
					"Lack a %s header field", self::HEADER_UPGRADE), self::ERROR_BAD_REQUEST);
			}

			/* Sec-WebSocket-Version header filed is mandatory */
			if (!isset($this->parsedHeader[self::HEADER_VERSION])) {
				throw new HandshakeException(sprintf(
					"Lack a %s header field", self::HEADER_VERSION), self::ERROR_BAD_REQUEST);
			}
			
			/* Sec-WebSocket-Key header filed is mandatory */
			if (!isset($this->parsedHeader[self::HEADER_KEY])) {
				throw new HandshakeException(sprintf(
					"Lack a %s header field", self::HEADER_KEY), self::ERROR_BAD_REQUEST);		
			}
						
			/* Connection header filed is mandatory */
			if (!isset($this->parsedHeader[self::HEADER_CONNECTION])) {
				throw new HandshakeException(sprintf(
					"Lack a %s header field", self::HEADER_CONNECTION), self::ERROR_BAD_REQUEST
				);
			}
			
			/* Host header filed is mandatory */
			if (!isset($this->parsedHeader[self::HEADER_HOST])) {
				throw new HandshakeException(sprintf(
					"Lack a %s header field", self::HEADER_HOST), self::ERROR_BAD_REQUEST
				);
			}
			
			/* Upgrade header filed value must be "websocket" */
			if (strpos(strtolower($this->parsedHeader[self::HEADER_UPGRADE]), 'websocket') === false) {
				throw new HandshakeException(sprintf(
					"Value of %s header field must be: 'websocket', not '%s'", 
					self::HEADER_UPGRADE, $this->parsedHeader[self::HEADER_UPGRADE]), self::ERROR_BAD_REQUEST
				);
			}
			
			/* Connection header filed value must be "Upgrade" */
			if (strpos($this->parsedHeader[self::HEADER_CONNECTION], 'Upgrade') === false) {
				throw new HandshakeException(sprintf(
					"Value of %s header field must be: 'Upgrade'", self::HEADER_CONNECTION), self::ERROR_BAD_REQUEST
				);
			}	
			
			/* Version header filed valuse must be "13" */
			if ($this->parsedHeader[self::HEADER_VERSION] != 13) {
				throw new HandshakeException(sprintf(
					"Value of header field must be: 13", self::HEADER_VERSION), self::ERROR_UPGRADE_REQUIRED
				);
			}
			
			/* Key is 16 byte value, that is next base64-encoded. So it is 24 charactes long */
			if (strlen($this->parsedHeader[self::HEADER_KEY]) != 24) {
				throw new HandshakeException(sprintf(
					"The %s is invailed", self::HEADER_KEY), self::ERROR_BAD_REQUEST
				);
			}
			/* Origin Policy */
			if (!$this->originPolicy()) {
				throw new OriginException(sprintf( 
					"The access from this Origin is forbidden"), self::ERROR_FORBIDDEN
				);
			}
		} catch (\Exception $e) {
			return $this->httpError($e->getMessage(), $e->getCode());
		}
		return $this->handshakeResponse();
	}
	/**
	 * Check if the connection from the origin is permitted
	 * You can define not allowed origins in the config file
	 *
	 * TEMPORARY REMOVED BECAUSE OF SCOPE OF CONFIG VARIABLES
	 */
	protected function originPolicy() {
		/* if (isset($this->parsedHeader[self::HEADER_ORIGIN])) {
			$origin = $this->parsedHeader[self::HEADER_ORIGIN];
			foreach (Config::$origin_forbidden as $org) {
				if ($org == $origin) return false;
			}
		} */
		return true;
	}
	/**
	 * Generate handshake response according to RFC, example:
	 * 		HTTP/1.1 101 Switching Protocols
	 * 		Upgrade: websocket
	 * 		Connection: Upgrade
	 * 		Sec-WebSocket-Accept: n2gKbAD7E4rcFdk8fkYXcnUH/D8=
	 * 		Sec-WebSocket-Version: 13
	 * 		Server: ALICE-DAQ-ACR
	 */
	protected function handshakeResponse() {
		$accept = base64_encode(
				sha1(
					$this->parsedHeader[self::HEADER_KEY] . '258EAFA5-E914-47DA-95CA-C5AB0DC85B11', true
					));
		$response = array();
		array_push($response, self::HTTP_PROTOCOL.' 101 Switching Protocols');
		array_push($response, self::HEADER_UPGRADE.': websocket');
		array_push($response, self::HEADER_CONNECTION.': Upgrade');
		array_push($response, self::HEADER_ACCEPT.': ' . $accept);
		array_push($response, self::HEADER_VERSION.': 13');
		array_push($response, "");
		return $response;
	}
	/**
	 * Generate HTTP error header, adds non-standard fields: Error-Reason and Server name
	 * @param $message error reason
	 * @param $code HTTP error code
	 */
	protected function httpError($message = "No details", $code = 404) {
		$response = array();
		switch ($code) {
			case self::ERROR_BAD_REQUEST: /* 400 */
				array_push($response, self::HTTP_PROTOCOL.' 400 Bad Request');
				array_push($response, self::HEADER_VERSION.': 13');
			break;
			case self::ERROR_FORBIDDEN: /* 403 */
				array_push($response, self::HTTP_PROTOCOL.' 403 Forbidden');
			break;
			case self::ERROR_NOT_FOUND: /* 404 */
				array_push($response, self::HTTP_PROTOCOL.' 404 Not Found');
			break;
			case self::ERROR_UPGRADE_REQUIRED: /* 426 */
				array_push($response, self::HTTP_PROTOCOL.' 426 Upgrade Required');
			break;	
		}
		/**
		 * On June 2011, the first IETF draft was posted to deprecate the use of the "X-" prefix for non-standard headers.
		 * The reason is that when non-standard headers prefixed with "X-" become standard, removing the "X-" prefix 
		 * breaks backwards compatibility, forcing application protocols to support both names (E.g, x-gzip & gzip are
		 * now equivalent). So, the recommendation is to just name them sensibly without the "X-" prefix.
		 * On June 2012, the deprecation of "X-" prefix has become official as RFC 6648.
		 */
		array_push($response, "Error-Reason: ".$message);
		array_push($response, "Server: ".Config::$socket['server_name']);
		array_push($response, "");
		return $response;
	}
}
