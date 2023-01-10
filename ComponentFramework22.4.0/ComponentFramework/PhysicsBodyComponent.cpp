#include "PhysicsBodyComponent.h"
#include "QMath.h"
#include "EngineManager.h"

using namespace MATH;
PhysicsBodyComponent::PhysicsBodyComponent(Component* parent_, Ref<TransformComponent> transform_) : Component(parent_) {
    transform = transform_;
    mass = 1.0f;

    vel = Vec3();
    accel = Vec3();
    maxSpeed = 4.0f;
    maxAcceleration = 200.0f;

    rotationalInertia = Matrix3();
    angularAccel = Vec3();
    angularVel = Vec3();
    maxAngular = 10.0f;
}

PhysicsBodyComponent::PhysicsBodyComponent(
    Component* parent_, Ref<TransformComponent> transform_, 
    float mass_ = 1.0f, 
    Vec3 vel_ = Vec3(),
    Vec3 accel_ = Vec3(),
    // These are not very good defaults, but they will prevent compiler warnings.
    float maxSpeed_ = 4.0f,
    float maxAcceleration_ = 10.0f,
    Matrix3 rotationalInertia_ = Matrix3(),
    Vec3 angularAccel_ = Vec3(),
    Vec3 angularVel_ = Vec3(),
    float maxAngular_ = 180.0f
) : Component(parent_)
{
    transform = transform_;
    mass = mass_;

    vel = vel_;
    accel = accel_;
    maxSpeed = maxSpeed_;
    maxAcceleration = maxAcceleration_;

    rotationalInertia = rotationalInertia_;
    angularAccel = angularAccel_;
    angularVel = angularVel_;
    maxAngular = maxAngular_;
}

PhysicsBodyComponent::PhysicsBodyComponent(
    Component* parent_, 
    float mass_ = 1.0f,
    Vec3 vel_ = Vec3(),
    Vec3 accel_ = Vec3(),
    // These are not very good defaults, but they will prevent compiler warnings.
    float maxSpeed_ = 4.0f,
    float maxAcceleration_ = 10.0f,
    Matrix3 rotationalInertia_ = Matrix3(),
    Vec3 angularAccel_ = Vec3(),
    Vec3 angularVel_ = Vec3(),
    float maxAngular_ = 180.0f
) : Component(parent_)
{
    transform = nullptr;
    mass = mass_;

    vel = vel_;
    accel = accel_;
    maxSpeed = maxSpeed_;
    maxAcceleration = maxAcceleration_;

    rotationalInertia = rotationalInertia_;
    angularAccel = angularAccel_;
    angularVel = angularVel_;
    maxAngular = maxAngular_;
}


PhysicsBodyComponent::~PhysicsBodyComponent() {
    OnDestroy();
}



void PhysicsBodyComponent::ApplyForce(Vec3 force_) {
    accel = force_ / mass;
}

void PhysicsBodyComponent::Update(float deltaTime) {
    if (vel != Vec3() || accel != Vec3()) {
        //I could use Physics::SimpleNewtonMotion but this is quicker
        transform->SetPosition(transform->GetPosition() + vel * deltaTime + accel * (0.5f * deltaTime * deltaTime));
        vel = vel + accel * deltaTime;

        // Clip to maxSpeed, if speed exceeds max
        /*if (VMath::mag(vel) > maxSpeed)
        {
            vel = VMath::normalize(vel) * maxSpeed;
        }*/
    }

    // Clip to maxRotation, if needed
    //if (rotation > maxRotation) rotation = maxRotation;

    // Could introduce dampening, of velocity and/or rotation, to simulate friction
    //vel -= 0.05 * vel;
    //rotation -= 0.05 * rotation;
}

bool PhysicsBodyComponent::OnCreate() {
    //find parent actor
    for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
        if (actor.second->GetComponent<PhysicsBodyComponent>() != nullptr && actor.second->GetComponent<PhysicsBodyComponent>().get() == this) {
            transform = actor.second->GetComponent<TransformComponent>();

            isCreated = true;
            break;
        }
    }
    if (transform == nullptr) {
        //throw std::invalid_argument("Transform component must be defined first before the PhysicsBodyComponent");
        isCreated = false;
    }
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