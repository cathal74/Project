<!DOCTYPE html>
<html>
	<head>
		<style>
			table {
				border-collapse: collapse;
				width: 100%;
				color: #1f5380;
				font-family: monospace;
				font-size: 20px;
				text-align: left;
			} 
			th {
				background-color: #1f5380;
				color: white;
			}
			tr:nth-child(even) {background-color: #f2f2f2}
		</style>
	</head>
	<?php
		
		$hostname = "localhost";		
		$username = "root";		
		$password = "";	
		$dbname = "tempdata";
		// Create connection
		$conn = mysqli_connect($hostname, $username, $password, $dbname);
		// Check connection
		if (!$conn) {
			die("Connection failed !!!");
		} 
	?>
	<body>
		<table>
			<tr>
				<th>No</th> 
				<th>Temp</th> 
				<th>Date</th>
				<th>Time</th>
			</tr>	
			<?php
				$table = mysqli_query($conn, "SELECT No, Temp, Date, Time FROM temp_data_table"); 
				while($row = mysqli_fetch_array($table))
				{
			?>
			<tr>
				<td><?php echo $row["No"]; ?></td>
				<td><?php echo $row["Temp"]; ?></td>
				<td><?php echo $row["Date"]; ?></td>
				<td><?php echo $row["Time"]; ?></td>
			</tr>
			<?php
				}
			?>
		</table>
	</body>
</html>