#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "modernize-deprecated-headers"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef GENERAL_GAME_CONTROLLER_H
#define GENERAL_GAME_CONTROLLER_H

#include <stdint.h>

/**
 * Holds the @enum buttonFlags in a specific size.
 */
typedef uint16_t BUTTON_FLAGS;

/**
 * Different buttons that can be pressed on a standard gamepad.
 */
enum buttonFlags {
    BUTTON_none = 0,
    
    BUTTON_leftBumper =         1u<<0u,
    BUTTON_rightBumper =        1u<<1u,
    
    BUTTON_leftStickButton =    1u<<2u,
    BUTTON_rightStickButton =   1u<<3u,

    BUTTON_dpadUp =             1u<<4u,
    BUTTON_dpadDown =           1u<<5u,
    BUTTON_dpadLeft =           1u<<6u,
    BUTTON_dpadRight =          1u<<7u,

    BUTTON_actionUp =           1u<<8u,
    BUTTON_actionDown =         1u<<9u,
    BUTTON_actionLeft =         1u<<10u,
    BUTTON_actionRight =        1u<<11u,

    BUTTON_sysStart =           1u<<12u,
    BUTTON_sysOption =          1u<<13u,
    BUTTON_sysHome =            1u<<14u
};

/**
 * Holds all variable inputs from a standard game controller.
 */
typedef struct {
    // [0.0, 1.0]
    float leftTrigger;
    float rightTrigger; 

    // [-1.0, 1.0]
    float leftStickX;  // +: Right; -: Left
    float leftStickY;  // +: Up; -: Down
    float rightStickX; // +: Right; -: Left
    float rightStickY; // +: Up; -: Down

    BUTTON_FLAGS buttons;
} GAME_CONTROLLER_BULK;

#endif
#pragma clang diagnostic pop