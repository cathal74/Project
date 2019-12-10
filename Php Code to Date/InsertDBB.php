<?php

    //Connect to database
    $servername = "localhost";		
    $username = "root";		
    $password = "";	
    $dbname = "tempdata";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Database Connection failed: " . $conn->connect_error);
    }

    //Get current date and time
    date_default_timezone_set('Europe/Dublin');
    $d = date("Y-m-d");
    $t = date("H:i:s");

    if(!empty($_POST['tempvalue']))
    {
		$tempvalue = $_POST['tempvalue'];
	    
		$sql = "INSERT INTO temp_data_table (temp, Date, Time) 
		
		VALUES ('".$tempvalue."', '".$d."', '".$t."')";

		if ($conn->query($sql) === TRUE) {
		    echo "OK";
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}
	}


	$conn->close();
?>