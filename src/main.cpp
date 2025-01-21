
#if 1

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <GL/glew.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#ifdef __linux__
#include <GL/glx.h>
#endif /* __linux__ */
#endif

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>


#include "pitchDetector.h"

#define WIDTH 800
#define HEIGHT 600

pitchDetector* pitch = NULL;

int window_size = 4096;


const int numNotes = 108;
const int numQuads = 43;

std::string note_text;


const float note_color[3] = { 1.f, 1.0f, 0.2f };
const float note_line_color[3] = { 1.0f, 0.0f, 0.0f };

int double_id = 0;
int note_offset_x2;
int note_offset_y2;

int showing_note = 0;
const int num_last_notes = 5;
int cur_note_id = 0;
int *last_notes;

int playing_notes[10];
double playing_error[10];

int num_showing_notes;
int showing_notes[10];
double showing_error[10];


std::string notes[] = {	"C0", "C#0/Db0", "D0", "D#0/Eb0", "E0", "F0", "F#0/Gb0", "G0", "G#0/Ab0", "A0", "A#0/Bb0", "B0",  // 11
						"C1", "C#1/Db1", "D1", "D#1/Eb1", "E1", "F1", "F#1/Gb1", "G1", "G#1/Ab1", "A1", "A#1/Bb1", "B1",  // 23
						"C2", "C#2/Db2", "D2", "D#2/Eb2", "E2", "F2", "F#2/Gb2", "G2", "G#2/Ab2", "A2", "A#2/Bb2", "B2",  // 35
						"C3", "C#3/Db3", "D3", "D#3/Eb3", "E3", "F3", "F#3/Gb3", "G3", "G#3/Ab3", "A3", "A#3/Bb3", "B3",  // 47
						"C4", "C#4/Db4", "D4", "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4",  // 59
						"C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5",  // 71
						"C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6", "G#6/Ab6", "A6", "A#6/Bb6", "B6",  // 83
						"C7", "C#7/Db7", "D7", "D#7/Eb7", "E7", "F7", "F#7/Gb7", "G7", "G#7/Ab7", "A7", "A#7/Bb7", "B7",  // 95
						"C8", "C#8/Db8", "D8", "D#8/Eb8", "E8", "F8", "F#8/Gb8", "G8", "G#8/Ab8", "A8", "A#8/Bb8", "B8" };// 107 



std::string notes__[] = {	"C0", "Db0", "D0", "Eb0", "E0", "F0", "Gb0", "G0", "Ab0", "A0", "Bb0", "B0",  // 11
							"C1", "Db1", "D1", "Eb1", "E1", "F1", "Gb1", "G1", "Ab1", "A1", "Bb1", "B1",  // 23
							"C2", "Db2", "D2", "Eb2", "E2", "F2", "Gb2", "G2", "Ab2", "A2", "Bb2", "B2",  // 35
							"C3", "Db3", "D3", "Eb3", "E3", "F3", "Gb3", "G3", "Ab3", "A3", "Bb3", "B3",  // 47
							"C4", "Db4", "D4", "Eb4", "E4", "F4", "Gb4", "G4", "Ab4", "A4", "Bb4", "B4",  // 59
							"C5", "Db5", "D5", "Eb5", "E5", "F5", "Gb5", "G5", "Ab5", "A5", "Bb5", "B5",  // 71
							"C6", "Db6", "D6", "Eb6", "E6", "F6", "Gb6", "G6", "Ab6", "A6", "Bb6", "B6",  // 83
							"C7", "Db7", "D7", "Eb7", "E7", "F7", "Gb7", "G7", "Ab7", "A7", "Bb7", "B7",  // 95
							"C8", "Db8", "D8", "Eb8", "E8", "F8", "Gb8", "G8", "Ab8", "A8", "Bb8", "B8" };// 107 


const double log_frequencies[] = { 2.7942279, 2.8518619, 2.90962957, 2.96784707, 3.02529108, 3.08328517,
									3.14069804, 3.19867312, 3.25655689, 3.314186, 3.3721118, 3.42978484,
									3.48737508, 3.54529773, 3.6030492, 3.66073715, 3.71843826, 3.77620328,
									3.83406146, 3.8918203, 3.94951145, 4.00733319, 4.06508738, 4.12293202,
									4.18067515, 4.23844491, 4.29619638, 4.35388433, 4.41170679, 4.469465,
									4.52720864, 4.58496748, 4.64275495, 4.70048037, 4.75823456, 4.81599821,
									4.87374589, 4.93151993, 4.98927545, 5.04703151, 5.1047933, 5.16255492,
									5.22035583, 5.27811466, 5.33585397, 5.39362755, 5.45138174, 5.50914539,
									5.56693129, 5.62466711, 5.68242264, 5.74021083, 5.79797081, 5.85573073,
									5.91347598, 5.97126184, 6.02900115, 6.08677473, 6.14452892, 6.20229257,
									6.26005936, 6.31783233, 6.37558684, 6.43334194, 6.49110283, 6.54887791,
									6.60663667, 6.66439627, 6.72216037, 6.77992191, 6.83768683, 6.89544988,
									6.95320654, 7.0109705, 7.06873402, 7.12649716, 7.18425759, 7.24201793,
									7.29978385, 7.35754345, 7.41530755, 7.47306909, 7.53083401, 7.588592,
									7.64635372, 7.70411768, 7.7618812, 7.81964434, 7.87740477, 7.93516869,
									7.99293103, 8.05069063, 8.10845473, 8.16621627, 8.22397851, 8.28174171,
									8.33950329, 8.39726486, 8.45502626, 8.51278951, 8.57055195, 8.62831408,
									8.68607653, 8.7438394, 8.80160191, 8.85936345, 8.91712569, 8.97488762 };

