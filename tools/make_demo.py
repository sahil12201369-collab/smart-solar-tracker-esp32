#!/usr/bin/env python3
"""Generate docs/demo.html from firmware/SolarTracker/dashboard.h.

The demo is the exact dashboard page, plus a small script that fakes the ESP32
(/data, /set, /mode) so it can be viewed on GitHub Pages without hardware.
Run again after editing dashboard.h:   python tools/make_demo.py
"""
import re
from pathlib import Path

root = Path(__file__).resolve().parent.parent
src = (root / "firmware" / "SolarTracker" / "dashboard.h").read_text(encoding="utf-8")
m = re.search(r'R"rawliteral\((.*?)\)rawliteral"', src, re.S)
if not m:
    raise SystemExit("could not find the PAGE raw string in dashboard.h")
html = m.group(1).strip()

shim = """
<div style="background:#facc15;color:#1e293b;padding:6px 10px;border-radius:6px;margin-bottom:10px;font:13px sans-serif">
DEMO MODE - simulated data. The real dashboard is served by the ESP32 with live sensor values.</div>
<script>
(function(){
  var angle=90, auto=true, t=0;
  function sim(){
    t+=1;
    var sun=90+50*Math.sin(t/20);
    if(auto){var d=sun-angle; if(Math.abs(d)>4) angle+=d>0?2:-2;}
    var err=Math.abs(sun-angle);
    var base=2200, diff=(sun-angle)*18;
    var l=Math.max(0,Math.min(4095,Math.round(base+diff+(Math.random()*40-20))));
    var r=Math.max(0,Math.min(4095,Math.round(base-diff+(Math.random()*40-20))));
    var f=Math.max(0,Math.cos(err*Math.PI/180));
    var v=5.4+0.3*f, i=240*f*f+Math.random()*4;
    return {p:v*i,v:v,i:i,b:3.95,bp:75,l:l,r:r,a:Math.round(angle),auto:auto};
  }
  var realFetch=window.fetch;
  window.fetch=function(url){
    url=String(url);
    if(url.indexOf('/data')===0) return Promise.resolve({json:function(){return Promise.resolve(sim());}});
    if(url.indexOf('/set')===0){auto=false; var m=url.match(/angle=(\\d+)/); if(m) angle=+m[1];}
    if(url.indexOf('/mode')===0){auto=true;}
    return Promise.resolve({ok:true,text:function(){return Promise.resolve('ok');}});
  };
})();
</script>
"""
html = html.replace("<h2>", shim + "<h2>", 1)
out = root / "docs" / "demo.html"
out.parent.mkdir(exist_ok=True)
out.write_text(html + "\n", encoding="utf-8")
print("wrote", out)
