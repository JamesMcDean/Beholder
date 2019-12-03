#ifndef GENERAL_ROBOT_CONTROL_H
#define GENERAL_ROBOT_CONTROL_H

/**
 * Holds all different control states for a robot.
 * All states are in the rand [-1.0, 1.0]
 */
typedef struct {
    float base_forward;             // +: Forward; -: Backward
    float base_strafe;              // +: Right; -: Left
    float base_rotation;            // +: Clockwise; -: Counter-Clockwise

    float primaryGun_pitch;         // +: Up; -: Down
    float primaryGun_rotation;      // +: Clockwise; -: Counter-Clockwise

    float secondaryGun_pitch;       // +: Up; -: Down
    float secondaryGun_rotation;    // +: Clockwise; -: Counter-Clockwise
} ROBOT_CONTROL_BULK;

#endif