const double log_sizes[] = {	0.05770084, 0.05770084, 0.05799258, 0.05783075, 0.05771905, 0.05770348,
								0.05769397, 0.05792942, 0.05775644, 0.05777745, 0.05779942, 0.05763164,
								0.05775644, 0.05783706, 0.05771971, 0.05769453, 0.05773307, 0.0578116,
								0.05780851, 0.05772499, 0.05775644, 0.05778797, 0.05779942, 0.05779389,
								0.05775644, 0.05776061, 0.05771971, 0.05775521, 0.05779034, 0.05775093,
								0.05775124, 0.05777315, 0.05775644, 0.05773981, 0.05775892, 0.05775566,
								0.05776086, 0.05776478, 0.05775579, 0.05775892, 0.0577617,  0.05778126,
								0.05777987, 0.05774907, 0.05775644, 0.05776389, 0.05775892, 0.05777477,
								0.05776086, 0.05774567, 0.05777186, 0.05777409, 0.05775995, 0.05775258,
								0.05776555, 0.05776259, 0.05775644, 0.05776389, 0.05775892, 0.05776522,
								0.05776988, 0.05776374, 0.0577548,  0.05775799, 0.05776799, 0.05776692,
								0.05775918, 0.05776185, 0.05776282, 0.05776323, 0.05776398, 0.05775986,
								0.05776031, 0.05776374, 0.05776333, 0.05776178, 0.05776039, 0.05776313,
								0.05776276, 0.05776185, 0.05776282, 0.05776323, 0.05776145, 0.05775986,
								0.05776284, 0.05776374, 0.05776333, 0.05776178, 0.05776218, 0.05776313,
								0.05776097, 0.05776185, 0.05776282, 0.05776189, 0.05776272, 0.05776239,
								0.05776157, 0.05776148, 0.05776233, 0.05776285, 0.05776229, 0.05776229,
								0.05776266, 0.05776269, 0.05776202, 0.05776189, 0.05776209, 0.05776209 };


int* harmonica_offset;
std::string* C_harmonica_notes;

float* harmonica_quad_colors;
float* harmonica_line_colors;

std::string* myNotes;
int* position_offset;


/* GLUT callback Handlers */
static void resize(int width, int height)
{
	//const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 1.0f);
	/**
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 100.0);
	//gluPerspective(60, 1.0, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
	*/


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



static void draw_quad(const int offset_x, const int offset_y, const float* color_quad, const float* color_line, const std::string text)
{


	int size = WIDTH / 24;

	int center_x = WIDTH / 2 + offset_x*size*2;
	int center_y = HEIGHT / 2 + offset_y*size*2 + size*2/3;

	glBegin(GL_QUADS);
	glColor3f(color_quad[0], color_quad[1], color_quad[2]);
	glVertex2i(center_x - size, center_y - size);
	glVertex2i(center_x + size, center_y - size);
	glVertex2i(center_x + size, center_y + size);
	glVertex2i(center_x - size, center_y + size);

	glEnd();


	glLineWidth(3.75f);

	glBegin(GL_LINE_STRIP);
	glColor3f(color_line[0], color_line[1], color_line[2]);
	glVertex2i(center_x - size, center_y - size);
	glVertex2i(center_x + size, center_y - size);
	glVertex2i(center_x + size, center_y + size);
	glVertex2i(center_x - size, center_y + size);
	glVertex2i(center_x - size, center_y - size);
	glEnd();



	glPushMatrix();

	int len = glutStrokeLength(GLUT_STROKE_ROMAN, (unsigned char*)text.c_str()) / 2;
	float xScale = 0.28f;
	float yScale = 0.353f;


	glTranslatef(float(center_x) - float(len)*xScale, float(center_y) - 42.86f*yScale, 0.0f);
	glScalef(xScale, yScale, 1.0f);


	glColor3f(color_line[0], color_line[1], color_line[2]);
	for (int i = 0; text[i] != '\0'; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}

	glPopMatrix();


}

static void draw_freq_quad(const int offset_x, const int offset_y, const float* color_quad, const float* color_line, const std::string text)
{


	int size = WIDTH / 24;

	int center_x = WIDTH / 2 + offset_x*size * 2;
	int center_y = HEIGHT / 2 + offset_y*size * 2 + size * 2 / 3;

	glBegin(GL_QUADS);
	glColor3f(color_quad[0], color_quad[1], color_quad[2]);
	glVertex2i(center_x - 5 * size / 2, center_y - size);
	glVertex2i(center_x + 2 * size, center_y - size);
	glVertex2i(center_x + 2 * size, center_y + size);
	glVertex2i(center_x - 5 * size / 2, center_y + size);

	glEnd();


	glBegin(GL_LINE_STRIP);
	glColor3f(color_line[0], color_line[1], color_line[2]);
	glVertex2i(center_x - 5*size/2, center_y - size);
	glVertex2i(center_x + 2*size, center_y - size);
	glVertex2i(center_x + 2*size, center_y + size);
	glVertex2i(center_x - 5*size/2, center_y + size);
	glVertex2i(center_x - 5*size/2, center_y - size);
	glEnd();


	glPushMatrix();

	int len = glutStrokeLength(GLUT_STROKE_ROMAN, (unsigned char*)text.c_str()) / 2;
	float xScale = 0.25f;
	float yScale = 0.32f;


	glTranslatef(float(center_x) - float(len)*xScale - size, float(center_y) - 42.86f*yScale, 0.0f);
	glScalef(xScale, yScale, 1.0f);


	glColor3f(color_line[0], color_line[1], color_line[2]);
	for (int i = 0; text[i] != '\0'; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}

	glPopMatrix();

	glPushMatrix();

	len = glutStrokeLength(GLUT_STROKE_ROMAN, (unsigned char*)"Hz") / 2;
	glTranslatef(float(center_x) - float(len)*xScale + size, float(center_y) - 42.86f*yScale, 0.0f);
	glScalef(xScale, yScale, 1.0f);

	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'H');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'z');

	glPopMatrix();


}


