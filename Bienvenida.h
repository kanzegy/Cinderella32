#ifndef BIENVENIDA_H
#define BIENVENIDA_H

#include "PaginaHTML.h"

inline void paginaBienvenida(EthernetClient &client) {
  String contenido =
    "<h1>Bienvenido</h1>"
    "<p>Bienvenido al sistema de monitoreo y control.</p>"
    "<p>Selecciona el siguiente botón para acceder al panel principal.</p>"
    "<a class='boton' href='/dashboard'>Entrar al dashboard</a>";

  String script = "";
  enviarHTML(client, "Bienvenida", contenido);
}

#endif