#include "my_geom_objects.h"

// callbacks
float PRP_distance_scale[6] = { 0.5f, 1.0f, 2.5f, 5.0f, 10.0f, 20.0f };
int get_cow_flag = 0;
int dragon_dir = 0;
int spider_back_flag = 0;
int spider_angle_flag = 0;
int wolf_dir = 0;
int tiger_flag = 0;
int X_pressed = 0;
int Y_pressed = 0;
int Z_pressed = 0;
int T_pressed = 0;
int shader_flag = 0;
float dragon_angle = -90;
float dragon_move[3] = {-40.0f , 220.0f, -80.f }; // 120 == initial y 
float cow_move[3] = { -100.0f, 30.0f, -80.0f };
float spider_move[3] = { -280.0f, 25.0f, 210.0f };
float spider_dir[3][2] = { {2.0f,0.0f},{1.0f,-1.0f},{0.0f,-2.0f} };
float wolf_move[3] = { 100.0f, 0.0f, 0.0f };
float wolf_base = 200.0f;
float wolf_rotate = 0;
float tiger_move[3] = { -400.0f, 0.0f, -200.0f };
float tiger_rotate = 90;
float camera_move[3] = { 500.0f, 600.0f, 500.0f };
float camera_dir[3] = { 0.0f, 0.0f, 0.0f };
float camera_nvec[3] = { 0.0f, 1.0f, 0.0f };
float zoom_angle = 45;
float aspect_ratio;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(h_ShaderProgram_simple);
	set_ViewMatrix_from_camera_frame();
	ProjectionMatrix = glm::perspective(zoom_angle * TO_RADIAN, aspect_ratio, 100.0f, 20000.0f);
	ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);

	
	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);
	
	// Must update the light 1's geometry in EC.
	glm::vec4 position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
		light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
	glm::vec3 direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
		light[1].spot_direction[1], light[1].spot_direction[2]);
	glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);

	glm::vec4 position_EC4 = ViewMatrix * glm::vec4(/*camera_move[0]*/camera.pos[0],camera.pos[1], camera.pos[2], 1.0f);
	glUniform4fv(loc_light[2].position, 1, &position_EC4[0]);
	glm::vec3 direction_EC4 = glm::mat3(ViewMatrix) * glm::vec3(camera_dir[0] - camera_move[0], camera_dir[1] - camera_move[1], camera_dir[2] - camera_move[2]);
	glUniform3fv(loc_light[2].spot_direction, 1, &direction_EC4[0]);
	glUseProgram(0); // light in WC, EC position

	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);
  	set_material_floor();
	glUniform1i(loc_texture, TEXTURE_ID_FLOOR);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-500.0f, 0.0f, 500.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 1000.0f, 1000.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_floor();

	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-300.0f, 0.0f, 250.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(200.0f, 200.0f, 200.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_floor();
	
 	set_material_tiger();
	glUniform1i(loc_texture, TEXTURE_ID_TIGER);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(tiger_move[0], tiger_move[1], tiger_move[2]));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, tiger_rotate*TO_RADIAN, glm::vec3(0.0f, 1.0f,0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_tiger();

	glUseProgram(h_ShaderProgram_simple);
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();

	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);

	set_material_wolf();
	glUniform1i(loc_texture, TEXTURE_ID_WOLF);
	ModelViewMatrix = glm::rotate(ViewMatrix, -wolf_rotate * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(wolf_move[0], wolf_move[1], wolf_move[2]));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(100.0f, 100.0f,100.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_wolf();


	set_material_tiger();
	glUniform1i(loc_texture, TEXTURE_ID_SPIDER);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(spider_move[0], spider_move[1], spider_move[2]));
	//printf("%d", spider_angle_flag);
	ModelViewMatrix = glm::rotate(ModelViewMatrix, spider_angle_flag* -45.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(30.0f, -30.0f, 30.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_spider();

	if (T_pressed == 1) {
		set_material_dragon();
		glUniform1i(loc_texture, TEXTURE_ID_DRAGON);
	}
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(dragon_move[0],dragon_move[1], dragon_move[2]));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, dragon_angle * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_dragon();

	set_material_tiger();
	glUniform1i(loc_texture, TEXTURE_ID_TIGER);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(cow_move[0], cow_move[1], cow_move[2]));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(80.0f, 80.0f, 80.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_cow();

	set_material_tank();
	glUniform1i(loc_texture, TEXTURE_ID_TANK);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(50.0f, 0.0f, 230.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 125.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	if (shader_flag == 0) {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	draw_tank();

	glUseProgram(0);

	glutSwapBuffers();
}

void timer_scene(int value) {
	timestamp_scene = (timestamp_scene + 1) % UINT_MAX;
	cur_frame_tiger = timestamp_scene/3 % N_TIGER_FRAMES;
	cur_frame_ben = timestamp_scene % N_BEN_FRAMES;
	cur_frame_wolf= (timestamp_scene/2) % N_WOLF_FRAMES;
	cur_frame_spider = (timestamp_scene) % N_SPIDER_FRAMES;
	//rotation_angle_tiger = (timestamp_scene % 180) * TO_RADIAN;

	// tiger's movement
	switch (tiger_flag) {
	case 0: 
		tiger_rotate = 90;
		tiger_move[0] += 2.0f;
		if (tiger_move[0] > 200.0f)
			tiger_flag++;
		break;
	case 1:
		tiger_move[2] += 2.0f;
		tiger_rotate = 0;
		if (tiger_move[2] > 100.0f)
			tiger_flag++;
		break; 
	case 2:
		tiger_move[0] -= 2.0f;
		tiger_rotate = -90;
		if (tiger_move[0] < -200.0f)
			tiger_flag++;
		break;
	case 3:
		tiger_move[0] += 2.0f;
		tiger_rotate = 90;
		if (tiger_move[0] > 200.0f)
			tiger_flag++;
		break;
	case 4:
		tiger_move[2] -= 2.0f;
		tiger_rotate = 180;
		if (tiger_move[2] < -200.0f)
			tiger_flag++;
		break;
	case 5:
		tiger_move[0] -= 2.0f;
		tiger_rotate = -90;
		break;
	}

	if (tiger_move[0] < -500.0f) {
		tiger_move[0] = -400.0f;
		tiger_move[2] = -200.0f;
		tiger_flag = 0;
		rotation_angle_tiger = 90;
	}

	if (timestamp_scene % 270 >= 0 && timestamp_scene % 270 < 30) {
		tiger_move[1] += 5;
	}
	else if (timestamp_scene % 270 >= 30 && timestamp_scene % 270 < 60) {
		tiger_move[1] -= 5;
	}

	// dragom's movement
	if (dragon_dir == 0) {
		dragon_move[0] -= 1.0f;
		dragon_move[1] = pow(dragon_move[0] + 100.0f, 2)/20 + 40.0f;
	}
	else {
		dragon_move[0] += 1.0f;
		dragon_move[1] = pow(dragon_move[0] + 100.0f, 2)/20 + 40.0f;
	}

	if (dragon_move[1] == 220.0f) {
		if (dragon_dir == 0) {
			dragon_angle = 90;
			dragon_dir = 1;
		}
		else {
			dragon_angle = -90;
			dragon_dir = 0;
		}
	}

	if (dragon_move[0] == -100) {
		if (get_cow_flag == 0)
			get_cow_flag = 1;
		else
			get_cow_flag = 0;
	}

	// cow's movement
	if (get_cow_flag == 1) {
		cow_move[0] = dragon_move[0];
		cow_move[1] = dragon_move[1] - 10.0f;
	}
	else {
		cow_move[0] = -100.0f;
		cow_move[1] = 30.0f;
	}

	// spider's movement
	if (spider_back_flag == 0) {
		spider_move[1] += spider_dir[spider_angle_flag][0];
		spider_move[2] += spider_dir[spider_angle_flag][1];
	}
	else {
		spider_move[1] -= spider_dir[spider_angle_flag][0];
		spider_move[2] -= spider_dir[spider_angle_flag][1];
	}

	if (spider_move[1] == 175.0f || spider_move[2]==60.0f) {
		spider_back_flag = 1;
	}
	else if (spider_move[1] == 25.0f && spider_move[2]==210.0f) {
		spider_back_flag = 0;
		spider_angle_flag = (spider_angle_flag + 1) % 3;
	}

	// wolf's movement
	wolf_rotate= (timestamp_scene % 360);
	wolf_move[0] += 1.0f;

	if (wolf_move[0] > 500.0f) {
		wolf_move[0] = 100.0f;
	}

	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);
	glm::vec4 position_MC = ViewMatrix * glm::vec4(tiger_move[0], tiger_move[1]+400.0f, tiger_move[2],1.0f);
	glUniform4fv(loc_light[3].position, 1, &position_MC[0]);
	glUseProgram(0);

	glutPostRedisplay();
	if (flag_tiger_animation)
		glutTimerFunc(10, timer_scene, 0);
}

void prepare_GS_shader(void) {
	int i;
	char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_GS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Gouraud.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Gouraud.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram_simple = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");

	h_ShaderProgram_GS = LoadShaders(shader_info_GS);
	loc_ModelViewProjectionMatrix_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewMatrixInvTrans");

	loc_global_ambient_color = glGetUniformLocation(h_ShaderProgram_GS, "u_global_ambient_color");
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[i].light_on = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[i].position = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[i].ambient_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[i].specular_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[i].spot_direction = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_GS, string);
	}

	loc_material.ambient_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.ambient_color");
	loc_material.diffuse_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.diffuse_color");
	loc_material.specular_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.specular_color");
	loc_material.emissive_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.emissive_color");
	loc_material.specular_exponent = glGetUniformLocation(h_ShaderProgram_GS, "u_material.specular_exponent");
	loc_texture = glGetUniformLocation(h_ShaderProgram_GS, "u_base_texture");

	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_GS, "u_flag_texture_mapping");
	loc_flag_fog = glGetUniformLocation(h_ShaderProgram_GS, "u_flag_fog");
}