static void draw_quad_with_bar(const int offset_x, const int offset_y, const double error, const float* color_quad, const float* color_line, const float* color_bar, const std::string text)
{


	int size = WIDTH / 24;

	double increase = 1.1;

	int center_x = WIDTH / 2 + offset_x*size * 2;
	int center_y = HEIGHT / 2 + offset_y*size * 2 + size*2/3;

	glBegin(GL_QUADS);
	glColor3f(color_quad[0], color_quad[1], color_quad[2]);
	glVertex2i(center_x - int(increase*size), center_y - int(increase*size));
	glVertex2i(center_x + int(increase*size), center_y - int(increase*size));
	glVertex2i(center_x + int(increase*size), center_y + int(increase*size));
	glVertex2i(center_x - int(increase*size), center_y + int(increase*size));

	glEnd();


	glLineWidth(3.75f);

	glBegin(GL_LINE_STRIP);
	glColor3f(color_line[0], color_line[1], color_line[2]);
	glVertex2i(center_x - int(increase*size), center_y - int(increase*size));
	glVertex2i(center_x + int(increase*size), center_y - int(increase*size));
	glVertex2i(center_x + int(increase*size), center_y + int(increase*size));
	glVertex2i(center_x - int(increase*size), center_y + int(increase*size));
	glVertex2i(center_x - int(increase*size), center_y - int(increase*size));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(color_bar[0], color_bar[1], color_bar[2]);
	glVertex2i(center_x - int(increase*size), center_y + int(2 * size * error));
	glVertex2i(center_x + int(increase*size), center_y + int(2 * size * error));
	glEnd();



	glPushMatrix();

	int len = glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)text.c_str()) / 2;
	float xScale = 0.3f;
	float yScale = 0.375f;


	glTranslatef(float(center_x) - float(len)*xScale, float(center_y) - 42.86f*yScale, 0.0f);
	glScalef(xScale, yScale, 1.0f);


	glColor3f(color_line[0], color_line[1], color_line[2]);
	for (int i = 0; text[i] != '\0'; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}

	glPopMatrix();


}



static void init_quad(int id, int offset_x, int offset_y, const float* color_quad, const float* color_line, const std::string text)
{


	harmonica_offset[id * 2] = offset_x;
	harmonica_offset[id * 2 + 1] = offset_y;
	harmonica_quad_colors[3*id + 0] = color_quad[0];
	harmonica_quad_colors[3*id + 1] = color_quad[1];
	harmonica_quad_colors[3*id + 2] = color_quad[2];

	harmonica_line_colors[3*id + 0] = color_line[0];
	harmonica_line_colors[3*id + 1] = color_line[1];
	harmonica_line_colors[3*id + 2] = color_line[2];

    C_harmonica_notes[id] = text.substr(0, text.size() - 1);


	int tmp_id = -1;
	for (int i = 0; i < numNotes; ++i)
	{

        if (notes[i].find(text) != std::string::npos)
		{
			tmp_id = i;
			break;
		}
	}


	if (tmp_id != -1)
	{

		position_offset[2 * tmp_id] = offset_x;
        position_offset[2 * tmp_id + 1] = offset_y;
    }


    if (tmp_id != -1)
    {
        myNotes[tmp_id] = text.substr(0, text.size() - 1);
	}


}


static int init_quad_return(int id, int offset_x, int offset_y, const float* color_quad, const float* color_line, const std::string text)
{


	harmonica_offset[id * 2] = offset_x;
	harmonica_offset[id * 2 + 1] = offset_y;
	harmonica_quad_colors[3 * id + 0] = color_quad[0];
	harmonica_quad_colors[3 * id + 1] = color_quad[1];
	harmonica_quad_colors[3 * id + 2] = color_quad[2];

	harmonica_line_colors[3 * id + 0] = color_line[0];
	harmonica_line_colors[3 * id + 1] = color_line[1];
	harmonica_line_colors[3 * id + 2] = color_line[2];

	C_harmonica_notes[id] = text.substr(0, text.size() - 1);


	int tmp_id = -1;
	for (int i = 0; i < numNotes; ++i)
	{

		if (notes[i].find(text) != std::string::npos)
		{
			tmp_id = i;
			break;
		}
	}


	if (tmp_id != -1)
	{

		position_offset[2 * tmp_id] = offset_x;
		position_offset[2 * tmp_id + 1] = offset_y;
	}


	if (tmp_id != -1)
	{
		myNotes[tmp_id] = text.substr(0, text.size() - 1);
	}

	return tmp_id;

}





