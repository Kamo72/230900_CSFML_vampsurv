#include "SFML_Custom.h"
#include "SFML_Data.h"
#include "Game.h"

using namespace std;
using namespace sf;

using namespace SFML_Custom;
using namespace SFML_Data;
using namespace Game;

int main()
{
    sf::VideoMode fullscreenMode = VideoMode::getDesktopMode(); // ���ϴ� �ػ󵵷� ����
    screenSize = Vector2i(fullscreenMode.width, fullscreenMode.height);
    RenderWindow window(fullscreenMode, "Kamo's SFML Project!", sf::Style::Fullscreen);

    nowWindow = &window;
    ProcessInitiate();  //���α׷��� ��ó���� ���

    //Loop State
    while (nowWindow->isOpen())
    {
        //Initiate in Loop
        Event event;
        while (nowWindow->pollEvent(event))
        {
            if (event.type == Event::Closed) { nowWindow->close(); }
            im->TotalProcess(event);   //�Է� ������ ���μ���
        }
        FrameTimeRefresh();

        //Do in Loop
        TotalSystem();      //���α׷��� ����ó���� ���
    }

    //End State
    return 0;
}