#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#include <vector>
using std::vector;

#define pi (2*acos(0.0))
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define STACK_SIZE 50

const int FRAME_DIMENSION = 500;
const int SLICE_SPHERE = 30;
const int STACK_SPHERE = 25;
const int SLICE_CYLINDER = 30;
const int STACK_CYLINDER = 50;
double sphere_size = 20;
double center_expand = 20;
int drawgrid,drawaxes;

void mouseListener(int KEY_PRESSED, int state, int x, int y);
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int KEY_PRESSED, int x, int y);


float RAD(int deg)
{
    return deg * pi / 180;
}

void setColor(int color)
{
    switch (color)
    {
    case WHITE:
        glColor3f(1, 1, 1);
        break;
    case BLUE:
        glColor3f(0, 0, 1);
        break;
    case GREEN:
        glColor3f(0, 1, 0);
        break;
    case RED:
        glColor3f(1, 0, 0);
        break;
    default:
        break;
    }
}

class point
{
public:
    double x, y, z;

    point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    point()
    {
        x=0;
        y=0;
        z=0;
    }

    point operator*(const point& right) const
    {
        return { x * right.x, y * right.y, z * right.z };
    }

} xAxis{1,0,0}, yAxis{0,1,0}, zAxis{0,0,1}, _xAxis{-1,0,0}, _yAxis{0,-1,0}, _zAxis{0,0,-1};



class CAMERA
{

private:
    point pos{100,100,0}, u{0,0,1}, r{-1/sqrt(2),1/sqrt(2),0}, l{-1/sqrt(2),-1/sqrt(2),0};

public:
    void mvForward()
    {
        pos.x = pos.x + l.x;
        pos.y = pos.y + l.y;
        pos.z = pos.z + l.z;
    }

    void mvBackward()
    {
        pos.x = pos.x - l.x;
        pos.y = pos.y - l.y;
        pos.z = pos.z - l.z;
    }

    void mvLeft()
    {
        pos.x = pos.x - r.x;
        pos.y = pos.y - r.y;
        pos.z = pos.z - r.z;
    }

    void mvRight()
    {
        pos.x = pos.x + r.x;
        pos.y = pos.y + r.y;
        pos.z = pos.z + r.z;
    }

    void mvUP()
    {
        pos.x = pos.x + u.x;
        pos.y = pos.y + u.y;
        pos.z = pos.z + u.z;

    }

    void mvDown()
    {
        pos.x = pos.x - u.x;
        pos.y = pos.y - u.y;
        pos.z = pos.z - u.z;
    }

    void lookLeft()
    {
        r.x = r.x * cos(RAD(3)) + l.x * sin(RAD(3));
        r.y = r.y * cos(RAD(3)) + l.y * sin(RAD(3));
        r.z = r.z * cos(RAD(3)) + l.z * sin(RAD(3));

        l.x = l.x * cos(RAD(3)) - r.x * sin(RAD(3));
        l.y = l.y * cos(RAD(3)) - r.y * sin(RAD(3));
        l.z = l.z * cos(RAD(3)) - r.z * sin(RAD(3));


    }

    void lookRight()
    {
        r.x = r.x * cos(-RAD(3)) + l.x * sin(-RAD(3));
        r.y = r.y * cos(-RAD(3)) + l.y * sin(-RAD(3));
        r.z = r.z * cos(-RAD(3)) + l.z * sin(-RAD(3));

        l.x = l.x * cos(-RAD(3)) - r.x * sin(-RAD(3));
        l.y = l.y * cos(-RAD(3)) - r.y * sin(-RAD(3));
        l.z = l.z * cos(-RAD(3)) - r.z * sin(-RAD(3));

    }

    void lookUp()
    {
        l.x = l.x * cos(RAD(3)) + u.x * sin(RAD(3));
        l.y = l.y * cos(RAD(3)) + u.y * sin(RAD(3));
        l.z = l.z * cos(RAD(3)) + u.z * sin(RAD(3));

        u.x = u.x * cos(RAD(3)) - l.x * sin(RAD(3));
        u.y = u.y * cos(RAD(3)) - l.y * sin(RAD(3));
        u.z = u.z * cos(RAD(3)) - l.z * sin(RAD(3));

    }

