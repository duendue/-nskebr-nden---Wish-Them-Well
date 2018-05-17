//Variabler til Serial server funktionen

var serial;
var options = {
    baudrate: 9600
};

var portName = '/COM11';

var xIn = 0;
var yIn = 0;


//-------------------------------------//

//Variabler til billeder, video og lyd

var img1;
var img2;

var vid1, vid2, vid3, vid4, vid5, vid6, vid7, vid8, vid9, vid10, vid11, vid12;

var sound1;
var sound2;
var sound3;
var sound4;
var sound5;
var sound6;
var sound7;

var music;


//-----------------------------------------//

//Variabler til de forskellige videoer

var idleMode = true;
var vid2Mode = false;
var vid3Mode = false;
var vid4Mode = false;
var vid5Mode = false;
var vid6Mode = false;


//---------------------------------------//

//Variabel der bestemmer om videoerne automatisk skal afspille den næste i rækken når de er slut

var autoplay = true;


//------------------------------------//

//Disse funktioner kører før programmet startes

function preload() {
    
    //Her bliver lydvariablerne tilknyttet deres lydfilsti

    var sound1 = new Howl({
        src: ['sounds/sound1.mp3']
    });

    var sound2 = new Howl({
        src: ['sounds/sound2.mp3']
    });

    var sound3 = new Howl({
        src: ['sounds/sound3.mp3']
    });

    var sound4 = new Howl({
        src: ['sounds/sound4.mp3']
    });

    var sound5 = new Howl({
        src: ['sounds/sound5.mp3']
    });

    var sound6 = new Howl({
        src: ['sounds/sound6.wav']
    });
    
    var sound7 = new Howl({
        src: ['sounds/sound7.mp3']
    });

    var music = new Howl({
        src: ['sounds/music.flac']
    });

    //Her bliver videovariablerne tilknyttet deres videofilsti

    vid1 = createVideo(['video/Brønd standby video_1.mp4']);
    vid1.size(innerWidth, innerHeight);

    vid2 = createVideo(['video/Well_after01_final01.mp4']);
    vid2.size(innerWidth, innerHeight);

    vid3 = createVideo(['video/Well_after02_final01.mp4']);
    vid3.size(innerWidth, innerHeight);

    vid4 = createVideo(['video/Well_before01_final01.mp4']);

    vid5 = createVideo(['video/Well_before02_final01.mp4']);
    vid5.size(innerWidth, innerHeight);

    vid7 = createVideo(['video/Well_after03_final01.mp4']);
    vid1.size(innerWidth, innerHeight);

    vid8 = createVideo(['video/Tak for din stotte_1.mp4']);
    vid2.size(innerWidth, innerHeight);

    vid10 = createVideo(['video/Donation.mp4']);
    vid3.size(innerWidth, innerHeight);
}


//------------------------------------//

//Disse funktioner kører i starten af programmet

function setup() {
    noCanvas();

    //------------------------------------//
    
    //Funktion der aktiverer serial serveren
    
    serial = new p5.SerialPort();
    serial.on('data', serialEvent);
    //  serial.on('error', serialError);
    serial.open(portName, options);
    
    //------------------------------------//
    
    //Her starter idlemodes musik og video
    
    music.play();
    music.loop();

    vid1.play();
    vid1.loop();

    //Her under bliver alle videoer der ikke vises skjult
    
    vid2.hide();
    vid3.hide();
    vid4.hide();
    vid5.hide();
    vid7.hide();
    vid8.hide();
    vid10.hide();

}


//------------------------------------//



function draw() {

}


//------------------------------------//

//serialEvent funktionen bliver kaldt hver gang der er kommunikation imellem P5 og Arduinoen. Inde i funktionen er lavet en switch funktion der bliver aktiveret når afstands / swipe sensoren eller knappen bliver brugt.

function serialEvent() {
    var inString = serial.readStringUntil('\r\n');

    console.log(inString);

    //Hvert case refererer til sensorens mulige udfald. I vores tilfælde bliver videokoden aktiveret uanset om det er et venstre eller højreswipe. 
    
    switch (inString) {
        case "Right Swipe":
            console.log("Right Swipe Confirmed");
            videoSwitch()
            break;

        case "Left Swipe":
            console.log("Left Swipe Confirmed");
            videoSwitch()
            break;

        case "Button Pressed!":
            console.log("Donation Complete");
            donation();
            break;
    }

    if (inString.length > 0) {
        var splitString = split(inString, "//");
        xIn = Number(splitString[0]);
        yIn = Number(splitString[1]);
    }
}


//------------------------------------//

//Donation funktionen beskriver hvad der skal ske når der doneres på brønden.

