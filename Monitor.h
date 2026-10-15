#ifndef MONITOR_H
#define MONITOR_H

#include <Ethernet2.h>
#include "PaginaHTML.h"

inline void paginaMonitor(EthernetClient &client) {
    String contenido = R"rawliteral(
        <h1>Monitor de entradas</h1>

        <p>Actualización automática cada 10 segundos.</p>

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

        <p id="estadoMonitor">Conectando...</p>
    )rawliteral";

    enviarHTML(client, "Monitor", contenido);

client.println(R"rawliteral(
<script>
  async function actualizarEntradas() {
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
          fragmento.appendChild(fila);
      });
      tabla.replaceChildren(fragmento);
      estado.textContent ='Última actualización: ' + new Date().toLocaleTimeString();
  )rawliteral");
client.println(R"rawliteral(
} catch (error) {
    estado.textContent = 'Error al consultar las entradas. Se intentará nuevamente en 10 segundos.';
    console.error(error);
    }
  }
  actualizarEntradas();
  setInterval(actualizarEntradas, 10000);
</script>)rawliteral");
}
#endif