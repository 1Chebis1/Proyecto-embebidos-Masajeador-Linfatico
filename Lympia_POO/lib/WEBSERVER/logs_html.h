#pragma once

static const char LOGS_HTML[] = R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Lymflow &mdash; Logs</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Cormorant+Garamond:ital,wght@0,300;0,400;1,300&family=Playfair+Display:wght@400;600&display=swap" rel="stylesheet">
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{background:#EDE9E2;min-height:100vh;padding:2rem 2.5rem;font-family:'Cormorant Garamond',Georgia,serif;color:#3a3530}
h1{font-family:'Playfair Display',Georgia,serif;font-size:2rem;font-weight:600;letter-spacing:.04em;color:#2d2a26}
.subtitle{font-size:.9rem;font-weight:300;font-style:italic;color:#7a746c;margin-top:.2rem}
hr{border:none;border-top:1px solid #c4bdb4;margin:1.25rem 0 2rem}
header{display:flex;justify-content:space-between;align-items:flex-start}
.nav-btn{font-family:'Cormorant Garamond',Georgia,serif;font-size:.8rem;font-weight:300;color:#9a9288;border:1px solid #c4bdb4;border-radius:6px;padding:.3rem .7rem;text-decoration:none;letter-spacing:.04em;display:inline-block;margin-top:.2rem}
.nav-btn:hover{color:#3a3530;border-color:#9a9288}
.badge{font-size:.63rem;font-weight:300;letter-spacing:.07em;text-transform:uppercase;padding:.12rem .4rem;border-radius:4px;border:1px solid;white-space:nowrap;text-align:center}
.badge-INFO{color:#7a746c;border-color:#c4bdb4}
.badge-WARN{color:#b45309;border-color:#f59e0b;background:rgba(245,158,11,.08)}
.badge-ERROR{color:#be123c;border-color:#fb7185;background:rgba(251,113,133,.1)}
.columns{display:flex;gap:1rem;align-items:flex-start}
.col{flex:1;min-width:0;display:flex;flex-direction:column;align-items:center;gap:.55rem}
.col-card{width:100%;background:rgba(255,255,255,.42);border:1px solid #d6d0c8;border-radius:10px;padding:.7rem .8rem;overflow-y:auto;height:70vh;display:flex;flex-direction:column;gap:.3rem}
.col-entry{padding:.45rem .5rem;border-bottom:1px solid rgba(196,189,180,.4);display:flex;flex-direction:column;gap:.15rem}
.col-entry:last-child{border-bottom:none}
.col-ts{font-size:.7rem;font-weight:300;color:#9a9288;font-variant-numeric:tabular-nums;letter-spacing:.02em;white-space:nowrap}
.col-msg{font-size:.85rem;font-weight:300;color:#2d2a26;word-break:break-word;line-height:1.35}
.col-empty{color:#9a9288;font-size:.85rem;font-style:italic;text-align:center;padding:.6rem .5rem}
</style>
</head>
<body>
<header>
  <div>
    <h1>Lymflow</h1>
    <p class="subtitle">Registro del sistema &middot; actualizando cada segundo</p>
  </div>
  <a href="/dashboard" class="nav-btn">&#8592; Dashboard</a>
</header>
<hr>
<div class="columns">
  <div class="col">
    <span class="badge badge-WARN">WARNINGS</span>
    <div class="col-card" id="colWarn"><p class="col-empty">Sin entradas&hellip;</p></div>
  </div>
  <div class="col">
    <span class="badge badge-ERROR">ERRORS</span>
    <div class="col-card" id="colError"><p class="col-empty">Sin entradas&hellip;</p></div>
  </div>
  <div class="col">
    <span class="badge badge-INFO">INFO</span>
    <div class="col-card" id="colInfo"><p class="col-empty">Sin entradas&hellip;</p></div>
  </div>
</div>
<script>
function fmtTs(ms){
  var tot=Math.floor(ms/1000),m=Math.floor(tot/60),s=tot%60,msRem=ms%1000;
  return'T+'+String(m).padStart(2,'0')+':'+String(s).padStart(2,'0')+'.'+String(msRem).padStart(3,'0');
}
function esc(s){return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');}
function rows(arr){
  if(!arr.length) return'<p class="col-empty">Sin entradas&hellip;</p>';
  return arr.map(function(e){
    return'<div class="col-entry">'+
      '<span class="col-ts">'+fmtTs(e.ms)+'</span>'+
      '<span class="col-msg">'+esc(e.msg)+'</span>'+
    '</div>';
  }).join('');
}
function render(data){
  var warns=[],errors=[],infos=[];
  if(data&&data.length){
    data.forEach(function(e){
      if(e.level==='WARN') warns.push(e);
      else if(e.level==='ERROR') errors.push(e);
      else infos.push(e);
    });
  }
  document.getElementById('colWarn').innerHTML=rows(warns);
  document.getElementById('colError').innerHTML=rows(errors);
  document.getElementById('colInfo').innerHTML=rows(infos);
}
function poll(){
  fetch('/logs-data').then(function(r){return r.ok?r.json():[];}).then(render).catch(function(){}).finally(function(){setTimeout(poll,1000);});
}
poll();
</script>
</body>
</html>)HTML";