static void init_G_harmonica()
{

	float black[3] = { 0.0f, 0.0f, 0.0f };
	float white[3] = { 1.0f, 1.0f, 1.0f };
	float light_blue[3] = { 0.4f, 0.7f, 1.0f };
	float light_purple[3] = { 0.4f, 0.46f, 1.0f };


	for (int i = 0; i < numNotes; ++i) // reset the position off the notes on the harmonica 
	{
		position_offset[2 * i] = -100; // x-offset
		position_offset[2 * i + 1] = 3; // y-offset
	}


	double_id = -1;
	for (int i = 0; i < numNotes; ++i)
	{

		if (notes[i].find("D4") != std::string::npos)
		{
			double_id = i;
			break;
		}
	}



	int id = -1;

	const int shift = -4;
	init_quad(++id, -1 + shift, 0, black, light_blue, "G_");
	init_quad(++id, 0 + shift, 0, black, light_blue, "1_");
	init_quad(++id, 1 + shift, 0, black, light_blue, "2_");
	init_quad(++id, 2 + shift, 0, black, light_blue, "3_");
	init_quad(++id, 3 + shift, 0, black, light_blue, "4_");
	init_quad(++id, 4 + shift, 0, black, light_blue, "5_");
	init_quad(++id, 5 + shift, 0, black, light_blue, "6_");
	init_quad(++id, 6 + shift, 0, black, light_blue, "7_");
	init_quad(++id, 7 + shift, 0, black, light_blue, "8_");
	init_quad(++id, 8 + shift, 0, black, light_blue, "9_");
	init_quad(++id, 9 + shift, 0, black, light_blue, "10_");


	pitch->harmonica_top_row[0] = init_quad_return(++id, 0 + shift, 1, light_blue, black, "G3");
	pitch->harmonica_top_row[1] = init_quad_return(++id, 1 + shift, 1, light_blue, black, "B3");
	pitch->harmonica_top_row[2] = init_quad_return(++id, 2 + shift, 1, light_blue, black, "D4");
	pitch->harmonica_top_row[3] = init_quad_return(++id, 3 + shift, 1, light_blue, black, "G4");
	pitch->harmonica_top_row[4] = init_quad_return(++id, 4 + shift, 1, light_blue, black, "B4");
	pitch->harmonica_top_row[5] = init_quad_return(++id, 5 + shift, 1, light_blue, black, "D5");
	pitch->harmonica_top_row[6] = init_quad_return(++id, 6 + shift, 1, light_blue, black, "G5");
	pitch->harmonica_top_row[7] = init_quad_return(++id, 7 + shift, 1, light_blue, black, "B5");
	pitch->harmonica_top_row[8] = init_quad_return(++id, 8 + shift, 1, light_blue, black, "D6");
	pitch->harmonica_top_row[9] = init_quad_return(++id, 9 + shift, 1, light_blue, black, "G6");


	init_quad(++id, 7 + shift, 2, light_purple, black, "A#5");
	init_quad(++id, 8 + shift, 2, light_purple, black, "C#6");
	init_quad(++id, 9 + shift, 2, light_purple, black, "F#6");

	init_quad(++id, 9 + shift, 3, light_purple, black, "F6");

	pitch->harmonica_bot_row[0] = init_quad_return(++id, 0 + shift, -1, light_blue, black, "A3");
	pitch->harmonica_bot_row[1] = init_quad_return(++id, 1 + shift, -1, light_blue, black, "D4");
	pitch->harmonica_bot_row[2] = init_quad_return(++id, 2 + shift, -1, light_blue, black, "F#4");
	pitch->harmonica_bot_row[3] = init_quad_return(++id, 3 + shift, -1, light_blue, black, "A4");
	pitch->harmonica_bot_row[4] = init_quad_return(++id, 4 + shift, -1, light_blue, black, "C5");
	pitch->harmonica_bot_row[5] = init_quad_return(++id, 5 + shift, -1, light_blue, black, "E5");
	pitch->harmonica_bot_row[6] = init_quad_return(++id, 6 + shift, -1, light_blue, black, "F#5");
	pitch->harmonica_bot_row[7] = init_quad_return(++id, 7 + shift, -1, light_blue, black, "A5");
	pitch->harmonica_bot_row[8] = init_quad_return(++id, 8 + shift, -1, light_blue, black, "C6");
	pitch->harmonica_bot_row[9] = init_quad_return(++id, 9 + shift, -1, light_blue, black, "E6");

	init_quad(++id, 0 + shift, -2, light_purple, black, "G#3");
	init_quad(++id, 1 + shift, -2, light_purple, black, "C#4");
	init_quad(++id, 2 + shift, -2, light_purple, black, "F4");
	init_quad(++id, 3 + shift, -2, light_purple, black, "G#4");
	init_quad(++id, 5 + shift, -2, light_purple, black, "D#5");

	init_quad(++id, 1 + shift, -3, light_purple, black, "C4");
	init_quad(++id, 2 + shift, -3, light_purple, black, "E4");
	init_quad(++id, 2 + shift, -4, light_purple, black, "D#4");

	printf("%d quads initilazed.\n", id + 1);


}







