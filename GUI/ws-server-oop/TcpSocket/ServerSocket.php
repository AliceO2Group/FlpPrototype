<?php
declare(strict_types=1);
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/Socket.php';
require_once __DIR__.'/ConnectedClient.php';
require_once __DIR__.'/../Observers/ZMQHandler.php';
require_once __DIR__.'/SslContext.php';

class ServerSocket {
	private $clients;

	private $serverSocket;
	private $zmqh;
	private $observer;

	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();
		$this->zmqh = new ZMQHandler();
	}
	private function bindServerSocket() {
		$this->serverSocket = stream_socket_server(
			Config::$socket['url'], 
			$errno, $errstr, 
			STREAM_SERVER_BIND|STREAM_SERVER_LISTEN,
			SslContext::getContext()
		);
		stream_socket_enable_crypto($this->serverSocket, false);
	}
	public function listen() {
		for (;;) {
			$this->checkNewClients();
			$this->readClientSockets();
			if (strlen($push = $this->zmqh->checkMessage()) !== 0) {
				$this->pushToAll($push);
			}
			usleep(10000);
		}
		
	}
	private function checkNewClients() {
		$write  = NULL;
		$except = NULL;
		$ssArray = array($this->serverSocket);
		if (($noChanged = stream_select($ssArray, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($noChanged > 0) {
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
	private function getSocketsArray(): array 
	{
		$sockets = array();
		foreach ($this->clients as $key => &$client) {
			// checks if socket is still conected (resource of type stream)
			if ($client->isValid() === false) {
				unset($this->clients[$key]);
				continue;
			}		
			array_push($sockets, $client->getSocket());
		}
		return $sockets;
	}
	private function pushToAll(string $payload) {
		foreach ($this->clients as &$client) {
			$client->sendFrame($payload);
		}
	}
	private function readClientSockets() {
		if (empty($sockets = $this->getSocketsArray())) return;
		$write  = $except = NULL;
		if (($noChanged = stream_select($sockets, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($noChanged > 0) {
    		foreach ($sockets as $socket) {
    			$id = intval($socket);
    			$data = $this->clients[$id]->readSocket();
    			//returns Resource ID that is in closed state
    			if (strlen($data) === 0) {
    				unset($this->clients[$data]);
    				//received payload of text frame
    			} else {
    				$zmqRsp = $this->zmqh->sendMessage($data);
    				$this->pushToAll($zmqRsp);
    			}
		    }
    	}
	}
}
?>