    void lookDown()
    {
        l.x = l.x * cos(-RAD(3)) + u.x * sin(-RAD(3));
        l.y = l.y * cos(-RAD(3)) + u.y * sin(-RAD(3));
        l.z = l.z * cos(-RAD(3)) + u.z * sin(-RAD(3));

        u.x = u.x * cos(-RAD(3)) - l.x * sin(-RAD(3));
        u.y = u.y * cos(-RAD(3)) - l.y * sin(-RAD(3));
        u.z = u.z * cos(-RAD(3)) - l.z * sin(-RAD(3));

    }

    void tiltClockwise()
    {
        u.x = u.x * cos(RAD(3)) + r.x * sin(RAD(3));
        u.y = u.y * cos(RAD(3)) + r.y * sin(RAD(3));
        u.z = u.z * cos(RAD(3)) + r.z * sin(RAD(3));

        r.x = r.x * cos(RAD(3)) - u.x * sin(RAD(3));
        r.y = r.y * cos(RAD(3)) - u.y * sin(RAD(3));
        r.z = r.z * cos(RAD(3)) - u.z * sin(RAD(3));

    }

    void tiltCounterClockwise()
    {
        u.x = u.x * cos(-RAD(3)) + r.x * sin(-RAD(3));
        u.y = u.y * cos(-RAD(3)) + r.y * sin(-RAD(3));
        u.z = u.z * cos(-RAD(3)) + r.z * sin(-RAD(3));

        r.x = r.x * cos(-RAD(3)) - u.x * sin(-RAD(3));
        r.y = r.y * cos(-RAD(3)) - u.y * sin(-RAD(3));
        r.z = r.z * cos(-RAD(3)) - u.z * sin(-RAD(3));

    }

    const point& up() const
    {
        return u;
    }

    const point& left() const
    {
        return l;
    }

    const point& position() const
    {
        return pos;
    }

} camera;


void clearDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rotate(double rotationDegree, const point& p)
{
    glRotated(rotationDegree, p.x, p.y, p.z);
}

void translate(const point& vector)
{
    glTranslated(vector.x, vector.y, vector.z);
}

