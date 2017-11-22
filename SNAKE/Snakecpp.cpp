#include <iostream>
#include <glut.h>
#include <time.h>
#define WIDTH 25
#define HEIGHT 15//объявляем константы ширины и высоты в блоках

using namespace std;

class game {
	int StartSize;
public:
	int zaderjka_on_timer;
	float koef_mash;
	int StartPosX;
	int StartPosY;

	void set_start_size(int s);
	int get_start_size();
	game() {
		int s;
		cout << "Еnter the initial size of the snake: ";
		cin >> s;
		set_start_size(s);
		
		StartPosX = (int)(WIDTH / 2);
		StartPosY = (int)(HEIGHT / 2);//начальные координаты головы змейки
		zaderjka_on_timer = 100; //задержка на таймере
		koef_mash = 1; //коэфициент масштабирования
	}

	void init();
}settings;

void game::set_start_size(int s) {
	StartSize = s;
}

int game::get_start_size() {
	return StartSize;
}

class block {
public:
	int x;
	int y;
	block() {
		x = -1;
		y = -1;
	}
	void draw();
}blocks[WIDTH*HEIGHT];

class food : public block {
public:
	void draw_food();
	void spawn();
}food;

class snake {
public:
	int direction;
	int size;
	int score = 0;
	bool turn;//запрещает нажать две клавиши сразу
	snake() {
		turn = false;
		size = settings.get_start_size();
		direction = GLUT_KEY_UP;//движение змейки вверх
	}
	void DrawSnake();//методы
	void move();
	void cut();
	void eat();
}player;

void food::draw_food() {
	glColor3f(0, 0, 1);
	glVertex2f(x*settings.koef_mash+ settings.koef_mash/2, y*settings.koef_mash);
	glVertex2f(x*settings.koef_mash+settings.koef_mash, y*settings.koef_mash+ settings.koef_mash/2);
	glVertex2f(x*settings.koef_mash + settings.koef_mash/2, y*settings.koef_mash + settings.koef_mash);
	glVertex2f(x*settings.koef_mash, y*settings.koef_mash + settings.koef_mash/2);
}

void block::draw() {
	glColor3f(0, 1, 0);
	glVertex2f(x*settings.koef_mash + settings.koef_mash / 2, y*settings.koef_mash);
	glVertex2f(x*settings.koef_mash + settings.koef_mash, y*settings.koef_mash + settings.koef_mash / 2);
	glVertex2f(x*settings.koef_mash + settings.koef_mash / 2, y*settings.koef_mash + settings.koef_mash);
	glVertex2f(x*settings.koef_mash, y*settings.koef_mash + settings.koef_mash / 2);
}//задание четырех точек для квадрата
void snake::move() {
	for (int i = size; i > 0; i--) {
		blocks[i].x = blocks[i - 1].x;
		blocks[i].y = blocks[i - 1].y;
	}
	switch (direction) {
	case GLUT_KEY_UP:
		blocks[0].y++;
		break;
	case GLUT_KEY_DOWN:
		blocks[0].y--;
		break;
	case GLUT_KEY_LEFT:
		blocks[0].x--;
		break;
	case GLUT_KEY_RIGHT:
		blocks[0].x++;
		break;
	}
	if (blocks[0].x < 0)blocks[0].x += WIDTH;
	if (blocks[0].x >= WIDTH)blocks[0].x -= WIDTH;
	if (blocks[0].y < 0)blocks[0].y += HEIGHT;
	if (blocks[0].y >= HEIGHT)blocks[0].y -= HEIGHT;
}

void snake::DrawSnake() {
	for (int i = 0; i < size; i++) {
		blocks[i].draw();
	}
}

void game::init() {//начальное положение головы змейки
	blocks[0].x = StartPosX;
	blocks[0].y = StartPosY;
}

void snake::eat() {
	if ((blocks[0].x == food.x) && (blocks[0].y == food.y)) {
		size++;
		player.score++;
		MessageBeep(MB_ICONHAND);
		food.spawn();//респавн еды
	}
}

void snake::cut() {
	for (int i = 3; i < size; i++) {
		if ((blocks[0].x == blocks[i].x) && (blocks[0].y == blocks[i].y)) {
			size = 0;
			cout << "GG WP !!!" << endl;
			cout << "Your score= " << player.score;
			glutIconifyWindow();
		}
	}
}

void food::spawn() {
	x = (rand() % WIDTH);
	y = (rand() % HEIGHT);//появляется еда в пределах поля рандомно
	for (int i = 0; i < player.size; i++) {
		if ((x == blocks[i].x) && (y == blocks[i].y))spawn();
	}//если координаты еды совпадут с координатой змейки, то цикл идет дальше
}

void keyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if ((player.direction != GLUT_KEY_DOWN) && (player.turn))player.direction = GLUT_KEY_UP;
		player.turn = false;
		break;
	case GLUT_KEY_DOWN:
		if ((player.direction != GLUT_KEY_UP) && (player.turn))player.direction = GLUT_KEY_DOWN;
		player.turn = false;
		break;
	case GLUT_KEY_LEFT:
		if ((player.direction != GLUT_KEY_RIGHT) && (player.turn))player.direction = GLUT_KEY_LEFT;
		player.turn = false;
		break;
	case GLUT_KEY_RIGHT:
		if ((player.direction != GLUT_KEY_LEFT) && (player.turn))player.direction = GLUT_KEY_RIGHT;
		player.turn = false;
		break;
	}
}

void Timer(int value) {
	player.turn = true;
	player.move();
	player.cut();
	player.eat();
	glutPostRedisplay();//перерисовка 
	glutTimerFunc(settings.zaderjka_on_timer, Timer, 0);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);//очищение буфера цветов
	glBegin(GL_QUADS);//четыре точки- это четырехугольник
	food.draw_food();
	player.DrawSnake();
	glEnd();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{


	settings.init();//создание изначального положения змейки
	food.spawn();//создание еды
	srand(time(NULL));//генератор случайных чисел
	glutInit(&argc, argv);//создание окон
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//настройка окна

	glutCreateWindow("Snake Game");//создание окна и заголовка

	glutFullScreen();
	glClearColor(1.0, 1.0, 0.6, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//считывает текущую матрицу
	gluOrtho2D(0, WIDTH, 0, HEIGHT);//обозначение плоскости

	glutDisplayFunc(draw);//отображение на окне
	glutSpecialFunc(keyboard);//функция для клавиатуры
	glutTimerFunc(settings.zaderjka_on_timer, Timer, 0);

	glutMainLoop();
}
