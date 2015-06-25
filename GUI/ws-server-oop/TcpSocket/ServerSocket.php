<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/Socket.php';
require_once __DIR__.'/ConnectedClient.php';
//require_once __DIR__.'/../Observers/ZMQHandler.php';

class ServerSocket {
	private $clients;

	private $serverSocket;
	private $zmqh;
	private $hshm;
	private $tcph;
	private $observer;

	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();
		//$this->zmqh = new ZMQHandler();
		//$this->hshm = new HandshakeManager();
		//$this->tcph = new TCPHandler();
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
			usleep(100000);
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
    			$id = intval($socket);
    			$this->clients[$id] = new ConnectedClient($socket);
    			Log::write(sprintf("Client #%s %s connected. [%s]", intval($socket), stream_socket_get_name($socket, true), count($this->clients)));
    		} else {
    			Log::write("socket_accept failed, it should never happen!", 'error');
    		}
    	}
	}
	private function getSocketsArray() {
		$sockets = array();
		foreach ($this->clients as &$client) {
			array_push($sockets, $client->getSocket());
		}
		return $sockets;
	}
	private function readClientSockets() {
		if (empty($sockets = $this->getSocketsArray())) return;
		$write  = $except = NULL;
		if (($no_changed = stream_select($sockets, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($no_changed > 0) {
    		foreach ($sockets as $socket) {
    			$id = intval($socket);
    			$data = $this->clients[$id]->readSocket();
    			if (gettype($data) == 'integer') {
    				unset($this->clients[$data]);
    			}
		    }
    	}
	}
}
?>