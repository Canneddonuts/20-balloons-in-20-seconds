#ifndef CONTROLS_HEADER
#define CONTROLS_HEADER

#define INPUT_OPTION_PRESSED IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)
#define INPUT_JUMP_PRESSED IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_Z) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)

#define INPUT_LEFT_DOWN IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)
#define INPUT_RIGHT_DOWN IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)

#endif
