#include "mem.h"
#include "Offsets.h"
#include <math.h>

# define M_PI           3.14159265358979323846

struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

struct Name
{
    char chars[16];
};

class Entity
{
public:
    uintptr_t baseAddress;          // 8 bytes
    Vector2 viewAngels;             // 8 bytes
    char name[16];                  // 16 bytes
    Vector3 feet, head;             // 24 bytes (2 * 12 bytes)
    float mag, viewOffset;          // 8 bytes (2 * 4 bytes)
    int health, currentAmmo;        // 8 bytes (2 * 4 bytes)
    bool team, alive;               // 2 bytes

    void CalcMag(Entity Player)
    {
        this->mag = sqrt(pow(Player.feet.x - this->feet.x, 2)
                    + pow(Player.feet.y - this->feet.y, 2)
                    + pow(Player.feet.z - this->feet.z, 2));
    }

    Vector2 CalcAngles(Entity destEnt)
    {
        Vector2 view;

        float deltaX = destEnt.head.x - this->head.x;
        float deltaY = destEnt.head.y - this->head.y;
        float deltaZ = destEnt.head.z - this->head.z;
        float dist = (float)sqrt(pow(destEnt.feet.x - this->feet.x, 2) + pow(destEnt.feet.y - this->feet.y, 2));

        view.x = (float)(atan2(deltaY, deltaX) * 180 / M_PI) + 90;
        view.y = (float)(atan2(deltaZ, dist) * 180 / M_PI);

        return view;
    }
};

class methods
{
public:
    Entity ReadEntity(uintptr_t EntityBaseAddress, Mem& mem)
    {
        Entity ent;

        ent.baseAddress = EntityBaseAddress;
        ent.feet = mem.Read<Vector3>(ent.baseAddress + Offsets::Feet);
        ent.head = mem.Read<Vector3>(ent.baseAddress + Offsets::Head);
        ent.viewAngels = mem.Read<Vector2>(ent.baseAddress + Offsets::View);
        ent.health = mem.Read<int>(ent.baseAddress + Offsets::Health);
        Name nameStruct = mem.Read<Name>(ent.baseAddress + Offsets::Name);
        std::memcpy(ent.name, nameStruct.chars, sizeof(ent.name));
        ent.currentAmmo = mem.Read<int>(ent.baseAddress + Offsets::Ammo);
        ent.team = mem.Read<bool>(ent.baseAddress + Offsets::Team);
        ent.alive = mem.Read<bool>(ent.baseAddress + Offsets::Alive);

        return ent;
    }
};

class ViewMatrix
{
public:
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;
};