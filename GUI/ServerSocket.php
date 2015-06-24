<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/ZMQHandler.php';

class ServerSocket {
	private $clients;

	private $serverSocket;
	private $zmqh;

	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();
		$this->zmqh = new ZMQHandler();

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
    			//$this->handshake($socket);
    			array_push($this->clients, $socket);
    		} else {
    			echo "ERROR: niepoprawy socket_accept (nie powinno sie do zdazyc)";
    		}
    	}
	}
	private function handshake(&$socket) {
		try {
			if (($read = $this->read($socket)) == false) {
				echo("Cannot read handshake");
			}
			print_r($read);
			$h = new Handshake($read);
			$response = $h->parseHandshake();
			

		} catch (\Exception $e) {
			return false;
		}
		return true;
	}
	protected function read(&$socket, $toRead = 1500) {
		$return = stream_socket_recvfrom($socket, $toRead);
        return $return;
    }
	private function readClientSockets() {
		echo count($this->clients).PHP_EOL;
		if (empty($this->clients)) {
			return;
		}
		$write  = NULL;
		$except = NULL;
		$clientsArray = array_filter($this->clients);
		//print_r($this->client);
		if (($no_changed = stream_select($clientsArray, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($no_changed > 0) {
    		foreach($clientsArray as $socket) {
		        if (($data = $this->read($socket)) === false) {
		        	echo "failed to read socket".PHP_EOL;
		        }
				echo "Reading out data from id ".intval($socket).": '".$data."'".PHP_EOL;
		        if(!$data) {
		            unset($this->clients[array_search($socket, $this->clients)]);
		            @fclose($socket);
		            echo "A client id ".intval($socket)." disconnected. Now there are total ". count($this->clients) . " clients".PHP_EOL;
		            continue;
		        }
		        echo "Sending response to id ".intval($socket).PHP_EOL;
		        //send the message back to client
		        fwrite($socket, "received");
		    }
    	}
	}
}
?>