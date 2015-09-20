// function updateNumber(newValue){
//   // Construct a dictionary
//   var dict = {
//     'NUMBER': newValue
//   };
  
//   Pebble.sendAppMessage(dict,
//     function(e) {
//       console.log('Send successful.');
//     },
//     function(e) {
//       console.log('Send failed!');
//     }
//   );
// }

Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  Pebble.openURL('http://sergei1152.github.io/PebbleTrek/');
});






Pebble.addEventListener('webviewclosed', function(e) {
  // Decode and parse config data as JSON
  var config_data = JSON.parse(decodeURIComponent(e.response));
  console.log('Config window returned: ', JSON.stringify(config_data));

  // Prepare AppMessage payload
  var dict = {
    'NAME': config_data[emergency_contact_name],
    'NUMBER': config_data[emergency_contact_phone],
    'COUNTDOWN_DURATION': config_data[countdown_duration]
  };

  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function(){
    console.log('Sent config data to Pebble');  
  }, function() {
    console.log('Failed to send config data!');
  });
});

Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS Ready!');
    
  // updateNumber('tes');
  // Receiving messages from Pebble
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

