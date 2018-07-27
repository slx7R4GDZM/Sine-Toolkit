// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "Common-Things.h"

const string BRIGHTNESS_OPTIONS[] =
{
    "BRIGHTNESS 15",
    "BRIGHTNESS 12",
    "BRIGHTNESS 7",
    "BRIGHTNESS 0"
};

const string GLOBAL_SCALE_OPTIONS[] =
{
    "GLOBAL SCALE (*2)",
    "GLOBAL SCALE (*1)",
    "GLOBAL SCALE (/2)",
    "GLOBAL SCALE (/4)"
};

const string VCTR_OPTIONS[] =
{
    "LOCAL SCALE UP",
    "LOCAL SCALE DOWN",
    "LOCAL SCALE (/512)",
    "LOCAL SCALE (*1)"
};

const string LABS_OPTIONS[] =
{
    "GLOBAL SCALE UP",
    "GLOBAL SCALE DOWN",
    "GLOBAL SCALE (*1)",
    "GLOBAL SCALE (/2)"
};

const string SVEC_OPTIONS[] =
{
    "LOCAL SCALE UP",
    "LOCAL SCALE DOWN",
    "LOCAL SCALE (*2)",
    "LOCAL SCALE (*16)"
};

const string SHIP_CREATOR_OPTIONS[] =
{
    "BRIGHTNESS UP",
    "BRIGHTNESS DOWN",
    "BRIGHTNESS 0",
    "BRIGHTNESS 12"
};

/////////////////

const array<string, 5> MAIN_MENU_CHOICES =
{
    "EDIT VECTOR OBJECT",
    "EDIT TEXT",
    "EDIT SHIP",
    "EDIT SHIP THRUST",
    "TOGGLE VECTORS"
};

const array<string, 3> INSTRUCTION_MENU_CHOICES =
{
    "DRAW LONG VECTOR",
    "LOAD ABSOLUTE",
    "DRAW SHORT VECTOR"
};

const array<string, 5> VECTOR_OBJECT_MENU_CHOICES =
{
    "ADD INSTRUCTION",
    "REMOVE INSTRUCTION",
    "SET GLOBAL SCALE",
    "OUTPUT VECTOR OBJECT",
    "CLEAR VECTOR OBJECT"
};

const array<string, 4> SHIP_MENU_CHOICES =
{
    "ADD INSTRUCTION",
    "REMOVE INSTRUCTION",
    "OUTPUT SHIP",
    "CLEAR SHIP"
};

const array<string, 4> THRUST_MENU_CHOICES =
{
    "ADD INSTRUCTION",
    "REMOVE INSTRUCTION",
    "OUTPUT SHIP THRUST",
    "CLEAR SHIP THRUST"
};

const array<string, 3> TOGGLE_VECTORS_MENU_CHOICES =
{
    "BOX",
    "VECTOR OBJECT",
    "SHIP AND THRUST"
};

const string DEFAULT_SETTINGS =
R"(# Lines starting with # are ignored by the program

#==============================================================================
# List of Available Keys
#==============================================================================

# Escape F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 F13 F14 F15 Pause
# Tilde Num1 Num2 Num3 Num4 Num5 Num6 Num7 Num8 Num9 Num0 Dash Equal BackSpace
# Tab Q W E R T Y U I O P LBracket RBracket BackSlash
#      A S D F G H J K L SemiColon Quote Return
# LShift Z X C V B N M Comma Period Slash RShift
# LControl LSystem LAlt Space RAlt RSystem Menu RControl
# Insert Home PageUp  Divide Multiply Subtract
# Delete End PageDown Numpad7 Numpad8 Numpad9 Add
#                     Numpad4 Numpad5 Numpad6
#         Up          Numpad1 Numpad2 Numpad3
#   Left Down Right   Numpad0

#==============================================================================
# Button Settings
#==============================================================================

          B-Confirm E
           B-Cancel F
               B-Up W
             B-Down S
             B-Left A
            B-Right D
          B-Options Q
B-Toggle-Fullscreen F11
             B-Exit Escape

#==============================================================================
# Window Settings
#==============================================================================

## Which mode to start the window with
# 0 = Normal window (default)
# 1 = Borderless window
# 2 = Exclusive fullscreen
Starting-Window-Mode 0

## Starting window resolution
Starting-X-Resolution 1024
Starting-Y-Resolution 790

## Starting position for the program window
# -1 = Don't set the starting position (default)
Start-Window-Position-X -1
Start-Window-Position-Y -1

## What to do when the window isn't focused
# 0 = Pause the program (default)
# 1 = Run in background without input
# 2 = Run in background with input
Inactive-Mode 0

#==============================================================================
# Graphics Settings
#==============================================================================

## The x:y ratio to crop the image to
# < 1 = Crops the image starting with the left and right sides
# 1.0 = Scales the image to the lower resolution axis
# > 1 = Crops the image starting with the top and bottom (default)
Crop-Ratio 1.2962025

## MSAA toggle and quality setting
# 0 = Off
# 1 = 2x
# 2 = 4x
# 3 = 8x (default)
MSAA-Quality 3

## Gamma correction
Gamma-Correction 1.0

## Vertical synchronization
# 0 = Disabled (default)
# 1 = Enabled
V-Sync-Enabled 0

## Whether to use busy waiting or sleeping to limit FPS
# 0 = Use sleeping (default)
# 1 = Use busy waiting; this has high CPU usage, but it's consistent
Frame-Limit-Mode 0
)";
