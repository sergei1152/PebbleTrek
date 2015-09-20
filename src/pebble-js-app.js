function updateNumber(newValue){
  // Construct a dictionary
  var dict = {
    'NUMBER': newValue
  };
  
  Pebble.sendAppMessage(dict,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
}

Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS Ready!');
    
  updateNumber('tes');
  // Reveiving messages from Pebble
  Pebble.addEventListener('appmessage',
    function(e) {
      console.log('Received message: ' + JSON.stringify(e.payload));
      if(e.payload.NOTIFY == 1) {
        console.log('Sending SMS');
        // Request current position
        getCurrentLocationGoogleMapsLink(function(link){
          console.log(link);
          // Send Notification SMS
          sendSMSNotificationWithContent("Location: " + link);
        });
      }
    }
  );
  
  
});

