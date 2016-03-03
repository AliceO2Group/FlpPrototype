<?php
/// \brief Handles connectivity with ZeroMQ servers
///
/// Two ZeroMQ socket patterns:
///   1. REQ-REP: Messages received from WebSocket client is forwarded to request socket of ZeroMQ server; then the server's response goes back to the client
///   2. PUB-SUB: Messages coming from publisher socket are broadcast to all connected WebSocket clients
///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
declare(strict_types=1);
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/../Exceptions/ZMQException.php';

use CERN\Alice\DAQ\O2\Exceptions\ZMQException;
use ZMQ, ZMQSocket, ZMQContext;

class ZMQHandler {
	/// ZMQ subsriber socket
	private $subscriber;

	/// ZMQ request socket
	private $request;
	
	/// Connects to PUB-SUB and REQ-REP sockets
	/// Subscribes to all incoming messages (empty filter)
	public function __construct() 
	{
		$this->subscriber = new ZMQSocket(new ZMQContext(), ZMQ::SOCKET_SUB);
		$this->request = new ZMQSocket(new ZMQContext(), ZMQ::SOCKET_REQ);
		try {
			$this->subscriber->connect(Config::$zmq['sub']);
			$this->request->connect(Config::$zmq['req']);
			$this->request->setSockOpt(ZMQ::SOCKOPT_RCVTIMEO, Config::$zmq['timeout']);
			$this->subscriber->setSockOpt(ZMQ::SOCKOPT_SUBSCRIBE, "");
		} catch (\Exception $e) {
			Log::write(sprintf("Unable to connect to ZeroMQ socket: %s", $e->getMessage()));
		}
	}
	
	/// Sends message to  REQ-REP socket and than awaits response
	/// \param $message - message to be sent
	/// \return response from ZMQ server
	public function sendMessage(string $message): string 
	{
		return $message;
		try {
			$this->request->send($message);
			if (($message = $this->request->recv()) === false) {
				throw new ZMQException('ZMQ_REQ Timeout after ' . Config::$zmq['timeout'] . 'ms');
			}
		} catch (\Exception $e) {
			Log::write(sprintf("Unable to communicate with ZMQ_REQ: %s", $e->getMessage()));
			return "";
		}
		return $message;
	}
	
	/// Checks is there any message provided by publisher
	/// \returns received message (or empty string)
	public function checkMessage(): string 
	{
		$string = $this->subscriber->recv(ZMQ::MODE_DONTWAIT);
		if (($string === null) || ($string === false) || (strlen($string) <= 0)) {
			return "";
		}
		return $string;
	}
}
?>
