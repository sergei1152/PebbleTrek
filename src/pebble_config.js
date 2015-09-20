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
    'KEY_EMERGENCY_CONTACT_NAME': config_data[emergency_contact_name],
    'KEY_EMERGENCY_CONTACT_PHONE': config_data[emergency_contact_phone],
    'KEY_COUNTDOWN_DURATION': config_data[countdown_duration]
  };

  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function(){
    console.log('Sent config data to Pebble');  
  }, function() {
    console.log('Failed to send config data!');
  });
});