void prepare_shader_program(void) {
	int i;
	char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_TXPS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong_Tx.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Phong_Tx.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram_simple = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");

	h_ShaderProgram_TXPS = LoadShaders(shader_info_TXPS);
	loc_ModelViewProjectionMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrixInvTrans");


	loc_global_ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_global_ambient_color");
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[i].light_on = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[i].position = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[i].ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[i].specular_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[i].spot_direction = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_TXPS, string);
	}
	loc_material.ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.ambient_color");
	loc_material.diffuse_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.diffuse_color");
	loc_material.specular_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.specular_color");
	loc_material.emissive_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.emissive_color");
	loc_material.specular_exponent = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.specular_exponent");
	loc_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");

	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_mapping");
	loc_flag_fog = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_fog");

}

void keyboard(unsigned char key, int x, int y) {
	static int flag_cull_face = 0;
	static int PRP_distance_level = 4;

	glm::vec4 position_EC;
	glm::vec3 direction_EC;

	if ((key >= '0') && (key <= '0' + NUMBER_OF_LIGHT_SUPPORTED - 1)) {
		int light_ID = (int)(key - '0');

		if (shader_flag == 0)
			glUseProgram(h_ShaderProgram_TXPS);
		else
			glUseProgram(h_ShaderProgram_GS);
		light[light_ID].light_on = 1 - light[light_ID].light_on;
		glUniform1i(loc_light[light_ID].light_on, light[light_ID].light_on);
		glUseProgram(0);

		glutPostRedisplay();
		return;
	}

	switch (key) {
	case 't':
		if (T_pressed == 0)
			T_pressed = 1;
		else
			T_pressed = 0;
		glutPostRedisplay();
		break;
	case 'i':
		if (zoom_angle < 30)
			break;
		zoom_angle -= 2.0f;
		glutPostRedisplay();
		break;
	case 'o':
		if (zoom_angle >80)
			break;
		zoom_angle += 2.0f;
		glutPostRedisplay();
		break;
	case 'x':
		if (X_pressed == 0) {
			X_pressed = 1;
			Y_pressed = 0;
			Z_pressed = 0;
		}
		else
			X_pressed = 0;
		break;
	case 'y':
		if (Y_pressed == 0) {
			X_pressed = 0;
			Y_pressed = 1;
			Z_pressed = 0;
		}
		else
			Y_pressed = 0;
		break;
	case 'z':
		if (Z_pressed == 0) {
			X_pressed = 0;
			Y_pressed = 0;
			Z_pressed = 1;
		}
		else
			Z_pressed = 0;
		break;
	case 'q':
		if (shader_flag == 0) {
			shader_flag = 1;
		}
		else {
			shader_flag = 0;
		}
		initialize_renderer();
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	}
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (X_pressed == 1) {
			/*camera_move[0] += 10;
			camera_dir[0] += 10;*/
			camera.pos[0] += 10.0f *  (camera.uaxis[0]);
			camera.pos[1] += 10.0f *  (camera.uaxis[1]);
			camera.pos[2] += 10.0f *  (camera.uaxis[2]);
		}
		else if (Y_pressed == 1) {
			/*amera_move[1] += 10;
			camera_dir[1] += 10;*/
			camera.pos[0] += 10.0f *  (camera.vaxis[0]);
			camera.pos[1] += 10.0f *  (camera.vaxis[1]);
			camera.pos[2] += 10.0f *  (camera.vaxis[2]);
		}
		else if (Z_pressed == 1) {
			/*camera_move[2] += 10;
			camera_dir[2] += 10;*/
			camera.pos[0] += 10.0f *  (-camera.naxis[0]);
			camera.pos[1] += 10.0f *  (-camera.naxis[1]);
			camera.pos[2] += 10.0f *  (-camera.naxis[2]);
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		if (X_pressed == 1) {
			/*camera_move[0] += 10;
			camera_dir[0] += 10;*/
			camera.pos[0] -= 10.0f * (camera.uaxis[0]);
			camera.pos[1] -= 10.0f * (camera.uaxis[1]);
			camera.pos[2] -= 10.0f * (camera.uaxis[2]);
		}
		else if (Y_pressed == 1) {
			/*amera_move[1] += 10;
			camera_dir[1] += 10;*/
			camera.pos[0] -= 10.0f * (camera.vaxis[0]);
			camera.pos[1] -= 10.0f * (camera.vaxis[1]);
			camera.pos[2] -= 10.0f * (camera.vaxis[2]);
		}
		else if (Z_pressed == 1) {
			/*camera_move[2] += 10;
			camera_dir[2] += 10;*/
			camera.pos[0] -= 10.0f * (-camera.naxis[0]);
			camera.pos[1] -= 10.0f * (-camera.naxis[1]);
			camera.pos[2] -= 10.0f * (-camera.naxis[2]);
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		if (X_pressed == 1) {
			renew_cam_orientation_rotation_around_u_axis(5.0f);
		}
		else if (Y_pressed == 1) {
			renew_cam_orientation_rotation_around_v_axis(5.0f);
		}
		else if (Z_pressed == 1) {
			renew_cam_orientation_rotation_around_n_axis(5.0f);
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if (X_pressed == 1) {
			renew_cam_orientation_rotation_around_u_axis(-5.0f);
		}
		else if (Y_pressed == 1) {
			renew_cam_orientation_rotation_around_v_axis(-5.0f);
		}
		else if (Z_pressed == 1) {
			renew_cam_orientation_rotation_around_n_axis(-5.0f);
		}
		glutPostRedisplay();
		break;
	}
}

void reshape(int width, int height) {

	glViewport(0, 0, width, height);
	
	aspect_ratio = (float) width / height;
	ProjectionMatrix = glm::perspective(zoom_angle*TO_RADIAN, aspect_ratio, 100.0f, 20000.0f);

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO); 
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);

	glDeleteTextures(N_TEXTURES_USED, texture_names);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
	glutSpecialFunc(special);
}



