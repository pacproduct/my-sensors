var SerialCom = require('./modules/serialCom');

var comArduino = new SerialCom();

// When data received, display it.
comArduino.on('data', function(data) {
  console.log('Received: [' + data + ']');
});

// Error
comArduino.on('error', function(err) {
  console.log('Error received:');
  console.log(err);
});

// Connection.
comArduino.openConnection(function(err) {
  if (err) {
    console.log('Error received while opening connection:');
    console.log(err);
  }
  else {
    console.log('Serial connection now open.');
  }
});
