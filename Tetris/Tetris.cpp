#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <thread>
#include <cstdlib>
#include <wingdi.h>
//#include <chrono>
#include <fstream>
#include <map>
#include <string>

using namespace std;

namespace tetris {
    static const int left_frame = 20;
    static const int top_frame = 20;
    static const int right_frame = 460;
    static const int bottom_frame = 900;
    const int step = 2;
    bool play = true;
    float speed = 10;
    short k_speed = 1;
    int score = 0;
    string name;
}
//HWND hWnd = GetConsoleWindow();
//HDC hDC = GetDC(hWnd);
///////////////////////////////////////////////////////////////////////////////////////
struct coordinate {
    int x_top{ 0 };
    int y_top{ 0 };
    int x_bottom{ 0 };
    int y_bottom{ 0 };
};
//////////////////////////////////////////////////////////////////////////////////////
enum elem { O = 1, T, L, J, Z, S, I };
///////////////////////////////////////////////////////////////////////////////////////
class square_one {
protected:
    coordinate sqr1;
    coordinate sqr2;
    coordinate sqr3;
    coordinate sqr4;
public:
    explicit square_one(coordinate sqr1 = { 0,0,0,0 }, coordinate sqr2 = { 0,0,0,0 }, coordinate sqr3 = { 0,0,0,0 }, coordinate sqr4 = { 0,0,0,0 })
        :sqr1{ sqr1 }, sqr2{ sqr2 }, sqr3{ sqr3 }, sqr4{ sqr4 }{}
    virtual void print(HWND, HDC) const = 0;
    virtual void move(int) = 0;
    virtual void turn() = 0;
    virtual void sideways(int) = 0;
    virtual int get_x_sqr(int) const = 0;
    virtual int get_y_sqr(int) const = 0;
    virtual int get_position_x(int) = 0;
    virtual int get_position_y(int) = 0;
    virtual void set_x_sqr(int) = 0;
    virtual void set_y_sqr(int) = 0;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
class O_elem :public square_one {
public:
    explicit O_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x,y,x + 20,y + 20 }, { x + 22,y,x + 42,y + 20 }, { x,y + 22,x + 20,y + 42 }, { x + 22,y + 22,x + 42,y + 42 }) {}

    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void O_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
    //HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPen = CreatePen(0, 2, RGB(255, 249, 9));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    //ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    //DeleteObject(hDC);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
inline void O_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void O_elem::turn() {}
inline void O_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int O_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int O_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int O_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int O_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void O_elem::set_x_sqr(int x) {
    sqr1.x_top = x;
    sqr1.x_bottom = x + 20;
    sqr2.x_top = x + 22;
    sqr2.x_bottom = x + 42;
    sqr3.x_top = x;
    sqr3.x_bottom = x + 20;
    sqr4.x_top = x + 22;
    sqr4.x_bottom = x + 42;
}
inline void O_elem::set_y_sqr(int y) {
    sqr1.y_top = y;
    sqr1.y_bottom = y + 20;
    sqr2.y_top = y;
    sqr2.y_bottom = y + 20;
    sqr3.y_top = y + 22;
    sqr3.y_bottom = y + 42;
    sqr4.y_top = y + 22;
    sqr4.y_bottom = y + 42;
}
//////////////////////////////////////////////////////////////////////////////////////
class I_elem :public square_one {
    bool pos{ true };
public:
    explicit I_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x, y, x + 20, y + 20 }, { x,y + 22,x + 20,y + 42 }, { x,y + 44,x + 20,y + 64 }, { x,y + 66,x + 20,y + 86 }) {}

    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void I_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    /*PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);*/
    HPEN hPen = CreatePen(0, 2, RGB(3, 192, 207));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    //ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    //DeleteObject(hDC);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
