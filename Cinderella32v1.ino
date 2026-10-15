
#include <SPI.h>
#include <Ethernet2.h>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "PaginaHTML.h"
#include "Bienvenida.h"
#include "Dashboard.h"
#include "Monitor.h"
#include "ApiEntradas.h"
#include "Entradas.h"
#include "Identificacion.h"
#include "HeartBeat.h"
#include "Alarmas.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(169, 254, 183, 10);
IPAddress gateway(169, 254, 183, 1);
IPAddress subnet(255, 255, 0, 0);

#define W5500_CS 5
#define W5500_RST 16
#define MAX_CUERPO_JSON 8192

EthernetServer server(80);
Adafruit_ADS1115 ads;

int entradasDigitales[] = {2, 12, 13, 14, 15, 17, 25, 26, 27, 32, 33};
int numDigitales = sizeof(entradasDigitales) / sizeof(int);

int entradasAnalogicas[] = {34, 35, 36, 39};
int numAnalogicas = sizeof(entradasAnalogicas) / sizeof(int);

void resetW5500() {
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, LOW);
  delay(100);
  digitalWrite(W5500_RST, HIGH);
  delay(100);
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < numDigitales; i++) {
    pinMode(entradasDigitales[i], INPUT);
  }

  ads.begin();
  ads.setGain(GAIN_ONE);

  resetW5500();

  Ethernet.init(W5500_CS);
  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  server.begin();

  Serial.println("Servidor web iniciado");
  Serial.print("IP: http://");
  Serial.println(Ethernet.localIP());
}

void responderError(EthernetClient &client, int codigo, const char *mensaje) {
  DynamicJsonDocument doc(256);
  doc["estado"] = "error";
  doc["mensaje"] = mensaje;

  client.print("HTTP/1.1 ");
  client.print(codigo);

  if (codigo == 413) {
    client.println(" Payload Too Large");
  } else if (codigo == 405) {
    client.println(" Method Not Allowed");
  } else if (codigo == 408) {
    client.println(" Request Timeout");
  } else {
    client.println(" Bad Request");
  }

  client.println("Content-Type: application/json; charset=UTF-8");
  client.println("Connection: close");
  client.println("Cache-Control: no-store");
  client.println();

  serializeJson(doc, client);
}

void procesarSolicitud(
  EthernetClient &client,
  String metodo,
  String ruta,
  const String &cuerpo
) {
  int queryIndex = ruta.indexOf('?');

  if (queryIndex >= 0) {
    ruta = ruta.substring(0, queryIndex);
  }

  if (ruta == "/api/obtener_entradas" && metodo == "GET") {
    obtener_entradas(client);
  }
  else if (ruta == "/api/guardar_entradas" && metodo == "POST") {
    //guardar_entradas(client, cuerpo);
  }
  else if (ruta == "/api/obtener_entradas" ||
           ruta == "/api/guardar_entradas") {
    responderError(client, 405, "Metodo HTTP no permitido para esta ruta.");
  }
  else if (metodo != "GET") {
    responderError(client, 405, "Metodo HTTP no permitido.");
  }
  else if (ruta == "/" || ruta == "/index.html") {
    paginaBienvenida(client);
  }
  else if (ruta == "/dashboard") {
    paginaDashboard(client);
  }
  else if (ruta == "/entradas") {
    paginaEntradas(client);
  }
  else if (ruta == "/identificacion") {
    paginaIdentificacion(client);
  }
  else if (ruta == "/hearbeat") {
      paginaHeartbeat(client);
  }
  else if (ruta == "/alarmas") {
      paginaAlarmas(client);
  }
  else if (ruta == "/monitor") {
    paginaMonitor(client);
  }
  else {
    enviar404(client);
  }
}

void loop() {
  EthernetClient client = server.available();

  if (!client) {
    return;
  }

  client.setTimeout(3000);

  // Leer la primera línea de la solicitud HTTP.
  String primeraLinea = client.readStringUntil('\n');
  primeraLinea.trim();

  if (primeraLinea.length() == 0) {
    client.stop();
    return;
  }

  int primerEspacio = primeraLinea.indexOf(' ');
  int segundoEspacio = primeraLinea.indexOf(' ', primerEspacio + 1);

  if (primerEspacio < 0 || segundoEspacio <= primerEspacio) {
    responderError(client, 400, "Primera linea HTTP invalida.");
    client.stop();
    return;
  }

  String metodo = primeraLinea.substring(0, primerEspacio);
  String ruta = primeraLinea.substring(primerEspacio + 1, segundoEspacio);

  // Leer encabezados HTTP y obtener Content-Length.
  int longitudCuerpo = 0;
  bool longitudValida = false;
  unsigned long inicioEncabezados = millis();

  while (client.connected() &&
         millis() - inicioEncabezados < 3000) {

    String encabezado = client.readStringUntil('\n');
    encabezado.trim();

    if (encabezado.length() == 0) {
      break;
    }

    String encabezadoMinusculas = encabezado;
    encabezadoMinusculas.toLowerCase();

    if (encabezadoMinusculas.startsWith("content-length:")) {
      String valor = encabezado.substring(
        encabezado.indexOf(':') + 1
      );

      valor.trim();

      longitudCuerpo = valor.toInt();
      longitudValida = true;
    }
  }

  String cuerpo;

  if (metodo == "POST" && ruta.startsWith("/api/guardar_entradas")) {
    if (!longitudValida || longitudCuerpo <= 0) {
      responderError(
        client, 400,
        "Falta el encabezado Content-Length o el cuerpo esta vacio."
      );

      client.stop();
      return;
    }

    if (longitudCuerpo > MAX_CUERPO_JSON) {
      responderError(
        client, 413,
        "El cuerpo JSON excede el tamano permitido."
      );

      client.stop();
      return;
    }

    cuerpo.reserve(longitudCuerpo);

    unsigned long inicioCuerpo = millis();

    while (cuerpo.length() < (unsigned int)longitudCuerpo &&
           millis() - inicioCuerpo < 5000) {

      while (client.available() &&
             cuerpo.length() < (unsigned int)longitudCuerpo) {
        int caracter = client.read();

        if (caracter >= 0) {
          cuerpo += (char)caracter;
        }
      }

      if (cuerpo.length() < (unsigned int)longitudCuerpo) {
        if (!client.connected() && !client.available()) {
          break;
        }

        delay(1);
      }
    }

    if (cuerpo.length() != (unsigned int)longitudCuerpo) {
      responderError(
        client, 408,
        "No se recibio completo el cuerpo de la solicitud."
      );

      client.stop();
      return;
    }
  }

  procesarSolicitud(client, metodo, ruta, cuerpo);

  delay(1);
  client.stop();
}