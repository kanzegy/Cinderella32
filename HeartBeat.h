
#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <Arduino.h>
#include <Ethernet2.h>
#include "PaginaHTML.h"
#include "configuracion_inicial.h"

inline void paginaHeartbeat(EthernetClient &client) {
    String contenido;

    contenido += R"rawliteral(
        <h1>Heartbeat</h1>
        <div style="overflow-x:auto;">
        <table>
            <thead>
                <tr>
                    <th>Heartbeat</th>
                    <th>Hora</th>
                    <th>OID</th>
                </tr>
            </thead>
            <tbody>
    )rawliteral";

    for (int i = 0; i < NUM_HEARTBEATS; i++) {
        HeartBeatTabla &hb = tabla_heartbeat[i];

        contenido += "<tr>";
        contenido += "<td>" + String(hb.heartbeat) + "</td>";
        contenido += "<td>" + String(hb.hora) + "</td>";
        contenido += "<td>" + String(hb.oid) + "</td>";
        contenido += "</tr>";
    }

    contenido += R"rawliteral(
            </tbody>
        </table>
        </div>
    )rawliteral";

    String script = "";
    enviarHTML(client, "Heartbeat", contenido);
}

#endif