inline void I_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void I_elem::turn() {
    if (pos == true) {
        sqr2 = sqr4;
        sqr1.x_top = sqr2.x_top - 22; sqr1.y_top = sqr2.y_top;
        sqr1.x_bottom = sqr2.x_bottom - 22; sqr1.y_bottom = sqr2.y_bottom;
        sqr3.x_top = sqr2.x_top + 22; sqr3.y_top = sqr2.y_top;
        sqr3.x_bottom = sqr3.x_top + 22; sqr3.y_bottom = sqr2.y_bottom;
        sqr4.x_top = sqr2.x_top + 44; sqr4.y_top = sqr2.y_top;
        sqr4.x_bottom = sqr2.x_bottom + 44; sqr4.y_bottom = sqr2.y_bottom;
        pos = false;
    }
    else {
        sqr4 = sqr2;
        sqr3.x_top = sqr4.x_top; sqr3.y_top = sqr4.y_top - 22;
        sqr3.x_bottom = sqr4.x_bottom; sqr3.y_bottom = sqr4.y_bottom - 22;
        sqr2.x_top = sqr4.x_top; sqr2.y_top = sqr4.y_top - 44;
        sqr2.x_bottom = sqr4.x_bottom; sqr2.y_bottom = sqr4.y_bottom - 44;
        sqr1.x_top = sqr4.x_top; sqr1.y_top = sqr4.y_top - 66;
        sqr1.x_bottom = sqr4.x_bottom; sqr1.y_bottom = sqr4.y_bottom - 66;
        pos = true;
    }
}
inline void I_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int I_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int I_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int I_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int I_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void I_elem::set_x_sqr(int x) {
    sqr1.x_top = x;
    sqr1.x_bottom = x + 20;
    sqr2.x_top = x;
    sqr2.x_bottom = x + 20;
    sqr3.x_top = x;
    sqr3.x_bottom = x + 20;
    sqr4.x_top = x;
    sqr4.x_bottom = x + 20;
}
inline void I_elem::set_y_sqr(int y) {
    sqr1.y_top = y;
    sqr1.y_bottom = y + 20;
    sqr2.y_top = y + 22;
    sqr2.y_bottom = y + 42;
    sqr3.y_top = y + 44;
    sqr3.y_bottom = y + 64;
    sqr4.y_top = y + 66;
    sqr4.y_bottom = y + 86;
}
/////////////////////////////////////////////////////////////////////////////
class Z_elem :public square_one {
    bool pos{ true };
public:
    explicit Z_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x - 22, y, x - 2, y + 20 }, { x, y, x + 20,y + 20 }, { x,y + 22,x + 20,y + 42 }, { x + 22,y + 22,x + 42,y + 42 }) {}

    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void Z_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
    //HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPen = CreatePen(0, 2, RGB(210, 0, 0));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    // ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    // DeleteObject(hDC);
     //DeleteObject(hWnd);
     //EndPaint(hWnd, &ps);
}
inline void Z_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void Z_elem::turn() {
    if (pos == true) {
        sqr4 = sqr3;
        sqr3 = sqr2;
        sqr1.x_top = sqr3.x_top + 22; sqr1.y_top = sqr3.y_top - 22;
        sqr1.x_bottom = sqr3.x_bottom + 22; sqr1.y_bottom = sqr3.y_bottom - 22;
        sqr2.x_top = sqr3.x_top + 22; sqr2.y_top = sqr3.y_top;
        sqr2.x_bottom = sqr3.x_bottom + 22; sqr2.y_bottom = sqr3.y_bottom;
        pos = false;
    }
    else {
        sqr2 = sqr3;
        sqr3 = sqr4;
        sqr1.x_top = sqr2.x_top - 22; sqr1.y_top = sqr2.y_top;
        sqr1.x_bottom = sqr2.x_bottom - 22; sqr1.y_bottom = sqr2.y_bottom;
        sqr4.x_top = sqr3.x_top + 22; sqr4.y_top = sqr3.y_top;
        sqr4.x_bottom = sqr3.x_bottom + 22; sqr4.y_bottom = sqr3.y_bottom;
        pos = true;
    }
}
inline void Z_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int Z_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int Z_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int Z_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int Z_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void Z_elem::set_x_sqr(int x) {
    sqr1.x_top = x - 22;
    sqr1.x_bottom = x - 2;
    sqr2.x_top = x;
    sqr2.x_bottom = x + 20;
    sqr3.x_top = x;
    sqr3.x_bottom = x + 20;
    sqr4.x_top = x + 22;
    sqr4.x_bottom = x + 42;
}
inline void Z_elem::set_y_sqr(int y) {
    sqr1.y_top = y;
    sqr1.y_bottom = y + 20;
    sqr2.y_top = y;
    sqr2.y_bottom = y + 20;
    sqr3.y_top = y + 22;
    sqr3.y_bottom = y + 42;
    sqr4.y_top = y + 22;
    sqr4.y_bottom = y + 42;
}
////////////////////////////////////////////////////////////////////////////////////
class S_elem :public square_one {
    bool pos{ true };
public:
    explicit S_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x - 22, y, x - 2, y + 20 }, { x, y, x + 20,y + 20 }, { x,y - 22,x + 20,y - 2 }, { x + 22,y - 22,x + 42,y - 2 }) {}

    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void S_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
    //HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPen = CreatePen(0, 2, RGB(62, 206, 4));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    //ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    //DeleteObject(hDC);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
inline void S_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void S_elem::turn() {
    if (pos == true) {
        //sqr3 no chsnge
        sqr4 = sqr2;
        sqr1.x_top = sqr3.x_top - 22; sqr1.y_top = sqr3.y_top - 22;
        sqr1.x_bottom = sqr3.x_bottom - 22; sqr1.y_bottom = sqr3.y_bottom - 22;
        sqr2.x_top = sqr3.x_top - 22; sqr2.y_top = sqr3.y_top;
        sqr2.x_bottom = sqr3.x_bottom - 22; sqr2.y_bottom = sqr3.y_bottom;
        pos = false;
    }
    else {
        //sqr3 no change
        sqr2 = sqr4;
        sqr1.x_top = sqr3.x_top - 22; sqr1.y_top = sqr3.y_top + 22;
        sqr1.x_bottom = sqr3.x_bottom - 22; sqr1.y_bottom = sqr3.y_bottom + 22;
        sqr4.x_top = sqr3.x_top + 22; sqr4.y_top = sqr3.y_top;
        sqr4.x_bottom = sqr3.x_bottom + 22; sqr4.y_bottom = sqr3.y_bottom;
        pos = true;
    }
}
inline void S_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int S_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int S_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int S_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int S_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void S_elem::set_x_sqr(int x) {
    sqr1.x_top = x - 22;
    sqr1.x_bottom = x - 2;
    sqr2.x_top = x;
    sqr2.x_bottom = x + 20;
    sqr3.x_top = x;
    sqr3.x_bottom = x + 20;
    sqr4.x_top = x + 22;
    sqr4.x_bottom = x + 42;
}
inline void S_elem::set_y_sqr(int y) {
    sqr1.y_top = y;
    sqr1.y_bottom = y + 20;
    sqr2.y_top = y;
    sqr2.y_bottom = y + 20;
    sqr3.y_top = y - 22;
    sqr3.y_bottom = y - 2;
    sqr4.y_top = y - 22;
    sqr4.y_bottom = y - 2;
}
///////////////////////////////////////////////////////////////////////////////////////
class L_elem :public square_one {
    short pos{ 1 };
public:
    explicit L_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x, y - 22, x + 20, y - 2 }, { x, y, x + 20,y + 20 }, { x,y + 22,x + 20,y + 42 }, { x + 22,y + 22,x + 42,y + 42 }) {}

    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void L_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
    //HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPen = CreatePen(0, 2, RGB(210, 90, 0));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    //ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    //DeleteObject(hDC);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
