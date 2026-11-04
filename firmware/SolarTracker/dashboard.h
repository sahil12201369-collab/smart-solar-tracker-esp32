// Dashboard page served by the ESP32 (stored in flash).
// docs/demo.html is generated from this file by tools/make_demo.py - run it again after editing.
#pragma once
#include <Arduino.h>

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Solar Tracker</title>
<style>
body{font-family:sans-serif;background:#0f172a;color:#e2e8f0;margin:0;padding:16px;max-width:760px;margin:auto}
h2{margin:4px 0 12px}
.g{display:grid;grid-template-columns:repeat(2,1fr);gap:10px}
.c{background:#1e293b;border-radius:10px;padding:12px;font-size:14px;color:#94a3b8}
.c b{font-size:24px;display:block;color:#e2e8f0;margin-top:2px}
canvas{width:100%;height:140px;background:#1e293b;border-radius:10px;margin-top:10px}
button{padding:8px 14px;border:0;border-radius:6px;background:#38bdf8;margin-top:6px;font-weight:bold;cursor:pointer}
input{width:100%}
#st{font-size:13px;color:#94a3b8}
</style></head><body>
<h2>Solar Tracker Dashboard</h2>
<div id="st">connecting...</div>
<div class="g">
<div class="c">Power<b id="p">-</b></div>
<div class="c">Panel voltage<b id="v">-</b></div>
<div class="c">Current<b id="i">-</b></div>
<div class="c">Battery<b id="b">-</b></div>
<div class="c">LDR left<b id="l">-</b></div>
<div class="c">LDR right<b id="r">-</b></div>
<div class="c">Servo angle<b id="a">-</b></div>
<div class="c">Mode<b id="m">-</b></div>
</div>
<canvas id="cv" width="600" height="140"></canvas>
<p>Manual angle (switches to MANUAL):
<input type="range" min="20" max="160" value="90" onchange="fetch('/set?angle='+this.value)"></p>
<button onclick="fetch('/mode?auto=1')">Back to AUTO</button>
<script>
const $=id=>document.getElementById(id);
const f={p:['mW',0],v:['V',2],i:['mA',0],l:['',0],r:['',0],a:['deg',0]};
let h=[];
async function u(){
 try{
  let d=await(await fetch('/data')).json();
  for(let k in f)$(k).textContent=d[k].toFixed(f[k][1])+' '+f[k][0];
  $('b').textContent=d.b.toFixed(2)+' V ('+d.bp+'%)';
  $('m').textContent=d.auto?'AUTO':'MANUAL';
  $('st').textContent='live - updated '+new Date().toLocaleTimeString();
  h.push(d.p);if(h.length>60)h.shift();
  let c=$('cv'),x=c.getContext('2d'),mx=Math.max(50,...h);
  x.clearRect(0,0,600,140);x.strokeStyle='#38bdf8';x.lineWidth=2;x.beginPath();
  h.forEach((y,n)=>{let X=n*10,Y=130-y/mx*120;n?x.lineTo(X,Y):x.moveTo(X,Y)});
  x.stroke();
  x.fillStyle='#94a3b8';x.font='12px sans-serif';x.fillText('Power (mW), last 60 s, max '+mx.toFixed(0),8,14);
 }catch(e){$('st').textContent='offline - cannot reach the ESP32'}
}
setInterval(u,1000);u();
</script></body></html>
)rawliteral";
