#include "chess.h"
#include "chess_bishop.h"


void show_chess(Chess &chess_total, Camera &camera)
{
	const glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	Shader our_shader("model.vs", "model.fs");
	our_shader.use();
	our_shader.setVec3("objectColor", 0.8f, 0.8f, 0.0f);
	our_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	our_shader.setVec3("lightPos", lightPos);
	our_shader.setVec3("viewPos", camera.Position);


	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// view/projection transformations
	glm::mat4 projection_sun = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view_sun = camera.GetViewMatrix();
	our_shader.setMat4("projection", projection);
	our_shader.setMat4("view", view);

	our_shader.setMat4("model", chess_total.get_model());

	chess_total.show(our_shader);
}
