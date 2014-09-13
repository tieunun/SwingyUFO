//
//  Config.h
//  SwingyChute
//

#ifndef SwingyChute_Config_h
#define SwingyChute_Config_h

typedef enum {
    PLAYER = 1 << 0,
    PLATFORM = 1 << 1,
    POINT_ZONE = 1 << 2,
    EDGE = 1 << 3
} PhysicsGroup;

#endif
