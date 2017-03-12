// Element refs
const video = document.querySelector('#video');
const mirror = document.querySelector('#mirror');
const screen = document.querySelector('#screen');
const sample = document.querySelector('#sample');
const giphyId = document.querySelector('#giphyId');
const giphyBtn = document.querySelector('#setGiphy');
const speed = document.querySelector('#speed');

// Contexts
const screenContext = screen.getContext('2d');
const sampleContext = sample.getContext('2d');

// LED dimensions
const WIDTH = 8;
const HEIGHT = 38;
const LEDS = 300;

// Set sample dimensions
sample.width = WIDTH;
sample.height = HEIGHT;

// Set mirror width restrictions
mirror.style.width = `${WIDTH * 10}px`;

for (let i = 0; i < (WIDTH * HEIGHT); i++) {
  // Create <b/> tag for every LED
  var b = document.createElement('b');
  mirror.appendChild(b);
}

// Mirror screen canvas to sample canvas
// and changing aspect ratio to fit.
// TODO: ability to select region of the source destination to mirror.
const mirrorScreenToSample = () => {
  sampleContext.drawImage(screen, 0, 0, WIDTH, HEIGHT);
  for (let i = 0, y = 0; y < HEIGHT; y++) {
    for (let x = 0; x < WIDTH; x++) {
      mirror.children[i++].style.backgroundColor = `rgba(${sampleContext.getImageData(x, y, 1, 1).data.join(', ')})`;
    }
  }
}

// Dump video frames to screen canvas rAF.
let foo = 0;
const videoFrameToScreen = () => {
  if (video.paused || video.ended) return;
  screenContext.drawImage(video, 0, 0, video.videoWidth, video.videoHeight);
  if (foo++ === 1) {
//    screenContext.translate(screen.width * 0.05, screen.height * -0.2);
//    screenContext.rotate(25 * Math.PI /180);
//    screenContext.scale(1.25, 1.25);

  }
  mirrorScreenToSample();
  requestAnimationFrame(videoFrameToScreen);
}

video.addEventListener('play', videoFrameToScreen);
video.addEventListener('loadedmetadata', () => {
  // Set screen width and height
  screen.width = video.videoWidth;
  screen.height = video.videoHeight;
});

let ws, intv;

// Write canvas to ESP controller
const writeSample = () => {
  if (!ws || ws.readyState !== 1) return;

  const buffer = new ArrayBuffer(5);
  const dv = new DataView(buffer);
  let i = 0;
  for (let y = 0; y < HEIGHT; y++) {
    for (let x = 0; x < WIDTH; x++) {
      if (i >= LEDS) return;
      const d = sampleContext.getImageData(x, y, 1, 1).data;
      const rgb = d.slice(0, 3).map(n => Math.max(0, Math.min(255, n)));
      dv.setUint16(0, i++);
      dv.setUint8(2, rgb[0]);
      dv.setUint8(3, rgb[1]);
      dv.setUint8(4, rgb[2]);
      ws.send(dv.buffer);
    }
  }
}

const connect = () => {
  ws = new WebSocket(`ws://10.0.1.20:81`);
  console.log('connecting...');
  ws.onopen = () => {
    console.log('connected');
  //  intv = setInterval(() => writeSample(), 500); // (1000 / 30));
    // writeSample();
  };
}

const disconnect = () => {
  if (ws && ws.readyState === 1) ws.close();
  if (intv) clearInterval(intv);
  console.log('disconnected');
}

var timer;

document.querySelector('#connect').addEventListener('click', connect);
document.querySelector('#send').addEventListener('click', writeSample);
document.querySelector('#disconnect').addEventListener('click', disconnect);
document.querySelector('#play').addEventListener('click', () => {
  if (timer) {
    clearInterval(timer);
    timer = null;
  } else {
    timer = setInterval(writeSample, (1000 / 30));
  }
});

function handleVideo(stream) {
  video.src = window.URL.createObjectURL(stream);
}

function videoError() {
  // do something
}

if (navigator.getUserMedia) {
  // navigator.getUserMedia({ video: true }, handleVideo, videoError);
}

giphyBtn.addEventListener('click', () => {
  const id = giphyId.value;
  video.src = 'https://media.giphy.com/media/' + id + '/giphy.mp4';
  video.playbackRate = Number(speed.value);
  video.play();
});

speed.addEventListener('change', () => {
  video.playbackRate = Number(speed.value);
})