function donation() {
    
    //Alle lydfiler der kører stoppes
    sound1.stop();
    sound2.stop();
    sound3.stop();
    music.stop();
    
    //Donationslydene spilles
    sound4.play();
    sound7.play();

    //Alle videoer stoppes og skjules
    vid1.hide();
    vid2.hide();
    vid2.stop();
    vid3.hide();
    vid3.stop();
    vid4.hide();
    vid4.stop();
    vid5.hide();
    vid5.stop();
    vid7.hide();
    vid7.stop();
    vid10.hide();

    //Donationsvideoen bliver vist, starter og bliver tilpasset til kanvasset.
    vid8.show();
    vid8.play();
    vid8.size(innerWidth, innerHeight);
    
    //Når donationsvideoen slutter bliver idleMode aktiveret
    vid8.onended(idleModeNow);
}


//------------------------------------//

//videoSwitch funktionen bruges til at navigere imellem de forskellige videoer. Den tjekker boolean variablen om hvilken videoMode der er sat til true, og kører så den pågælende kode.

function videoSwitch() {
    if (idleMode) {
        music.stop();
        sound2.play();
        sound6.play();
        video2Load();
        console.log("Playing video_1")
    }
    if (vid2Mode) {
        sound6.play();
        video3Load();
        console.log("Playing video_2")
    }
    if (vid3Mode) {
        sound6.play();
        video4Load();
        console.log("Playing video_3")
    }
    if (vid4Mode) {
        sound6.play();
        video5Load();
        console.log("Playing video_4")
    }
    if (vid5Mode) {
        sound6.play();
        video6Load();
        console.log("Playing video_5")
    }
    if (vid6Mode) {
        sound6.play();
        console.log("Playing video_6")
        video7Load();
    }
}


//------------------------------------//


function idleLights(){
    serial.write(5);
}


//------------------------------------//

//Denne funktion stopper alle andre videoer, og sætter programmet til at køre idleMode

function idleModeNow() {
    
    music.play();
    music.loop();
    
    vid1.show();
    vid1.loop();
    
    vid8.hide();
    
    idleMode = true;
    vid2Mode = false;
    vid3Mode = false;
    vid4Mode = false;
    vid5Mode = false;
    vid6Mode = false;
    
    serial.write(0);
}


//------------------------------------//

//Århus_fremmad introduktion

//Well_Before_01
function video2Load() {
    idleMode = false;
    vid1.hide();

    vid4.show();
    vid4.size(innerWidth, innerHeight);
    vid4.play();
    
    setTimeout(idleLights, 4000);
    setTimeout(video3Preload, 30000);

    if (autoplay) {
        vid4.onended(video3Load);
    }
}


//------------------------------------//

//videoPreload 

function video3Preload() {
    console.log("Video_1 ended");
    serial.write(4);
    vid2Mode = true
}


//Well_Before_02
function video3Load() {
    vid2Mode = false;
    vid4.hide();
    vid4.stop();

    vid5.show();
    vid5.size(innerWidth, innerHeight);
    vid5.play();
    
    setTimeout(idleLights, 4000);
    setTimeout(video4Preload, 10000);

    if (autoplay) {
        vid5.onended(video4Load);
    }
}


//------------------------------------//


function video4Preload() {
    console.log("Video_2 ended");
    serial.write(4);
    vid3Mode = true;
}


//Well_After_01
function video4Load() {
    vid3Mode = false;
    vid5.hide();
    vid5.stop();

    vid2.show();
    vid2.size(innerWidth, innerHeight);
    vid2.play();
    
    setTimeout(idleLights, 4000);
    setTimeout(video5Preload, 12000);

    if (autoplay) {
        vid2.onended(video5Load);
    }
}


//------------------------------------//


function video5Preload() {
    console.log("Video_3 ended");
    serial.write(4);
    vid4Mode = true;
}

//Well_After_02
function video5Load() {
    vid4Mode = false;
    vid2.hide();
    vid2.stop();

    vid3.show();
    vid3.size(innerWidth, innerHeight);
    vid3.play();
    
    setTimeout(idleLights, 4000);  
    setTimeout(video6Preload, 6000);

    if (autoplay) {
        vid3.onended(video6Load);
    }
}


//------------------------------------//


function video6Preload() {
    console.log("Video_4 ended");
    serial.write(4);
    vid5Mode = true;
}

//Well_After_03
function video6Load() {
    sound2.stop();
    sound1.play();

    vid5Mode = false;
    vid3.hide();
    vid3.stop();

    vid7.show();
    vid7.size(innerWidth, innerHeight);
    vid7.play();
    
    setTimeout(idleLights, 4000); 
    vid7.onended(video7Load);
}


//------------------------------------//


function video7Load() {
    sound1.stop();
    music.play();

    vid7.hide();
    vid7.stop();

    vid10.show();
    vid10.size(innerWidth, innerHeight);
    vid10.play();
    vid10.loop();

    setTimeout(idleModeNow, 90000);
}