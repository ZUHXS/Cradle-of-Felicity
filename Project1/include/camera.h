#ifndef CAMERA_H
#define CAMERA_H

#include "../glad/glad.h"
#include "../chess_class/chess.h"

#include <vector>
#include "../ChessBoard.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
const double G = 0.00223;
const double init_gun_hor_speed = 0.05;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
	
	// used for gun
	bool If_Move_Auto;
	glm::vec3 move_origin_position;
	glm::vec3 move_destination;
	glm::vec3 destination_sight;

    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

	int move_duration;
	bool if_parabola;   // 是否为抛物线视角，即是否为炮弹视角
	double init_gun_vert_speed;

	int step;  // step status, 0, when no select, 1 for select one

    // Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix(std::vector<Chess*>chess_list = std::vector<Chess*>());


    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, std::vector<Chess *> chess_list, std::vector<ChessBoard *> Block_list, bool &mouse_button, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};
#endif