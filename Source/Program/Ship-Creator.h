// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Ship_Creator
{
public:
    static void rotate_vector(Basic_Vector vector_to_rotate, vector<u16> vector_object_table[]);
    static void output_lives_icon(const vector<u16>& upward_ship_VO);
};
