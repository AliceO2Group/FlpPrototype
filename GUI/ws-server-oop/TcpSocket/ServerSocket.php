<?php
declare(strict_types=1);
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/Socket.php';
require_once __DIR__.'/ConnectedClient.php';
require_once __DIR__.'/../Observers/ZMQHandler.php';
require_once __DIR__.'/SslContext.php';


/// Represents TCP socket that WebSocket clients connects to; supports TLS and SSL
class ServerSocket {

	/// array of connected clients
	private $clients;
	
	/// server socket resource
	private $serverSocket;

	/// WebSocket server observer, gets notfied about new clients and new websocket messages
	private $zmqh;
	
	/// Inits client array, and bind server socket
	public function __construct() {
		$this->clients = array();
		$this->bindServerSocket();
		$this->zmqh = new ZMQHandler();
	}

	/// Binds server socket
	private function bindServerSocket() {
		$this->serverSocket = stream_socket_server(
			Config::$socket['url'], 
			$errno, $errstr, 
			STREAM_SERVER_BIND|STREAM_SERVER_LISTEN,
			SslContext::getContext()
		);
		stream_socket_enable_crypto($this->serverSocket, false);
	}

	/// Server main loop: listen for new client, listen for new messages form connected clients, listen for messages from ZMQ publisher
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

	/// Check if there is new connection to server socket
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

	/// Returns array of references to all connected sockets
	/// If a socket is unvalid it is removed (Socket destructor closes connection)
	/// \todo Check whether removing element while looping over refereces is safe
	private function getSocketsArray(): array 
	{
		$sockets = array();
		foreach ($this->clients as $key => &$client) {
			// checks if socket is still conected (resource of type stream)
			if ($client->isValid() === false) {
				unset($this->clients[$key]);
				continue;
			}		
			// get reference of socket
			array_push($sockets, $client->getSocket());
		}
		return $sockets;
	}

	/// Broadcast WebSocket frame to all connected clients
	/// \param $payload - frame body
	private function pushToAll(string $payload) {
		foreach ($this->clients as &$client) {
			$client->sendFrame($payload);
		}
	}

	/// Creates list of sockets that change their status
	private function readClientSockets() {
		if (empty($sockets = $this->getSocketsArray())) return;
		$write  = $except = NULL;
		if (($noChanged = stream_select($sockets, $write, $except, 0)) === false) {
        	//no awaiting connection
    	} elseif ($noChanged > 0) {
    		foreach ($sockets as &$socket) {
    			$id = intval($socket);
    			$data = $this->clients[$id]->readSocket();
    			// If payload not empty forward to ZMQ server and broadcast its response
    			if (strlen($data) > 0) {
    				$zmqRsp = $this->zmqh->sendMessage($data);
    				$this->pushToAll($zmqRsp);
    			}
		    }
    	}
	}
}
?>
