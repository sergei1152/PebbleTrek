var submitButton = $('#save-button');
submitButton.addEventListener('click', function() {
	var config = {
		'emergency_contact_name': $('#name-field').val(),
		'emergency_contact_phone': $('#phone-field').val(),
		'countdown_duration': $('#countdown-field').val()
	}
	// Return data to watchapp
	location.href = 'pebblejs://close#' + encodeURIComponent(JSON.stringify(config));
});