static void init_A_harmonica()
{

	float black[3] = { 0.0f, 0.0f, 0.0f };
	float white[3] = { 1.0f, 1.0f, 1.0f };
	float light_blue[3] = { 0.4f, 0.7f, 1.0f };
	float light_purple[3] = { 0.4f, 0.46f, 1.0f };

	for (int i = 0; i < numNotes; ++i) // reset the position off the notes on the harmonica 
	{
		position_offset[2 * i] = -100; // x-offset
		position_offset[2 * i + 1] = 3; // y-offset
	}


	double_id = -1;
	for (int i = 0; i < numNotes; ++i)
	{
		//printf("Search for note %d %d  %s   %s\n", i, id, text.c_str(), notes[i].c_str());

		if (notes[i].find("E4") != std::string::npos)
		{
			double_id = i;
			break;
		}
	}



	int id = -1;

	const int shift = -4;
	init_quad(++id, -1 + shift, 0, black, light_blue, "A_");
	init_quad(++id, 0 + shift, 0, black, light_blue, "1_");
	init_quad(++id, 1 + shift, 0, black, light_blue, "2_");
	init_quad(++id, 2 + shift, 0, black, light_blue, "3_");
	init_quad(++id, 3 + shift, 0, black, light_blue, "4_");
	init_quad(++id, 4 + shift, 0, black, light_blue, "5_");
	init_quad(++id, 5 + shift, 0, black, light_blue, "6_");
	init_quad(++id, 6 + shift, 0, black, light_blue, "7_");
	init_quad(++id, 7 + shift, 0, black, light_blue, "8_");
	init_quad(++id, 8 + shift, 0, black, light_blue, "9_");
	init_quad(++id, 9 + shift, 0, black, light_blue, "10_");

	pitch->harmonica_top_row[0] = init_quad_return(++id, 0 + shift, 1, light_blue, black, "A3");
	pitch->harmonica_top_row[1] = init_quad_return(++id, 1 + shift, 1, light_blue, black, "C#4");
	pitch->harmonica_top_row[2] = init_quad_return(++id, 2 + shift, 1, light_blue, black, "E4");
	pitch->harmonica_top_row[3] = init_quad_return(++id, 3 + shift, 1, light_blue, black, "A4");
	pitch->harmonica_top_row[4] = init_quad_return(++id, 4 + shift, 1, light_blue, black, "C#5");
	pitch->harmonica_top_row[5] = init_quad_return(++id, 5 + shift, 1, light_blue, black, "E5");
	pitch->harmonica_top_row[6] = init_quad_return(++id, 6 + shift, 1, light_blue, black, "A5");
	pitch->harmonica_top_row[7] = init_quad_return(++id, 7 + shift, 1, light_blue, black, "C#6");
	pitch->harmonica_top_row[8] = init_quad_return(++id, 8 + shift, 1, light_blue, black, "E6");
	pitch->harmonica_top_row[9] = init_quad_return(++id, 9 + shift, 1, light_blue, black, "A6");


	init_quad(++id, 7 + shift, 2, light_purple, black, "C6");
	init_quad(++id, 8 + shift, 2, light_purple, black, "D#6");
	init_quad(++id, 9 + shift, 2, light_purple, black, "G#6");

	init_quad(++id, 9 + shift, 3, light_purple, black, "G6");

	pitch->harmonica_bot_row[0] = init_quad_return(++id, 0 + shift, -1, light_blue, black, "B3");
	pitch->harmonica_bot_row[1] = init_quad_return(++id, 1 + shift, -1, light_blue, black, "E4");
	pitch->harmonica_bot_row[2] = init_quad_return(++id, 2 + shift, -1, light_blue, black, "G#4");
	pitch->harmonica_bot_row[3] = init_quad_return(++id, 3 + shift, -1, light_blue, black, "B4");
	pitch->harmonica_bot_row[4] = init_quad_return(++id, 4 + shift, -1, light_blue, black, "D5");
	pitch->harmonica_bot_row[5] = init_quad_return(++id, 5 + shift, -1, light_blue, black, "F#5");
	pitch->harmonica_bot_row[6] = init_quad_return(++id, 6 + shift, -1, light_blue, black, "G#5");
	pitch->harmonica_bot_row[7] = init_quad_return(++id, 7 + shift, -1, light_blue, black, "B5");
	pitch->harmonica_bot_row[8] = init_quad_return(++id, 8 + shift, -1, light_blue, black, "D6");
	pitch->harmonica_bot_row[9] = init_quad_return(++id, 9 + shift, -1, light_blue, black, "F#6");

	init_quad(++id, 0 + shift, -2, light_purple, black, "A#3");
	init_quad(++id, 1 + shift, -2, light_purple, black, "D#4");
	init_quad(++id, 2 + shift, -2, light_purple, black, "G4");
	init_quad(++id, 3 + shift, -2, light_purple, black, "A#4");
	init_quad(++id, 5 + shift, -2, light_purple, black, "F5");

	init_quad(++id, 1 + shift, -3, light_purple, black, "D4");
	init_quad(++id, 2 + shift, -3, light_purple, black, "F#4");

	init_quad(++id, 2 + shift, -4, light_purple, black, "F4");

	printf("%d quads initilazed.\n", id + 1);


}








