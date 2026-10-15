#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "PaginaHTML.h"

inline void paginaDashboard(EthernetClient &client) {
  String contenido =
    "<h1>Dashboard</h1>"
    "<p>Selecciona una sección para continuar.</p>"
    "<div class='menu'>"
      "<a class='enlace' href='/identificacion'>Identificación<div class='ruta'>/identificacion</div></a>"
      "<a class='enlace' href='/entradas'>Entradas<div class='ruta'>/entradas</div></a>"
      "<a class='enlace' href='/hearbeat'>Heartbeat<div class='ruta'>/hearbeat</div></a>"
      "<a class='enlace' href='/alarmas'>Alarmas<div class='ruta'>/alarmas</div></a>"
      "<a class='enlace' href='/monitor'>Monitor<div class='ruta'>/monitor</div></a>"
      "<a class='enlace' href='/dashboard'>Dashboard<div class='ruta'>/dashboard</div></a>"
    "</div>";
  String script = "";
  enviarHTML(client, "Dashboard", contenido);
}

#endif