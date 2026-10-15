
#ifndef ENTRADAS_H
#define ENTRADAS_H

#include <Arduino.h>
#include <Ethernet2.h>
#include <Adafruit_ADS1X15.h>
#include <cstring>
#include <cstdlib>

#include "PaginaHTML.h"
#include "configuracion_inicial.h"

extern Adafruit_ADS1115 ads;

inline void paginaEntradas(EthernetClient &client) {
    String contenido;

    contenido += R"rawliteral(
        <h1>Entradas</h1>
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
            <tbody>
    )rawliteral";

    for (int i = 0; i < NUM_ENTRADAS; i++) {
        EntradaTabla &config = tabla_entradas[i];

        int valor = 0;

        if (strncmp(config.entrada_fisica, "GPIO", 4) == 0) {
            int pin = atoi(config.entrada_fisica + 4);

            if (strcmp(config.tipo, "DIGITAL") == 0) {
                valor = digitalRead(pin);
            } else {
                valor = analogRead(pin);
            }
        } else if (strcmp(config.entrada_fisica, "ADS1115-01") == 0) {
            valor = ads.readADC_Differential_0_1();
        } else if (strcmp(config.entrada_fisica, "ADS1115-23") == 0) {
            valor = ads.readADC_Differential_2_3();
        }

        float valorReal = (valor - config.offset) * config.slope;

        contenido += "<tr>";
        contenido += "<td>" + String(config.entrada_fisica) + "</td>";
        contenido += "<td>" + String(config.asignacion) + "</td>";
        contenido += "<td>" + String(valor) + "</td>";
        contenido += "<td>" + String(config.nombre) + "</td>";
        contenido += "<td>" + String(config.tipo) + "</td>";
        contenido += "<td>" + String(config.offset) + "</td>";
        contenido += "<td>" + String(config.slope) + "</td>";
        contenido += "<td>" + String(config.unidades) + "</td>";
        contenido += "<td>" + String(valorReal, 2) + "</td>";
        contenido += "</tr>";
    }

    contenido += R"rawliteral(
            </tbody>
        </table>
        </div>
    )rawliteral";
    String script = "";
    enviarHTML(client, "Entradas", contenido);
}

#endif
