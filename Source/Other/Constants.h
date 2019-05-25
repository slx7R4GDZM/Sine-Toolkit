// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include <cmath>
#include "Common-Things.h"

const nanoseconds TARGET_FRAME_TIME = 16666666ns;

const string SETTINGS_FILENAME = "Sine-Toolkit-Settings.cfg";

const string KEY_TABLE[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9", "Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt", "RSystem", "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period", "Quote", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", "Space", "Enter", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right", "Up", "Down", "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "Pause"};

const u8 TOTAL_BUTTONS = 9;
const string BUTTON_TABLE[] = {"B-Confirm", "B-Cancel", "B-Up", "B-Down", "B-Left", "B-Right", "B-Options", "B-Toggle-Fullscreen", "B-Exit"};

const u16 MAX_VO_INSTRUCTIONS = 100;

const float PI = std::atan(1) * 4;

// ship creator
const u8 DELTA_LENGTH = 8;
const u8 DELTA_MAX = 256 / DELTA_LENGTH;
const u8 ROTATIONS = 17;
