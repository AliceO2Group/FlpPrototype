<?php
namespace CERN\Alice\DAQ\O2;

use ZMQ, ZMQSocket, ZMQContext;

class ZMQHandler {
	private $subscriber;

	public function __construct() {
		$this->subscribe();
	}

	private function subscribe() {
		$context = new ZMQContext();
		$this->subscriber = new ZMQSocket($context, ZMQ::SOCKET_SUB);
		try {
			$this->subscriber->connect("tcp://127.0.0.1:5444/");
		} catch (\Exception $e) {
			echo("ZMQ > Couldn't connect to the server");
		}
		$this->subscriber->setSockOpt(ZMQ::SOCKOPT_SUBSCRIBE, "");
	}
	public function sendMessage() {
		$context = new ZMQContext();
		$socket = $context->getSocket(ZMQ::SOCKET_REQ, 'Sock');
		$socket->setSockOpt(ZMQ::SOCKOPT_RCVTIMEO, 5000);
		$socket->connect("tcp://127.0.0.1:6444/");
		$socket->send('active_global_runs');
		
		$message = $socket->recv();
		echo "From ZMQ-RSP > ".$message.PHP_EOL;
	}
	public function checkMessage() {
		$string = $this->subscriber->recv(ZMQ::MODE_DONTWAIT);
		if ($string === null || $string == '') {
			return;
		}
		echo "From ZMQ-PUB > ".$string.PHP_EOL;
	}
}
?>