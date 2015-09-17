/* Blip 'n Blup
// Copyright (C) 2015 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// Commercial licenses are available through frode@lindeijer.nl
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef INPUTMASTER_H
#define INPUTMASTER_H

#include <Urho3D/Urho3D.h>
#include "mastercontrol.h"
#include "helper.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

enum class JoystickButton {SELECT, LEFTSTICK, RIGHTSTICK, START, DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE};
enum class MasterInputAction { UP, RIGHT, DOWN, LEFT, CONFIRM, CANCEL, PAUSE, MENU, SCREENSHOT };
enum class PlayerInputAction { UP, RIGHT, DOWN, LEFT, RUN, JUMP, BUBBLE };

struct InputActions {
    Vector<MasterInputAction> master_;
    Vector<PlayerInputAction> player1_;
    Vector<PlayerInputAction> player2_;
};

class InputMaster : public Object
{
    OBJECT(InputMaster);
public:
    InputMaster(Context* context, MasterControl* masterControl);
private:
    MasterControl* masterControl_;
    Input* input_;

    HashMap<int, MasterInputAction> keyBindingsMaster_;
    HashMap<int, MasterInputAction> buttonBindingsMaster_;
    HashMap<int, PlayerInputAction> keyBindingsPlayer1_;
    HashMap<int, PlayerInputAction> buttonBindingsPlayer1_;
    HashMap<int, PlayerInputAction> keyBindingsPlayer2_;
    HashMap<int, PlayerInputAction> buttonBindingsPlayer2_;

    Vector<int> pressedKeys_;
    Vector<JoystickButton> pressedJoystickButtons_;

    void HandleUpdate(StringHash eventType, VariantMap &eventData);
    void HandleKeyDown(StringHash eventType, VariantMap &eventData);
    void HandleKeyUp(StringHash eventType, VariantMap &eventData);
    void HandleJoyButtonDown(StringHash eventType, VariantMap &eventData);
    void HandleJoyButtonUp(StringHash eventType, VariantMap &eventData);

    void HandleActions(const InputActions &actions);
    void HandlePlayerAction(PlayerInputAction action, CharacterID player = BLIP);
};

#endif // INPUTMASTER_H