#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

														  // include glm/*.hpp only if necessary
														  //#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 0, win_height = 0;
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;

GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.25f; axes[1][0] = win_width / 2.25f;
	axes[2][1] = -win_height / 2.25f;
	axes[3][1] = win_height / 2.25f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

GLfloat line[2][2];
GLfloat line_color[3] = { 1.0f, 0.0f, 0.0f };
GLuint VBO_line, VAO_line;

void prepare_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height;
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f;
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_line);
	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height;
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f;
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_line(void) { // Draw line in its MC.
					   // y = x - win_height/4
	glUniform3fv(loc_primitive_color, 1, line_color);
	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 },{ -20.0, 15.0 },{ -20.0, 20.0 },{ 0.0, 23.0 },{ 20.0, 20.0 },{ 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 },{ -11.0, -12.0 },{ -12.0, -7.0 },{ 0.0, -10.0 },{ 12.0, -7.0 },{ 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 },{ -6.0, 0.0 },{ -6.0, 22.0 },{ 6.0, 22.0 },{ 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 },{ -7.0, 24.0 },{ -7.0, 21.0 },{ 7.0, 21.0 },{ 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 },{ -18.0, 3.0 },{ -16.0, 10.0 },{ -18.0, 20.0 },{ -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 },{ 18.0, 3.0 },{ 16.0, 10.0 },{ 18.0, 20.0 },{ 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

int airplane_clock = 0;
float airplane_s_factor = 1.0f;
#define AIRPLANE_ROTATION_RADIUS 200.0f

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2) + sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}

//shirt
#define SHIRT_LEFT_BODY 0
#define SHIRT_RIGHT_BODY 1
#define SHIRT_LEFT_COLLAR 2
#define SHIRT_RIGHT_COLLAR 3
#define SHIRT_FRONT_POCKET 4
#define SHIRT_BUTTON1 5
#define SHIRT_BUTTON2 6
#define SHIRT_BUTTON3 7
#define SHIRT_BUTTON4 8
GLfloat left_body[6][2] = { { 0.0, -9.0 },{ -8.0, -9.0 },{ -11.0, 8.0 },{ -6.0, 10.0 },{ -3.0, 7.0 },{ 0.0, 9.0 } };
GLfloat right_body[6][2] = { { 0.0, -9.0 },{ 0.0, 9.0 },{ 3.0, 7.0 },{ 6.0, 10.0 },{ 11.0, 8.0 },{ 8.0, -9.0 } };
GLfloat left_collar[4][2] = { { 0.0, 9.0 },{ -3.0, 7.0 },{ -6.0, 10.0 },{ -4.0, 11.0 } };
GLfloat right_collar[4][2] = { { 0.0, 9.0 },{ 4.0, 11.0 },{ 6.0, 10.0 },{ 3.0, 7.0 } };
GLfloat front_pocket[6][2] = { { 5.0, 0.0 },{ 4.0, 1.0 },{ 4.0, 3.0 },{ 7.0, 3.0 },{ 7.0, 1.0 },{ 6.0, 0.0 } };
GLfloat button1[3][2] = { { -1.0, 6.0 },{ 1.0, 6.0 },{ 0.0, 5.0 } };
GLfloat button2[3][2] = { { -1.0, 3.0 },{ 1.0, 3.0 },{ 0.0, 2.0 } };
GLfloat button3[3][2] = { { -1.0, 0.0 },{ 1.0, 0.0 },{ 0.0, -1.0 } };
GLfloat button4[3][2] = { { -1.0, -3.0 },{ 1.0, -3.0 },{ 0.0, -4.0 } };

GLfloat shirt_color[9][3] = {
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f }
};

