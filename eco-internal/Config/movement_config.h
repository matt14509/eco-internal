#pragma once
#include "Headers.h"
using json = nlohmann::json;
class MovementConfig
{
    std::string ConfigName;

public:
    MovementConfig(const std::string& name)
    {
        ConfigName = name;
    }
    //vars
    bool needToggleFly = false;
    bool needTeleport = false;
    Vector3 teleportCoords = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 saved_pos = Vector3(0.0f, 0.0f, 0.0f);
    float moveSpeed = 3.30f;
    float maxSpeed = 6.00f;
    float groundAccel = 30.00f;
    float airAccel = 5.00f;
    float jumpForce = 11.00f;
    float swimUpSpeed = 1.80f;





    json ToJson()
    {
        json j;
        j["needToggleFly"] = needToggleFly;
        j["needTeleport"] = needTeleport;
        j["teleportCoords"] = teleportCoords;

        j["saved_pos"] = saved_pos;
        j["moveSpeed"] = moveSpeed;
        j["maxSpeed"] = maxSpeed;
        j["groundAccel"] = groundAccel;
        j["airAccel"] = airAccel;
        j["jumpForce"] = jumpForce;
        j["swimUpSpeed"] = swimUpSpeed;


        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j.contains("needToggleFly"))   needToggleFly = j["needToggleFly"];
        if (j.contains("needTeleport"))   needTeleport = j["needTeleport"];
        if (j.contains("teleportCoords"))
            teleportCoords = j["teleportCoords"].get<Vector3>();
        if (j.contains("saved_pos"))
            saved_pos = j["saved_pos"].get<Vector3>();
        if (j.contains("moveSpeed"))   moveSpeed = j["moveSpeed"];
        if (j.contains("maxSpeed"))    maxSpeed = j["maxSpeed"];
        if (j.contains("groundAccel")) groundAccel = j["groundAccel"];
        if (j.contains("airAccel"))    airAccel = j["airAccel"];
        if (j.contains("jumpForce"))   jumpForce = j["jumpForce"];
        if (j.contains("swimUpSpeed")) swimUpSpeed = j["swimUpSpeed"];


    }




};