#ifndef MONITOR_H
#define MONITOR_H

#include <Ethernet2.h>
#include "PaginaHTML.h"

inline void paginaMonitor(EthernetClient &client) {
    String contenido = R"rawliteral(
<h1>Monitor de entradas</h1>

<p>Actualización automática cada 10 segundos.</p>
<p id="estadoMonitor">Conectando...</p>
<div class="progress-container">
			<div id="progressBar" class="progress-bar" style="width: 100%;">Siguiente actualización en: 10 segundos</div>
		</div>
<div style="overflow-x:auto;">
  <table>
    <thead>
      <tr>
        <th>Entrada física</th>
        <th>Asignación</th>
        <th>Valor de entrada</th>
        <th>Nombre</th>
        <th>Tipo</th>
        <th>Offset</th>
        <th>Slope</th>
        <th>Unidades</th>
        <th>Valor real</th>
      </tr>
    </thead>
    <tbody id="tablaEntradas">
      <tr><td colspan="9">Cargando entradas...</td></tr>
    </tbody>
  </table>
</div>
)rawliteral";

    enviarHTML(client, "Monitor", contenido);

client.println(R"rawliteral(
<script>
let progress = 100;
const progressBar = document.getElementById('progressBar');
  async function actualizarEntradas() {
    progress -= 10;
    if (progress < 0) {
    progress = 100;
    progressBar.textContent = 'Actualizando...';
	  progressBar.style.color = 'black';
    const estado = document.getElementById('estadoMonitor');
    try {
      const respuesta = await fetch('/api/obtener_entradas', {cache: 'no-store'});
      if (!respuesta.ok) {throw new Error('Error HTTP: ' + respuesta.status);}
      const datos = await respuesta.json();
      const tabla = document.getElementById('tablaEntradas');
      const fragmento = document.createDocumentFragment();
      datos.entradas.forEach(e => {
          const fila = document.createElement('tr');
          [e.entrada_fisica, e.asignacion, e.valor_entrada, e.nombre, e.tipo, e.offset, e.slope, e.unidades, e.valor_real].forEach(valor => {
              const celda = document.createElement('td');
              celda.textContent = valor ?? '';
              fila.appendChild(celda);
          });
)rawliteral");
client.println(R"rawliteral(
      fragmento.appendChild(fila);
      });
      tabla.replaceChildren(fragmento);
      estado.textContent ='Última actualización: ' + new Date().toLocaleTimeString();

} catch (error) {
    estado.textContent = 'Error al consultar las entradas. Se intentará nuevamente en 10 segundos.';
    console.error(error);
    }
    }
    progressBar.style.color = 'white';
    progressBar.style.width = progress + '%';
    progressBar.textContent = 'Siguiente actualización en: '+ (progress/10) + ' segundos.';
  }
  actualizarEntradas();
  setInterval(actualizarEntradas, 1000);
</script>)rawliteral");
}
#endif