<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/TCPHandler.php';
require_once __DIR__.'/../Observers/ZMQHandler.php';
require_once __DIR__.'/../Observers/WSManager.php';
require_once __DIR__.'/../WebSocket/HandshakeManager.php';

class ServerSocket {
	private $clients;

	private $serverSocket;
	private $zmqh;
	private $hshm;
	private $tcph;
	private $observer;

	const FRAME_CLASS_NAME = 'CERN\Alice\DAQ\O2\Frame';

	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();
		$this->zmqh = new ZMQHandler();
		$this->hshm = new HandshakeManager();
		$this->tcph = new TCPHandler();
		$this->wsm = new WSManager();
	}
	private function bindServerSocket() {
		$this->serverSocket = stream_socket_server(
			'tcp://127.0.0.1:4444/', 
			$errno, $errstr, 
			STREAM_SERVER_BIND|STREAM_SERVER_LISTEN
		);
	}
	public function listen() {
		for (;;) {
			$this->checkNewClients();
			$this->readClientSockets();
			//$this->zmqh->sendMessage();
			//$this->zmqh->getMessage();
			usleep(1000000);
		}
		
	}
	private function checkNewClients() {
		$write  = NULL;
		$except = NULL;
		$ssArray = array($this->serverSocket);
		if (($no_changed = stream_select($ssArray, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($no_changed > 0) {
    		$socket = stream_socket_accept($this->serverSocket);
    		if ($socket) {
    			array_push($this->clients, $socket);
    			Log::write(sprintf("Client #%s %s connected. [%s]", intval($socket), stream_socket_get_name($socket, true), count($this->clients)));
    		} else {
    			Log::write("ERROR: niepoprawy socket_accept (nie powinno sie do zdazyc)");
    		}
    	}
	}
	private function broadcastMessage() {
		//foreach write
	}
	protected function sendResponse(&$socket, $frame) {
		$opcode = $frame->getOpcode();
		$payload = $frame->getPayload();
		if ($opcode == Frame::OPCODE_TXT) {
			//$response = $this->clientObserver->onTextMessage($payload);
			$this->tcph->sendFrame($payload, Frame::OPCODE_TXT);
			$this->tcph->write($socket, $response);
		} else if ($opcode == Frame::OPCODE_CLS) {
			$this->tcph->sendFrame($payload, Frame::OPCODE_CLS);
			$this->tcph->write($socket, $response);
			$this->close($this->socket);	
		} else if ($opcode == Frame::OPCODE_PING) {
			$this->tcph->sendFrame($payload, Frame::OPCODE_PONG);
			$this->tcph->write($socket, $response);
		}
		return true;
	}
	private function close(&$socket) {
		unset($this->clients[array_search($socket, $this->clients)]);
		$this->tcph->close($socket);
	}
	private function readClientSockets() {;
		//if no clients
		if (empty($this->clients)) return;
		$write  = $except = NULL;
		$clientsArray = $this->clients;
		if (($no_changed = stream_select($clientsArray, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($no_changed > 0) {
    		foreach ($clientsArray as $socket) {
    			//data returns WebSocket frame or handshake
		        if (($data = $this->tcph->readSocket($socket)) === false) {
		        	$this->close($socket);
		            continue;
		        }
		        //based on what readSocket returns; to be changed in PHP7
		        if (gettype($data) == 'object') {
		        	$response = $this->sendResponse($socket, $data);
		        } else {
		        	$response = $this->hshm->handshake($data);
		        }
		        if ($this->tcph->write($socket, $response) === false) {
		        	$this->close($socket);
		        	continue;
		        }
		    }
    	}
	}
}
?>