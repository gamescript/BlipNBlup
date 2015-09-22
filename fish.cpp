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

#include "fish.h"

Fish::Fish(Context *context, MasterControl *masterControl, bool blip) : Object(context),
    masterControl_{masterControl},
    blip_{blip},
    human_{true},
    alive_{true},
    running_{false},
    onGround_{false},
    doubleJumped_{false},
    jumpReleased_{true},
    sinceJumped_{0.0f},
    blink_{0.0f},
    maxWalkSpeed_{7.0f},
    runThrust_{2048.0f},
    movement_{Vector2::ZERO},
    jumpInterval_{0.23f},
    jumpForce_{23.0f}
{
    rootNode_ = masterControl->world_.scene_->CreateChild("Player");
    if (blip_) rootNode_->SetName("Blip");
    else rootNode_->SetName("Blup");

    model_ = rootNode_->CreateComponent<AnimatedModel>();
    if (blip_) model_->SetModel(masterControl->cache_->GetResource<Model>("Resources/Models/Blip.mdl"));
    else model_->SetModel(masterControl->cache_->GetResource<Model>("Resources/Models/Blup.mdl"));
    model_->SetMaterial(masterControl->cache_->GetResource<Material>("Resources/Materials/VCol.xml"));
    model_->SetCastShadows(true);
    model_->SetMorphWeight(1, !blip_);

    animCtrl_ = rootNode_->CreateComponent<AnimationController>();
    animCtrl_->PlayExclusive("Resources/Models/Walk.ani", 0, true);

    rigidBody_ = rootNode_->CreateComponent<RigidBody>();
    rigidBody_->SetMass(1.0f);
    rigidBody_->SetAngularFactor(Vector3::UP);
    rigidBody_->SetAngularDamping(0.9f);
    rigidBody_->SetFriction(0.6f);
    collider_ = rootNode_->CreateComponent<CollisionShape>();
    collider_->SetSphere(2.2f, Vector3::UP*1.1f);

    SubscribeToEvent(E_UPDATE, HANDLER(Fish, HandleUpdate));
    SubscribeToEvent(rootNode_, E_NODECOLLISIONSTART, HANDLER(Fish, HandleNodeCollisionStart));
}

void Fish::SetMovement(const Vector2 movement) {
    if (movement.Length() > 1.0f)
        movement_ = movement.Normalized();
    else movement_ = movement;
}

void Fish::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    sinceJumped_ += timeStep;

    if (alive_){
        //Apply movement
        Move(timeStep);
        //Blink
        Blink();
        //Think
        if (!human_){
            Think();
        }
    }
}

void Fish::HandleNodeCollisionStart(StringHash eventType, VariantMap &eventData)
{
    VectorBuffer contacts = eventData[NodeCollisionStart::P_CONTACTS].GetBuffer();
    while (!contacts.IsEof()) {
        Vector3 contactPosition = contacts.ReadVector3();
        Vector3 contactNormal = contacts.ReadVector3();
        if (contactNormal.Angle(Vector3::UP) < 23.0f) {
            onGround_ = true;
            doubleJumped_ = false;
        }
    }
}

void Fish::Move(float timeStep)
{
    Vector3 velocity = rigidBody_->GetLinearVelocity();
    Vector3 planarVelocity = Vector3(velocity.x_, 0.0f, velocity.z_);
    float runFloat = (0.3f*running_+0.7f);
    float onGroundFloat = (0.5f*onGround_+0.5f);
    if (planarVelocity.Length() < maxWalkSpeed_ * runFloat || planarVelocity.Angle(Vector3(movement_.x_, 0.0f, movement_.y_)) > 90.0f){
        float forceMultiplier = onGroundFloat * runFloat;
        rigidBody_->ApplyForce(forceMultiplier * timeStep * runThrust_ * Vector3(movement_.x_, 0.0f, movement_.y_));
    }
    //Update rotation in accordance with movement
    if (movement_.Length() > 0.23f)
    {
        Vector3 lookDirection = Vector3(movement_.x_, 0.0f, movement_.y_);
        Quaternion rotation = rootNode_->GetWorldRotation();
        Quaternion aimRotation = rotation;
        aimRotation.FromLookRotation(lookDirection);
        rootNode_->SetRotation(rotation.Slerp(aimRotation, Clamp((1.0f + (planarVelocity+movement_).Length()) * timeStep, 0.0f, 1.0f)));
    }
    animCtrl_->SetSpeed("Resources/Models/Walk.ani", planarVelocity.Length()*0.25f*onGroundFloat);
}

void Fish::Jump()
{
    if (sinceJumped_ > jumpInterval_ && jumpReleased_) {
        if (onGround_) {
            rigidBody_->ApplyImpulse(Vector3(
                                         0.23f*movement_.x_,
                                         Clamp(1.0f - movement_.Length(), 0.8f, 1.0f),
                                         0.23f*movement_.y_) * jumpForce_);
            onGround_ = false;
            sinceJumped_ = 0.0f;
            jumpReleased_ = false;
        } /*else if (!doubleJumped_ && rigidBody_->GetLinearVelocity().y_ > 0.0f) {
            rigidBody_->ApplyImpulse(Vector3::UP*0.75f*jumpForce_);
            doubleJumped_ = true;
            sinceJumped_ = 0.0f;
            jumpReleased_ = false;
        }*/
    }
}
void Fish::Blink()
{
    if (blink_ < 0.0001f && !Random(23)) blink_ = 1.0f;
    blink_ *= 0.9f;

    if (blink_ < 0.1f){
        if (model_->GetMorphWeight(0) != 0.0f) model_->SetMorphWeight(0, 0.0f);
    }
    else model_->SetMorphWeight(0, blink_);
}

void Fish::Think()
{
    movement_.x_ = sin(masterControl_->world_.scene_->GetElapsedTime()*1.23f);
    movement_.y_ = cos(masterControl_->world_.scene_->GetElapsedTime()*1.23f);
}