<?php
/// \file ZMQHandler.php
/// \brief Definition and implementation of the CERN\Alice\DAQ\O2\ZMQHandler class.
///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/../Exceptions/ZMQHandlerException.php';

use CERN\Alice\DAQ\O2\Exceptions\ZMQHandlerException;
use ZMQ, ZMQSocket, ZMQContext;

/// Handles connection with ZeroMQ sockets
///
/// Subscribes to ZeroMQ server's sockets: (1) PUL-SUB (2) REQ-REP
/// It is created for each connected client.
class ZMQHandler {

	private $subscriber;
	private $request;

	const ZMQ_REQ_TIMEOUT = 1000;
	const SUB = "tcp://127.0.0.1:5444/";
	const REQ = "tcp://127.0.0.1:6444/";

	public function __construct() {
		$this->subscriber = new ZMQSocket(new ZMQContext(), ZMQ::SOCKET_SUB);
		$this->request = new ZMQSocket(new ZMQContext(), ZMQ::SOCKET_REQ);
		try {
			$this->subscriber->connect(self::SUB);
			$this->request->connect(self::REQ);
			$this->request->setSockOpt(ZMQ::SOCKOPT_RCVTIMEO, self::ZMQ_REQ_TIMEOUT);
			$this->subscriber->setSockOpt(ZMQ::SOCKOPT_SUBSCRIBE, ""); //this is obligatory to subscribe to all filters
		} catch (\Exception $e) {
			Log::write(sprintf("Unable to connect to ZeroMQ socket: %s", $e->getMessage()));
		}
	}
	public function sendMessage($message) {
		try {
			$this->request->send($message);
			if (($message = $this->request->recv()) === false) {
				throw new ZMQHandlerException('ZMQ_REQ Timeout after ' . self::ZMQ_REQ_TIMEOUT . 'ms');
			}
		} catch (\Exception $e) {
			Log::write(sprintf("Unable to communicate with ZMQ_REQ: %s", $e->getMessage()));
			return false;
		}
		return $message;
	}
	public function checkMessage() {
		$string = $this->subscriber->recv(ZMQ::MODE_DONTWAIT);
		if (($string === null) || (strlen($string) <= 0) || ($string === false)) {
			return false;
		}
		return $string;
	}
}
?>