// sigslot examples (adapted from Sarah Thompson's documentation)

// g++ examples.cpp -o examples -std=c++11 -pedantic -Wall

#include "sigslot.h"
#include <iostream>
#include <string>

// helper print function
void PRINT()
{
   std::cout << std::endl;
}
template <typename T, typename ... P>
void PRINT(T t, P ... p)
{
   std::cout << t;
   PRINT(p...);
}

class Switch
{
public:
    sigslot::signal0<> clicked;
};

class Light : public sigslot::has_slots<>
{
public:
    Light()
    {
        id = ++UID;
    }

    void toggleState()
    {
        isLit = !isLit;
    }
    
    void turnOn()
    {
        isLit = true;
    }
    
    void turnOff()
    {
        isLit = false;
    }
    
    void print()
    {
        PRINT("The light #", id, " is ", (isLit ? "on" : "off"));
    }
   
private:
    bool isLit = false;
    int id;
    static int UID;
};

int Light::UID = 0;

void testLightSwitch()
{
    Switch s1, s2, allOn, allOff;
    Light l1, l2;
    
    s1.clicked.connect(&l1, &Light::toggleState);
    s2.clicked.connect(&l2, &Light::toggleState);
    allOn.clicked.connect(&l1, &Light::turnOn);
    allOn.clicked.connect(&l2, &Light::turnOn);
    allOff.clicked.connect(&l1, &Light::turnOff);
    allOff.clicked.connect(&l2, &Light::turnOff);

    PRINT("TEST: LightSwitch");
    s1.clicked();
    l1.print();
    s2.clicked();
    l2.print();
    s1.clicked();
    l1.print();
    allOn.clicked();
    l1.print();
    l2.print();
    allOff.clicked();
    l1.print();
    l2.print();
}

class Window
{
public:
    enum State { MINIMIZED, NORMAL, MAXIMIZED };
    sigslot::signal1<State> stateChanged;
    sigslot::signal2<int, int> movedTo;
    sigslot::signal2<int, int> resized;
};

class Control : public sigslot::has_slots<>
{
public:
    void onStateChanged(Window::State ws)
    {
        std::string state;
        switch (ws) {
            case Window::State::MINIMIZED:
                state = "minimized";
                break;
            case Window::State::NORMAL:
                state = "normal";
                break;
            case Window::State::MAXIMIZED:
                state = "maximized";
                break;
        }
        PRINT("Window state changed to ", state);
    }
    
    void onMovedTo(int x, int y)
    {
        PRINT("Window moved to (", x, ", ", y, ")");
    }
    
    void onResized(int w, int h)
    {
        PRINT("Window resized to ", w, "x", h);
    }
};

void testWindowControl()
{
    Window w;
    Control c;
    
    w.stateChanged.connect(&c, &Control::onStateChanged);
    w.movedTo.connect(&c, &Control::onMovedTo);
    w.resized.connect(&c, &Control::onResized);

    PRINT("\nTEST: WindowControl");
    w.stateChanged(Window::State::MAXIMIZED);
    w.movedTo(42, 47);
    w.resized(800, 600);
}

int main()
{
    testLightSwitch();
    testWindowControl();

    return 0;
}
