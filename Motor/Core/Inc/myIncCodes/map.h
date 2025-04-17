#ifndef MY_MAP_H
#define MY_MAP_H

#include <stdint.h>
#include <stdbool.h>
#include "myIncCodes/vehicle.h"

#define MAX_CONNECTIONS 8
#define UINT32INF UINT32_MAX

typedef struct {
    uint32_t locate;
    uint32_t length;
} CONNECT;
typedef struct {
    uint32_t locate;
    uint32_t length;
} CONNECT_INPUT;
typedef struct LOCATION {
    uint32_t id;
    CONNECT connect[MAX_CONNECTIONS];
} LOCATION;
typedef struct {
    uint32_t id;
    CONNECT_INPUT connect[MAX_CONNECTIONS];
} LOCATION_INPUT;

bool MapCreate(void);

#endif
