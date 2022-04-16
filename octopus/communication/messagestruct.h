#ifndef MYSTERIO_OCTOPUS_MESSAGE_STRUCT_H_
#define MYSTERIO_OCTOPUS_MESSAGE_STRUCT_H_
#pragma pack(1)

typedef struct message_t {
    uint32_t idMessage;
    uint32_t typeMessage;
    uint32_t idUAV;
    float velocity;
    float battery;
    float location_x;
    float location_y;
    float location_z;
} message;

#pragma pack()
#endif