inline void L_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void L_elem::turn() {
    if (pos == 1) {
        //sqr2 no change
        sqr1.x_top = sqr2.x_top + 22; sqr1.y_top = sqr2.y_top;
        sqr1.x_bottom = sqr2.x_bottom + 22; sqr1.y_bottom = sqr2.y_bottom;
        sqr3.x_top = sqr2.x_top - 22; sqr3.y_top = sqr2.y_top;
        sqr3.x_bottom = sqr2.x_bottom - 22; sqr3.y_bottom = sqr2.y_bottom;
        sqr4.x_top = sqr2.x_top - 22; sqr4.y_top = sqr2.y_top + 22;
        sqr4.x_bottom = sqr2.x_bottom - 22; sqr4.y_bottom = sqr2.y_bottom + 22;
        pos = 2;
    }
    else
        if (pos == 2) {
            //sqr2 no change
            sqr1.x_top = sqr2.x_top; sqr1.y_top = sqr2.y_top + 22;
            sqr1.x_bottom = sqr2.x_bottom; sqr1.y_bottom = sqr2.y_bottom + 22;
            sqr3.x_top = sqr2.x_top; sqr3.y_top = sqr2.y_top - 22;
            sqr3.x_bottom = sqr2.x_bottom; sqr3.y_bottom = sqr2.y_bottom - 22;
            sqr4.x_top = sqr2.x_top - 22; sqr4.y_top = sqr2.y_top - 22;
            sqr4.x_bottom = sqr2.x_bottom - 22; sqr4.y_bottom = sqr2.y_bottom - 22;
            pos = 3;
        }
        else
            if (pos == 3) {
                sqr2 = sqr1;
                sqr1.x_top = sqr2.x_top - 22; sqr1.y_top = sqr2.y_top;
                sqr1.x_bottom = sqr2.x_bottom - 22; sqr1.y_bottom = sqr2.y_bottom;
                sqr3.x_top = sqr2.x_top + 22; sqr3.y_top = sqr2.y_top;
                sqr3.x_bottom = sqr2.x_bottom + 22; sqr3.y_bottom = sqr2.y_bottom;
                sqr4.x_top = sqr2.x_top + 22; sqr4.y_top = sqr2.y_top - 22;
                sqr4.x_bottom = sqr2.x_bottom + 22; sqr4.y_bottom = sqr2.y_bottom - 22;
                pos = 4;
            }
            else
                if (pos == 4) {
                    sqr4 = sqr3;
                    sqr3 = sqr2;
                    sqr1.x_top = sqr3.x_top; sqr1.y_top = sqr3.y_top - 44;
                    sqr1.x_bottom = sqr3.x_bottom; sqr1.y_bottom = sqr3.y_bottom - 44;
                    sqr2.x_top = sqr3.x_top; sqr2.y_top = sqr3.y_top - 22;
                    sqr2.x_bottom = sqr3.x_bottom; sqr2.y_bottom = sqr3.y_bottom - 22;
                    pos = 1;
                }
}
inline void L_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int L_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int L_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int L_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int L_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void L_elem::set_x_sqr(int x) {
    sqr1.x_top = x;
    sqr1.x_bottom = x + 20;
    sqr2.x_top = x;
    sqr2.x_bottom = x + 20;
    sqr3.x_top = x;
    sqr3.x_bottom = x + 20;
    sqr4.x_top = x + 22;
    sqr4.x_bottom = x + 42;
}
inline void L_elem::set_y_sqr(int y) {
    sqr1.y_top = y - 22;
    sqr1.y_bottom = y - 2;
    sqr2.y_top = y;
    sqr2.y_bottom = y + 20;
    sqr3.y_top = y + 22;
    sqr3.y_bottom = y + 42;
    sqr4.y_top = y + 22;
    sqr4.y_bottom = y + 42;
}
//////////////////////////////////////////////////////////////////////////////////////////////
class J_elem :public square_one {
    short pos{ 1 };
public:
    explicit J_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x, y - 22, x + 20, y - 2 }, { x, y, x + 20,y + 20 }, { x,y + 22,x + 20,y + 42 }, { x - 22,y + 22,x - 2,y + 42 }) {}
    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void J_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
    //HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPen = CreatePen(0, 2, RGB(15, 6, 204));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    //ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    //DeleteObject(hDC);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
