#include <bits/stdc++.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

#define Ponto pair<float, float>

// armazena os pontos do fecho convexo (points of convex hull)
set<Ponto> hull;

GLfloat xf, yf, win;
GLint view_w, view_h;
vector<Ponto> a;
vector<Ponto> teste;

// retorna de que lado o ponto p está em relação a linha entre p1 e p2
int findSide(Ponto p1, Ponto p2, Ponto p)
{
    int val = (p.second - p1.second) * (p2.first - p1.first) -
              (p2.second - p1.second) * (p.first - p1.first);

    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

// retorna a distância entre o ponto p e a linha entre p1 e p2 
int distanciaLinha(Ponto p1, Ponto p2, Ponto p)
{
    return abs((p.second - p1.second) * (p2.first - p1.first) -
               (p2.second - p1.second) * (p.first - p1.first));
}


void quickHull(vector<Ponto> a, int n, Ponto p1, Ponto p2, int side)
{
    int ind = -1;
    int distanciaMax = 0;

    // Encontra o ponto mais distante da linha formada por p1 e p2
    for (int i = 0; i < n; i++)
    {
        int temp = distanciaLinha(p1, p2, a[i]);
        if (findSide(p1, p2, a[i]) == side && temp > distanciaMax)
        {
            ind = i;
            distanciaMax = temp;
        }
    }

    // Se nenhum ponto é encontrado
    // adiciona os pontos p1 e p2 ao fecho convexo.
    if (ind == -1)
    {
        hull.insert(p1);
        hull.insert(p2);
        teste.push_back(p1);
        teste.push_back(p2);
        return;
    }

    // Chama a recursão para as duas partes dividindo em a[ind]
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
}

void quickhull(vector<Ponto> a, int n)
{
    // limpa o eixo convexo
    hull.clear();
    teste.clear();

    if (n < 3)
    {
        cout << "Não é possível calcular o fecho convexo.";
        return;
    }

    // Encontra o ponto máximo e mínimo no eixo x
    int minX = 0, maxX = 0;
    for (int i = 1; i < n; i++)
    {
        if (a[i].first < a[minX].first)
            minX = i;
        if (a[i].first > a[maxX].first)
            maxX = i;
    }

    // Recursivamente encontra os pontos do fecho convexo
    // de um lado da linha que liga a[maxX] e a[minX]
    quickHull(a, n, a[minX], a[maxX], 1);

    // Recursivamente encontra os pontos do fecho convexo
    // do outro lado da linha que liga a[maxX] e a[minX]
    quickHull(a, n, a[minX], a[maxX], -1);
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
    int i, j;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);
    // Desenha um retângulo preenchido com a cor corrente
    glPointSize(10);

    glColor3f(1.0f, 0.0f, 0.0f);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // desenha os pontos na tela
    for (Ponto p : a)
    {
        glBegin(GL_POINTS);
        glVertex2f(p.first, p.second);
        glEnd();
    }

    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(2.5);
    glBegin(GL_LINES);

    // desenha as linhas do fecho convexo
    for (Ponto aux : teste)
    {
        glVertex2f(aux.first, aux.second);
    }
    glEnd();

    glEnd();
    glFlush();
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    xf = 50.0f;
    yf = 50.0f;
    win = 250.0f;
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-win, win, -win, win);
}

// Função callback chamada para gerenciar eventos de teclado
void GerenciaTeclado(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q': // executa o quickhull 
        quickhull(a, a.size());
        break;
    case 'c': // limpa a tela e os vetores de apoio
        hull.clear();
        teste.clear();
        a.clear();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN)
        {
            Ponto ponto;
            ponto.first = ((2 * win * x) / view_w) - win;
            ponto.second = (((2 * win) * (y - view_h)) / -view_h) - win;
            a.push_back(ponto);

        }
    glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do teclado
// para teclas especiais, tais como F1, PgDn e Home
void TeclasEspeciais(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        win -= 20;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-win, win, -win, win);
    }
    if (key == GLUT_KEY_DOWN)
    {
        win += 20;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-win, win, -win, win);
    }
    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1200, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("QuickHull");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(GerenciaTeclado);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(TeclasEspeciais);
    Inicializa();
    glutMainLoop();
}
