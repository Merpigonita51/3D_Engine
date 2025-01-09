#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdbool.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float vert[] = {1, 1, 0, 1, -1, 0, -1, -1, 0, -1, 1, 0};
float xAlfa = 20;
float zAlfa = 20;
POINTFLOAT pos = {0, 0};
float piramid[] = {0, 0, 2, 1, 1, 0, 1, -1, 0, -1, -1, 0, -1, 1, 0, 1, 1, 0};

float Kube[] = {0, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2};
GLuint KubeInd[] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 3, 2, 5, 6, 7, 3, 0, 1, 5, 5, 4, 0,
                    1, 2, 6, 6, 5, 1, 0, 3, 7, 7, 4, 0};
float a = 0;

// Параметры куба
float cubeX = 0; // Положение куба по X
float cubeY = 10; // Положение куба по Y
float cubeZ = 0; // Положение куба по Z
float cubeSize = 2; // Размер куба

void WndResize(int x, int y)
{
    glViewport(0, 0, x, y);
    float k = x / (float)y;
    float sz = 0.1;
    glLoadIdentity();
    glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 100);
}

void ShowPiramid(float x, float y, float z, float r, float g, float b)
{
    glVertexPointer(3, GL_FLOAT, 0, &piramid);
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(r, g, b);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glPopMatrix();
}

void ShowKube(float x, float y, float z, float r, float g, float b)
{
    // Определяем нормали для куба
    float normals[][3] = {
        {0, 0, -1}, {0, 0, 1},   // передняя и задняя грани
        {0, -1, 0}, {0, 1, 0},   // нижняя и верхняя грани
        {-1, 0, 0}, {1, 0, 0}    // левая и правая грани
    };

    GLuint indices[][4] = {
        {0, 1, 2, 3}, // передняя грань
        {4, 5, 6, 7}, // задняя грань
        {0, 3, 7, 4}, // левая грань
        {1, 2, 6, 5}, // правая грань
        {0, 1, 5, 4}, // нижняя грань
        {3, 2, 6, 7}  // верхняя грань
    };

    // Отрисовка граней с нормалями
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(r,g,b);

    for (int i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        glNormal3f(normals[i][0], normals[i][1], normals[i][2]); // Устанавливаем нормаль

        for (int j = 0; j < 4; j++) {
            glVertex3f(Kube[indices[i][j] * 3], Kube[indices[i][j] * 3 + 1], Kube[indices[i][j] * 3 + 2]);
        }
        glEnd();
    }

    // Отображение ребер куба
    glColor3f(1.0f, 1.0f, 1.0f); // Цвет линий (белый)
    glBegin(GL_LINES);
    // Передняя грань
    glVertex3f(Kube[0 * 3], Kube[0 * 3 + 1], Kube[0 * 3 + 2]);
    glVertex3f(Kube[1 * 3], Kube[1 * 3 + 1], Kube[1 * 3 + 2]);
    glVertex3f(Kube[1 * 3], Kube[1 * 3 + 1], Kube[1 * 3 + 2]);
    glVertex3f(Kube[2 * 3], Kube[2 * 3 + 1], Kube[2 * 3 + 2]);
    glVertex3f(Kube[2 * 3], Kube[2 * 3 + 1], Kube[2 * 3 + 2]);
    glVertex3f(Kube[3 * 3], Kube[3 * 3 + 1], Kube[3 * 3 + 2]);
    glVertex3f(Kube[3 * 3], Kube[3 * 3 + 1], Kube[3 * 3 + 2]);
    glVertex3f(Kube[0 * 3], Kube[0 * 3 + 1], Kube[0 * 3 + 2]);

    // Задняя грань
    glVertex3f(Kube[4 * 3], Kube[4 * 3 + 1], Kube[4 * 3 + 2]);
    glVertex3f(Kube[5 * 3], Kube[5 * 3 + 1], Kube[5 * 3 + 2]);
    glVertex3f(Kube[5 * 3], Kube[5 * 3 + 1], Kube[5 * 3 + 2]);
    glVertex3f(Kube[6 * 3], Kube[6 * 3 + 1], Kube[6 * 3 + 2]);
    glVertex3f(Kube[6 * 3], Kube[6 * 3 + 1], Kube[6 * 3 + 2]);
    glVertex3f(Kube[7 * 3], Kube[7 * 3 + 1], Kube[7 * 3 + 2]);
    glVertex3f(Kube[7 * 3], Kube[7 * 3 + 1], Kube[7 * 3 + 2]);
    glVertex3f(Kube[4 * 3], Kube[4 * 3 + 1], Kube[4 * 3 + 2]);

    // Соединения между передней и задней гранями
    glVertex3f(Kube[0 * 3], Kube[0 * 3 + 1], Kube[0 * 3 + 2]);
    glVertex3f(Kube[4 * 3], Kube[4 * 3 + 1], Kube[4 * 3 + 2]);
    glVertex3f(Kube[1 * 3], Kube[1 * 3 + 1], Kube[1 * 3 + 2]);
    glVertex3f(Kube[5 * 3], Kube[5 * 3 + 1], Kube[5 * 3 + 2]);
    glVertex3f(Kube[2 * 3], Kube[2 * 3 + 1], Kube[2 * 3 + 2]);
    glVertex3f(Kube[6 * 3], Kube[6 * 3 + 1], Kube[6 * 3 + 2]);
    glVertex3f(Kube[3 * 3], Kube[3 * 3 + 1], Kube[3 * 3 + 2]);
    glVertex3f(Kube[7 * 3], Kube[7 * 3 + 1], Kube[7 * 3 + 2]);
    glEnd();

    // Отображение вершин куба
    glPointSize(5.0f); // Увеличиваем размер точек
    glBegin(GL_POINTS);
    for (int i = 0; i < 8; i++) {
        glVertex3f(Kube[i * 3], Kube[i * 3 + 1], Kube[i * 3 + 2]);
    }
    glEnd();

    glPopMatrix();
}



