Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  Pebble.openURL('http://sergei1152.github.io/PebbleTrek/');
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log('Configuration window returned: ' + e.response);
});