GLuint VBO_shirt, VAO_shirt;
void prepare_shirt() {
	GLsizeiptr buffer_size = sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1) + sizeof(button2) + sizeof(button3) + sizeof(button4);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_shirt);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(left_body), left_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body), sizeof(right_body), right_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body), sizeof(left_collar), left_collar);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar), sizeof(right_collar), right_collar);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar),
		sizeof(front_pocket), front_pocket);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket), sizeof(button1), button1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1), sizeof(button2), button2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1) + sizeof(button2), sizeof(button3), button3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1) + sizeof(button2) + sizeof(button3), sizeof(button4), button4);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_shirt);
	glBindVertexArray(VAO_shirt);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_shirt() {
	glBindVertexArray(VAO_shirt);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_LEFT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_RIGHT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_LEFT_COLLAR]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_RIGHT_COLLAR]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_FRONT_POCKET]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON1]);
	glDrawArrays(GL_TRIANGLE_FAN, 26, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON2]);
	glDrawArrays(GL_TRIANGLE_FAN, 29, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON3]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON4]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);
	glBindVertexArray(0);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

//car
#define CAR_BODY 0
#define CAR_FRAME 1
#define CAR_WINDOW 2
#define CAR_LEFT_LIGHT 3
#define CAR_RIGHT_LIGHT 4
#define CAR_LEFT_WHEEL 5
#define CAR_RIGHT_WHEEL 6

GLfloat car_body[4][2] = { { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } };
GLfloat car_frame[4][2] = { { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } };
GLfloat car_window[4][2] = { { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } };
GLfloat car_left_light[4][2] = { { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } };
GLfloat car_right_light[4][2] = { { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } };
GLfloat car_left_wheel[4][2] = { { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } };
GLfloat car_right_wheel[4][2] = { { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } };

GLfloat car_color[7][3] = {
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f }
};

GLuint VBO_car, VAO_car;
void prepare_car() {
	GLsizeiptr buffer_size = sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel) + sizeof(car_right_wheel);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car_body), car_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body), sizeof(car_frame), car_frame);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame), sizeof(car_window), car_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window), sizeof(car_left_light), car_left_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light),
		sizeof(car_right_light), car_right_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light), sizeof(car_left_wheel), car_left_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel), sizeof(car_right_wheel), car_right_wheel);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car);
	glBindVertexArray(VAO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car() {
	glBindVertexArray(VAO_car);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_FRAME]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glBindVertexArray(0);
}

//draw cocktail
#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

GLfloat neck[6][2] = { { -6.0, -12.0 },{ -6.0, -11.0 },{ -1.0, 0.0 },{ 1.0, 0.0 },{ 6.0, -11.0 },{ 6.0, -12.0 } };
GLfloat liquid[6][2] = { { -1.0, 0.0 },{ -9.0, 4.0 },{ -12.0, 7.0 },{ 12.0, 7.0 },{ 9.0, 4.0 },{ 1.0, 0.0 } };
GLfloat remain[4][2] = { { -12.0, 7.0 },{ -12.0, 10.0 },{ 12.0, 10.0 },{ 12.0, 7.0 } };
GLfloat straw[4][2] = { { 7.0, 7.0 },{ 12.0, 12.0 },{ 14.0, 12.0 },{ 9.0, 7.0 } };
GLfloat deco[8][2] = { { 12.0, 12.0 },{ 10.0, 14.0 },{ 10.0, 16.0 },{ 12.0, 18.0 },{ 14.0, 18.0 },{ 16.0, 16.0 },{ 16.0, 14.0 },{ 14.0, 12.0 } };

GLfloat cocktail_color[5][3] = {
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
	{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f }
};

GLuint VBO_cocktail, VAO_cocktail;
void prepare_cocktail() {
	GLsizeiptr buffer_size = sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw)
		+ sizeof(deco);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(neck), neck);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck), sizeof(liquid), liquid);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid), sizeof(remain), remain);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain), sizeof(straw), straw);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw),
		sizeof(deco), deco);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cocktail);
	glBindVertexArray(VAO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cocktail() {
	glBindVertexArray(VAO_cocktail);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_NECK]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_LIQUID]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_REMAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_STRAW]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_DECO]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);

	glBindVertexArray(0);
}

//draw car2
#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

