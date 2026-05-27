#pragma once

static const char INDEX_HTML[] = R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Lymphia</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Cormorant+Garamond:ital,wght@0,300;0,400;1,300&family=Playfair+Display:wght@400;600&display=swap" rel="stylesheet">
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{background:#EDE9E2;min-height:100vh;padding:2rem 2.5rem;font-family:'Cormorant Garamond',Georgia,serif;color:#3a3530}
h1{font-family:'Playfair Display',Georgia,serif;font-size:2rem;font-weight:600;letter-spacing:.04em;color:#2d2a26}
.subtitle{font-size:.9rem;font-weight:300;font-style:italic;color:#7a746c;margin-top:.2rem}
hr{border:none;border-top:1px solid #c4bdb4;margin:1.25rem 0 2rem}
.main{display:flex;flex-direction:column;align-items:center;gap:2rem}
.visual{position:relative;width:260px;height:260px}
.visual svg{width:100%;height:100%}
.center-label{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);text-align:center;pointer-events:none}
.pct-value{font-family:'Playfair Display',Georgia,serif;font-size:2.2rem;font-weight:400;color:#2d2a26;line-height:1}
.estado-label{font-size:.78rem;font-weight:300;color:#7a746c;letter-spacing:.06em;margin-top:.25rem}
.cards{display:flex;gap:1rem;width:100%;max-width:420px}
.card{flex:1;background:rgba(255,255,255,.42);border:1px solid #d6d0c8;border-radius:10px;padding:1rem 1.2rem}
.card-title{font-size:.7rem;font-weight:300;color:#9a9288;letter-spacing:.07em;text-transform:uppercase;margin-bottom:.4rem}
.card-value{font-family:'Playfair Display',Georgia,serif;font-size:1.55rem;font-weight:400;color:#2d2a26}
.card-unit{font-size:.85rem;color:#9a9288}
.zone-wrap{width:100%;max-width:420px}
.zone-title{font-size:.7rem;font-weight:300;color:#9a9288;letter-spacing:.07em;text-transform:uppercase;margin-bottom:.5rem}
.zone-bar{position:relative;height:8px;border-radius:4px;background:linear-gradient(to right,#f59e0b 0%,#f59e0b 30%,#22c55e 30%,#22c55e 80%,#fb7185 80%,#fb7185 100%)}
.zone-dot{position:absolute;top:50%;transform:translate(-50%,-50%);width:14px;height:14px;background:#2d2a26;border:2.5px solid #EDE9E2;border-radius:50%;transition:left .18s ease}
.zone-labels{display:flex;justify-content:space-between;margin-top:.35rem;font-size:.68rem;color:#9a9288;font-weight:300}
.logs-btn{font-family:'Cormorant Garamond',Georgia,serif;font-size:.8rem;font-weight:300;color:#9a9288;border:1px solid #c4bdb4;border-radius:6px;padding:.3rem .7rem;text-decoration:none;letter-spacing:.04em;display:inline-block;margin-top:.2rem}
.logs-btn:hover{color:#3a3530;border-color:#9a9288}
</style>
</head>
<body>
<header style="display:flex;justify-content:space-between;align-items:flex-start">
  <div>
    <h1>Lymphia</h1>
    <p class="subtitle">Conexi&oacute;n ESP confirmada &middot; actualizando cada 200ms</p>
  </div>
  <a href="/logs" class="logs-btn">Logs</a>
</header>
<hr>
<div class="main">
  <div class="visual">
    <svg viewBox="0 0 260 260" fill="none" xmlns="http://www.w3.org/2000/svg">
      <ellipse id="r1" cx="130" cy="130" rx="40"  ry="37"  stroke="#5c5448" stroke-width="8"   fill="none" opacity="0"/>
      <ellipse id="r2" cx="130" cy="130" rx="66"  ry="62"  stroke="#5c5448" stroke-width="4.5" fill="none" opacity="0" transform="rotate(-1,130,130)"/>
      <ellipse id="r3" cx="130" cy="130" rx="93"  ry="86"  stroke="#5c5448" stroke-width="2.5" fill="none" opacity="0" transform="rotate(2,130,130)"/>
      <ellipse id="r4" cx="130" cy="130" rx="119" ry="108" stroke="#5c5448" stroke-width="1.5" fill="none" opacity="0" transform="rotate(-3,130,130)"/>
    </svg>
    <div class="center-label">
      <div class="pct-value"   id="pctVal">--</div>
      <div class="estado-label" id="estadoVal">&#x2014;</div>
    </div>
  </div>
  <div class="cards">
    <div class="card">
      <div class="card-title">Tiempo en zona &oacute;ptima</div>
      <div class="card-value" id="tiempoVal">--:--</div>
    </div>
    <div class="card">
      <div class="card-title">Estabilidad</div>
      <div class="card-value" id="estabilidadVal">--</div>
    </div>
  </div>
  <div class="zone-wrap">
    <div class="zone-title">Zona de presi&oacute;n</div>
    <div class="zone-bar">
      <div class="zone-dot" id="zoneDot" style="left:0%"></div>
    </div>
    <div class="zone-labels">
      <span>0%</span><span>30%</span><span>80%</span><span>100%</span>
    </div>
  </div>
</div>
<script>
var r1=document.getElementById('r1'),r2=document.getElementById('r2'),r3=document.getElementById('r3'),r4=document.getElementById('r4');
var pctVal=document.getElementById('pctVal'),estadoVal=document.getElementById('estadoVal');
var tiempoVal=document.getElementById('tiempoVal'),estabilidadVal=document.getElementById('estabilidadVal'),zoneDot=document.getElementById('zoneDot');
function applyRings(e){
  var ins=(e==='insuficiente'),opt=(e==='óptimo'),exc=(e==='excesivo');
  r1.setAttribute('opacity',(ins||opt||exc)?'1':'0');
  r2.setAttribute('opacity',(opt||exc)?'1':'0');
  r3.setAttribute('opacity',(opt||exc)?'1':'0');
  r4.setAttribute('opacity',exc?'1':'0');
}
function update(d){
  var p=parseFloat(d.pct);
  pctVal.textContent=p.toFixed(1)+'%';
  estadoVal.textContent=d.estado;
  tiempoVal.textContent=String(d.minutos).padStart(2,'0')+':'+String(d.segundos).padStart(2,'0');
  estabilidadVal.textContent=d.estabilidad;
  zoneDot.style.left=Math.min(100,Math.max(0,p))+'%';
  applyRings(d.estado);
}
function poll(){
  fetch('/data').then(function(r){return r.ok?r.json():null;}).then(function(d){if(d)update(d);}).catch(function(){}).finally(function(){setTimeout(poll,200);});
}
poll();
</script>
</body>
</html>)HTML";
