// 20171666 이예은 HW3

// 2D 물체 정의 부분은 objects.h 파일로 분리
// 새로운 물체 추가 시 prepare_scene() 함수에서 해당 물체에 대한 prepare_***() 함수를 수행함.
// (필수는 아니나 올바른 코딩을 위하여) cleanup() 함수에서 해당 resource를 free 시킴.*/
#include "objects.h"

int sword_flag = 0; // if you use sword
int pre_input = 0;
int house_flag = 1;
int hat_flag = 0;
int hat_rotate_flag = 1;
int bounce_flag = 0;
int bounce_time = 0;
int frag_flag = 0;
int frag_time = 0;
int mushroom_flag = 0;
float house_x[2] = { 0, };
float house_y[2] = { 0, };
float house_scale = 2.0f;
float sword_x = 0, sword_y = 0;
float hat_x = 0, hat_y = 0;
float plane_x = (float)(win_width / 2 * 0.975);
float plane_y = (float)(win_height / 2 * 0.95);
float plane_rotate;
float plane_scale = 1.7f;
float car_x = -(float)(win_width / 2 * 0.975) + 200;
float car_y = -(float)(win_height / 2 * 0.95);
float car_rotate;
float car_scale = 1.5f;
float pre_hat_rotate;
float hat_rotate = 0;
float sword_rotate;
float pre_x, pre_y;
float frag_x[5] = { 0, };
float frag_y[5] = { 0, };
float mushroom_x = (float)(win_width / 2 * 0.975) - 200;
float mushroom_y = -(float)(win_height / 2 * 0.95) + 200;
float mushroom_rotate = 0;
float mushroom_line= mushroom_y;
float mushroom_pre_rotate;
float mushroom_pre_x = mushroom_x;

