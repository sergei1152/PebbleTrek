Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS Ready!');

  // Construct a dictionary
  var dict = {
    'NUMBER':'+1 23408230498'
  };

  // Send a string to Pebble
  Pebble.sendAppMessage(dict,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  
  // Reveiving messages from Pebble
  Pebble.addEventListener('appmessage',
    function(e) {
      console.log('Received message: ' + JSON.stringify(e.payload));
    }
  );
});

