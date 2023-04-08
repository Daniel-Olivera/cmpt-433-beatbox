"use strict";

var socket = io.connect();
// Run when webpage fully loaded
$(document).ready(function() {

    //Hide error box by default
    $("#errorBox").hide();

//=========================================================//
//                        Buttons                          //
//=========================================================//

	$('#noBeatBtn').click(function() {
		console.log("noBeatBtn clicked!");
        sendCommandViaUDP("none");
	});

    $('#rockOneBtn').click(function() {
		console.log("rockOneBtn clicked!");
        sendCommandViaUDP("rock1");
	});

    $('#rockTwoBtn').click(function() {
		console.log("rockTwoBtn clicked!");
        sendCommandViaUDP("rock2");
	});

    $('#decreaseVolumeBtn').click(function() {
		console.log("decreaseVolumeBtn clicked!");
        sendCommandViaUDP("decreaseVolume");
	});

    $('#increaseVolumeBtn').click(function() {
		console.log("increaseVolumeBtn clicked!");
        sendCommandViaUDP("increaseVolume");
	});

    $('#decreaseTempoBtn').click(function() {
		console.log("decreaseTempoBtn clicked!");
        sendCommandViaUDP("decreaseTempo");
	});

    $('#increaseTempoBtn').click(function() {
		console.log("increaseTempoBtn clicked!");
        sendCommandViaUDP("increaseTempo");
	});

    $('#hihatBtn').click(function() {
		console.log("hihatBtn clicked!");
        sendCommandViaUDP("hihat");
	});

    $('#snareBtn').click(function() {
		console.log("snareBtn clicked!");
        sendCommandViaUDP("snare");
	});

    $('#bassBtn').click(function() {
		console.log("bassBtn clicked!");
        sendCommandViaUDP("bass");
	});

    $('#terminateBtn').click(function() {
		console.log("terminateBtn clicked!");
        sendCommandViaUDP("shutdown");
	});

//=========================================================//
//                        Replies                          //
//=========================================================//

    socket.on('beatReply', function(result) {
        $("#errorBox").hide();
		updateBeatName(result);
	});

    socket.on('volumeReply', function(result) {
        $("#errorBox").hide();
		updateVolume(result);
	});

    socket.on('tempoReply', function(result) {
        $("#errorBox").hide();
		updateTempo(result);
	});

    socket.on('updateTime', function(result) {
        $('#deviceUptime').text(result);
    });

    socket.on("noResponse", function() {
        $("#errorBox").show();
    });

    socket.on("playedSoundReply", function() {
        console.log("Sound was played");
        $("#errorBox").hide();
    });

    socket.on("getTempo", function() {
        console.log("sending getTempo via UDP")
        sendCommandViaUDP('getTempo')
    });

    socket.on("getVolume", function() {
        console.log("sending getVolume via UDP")
        sendCommandViaUDP('getVolume')
    });

    socket.on("getBeat", function() {
        console.log("sending getBeat via UDP")
        sendCommandViaUDP('getBeat')
    });

});

//=========================================================//
//                        Helpers                          //
//=========================================================//

function sendCommandViaUDP(message) {
	socket.emit('serverCommand', message);
};

function updateBeatName(input) {
    $('#beatName').text(input);
}

function updateVolume(input) {
    $('#volumeTxt').val(input);
}

function updateTempo(input) {
    $('#tempoTxt').val(input);
}