static void init_D_harmonica()
{

	float black[3] = { 0.0f, 0.0f, 0.0f };
	float white[3] = { 1.0f, 1.0f, 1.0f };
	float light_blue[3] = { 0.4f, 0.7f, 1.0f };
	float light_purple[3] = { 0.4f, 0.46f, 1.0f };

	for (int i = 0; i < numNotes; ++i) // reset the position off the notes on the harmonica 
	{
		position_offset[2 * i] = -100; // x-offset
		position_offset[2 * i + 1] = 3; // y-offset
	}


	double_id = -1;
	for (int i = 0; i < numNotes; ++i)
	{
		//printf("Search for note %d %d  %s   %s\n", i, id, text.c_str(), notes[i].c_str());

		if (notes[i].find("A4") != std::string::npos)
		{
			double_id = i;
			break;
		}
	}



	int id = -1;

	const int shift = -4;
	init_quad(++id, -1 + shift, 0, black, light_blue, "D_");
	init_quad(++id, 0 + shift, 0, black, light_blue, "1_");
	init_quad(++id, 1 + shift, 0, black, light_blue, "2_");
	init_quad(++id, 2 + shift, 0, black, light_blue, "3_");
	init_quad(++id, 3 + shift, 0, black, light_blue, "4_");
	init_quad(++id, 4 + shift, 0, black, light_blue, "5_");
	init_quad(++id, 5 + shift, 0, black, light_blue, "6_");
	init_quad(++id, 6 + shift, 0, black, light_blue, "7_");
	init_quad(++id, 7 + shift, 0, black, light_blue, "8_");
	init_quad(++id, 8 + shift, 0, black, light_blue, "9_");
	init_quad(++id, 9 + shift, 0, black, light_blue, "10_");

	init_quad(++id, 0 + shift, 1, light_blue, black, "D4");
	init_quad(++id, 1 + shift, 1, light_blue, black, "F#4");
	init_quad(++id, 2 + shift, 1, light_blue, black, "A4");
	init_quad(++id, 3 + shift, 1, light_blue, black, "D5");
	init_quad(++id, 4 + shift, 1, light_blue, black, "F#5");
	init_quad(++id, 5 + shift, 1, light_blue, black, "A5");
	init_quad(++id, 6 + shift, 1, light_blue, black, "D6");
	init_quad(++id, 7 + shift, 1, light_blue, black, "F#6");
	init_quad(++id, 8 + shift, 1, light_blue, black, "A6");
	init_quad(++id, 9 + shift, 1, light_blue, black, "D7");


	init_quad(++id, 7 + shift, 2, light_purple, black, "F6");
	init_quad(++id, 8 + shift, 2, light_purple, black, "G#6");
	init_quad(++id, 9 + shift, 2, light_purple, black, "C#");

	init_quad(++id, 9 + shift, 3, light_purple, black, "C6");

	init_quad(++id, 0 + shift, -1, light_blue, black, "E4");
	init_quad(++id, 1 + shift, -1, light_blue, black, "A4");
	init_quad(++id, 2 + shift, -1, light_blue, black, "C#5");
	init_quad(++id, 3 + shift, -1, light_blue, black, "E5");
	init_quad(++id, 4 + shift, -1, light_blue, black, "G5");
	init_quad(++id, 5 + shift, -1, light_blue, black, "B5");
	init_quad(++id, 6 + shift, -1, light_blue, black, "C#6");
	init_quad(++id, 7 + shift, -1, light_blue, black, "E6");
	init_quad(++id, 8 + shift, -1, light_blue, black, "G6");
	init_quad(++id, 9 + shift, -1, light_blue, black, "B6");

	init_quad(++id, 0 + shift, -2, light_purple, black, "D#4");
	init_quad(++id, 1 + shift, -2, light_purple, black, "G#4");
	init_quad(++id, 2 + shift, -2, light_purple, black, "C4");
	init_quad(++id, 3 + shift, -2, light_purple, black, "D#5");
	init_quad(++id, 5 + shift, -2, light_purple, black, "A#5");

	init_quad(++id, 1 + shift, -3, light_purple, black, "G4");
	init_quad(++id, 2 + shift, -3, light_purple, black, "B4");

	init_quad(++id, 2 + shift, -4, light_purple, black, "A#4");

	printf("%d quads initilazed.\n", id + 1);


}









