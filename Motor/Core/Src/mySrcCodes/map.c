#include "myIncCodes/map.h"

#define LOCATION_NUMBERS 16
LOCATION locations[LOCATION_NUMBERS] = {0};
uint8_t paths[LOCATION_NUMBERS];
uint8_t origine_pos;

// Map create
bool MapCreate(void) {
    int i, j;
    uint32_t _id, _locate, _length;
    // Init map
    for(i = 0; i < LOCATION_NUMBERS; i++) {
        locations[i].id = i;
        for(j = 0; j < MAX_CONNECTIONS; j++) {
            locations[i].connect[j].locate = 0;
            locations[i].connect[j].length = 0;
        }
    }
    // Input map
    LOCATION_INPUT locations_input[LOCATION_NUMBERS] = {
        {0, {{0, 0}}},
        {1, {
            {2, 1},
            {3, 1},
            {0, 0}}},
        {2, {
            {1, 1},
            {0, 0}}},
        {3, {
            {1, 1},
            {4, 1},
            {0, 0}}},
        {4, {
            {3, 1},
            {0, 0}}}
    };
    // Write map
    for(i = 0; i < LOCATION_NUMBERS; i++) {
        _id = locations_input[i].id;
        // Invalid index
        if(_id >= LOCATION_NUMBERS) return false;

        for(j = 0; j < MAX_CONNECTIONS; j++) {
            _locate = locations_input[i].connect[j].locate;
            _length = locations_input[i].connect[j].length;
            
            // Input finished
            if(_locate == 0) break;
            // Invalid index
            if(_locate >= LOCATION_NUMBERS) return false;
            
            locations[_id].connect[j].locate = _locate;
            locations[_id].connect[j].length = _length;
        }
    }
    return true;
}

// Find path
bool MapFindPath(uint32_t target) {
    int i, j;
    bool visited[LOCATION_NUMBERS] = {false};
    uint32_t dist[LOCATION_NUMBERS];
    uint32_t prev[LOCATION_NUMBERS] = {0};
    VehicleStop();
    for(i = 0; i < LOCATION_NUMBERS; i++) {
        dist[i] = UINT32INF;
    }
    dist[origine_pos] = 0;
    visited[origine_pos] = true;
    // Dijkstra 演算法
    /*for (i = 0; i < LOCATION_NUMBERS; i++) {
        // 找出目前未訪問、且距離最小的節點
        uint32_t min_dist = UINT32INF, u = UINT32INF;
        for (j = 0; j < LOCATION_NUMBERS; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }

        // 如果所有可到達的點都處理完，或找到終點 target
        if (u == UINT32INF || u == target) break;

        // 標記 u 為已訪問
        visited[u] = true;

        // 更新 u 的所有鄰居 v
        for (j = 0; j < MAX_CONNECTIONS; j++) {
            if (locations[u].connect[j].locate == NULL) break;
            uint32_t v = locations[u].connect[j].locate->id;
            uint32_t weight = locations[u].connect[j].length;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }*/
    return true;
}