void drawAxes()
{
    if (drawaxes == 1)
    {
        glBegin(GL_LINES);
        {
            // x-axis
            setColor(WHITE);
            glVertex3f(FRAME_DIMENSION, 0, 0);
            glVertex3f(-FRAME_DIMENSION, 0, 0);

            // y-axis
            setColor(WHITE);
            glVertex3f(0, -FRAME_DIMENSION, 0);
            glVertex3f(0, FRAME_DIMENSION, 0);

            // z-axis
            setColor(WHITE);
            glVertex3f(0, 0, FRAME_DIMENSION);
            glVertex3f(0, 0, -FRAME_DIMENSION);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;

                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}


void drawOctahedron(double a) {
	glBegin(GL_TRIANGLES);
    {
        // Top face
        glVertex3f(0, a, 0);
        glVertex3f(-a, 0, 0);
        glVertex3f(0, 0, -a);

        glVertex3f(0, a, 0);
        glVertex3f(0, 0, -a);
        glVertex3f(a, 0, 0);

        glVertex3f(0, a, 0);
        glVertex3f(a, 0, 0);
        glVertex3f(0, 0, a);

        glVertex3f(0, a, 0);
        glVertex3f(0, 0, a);
        glVertex3f(-a, 0, 0);

        // Bottom face
        glVertex3f(0, -a, 0);
        glVertex3f(0, 0, -a);
        glVertex3f(-a, 0, 0);

        glVertex3f(0, -a, 0);
        glVertex3f(a, 0, 0);
        glVertex3f(0, 0, -a);

        glVertex3f(0, -a, 0);
        glVertex3f(0, 0, a);
        glVertex3f(a, 0, 0);

        glVertex3f(0, -a, 0);
        glVertex3f(-a, 0, 0);
        glVertex3f(0, 0, a);
    }
    glEnd();
}


/* centering (0,0,0) at +x,+y, Z : height at z-axis, curved surface at +x,+y*/
void drawCylinder(double radius, double height)
{
    int slices = 30;
    int stacks = 50;
    double h, r;
    int i, j;
    vector<vector<point>> pointArray(stacks + 1, vector<point>(slices + 1));

//    glBegin(GL_QUADS);
//    {
//        glVertex3f(a, a, 0);
//        glVertex3f(a, -a, 0);
//        glVertex3f(-a, -a, 0);
//        glVertex3f(-a, a, 0);
//    }
//    glEnd();

//generate pointArray
    for (i=0; i<=stacks; i++)
    {
        h = height*sin(((double)i/(double)stacks)*(pi/2));
        r = radius;
        for (j=0; j<=slices; j++)
        {
            pointArray[i][j].x = r*cos(((double)j/(double)slices)*pi/2);
            pointArray[i][j].y = r*sin(((double)j/(double)slices)*pi/2);
            pointArray[i][j].z = h;
        }
    }
    //draw quads using generated pointArray
    for (i=0; i<stacks; i++)
    {
        for (j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
//              glVertex3f(a, a, 0);
//              glVertex3f(a, -a, 0);
//              glVertex3f(-a, -a, 0);
//              glVertex3f(-a, a, 0);
                //lower hemisphere
                glVertex3f(pointArray[i][j].x, pointArray[i][j].y, -pointArray[i][j].z);
                glVertex3f(pointArray[i][j + 1].x, pointArray[i][j + 1].y, -pointArray[i][j + 1].z);
                glVertex3f(pointArray[i + 1][j + 1].x, pointArray[i + 1][j + 1].y, -pointArray[i + 1][j + 1].z);
                glVertex3f(pointArray[i + 1][j].x, pointArray[i + 1][j].y, -pointArray[i + 1][j].z);
                //upper hemisphere
                glVertex3f(pointArray[i][j].x, pointArray[i][j].y, pointArray[i][j].z);
                glVertex3f(pointArray[i][j + 1].x, pointArray[i][j + 1].y, pointArray[i][j + 1].z);
                glVertex3f(pointArray[i + 1][j + 1].x, pointArray[i + 1][j + 1].y, pointArray[i + 1][j + 1].z);
                glVertex3f(pointArray[i + 1][j].x, pointArray[i + 1][j].y, pointArray[i + 1][j].z);

            }
            glEnd();
        }
    }

}

void drawSphere(double radius)
{
    int slices = 30;
    int stacks = 25;
    int i, j;
    double h, r;
    vector<vector<point>> pointArray(stacks + 1, vector<point>(slices + 1));
//    glBegin(GL_QUADS);
//    {
//        glVertex3f(a, a, 0);
//        glVertex3f(a, -a, 0);
//        glVertex3f(-a, -a, 0);
//        glVertex3f(-a, a, 0);
//    }
//    glEnd();

    for (i = 0; i <= stacks; i++)
    {
        r = radius*cos(((double)i/(double)stacks)*(pi/2));
        h = radius*sin(((double)i/(double)stacks)*(pi/2));

        for (j = 0; j <= slices; j++)
        {
            pointArray[i][j].x = r*cos(((double)j/(double)slices)*pi/2);
            pointArray[i][j].y = r*sin(((double)j/(double)slices)*pi/2);
            pointArray[i][j].z = h;
        }
    }

    for (i = 0; i < stacks; i++)
    {
        for (j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(pointArray[i][j].x, pointArray[i][j].y, pointArray[i][j].z);
                glVertex3f(pointArray[i][j + 1].x, pointArray[i][j + 1].y, pointArray[i][j + 1].z);
                glVertex3f(pointArray[i + 1][j + 1].x, pointArray[i + 1][j + 1].y, pointArray[i + 1][j + 1].z);
                glVertex3f(pointArray[i + 1][j].x, pointArray[i + 1][j].y, pointArray[i + 1][j].z);
            }
            glEnd();
        }
    }

}


void drawTranslatedSphereQuarter(double center_expand, double rad)     // +z axis  translation
{
    GLdouble angleArray[] = {0, 270, 90, 180};
    point translatePointArray[] = { point(1, 1, 1), point(1, -1, 1), point(-1, 1, 1), point(-1, -1, 1) };

//    translate(transVector);
//    rotate(angleArray[i], zAxis);
//    drawSphere(rad);
    for (auto i = 0; i < 4; i++)
    {
        auto transVector = point(center_expand, center_expand, center_expand) * translatePointArray[i];
        glPushMatrix();
        {
            translate(transVector);
            rotate(angleArray[i], zAxis);
            drawSphere(rad);
        }
        glPopMatrix();
    }
}

void translateSphere(double center_expand, double rad)
{
    setColor(RED);

    drawTranslatedSphereQuarter(center_expand, rad);
//    translate(transVector);
//    rotate(angleArray[i], zAxis);
//    drawSphere(rad);
    glPushMatrix();
    {
        rotate(180, xAxis);
        drawTranslatedSphereQuarter(center_expand, rad);
    }
    glPopMatrix();
}


