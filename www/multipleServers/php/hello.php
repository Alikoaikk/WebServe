<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>PHP CGI</title>
<style>
  body { font-family: sans-serif; max-width: 760px; margin: 24px auto; padding: 0 16px; color: #222; line-height: 1.5; }
  h1 { color: #2c5282; }
  a { color: #2b6cb0; }
</style>
</head>
<body>
  <h1>Hello from PHP CGI</h1>
  <p>Method: <?php echo htmlspecialchars($_SERVER["REQUEST_METHOD"]); ?></p>
  <p>name = <?php echo htmlspecialchars(isset($_GET["name"]) ? $_GET["name"] : ""); ?></p>
  <p><a href="/">back home</a></p>
</body>
</html>
