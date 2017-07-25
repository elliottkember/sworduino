const $ = (t) => document.querySelector(t);
var ip = '';
var ws;
var brightness;
var pattern;
var speed;
var wifi;

for (let i = 0; i < 10; i++) {
  $(`#button${i}`).addEventListener('click', function(){
    playPattern(i);
  });
}
$('#sign-selector').addEventListener('change', (e) => connect(ip = e.target.value));
$('#speed').addEventListener('input', function(){ setSpeed(-parseInt($('#speed').value)) });
$('#brightness').addEventListener('input', function(){ setBrightness($('#brightness').value) });
$('#brightnessLow').addEventListener('click', function(){ setBrightness(40) });
$('#brightnessMedium').addEventListener('click', function(){ setBrightness(180) });
$('#brightnessHigh').addEventListener('click', function(){ setBrightness(255) });

const connect = (ip) => {
  ws = new WebSocket(`ws://${ip}:81`);
  var lastReadyState = 0;
  console.log('Websocket: connecting...');
  document.querySelector('#light').className = 'connecting';

  ws.onmessage = (event) => {
    console.log('Websocket [IN]:', event.data);
    data = JSON.parse(event.data);
    let shouldRender = false;
    if (speed !== data.speed || brightness !== data.brightness || pattern !== data.pattern) {
      shouldRender = true;
    }
    speed = data.speed;
    brightness = data.brightness;
    pattern = data.pattern;
    if (shouldRender) render();
  }

  ws.onopen = () => {
    console.log('Websocket: connected');

    ws.send("status");

    document.querySelector('#light').className = 'connected';
    window.ws = ws;

    setInterval(function(){
      if (ws.readyState !== lastReadyState) {
        console.log('Websocket: status:', ws.readyState);
        if (ws.readyState === WebSocket.OPEN) {
          document.querySelector('#light').className = 'connected';
        } else {
          document.querySelector('#light').className = 'disconnected';
        }
      }

      lastReadyState = ws.readyState;
    }, 1000);
  };
}

const disconnect = () => {
  ws.close();
}

if (window.location.host === 'ueno-sign.getforge.io') {
  ip = '10.15.1.129';
  wifi = 'UENO';
} else if (window.location.host === 'socket.dev') {
  ip = '10.0.1.22';
  wifi = 'Thou BBC';
}

$('#sign-selector').value = ip;
$('#wifi').innerText = wifi;

setInterval(() => {
  if (ws.readyState !== WebSocket.OPEN) return;
  ws.send("status");
}, 5000)

const render = _.debounce(() => {
  if (ws.readyState !== WebSocket.OPEN) return;
  $("#brightness").value = brightness;
  $('#sign-selector').value = ip;
  $('#speed').value = -speed;
  $('#wifi').innerText = wifi;
  const message = JSON.stringify({
    pattern,
    brightness,
    speed,
   });
   console.log('Websocket [OUT]:', message);
  ws.send(message);
}, 100);

playPattern = (number) => {
  pattern = number;
  render();
}
setBrightness = (number) => {
  brightness = number;
  render();
}
setSpeed = (number) => {
  speed = number;
  render();
}

connect(ip);