GLfloat car2_body[8][2] = { { -18.0, -7.0 },{ -18.0, 0.0 },{ -13.0, 0.0 },{ -10.0, 8.0 },{ 10.0, 8.0 },{ 13.0, 0.0 },{ 18.0, 0.0 },{ 18.0, -7.0 } };
GLfloat car2_front_window[4][2] = { { -10.0, 0.0 },{ -8.0, 6.0 },{ -2.0, 6.0 },{ -2.0, 0.0 } };
GLfloat car2_back_window[4][2] = { { 0.0, 0.0 },{ 0.0, 6.0 },{ 8.0, 6.0 },{ 10.0, 0.0 } };
GLfloat car2_front_wheel[8][2] = { { -11.0, -11.0 },{ -13.0, -8.0 },{ -13.0, -7.0 },{ -11.0, -4.0 },{ -7.0, -4.0 },{ -5.0, -7.0 },{ -5.0, -8.0 },{ -7.0, -11.0 } };
GLfloat car2_back_wheel[8][2] = { { 7.0, -11.0 },{ 5.0, -8.0 },{ 5.0, -7.0 },{ 7.0, -4.0 },{ 11.0, -4.0 },{ 13.0, -7.0 },{ 13.0, -8.0 },{ 11.0, -11.0 } };
GLfloat car2_light1[3][2] = { { -18.0, -1.0 },{ -17.0, -2.0 },{ -18.0, -3.0 } };
GLfloat car2_light2[3][2] = { { -18.0, -4.0 },{ -17.0, -5.0 },{ -18.0, -6.0 } };

GLfloat car2_color[7][3] = {
	{ 100 / 255.0f, 141 / 255.0f, 159 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f }
};

GLuint VBO_car2, VAO_car2;
void prepare_car2() {
	GLsizeiptr buffer_size = sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1) + sizeof(car2_light2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car2_body), car2_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body), sizeof(car2_front_window), car2_front_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window), sizeof(car2_back_window), car2_back_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window), sizeof(car2_front_wheel), car2_front_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel),
		sizeof(car2_back_wheel), car2_back_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel), sizeof(car2_light1), car2_light1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1), sizeof(car2_light2), car2_light2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car2);
	glBindVertexArray(VAO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car2() {
	glBindVertexArray(VAO_car2);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);

	glBindVertexArray(0);
}

// hat
#define HAT_LEAF 0
#define HAT_BODY 1
#define HAT_STRIP 2
#define HAT_BOTTOM 3

GLfloat hat_leaf[4][2] = { { 3.0, 20.0 },{ 3.0, 28.0 },{ 9.0, 32.0 },{ 9.0, 24.0 } };
GLfloat hat_body[4][2] = { { -19.5, 2.0 },{ 19.5, 2.0 },{ 15.0, 20.0 },{ -15.0, 20.0 } };
GLfloat hat_strip[4][2] = { { -20.0, 0.0 },{ 20.0, 0.0 },{ 19.5, 2.0 },{ -19.5, 2.0 } };
GLfloat hat_bottom[4][2] = { { 25.0, 0.0 },{ -25.0, 0.0 },{ -25.0, -4.0 },{ 25.0, -4.0 } };

GLfloat hat_color[4][3] = {
	{ 167 / 255.0f, 255 / 255.0f, 55 / 255.0f },
{ 255 / 255.0f, 144 / 255.0f, 32 / 255.0f },
{ 255 / 255.0f, 40 / 255.0f, 33 / 255.0f },
{ 255 / 255.0f, 144 / 255.0f, 32 / 255.0f }
};

GLuint VBO_hat, VAO_hat;

