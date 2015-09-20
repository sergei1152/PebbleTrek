// ---------------------------------LOCATION-------------------------
var locationOptions = {
  enableHighAccuracy: true, 
  maximumAge: 5000, 
  timeout: 10000
};

function getCurrentLocationGoogleMapsLink(callback){
  var lat;
  var long;
  
  navigator.geolocation.getCurrentPosition(
    function(pos){
      console.log('current location: lat= ' + pos.coords.latitude + ' lon= ' + pos.coords.longitude);
      lat = pos.coords.latitude;
      long = pos.coords.longitude;
      callback("http://www.google.com/maps/place/"+lat+","+long+"/@"+lat+","+long+",17z");
    },
    function(err){
      console.log('location error (' + err.code + '): ' + err.message);
      callback("[no link available]");
    },                               
    locationOptions
  );  
}

// ---------------------------------NOTIFICATION-------------------------
var endpoint = 'https://api.twilio.com/2010-04-01/Accounts/AC46c61d10daf88f73ce17ddd0c50ff327/Messages';

function sendSMSNotificationWithContent(message){
  var req = new XMLHttpRequest();
  req.open('POST', endpoint, false, 'AC46c61d10daf88f73ce17ddd0c50ff327', '4797e76a39c42c3694d6604fc9cb10d9');
  req.setRequestHeader('Authentication', 'Basic QUM0NmM2MWQxMGRhZjg4ZjczY2UxN2RkZDBjNTBmZjMyNzo0Nzk3ZTc2YTM5YzQyYzM2OTRkNjYwNGZjOWNiMTBkOQ==');
  req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  req.onreadystatechange = function() {
    if (req.readyState == 4) {
      console.log(req.responseText);
    }
  };

  var encodedMessage = "From=%2B16042435332&To=%2B16477719615&Body="+message;
  req.send(encodedMessage);
}