void translateSquare(double center_expand, double SIZE) // Total 4 Squares
{
//    translate(transVector);
//    rotate(angleArray[i], zAxis);
//    drawSphere(radius);
    setColor(WHITE);
    double rotationAngleArray[] = {90, 90, 90, 90, 0, 0};
    point rotations[] = {yAxis, yAxis, xAxis, xAxis, point(), point()};
    point directions[] = {xAxis, _xAxis, yAxis, _yAxis, zAxis, _zAxis};



    for (auto k=0; k<6; k++)
    {
        auto transVector = point(center_expand + SIZE, center_expand + SIZE, center_expand + SIZE)*directions[k];

        glPushMatrix();
        {
            translate(transVector);
            rotate(rotationAngleArray[k], rotations[k]);
            drawOctahedron(center_expand);
        }
        glPopMatrix();
    }
}

void drawCylinderSide(double center_expand, double radius)
{
    GLdouble vertical_Angle_Array[] = {0, 270};
    point vertical_point_Array[] = {point(1, 1, 0), point(1, -1, 0)};

    for (auto k=0; k<4; k++)
    {
        auto transVector = point(center_expand, center_expand, 0)*vertical_point_Array[k%2];
        glPushMatrix();
        {
            if (k>1)
                rotate(90, xAxis);
            translate(transVector);
            rotate(vertical_Angle_Array[k % 2], zAxis);
            drawCylinder(radius, center_expand);
        }
        glPopMatrix();
    }
}


void translateCylinder(double center_expand, double rad) // Total Cylinders = 12
{
    setColor(GREEN);

    vector<GLdouble> angle_array = {0, 90, 180, 270};
    for (auto& angle : angle_array)
    {
        glPushMatrix();
        {
            rotate(angle, zAxis);
            drawCylinderSide(center_expand, rad);
        }
        glPopMatrix();
    }
}


void drawObjects()
{

    drawAxes();
    drawGrid();

    translateSphere(center_expand, sphere_size);

    translateCylinder(center_expand, sphere_size);

    translateSquare(center_expand, sphere_size);
}


void setupCamera();

void animate()
{
    glutPostRedisplay();
}

void display()
{
    clearDisplay();
    setupCamera();
    drawObjects();
    glutSwapBuffers();
}

void setupCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.position().x, camera.position().y, camera.position().z, camera.position().x + camera.left().x,camera.position().y + camera.left().y, camera.position().z + camera.left().z, camera.up().x,camera.up().y, camera.up().z);
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
    drawaxes = 1;
    drawgrid = 0;
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, 1, 1, 1000.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("1705111_Task_1_2");

    init();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);
    glutKeyboardFunc(keyboardListener);

    glutMainLoop();
    return 0;
}


void mouseListener(int KEY_PRESSED, int state, int x, int y)
{
    switch (KEY_PRESSED)
    {
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN)
        {
            drawaxes = 1 - drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            drawgrid = !drawgrid;
        }
        break;

    case GLUT_MIDDLE_BUTTON:
        break;

    default:
        break;
    }
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:        //down arrow key
		camera.mvBackward();
		break;
	case GLUT_KEY_UP:        // up arrow key
		camera.mvForward();
		break;

	case GLUT_KEY_RIGHT:
		camera.mvRight();
		break;
	case GLUT_KEY_LEFT:
		camera.mvLeft();
		break;

	case GLUT_KEY_PAGE_UP:
		camera.mvUP();
		break;
	case GLUT_KEY_PAGE_DOWN:
		camera.mvDown();
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		if (sphere_size != 0) {
			sphere_size -= 1;
			center_expand += 1;
		}

		break;
	case GLUT_KEY_END:
		if (center_expand != 0) {
			sphere_size += 1;
			center_expand -= 1;
		}
		break;

	default:
		break;
	}
}

void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

	case '1': //rotate/look left
		camera.lookLeft();
		break;
	case '2': //rotate/look right
		camera.lookRight();
		break;

	case '3': //look up
		camera.lookUp();
		break;
	case '4': //look down
		camera.lookDown();
		break;

	case '5':
		camera.tiltCounterClockwise();
		break;
	case '6':
		camera.tiltClockwise();
		break;
	case ',':
		if (sphere_size != 0) {
			sphere_size -= 1;
			center_expand += 1;
		}
		break;
	case '.':
		if (center_expand != 0) {
			sphere_size += 1;
			center_expand -= 1;
		}
		break;
	default:
		break;
	}
}
