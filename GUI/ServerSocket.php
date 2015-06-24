<?php
namespace CERN\Alice\DAQ\O2;

class ServerSocket {

	private $clients;

	private $serverSocket;

	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();

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
			usleep(1000);
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
    		} else {
    			echo "ERROR: niepoprawy socket_accept (nie powinno sie do zdazyc)";
    		}
    	}
	}
	private function readClientSockets() {
		if (count($this->clients) <= 0) {
			return;
		}
		$write  = NULL;
		$except = NULL;
		$clientsArray = $this->clients;
		if (($no_changed = stream_select($clientsArray, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($no_changed > 0) {
    		foreach($clientsArray as $socket) {
		        $data = fread($socket, 128);
				echo "Reading out data from id ".intval($socket).": '".$data."'".PHP_EOL;
		        if(!$data) {
		            unset($client_socks[array_search($sock, $this->clients)]);
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