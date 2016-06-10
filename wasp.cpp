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

#include "wasp.h"
#include "castmaster.h"
#include "resourcemaster.h"

Wasp::Wasp(Context* context) : Flyer(context)
{
    maxFlySpeed_ = 13.0f;
    flyThrust_ = 500.0f;

    chargeForce_ = 5000.0f;
    chargeInterval_ = 5.0f;

    SetUpdateEventMask(USE_UPDATE);
}

void Wasp::RegisterObject(Context* context)
{
    context->RegisterFactory<Wasp>();
}

void Wasp::OnNodeSet(Node *node)
{

    Flyer::OnNodeSet(node);

    node_->Translate(Quaternion(Random(360.0f), Vector3::UP) * Random(2.0f) * Vector3::FORWARD + Vector3::UP * 5.0f);
    node_->Rotate(Quaternion(Random(360.0f), Vector3::UP));

    model_->SetModel(RM->GetModel("Wasp"));
    model_->SetMaterial(RM->GetMaterial("VCol"));

    rigidBody_->SetCollisionLayer(LAYER(2));
    rigidBody_->SetCollisionMask(LAYER(0) + LAYER(1) + LAYER(2));
    rigidBody_->SetMass(2.0f);
    collider_->SetSphere(1.0f);

}

void Wasp::Update(float timeStep)
{
    Flyer::Update(timeStep);

    move_ = Quaternion(5.0f, Vector3::UP) * node_->GetDirection();
}
