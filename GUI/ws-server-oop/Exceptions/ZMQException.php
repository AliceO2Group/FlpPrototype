<?php
namespace CERN\Alice\DAQ\O2\Exceptions;

require_once __DIR__.'/CustomException.php';

class ZMQException extends CustomException {
    public function __construct($message = null, $code = null, $previous = null)
    {
        parent::__construct($message, $code, $previous);
    }
}
?>
