// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Common-Things.h"

#include <cstdlib>

int add_1(const int base_val, const int max_val)
{
    return (base_val >= max_val ? max_val : base_val + 1);
}

int sub_1(const int base_val, const int min_val)
{
    return (base_val <= min_val ? min_val : base_val - 1);
}

void add_blank_instructions(vector<u16>& instructions, const int amount_to_add)
{
    for (int i = 0; i < amount_to_add; i++)
        instructions.insert(instructions.end() - 1, 0);
}

void remove_last_instruction(vector<u16>& instructions)
{
    for (unsigned int i = 0; i < instructions.size(); i++)
    {
        const u8 opcode = instructions[i] >> 12;
        if (opcode <= LABS)
        {
            if (i < instructions.size() - 3)
                i++;
            else
                instructions.erase(instructions.begin() + i, instructions.begin() + i + 2);
        }
        else if (opcode == SVEC && i == instructions.size() - 2)
            instructions.erase(instructions.begin() + i);
    }
}

void remove_last_instructions(vector<u16>& instructions, const int amount_to_remove)
{
    instructions.erase(instructions.end() - (1 + amount_to_remove),
                       instructions.end() - 1);
}

void clear_instructions(vector<u16>& instructions)
{
    instructions.erase(instructions.begin(),
                       instructions.end() - 1);
}

void calculate_VCTR(const Long_Vector lv, vector<u16>& instructions)
{
    instructions[instructions.size() - 3] = (lv.opcode << 12)
                                          | ((lv.delta_y < 0) << 10)
                                          | std::abs(lv.delta_y);
    instructions[instructions.size() - 2] = (lv.brightness << 12)
                                          | ((lv.delta_x < 0) << 10)
                                          | std::abs(lv.delta_x);
}

void calculate_LABS(const Load_Absolute la, vector<u16>& instructions)
{
    instructions[instructions.size() - 3] = (LABS << 12)
                                          | ((la.y_position < 0) << 10)
                                          | std::abs(la.y_position);
    instructions[instructions.size() - 2] = (la.global_scale << 12)
                                          | ((la.x_position < 0) << 10)
                                          | std::abs(la.x_position);
}

void calculate_SVEC(const Short_Vector sv, vector<u16>& instructions)
{
    instructions[instructions.size() - 2] = (SVEC << 12)
                                          | ((sv.local_scale & 1) << 11)
                                          | ((sv.delta_y < 0) << 10)
                                          | ((std::abs(sv.delta_y) & 3) << 8)
                                          | (sv.brightness << 4)
                                          | ((sv.local_scale & 2) << 2)
                                          | ((sv.delta_x < 0) << 2)
                                          | (std::abs(sv.delta_x) & 3);
}