static void init_C_harmonica()
{

	float black[3] = { 0.0f, 0.0f, 0.0f };
	float white[3] = { 1.0f, 1.0f, 1.0f };
	float light_blue[3] = { 0.4f, 0.7f, 1.0f};
	float light_purple[3] = { 0.4f, 0.46f, 1.0f };

	for (int i = 0; i < numNotes; ++i) // reset the position off the notes on the harmonica 
	{
		position_offset[2 * i] = -100; // x-offset
		position_offset[2 * i + 1] = 3; // y-offset
	} 


	double_id = -1;
	for (int i = 0; i < numNotes; ++i)
	{
		//printf("Search for note %d %d  %s   %s\n", i, id, text.c_str(), notes[i].c_str());

		if (notes[i].find("G4") != std::string::npos)
		{
			double_id = i;
			break;
		}
	}



	int id = -1;

	const int shift = -4;
	init_quad(++id, -1 + shift, 0, black, light_blue, "C_");
	init_quad(++id, 0 + shift, 0, black, light_blue, "1_");
	init_quad(++id, 1 + shift, 0, black, light_blue, "2_");
	init_quad(++id, 2 + shift, 0, black, light_blue, "3_");
	init_quad(++id, 3 + shift, 0, black, light_blue, "4_");
	init_quad(++id, 4 + shift, 0, black, light_blue, "5_");
	init_quad(++id, 5 + shift, 0, black, light_blue, "6_");
	init_quad(++id, 6 + shift, 0, black, light_blue, "7_");
	init_quad(++id, 7 + shift, 0, black, light_blue, "8_");
	init_quad(++id, 8 + shift, 0, black, light_blue, "9_");
	init_quad(++id, 9 + shift, 0, black, light_blue, "10_");

	pitch->harmonica_top_row[0] = init_quad_return(++id, 0 + shift, 1, light_blue, black, "C4");
	pitch->harmonica_top_row[1] = init_quad_return(++id, 1 + shift, 1, light_blue, black, "E4");
	pitch->harmonica_top_row[2] = init_quad_return(++id, 2 + shift, 1, light_blue, black, "G4");
	pitch->harmonica_top_row[3] = init_quad_return(++id, 3 + shift, 1, light_blue, black, "C5");
	pitch->harmonica_top_row[4] = init_quad_return(++id, 4 + shift, 1, light_blue, black, "E5");
	pitch->harmonica_top_row[5] = init_quad_return(++id, 5 + shift, 1, light_blue, black, "G5");
	pitch->harmonica_top_row[6] = init_quad_return(++id, 6 + shift, 1, light_blue, black, "C6");
	pitch->harmonica_top_row[7] = init_quad_return(++id, 7 + shift, 1, light_blue, black, "E6");
	pitch->harmonica_top_row[8] = init_quad_return(++id, 8 + shift, 1, light_blue, black, "G6");
	pitch->harmonica_top_row[9] = init_quad_return(++id, 9 + shift, 1, light_blue, black, "C7");


	init_quad(++id, 7 + shift, 2, light_purple, black, "Eb6");
	init_quad(++id, 8 + shift, 2, light_purple, black, "Gb6");
	init_quad(++id, 9 + shift, 2, light_purple, black, "B6");

	init_quad(++id, 9 + shift, 3, light_purple, black, "Bb6");

	pitch->harmonica_bot_row[0] = init_quad_return(++id, 0 + shift, -1, light_blue, black, "D4");
	pitch->harmonica_bot_row[1] = init_quad_return(++id, 1 + shift, -1, light_blue, black, "G4");
	pitch->harmonica_bot_row[2] = init_quad_return(++id, 2 + shift, -1, light_blue, black, "B4");
	pitch->harmonica_bot_row[3] = init_quad_return(++id, 3 + shift, -1, light_blue, black, "D5");
	pitch->harmonica_bot_row[4] = init_quad_return(++id, 4 + shift, -1, light_blue, black, "F5");
	pitch->harmonica_bot_row[5] = init_quad_return(++id, 5 + shift, -1, light_blue, black, "A5");
	pitch->harmonica_bot_row[6] = init_quad_return(++id, 6 + shift, -1, light_blue, black, "B5");
	pitch->harmonica_bot_row[7] = init_quad_return(++id, 7 + shift, -1, light_blue, black, "D6");
	pitch->harmonica_bot_row[8] = init_quad_return(++id, 8 + shift, -1, light_blue, black, "F6");
	pitch->harmonica_bot_row[9] = init_quad_return(++id, 9 + shift, -1, light_blue, black, "A6");

	init_quad(++id, 0 + shift, -2, light_purple, black, "Db4");
	init_quad(++id, 1 + shift, -2, light_purple, black, "Gb4");
	init_quad(++id, 2 + shift, -2, light_purple, black, "Bb4");
	init_quad(++id, 3 + shift, -2, light_purple, black, "Db5");
	init_quad(++id, 5 + shift, -2, light_purple, black, "Ab5");

	init_quad(++id, 1 + shift, -3, light_purple, black, "F4");
	init_quad(++id, 2 + shift, -3, light_purple, black, "A4");

	init_quad(++id, 2 + shift, -4, light_purple, black, "Ab4");

	printf("%d quads initilazed.\n", id+1);


}


static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float black[3] = { 0.0f, 0.0f, 0.0f };
	float white[3] = { 1.0f, 1.0f, 1.0f };

	for (int i = 0; i < numQuads; ++i)
	{
            draw_quad(harmonica_offset[2*i], harmonica_offset[2*i+1], harmonica_quad_colors + 3*i, harmonica_line_colors + 3*i, C_harmonica_notes[i]);
	}



	switch (num_showing_notes)
	{
		case 0:
		{
			draw_quad(-1, 3, black, white, "");
			draw_freq_quad(1, 3, black, white, "0");
			break;
		}
		case 1:
		{
			draw_quad_with_bar(-1, 3, showing_error[0], black, white, note_color, notes__[showing_notes[0]]);

			double f = std::exp(showing_error[0] * log_sizes[showing_notes[0]] + log_frequencies[showing_notes[0]]);
			draw_freq_quad(1, 3, black, white, std::to_string(int(f)));

			break;
		}
		default:
		{
			draw_quad(-1, 3, black, white, "");
			draw_freq_quad(1, 3, black, white, "0");
			break;
		}
	}


	for (int i = 0; i < num_showing_notes; ++i)
	{

		int note_offset_x = position_offset[2 * showing_notes[i]];		// note_ offset specifies the position of the quad to be highlighted
		int note_offset_y = position_offset[2 * showing_notes[i] + 1];


		note_text = myNotes[showing_notes[i]];

		if (showing_notes[i] == double_id)
		{
			draw_quad_with_bar(note_offset_x2, note_offset_y2, showing_error[i], note_color, note_line_color, note_line_color, note_text);
		}



		draw_quad_with_bar(note_offset_x, note_offset_y, showing_error[i], note_color, note_line_color, note_line_color, note_text);
	}


	glutSwapBuffers();


}


