// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Input-Handler.h"

#include "../Settings/Settings-Handler.h"

void Input_Handler::update(const Settings_Handler& settings_handler)
{
    for (int i = 0; i < TOTAL_BUTTONS; i++)
    {
        const bool was_button_pushed = button_held[i];
        if (Kb::isKeyPressed(settings_handler.get_button_key_code(static_cast<Button>(i))))
            button_held[i] = true;
        else
            button_held[i] = false;

        button_pushed[i] = !was_button_pushed && button_held[i];
    }
}

bool Input_Handler::is_pressed(Button button) const
{
    return button_held[button];
}

bool Input_Handler::on_press(Button button) const
{
    return button_pushed[button];
}

void Input_Handler::release_all_buttons()
{
    for (int i = 0; i < TOTAL_BUTTONS; i++)
    {
        button_held[i] = false;
        button_pushed[i] = false;
    }
}

bool Input_Handler::any_pressed() const
{
    for (int i = 0; i < TOTAL_BUTTONS; i++)
    {
        if (button_held[i])
            return true;
    }
    return false;
}

bool Input_Handler::movement_is_pressed() const
{
    for (int i = UP; i <= RIGHT; i++)
    {
        if (button_held[i])
            return true;
    }
    return false;
}

bool Input_Handler::movement_on_press() const
{
    for (int i = UP; i <= RIGHT; i++)
    {
        if (button_pushed[i])
            return true;
    }
    return false;
}