void prepare_hat() {
	GLsizeiptr buffer_size = sizeof(hat_leaf) + sizeof(hat_body) + sizeof(hat_strip) + sizeof(hat_bottom);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_hat);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_hat);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(hat_leaf), hat_leaf);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hat_leaf), sizeof(hat_body), hat_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hat_leaf) + sizeof(hat_body), sizeof(hat_strip), hat_strip);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hat_leaf) + sizeof(hat_body) + sizeof(hat_strip), sizeof(hat_bottom), hat_bottom);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_hat);
	glBindVertexArray(VAO_hat);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_hat);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_hat() {
	glBindVertexArray(VAO_hat);

	glUniform3fv(loc_primitive_color, 1, hat_color[HAT_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, hat_color[HAT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, hat_color[HAT_STRIP]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, hat_color[HAT_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glBindVertexArray(0);
}

// cake
#define CAKE_FIRE 0
#define CAKE_CANDLE 1
#define CAKE_BODY 2
#define CAKE_BOTTOM 3
#define CAKE_DECORATE 4

GLfloat cake_fire[4][2] = { { -0.5, 14.0 },{ -0.5, 13.0 },{ 0.5, 13.0 },{ 0.5, 14.0 } };
GLfloat cake_candle[4][2] = { { -1.0, 8.0 } ,{ -1.0, 13.0 },{ 1.0, 13.0 },{ 1.0, 8.0 } };
GLfloat cake_body[4][2] = { { 8.0, 5.0 },{ -8.0, 5.0 } ,{ -8.0, 8.0 },{ 8.0, 8.0 } };
GLfloat cake_bottom[4][2] = { { -10.0, 1.0 },{ -10.0, 5.0 },{ 10.0, 5.0 },{ 10.0, 1.0 } };
GLfloat cake_decorate[4][2] = { { -10.0, 0.0 },{ -10.0, 1.0 },{ 10.0, 1.0 },{ 10.0, 0.0 } };

GLfloat cake_color[5][3] = {
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 102 / 255.0f, 51 / 255.0f, 0 / 255.0f }
};

GLuint VBO_cake, VAO_cake;

void prepare_cake() {
	int size = sizeof(cake_fire);
	GLsizeiptr buffer_size = sizeof(cake_fire) * 5;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, cake_fire);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, cake_candle);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, cake_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, cake_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, cake_decorate);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cake);
	glBindVertexArray(VAO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cake() {
	glBindVertexArray(VAO_cake);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_FIRE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_CANDLE]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_DECORATE]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glBindVertexArray(0);
}

// sword

#define SWORD_BODY 0
#define SWORD_BODY2 1
#define SWORD_HEAD 2
#define SWORD_HEAD2 3
#define SWORD_IN 4
#define SWORD_DOWN 5
#define SWORD_BODY_IN 6

GLfloat sword_body[4][2] = { { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } };
GLfloat sword_body2[4][2] = { { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } };
GLfloat sword_head[4][2] = { { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } };
GLfloat sword_head2[3][2] = { { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } };
GLfloat sword_in[4][2] = { { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } };
GLfloat sword_down[4][2] = { { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } };
GLfloat sword_body_in[4][2] = { { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } };

GLfloat sword_color[7][3] = {
	{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sword, VAO_sword;

void prepare_sword() {
	GLsizeiptr buffer_size = sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down) + sizeof(sword_body_in);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sword_body), sword_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body), sizeof(sword_body2), sword_body2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2), sizeof(sword_head), sword_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head), sizeof(sword_head2), sword_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2), sizeof(sword_in), sword_in);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in), sizeof(sword_down), sword_down);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down), sizeof(sword_body_in), sword_body_in);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sword);
	glBindVertexArray(VAO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sword() {
	glBindVertexArray(VAO_sword);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_DOWN]);
	glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 23, 4);

	glBindVertexArray(0);
}

//======================= my own objects====================================

// fragment

GLfloat fragment[4][2] = { {2.0,3.0},{2.0,-3.0},{-2.0,-3.0}, {-2.0,3.0} };
GLfloat fragment_color[3] = { 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };

GLuint VBO_fragment, VAO_fragment;

void prepare_fragment() {
	GLsizeiptr buffer_size = sizeof(fragment);

	glGenBuffers(1, &VBO_fragment);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_fragment);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(fragment), fragment);

	glGenVertexArrays(1, &VAO_fragment);
	glBindVertexArray(VAO_fragment);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_fragment);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_fragment() {
	glBindVertexArray(VAO_fragment);

	glUniform3fv(loc_primitive_color, 1, fragment_color);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}

// mushroom

#define MUSHROOM_HAT 0
#define MUSHROOM_BODY 1
#define MUSHROOM_EYE1 2
#define MUSHROOM_EYE2 3
#define MUSHROOM_MOUSE 4
#define MUSHROOM_FOOT1 5
#define MUSHROOM_FOOT2 6
#define MUSHROOM_PATTERN1 7
#define MUSHROOM_PATTERN2 8

