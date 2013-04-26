<html>
  <head><title>PHP Test</title></head>
    <body>
      <?php
        /**
        * Documentation, License etc.
        *
        * @package LibraryInteractions
        */
        include 'db.php';
		
        $user_agent=$_SERVER['HTTP_USER_AGENT'];
        $user_agent_split = explode("/", $user_agent);
        $device_number = $user_agent_split[1];
		
        $query = "SELECT ID from Device WHERE Device_ID='" . $device_number . "' AND Active=1 LIMIT 1";
        $device_result = mysqli_query($cxn, $query) or die("couldn't execute device lookup query for: " . $device_number);
        $row = mysqli_fetch_assoc($device_result);
		$device_id = $row['ID'];

        $query2 = "SELECT ID from Interaction_Type WHERE Device_ID='" . $device_id . "' AND Button_ID='" . $_POST['button'] . "' AND Active=1 LIMIT 1";
        $interaction_type_result = mysqli_query($cxn, $query2) or die("couldn't execute interaction lookup query for device id: " . $device_id);
        $row = mysqli_fetch_assoc($interaction_type_result);
		$interaction_type_id = $row['ID'];

		$query3 = "INSERT INTO 'Interactions' ('Interaction_Type_ID') VALUES (" . $interaction_type_id . ")";
        $insert_result = mysqli_query($cxn, $query3) or die("couldn't execute insert query for: " . $device_number . " with button: " . $_POST['button']);
        if ($insert_result === TRUE) {
          print "Success";
        }
        else {
          print "Fail";
        }
		
      ?>
    </body>
  </head>
</html>