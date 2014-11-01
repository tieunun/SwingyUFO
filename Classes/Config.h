//
//  Config.h
//  SwingyChute
//

#ifndef SwingyChute_Config_h
#define SwingyChute_Config_h

#define BG_COLOR Color4B(0.0f, 28.0f, 113.0f, 255.0f)

typedef enum {
    PLAYER = 1 << 0,
    PLATFORM = 1 << 1,
    POINT_ZONE = 1 << 2,
    EDGE = 1 << 3
} PhysicsGroup;

#endif