void initialize_lights_and_material(void) { // follow OpenGL conventions for initialization
	int i;

	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);

	glUniform4f(loc_global_ambient_color, 0.115f, 0.115f, 0.115f, 1.0f);
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light[i].light_on, 0); // turn off all lights initially
		glUniform4f(loc_light[i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light[i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light[i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light[i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light[i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light[i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light[i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light[i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light[i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light[i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
	}
	//glUniform4f(loc_light[3].light_attenuation_factors, 1.0f, 0.09f, 0.031f, 0.0f);

	glUniform4f(loc_material.ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material.diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material.specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material.emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material.specular_exponent, 0.0f); // [0.0, 128.0]

	glUseProgram(0);
}

void initialize_flags(void) {
	flag_tiger_animation = 1;
	flag_polygon_fill = 1;
	flag_texture_mapping = 1;
	flag_fog = 0;

	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);
	glUniform1i(loc_flag_fog, flag_fog);
	glUniform1i(loc_flag_texture_mapping, flag_texture_mapping);
	glUseProgram(0);
}

void initialize_OpenGL(void) {

	glEnable(GL_MULTISAMPLE);


  	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//ViewMatrix = glm::lookAt(PRP_distance_scale[0] * glm::vec3(500.0f, 300.0f, 500.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ViewMatrix = glm::lookAt(4.0f/6.0f * glm::vec3(500.0f, 600.0f, 500.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	initialize_camera();
	initialize_lights_and_material();
	initialize_flags();

	glGenTextures(N_TEXTURES_USED, texture_names);
}

void set_up_scene_lights(void) {
	// point_light_EC: use light 0
	light[0].light_on = 1;
	light[0].position[0] = 0.0f; light[0].position[1] = 100.0f; 	// point light position in EC
	light[0].position[2] = 0.0f; light[0].position[3] = 1.0f;

	light[0].ambient_color[0] = 0.33f; light[0].ambient_color[1] = 0.33f;
	light[0].ambient_color[2] = 0.33f; light[0].ambient_color[3] = 1.0f;

	light[0].diffuse_color[0] = 0.5f; light[0].diffuse_color[1] = 0.5f;
	light[0].diffuse_color[2] = 0.5f; light[0].diffuse_color[3] = 1.5f;

	light[0].specular_color[0] = 0.8f; light[0].specular_color[1] = 0.8f;
	light[0].specular_color[2] = 0.8f; light[0].specular_color[3] = 1.0f;

	// spot_light_WC: use light 1
	light[1].light_on = 1;
	light[1].position[0] = 100.0f; light[1].position[1] = 500.0f; // spot light position in WC
	light[1].position[2] = 100.0f; light[1].position[3] = 1.0f;

	light[1].ambient_color[0] = 0.05f; light[1].ambient_color[1] = 0.05f;
	light[1].ambient_color[2] = 0.0f; light[1].ambient_color[3] = 1.0f;

	light[1].diffuse_color[0] = 0.3f; light[1].diffuse_color[1] = 0.3f;
	light[1].diffuse_color[2] = 0.1f; light[1].diffuse_color[3] = 1.0f;

	light[1].specular_color[0] = 0.5f; light[1].specular_color[1] = 0.5f;
	light[1].specular_color[2] = 0.04f; light[1].specular_color[3] = 1.0f;

	light[1].spot_direction[0] = 0.0f; light[1].spot_direction[1] = -1.0f; // spot light direction in WC
	light[1].spot_direction[2] = 0.0f;
	light[1].spot_cutoff_angle = 15.0f;
	light[1].spot_exponent = 45.0f;

	// light 2
	light[2].light_on = 0;
	light[2].position[0] = camera_move[0]; light[2].position[1] = camera_move[1]; // spot light position in WC
	light[2].position[2] = camera_move[2]; light[2].position[3] = 1.0f;

	light[2].ambient_color[0] = 0.05f; light[2].ambient_color[1] = 0.05f;
	light[2].ambient_color[2] = 0.0f; light[2].ambient_color[3] = 1.0f;

	light[2].diffuse_color[0] = 0.1f; light[2].diffuse_color[1] = 0.1f;
	light[2].diffuse_color[2] = 0.3f; light[2].diffuse_color[3] = 1.0f;

	light[2].specular_color[0] = 0.04f; light[2].specular_color[1] = 0.04f;
	light[2].specular_color[2] = 0.5f; light[2].specular_color[3] = 1.0f;

	light[2].spot_direction[0] =camera_dir[0]-camera_move[0]; light[2].spot_direction[1] = camera_dir[1]-camera_move[1]; // spot light direction in WC
	light[2].spot_direction[2] =camera_dir[2]-camera_move[2];
	light[2].spot_cutoff_angle = 10.0f;
	light[2].spot_exponent = 45.0f;

	// light 3
	light[3].light_on = 0;
	light[3].position[0] = tiger_move[0]; light[3].position[1] = 500.0f; // spot light position in WC
	light[3].position[2] = tiger_move[2]; light[3].position[3] = 1.0f;

	light[3].ambient_color[0] = 0.751f; light[3].ambient_color[1] = 0.200f;
	light[3].ambient_color[2] = 0.221f; light[3].ambient_color[3] = 1.0f;

	light[3].diffuse_color[0] = 0.700f; light[3].diffuse_color[1] = 0.041f;
	light[3].diffuse_color[2] = 0.041f; light[3].diffuse_color[3] = 1.0f;

	light[3].specular_color[0] = 0.727f; light[3].specular_color[1] = 0.626f;
	light[3].specular_color[2] = 0.626f; light[3].specular_color[3] = 1.0f;

	light[3].spot_direction[0] = 0.0f; light[3].spot_direction[1] = -1.0f; // spot light direction in WC
	light[3].spot_direction[2] = 0.0f;
	light[3].spot_cutoff_angle = 10.0f;
	light[3].spot_exponent = 30.0f;

	if (shader_flag == 0)
		glUseProgram(h_ShaderProgram_TXPS);
	else
		glUseProgram(h_ShaderProgram_GS);
	glUniform1i(loc_light[0].light_on, light[0].light_on);
	glUniform4fv(loc_light[0].position, 1, light[0].position);
	glUniform4fv(loc_light[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light[0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light[1].light_on, light[1].light_on);
	// need to supply position in EC for shading
	glm::vec4 position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
												light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[1].position, 1, &position_EC[0]); 
	glUniform4fv(loc_light[1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[1].specular_color, 1, light[1].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0], light[1].spot_direction[1], 
																light[1].spot_direction[2]);
	glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]); 
	glUniform1f(loc_light[1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[1].spot_exponent, light[1].spot_exponent);

	//light 2
	glUniform1i(loc_light[2].light_on, light[2].light_on);
	glm::vec4 position_EC4 = ViewMatrix * glm::vec4(light[2].position[0], light[2].position[1], light[2].position[2], light[2].position[3]);
	glUniform4fv(loc_light[2].position, 1, &position_EC4[0]);
	glUniform4fv(loc_light[2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[2].specular_color, 1, light[2].specular_color);
	glm::vec3 direction_EC4 = glm::mat3(ViewMatrix) * glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1],
		light[2].spot_direction[2]);
	glUniform3fv(loc_light[2].spot_direction, 1, &direction_EC4[0]);
	glUniform1f(loc_light[2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[2].spot_exponent, light[2].spot_exponent);

	// light 3
	glUniform1i(loc_light[3].light_on, light[3].light_on);
	glm::vec4 position_EC3 = ViewMatrix * glm::vec4(light[3].position[0], light[3].position[1], light[3].position[2], light[3].position[3]);
	glUniform4fv(loc_light[3].position, 1, &position_EC3[0]);
	glUniform4fv(loc_light[3].ambient_color, 1, light[3].ambient_color);
	glUniform4fv(loc_light[3].diffuse_color, 1, light[3].diffuse_color);
	glUniform4fv(loc_light[3].specular_color, 1, light[3].specular_color);
	glm::vec3 direction_EC3 = glm::mat3(ViewMatrix) * glm::vec3(light[3].spot_direction[0], light[3].spot_direction[1],
		light[3].spot_direction[2]);
	glUniform3fv(loc_light[3].spot_direction, 1, &direction_EC3[0]);
	glUniform1f(loc_light[3].spot_cutoff_angle, light[3].spot_cutoff_angle);
	glUniform1f(loc_light[3].spot_exponent, light[3].spot_exponent);

	glUseProgram(0);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_floor();
	prepare_tiger();
	prepare_wolf();
	prepare_spider();
	prepare_dragon();
	prepare_cow();
	prepare_tank();
	set_up_scene_lights();
}

void initialize_renderer(void) {
	register_callbacks();
	if (shader_flag == 0)
		prepare_shader_program();
	else
		prepare_GS_shader();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 3D Objects";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0', '1', 'a', 't', 'f', 'c', 'd', 'y', 'u', 'i', 'o', 'ESC'"  };

	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 700);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}