GLfloat mushroom_hat[5][2] = { {-14.0, 8.0 },{-14.0, 12.0},{0.0, 19.0},{14.0, 12.0},{14.0, 8.0} };
GLfloat mushroom_body[4][2] = { {-10.0,8.0},{10.0,8.0},{10.0,-8.0},{-10.0,-8.0} };
GLfloat mushroom_eye1[4][2] = { {2.0,2.0},{4.0,2.0},{4.0,6.0},{2.0,6.0} };
GLfloat mushroom_eye2[4][2] = { {-2.0,2.0},{-4.0,2.0},{-4.0,6.0},{-2.0,6.0} };
GLfloat mushroom_mouse[3][2] = { {-2.0,1.0},{0.0,-3.0},{2.0,1.0} };
GLfloat mushroom_foot1[6][2] = { {4.0,-12.0},{14.0,-12.0},{14.0,-10.0},{12.0,-6.0},{6.0,-6.0},{4.0,-10.0} };
GLfloat mushroom_foot2[6][2] = { {-4.0,-12.0},{-14.0,-12.0},{-14.0,-10.0},{-12.0,-6.0},{-6.0,-6.0},{-4.0,-10.0} };
GLfloat mushroom_pattern1[4][2] = { {6.0,16.0},{6.0,12.0},{10.0,12.0},{10.0,14.0} };
GLfloat mushroom_pattern2[4][2] = { {-8.0,11.0},{-8.0,8.0},{-4.0,8.0},{-4.0,11.0} };

GLfloat mushroom_color[9][3] = {
	{ 216 / 255.0f, 85 / 255.0f, 43 / 255.0f},
{ 244 / 255.0f, 232 / 255.0f, 221 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 216 / 255.0f, 85 / 255.0f, 43 / 255.0f },
{ 216 / 255.0f, 85 / 255.0f, 43 / 255.0f},
{ 255 / 255.0f, 255 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 255 / 255.0f, 0 / 255.0f }
};

GLuint VBO_mushroom, VAO_mushroom;

void prepare_mushroom() {
	GLsizeiptr buffer_size = sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1) + sizeof(mushroom_eye2) + sizeof(mushroom_mouse) + sizeof(mushroom_foot1) + sizeof(mushroom_foot2) + sizeof(mushroom_pattern1) + sizeof(mushroom_pattern2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_mushroom);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_mushroom);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mushroom_hat), mushroom_hat);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat), sizeof(mushroom_body), mushroom_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body), sizeof(mushroom_eye1), mushroom_eye1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1), sizeof(mushroom_eye2), mushroom_eye2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1) + sizeof(mushroom_eye2), sizeof(mushroom_mouse), mushroom_mouse);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1) + sizeof(mushroom_eye2) + sizeof(mushroom_mouse), sizeof(mushroom_foot1), mushroom_foot1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1) + sizeof(mushroom_eye2) + sizeof(mushroom_mouse) + sizeof(mushroom_foot1), sizeof(mushroom_foot2), mushroom_foot2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1) + sizeof(mushroom_eye2) + sizeof(mushroom_mouse) + sizeof(mushroom_foot1) + sizeof(mushroom_foot2), sizeof(mushroom_pattern1), mushroom_pattern1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mushroom_hat) + sizeof(mushroom_body) + sizeof(mushroom_eye1) + sizeof(mushroom_eye2) + sizeof(mushroom_mouse) + sizeof(mushroom_foot1) + sizeof(mushroom_foot2) + sizeof(mushroom_pattern1), sizeof(mushroom_pattern2), mushroom_pattern2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_mushroom);
	glBindVertexArray(VAO_mushroom);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_mushroom);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_mushroom() {
	glBindVertexArray(VAO_mushroom);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_HAT]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 5, 4);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_EYE1]);
	glDrawArrays(GL_TRIANGLE_FAN, 9, 4);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_EYE2]);
	glDrawArrays(GL_TRIANGLE_FAN, 13, 4);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_MOUSE]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 3);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_FOOT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 6);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_FOOT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 26, 6);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_PATTERN1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 4);

	glUniform3fv(loc_primitive_color, 1, mushroom_color[MUSHROOM_PATTERN2]);
	glDrawArrays(GL_TRIANGLE_FAN, 36, 4);

	glBindVertexArray(0);
}
