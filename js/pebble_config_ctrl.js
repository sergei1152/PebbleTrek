$(function(){
	$('#name-field').val("Serge");
	$('#phone-field').val(6472889087);
	$('#countdown-field').val(15);
	var submitButton = $('#save-button');
	submitButton.on('click', function() {
		var config = {
			'emergency_contact_name': $('#name-field').val(),
			'emergency_contact_phone': $('#phone-field').val(),
			'countdown_duration': $('#countdown-field').val()
		}
		$('#save-button').attr('href','pebblejs://close#' + encodeURIComponent(JSON.stringify(config)));
		// Return data to watchapp
		location.href = 'pebblejs://close#' + encodeURIComponent(JSON.stringify(config));
	});
});
