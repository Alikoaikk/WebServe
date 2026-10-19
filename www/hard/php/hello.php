<!DOCTYPE html>
<html>
<head><meta charset="utf-8"><title>PHP CGI</title></head>
<body>
  <h1>Hello from PHP CGI</h1>
  <p>Method: <?php echo htmlspecialchars($_SERVER["REQUEST_METHOD"]); ?></p>
  <p>name = <?php echo htmlspecialchars(isset($_GET["name"]) ? $_GET["name"] : ""); ?></p>
  <p><a href="/">back home</a></p>
</body>
</html>