unsigned int timestamp = 0;
void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	glutPostRedisplay();


	//===============================hat move=================================
	if (hat_flag == 1) {
		if (hat_rotate - pre_hat_rotate > 30) {
			hat_rotate_flag = -1;
		}
		hat_rotate += 5.0f * hat_rotate_flag;

		if (hat_rotate == pre_hat_rotate) {
			hat_flag = 0;
		}
	}

	//=================================sword move===============================
	if (sword_flag == 1) {
		switch ((int)sword_rotate) {
		case 270:
			sword_x -= 6.0f;
			break;
		case 90:
			sword_x += 6.0f;
			break;
		case 0:
			sword_y -= 6.0f;
			break;
		case 180:
			sword_y += 6.0f;
			break;
		}

		if (sword_x > (win_width/2)*0.975 || sword_x<-(win_width/2)*0.975 || sword_y>(win_height/2)*0.95 || sword_y < -(win_height/2)*0.95) {
			sword_flag = 0;
			sword_x = (float)(win_width / 2 * 0.975) - 1;
			sword_y = (float)(win_height / 2 * 0.95) - 1;
		}

		// meet plane
		if (abs(sword_x - plane_x) < plane_scale*20 && abs(sword_y - plane_y) < plane_scale*20) {
			sword_flag = 0;
			frag_flag = 1;
			pre_x = plane_x;
			pre_y = plane_y;
			frag_time = 0;
			sword_x = (float)(win_width / 2 * 0.975) - 1;
			sword_y = (float)(win_height / 2 * 0.95) - 1;
			plane_x = (float)(win_width / 2 * 0.975);
			plane_y = (float)(win_height / 2 * 0.95);
		}

		// meet car
		if (abs(sword_x - car_x) < car_scale*15 && abs(sword_y - car_y) < car_scale*15) {
			sword_flag = 0;
			frag_flag = 1;
			pre_x = car_x;
			pre_y = car_y;
			frag_time = 0;
			sword_x = (float)(win_width / 2 * 0.975) - 1;
			sword_y = (float)(win_height / 2 * 0.95) - 1;
			car_x = -(float)(win_width / 2 * 0.975) + 200;
			car_y = -(float)(win_height / 2 * 0.95);
			car_scale = 1.5f;
		}

		// meet mushroom
		if (abs(sword_x - mushroom_x) < 2.0*16 && abs(sword_y - mushroom_y) < 2.0*16) {
			sword_flag = 0;
			frag_flag = 1;
			pre_x = mushroom_x;
			pre_y = mushroom_y;
			frag_time = 0;
			sword_x = (float)(win_width / 2 * 0.975) - 1;
			sword_y = (float)(win_height / 2 * 0.95) - 1;
			mushroom_x = (float)(win_width / 2 * 0.975) - 200;
			mushroom_y = -(float)(win_height / 2 * 0.95) + 200;
			mushroom_line = -(float)(win_height / 2 * 0.95) + 200;
			mushroom_rotate = 0;
		}
		
		// meet house
		for (int i = 0; i < 2; i++) {
			if (abs(sword_x - house_x[i]) < house_scale * 15 && abs(sword_y - house_y[i]) < house_scale * 15) {
				sword_flag = 0;
				bounce_flag = 1;
				break;
			}
		}
	}

	if (bounce_flag > 0) {
		if (bounce_time < 60) {
			sword_rotate += 10.0f;
			bounce_time++;
		}
		else {
			bounce_time = 0;
			bounce_flag = 0;
			sword_x = (float)(win_width / 2 * 0.975) - 1;
			sword_y = (float)(win_height / 2 * 0.95) - 1;
		}
	}

	// ========================= plane move ===============================
	if (plane_x > (win_width / 2) * 0.975 || plane_x<-(win_width / 2) * 0.975 || plane_y>(win_height / 2) * 0.95 || plane_y < -(win_height / 2) * 0.95) {
		plane_x = (float)(win_width / 2 * 0.975);
		plane_y = (float)(win_height / 2 * 0.95);

	}
	if (timestamp % 360 > 160 && timestamp%360 < 200) {
		plane_rotate += 10.0f;
		if (timestamp % 90 < 45) {
			plane_x -= 3.0f;
			plane_y -= 1.5f;
		}
		else {
			plane_y -= 3.0f;
			plane_x -= 1.5f;
		}
	}
	else {
		if (timestamp % 90 < 45) {
			plane_x -= 3.0f;
			plane_y -= 1.5f;
			plane_rotate = 120;
		}
		else {
			plane_y -= 3.0f;
			plane_x -= 1.5f;
			plane_rotate = 150;
		}
	}

	//==========================car move====================================
	if (car_x > (win_width / 2) * 0.975 || car_x<-(win_width / 2) * 0.975 || car_y>(win_height / 2) * 0.95 || car_y < -(win_height / 2) * 0.95) {
		car_x = -(float)(win_width / 2 * 0.975) + 200;
		car_y = -(float)(win_height / 2 * 0.95);
		car_scale = 1.5f;
	}
	car_y += 1.2f;
	car_x = -(float)(win_width / 2 * 0.975) + 200 + sin(car_y/10)*50;
	car_scale += 0.01f;

	//==========================house move==================================
	if (timestamp % 360 == 180 || timestamp % 360 == 0) {
		house_x[0] = (float)(rand() % win_width - win_width / 2);
		house_y[0] = (float)(rand() % win_height - win_height / 2);
	}

	if (timestamp % 450 == 270 || timestamp % 360 == 120) {
		house_x[1] = (float)(rand() % win_width - win_width / 2);
		house_y[1] = (float)(rand() % win_height - win_height / 2);
	}

	if (house_scale > 6 && house_flag==1) {
		house_flag = -1;
	}
	else if (house_scale < 2 && house_flag == -1) {
		house_flag = 1;
	}
	house_scale += 0.2f * house_flag;

	//=========================fragment move==================================
	if (frag_flag == 1) {
		if (frag_time == 0) {
			for (int i = 0; i < 5; i++) {
				frag_x[i] = pre_x;
				frag_y[i] = pre_y;
			}
		}
		if (frag_time < 20) {
			int x_ratio = -2.0f;
			int y_ratio = 1.0f;

			for (int i = 0; i < 5; i++) {
				frag_x[i] += x_ratio;
				frag_y[i] -= y_ratio;
				x_ratio += 1.5f;
				if (i < 3) {
					y_ratio += 0.5f;
				}
				else {
					y_ratio -= 0.5f;
				}
			}

			x_ratio = -2.0f;
			y_ratio = 1.0f;
			frag_time++;
		}
		else {
			int x_ratio = -2.0f;
			int y_ratio = 1.0f;
			for (int i = 0; i < 5; i++) {
				frag_x[i] += x_ratio;
				frag_y[i] += y_ratio;
				x_ratio += 1.5f;
				if (i < 3) {
					y_ratio += 0.5f;
				}
				else {
					y_ratio -= 0.5f;
				}
			}
			x_ratio = -2.0f;
			y_ratio = 1.0f;
		}
		for (int i = 0; i < 5; i++) {
			if (frag_x[i] > (win_width / 2) * 0.975 || frag_x[i]<-(win_width / 2) * 0.975 || frag_y[i]>(win_height / 2) * 0.95 || frag_y[i] < -(win_height / 2) * 0.95) {
				frag_flag = 0;
				frag_time = 0;
				for (int j = 0; j < 5; j++) {
					frag_x[j] = (win_width / 2) * 0.975;
					frag_y[j] = (win_height / 2) * 0.95;
				}
				break;
			}
		}
	}

	//============================mushroom move==============================
	if (mushroom_x > (win_width / 2) * 0.975 || mushroom_x < -(win_width / 2) * 0.975) {
		mushroom_x = (float)(win_width / 2 * 0.975) - 200;
		mushroom_pre_x = mushroom_x;
		mushroom_line += 200;
		mushroom_rotate = 0;
	}

	if (mushroom_y > (win_height / 2) * 0.95 || mushroom_y < -(win_height / 2) * 0.95) {
		mushroom_x = (float)(win_width / 2 * 0.975) - 200;
		mushroom_pre_x = mushroom_x;
		mushroom_line = -(float)(win_height / 2 * 0.95) + 200;
		mushroom_y = mushroom_line;
		mushroom_rotate = 0;
	}

	if (timestamp % 180 == 90) {
		mushroom_pre_rotate = mushroom_rotate;
		mushroom_flag = 1;
	}

	if (mushroom_flag == 1) {
		if (mushroom_rotate - mushroom_pre_rotate == 180) {
			mushroom_flag = 0;
			if (mushroom_rotate >= 360) {
				mushroom_rotate = 0;
			}
		}
		else {
			mushroom_rotate += 5.0f;
		}
		mushroom_line = mushroom_y;
	}
	else {
		if (mushroom_rotate == 0) {
			mushroom_x -= 1.2f;
			mushroom_y = mushroom_line + sin(mushroom_x / 15) * 50-sin(mushroom_pre_x/15)*50;
			if (sin(mushroom_x / 15) * 50 - sin(mushroom_pre_x / 15) * 50 > 0) {
				mushroom_y = mushroom_line - sin(mushroom_x / 15) * 50 + sin(mushroom_pre_x / 15) * 50;
			}
		}
		else {
			mushroom_x -= 1.2f;
			mushroom_y = mushroom_line - sin(mushroom_x / 15) * 50 + sin(mushroom_pre_x / 15) * 50;
			if (sin(mushroom_x / 15) * 50 - sin(mushroom_pre_x / 15) * 50 > 0) {
				mushroom_y = mushroom_line + sin(mushroom_x / 15) * 50 - sin(mushroom_pre_x / 15) * 50;
			}
		}
	}
	

	glutTimerFunc(10, timer, 0);
}

