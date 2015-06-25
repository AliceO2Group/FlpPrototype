<?php
namespace CERN\Alice\DAQ\O2;

class WSManager {

		protected function sendResponse($opcode, $payload) {
		if ($opcode == Frame::OPCODE_TXT) {
			$response = $this->clientObserver->onTextMessage($payload);			
			$this->pushMessage($response);
		} else if ($opcode == Frame::OPCODE_CLS) {
			$this->pushMessage($payload, Frame::OPCODE_CLS);	
			$this->connected = false;
		} else if ($opcode == Frame::OPCODE_PING) {
			$this->pushMessage($payload, Frame::OPCODE_PONG);
		}	
	}

}
?>