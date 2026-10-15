
#ifndef ALARMAS_H
#define ALARMAS_H

#include <Arduino.h>
#include <Ethernet2.h>
#include "PaginaHTML.h"
#include "configuracion_inicial.h"

inline void paginaAlarmas(EthernetClient &client) {
    String contenido;

    contenido += R"rawliteral(
        <h1>Alarmas</h1>
        <div style="overflow-x:auto;">
        <table>
            <thead>
                <tr>
                    <th>Slot</th>
                    <th>Nombre</th>
                    <th>Entrada</th>
                    <th>Habilitada</th>
                    <th>Límite</th>
                    <th>Lógica</th>
                    <th>OID Set</th>
                    <th>OID Clear</th>
                </tr>
            </thead>
            <tbody>
    )rawliteral";

    for (int i = 0; i < NUM_ALARMAS; i++) {
        AlarmaTabla &alarma = tabla_alarmas[i];

        contenido += "<tr>";
        contenido += "<td>" + String(alarma.slot) + "</td>";
        contenido += "<td>" + String(alarma.nombre) + "</td>";
        contenido += "<td>" + String(alarma.entrada) + "</td>";
        contenido += "<td>" + String(alarma.habilitada) + "</td>";
        contenido += "<td>" + String(alarma.limite, 2) + "</td>";
        contenido += "<td>" + String(alarma.logica) + "</td>";
        contenido += "<td>" + String(alarma.oid_set) + "</td>";
        contenido += "<td>" + String(alarma.oid_clear) + "</td>";
        contenido += "</tr>";
    }

    contenido += R"rawliteral(
            </tbody>
        </table>
        </div>
    )rawliteral";

    String script = "";
    enviarHTML(client, "Alarmas", contenido);
}

#endif
