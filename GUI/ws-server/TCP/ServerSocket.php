<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/TCPHandler.php';
require_once __DIR__.'/../ZeroMQ/ZMQHandler.php';
require_once __DIR__.'/../WebSocket/HandshakeManager.php';

class ServerSocket {
	private $clients;

	private $serverSocket;
	private $zmqh;
	private $hshm;
	private $tcph;

	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();
		$this->zmqh = new ZMQHandler();
		$this->hshm = new HandshakeManager();
		$this->tcph = new TCPHandler();
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
		        	unset($this->clients[array_search($socket, $this->clients)]);
		            @fclose($socket);
		            Log::write(sprintf("Socket id %d has been closed. [%d]", intval($socket), count($this->clients)));
		            continue;
		        }
		        $response = $this->hshm->handshake($data);
		        //assumes (for now) only handshakes
		        Log::write("Sending response to id ".intval($socket));
		        //send the message back to client
		        stream_socket_sendto($socket, $response);
		    }
    	}
	}
}
?>