void ShowWorld()
{
    a += 0.1;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vert);
    for (int i = -50; i < 50; i++)
        for (int j = -50; j < 50; j++)
        {
            glPushMatrix();
            if ((i + j) % 2 == 0) glColor3f(0, 0.5, 0);
            else glColor3f(0, 0.3, 0);
            glTranslatef(i * 2, j * 2, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
    ShowKube(10, 12, 0, 1, 0, 0.5);
    ShowKube(10, 10, 0, 1, 0, 1);
    ShowKube(10, 8, 0, 0.5, 0, 1);
    ShowKube(10, 6, 0, 0, 0, 1);
    ShowKube(10, 4, 0, 0, 1, 1);
    ShowKube(10, 2, 0, 0, 1, 0);
    ShowKube(10, 0, 0, 1, 1, 0);
    ShowKube(10, -2, 0, 1, 0.5, 0);
    ShowKube(10, -4, 0, 1, 0, 0);

    ShowKube(10, 12, 2, 1, 0, 0.5);
    ShowKube(10, 10, 2, 1, 0, 1);
    ShowKube(10, 8, 2, 0.5, 0, 1);
    ShowKube(10, 6, 2, 0, 0, 1);
    ShowKube(10, 4, 2, 0, 1, 1);
    ShowKube(10, 2, 2, 0, 1, 0);
    ShowKube(10, 0, 2, 1, 1, 0);
    ShowKube(10, -2, 2, 1, 0.5, 0);
    ShowKube(10, -4, 2, 1, 0, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
}

bool CheckCollision(float newX, float newY, float newZ)
{
    // Проверка на столкновение с кубом
    return (newX >= cubeX - cubeSize && newX <= cubeX + cubeSize &&
            newY >= cubeY - cubeSize && newY <= cubeY + cubeSize &&
            newZ >= cubeZ - cubeSize && newZ <= cubeZ + cubeSize);
}

void MoveCamera()
{
    if (GetKeyState(VK_UP) < 0) xAlfa = xAlfa++ > 180 ? 180 : xAlfa;
    if (GetKeyState(VK_DOWN) < 0) xAlfa = xAlfa-- < 0 ? 0 : xAlfa;
    if (GetKeyState(VK_LEFT) < 0) zAlfa++;
    if (GetKeyState(VK_RIGHT) < 0) zAlfa--;

    float ugol = -zAlfa / 180 * M_PI;
    float speed = 0;
    if (GetKeyState('S') < 0) speed = 0.1;
    if (GetKeyState('W') < 0) speed = -0.1;
    if (GetKeyState('D') < 0) { speed = 0.1; ugol -= M_PI * 0.5; }
    if (GetKeyState('A') < 0) { speed = 0.1; ugol += M_PI * 0.5; }
    if (GetKeyState(VK_SHIFT) < 0 && GetKeyState('W') < 0) speed = -0.5;
    if (GetKeyState(VK_SHIFT) < 0 && GetKeyState('S') < 0) speed = 0.5;

    if (speed != 0)
    {
        float newX = pos.x + sin(ugol) * speed;
        float newY = pos.y + cos(ugol) * speed;
        float newZ = 0; // Задаем высоту камеры

        // Проверка на столкновение
        if (CheckCollision(newX, newY, newZ) == true)
        {
            pos.x = 0;
            pos.y = 0;
        }
        else{
            pos.x = newX;
            pos.y = newY;
        }
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(pos.x, pos.y, -3);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Simple 3D",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          1000,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1, 1, -1, 1, 2, 80);
    glEnable(GL_DEPTH_TEST);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            MoveCamera();
            ShowWorld();
            glPopMatrix();

            SwapBuffers(hDC);

            Sleep(1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            WndResize(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
