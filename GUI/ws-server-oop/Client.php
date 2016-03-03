<?php
declare(strict_types=1);
namespace CERN\Alice\DAQ\O2;

require_once __DIR__.'/Log.php';
require_once __DIR__.'/Exceptions/TcpException.php';
require_once __DIR__.'/WebSocket/Frame.php';
require_once __DIR__.'/WebSocket/Handshake.php';
require_once __DIR__.'/TcpSocket/Socket.php';
require_once __DIR__.'/TcpSocket/ConnectedClient.php';

use CERN\Alice\DAQ\O2\Exceptions\TcpException;

/// WebSocket client to test server implementation
class Client extends ConnectedClient {
	
	/// Socket timeout
	const SOCKET_TIMEOUT = 30;
	
	/// Remote endpoint URL (server)
	private $url;
	
	/// Local endpoint URL
	private $local;
	
	/// Validates and stores provided URL
	public function __construct(string $url) 
	{
		if (\filter_var($url, FILTER_VALIDATE_URL) === false) {
			throw new TcpException(sprintf("%s Provided URL is invalid", $url));
		}
		$this->url = $url;
		$this->local = gethostname();
	}

	/// Connects to server using
	/// Sets socket timeout
	/// throw TcpException when it fails to connect
	public function connect() 
	{
		$socket = stream_socket_client($this->url, $errno, $errorMessage);
		if ($socket === false) {
			throw new TcpException("Failed to connect: $errorMessage");
		}
		stream_set_timeout($socket, self::SOCKET_TIMEOUT);
		parent::__construct($socket);
	}
	
	/// Closes TCP connection
	public function __destruct() 
	{
		$this->close();
	}

	/// Generates and sends WebSocket handshake
	/// \return generated handshake
	public function handshake(): string
	{
		$hdshk = '';
		$hdshk .= 'GET ' . $this->url . ' ' . Handshake::HTTP_PROTOCOL . "\r\n";
		$hdshk .= Handshake::HEADER_ORIGIN . ': ' . $this->local . "\r\n";
		$hdshk .= Handshake::HEADER_HOST . ': ' . $this->url . "\r\n";
		$hdshk .= Handshake::HEADER_KEY . ': ' . base64_encode(substr(uniqid("", true), 0, 16)) . "\r\n";
		$hdshk .= Handshake::HEADER_UPGRADE . ': websocket' . "\r\n";
		$hdshk .= Handshake::HEADER_CONNECTION . ': Upgrade' . "\r\n";
		$hdshk .= Handshake::HEADER_VERSION . ': 13'. "\r\n";
		$hdshk .= "\r\n";
		$this->write($hdshk);
		return $hdshk;
	}

	/// Compares handshake received from server with one calculates on client side
	/// \param $sentHandshake - handshake that was sent to server
	/// return true/false whether handshake matches or not
	public function verifyHandshake(string $sentHandshake) 
	{
		$h = new Handshake($sentHandshake);
		return ($h->handshakeResponse() == $this->readHandshake());
	}

	/// Disconnects from server by sending closing frame
	public function disconnect() 
	{
		$this->sendFrame('', Frame::OPCODE_CLS);
	}

}
$options = getopt("c:");
if (!isset($options['c'])) {
	echo sprintf('Usage: %s -c <websocket_URL>' . PHP_EOL, $argv[0]);
	exit(1);
}

$c = new Client($options['c']);
$c->connect();
$h = $c->handshake();
$c->verifyHandshake($h);
$c->sendFrame("Hello");
echo $c->readSocket();
$c->disconnect();
?>