inline void J_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void J_elem::turn() {
    if (pos == 1) {
        sqr2 = sqr3;
        sqr3 = sqr4;
        sqr1.x_top = sqr2.x_top + 22; sqr1.y_top = sqr2.y_top;
        sqr1.x_bottom = sqr2.x_bottom + 22; sqr1.y_bottom = sqr2.y_bottom;
        sqr4.x_top = sqr2.x_top - 22; sqr4.y_top = sqr2.y_top - 22;
        sqr4.x_bottom = sqr2.x_bottom - 22; sqr4.y_bottom = sqr2.y_bottom - 22;
        pos = 2;
    }
    else
        if (pos == 2) {
            sqr1 = sqr2;
            sqr2.x_top = sqr1.x_top; sqr2.y_top = sqr1.y_top - 22;
            sqr2.x_bottom = sqr1.x_bottom; sqr2.y_bottom = sqr1.y_bottom - 22;
            sqr3.x_top = sqr1.x_top; sqr3.y_top = sqr1.y_top - 44;
            sqr3.x_bottom = sqr1.x_bottom; sqr3.y_bottom = sqr1.y_bottom - 44;
            sqr4.x_top = sqr1.x_top + 22; sqr4.y_top = sqr1.y_top - 44;
            sqr4.x_bottom = sqr1.x_bottom + 22; sqr4.y_bottom = sqr1.y_bottom - 44;
            pos = 3;
        }
        else
            if (pos == 3) {
                //sqr2 no change
                sqr1.x_top = sqr2.x_top - 22; sqr1.y_top = sqr2.y_top;
                sqr1.x_bottom = sqr2.x_bottom - 22; sqr1.y_bottom = sqr2.y_bottom;
                sqr3.x_top = sqr2.x_top + 22; sqr3.y_top = sqr2.y_top;
                sqr3.x_bottom = sqr2.x_bottom + 22; sqr3.y_bottom = sqr2.y_bottom;
                sqr4.x_top = sqr2.x_top + 22; sqr4.y_top = sqr2.y_top + 22;
                sqr4.x_bottom = sqr2.x_bottom + 22; sqr4.y_bottom = sqr2.y_bottom + 22;
                pos = 4;
            }
            else
                if (pos == 4) {
                    //sqr2 no change
                    sqr1.x_top = sqr2.x_top; sqr1.y_top = sqr2.y_top - 22;
                    sqr1.x_bottom = sqr2.x_bottom; sqr1.y_bottom = sqr2.y_bottom - 22;
                    sqr3.x_top = sqr2.x_top; sqr3.y_top = sqr2.y_top + 22;
                    sqr3.x_bottom = sqr2.x_bottom; sqr3.y_bottom = sqr2.y_bottom + 22;
                    sqr4.x_top = sqr2.x_top - 22; sqr4.y_top = sqr2.y_top + 22;
                    sqr4.x_bottom = sqr2.x_bottom - 22; sqr4.y_bottom = sqr2.y_bottom + 22;
                    pos = 1;
                }
}
inline void J_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int J_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int J_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int J_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int J_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void J_elem::set_x_sqr(int x) {
    sqr1.x_top = x;
    sqr1.x_bottom = x + 20;
    sqr2.x_top = x;
    sqr2.x_bottom = x + 20;
    sqr3.x_top = x;
    sqr3.x_bottom = x + 20;
    sqr4.x_top = x - 22;
    sqr4.x_bottom = x - 2;
}
inline void J_elem::set_y_sqr(int y) {
    sqr1.y_top = y - 22;
    sqr1.y_bottom = y - 2;
    sqr2.y_top = y;
    sqr2.y_bottom = y + 20;
    sqr3.y_top = y + 22;
    sqr3.y_bottom = y + 42;
    sqr4.y_top = y + 22;
    sqr4.y_bottom = y + 42;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
class T_elem :public square_one {
    short pos{ 1 };
public:
    explicit T_elem(int x = (tetris::right_frame - tetris::left_frame) / 2, int y = tetris::top_frame - 40)
        :square_one({ x - 22, y, x - 2, y + 20 }, { x, y, x + 20,y + 20 }, { x + 22,y,x + 42,y + 20 }, { x,y + 22,x + 20,y + 42 }) {}
    void print(HWND, HDC) const override;
    void move(int) override;
    void turn() override;
    void sideways(int) override;
    int get_x_sqr(int) const override;
    int get_y_sqr(int) const override;
    int get_position_x(int) override;
    int get_position_y(int) override;
    void set_x_sqr(int) override;
    void set_y_sqr(int) override;
};
inline void T_elem::print(HWND hWnd, HDC hDC) const {
    //HWND hWnd = GetConsoleWindow();
    ///HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
    //HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPen = CreatePen(0, 2, RGB(204, 0, 199));
    SelectObject(hDC, hPen);
    if (sqr1.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr1.x_top, sqr1.y_top, sqr1.x_bottom, sqr1.y_bottom);
    if (sqr2.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr2.x_top, sqr2.y_top, sqr2.x_bottom, sqr2.y_bottom);
    if (sqr3.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr3.x_top, sqr3.y_top, sqr3.x_bottom, sqr3.y_bottom);
    if (sqr4.y_top >= tetris::top_frame)
        Rectangle(hDC, sqr4.x_top, sqr4.y_top, sqr4.x_bottom, sqr4.y_bottom);
    //ReleaseDC(hWnd, hDC);
    DeleteObject(hPen);
    //DeleteObject(hDC);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
inline void T_elem::move(int step) {
    sqr1.y_top += step; sqr1.y_bottom += step;
    sqr2.y_top += step; sqr2.y_bottom += step;
    sqr3.y_top += step; sqr3.y_bottom += step;
    sqr4.y_top += step; sqr4.y_bottom += step;
}
inline void T_elem::turn() {
    if (pos == 1) {
        //sqr2 no change
        sqr3 = sqr4;
        sqr4 = sqr1;
        sqr1.x_top = sqr2.x_top; sqr1.y_top = sqr2.y_top - 22;
        sqr1.x_bottom = sqr2.x_bottom; sqr1.y_bottom = sqr2.y_bottom - 22;
        pos = 2;
    }
    else
        if (pos == 2) {
            sqr4 = sqr2;
            sqr2 = sqr3;
            sqr3.x_top = sqr2.x_top - 22; sqr3.y_top = sqr2.y_top;
            sqr3.x_bottom = sqr2.x_bottom - 22; sqr3.y_bottom = sqr2.y_bottom;
            sqr1.x_top = sqr2.x_top + 22; sqr1.y_top = sqr2.y_top;
            sqr1.x_bottom = sqr2.x_bottom + 22; sqr1.y_bottom = sqr2.y_bottom;
            pos = 3;
        }
        else
            if (pos == 3) {
                sqr1 = sqr2;
                sqr2 = sqr4;
                sqr3.x_top = sqr2.x_top; sqr3.y_top = sqr2.y_top - 22;
                sqr3.x_bottom = sqr2.x_bottom; sqr3.y_bottom = sqr2.y_bottom - 22;
                sqr4.x_top = sqr2.x_top + 22; sqr4.y_top = sqr2.y_top;
                sqr4.x_bottom = sqr2.x_bottom + 22; sqr4.y_bottom = sqr2.y_bottom;
                pos = 4;
            }
            else
                if (pos == 4) {
                    //sqr2 no change
                    sqr3 = sqr4;
                    sqr4 = sqr1;
                    sqr1.x_top = sqr2.x_top - 22; sqr1.y_top = sqr2.y_top;
                    sqr1.x_bottom = sqr2.x_bottom - 22; sqr1.y_bottom = sqr2.y_bottom;
                    pos = 1;
                }
}
inline void T_elem::sideways(int x) {
    sqr1.x_top += x;
    sqr1.x_bottom += x;
    sqr2.x_top += x;
    sqr2.x_bottom += x;
    sqr3.x_top += x;
    sqr3.x_bottom += x;
    sqr4.x_top += x;
    sqr4.x_bottom += x;
}
inline int T_elem::get_x_sqr(int n) const {
    if (n == 1) return sqr1.x_top;
    if (n == 2) return sqr2.x_top;
    if (n == 3) return sqr3.x_top;
    if (n == 4) return sqr4.x_top;
    return 0;
}
inline int T_elem::get_y_sqr(int n) const {
    if (n == 1) return sqr1.y_top;
    if (n == 2) return sqr2.y_top;
    if (n == 3) return sqr3.y_top;
    if (n == 4) return sqr4.y_top;
    return 0;
}
inline int T_elem::get_position_x(int n) {
    if (n == 1)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr1.x_top) / 22);
    if (n == 2)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr2.x_top) / 22);
    if (n == 3)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr3.x_top) / 22);
    if (n == 4)
        return 19 - ((tetris::right_frame - tetris::left_frame - sqr4.x_top) / 22);
}
inline int T_elem::get_position_y(int n) {
    if (n == 1)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr1.y_top) / 22));
    if (n == 2)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr2.y_top) / 22));
    if (n == 3)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr3.y_top) / 22));
    if (n == 4)
        return (39 - (int)((tetris::bottom_frame - tetris::top_frame - sqr4.y_top) / 22));
}
inline void T_elem::set_x_sqr(int x) {
    sqr1.x_top = x - 22;
    sqr1.x_bottom = x - 2;
    sqr2.x_top = x;
    sqr2.x_bottom = x + 20;
    sqr3.x_top = x + 22;
    sqr3.x_bottom = x + 42;
    sqr4.x_top = x;
    sqr4.x_bottom = x + 20;
}
inline void T_elem::set_y_sqr(int y) {
    sqr1.y_top = y;
    sqr1.y_bottom = y + 20;
    sqr2.y_top = y;
    sqr2.y_bottom = y + 20;
    sqr3.y_top = y;
    sqr3.y_bottom = y + 20;
    sqr4.y_top = y + 22;
    sqr4.y_bottom = y + 42;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void show_next(HWND hWnd, HDC hDC, square_one* ln) {
    ln->set_x_sqr(tetris::right_frame + 85);
    ln->set_y_sqr(tetris::top_frame + 210);
    ln->print(hWnd, hDC);
    //DeleteObject(hWnd);
    //ReleaseDC(hWnd, hDC);
}
/////////////////////////////////////////////////////////////////////////////////////
void frame(HWND hWnd, HDC hDC, square_one* ln, short array[][20]) {
    //HWND hWnd = GetConsoleWindow();
    //HDC hDC = GetDC(hWnd);
    //PAINTSTRUCT ps;
   // HDC hDC = BeginPaint(hWnd, &ps);
    HPEN hPenFrame = CreatePen(0, 1, RGB(255, 255, 255));
    POINT op;
    SelectObject(hDC, hPenFrame);
    MoveToEx(hDC, tetris::left_frame, tetris::top_frame, &op);
    LineTo(hDC, tetris::left_frame, tetris::bottom_frame);
    LineTo(hDC, tetris::right_frame, tetris::bottom_frame);
    LineTo(hDC, tetris::right_frame, tetris::top_frame);
    LineTo(hDC, tetris::left_frame, tetris::top_frame);

    show_next(hWnd, hDC, ln);

    for (int i{ 0 }; i < 40; ++i) {
        for (int j{ 0 }; j < 20; ++j) {
            int x = ((j - 19) * 22 + tetris::right_frame - tetris::left_frame);
            int y = ((i - 39) * 22 + tetris::bottom_frame - tetris::top_frame);
            HPEN hPenFig = CreatePen(0, 2, RGB(0, 0, 0));
            if (array[i][j] == O) {
                hPenFig = CreatePen(0, 2, RGB(255, 249, 9));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (array[i][j] == T) {
                hPenFig = CreatePen(0, 2, RGB(204, 0, 199));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (array[i][j] == L) {
                hPenFig = CreatePen(0, 2, RGB(210, 90, 0));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (array[i][j] == J) {
                hPenFig = CreatePen(0, 2, RGB(15, 6, 204));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (array[i][j] == Z) {
                hPenFig = CreatePen(0, 2, RGB(210, 0, 0));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (array[i][j] == S) {
                hPenFig = CreatePen(0, 2, RGB(62, 206, 4));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (array[i][j] == I) {
                hPenFig = CreatePen(0, 2, RGB(3, 192, 207));
                SelectObject(hDC, hPenFig);
                Rectangle(hDC, x, y, x + 20, y + 20);
                DeleteObject(hPenFig);
                continue;
            }
            if (hPenFig)
                DeleteObject(hPenFig);
        }
    }
    //ReleaseDC(hWnd, hDC);
    //DeleteObject(hPenFig);
    DeleteObject(hPenFrame);
    DeleteObject(&op);
    //DeleteObject(hWnd);
    //EndPaint(hWnd, &ps);
}
////////////////////////////////
void deletecursor() {
    void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &structCursorInfo);
}
//////////////////////////////////////////////////////
void clear(HWND hWnd, short array[][20])
{
    /*const unsigned FPS = 25;
    std::vector<char> frameData;
        bool bufferSwitch = true;
        auto firstBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        auto secondBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_WRITE | FILE_SHARE_READ,
            nullptr,
            CONSOLE_TEXTMODE_BUFFER,
            nullptr);
        HANDLE backBuffer = secondBuffer;

        WriteConsole(backBuffer, &frameData.front(), static_cast<short>(frameData.size()), nullptr, nullptr);
        SetConsoleActiveScreenBuffer(backBuffer);
        backBuffer = bufferSwitch ? firstBuffer : secondBuffer;
        bufferSwitch = !bufferSwitch;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));*/

        //HWND hwnd = GetConsoleWindow();
        //PAINTSTRUCT ps;
        //HDC hdc = /*GetDC(hwnd)*/BeginPaint(hwnd, &ps);
    //RECT rect;
    //GetClientRect(hWnd, &rect);
    //HDC mem_dc;
    //mem_dc = CreateCompatibleDC(hDC);
    //HBITMAP mem_bitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
    //SelectObject(mem_dc, mem_bitmap);
    ////frame(array);
    //BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, mem_dc, 0, 0, SRCCOPY);

    //DeleteObject(mem_bitmap);
    //DeleteObject(mem_dc);

    //COORD coord;
    //coord.X = 0;
    //coord.Y = 0;
    //CONSOLE_SCREEN_BUFFER_INFO screen;
    //COORD topLeft = { 0, 0 };
    //DWORD written;
    //GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);
    //FillConsoleOutputCharacterA(
    //    GetStdHandle(STD_OUTPUT_HANDLE), ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    //);
    ///*FillConsoleOutputAttribute(
    //    GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
    //    screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    //);*/
    //CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);


//    HDC hdcScreen = CreateDC(NULL, NULL, NULL, NULL);
//    //HDC hdcCompatible = CreateCompatibleDC(hdcScreen);
//
//    // Создаем совместимый точечный рисунок для параметра hdcScreen. 
//
//    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen,
//        GetDeviceCaps(hdcScreen, HORZRES),
//        GetDeviceCaps(hdcScreen, VERTRES));
//
//    // Выбираем совместимый точечный рисунок в DC. 
//
//    SelectObject(/*hdcCompatible*/hDC, hbmScreen);
//
//    // Скрываем окно приложения. 
//
//    ShowWindow(hWnd, SW_HIDE);
//
//    // Копируем цветовые данные для всего дисплея в точечный  
//    // рисунок, который выбирается в совместимый DC. 
//
//    BitBlt(/*hdcCompatible*/hDC,
//        0, 0,
//        rect.right - rect.left, rect.bottom - rect.top,
//        hdcScreen,
//        0, 0,
//        SRCCOPY);
//    //errhandler("Screen to Compat Blt Failed", hWnd);
//
//// Перерисовываем окно приложения. 
//
//    ShowWindow(hWnd, SW_SHOWNA);

};
////////////////////////////////////////
bool check_move(square_one* l, short array[][20]) {
    if ((array[l->get_position_y(1)][l->get_position_x(1)] <= 0 && array[l->get_position_y(2)][l->get_position_x(2)] <= 0 &&
        array[l->get_position_y(3)][l->get_position_x(3)] <= 0 && array[l->get_position_y(4)][l->get_position_x(4)] <= 0)
        && ((*l).get_y_sqr(1) <= (tetris::bottom_frame - 22) && (*l).get_y_sqr(2) <= (tetris::bottom_frame - 22) &&
            (*l).get_y_sqr(3) <= (tetris::bottom_frame - 22) && (*l).get_y_sqr(4) <= (tetris::bottom_frame - 22)))
        return true;
    return false;
}
//////////////////////////////////////////////////////
void gotoXY(int X, int Y) {
    /*RECT rect;
    GetClientRect(hWnd, &rect);*/
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cd;
    cd.X = X;
    cd.Y = Y;
    SetConsoleCursorPosition(hd, cd);
    //DeleteObject(hWnd);
    DeleteObject(hd);
    DeleteObject(&cd);
}
////////////////////////////////////////////////////////////////////////////////////
void go_down(HWND hWnd, HDC hDC, square_one* l, square_one* ln, short array[][20]) {
    gotoXY(40, 5);
    cout << "Your name: " << tetris::name;
    gotoXY(40, 8);
    cout << "Next element:";
    gotoXY(40, 17);
    cout << "Score: " << tetris::score;

    bool check = true;
    while (check && tetris::play) {
        //clear(hWnd, array);
        deletecursor();
        // system("cls");

       // HWND hwnd = GetConsoleWindow();

        //HDC hDC = GetDC(hWnd);

        /*PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);*/
        //RestoreDC(hDC, 1);

        frame(hWnd, hDC, ln, array);
        (*l).move(tetris::step);
        (*l).print(hWnd, hDC);
        check = check_move(l, array);

        //PAINTSTRUCT ps;
        //HDC hDC = BeginPaint(hWnd, &ps);
        //hDC = GetDC(hWnd);
        HDC mem_dc = CreateCompatibleDC(hDC);
        //BITMAP bm;
        //HBITMAP hBitmap;
        //GetObject(hDC, sizeof(BITMAP), &bm);

        RECT rect;
        rect.left = l->get_x_sqr(1);
        rect.right = l->get_x_sqr(4);
        rect.top = l->get_y_sqr(1);
        rect.bottom = l->get_y_sqr(4);
        //GetClientRect(hWnd, &rect);
        //HBITMAP mem_bitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
        //HBITMAP mem_bitmap = CreateCompatibleBitmap(hDC, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES));
        //StretchBlt(mem_dc, 0, 0, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES), hDC, 0, 0, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES), SRCCOPY);
        //BitBlt(hDC, 0, 0, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES), mem_dc, 0, 0, SRCCOPY);
        //HBITMAP old_bitmap = (HBITMAP);
        // SelectObject(mem_dc, mem_bitmap);
        //BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, mem_dc, 0, 0, SRCCOPY);
        //StretchBlt(mem_dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SRCCOPY);
        //Sleep(tetris::speed * tetris::k_speed);
        //SelectObject(hDC, old_bitmap);
        //InvalidateRgn(hWnd, (HRGN)&rect, TRUE);
        //InvalidateRect(hWnd, &rect, TRUE);
        //UpdateWindow(hWnd);
        //DeleteObject(mem_bitmap);
        //DeleteDC(mem_dc);
        //DeleteObject(hDC);
       // EndPaint(hWnd, &ps);
        InvalidateRect(hWnd, NULL, TRUE);
        //hDC = mem_dc;
        //HBITMAP mem_bitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
        //BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, mem_dc, 0, 0, SRCCOPY);
        ////SaveDC(mem_dc);
        ////DeleteObject(hDC);
        ////RestoreDC(mem_dc, 1);
        //SelectObject(mem_dc, mem_bitmap);
        //SetConsoleActiveScreenBuffer(mem_dc);


        //HBITMAP mem_bitmap = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top,1,24, NULL);
        //HBITMAP mold_bitmap = (HBITMAP)SelectObject(mem_dc, mem_bitmap);

        //SelectObject(mem_dc, mem_bitmap);
        //DeleteObject(mem_bitmap);
        //DeleteDC(mem_dc);
        //SelectObject(mem_dc, mem_bitmap);
        SetConsoleActiveScreenBuffer(mem_dc);
        //EndPaint(hwnd, &ps);
        //DeleteObject(mem_bitmap);
        //DeleteObject(mem_dc);
        //ReleaseDC(hwnd, hdc);

        //(*l).move(tetris::step);
        //check = check_move(l, array);
        //frame(ln, array);
        //(*l).print();
        //EndPaint(hWnd, &ps);

        //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        Sleep(tetris::speed * tetris::k_speed);
        //hDC = mem_dc;
        //clear(array);
        DeleteObject(&rect);
    };
    tetris::k_speed = 1;
    //DeleteObject(hWnd);
}
////////////////////////////////////////////////////////////
void doing(square_one* l, short array[][20]) {
    while (check_move(l, array)) {
        if (_kbhit()) {
            switch (_getch()) {
            case 75: {//vlevo
                if ((l->get_x_sqr(1) > tetris::left_frame + 2 && l->get_x_sqr(2) > tetris::left_frame + 2 && l->get_x_sqr(3) > tetris::left_frame + 2 && l->get_x_sqr(4) > tetris::left_frame + 2)
                    && (array[l->get_position_y(1)][l->get_position_x(1) - 1] == 0 && array[l->get_position_y(2)][l->get_position_x(2) - 1] == 0
                        && array[l->get_position_y(3)][l->get_position_x(3) - 1] == 0 && array[l->get_position_y(4)][l->get_position_x(4) - 1] == 0))
                    (*l).sideways(-22);
                break; }
            case 77: {//vpravo
                if ((l->get_x_sqr(1) < tetris::right_frame - 22 && l->get_x_sqr(2) < tetris::right_frame - 22 && l->get_x_sqr(3) < tetris::right_frame - 22 && l->get_x_sqr(4) < tetris::right_frame - 22)
                    && (array[l->get_position_y(1)][l->get_position_x(1) + 1] == 0 && array[l->get_position_y(2)][l->get_position_x(2) + 1] == 0
                        && array[l->get_position_y(3)][l->get_position_x(3) + 1] == 0 && array[l->get_position_y(4)][l->get_position_x(4) + 1] == 0))
                    (*l).sideways(22);
                break; }
            case 32: {//turn
                cin.clear();
                (*l).turn();
                if ((l->get_x_sqr(1) < tetris::left_frame || l->get_x_sqr(2) < tetris::left_frame || l->get_x_sqr(3) < tetris::left_frame || l->get_x_sqr(4) < tetris::left_frame)) {
                    (*l).sideways(22);
                    break;
                }
                if (l->get_x_sqr(1) > tetris::right_frame - 20 || l->get_x_sqr(2) > tetris::right_frame - 20 || l->get_x_sqr(3) > tetris::right_frame - 20 || l->get_x_sqr(4) > tetris::right_frame - 20) {
                    (*l).sideways(-22);
                    if (typeid(*l).name() == typeid(I_elem).name())
                        (*l).sideways(-22);
                    break;
                }
                break;
            }
            case 13: {//put down
                tetris::k_speed = 0;
                break;
            }
            case 9: {//pause
                //SuspendThread(tr);
                break;
            }
            case 27: {//exit
                tetris::play = false;
                break;
            }
            default: break;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////
void fill_array(short array[][20], square_one* l) {
    (*l).move(-1 * tetris::step);
    if (typeid(*l).name() == typeid(O_elem).name()) {
        array[l->get_position_y(1)][l->get_position_x(1)] = O;
        array[l->get_position_y(2)][l->get_position_x(2)] = O;
        array[l->get_position_y(3)][l->get_position_x(3)] = O;
        array[l->get_position_y(4)][l->get_position_x(4)] = O;
    }
    else
        if (typeid(*l).name() == typeid(T_elem).name()) {
            array[l->get_position_y(1)][l->get_position_x(1)] = T;
            array[l->get_position_y(2)][l->get_position_x(2)] = T;
            array[l->get_position_y(3)][l->get_position_x(3)] = T;
            array[l->get_position_y(4)][l->get_position_x(4)] = T;
        }
        else
            if (typeid(*l).name() == typeid(L_elem).name()) {
                array[l->get_position_y(1)][l->get_position_x(1)] = L;
                array[l->get_position_y(2)][l->get_position_x(2)] = L;
                array[l->get_position_y(3)][l->get_position_x(3)] = L;
                array[l->get_position_y(4)][l->get_position_x(4)] = L;
            }
            else
                if (typeid(*l).name() == typeid(J_elem).name()) {
                    array[l->get_position_y(1)][l->get_position_x(1)] = J;
                    array[l->get_position_y(2)][l->get_position_x(2)] = J;
                    array[l->get_position_y(3)][l->get_position_x(3)] = J;
                    array[l->get_position_y(4)][l->get_position_x(4)] = J;
                }
                else
                    if (typeid(*l).name() == typeid(S_elem).name()) {
                        array[l->get_position_y(1)][l->get_position_x(1)] = S;
                        array[l->get_position_y(2)][l->get_position_x(2)] = S;
                        array[l->get_position_y(3)][l->get_position_x(3)] = S;
                        array[l->get_position_y(4)][l->get_position_x(4)] = S;
                    }
                    else
                        if (typeid(*l).name() == typeid(Z_elem).name()) {
                            array[l->get_position_y(1)][l->get_position_x(1)] = Z;
                            array[l->get_position_y(2)][l->get_position_x(2)] = Z;
                            array[l->get_position_y(3)][l->get_position_x(3)] = Z;
                            array[l->get_position_y(4)][l->get_position_x(4)] = Z;
                        }
                        else
                            if (typeid(*l).name() == typeid(I_elem).name()) {
                                array[l->get_position_y(1)][l->get_position_x(1)] = I;
                                array[l->get_position_y(2)][l->get_position_x(2)] = I;
                                array[l->get_position_y(3)][l->get_position_x(3)] = I;
                                array[l->get_position_y(4)][l->get_position_x(4)] = I;
                            }
}
//////////////////////////////////////////////////////////////////////////////
void delete_line(short array[][20]) {
    int k = 0;
    for (int i{ 0 }; i < 40; ++i) {
        for (int j{ 0 }; j < 20; ++j)
            if (array[i][j] != 0)
                k++;
        if (k == 20) {
            for (int ii{ 39 }; ii > 0; --ii)
                for (int jj{ 0 }; jj < 20; ++jj)
                    array[ii][jj] = array[ii - 1][jj];
            for (int ii{ 0 }; ii < 20; ++ii)
                array[0][ii] = 0;
            tetris::score += 1000 / tetris::speed;
            tetris::speed *= 0.97;
        }
        k = 0;
    }
}
/////////////////////////////////////////////////////////////////////

square_one* get_elem(int num) {
    switch (num) {
    case 1: {
        return new O_elem;
        break;
    }
    case 2: {
        return new T_elem;
        break;
    }
    case 3: {
        return new L_elem;
        break;
    }
    case 4: {
        return new J_elem;
        break;
    }
    case 5: {
        return new Z_elem;
        break;
    }
    case 6: {
        return new S_elem;
        break;
    }
    case 7: {
        return new I_elem;
        break;
    }
    default: break;
    }
}
///////////////////////////////////////////////////////////////////////////////////////
bool check_loss(short array[][20]) {
    for (int i{ 0 }; i < 20; ++i) {
        if (array[0][i] != 0)
            return true;
    }
    return false;
}
/////////////////////////////////////////////////////////////////////
void intro(HWND hWnd) {
    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);
    COORD coord;
    coord.X = screen.dwMaximumWindowSize.X / 2 + 12;
    coord.Y = screen.dwMaximumWindowSize.Y / 2;
    gotoXY(coord.X, coord.Y);
    cout << "T E T R I S";
    deletecursor();
    _getch();
    system("cls");
    cout << "RULES:\n\tESC - выйти из игры\n\t-> / <- - управление фигурками\n\tSPACE - переворот фигуры\n\tEnter - опустить фигуру вниз\n\tTab - пауза\n\n\n\n\n";
    system("pause");
    system("cls");
}
//////////////////////////////////////////////////////////////////////////////
const int menu_count = 3;
void print_menu(int pos = 1) {
    static string menu[menu_count] = { "Начать игру\n",
                                       "Показать лист чемпионов\n",
                                       "Покинуть игру\n" };
    for (int i{ 0 }; i < menu_count; ++i)
        if (pos - 1 == i)
            cout << "\033[3;47;30m" << menu[i] << "\033[0m";
        else
            cout << menu[i];
}
////////////////////////////////////////////////////////////////////////
void read_list_champions(string p) {
    system("cls");
    fstream f(p, ios::in);
    if (!f.is_open())
        cout << "Error opened file " << p << endl;
    else
        cout << f.rdbuf() << endl;
    f.close();
    cout << "\n\n\n\n\n";
    system("pause");
    system("cls");
}
////////////////////////////////////////////////////////////////////////////////
void write_list_champions(string p) {
    fstream f(p, ios::in);
    multimap<int, string> spisok;

    if (!f.is_open())
        cout << "Error opened file " << p << endl;
    else
        while (!f.eof()) {
            string str{};
            string strnum{};
            int num{};
            char ch{ ' ' };

            while (ch != '\n') {
                ch = f.get();
                if (f.eof())
                    break;
                if ((ch < 48 || ch > 57) && ch != 32 && ch != ':' && ch != '\n')
                    str += ch;
                else
                    if (ch >= 48 && ch <= 57)
                        strnum += ch;
            };
            if (!f.eof() || ch == '\n') {
                num = stoi(strnum);
                spisok.insert(make_pair(num, str));
            }
        }
    f.close();

    spisok.insert(make_pair(tetris::score, tetris::name));

    fstream f1(p, ios_base::out | ios_base::trunc);
    if (!f1.is_open())
        cout << "Error opened file " << p << endl;
    else {
        multimap<int, string>::reverse_iterator it = spisok.rbegin();
        for (int i = 0; i < 9 && it != spisok.rend(); ++it, ++i) {
            f1 << it->second << ": " << it->first << "\n";
        }
    }
    f1.close();
}
/////////////////////////////////////////////////////////////////////////////
void play(HWND hWnd, HDC hDC) {
    const short col{ 20 };
    const short row{ 40 };
    short array[row][col]{};
    O_elem o;
    square_one* one = &o;
    square_one* two = &o;

    int first = rand() % 7 + 1;
    tetris::play = true;
    while (!check_loss(array) && tetris::play) {
        //HANDLE tr = GetCurrentThread();

        int second = rand() % 7 + 1;
        one = get_elem(first);
        two = get_elem(second);

        thread th(go_down, hWnd, hDC, one, two, array);
        thread th2(doing, one, array);

        th2.detach();
        th.join();

        first = second;
        fill_array(array, one);
        delete_line(array);
        system("cls");
        delete one;
        delete two;
    }
    if (check_loss(array)) {
        gotoXY(15, 20);
        cout << "===ВЫ ПРОИГРАЛИ!!!===";
        _getch();
    }

    //DeleteObject(hDC);
    //DeleteObject(hWnd);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));
    string path = "D:\\LeaderListTetris.txt";

    bool exit = true;
    int pos = 1;

    HWND hWnd = GetConsoleWindow();
    intro(hWnd);

    while (exit) {
        print_menu(pos);
        switch (_getch()) {
        case 72: {
            if (pos > 1)
                pos -= 1;
            break;
        }
        case 80: {
            if (pos < menu_count)
                pos += 1;
            break;
        }
        case 13: {
            switch (pos) {
            case 1: {
                //PAINTSTRUCT ps;
                //HDC hDC = BeginPaint(hWnd, &ps);
                HDC hDC = GetDC(hWnd);
                system("cls");
                cout << "Введите свое имя: ";
                cin >> tetris::name;
                system("cls");
                play(hWnd, hDC);
                DeleteObject(hDC);
                //DeleteObject(hWnd);
                ReleaseDC(hWnd, hDC);
                //EndPaint(hWnd, &ps);
                write_list_champions(path);
                break;
            }
            case 2: {
                read_list_champions(path);
                pos = 1;
                break;
            }
            case 3: {
                exit = false;
                break;
            }
            default: break;
            }
            break;
        }
        default: break;
        }
        system("cls");
    }

    DeleteObject(hWnd);
    return 0;
}