static void key(unsigned char key, int x, int y)
{


	// we'll switch between red and blue when the user presses a key:
	GLfloat colors[][3] = { { 0.2f, 0.2f, 0.2f }, { 0.4f, 0.4f, 0.4f },{ 0.6f, 0.6f, 0.6f },{ 0.8f, 0.8f, 0.8f },{ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
	static int back = 0;

	switch (key)
	{
	case 27:
	case 'q':
		exit(0);
		break;
	case 'a':
		init_A_harmonica();
		break;
	case 'c':
		init_C_harmonica();
		break;
	case 'd':
		init_D_harmonica();
		break;
	case 'g':
		init_G_harmonica();
		break;
	case '1':
		delete pitch;
		pitch = new pitchDetector(512);
		init_C_harmonica();
		break;
	case '2':
		delete pitch;
		pitch = new pitchDetector(1024);
		init_C_harmonica();
		break;
	case '3':
		delete pitch;
		pitch = new pitchDetector(2048);
		init_C_harmonica();
		break;
	case '4':
		delete pitch;
		pitch = new pitchDetector(4096);
		init_C_harmonica();
		break;
	case '5':
		delete pitch;
		pitch = new pitchDetector(8192);
		init_C_harmonica();
		break;
	case '6':
		delete pitch;
		pitch = new pitchDetector(16384);
		init_C_harmonica();
		break;
	case '7':
		delete pitch;
		pitch = new pitchDetector(32768);
		init_C_harmonica();
		break;


	case 'm':
		pitch->m_multiple_note_detection = !pitch->m_multiple_note_detection;
        break;
	default:
		++back;
		back = back % 6;
		glClearColor(colors[back][0], colors[back][1], colors[back][2], 1.0f);
		glutPostRedisplay();
		break;
	}
	glutPostRedisplay();
}


void mainMenu(int i)
{
	key((unsigned char)i, 0, 0);
}

void initMenus()
{
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("G Harmonica [g]", 'g');
	glutAddMenuEntry("C Harmonica [c]", 'c');
	glutAddMenuEntry("C Harmonica [d]", 'd');
	glutAddMenuEntry("A Harmonica [a]", 'a');
	glutAddMenuEntry("Toggle multiple note detection [m]", 'm');
	glutAddMenuEntry("Set window size to 512 [1]", '1');
	glutAddMenuEntry("Set window size to 1024 [2]", '2');
	glutAddMenuEntry("Set window size to 2048 [3]", '3');
	glutAddMenuEntry("Set window size to 4096 [4]", '4');
	glutAddMenuEntry("Set window size to 8192 [5]", '5');
	glutAddMenuEntry("Quit (esc)", '\033');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}





static void idle(void)
{

	int cur_playing_notes = pitch->detectPitch(playing_notes, playing_error);

	int currentNote = 1000 * cur_playing_notes + playing_notes[0];


	last_notes[cur_note_id] = currentNote; // override the oldest note with the current note

	cur_note_id++; // set the current note id for the next note
	cur_note_id = cur_note_id % num_last_notes;

	int times_played = 0;  // 
	for (int i = 0; i < num_last_notes; ++i)
	{
		if (currentNote == last_notes[i])
		{
			++times_played;
		}
	}


	// update showing notes and errors
	if (times_played > num_last_notes / 2)
	{
		showing_note = currentNote;
		num_showing_notes = cur_playing_notes;
		for (int i = 0; i < cur_playing_notes; ++i)
		{
			showing_notes[i] = playing_notes[i];
			showing_error[i] = playing_error[i];
		}
		
	}
	else
	{
		int times_played = 0;  // 
		for (int i = 0; i < num_last_notes; ++i)
		{
			if (showing_note == last_notes[i])
			{
				++times_played;
			}
		}

		if (!(times_played > num_last_notes / 2))
		{
			num_showing_notes = 0;
		}
	}

	glutPostRedisplay();
	return;
}




/* Program entry point */
int main(int argc, char *argv[])
{



	pitch = new pitchDetector;



	harmonica_quad_colors = new float[3 * numQuads];
	harmonica_line_colors = new float[3 * numQuads];

	harmonica_offset = new int[2 * numQuads];
    C_harmonica_notes = new std::string[numQuads];

	position_offset = new int[2 * numNotes];			// stores the position off the quad of the note
    myNotes = new std::string[numNotes];

	last_notes = new int[num_last_notes];



	for (int i = 0; i < numNotes; ++i)
	{
		position_offset[2*i] = -100; // x-offset
		position_offset[2*i+1] = 3; // y-offset

		myNotes[i] = notes__[i];
	}


	printf("Initialized arrays\n");

	init_C_harmonica();

    note_offset_x2 = harmonica_offset[2 * 13];
    note_offset_y2 = harmonica_offset[2 * 13 + 1];

	printf("Initialized harmonica\n");


	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Harmonica");

	
	initMenus();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);

	glutMainLoop();

	delete pitch;
	delete[] last_notes;
	delete[] position_offset;
	delete[] myNotes;
	delete[] harmonica_offset;
	delete[] C_harmonica_notes;
	delete[] harmonica_quad_colors;
	delete[] harmonica_line_colors;

	return EXIT_SUCCESS;
}
#endif
