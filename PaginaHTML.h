#ifndef PAGINA_HTML_H
#define PAGINA_HTML_H

#include <Ethernet2.h>

void imprimirArray(EthernetClient &client, String arreglo[], int longitud) {
    for (int i = 0; i < longitud; i++) {
        client.println(arreglo[i]);
    }
}

inline void enviarHTML(EthernetClient &client, const char* titulo, const String& contenido) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html lang='es'>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.print("<title>");
  client.print(titulo);
  client.println("</title>");

  client.println("<style>");
  client.println("*{box-sizing:border-box}");
  client.println("body{margin:0;font-family:Arial,sans-serif;background:#101827;color:#f1f5f9}");
  client.println(".contenedor{max-width:900px;margin:60px auto;padding:24px}");
  client.println(".panel{background:#1e293b;padding:32px;border-radius:16px;box-shadow:0 8px 30px #0004}");
  client.println("h1{margin-top:0;color:#38bdf8}");
  client.println("p{color:#cbd5e1;line-height:1.6}");
  client.println(".boton{display:inline-block;background:#0284c7;color:white;text-decoration:none;padding:13px 20px;border-radius:8px;margin-top:12px;border:0;cursor:pointer;font-size:16px}");
  client.println(".boton:hover{background:#0369a1}");
  client.println(".menu{display:grid;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));gap:16px;margin-top:24px}");
  client.println(".enlace{display:block;background:#334155;padding:22px;border-radius:10px;color:#f8fafc;text-decoration:none;border:1px solid #475569}");
  client.println(".enlace:hover{background:#475569;border-color:#38bdf8}");
  client.println(".ruta{font-size:13px;color:#38bdf8;margin-top:8px}");
  client.println(".volver{display:inline-block;margin-top:24px;color:#38bdf8;text-decoration:none}");
  client.println(".tabla-bonita { width: 100%; border-collapse: collapse; margin: 20px 0; font-family: Arial, sans-serif; font-size: 14px; background: #ffffff; border-radius: 8px; overflow: hidden; box-shadow: 0 2px 8px rgba(0, 0, 0, 0.10); }");
  client.println(".tabla-bonita thead { background: #174a7e; color: #ffffff; }");
  client.println(".tabla-bonita th, .tabla-bonita td { padding: 12px 16px; text-align: left; border-bottom: 1px solid #e5e7eb; }");
  client.println(".tabla-bonita tbody tr:nth-child(even) { background: #f3f6fa; }");
  client.println(".tabla-bonita tbody tr:hover { background: #e3edf8; }");
  client.println(".tabla-bonita tbody tr:last-child td { border-bottom: none; }");
  client.println(".progress-container { width: 100%; height: 24px; background: #e5e7eb; border-radius: 12px; overflow: hidden; }");
  client.println(".progress-bar { height: 100%; width: 0%; background: #1976d2; color: white; text-align: center; line-height: 24px; font-size: 12px; font-weight: bold; transition: width 0.3s ease; }");
  client.println("table { width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem; }");
  client.println("th { color: #00a2ff; font-weight: 600; padding: 12px 16px; border-bottom: 2px solid #334155; text-transform: uppercase; font-size: 0.8rem; letter-spacing: 0.5px; }");
  client.println("td { padding: 14px 16px; color: #ffffff; border-bottom: 1px solid #334155; }");
  client.println("tbody tr:hover { background-color: #283548; transition: background-color 0.2s ease-in-out; }");
  client.println("tbody tr:last-child td { border-bottom: none; }");
  client.println("</style>");

  client.println("</head>");
  client.println("<body>");
  client.println("<div class='contenedor'>");
  client.println("<div class='panel'>");
  client.print(contenido);
  client.println("</div>");
  client.println("</div>");
  client.println("</body>");
  client.println("</html>");
}

inline void enviar404(EthernetClient &client) {
  client.println("HTTP/1.1 404 Not Found");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html><html lang='es'><head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<title>404</title></head>");
  client.println("<body style='font-family:Arial;background:#101827;color:white;padding:40px'>");
  client.println("<h1>404 - Página no encontrada</h1>");
  client.println("<p>La dirección solicitada no existe.</p>");
  client.println("<a href='/' style='color:#38bdf8'>Ir al inicio</a>");
  client.println("</body></html>");
}
inline void paginaSeccion(
    EthernetClient &client,
    const char* nombre,
    const char* ruta
) {
    String contenido;
    contenido += "<h1>";
    contenido += nombre;
    contenido += "</h1>";
    String script = "";
    enviarHTML(client, nombre, contenido);
}
#endif