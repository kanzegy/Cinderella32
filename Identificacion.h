#ifndef IDENTIFICACION_H
#define IDENTIFICACION_H

#include <Arduino.h>
#include <Ethernet2.h>
#include "PaginaHTML.h"
#include "configuracion_inicial.h"

inline void paginaIdentificacion(EthernetClient &client) {
    String contenido = "";

    contenido += "<h1>Identificación del sitio</h1>";
    contenido += "<div style='overflow-x:auto;'>";
    contenido += "<table>";
    contenido += "<thead><tr><th>Parámetro</th><th>Valor</th></tr></thead>";
    contenido += "<tbody>";

    contenido += "<tr><td>Sitio</td><td>" + String(config_sitio.sitio) + "</td></tr>";
    contenido += "<tr><td>Ciudad</td><td>" + String(config_sitio.ciudad) + "</td></tr>";
    contenido += "<tr><td>Responsable</td><td>" + String(config_sitio.responsable) + "</td></tr>";
    contenido += "<tr><td>IP</td><td>" + String(config_sitio.ip) + "</td></tr>";
    contenido += "<tr><td>Subred</td><td>" + String(config_sitio.subred) + "</td></tr>";
    contenido += "<tr><td>Gateway</td><td>" + String(config_sitio.gateway) + "</td></tr>";
    contenido += "<tr><td>DHCP</td><td>" + String(config_sitio.dhcp) + "</td></tr>";
    contenido += "<tr><td>Destino SNMP 1</td><td>" + String(config_sitio.dest_snmp_1) + "</td></tr>";
    contenido += "<tr><td>Destino SNMP 2</td><td>" + String(config_sitio.dest_snmp_2) + "</td></tr>";

    contenido += "</tbody>";
    contenido += "</table>";
    contenido += "</div>";

    String script = "";
    enviarHTML(client, "Identificación", contenido);
}

#endif