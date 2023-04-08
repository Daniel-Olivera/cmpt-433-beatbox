"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io;

var dgram = require('dgram');
var fs = require('fs');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');

	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
	// Pased string of command to relay
	socket.on('serverCommand', function(data) {
		console.log('serverCommand command: ' + data);

		// Info for connecting to the local process via UDP
		var PORT = 12345;
		var HOST = '127.0.0.1';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		//start timer when a udp packet is sent
        var errorTimer = setTimeout(function() {
            console.log("BBG Timed out");
            socket.emit("noResponse", "");
        }, 5000);

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {

			//If we receive a response, end the timer
            clearTimeout(errorTimer);

			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')
            if(reply.includes("None") || reply.includes("Rock 1") || reply.includes("Rock 2")){
			    socket.emit('beatReply', reply);
            }
        
			//If the packet sent had the word volume in it,
			//then the reply is about the volume, so update it
            if(data.includes("Volume")){
                socket.emit('volumeReply', reply);
            }

			//If the packet sent had the word tempo in it,
			//then the reply is about the tempo, so update it
            if(data.includes("Tempo")){
                socket.emit('tempoReply', reply);
            }
            
			//Likewise with the sounds
			//Though, these don't update anything, they just play the sound
            if(reply.includes("hihat") || reply.includes("snare") || reply.includes("bass")){
			    socket.emit('playedSoundReply', reply);
            }


			client.close();

		});
		client.on("UDP Client: close", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
	});

	//interval set to 1 second to update the device uptime on the server
    setInterval(function() {updateTime(socket)}, 1000);

	setInterval(function() {socket.emit('getBeat')}, 1000);
	setInterval(function() {socket.emit('getVolume')}, 1000);
	setInterval(function() {socket.emit('getTempo')}, 1000);

};


//Reads the /proc/uptime file and converts the value into
//hours, minutes, and seconds to display on the website
function updateTime(socket) {
	//change device uptime on right box
    var buffer = fs.readFileSync("/proc/uptime");
    const outputAsString = buffer.toString();
    const fileOutput = outputAsString.split(" ");

    const totalMinutes = Math.floor(fileOutput[0] / 60);
    
    var seconds = Math.floor(fileOutput[0] % 60);
    var hours = Math.floor(totalMinutes / 60);
    var minutes = totalMinutes % 60;

    var output = hours.toString() + ":" + minutes.toString() + ":" + seconds.toString() + "(H:M:S)";

    socket.emit('updateTime', output);
}
