/* Blip 'n Blup
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
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

#include "fish.h"
#include "bubble.h"
#include "resourcemaster.h"

Fish::Fish(Context *context) : Walker(context),
    bubbleInterval_{0.5f},
    sinceBubble_{bubbleInterval_},
    blink_{}
{
    runThrust_ = 23500.0f;
    maxRunSpeed_ = 5.0f;

    jumpForce_ = 34.0f;
    jumpInterval_ = 0.1f;
    sinceJump_ = jumpInterval_;
    doubleJumper_ = true;

    idleAnim_ = "Models/Idle.ani";
    walkAnim_ = "Models/Walk.ani";
    midairAnim_ = "Models/Midair.ani";

//    SetUpdateEventMask(USE_UPDATE);
}

void Fish::RegisterObject(Context* context)
{
    context->RegisterFactory<Fish>();
}

void Fish::OnNodeSet(Node *node)
{ (void)node;

    Walker::OnNodeSet(node);

    model_->SetModel(RM->GetModel("Blip"));
    model_->SetMaterial(RM->GetMaterial("VColOutline"));

    model_->GetNode()->SetPosition(Vector3::DOWN * 0.523f);

    rigidBody_->SetCollisionLayer(LAYER(1));
    rigidBody_->SetCollisionMask(M_MAX_UNSIGNED);
    rigidBody_->SetMass(2.0f);
    collider_->SetCapsule(0.8f, 1.0f);

}
void Fish::BecomeBlup()
{
    model_->SetModel(RM->GetModel("Blup"));
    model_->SetMorphWeight(1, 1.0f);
    node_->Translate(Vector3::RIGHT);
}

void Fish::Update(float timeStep)
{
    Walker::Update(timeStep);

    sinceBubble_ += timeStep;

    //Blink
    model_->SetMorphWeight(0, Lerp(model_->GetMorphWeight(0), 0.0f, Clamp(timeStep * 13.0f, 0.0f, 1.0f)));
    blink_ -= timeStep;

    if (blink_ < 0.0f) {
        blink_ = Random(1.3f, 3.2);

        model_->SetMorphWeight(0, 1.0f);
    }

}

void Fish::HandleAction(int actionId)
{
    switch (actionId){
    case 0:
        break;
    case 1: Jump();
        break;
    case 2: BlowBubble();
        break;
    case 3:
        break;
    default: break;
    }
}

void Fish::BlowBubble()
{
    if (sinceBubble_ > bubbleInterval_) {
        sinceBubble_ = 0.0f;

        Node* bubbleNode{ MC->GetScene()->CreateChild("Bubble") };
        bubbleNode->SetPosition(node_->GetPosition() + node_->GetDirection() * 0.1f);
        bubbleNode->CreateComponent<Bubble>();
        RigidBody* bubbleBody{ bubbleNode->GetComponent<RigidBody>() };
        bubbleBody->SetLinearVelocity(rigidBody_->GetLinearVelocity());
        bubbleBody->ApplyImpulse(node_->GetDirection() * 2.3f);

        animCtrl_->Play("Models/Bubble.ani", 1, false, 0.05f);
        animCtrl_->SetTime("Models/Bubble.ani", 0.0f);
        animCtrl_->SetStartBone("Models/Bubble.ani", "Mouth");
        model_->SetMorphWeight(0, 1.0f);

        node_->GetOrCreateComponent<SoundSource>()->Play(RM->GetSample("ShootBubble"));
    }
}
