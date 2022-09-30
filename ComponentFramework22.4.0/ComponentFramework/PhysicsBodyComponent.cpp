#include "PhysicsBodyComponent.h"
#include "QMath.h"

using namespace MATH;
PhysicsBodyComponent::PhysicsBodyComponent(Component* parent_, Ref<TransformComponent> transform_) : Component(parent_) {
    transform = transform_;

    vel = Vec3();
    accel = Vec3();
    mass = 1.0f;
    radius = 0.2f;
    rotation = 0.0f;
    angular = 0.0f;
    maxSpeed = 4.0f;
    maxAcceleration = 200.0f;
    maxRotation = 60.0f;
    maxAngular = 10.0f;
}

PhysicsBodyComponent::PhysicsBodyComponent(
    Component* parent_, Ref<TransformComponent> transform_, Vec3 vel_, Vec3 accel_,
    float mass_,
    float radius_ = 0.2f,
    float rotation_ = 0.0f,
    float angular_ = 0.0f,
    // These are not very good defaults, but they will prevent compiler warnings.
    float maxSpeed_ = 4.0f,
    float maxAcceleration_ = 10.0f,
    float maxRotation_ = 180.0f,
    float maxAngular_ = 180.0f
) : Component(parent_)
{
    transform = transform_;
    vel = vel_;
    accel = accel_;
    mass = mass_;
    radius = radius_;
    rotation = rotation_;
    angular = angular_;
    maxSpeed = maxSpeed_;
    maxAcceleration = maxAcceleration_;
    maxRotation = maxRotation_;
    maxAngular = maxAngular_;
}


PhysicsBodyComponent::~PhysicsBodyComponent() {
    OnDestroy();
}



void PhysicsBodyComponent::ApplyForce(Vec3 force_) {
    accel = force_ / mass;
}

void PhysicsBodyComponent::Update(float deltaTime) {
    transform->SetPosition(transform->GetPosition() + vel * deltaTime + accel * (0.5f * deltaTime * deltaTime));
    vel = vel + accel * deltaTime;
    // Update orientation
    transform->setOrientation(QMath::angleAxisRotation(90,Vec3(1.0f,0.0f,0.0f)) * QMath::angleAxisRotation(rotation, Vec3(0.0f, 0.0f, 1.0f))); //IDK YET ABOUT THIS
    ////orientation += rotation * deltaTime;
    rotation += angular;

    // Clip to maxSpeed, if speed exceeds max
    if (VMath::mag(vel) > maxSpeed)
    {
        vel = VMath::normalize(vel) * maxSpeed;
    }

    // Clip to maxRotation, if needed
    if (rotation > maxRotation) rotation = maxRotation;

    // Could introduce dampening, of velocity and/or rotation, to simulate friction
    //vel -= 0.05 * vel;
    //rotation -= 0.05 * rotation;
}

bool PhysicsBodyComponent::OnCreate() {
    isCreated = true;
    return isCreated;
}

void PhysicsBodyComponent::OnDestroy() {
    isCreated = false;
}

void PhysicsBodyComponent::Render()const {}

/*void PhysicsBodyComponent::HandleEvents(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        printf("Mousedown\n");
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            printf("Space\n");
        }
    }
    // etc
}*/