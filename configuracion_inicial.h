#ifndef CONFIGURACION_INICIAL_H
#define CONFIGURACION_INICIAL_H

#define NUM_ENTRADAS 17
#define NUM_HEARTBEATS 4
#define NUM_ALARMAS 32
#define LONG_STRING 32
#define LONG_OID 64

// Estructura para la tabla de Entradas[cite: 1]
typedef struct {
    char entrada_fisica[LONG_STRING];
    char asignacion[LONG_STRING];
    char nombre[LONG_STRING];
    char tipo[LONG_STRING];
    int offset;
    int slope;
    char unidades[LONG_STRING];
} EntradaTabla;

// Estructura para la tabla de Configuración del Sitio[cite: 2]
typedef struct {
    char sitio[LONG_STRING];
    char ciudad[LONG_STRING];
    char responsable[LONG_STRING];
    char ip[LONG_STRING];
    char subred[LONG_STRING];
    char gateway[LONG_STRING];
    char dhcp[LONG_STRING];
    char dest_snmp_1[LONG_STRING];
    char dest_snmp_2[LONG_STRING];
    char password_admin[LONG_STRING];
} ConfigSitio;

// Estructura para la tabla HEART-BEAT[cite: 3]
typedef struct {
    char heartbeat[LONG_STRING];
    char hora[LONG_STRING];
    char oid[LONG_OID];
} HeartBeatTabla;

// Estructura para la tabla ALARMAS[cite: 4]
typedef struct {
    int slot;
    char nombre[LONG_STRING];
    char entrada[LONG_STRING];
    char habilitada[2]; // "S" o "N"
    float limite;
    char logica[LONG_STRING]; // "NA" o "NC"
    char oid_set[LONG_OID];
    char oid_clear[LONG_OID];
} AlarmaTabla;

// Declaración de variables globales editables
extern EntradaTabla tabla_entradas[NUM_ENTRADAS];
extern ConfigSitio config_sitio;
extern HeartBeatTabla tabla_heartbeat[NUM_HEARTBEATS];
extern AlarmaTabla tabla_alarmas[NUM_ALARMAS];

#endif // CONFIGURACION_INICIAL_H