<?php
require_once('config.php');

// Connect to the MySQL database
$conn = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);

// Check for errors
if (!$conn) {
    die('Failed to connect to MySQL: ' . mysqli_connect_error());
}

// Check if the user submitted the form
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = $_POST['name'];
    $email = $_POST['email'];

    // Insert the data into the database
    $sql = "INSERT INTO users (name, email) VALUES ('$name', '$email')";
    if (mysqli_query($conn, $sql)) {
        echo 'User added successfully.';
    } else {
        echo 'Error adding user: ' . mysqli_error($conn);
    }
}

// Query the database for all users
$sql = 'SELECT * FROM users';
$result = mysqli_query($conn, $sql);

// Check for errors
if (!$result) {
    die('Error retrieving users: ' . mysqli_error($conn));
}

// Display the users in an HTML table
echo '<h1>Users</h1>';
echo '<table>';
echo '<tr><th>Name</th><th>Email</th></tr>';
while ($row = mysqli_fetch_assoc($result)) {
    echo '<tr><td>' . $row['name'] . '</td><td>' . $row['email'] . '</td></tr>';
}
echo '</table>';

// Close the database connection
mysqli_close($conn);
?>
