#include "../include/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(-position.x, -position.y, -position.z)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	If_Move_Auto = false;
	move_origin_position = glm::vec3(1.9f, 1.3f, 2.3f);
	move_destination = glm::vec3(-1.6f, 0.6f, 1.3f);
	destination_sight = move_destination - move_origin_position;
	move_duration = 1000;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	//return glm::lookAt(Position, Position + Front, Up);
	static int x = 0;
	if(!If_Move_Auto)
		return glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
	else
	{
		x += 1;
		if (x == move_duration)
		{
			this->If_Move_Auto = false;
			x = 0;
		}
		glm::vec3 gun_position(move_origin_position + ((move_destination - move_origin_position) / static_cast<float>(move_duration)) * static_cast<float>(x));
		//glm::vec3 gun_front(move_destination - move_origin_position);
		return glm::lookAt(gun_position, destination_sight, Up);
	}
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	static float x;
	static float redius = 1.3f;
	float velocity = MovementSpeed * deltaTime / 2;
	if (direction == FORWARD) {
		if (redius - velocity <= 0)
			return;
		Position.y = ((redius - velocity) / redius) * (float)Position.y;
		redius -= velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}
	//Position += Front * velocity;
	if (direction == BACKWARD)
	{
		Position.y = ((redius + velocity) / redius) * (float)Position.y;
		redius += velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}
	//Position -= Front * velocity;
	/*if (direction == LEFT)
	Position -= Right * velocity;
	if (direction == RIGHT)
	Position += Right * velocity;*/
	if (direction == LEFT)
	{
		x += velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}
	if (direction == RIGHT)
	{
		x -= velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}

}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}



void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}