#ifndef API_ENTRADAS_H
#define API_ENTRADAS_H

#include <Ethernet2.h>
#include <ArduinoJson.h>
#include <Adafruit_ADS1X15.h>
#include <cstring>
#include <cstdlib>
#include "configuracion_inicial.h"

extern Adafruit_ADS1115 ads;

inline void obtener_entradas(EthernetClient &client) {
    DynamicJsonDocument doc(8192);
    JsonArray entradas = doc.createNestedArray("entradas");

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

        JsonObject entrada = entradas.createNestedObject();

        entrada["entrada_fisica"] = config.entrada_fisica;
        entrada["asignacion"] = config.asignacion;
        entrada["valor_entrada"] = valor;
        entrada["nombre"] = config.nombre;
        entrada["tipo"] = config.tipo;
        entrada["offset"] = config.offset;
        entrada["slope"] = config.slope;
        entrada["unidades"] = config.unidades;
        entrada["valor_real"] = (valor - config.offset) * config.slope;
    }
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json; charset=UTF-8");
    client.println("Cache-Control: no-store");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Access-Control-Allow-Methods: GET, OPTIONS");
    client.println("Access-Control-Allow-Headers: Content-Type");
    client.println("Connection: close");
    client.println();
    serializeJson(doc, client);
}
#endif