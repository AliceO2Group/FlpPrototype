<?php
	$fp = stream_socket_client("tcp://127.0.0.1:4444", $errno, $errstr, 30);
    while(true) {
    	fwrite($fp, "gowno gowno ".rand(1, 100));
    	sleep(1);
    	var_dump(fread($fp, 1024));
    }
    fclose($fp);
    ?>