void display(void) {
	glm::mat4 ModelMatrix;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-plane_x, -plane_y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, plane_rotate*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(plane_scale, plane_scale, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();
	
	for (int i = 0; i < 2; i++) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-house_x[i], -house_y[i], 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(house_scale, house_scale, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();
	}
	
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-car_x, -car_y, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(car_scale, car_scale, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-hat_x, -hat_y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix,hat_rotate*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_hat();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-sword_x, -sword_y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, sword_rotate * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_sword();

	for (int i = 0; i < 5; i++) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-frag_x[i], -frag_y[i], 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_fragment();
	}

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-mushroom_x, -mushroom_y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, mushroom_rotate * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_mushroom();

	glFlush();	
}   

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT: // 0
		if (pre_input == 0) {
			hat_x -= 8.0f;
		}
		else {
			hat_rotate = 0;
			pre_input = 0;
		}
		break;
	case GLUT_KEY_LEFT: // 1
		if (pre_input == 1) {
			hat_x += 8.0f;
		}
		else {
			hat_rotate = 180;
			pre_input = 1;
		}
		break;
	case GLUT_KEY_UP:  // 2
		if (pre_input == 2) {
			hat_y -= 8.0f;
		}
		else {
			hat_rotate = 90;
			pre_input = 2;
		}
		break;
	case GLUT_KEY_DOWN: // 3
		if (pre_input == 3) {
			hat_y += 8.0f;
		}
		else {
			hat_rotate = 270;
			pre_input = 3;
		}
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;

	case ' ':
		sword_x = hat_x;
		sword_y = hat_y;
		sword_rotate = hat_rotate - 90;
		if (sword_rotate < 0) {
			sword_rotate += 360;
		}
		sword_flag = 1;
		hat_flag = 1;
		hat_rotate_flag = 1;
		pre_hat_rotate = hat_rotate;
	}
}

int leftbuttonpressed = 0;
void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		leftbuttonpressed = 1;
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;
}

void motion(int x, int y) {
	if (leftbuttonpressed) {
		centerx =  x - win_width/2.0f, centery = (win_height - y) - win_height/2.0f;
		glutPostRedisplay();
	}
} 
	
void reshape(int width, int height) {
	win_width = width, win_height = height;
	
  	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0, 
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);

	glDeleteVertexArrays(1, &VAO_house);
	glDeleteBuffers(1, &VBO_house);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_line();
	prepare_airplane();
	prepare_house();
	prepare_car();
	prepare_hat();
	prepare_sword();
	prepare_fragment();
	prepare_mushroom();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program(); 
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

#define N_MESSAGE_LINES 2
int main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 Simple2DTransformationMotion_GLSL_3.0.3";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC'"
		"    - Mouse used: L-click and move"
	};

	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (1000, 650);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


